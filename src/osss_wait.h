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

#ifndef OSSS_WAIT_INCLUDED
#define OSSS_WAIT_INCLUDED

namespace osss {

extern
void wait( const sc_core::sc_event& e,  
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() );
  
extern
void wait( sc_core::sc_event_or_list& el,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext());

extern
void wait(sc_core::sc_event_and_list& evl, 
          sc_core::sc_simcontext* sc = sc_get_curr_simcontext());

extern
void wait( const sc_core::sc_time& t,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext());

inline
void wait(sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait( osss_global_port_registry::get_clock_period(), sc ); }

inline
void wait(int n, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait( n * osss_global_port_registry::get_clock_period(), sc ); }

inline
void wait_delta()
{ osss::wait(sc_core::SC_ZERO_TIME); }

inline
void wait_delta(int n)
{
  for(int i=0; i<n; ++i)
    osss::wait(sc_core::SC_ZERO_TIME);
}

inline
void wait(double v, sc_core::sc_time_unit tu, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait(sc_core::sc_time(v, tu), sc); }

extern
void wait( const sc_core::sc_time& t, const sc_core::sc_event& e,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() );

inline
void wait(double v, sc_core::sc_time_unit tu, const sc_core::sc_event& ev, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait(sc_core::sc_time(v, tu), ev, sc); }

extern
void wait( const sc_core::sc_time& t, sc_core::sc_event_or_list& el,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() );

inline
void wait(double v, sc_core::sc_time_unit tu, sc_core::sc_event_or_list& evl, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait(sc_core::sc_time(v, tu), evl, sc); }

extern
void wait(const sc_core::sc_time& t, sc_core::sc_event_and_list& evl, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext());

inline
void wait(double v, sc_core::sc_time_unit tu, sc_core::sc_event_and_list& evl, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ osss::wait(sc_core::sc_time(v, tu), evl, sc); }

}

#endif
