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

#ifndef OSSS_SHARED_VARIABLE_IFS_INCLUDED
#define OSSS_SHARED_VARIABLE_IFS_INCLUDED

namespace osss {

// forward declarations
namespace osssi {
  template<typename T, unsigned int Size>
  class osss_shared_variable_array_accessor;
}

// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_in_if<T>
//
//  The osss_shared_variable<T> input interface class.
// ----------------------------------------------------------------------------

template<typename T>
class osss_shared_variable_in_if : 
  virtual public sc_core::sc_interface
{
public:
  // get the value changed event
  virtual const sc_core::sc_event& value_changed_event() const = 0;

  // read the current value
  virtual const T& read() const = 0;

  // get a reference to the current value (for tracing)
  virtual const T& get_data_ref() const = 0;
  
  // was there a value changed event?
  virtual bool event() const = 0;

protected:
  // constructor
  osss_shared_variable_in_if() {}

private:
  // disabled
  osss_shared_variable_in_if( const osss_shared_variable_in_if<T>& );

  osss_shared_variable_in_if<T>& 
  operator=( const osss_shared_variable_in_if<T>& );
};

//
// specialization for osss_arrays
//
template<typename T, unsigned int Size>
class osss_shared_array_in_if :
  public osss_shared_variable_in_if< osss_array<T, Size> >
{
 public:
  typedef typename osss_array< T, Size >::const_reference const_reference;
  typedef typename osss_array< T, Size >::size_type       size_type;
  
  // constructor
  osss_shared_array_in_if() {}

  // returns the size of the array
  virtual size_type size() const = 0;

  // reads value at the specified array index
  virtual const_reference operator[]( size_type index ) const = 0;
  virtual const_reference get_read_ref( size_type index ) const = 0;

 private:
  // disabled
  osss_shared_array_in_if( const osss_shared_array_in_if<T, Size>& );

  osss_shared_array_in_if<T, Size>&
  operator=( const osss_shared_array_in_if<T, Size>& );
};


// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_write_if<T>
//
//  The standard output interface class.
// ----------------------------------------------------------------------------
template<typename T>
class osss_shared_variable_write_if : 
  public virtual sc_core::sc_interface
{
public:
  // constructor
  osss_shared_variable_write_if() {}

  // write the new value
  virtual void write( const T& ) = 0;

private:
  // disabled
  osss_shared_variable_write_if( const osss_shared_variable_write_if<T>& );

  osss_shared_variable_write_if<T>& 
  operator=( const osss_shared_variable_write_if<T>& );
};

//
// specialization for osss_arrays
//
template<typename T, unsigned int Size>
class osss_shared_array_write_if :
  public osss_shared_variable_write_if< osss_array<T, Size> >
{
 public:
  typedef osssi::osss_shared_variable_array_accessor<T, Size> reference;
  typedef typename osss_array< T, Size >::size_type           size_type;

  // constructor
  osss_shared_array_write_if() {}

  // writes value at the specified array index
  virtual reference operator[]( size_type index ) = 0;
  virtual reference get_write_ref( size_type index ) = 0;

 private:
  // disabled
  osss_shared_array_write_if( const osss_shared_array_write_if<T, Size>& );

  osss_shared_array_write_if<T, Size>&
  operator=(const osss_shared_array_write_if<T, Size>& );
};


// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_inout_if<T>
//
//  The osss_shared_variable<T> input/output interface class.
// ----------------------------------------------------------------------------

template<typename T>
class osss_shared_variable_inout_if : 
  public osss_shared_variable_in_if<T>, 
  public osss_shared_variable_write_if<T>
{
protected:
  // constructor
  osss_shared_variable_inout_if() {}

private:
  // disabled
  osss_shared_variable_inout_if( const osss_shared_variable_inout_if<T>& );

  osss_shared_variable_inout_if<T>& 
  operator=( const osss_shared_variable_inout_if<T>& );
};

//
// specialization for osss_arrays
//
template<typename T, unsigned int Size>
class osss_shared_array_inout_if :
  public osss_shared_array_in_if<T, Size>,
  public osss_shared_array_write_if<T, Size>
{
 public:
  typedef osssi::osss_shared_variable_array_accessor<T, Size> reference;
  typedef typename osss_array< T, Size >::const_reference     const_reference;
  typedef typename osss_array< T, Size >::size_type           size_type;

  // constructor
  osss_shared_array_inout_if() {}

 private:
  // disabled
  osss_shared_array_inout_if( const osss_shared_array_inout_if<T, Size>& );

  osss_shared_array_inout_if<T, Size>&
  operator=( const osss_shared_array_inout_if<T, Size>& );
};


// ----------------------------------------------------------------------------
//  CLASS : osss_shared_variable_out_if<T>
//
//  The sc_signal<T> output interface class.
// ----------------------------------------------------------------------------

// osss_shared_variable_out_if can also be read from, hence no difference with
// osss_shared_variable_inout_if.

#define osss_shared_variable_out_if osss_shared_variable_inout_if

//
// specialization for osss_arrays
//
#define osss_shared_array_out_if osss_shared_array_inout_if

} // namespace osss

#endif

