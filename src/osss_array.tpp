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

#include <iostream>

namespace osss {
namespace osssi {

template< class T>
inline
osss_array_base<T>::osss_array_base()
{
  m_elements = std::vector< T >();
}

template< class T>
inline
osss_array_base<T>::osss_array_base(const size_type size)
{
  m_elements = std::vector< T >(size);
}

template< class T >
inline
osss_array_base<T>::osss_array_base(const osss_array_base<T> &sourceArray)
{
  m_elements = std::vector<T>(sourceArray.m_elements);
}

template< class T >
inline
osss_array_base<T>::~osss_array_base()
{
  m_elements.clear();
}

template< class T >
inline
typename osss_array_base<T>::size_type
osss_array_base<T>::size() const
{
  return m_elements.size();
}

template< class T >
inline
osss_array_base<T> &
osss_array_base<T>::operator=( const osss_array_base<T> & sourceArray )
{
  if ( &sourceArray != this )
  {
    m_elements = sourceArray.m_elements;
  }
  return(*this);
}

template< class T>
inline
typename osss_array_base<T>::reference
osss_array_base<T>::operator[]( size_type index )
{
  if ( index >= m_elements.size() )
  {
    std::cerr << "ERROR: " << __func__ << "() of an osss_array called "
	      << "during runtime. Maybe you want to use osss_vector instead?"
	      << std::endl;
    exit(EXIT_FAILURE);
  }
  return m_elements.operator[](index);
}

template< class T>
inline
typename osss_array_base<T>::const_reference
osss_array_base<T>::operator[]( size_type index ) const
{
  if ( index >= m_elements.size() )
  {
    // we cannot re-size m_elements to fit the needs since we are "const".
    std::cerr << "ERROR: " << __func__ << "() "
	      << "Cannot access index larger than vector size" << std::endl;
    exit(EXIT_FAILURE);
  }
  return m_elements.operator[](index);
}

template< class T>
inline
bool
osss_array_base<T>::operator==( const osss_array_base<T> & rhsArray ) const
{
  return ( m_elements == rhsArray.m_elements );
}

} // end namespace osssi
} // end namespace osss


//----------------------------------------------------------------------------//
// class osss_array
//----------------------------------------------------------------------------//

namespace osss {

template< class T, unsigned int Size >
inline
osss_array<T, Size>::osss_array() : base_type(Size)
{ }

template< class T, unsigned int Size >
inline
osss_array<T, Size>::osss_array(const osss_array<T, Size> &sourceArray)
{
  base_type::m_elements = std::vector<T>(sourceArray.m_elements);
}

template< class T, unsigned int Size >
inline
osss_array<T, Size>::osss_array( const T* array )
{
  for(unsigned int i=0; i<Size; ++i)
  {
    base_type::m_elements[i] = array[i];
  }
}

template< class T, unsigned int Size >
inline
osss_array<T, Size>::~osss_array()
{
  base_type::m_elements.clear();
}

template< class T, unsigned int Size >
inline
osss_array<T, Size> &
osss_array<T, Size>::operator=( const osss_array<T, Size> & sourceArray )
{
  if ( &sourceArray != this )
  {
    base_type::m_elements = sourceArray.m_elements;
  }
  return(*this);
}

} // end namespace osss


// Overloading this method is required by SystemC. But it does
// actually have some reasonable functionality for instances of this class.
template< class T, unsigned int Size >
inline
void
sc_core::sc_trace ( sc_core::sc_trace_file *,
                    const osss::osss_array<T, Size> &,
                    const std::string & )
{ }

namespace osss {

// Dumps the actual content of the array into an ostream.
// Overloading of this operator is required by SystemC.
template< class T, unsigned int Size >
inline
::std::ostream &
operator<<( ::std::ostream & os, const osss::osss_array<T, Size> & object )
{
  for( typename osss::osss_array<T, Size>::size_type i = 0;
       i < object.size(); ++i )
  {
    os << object[i] << " ";
  }
  return os;
}

} // end namespace osss
