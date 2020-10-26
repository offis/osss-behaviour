/* -----------------------------------------------------------------------
 * Copyright (c) 2005-2008  OFFIS Institute for Information Technology,
 *                          Oldenburg, Germany
 * All rights reserved.
 *
 * This file is directly or indirectly part of the OSSS simulation
 * library, a library for synthesisable system level models in
 * SystemC.
 *
 * Created for the projects:
 *   - ICODES  (1)
 *   - ANDRES  (2)
 *
 * 1) http://icodes.offis.de/
 * 2) http://andres.offis.de/
 *
 * A list of authors and contributors can be found in the accompanying
 * AUTHORS file.  For detailed copyright information, please refer
 * to the COPYING file.
 *
 * -----------------------------------------------------------------------
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * Contact information:
 *  OFFIS
 *  Institute for Information Technology
 *    Escherweg 2
 *    D-26121 Oldenburg
 *    Germany
 *  www  : http://offis.de/
 *  phone: +49 (441) 9722-0
 *  fax  : +49 (441) 9722-102
 * -----------------------------------------------------------------------
 */

#ifndef OSSS_SHARED_VARIABLE_BASE_INCLUDED
#define OSSS_SHARED_VARIABLE_BASE_INCLUDED

#include "osss_shared_variable_ifs.h"
#include "osss_event_list.h"

namespace osss {
namespace osssi {

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_b<T>
//
// ----------------------------------------------------------------------------

template<typename T>
class osss_shared_variable_b
{
 public:
  osss_shared_variable_b() {}

  virtual ~osss_shared_variable_b() {}

  virtual void assign_new_value( const T& ) = 0;

 private:
  osss_shared_variable_b( const osss_shared_variable_b& );
};

//
// partial specialisation for osss_arrays
//
template<typename T, unsigned int Size>
class osss_shared_variable_b<osss_array<T, Size> >
{
 public:
  osss_shared_variable_b() {}

  virtual ~osss_shared_variable_b() {}

