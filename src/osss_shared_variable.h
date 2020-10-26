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
// This is based on the sc_signal implementation of SystemC
//

#ifndef OSSS_SHARED_VARIABLE_INCLUDED
#define OSSS_SHARED_VARIABLE_INCLUDED

#include "osss_shared_variable_base.h"

namespace osss {

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable<T, depth, multiple_writer_check>
//
//  The osss_shared_variable<T, depth, multiple_writer_check> 
//  primitive channel class.
// ----------------------------------------------------------------------------

template <class T, unsigned int depth = 0, 
          bool multiple_writer_check = false>
class osss_shared_variable : 
  public osssi::osss_shared_variable_base<T, depth, multiple_writer_check>
{
  public: // constructor and destructor:

  typedef osssi::osss_shared_variable_base<T, depth, multiple_writer_check>
          base_type;

  osss_shared_variable(sc_core::sc_module_name name = 
                       sc_core::sc_gen_unique_name("shared_variable"))
  : base_type( name )
  {}


  virtual ~osss_shared_variable()
  {}

  virtual const char* kind() const
  { return "osss_shared_variable"; }

private:
    // disabled
    osss_shared_variable( 
      const osss_shared_variable<T, depth, multiple_writer_check>& );
};


// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable<T, multiple_writer_check>
//
//  The osss_shared_variable<T, multiple_writer_check> 
//  primitive channel class. Partial specialisation of
//  osss_shared_variable<T, depth, multiple_writer_check>
// ----------------------------------------------------------------------------

template <class T, bool multiple_writer_check>
class osss_shared_variable<T, 0, multiple_writer_check> : 
  public osssi::osss_shared_variable_base<T, 0, multiple_writer_check>
{
  public: // constructors and destructor:

  typedef osssi::osss_shared_variable_base<T, 0, multiple_writer_check>
    base_type;

  osss_shared_variable()
    : base_type()
    {}

  explicit osss_shared_variable( const char* name_ )
    : base_type( name_ )
    {}


  virtual ~osss_shared_variable()
    {}

  virtual const char* kind() const
    { return "osss_shared_variable"; }

 private:
    // disabled
    osss_shared_variable( 
      const osss_shared_variable<T, 0, multiple_writer_check>& );
};

// ===========================================================================
//
// Implementation for osss_array<T, Size>
//
// ===========================================================================

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_array_accessor<...>
//
//  Used to access elements of an array.
// ----------------------------------------------------------------------------

namespace osssi {

template<typename T, unsigned int Size>
class osss_shared_variable_array_accessor
{
 public:
 
 explicit osss_shared_variable_array_accessor(
    osss_shared_variable_b<osss_array<T, Size> >* sv,
    const typename osss_array<T, Size>::size_type& index) : 
  m_sv(sv),
  m_index(index) 
 {}

 void operator=( const T& value );

 protected:
 osss_shared_variable_b<osss_array<T, Size> >* m_sv;
 const typename osss_array<T, Size>::size_type& m_index;

 private:
 // disabled
 osss_shared_variable_array_accessor();

 osss_shared_variable_array_accessor<T, Size>&
 operator=(const osss_shared_variable_array_accessor<T, Size>& );
};

//
// partial specialisation for osss_array inside osss_array
//
template<typename T_inner, unsigned int Size_inner, unsigned int Size>
  class osss_shared_variable_array_accessor<osss_array<T_inner, Size_inner>, Size >
{
 public:
 
 explicit osss_shared_variable_array_accessor(
    osss_shared_variable_b<osss_array<osss_array<T_inner, Size_inner>, Size> >* sv,
    const typename osss_array<osss_array<T_inner, Size_inner>, Size>::size_type& index) : 
  m_sv(sv),
  m_index(index) 
 {}

// void operator=( const osss_array<T_inner, Size_inner>& array );
 
// osss_shared_variable_array_accessor<T_inner, Size_inner>& operator[]( size_type index );

