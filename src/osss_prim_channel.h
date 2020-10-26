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

#ifndef OSSS_PRIM_CHANNEL_INCLUDED
#define OSSS_PRIM_CHANNEL_INCLUDED

namespace osss {

class osss_prim_channel : public sc_core::sc_prim_channel
{
public:
  virtual const char* kind() const
    { return "osss_prim_channel"; }

protected:

  // constructors
  osss_prim_channel() : sc_core::sc_prim_channel() 
    { }
  
  explicit osss_prim_channel( const char* name ) : sc_core::sc_prim_channel( name ) 
    { }

  // destructor
  virtual ~osss_prim_channel() {}

protected:
  // static sensitivity for SC_THREADs and SC_CTHREADs

  void wait()
    { osss::wait( simcontext() ); }

  // dynamic sensitivity for SC_THREADs and SC_CTHREADs

  void wait( const sc_core::sc_event& e )
    { osss::wait( e, simcontext() ); }

  void wait( sc_core::sc_event_or_list& el )
    { osss::wait( el, simcontext() ); }

  void wait( sc_core::sc_event_and_list& el )
    { osss::wait( el, simcontext() ); }

  void wait( const sc_core::sc_time& t )
    { osss::wait( t, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu )
    { osss::wait( sc_core::sc_time( v, tu, simcontext() ), simcontext() ); }

  void wait( const sc_core::sc_time& t, const sc_core::sc_event& e )
    { osss::wait( t, e, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu, const sc_core::sc_event& e )
    { osss::wait( sc_core::sc_time( v, tu, simcontext() ), e, simcontext() ); }

  void wait( const sc_core::sc_time& t, sc_core::sc_event_or_list& el )
    { osss::wait( t, el, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu, sc_core::sc_event_or_list& el )
    { osss::wait( sc_core::sc_time( v, tu, simcontext() ), el, simcontext() ); }

  void wait( const sc_core::sc_time& t, sc_core::sc_event_and_list& el )
    { osss::wait( t, el, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu, sc_core::sc_event_and_list& el )
    { osss::wait( sc_core::sc_time( v, tu, simcontext() ), el, simcontext() ); }

  void wait( int n )
    { osss::wait( n, simcontext() ); }

private:

    // disabled
    osss_prim_channel( const osss_prim_channel& );
    osss_prim_channel& operator=( const osss_prim_channel& );
};

} // namespace osss

#endif
