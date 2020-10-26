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
// CAUTION: This file is deprecated and should not be included anymore
//

#ifndef OSSS_PAR_INCLUDED
#define OSSS_PAR_INCLUDED

//------------------------------------------------------------------------------
// PAR construct
//------------------------------------------------------------------------------

#define _OSSS_PAR0() 

#define _OSSS_PAR1( p1 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)) \
   OSSS_JOIN

#define _OSSS_PAR2( p1, p2 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2))  \
   OSSS_JOIN

#define _OSSS_PAR3( p1, p2, p3 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p3))  \
   OSSS_JOIN 

#define _OSSS_PAR4( p1, p2, p3, p4 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p3)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p4))  \
   OSSS_JOIN 

#define _OSSS_PAR5( p1, p2, p3, p4, p5 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p3)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p4)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p5))  \
   OSSS_JOIN 

#define _OSSS_PAR6( p1, p2, p3, p4, p5, p6 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p3)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p4)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p5)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p6))  \
   OSSS_JOIN

#define _OSSS_PAR7( p1, p2, p3, p4, p5, p6, p7 ) \
   OSSS_FORK \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p1)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p2)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p3)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p4)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p5)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p6)), \
     sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, &p7))  \
   OSSS_JOIN

//
// ... and so on ...
//

#define OSSS_PAR( N, ...) \
  _OSSS_PAR##N( __VA_ARGS__ )

namespace osss {

// same as macro above using ellipse (...) operator
//
// needs to be called ::osss::osss_par(3, &b1, &b2, &b3);
// the address (&) operator is needed to get a raw void* 
//
inline void osss_par( unsigned int amount, ... )
{
  va_list argument_list;
  va_start(argument_list, amount);

  // FORK
  std::vector<sc_core::sc_process_handle> forkees;
  for(unsigned int i=0; i<amount; ++i)
  {
    void* vp = va_arg(argument_list, void*);
    ::osss::osss_behaviour* beh = 
        (::osss::osss_behaviour*)(vp);
    forkees.push_back( 
      sc_core::sc_spawn(sc_bind(&::osss::osss_behaviour::main, beh)));
  }

  // JOIN
  sc_core::sc_join join;
  for ( unsigned int i=0; i<amount; ++i)
    join.add_process(forkees[i]);
  sc_core::sc_process_handle cph = sc_core::sc_get_current_process_handle();
  if (cph.proc_kind() == sc_core::SC_THREAD_PROC_)
    join.wait();
  else if (cph.proc_kind() == sc_core::SC_CTHREAD_PROC_)
    join.wait_clocked();

 va_end(argument_list);
}

}

#endif