 protected:
 osss_shared_variable_b<osss_array<osss_array<T_inner, Size_inner>, Size> >* m_sv;
 const typename osss_array<osss_array<T_inner, Size_inner>, Size>::size_type& m_index;

 private:
 // disabled
 osss_shared_variable_array_accessor<osss_array<T_inner, Size_inner>, Size>();

 osss_shared_variable_array_accessor<osss_array<T_inner, Size_inner>, Size>&
 operator=(const osss_shared_variable_array_accessor<osss_array<T_inner, Size_inner>, Size>& );
};

} // namespace osssi

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// pipelined version
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable<osss_array<T, Size>,
//                               depth  multiple_writer_check >
//
//  The osss_shared_variable<osss_array<T, Size>, depth, multiple_writer_check> 
//  primitive channel class. Partial specialisation of
//  osss_shared_variable<T, depth, multiple_writer_check>
// ----------------------------------------------------------------------------

/*
template <typename T, unsigned int Size, 
          template<typename, unsigned int> class wrapper,
          unsigned int depth,
          bool multiple_writer_check>
class osss_shared_variable<wrapper<T, Size>, depth, multiple_writer_check> {};
*/

template <typename T, unsigned int Size,
          unsigned int depth,
          bool multiple_writer_check>
class osss_shared_variable<osss_array<T, Size>, depth, multiple_writer_check> :
  public virtual osss_shared_array_inout_if<T, Size>,
  public virtual osssi::osss_shared_variable_base<osss_array<T, Size>, 
                                                  depth,
                                                  multiple_writer_check>
{
  friend
  class osssi::osss_shared_variable_array_accessor<T, Size>;

  public: // constructors and destructor:
    typedef osssi::osss_shared_variable_base<
      osss_array<T, Size>, depth, multiple_writer_check> base_type;
    
    typedef osss_shared_array_inout_if<T, Size> if_type;
    typedef typename if_type::const_reference   const_reference;
    typedef typename if_type::reference         reference;
    typedef typename if_type::size_type         size_type;

    osss_shared_variable(sc_core::sc_module_name name = 
                         sc_core::sc_gen_unique_name("shared_variable"))
      : base_type( name )
      {}

    virtual ~osss_shared_variable()
      {}

    virtual const char* kind() const
      { return "osss_shared_variable"; }

    // interface methods

    // get the default event
    virtual const sc_core::sc_event& default_event() const
      { return base_type::default_event(); }

    // get the value changed event
    virtual const sc_core::sc_event& value_changed_event() const
      { return base_type::value_changed_event(); }

    // read the current value
    virtual const osss_array<T, Size>& read() const
      { return base_type::read();  }

    // get a reference to the current value (for tracing)
    virtual const osss_array<T, Size>& get_data_ref() const
      { return base_type::get_data_ref(); }

    // was there an event?
    virtual bool event() const
      { return base_type::event(); }

    // write the new value
    virtual void write( const osss_array<T, Size>& data)
      { base_type::write(data); }
    
    virtual size_type size() const 
      { return Size; }

    // Allows to assign values to instances of class osss_array like to 
    // any other array. If a value is assigned to an index which is not in the 
    // range of the actual array, an error will occur. 
    virtual reference operator[]( size_type index );

    virtual reference get_write_ref( size_type index );

    // Allows to read from instances of class osss_array like from every 
    // other array. If the reading is performed on an index which is not in the
    // range of the actual array, an error will occur.
    virtual const_reference operator[]( size_type index ) const 
      { return base_type::m_queue[depth][index]; }

    virtual const_reference get_read_ref( size_type index ) const
      { return base_type:: m_queue[depth][index]; }

 protected:
    virtual void assign_new_value( typename osss_array<T, Size>::size_type,
                                   const T& );
    
 private:
    // disabled
    osss_shared_variable(const osss_shared_variable<
                         osss_array<T, Size>, depth, multiple_writer_check>& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T, unsigned int Size, 
          unsigned int depth, bool multiple_writer_check>
inline
typename osss_shared_variable<osss_array<T, Size>, 
                              depth, multiple_writer_check>::reference
osss_shared_variable<osss_array<T, Size>, depth, multiple_writer_check>::
operator[]( typename osss_shared_variable<osss_array<T, Size>, 
            depth, multiple_writer_check>::size_type index )
{
  typedef osssi::osss_shared_variable_base<
    osss_array<T, Size>, depth, multiple_writer_check> base_type;

  // perform multiple writer check
  if (multiple_writer_check)
  {
    sc_core::sc_object* writer = 
      sc_core::sc_get_curr_simcontext()->get_current_writer();
    if( base_type::m_writer == 0 ) {
	base_type::m_writer = writer;
    } else if( base_type::m_writer != writer ) {
      sc_core::sc_signal_invalid_writer( this, base_type::m_writer, writer, true );
    }
  }

  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size, 
          unsigned int depth, bool multiple_writer_check>
inline
typename osss_shared_variable<osss_array<T, Size>, 
                              depth, multiple_writer_check>::reference
osss_shared_variable<osss_array<T, Size>, depth, multiple_writer_check>::
get_write_ref( typename osss_shared_variable<osss_array<T, Size>, 
               depth, multiple_writer_check>::size_type index )
{
  typedef osssi::osss_shared_variable_base<
    osss_array<T, Size>, depth, multiple_writer_check> base_type;

  // perform multiple writer check
  if (multiple_writer_check)
  {
    sc_core::sc_object* writer = 
      sc_core::sc_get_curr_simcontext()->get_current_writer();
    if( base_type::m_writer == 0 ) {
	base_type::m_writer = writer;
    } else if( base_type::m_writer != writer ) {
      sc_core::sc_signal_invalid_writer( this, base_type::m_writer, writer, true );
    }
  }

  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size,
          unsigned int depth, bool multiple_writer_check>
inline
void
osss_shared_variable<osss_array<T, Size>, depth, multiple_writer_check>::
assign_new_value( typename osss_array<T, Size>::size_type index,
                                   const T& value_)
{
  typedef osssi::osss_shared_variable_base<
    osss_array<T, Size>, depth, multiple_writer_check> base_type;

  base_type::m_new_val[index] = value_;
  if( !( base_type::m_new_val == base_type::m_queue[0] ) ) {
    base_type::m_queue[0] = base_type::m_new_val;
    if ( base_type::m_change_event_p ) 
      base_type::m_change_event_p->notify(sc_core::SC_ZERO_TIME);
    base_type::m_delta = sc_core::sc_delta_count();
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// non-pipelined version
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable<osss_array<T, Size>, multiple_writer_check >
//
//  The osss_shared_variable<osss_array<T, Size>, multiple_writer_check> 
//  primitive channel class. Partial specialisation of
//  osss_shared_variable<T, multiple_writer_check>
// ----------------------------------------------------------------------------

//
// multiple_writer_check turned off
//
template <typename T, unsigned int Size>
class osss_shared_variable<osss_array<T, Size>, 0, false> : 
  public virtual osss_shared_array_inout_if<T, Size>,
  public virtual osssi::osss_shared_variable_base<osss_array<T, Size>, 0, false>
{
  friend
  class osssi::osss_shared_variable_array_accessor<T, Size>;

  public: // constructors and destructor:
    typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, false> 
            base_type;
    
    typedef osss_shared_array_inout_if<T, Size> if_type;
    typedef typename if_type::const_reference   const_reference;
    typedef typename if_type::reference         reference;
    typedef typename if_type::size_type         size_type;

    osss_shared_variable()
      : base_type()
    {}

    explicit osss_shared_variable( const char* name_ )
      : base_type( name_ )
    {}

    virtual ~osss_shared_variable()
      {}

    virtual const char* kind() const
      { return "osss_shared_variable"; }

    // interface methods

    // get the default event
    virtual const sc_core::sc_event& default_event() const
      { return base_type::default_event(); }

    // get the value changed event
    virtual const sc_core::sc_event& value_changed_event() const
      { return base_type::value_changed_event(); }

    // read the current value
    virtual const osss_array<T, Size>& read() const
      { return base_type::read();  }

    // get a reference to the current value (for tracing)
    virtual const osss_array<T, Size>& get_data_ref() const
      { return base_type::get_data_ref(); }

    // was there an event?
    virtual bool event() const
      { return base_type::event(); }

    // write the new value
    virtual void write( const osss_array<T, Size>& data)
      { base_type::write(data); }
    
    virtual size_type size() const 
      { return Size; }

    // Allows to assign values to instances of class osss_array like to 
    // any other array. If a value is assigned to an index which is not in the 
    // range of the actual array, an error will occur. 
    virtual reference operator[]( size_type index );

    virtual reference get_write_ref( size_type index );

    // Allows to read from instances of class osss_array like from every 
    // other array. If the reading is performed on an index which is not in the
    // range of the actual array, an error will occur.
    virtual const_reference operator[]( size_type index ) const 
      { return base_type::m_cur_val[index]; }

    virtual const_reference get_read_ref( size_type index ) const
      { return base_type::m_cur_val[index]; }

 protected:
    virtual void assign_new_value( typename osss_array<T, Size>::size_type,
                                   const T& );
    
 private:
    // disabled
    osss_shared_variable( 
      const osss_shared_variable<osss_array<T, Size>, 0, false>& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T, unsigned int Size>
inline
typename osss_shared_variable<osss_array<T, Size>, 0, false>::reference
osss_shared_variable<osss_array<T, Size>, 0, false>::
operator[]( 
  typename osss_shared_variable<osss_array<T, Size>, 0, false>::size_type index )
{
  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size>
inline
typename osss_shared_variable<osss_array<T, Size>, 0, false>::reference
osss_shared_variable<osss_array<T, Size>, 0, false>::
get_write_ref( 
  typename osss_shared_variable<osss_array<T, Size>, 0, false>::size_type index )
{
  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size>
inline
void
osss_shared_variable<osss_array<T, Size>, 0, false>::
assign_new_value( typename osss_array<T, Size>::size_type index,
                                   const T& value_)
{
  typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, false> 
    base_type;

  base_type::m_new_val[index] = value_;
  if( !( base_type::m_cur_val[index] == base_type::m_new_val[index] ) ) 
  {
    base_type::m_cur_val[index] = base_type::m_new_val[index];
    if ( base_type::m_change_event_p ) 
      base_type::m_change_event_p->notify(sc_core::SC_ZERO_TIME);
    base_type::m_delta = sc_core::sc_delta_count();
  }
}

//
// multiple_writer_check turned on
//
template <typename T, unsigned int Size>
class osss_shared_variable<osss_array<T, Size>, 0, true> : 
  public virtual osss_shared_array_inout_if<T, Size>,
  public virtual osssi::osss_shared_variable_base<osss_array<T, Size>, 0, true>
{
  friend
  class osssi::osss_shared_variable_array_accessor<T, Size>;

  public: // constructors and destructor:
    typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, true>
            base_type;
    
    typedef osss_shared_array_inout_if<T, Size> if_type;
    typedef typename if_type::const_reference   const_reference;
    typedef typename if_type::reference         reference;
    typedef typename if_type::size_type         size_type;

    osss_shared_variable()
      : base_type()
      {}

    explicit osss_shared_variable( const char* name_ )
      : base_type( name_ )
      {}

    virtual ~osss_shared_variable()
      {}

    virtual const char* kind() const
      { return "osss_shared_variable"; }

    // interface methods

    // get the default event
    virtual const sc_core::sc_event& default_event() const
      { return base_type::default_event(); }

    // get the value changed event
    virtual const sc_core::sc_event& value_changed_event() const
      { return base_type::value_changed_event(); }

    // read the current value
    virtual const osss_array<T, Size>& read() const
      { return base_type::read();  }

    // get a reference to the current value (for tracing)
    virtual const osss_array<T, Size>& get_data_ref() const
      { return base_type::get_data_ref(); }

    // was there an event?
    virtual bool event() const
      { return base_type::event(); }

    // write the new value
    virtual void write( const osss_array<T, Size>& data)
      { base_type::write(data); }
    
    virtual size_type size() const 
      { return Size; }

    // Allows to assign values to instances of class osss_array like to 
    // any other array. If a value is assigned to an index which is not in the 
    // range of the actual array, an error will occur. 
    virtual reference operator[]( size_type index );
 
    virtual reference get_write_ref( size_type index );    

    // Allows to read from instances of class osss_array like from every 
    // other array. If the reading is performed on an index which is not in the
    // range of the actual array, an error will occur.
    virtual const_reference operator[]( size_type index ) const 
      { return base_type::m_cur_val[index]; }

    virtual const_reference get_read_ref( size_type index ) const
      { return base_type::m_cur_val[index]; }

 protected:
    virtual void assign_new_value( typename osss_array<T, Size>::size_type,
                                   const T& );
    
 private:
    // disabled
    osss_shared_variable( 
      const osss_shared_variable<osss_array<T, Size>, 0, true>& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T, unsigned int Size>
inline
typename osss_shared_variable<osss_array<T, Size>, 0, true>::reference
osss_shared_variable<osss_array<T, Size>, 0, true>::
operator[]( 
  typename osss_shared_variable<osss_array<T, Size>, 0, true>::size_type index )
{
  typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, true>
    base_type;
 
  // perform multiple writer check
  sc_core::sc_object* writer = 
    sc_core::sc_get_curr_simcontext()->get_current_writer();
  if( base_type::m_writer == 0 ) {
    base_type::m_writer = writer;
  } else if( base_type::m_writer != writer ) {
    sc_core::sc_signal_invalid_writer( this, base_type::m_writer, writer, true );
  }

  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size>
inline
typename osss_shared_variable<osss_array<T, Size>, 0, true>::reference
osss_shared_variable<osss_array<T, Size>, 0, true>::
get_write_ref( 
  typename osss_shared_variable<osss_array<T, Size>, 0, true>::size_type index )
{
  typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, true>
    base_type;

  // perform multiple writer check
  sc_core::sc_object* writer = 
    sc_core::sc_get_curr_simcontext()->get_current_writer();
  if( base_type::m_writer == 0 ) {
    base_type::m_writer = writer;
  } else if( base_type::m_writer != writer ) {
    sc_core::sc_signal_invalid_writer( this, base_type::m_writer, writer, true );
  }

  if ( index >= Size )
  {
    std::cerr << "Error: Index out of bounds! Size of array is " 
              << Size << ". Requested Index is " << index << "."
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return osssi::osss_shared_variable_array_accessor<T, Size>(this, index);
}

template <class T, unsigned int Size>
inline
void
osss_shared_variable<osss_array<T, Size>, 0, true>::
assign_new_value( typename osss_array<T, Size>::size_type index,
                                   const T& value_)
{
  typedef osssi::osss_shared_variable_base<osss_array<T, Size>, 0, true>
    base_type;

  base_type::m_new_val[index] = value_;
  if( !( base_type::m_cur_val[index] == base_type::m_new_val[index] ) ) 
  {
    base_type::m_cur_val[index] = base_type::m_new_val[index];
    if ( base_type::m_change_event_p ) 
      base_type::m_change_event_p->notify(sc_core::SC_ZERO_TIME);
    base_type::m_delta = sc_core::sc_delta_count();
  }
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

namespace osssi {

template<typename T, unsigned int Size>
inline
void 
osss_shared_variable_array_accessor<T, Size>::
operator=( const T& value )
{
  m_sv->assign_new_value( m_index, value);
}

} // namespace osssi

//-----------------------------------------------------------------------------

template <class T, unsigned int depth, bool multiple_writer_check>
inline
::std::ostream&
operator << ( ::std::ostream& os, 
              const osss_shared_variable<T, depth, multiple_writer_check>& a )
{
  return ( os << a.read() );
}


} // namespace osss

#endif