  virtual void assign_new_value( typename osss_array<T, Size>::size_type,
                                 const T& ) = 0;
};

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_base<T, depth, multiple_writer_check>
//
// ----------------------------------------------------------------------------

template <class T, unsigned int depth = 0, 
          bool multiple_writer_check = false>
class osss_shared_variable_base : 
  public osss_shared_variable_b<T>,
  public osss_shared_variable_inout_if<T>,
  public sc_core::sc_module
{
  public: // constructor and destructor:

    SC_HAS_PROCESS(osss_shared_variable_base);

    osss_shared_variable_base(const sc_core::sc_module_name& name)
        : sc_core::sc_module( name ),
	  m_change_event_p( 0 ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
          m_writer( 0 ),
          m_pipeline_behaviour( 0 )
	{
          this->init();
          SC_THREAD(shift_proc);
        }


    virtual ~osss_shared_variable_base()
	{
	    if ( !m_change_event_p ) delete m_change_event_p;
	}

  protected:

    void init()
       {
         // array initialisation
         for(unsigned int i=0; i<depth+1; i++)
         {
           T tmp = T();
           m_queue[i] = tmp;
         }
       }

    void shift_proc()
      {
        while(true)
        {
          // wait for completion of all pipeline stages
          if(m_pipeline_behaviour->behaviour_list().size() == 1)
          wait(dynamic_cast<osss_pipeline_stage*>(
                 m_pipeline_behaviour->behaviour_list()[0])->complete_event());
          else
          {
            osssi::osss_event_and_list 
              complete_event_list(
                dynamic_cast<osss_pipeline_stage*>(
                  m_pipeline_behaviour->behaviour_list()[0])->complete_event());
            for(unsigned int i=1; 
                i<m_pipeline_behaviour->behaviour_list().size(); 
                ++i)
              complete_event_list.add_event(
                dynamic_cast<osss_pipeline_stage*>(
                  m_pipeline_behaviour->behaviour_list()[i])->complete_event());

            wait(complete_event_list);
            
            // perform shift/update operation
            this->shift();
          }
        }
      }

    void shift()
      {
        for(unsigned int i=depth; i>=1; i--)
          m_queue[i] = m_queue[i-1];
      }

    virtual void end_of_elaboration()
      {
        sc_core::sc_object* parent_obj = this->get_parent_object();
        osss_behaviour* beh = dynamic_cast<osss_behaviour*>(parent_obj);
        if(beh && beh->node_type() == OSSS_PIPE_BEHAVIOUR)
        {
          osssi::osss_pipeline_behaviour* pipe_beh = 
            dynamic_cast<osssi::osss_pipeline_behaviour*>(beh->m_composite_beh);
          if(pipe_beh)
          {
            m_pipeline_behaviour = pipe_beh;
          }
        }
        else
        {
          std::cerr << "Error: parent of piped shared variable is not a "
                    << "pipeline behaviour!" << std::endl;
          exit(EXIT_FAILURE);
        }
      }

  public:

    // interface methods

    virtual void register_port( sc_core::sc_port_base&, const char* );


    // get the default event
    virtual const sc_core::sc_event& default_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_core::sc_event;
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_core::sc_event& value_changed_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_core::sc_event; 
	    return *m_change_event_p; 
	}


    // read the current value
    virtual const T& read() const
	{ return m_queue[depth]; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { sc_core::sc_deprecated_get_data_ref(); return m_queue[depth]; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta); }

    // write the new value
    virtual void write( const T& );

    void initialize( const T& );

    // other methods

    operator const T& () const
	{ return read(); }


    osss_shared_variable_base<T, depth, multiple_writer_check>& 
    operator = ( const T& a )
	{ write( a ); return *this; }

    osss_shared_variable_base<T, depth, multiple_writer_check>& 
    operator = ( 
      const osss_shared_variable_base<T, depth, multiple_writer_check>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }


    void trace( sc_core::sc_trace_file* tf ) const
	{ 
	    sc_core::sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_core::sc_trace( tf, read(), name() ); 
#	    endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "osss_shared_variable_base"; }

protected:

    virtual void assign_new_value( const T& value_ );
    void update();

protected:

    mutable sc_core::sc_event*      m_change_event_p;
    sc_dt::uint64                   m_delta;   // delta of last event
    T                               m_new_val;
    sc_core::sc_port_base*          m_output; // used for static design rule checking
    sc_core::sc_object*             m_writer; // used for dynamic design rule checking

    osssi::osss_pipeline_behaviour* m_pipeline_behaviour;
    T                               m_queue[depth+1];

private:

    // disabled
    osss_shared_variable_base( 
      const osss_shared_variable_base<T, depth, multiple_writer_check>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
register_port( sc_core::sc_port_base& port_, const char* if_typename_ )
{
  if ( sc_core::sc_get_curr_simcontext()->write_check() )
  {
    std::string nm( if_typename_ );
    if( nm == typeid( osss_shared_variable_inout_if<T> ).name() ) {
      // an out or inout port; only one can be connected
      if( m_output != 0) {
        sc_core::sc_signal_invalid_writer( this, m_output, &port_, true );
      }
      m_output = &port_;
    }
  }
}


// write the new value

template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
write( const T& value_ )
{
    //
    // perform multiple writer check
    //
    //
  if (multiple_writer_check)
  {
    sc_core::sc_object* writer = 
      sc_core::sc_get_curr_simcontext()->get_current_writer();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
      sc_core::sc_signal_invalid_writer( this, m_writer, writer, true );
    }
  }

  assign_new_value(value_);
}

template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
initialize( const T& value_ )
{
  m_queue[0] = value_;
}

template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
assign_new_value( const T& value_ )
{
  m_new_val = value_;
  if( !( m_new_val == m_queue[0] ) ) {
    update();
  }
}

template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
print( ::std::ostream& os ) const
{
    os << m_queue[depth];
}

template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
dump( ::std::ostream& os ) const
{
  os << "         name = " << name() << ::std::endl;
  os << "        depth = " << depth << ::std::endl;
  os << " writer check = " 
     << (multiple_writer_check ? "true" : "false") << ::std::endl;
  os << "        value = ";
  for(unsigned int i=0; i<depth+1; i++)
    os << m_queue[i] << " ";
  os << std::endl;
  os << "    new value = " << m_new_val << ::std::endl;
}


template <class T, unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, depth, multiple_writer_check>::
update()
{
  if( !( m_new_val ==  m_queue[0] ) ) {
    m_queue[0] = m_new_val;
    if ( m_change_event_p ) m_change_event_p->notify(sc_core::SC_ZERO_TIME);
    m_delta = sc_core::sc_delta_count();
  }
}

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_base<T, multiple_writer_check>
//
//  Partial specialisation of
//  osss_shared_variable_base<T, depth, multiple_writer_check>
// ----------------------------------------------------------------------------

template <class T, bool multiple_writer_check>
class osss_shared_variable_base<T, 0, multiple_writer_check> : 
  public osss_shared_variable_b<T>,
  public osss_shared_variable_inout_if<T>,
  public sc_core::sc_prim_channel
{
  public: // constructors and destructor:

    osss_shared_variable_base()
	: sc_core::sc_prim_channel( 
          sc_core::sc_gen_unique_name( "shared_variable" ) ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
	{}

    explicit osss_shared_variable_base( const char* name_ )
	: sc_core::sc_prim_channel( name_ ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
        {}


    virtual ~osss_shared_variable_base()
	{
	    if ( !m_change_event_p ) delete m_change_event_p;
	}


    // interface methods

    virtual void register_port( sc_core::sc_port_base&, const char* );


    // get the default event
    virtual const sc_core::sc_event& default_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_core::sc_event;
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_core::sc_event& value_changed_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_core::sc_event; 
	    return *m_change_event_p; 
	}


    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { sc_core::sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta); }

    // write the new value
    virtual void write( const T& );

    void initialize( const T& );

    // other methods

    operator const T& () const
	{ return read(); }


    osss_shared_variable_base<T, 0, multiple_writer_check>& 
    operator = ( const T& a )
	{ write( a ); return *this; }

    osss_shared_variable_base<T, 0, multiple_writer_check>& 
    operator = ( const osss_shared_variable_base<T, 0, multiple_writer_check>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }


    void trace( sc_core::sc_trace_file* tf ) const
	{ 
	    sc_core::sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_core::sc_trace( tf, read(), name() ); 
#	    endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "osss_shared_variable_base"; }

protected:

    virtual void assign_new_value( const T& value_ );
    virtual void update();

protected:

    mutable sc_core::sc_event*  m_change_event_p;
    T                           m_cur_val;
    sc_dt::uint64               m_delta;   // delta of last event
    T                           m_new_val;
    sc_core::sc_port_base*      m_output; // used for static design rule checking
    sc_core::sc_object*         m_writer; // used for dynamic design rule checking

private:

    // disabled
    osss_shared_variable_base( 
      const osss_shared_variable_base<T, 0, multiple_writer_check>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
register_port( sc_core::sc_port_base& port_, const char* if_typename_ )
{
  if ( sc_core::sc_get_curr_simcontext()->write_check() )
  {
    std::string nm( if_typename_ );
    if( nm == typeid( osss_shared_variable_inout_if<T> ).name() ) {
      // an out or inout port; only one can be connected
      if( m_output != 0) {
        sc_core::sc_signal_invalid_writer( this, m_output, &port_, true);
      }
      m_output = &port_;
    }
  }
}


// write the new value

template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
write( const T& value_ )
{
  //
  // perform multiple writer check
  //
  //
  if (multiple_writer_check)
  {
    sc_core::sc_object* writer = 
      sc_core::sc_get_curr_simcontext()->get_current_writer();
    if( m_writer == 0 ) {
      m_writer = writer;
    } else if( m_writer != writer ) {
      sc_core::sc_signal_invalid_writer( this, m_writer, writer, true );
    }
  }

  assign_new_value(value_);
}

template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
initialize( const T& value_ )
{
  m_cur_val = value_;
}

template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
assign_new_value( const T& value_ )
{
  m_new_val = value_;
  if( !( m_new_val == m_cur_val ) ) {
    update();
  }
}

template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
dump( ::std::ostream& os ) const
{
  os << "         name = " << name() << ::std::endl;
  os << "        depth = 0" << ::std::endl;
  os << " writer check = " 
     << (multiple_writer_check ? "true" : "false") << ::std::endl;
  os << "        value = " << m_cur_val << ::std::endl;
  os << "    new value = " << m_new_val << ::std::endl;
}


template <class T, bool multiple_writer_check>
inline
void
osss_shared_variable_base<T, 0, multiple_writer_check>::
update()
{
  if( !( m_new_val == m_cur_val ) ) {
    m_cur_val = m_new_val;
    if ( m_change_event_p ) m_change_event_p->notify(sc_core::SC_ZERO_TIME);
    m_delta = sc_core::sc_delta_count();
  }
}

} // namespace osssi
} // namespace osss

#endif
