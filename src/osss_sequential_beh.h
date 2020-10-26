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

#ifndef OSSS_SEQUENTIAL_BEH_INCLUDED
#define OSSS_SEQUENTIAL_BEH_INCLUDED

#include "osss_composite_beh.h"

namespace osss {
namespace osssi {

class osss_sequential_behaviour : public osss_composite_behaviour
{
public:

  explicit osss_sequential_behaviour(osss_behaviour* beh_ptr) :
    osss_composite_behaviour(beh_ptr)
  {}
  
  virtual void main()
  {
    // call initialization action
    this->behaviour()->init();
    
    for(unsigned int i=0; i<m_behaviour_list.size(); ++i)
    {
#ifdef OSSS_SEQUENTIAL_BEHAVIOUR_SIMPLE
      //
      // Using this complicated fork-join construct induces
      // lots of overhead for a simple sequence call.
      // A much more easier way of implementing a seq would simply be

      m_behaviour_list[i]->main();

      // The main problem with this simple construction is that 
      // the main is not really a process of its own and all manually
      // annotated sensitivities would be discarded.
      //
#else
      // FORK a single process
      sc_core::sc_process_handle forkee =sc_core::sc_spawn(
        sc_bind(&::osss::osss_behaviour::main, m_behaviour_list[i]),
        sc_core::sc_gen_unique_name("process_p_"),
        m_behaviour_list[i]->get_spawn_options());
   
      // "JOIN"
      sc_core::sc_join join;
      join.add_process(forkee);
      sc_core::sc_process_handle cph = 
        sc_core::sc_get_current_process_handle();
      if (cph.proc_kind() == sc_core::SC_THREAD_PROC_)
        join.wait();
      else if (cph.proc_kind() == sc_core::SC_CTHREAD_PROC_)
        join.wait_clocked();
#endif
    }

    // call finalization action
    this->behaviour()->final();
  }

private:
  osss_sequential_behaviour();
  osss_sequential_behaviour(const osss_sequential_behaviour& beh);
  osss_sequential_behaviour& operator=(const osss_sequential_behaviour& beh);
};

} // namespace osssi
} // namespace osss

#endif
