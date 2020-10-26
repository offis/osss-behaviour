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

#ifndef OSSS_FORK_JOIN_INCLUDED
#define OSSS_FORK_JOIN_INCLUDED

//------------------------------------------------------------------------------
// OSSS_FORK & OSSS_JOIN
//
// introduced to avoid problems with SC_JOIN & SC_CJOIN
//------------------------------------------------------------------------------

#define OSSS_FORK \
 { \
     sc_core::sc_process_handle forkees[] = {
 
#define OSSS_JOIN \
     }; \
     sc_core::sc_join           join; \
     for ( unsigned int i = 0; \
           i < sizeof(forkees)/sizeof(sc_core::sc_process_handle); \
           i++ ) \
         join.add_process(forkees[i]); \
     sc_core::sc_process_handle cph = sc_core::sc_get_current_process_handle(); \
     if (cph.proc_kind() == sc_core::SC_THREAD_PROC_) \
       join.wait(); \
     else if (cph.proc_kind() == sc_core::SC_CTHREAD_PROC_) \
       join.wait_clocked(); \
 }

#endif
