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

#ifndef OSSS_PORT_INCLUDE
#define OSSS_PORT_INCLUDE

namespace osss {

template <class IF, 
          int N = 1, sc_core::sc_port_policy P=sc_core::SC_ONE_OR_MORE_BOUND>
class osss_port : public sc_core::sc_port<IF, N, P>
{
  // typdefs
  
  typedef sc_core::sc_port<IF, N, P> base_type;
  typedef osss_port<IF, N, P>        this_type;
  
  public:
  
  // constructors
  
  osss_port() : base_type() 
  {}
  
  explicit osss_port( const char* name_ ) : base_type(name_) 
  {}

  explicit osss_port( IF& interface_ ) : base_type(interface_) 
  {}
 
  osss_port( const char* name_, IF& interface_ ) : base_type(name_, interface_) 
  {}
 
  explicit osss_port( base_type& parent_ ) : base_type(parent_) 
  {}
 
  osss_port( const char* name_, base_type& parent_ ) : base_type(name_, parent_) 
  {}
  
  osss_port( this_type& parent_ ) : base_type(parent_) 
  {}
  
  osss_port( const char* name_, this_type& parent_ ) : base_type(name_, parent_)
  {}
  
  // destructor (does nothing)
 
  virtual ~osss_port() 
  {}
 
  virtual const char* kind() const
  { return "osss_port"; }
 
  private:
 
  // disabled
  osss_port( const this_type& );
  this_type& operator = ( const this_type& );
};

} // namespace osss

#endif
