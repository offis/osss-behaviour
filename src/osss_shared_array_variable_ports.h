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

//
// This is based on the sc_in<...> port implementation of SystemC
//

#ifndef OSSS_SHARED_ARRAY_VARIABLE_PORTS_INCLUDED
#define OSSS_SHARED_ARRAY_VARIABLE_PORTS_INCLUDED

#include "osss_shared_variable_ifs.h"

namespace osss {

// ----------------------------------------------------------------------------
//  CLASS : osss_in<osss_array<T, Size> >
//
//  The osss_shared_variable<osss_array<T, Size> > input port class.
// ----------------------------------------------------------------------------

template <typename T, unsigned int Size>
class osss_in<osss_array<T, Size> > : 
  public sc_core::sc_port< osss_shared_array_in_if<T, Size>, 
                           1, SC_ONE_OR_MORE_BOUND >
{
public:

    // typedefs

    typedef T                                           data_type;
    typedef osss_array<data_type, Size>                 array_type;

    typedef osss_shared_array_in_if<data_type, Size>    if_type;
    typedef sc_core::sc_port<if_type, 1, 
                             SC_ONE_OR_MORE_BOUND>      base_type;
    typedef osss_in<array_type>                         this_type;

    typedef if_type                                     in_if_type;
    typedef base_type                                   in_port_type;
    typedef osss_shared_array_inout_if<data_type, Size> inout_if_type;
    typedef sc_core::sc_port<inout_if_type, 1, 
                             SC_ONE_OR_MORE_BOUND>      inout_port_type;

    typedef typename if_type::const_reference           const_reference;
    typedef typename if_type::size_type                 size_type;

public:

    // constructors

    osss_in()
	: base_type(), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_in( const char* name_ )
	: base_type( name_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_in( const in_if_type& interface_ )
        : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 ),
	  m_change_finder_p(0)
        {}

    osss_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_in( in_port_type& parent_ )
	: base_type( parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_in( inout_port_type& parent_ )
	: base_type(), m_traces( 0 ),
	  m_change_finder_p(0)
	{ sc_core::sc_port_base::bind( parent_ ); }

    osss_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{ sc_core::sc_port_base::bind( parent_ ); }

    osss_in( this_type& parent_ )
	: base_type( parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}


    // destructor

    virtual ~osss_in()
        {
	    remove_traces();
	    if ( m_change_finder_p ) delete m_change_finder_p;
	}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_core::sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_core::sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_core::sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_core::sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_core::sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_core::sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const array_type& read() const
	{ return (*this)->read(); }

    operator const array_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // (other) event finder method(s)

    sc_core::sc_event_finder& value_changed() const
    {
        if ( !m_change_finder_p )
	{
	    m_change_finder_p = new sc_core::sc_event_finder_t<in_if_type>(
	        *this, &in_if_type::value_changed_event );
	}
	return *m_change_finder_p;
    }

    // returns the size of the array
    virtual size_type size() const 
      { return (*this)->size(); }

    // reads value at the specified array index
    virtual const_reference operator[]( size_type index ) const
      { return (*this)->get_read_ref( index ); }

    virtual const_reference get_read_ref( size_type index ) const
      { return (*this)->get_read_ref( index ); }

    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "osss_in"; }


    void add_trace( sc_core::sc_trace_file*, 
                    const std::string& ) const;

    // called by sc_trace
    void add_trace_internal( sc_core::sc_trace_file*, 
                             const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_core::sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_core::sc_interface& );
    virtual int vbind( sc_core::sc_port_base& );

private:
  mutable sc_core::sc_event_finder* m_change_finder_p;

private:

    // disabled
    osss_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static array_type dummy;
#endif
};

template<typename T, unsigned int Size>
::std::ostream& operator << ( ::std::ostream& os, 
                              const osss_in<osss_array<T, Size> >& a )
{
    return os << a->read();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// called when elaboration is done

template <typename T, unsigned int Size>
inline
void
osss_in<osss_array<T, Size> >::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_core::sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->read(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <typename T, unsigned int Size>
inline
void
osss_in<osss_array<T, Size> >::add_trace_internal( sc_core::sc_trace_file* tf_, 
                                                   const std::string& name_ ) 
const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_core::sc_trace_params_vec;
	}
	m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
    }
}

template <typename T, unsigned int Size>
inline
void
osss_in<osss_array<T, Size> >::add_trace( sc_core::sc_trace_file* tf_, 
                                          const std::string& name_ ) 
const
{
    sc_core::sc_deprecated_add_trace();
    add_trace_internal(tf_, name_);
}

template <typename T, unsigned int Size>
inline
void
osss_in<osss_array<T, Size> >::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = (int)m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

template <typename T, unsigned int Size>
inline
int
osss_in<osss_array<T, Size> >::vbind( sc_core::sc_interface& interface_ )
{
    return sc_core::sc_port_b<if_type>::vbind( interface_ );
}

