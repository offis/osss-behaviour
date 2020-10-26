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

#ifndef OSSS_ARRAY_INCLUDED
#define OSSS_ARRAY_INCLUDED

#include <vector>

namespace osss {
namespace osssi {

/**
 * \class osss_array_base
 *
 * \brief This class is a base class for synthesisable arrays.
 *
 * \note This class is not intended to be utilised in a user-design
 *       In a user-design use either: osss_array or osss_vector
 *
 * \see osss_array
 * \see osss_vector
 */
template< class T > class osss_array_base
{

public:
  typedef typename std::vector< T >::reference reference;
  typedef typename std::vector< T >::const_reference const_reference;

  // We re-use the size_type of the underlying STL container.
  // STL recommends not to use your own datatypes here.
  typedef typename std::vector< T >::size_type size_type;

  //! \brief Constructor. 
  //!
  //! Creates an empty array of size zero
  osss_array_base();

  //! \brief Constructor. 
  //!
  //! Creates an empty array of the specified size
  osss_array_base(const size_type size);
  
  //! \brief Copy constructor. 
  //!
  //! Creates a new array as exact copy of the passed one.
  osss_array_base( const osss_array_base<T> & sourceArray );

  ~osss_array_base();

  //! \brief Returns the size of the array. 
  //!
  //! The size is determined by the 'Size' template argument
  size_type
  size() const;

  //! Copies the contents of the source array to the target array.
  //! This only works when the type T and the size 'Size' of both arrays
  //! are exactly the same
  osss_array_base &
  operator=( const osss_array_base<T> & sourceArray );

  //! Allows to assign values to instances of class osss_array like to 
  //! any other array. If a value is assigned to an index which is not in the 
  //! range of the actual array, an error will occur. 
  reference
  operator[]( size_type index );

  //! Allows to read from instances of class osss_array like from every 
  //! other array. If the reading is performed on an index which is not in the
  //! range of the actual array, an error will occur.
  const_reference
  operator[]( size_type index ) const;

  //! Two instances of class osss_array are only equal, if they are of 
  //! the same size and the value of all corresponding entries are equal. 
  //! In this case true is returned, otherwise false is returned.
  //! ==operator is necessary for using class osss_array with 
  //! sc_signals.
  bool 
  operator == ( const osss_array_base<T> & rhsArray ) const;

protected:
 
  //! This member contains the array itself.
  //! Using a pointer to a vector here will make valgrind unhappy.
  std::vector< T > m_elements;

};

} // namespace osssi


template < class T, unsigned int Size >
class osss_array;

template < class T, unsigned int Size >
::std::ostream &
operator << ( ::std::ostream & os, 
              const osss_array< T, Size > & object );

} // namespace osss

namespace sc_core {
template < class T, unsigned int Size >
void
sc_trace( sc_trace_file * tf, 
          const ::osss::osss_array< T, Size > & object,
          const ::std::string & str );
}

/** \namespace osss */
namespace osss {

/**
 * \class osss_array
 *
 * \brief This class provides a synthesisable array of a fixed size.
 *        The size is determined at compile time and cannot be chaged
 *        during runtime (e.g. during the elaboration phase)
 *
 * \note  For an array that can resized during the elaboration pahse
 *        please refer to class osss_vector
 *
 * \see osss_vector
 */
template< class T, unsigned int Size >
class osss_array : public osssi::osss_array_base<T>
{
public:
  typedef typename osssi::osss_array_base< T > base_type;

  typedef typename osssi::osss_array_base< T >::reference reference;
  typedef typename osssi::osss_array_base< T >::const_reference const_reference;

  // We re-use the size_type of the underlying STL container.
  // STL recommends not to use your own datatypes here.
  typedef typename osssi::osss_array_base< T >::size_type size_type;

  //! \brief Constructor. 
  //!
  //! Creates an empty array of size 'Size'
  osss_array();

  //! \brief Copy constructor. 
  //!
  //! Creates a new array as exact copy of the passed one.
  osss_array( const osss_array<T, Size> & sourceArray );

  osss_array( const T* array );

  ~osss_array();

  //! Copies the contents of the source array to the target array.
  //! This only works when the type T and the size 'Size' of both arrays
  //! are exactly the same
  osss_array &
  operator=( const osss_array<T, Size> & sourceArray );

 private:

  friend
  ::std::ostream &
  operator << <>( ::std::ostream & os, 
                  const osss_array< T, Size > & object );

  friend
  void
  ::sc_core::sc_trace<>( ::sc_core::sc_trace_file * tf, 
                         const osss_array< T, Size > & object,
                         const ::std::string & str );
};

} //namespace osss

#include "osss_array.tpp"

#endif