template <typename T, unsigned int Size>
inline
int
osss_in<osss_array<T, Size> >::vbind( sc_core::sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_core::sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_core::sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : osss_inout<osss_array<T, Size> >
//
//  The osss_shared_variable<osss_array<T, Size> > input/output port class.
// ----------------------------------------------------------------------------

template <typename T, unsigned int Size>
class osss_inout<osss_array<T, Size> > : 
public sc_core::sc_port<osss_shared_array_inout_if<T, Size>, 
                        1, SC_ONE_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef T                                           data_type;
    typedef osss_array<data_type, Size>                 array_type;

    typedef osss_shared_array_inout_if<data_type, Size> if_type;
    typedef sc_core::sc_port<if_type, 1, 
                             SC_ONE_OR_MORE_BOUND>      base_type;
    typedef osss_inout<array_type>                      this_type;

    typedef osss_shared_array_in_if<data_type, Size>    in_if_type;
    typedef sc_core::sc_port<in_if_type, 1, 
                             SC_ONE_OR_MORE_BOUND>      in_port_type;
    typedef if_type                                     inout_if_type;
    typedef base_type                                   inout_port_type;

    typedef typename if_type::const_reference           const_reference;
    typedef typename if_type::reference                 reference;
    typedef typename if_type::size_type                 size_type;

public:

    // constructors

    osss_inout()
	: base_type(), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_inout( const char* name_ )
	: base_type( name_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    explicit osss_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}

    osss_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 ),
	  m_change_finder_p(0)
	{}


    // destructor

    virtual ~osss_inout();


    // interface access shortcut methods

    // get the default event

    const sc_core::sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_core::sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const array_type& read() const
	{ return (*this)->read(); }

    operator const array_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    void write( const array_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const array_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    // returns the size of the array
    virtual size_type size() const
      { return (*this)->size(); }

    // reads value at the specified array index
    virtual const_reference operator[]( size_type index ) const
      { return (*this)->get_read_ref( index ); }

    virtual const_reference get_read_ref( size_type index ) const
      { return (*this)->get_read_ref( index ); }

     // writes value at the specified array index
    virtual reference operator[]( size_type index )
      { return (*this)->get_write_ref( index ); }

    virtual reference get_write_ref( size_type index )
      { return (*this)->get_write_ref( index ); }

    // set initial value (can also be called when port is not bound yet)

    void initialize( const array_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_core::sc_event_finder& value_changed() const
    {
        if ( !m_change_finder_p )
	{
	    m_change_finder_p = new sc_core::sc_event_finder_t<in_if_type>(
	        *this, &in_if_type::value_changed_event );
	}
	return *m_change_finder_p;
    }

    virtual const char* kind() const
        { return "osss_inout"; }

protected:

    array_type* m_init_val;

public:

    // called by sc_trace
    void add_trace_internal( sc_core::sc_trace_file*, 
                             const std::string& ) const;

    void add_trace( sc_core::sc_trace_file*, 
                    const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_core::sc_trace_params_vec* m_traces;

private:
    mutable sc_core::sc_event_finder* m_change_finder_p;

private:

    // disabled
    osss_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static array_type dummy;
#endif
};

template<typename T, unsigned int Size>
::std::ostream& operator << ( ::std::ostream& os, 
                              const osss_inout<osss_array<T, Size> >& a )
{
    return os << a->read();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <typename T, unsigned int Size>
inline
osss_inout<osss_array<T, Size> >::~osss_inout()
{
    if ( m_change_finder_p ) delete m_change_finder_p;
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

template <typename T, unsigned int Size>
inline
void
osss_inout<osss_array<T, Size> >::initialize( const array_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new array_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

template <typename T, unsigned int Size>
inline
void
osss_inout<osss_array<T, Size> >::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_core::sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->read(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <typename T, unsigned int Size>
inline
void
osss_inout<osss_array<T, Size> >::add_trace_internal( sc_core::sc_trace_file* tf_, 
                                                      const std::string& name_) 
const
{
    if( tf_ != 0 ) {
	    if( m_traces == 0 ) {
	        m_traces = new sc_core::sc_trace_params_vec;
	    }
	    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
    }
}

template <typename T, unsigned int Size>
inline
void
osss_inout<osss_array<T, Size> >::add_trace( sc_core::sc_trace_file* tf_, 
                                             const std::string& name_) const
{
    sc_core::sc_deprecated_add_trace();
    add_trace_internal(tf_, name_);
}

template <typename T, unsigned int Size>
inline
void
osss_inout<osss_array<T, Size> >::remove_traces() const
{
    if( m_traces != 0 ) {
      for( int i = m_traces->size() - 1; i >= 0; -- i ) {
        delete (*m_traces)[i];
      }
      delete m_traces;
      m_traces = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : osss_out<osss_array<T, Size> >
//
//  The osss_shared_variable<osss_array<T, Size> > output port class.
// ----------------------------------------------------------------------------

// osss_out can also read from its port, hence no difference with osss_inout.
// For debugging reasons, a class is provided instead of a define.

template <typename T, unsigned int Size>
class osss_out<osss_array<T, Size> > : public osss_inout<osss_array<T, Size> >
{
public:

    // typedefs

    typedef T                                   data_type;
    typedef osss_array<data_type, Size>         array_type;

    typedef osss_out<array_type>                this_type;
    typedef osss_inout<array_type>              base_type;

    typedef typename base_type::in_if_type      in_if_type;
    typedef typename base_type::in_port_type    in_port_type;
    typedef typename base_type::inout_if_type   inout_if_type;
    typedef typename base_type::inout_port_type inout_port_type;

public:

    // constructors

    osss_out()
	: base_type()
	{}

    explicit osss_out( const char* name_ )
	: base_type( name_ )
	{}

    explicit osss_out( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    osss_out( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit osss_out( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    osss_out( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    osss_out( this_type& parent_ )
	: base_type( parent_ )
	{}

    osss_out( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~osss_out()
	{}


    // write the new value

    this_type& operator = ( const array_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    virtual const char* kind() const
        { return "osss_out"; }

private:

    // disabled
    osss_out( const this_type& );
};

} // namespace osss

#endif
