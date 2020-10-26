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

#ifndef OSSS_PARALLEL_BEH_INCLUDED
#define OSSS_PARALLEL_BEH_INCLUDED

#include "osss_composite_beh.h"

namespace osss {
namespace osssi {

class osss_parallel_behaviour :  public osss_composite_behaviour
{
public:

  osss_parallel_behaviour(osss_behaviour* beh_ptr) :
    osss_composite_behaviour(beh_ptr)
  {}
  
  virtual void main()
  { 
    // call initialization action
    this->behaviour()->init();
        
    // FORK
    std::vector<sc_core::sc_process_handle> forkees;
    for(unsigned int i=0; i<m_behaviour_list.size(); ++i)
    {
      forkees.push_back( 
        sc_core::sc_spawn(
          sc_bind(&::osss::osss_behaviour::main, m_behaviour_list[i]),
                  sc_core::sc_gen_unique_name("process_p_"),
                  m_behaviour_list[i]->get_spawn_options()));
    }

    // JOIN
    sc_core::sc_join join;
    for ( unsigned int i=0; i<forkees.size(); ++i)
    {
      join.add_process(forkees[i]);
    }
    sc_core::sc_process_handle cph = sc_core::sc_get_current_process_handle();
    if (cph.proc_kind() == sc_core::SC_THREAD_PROC_)
      join.wait();
    else if (cph.proc_kind() == sc_core::SC_CTHREAD_PROC_)
      join.wait_clocked();

    // call finalization action
    this->behaviour()->final();
  }

private:
  osss_parallel_behaviour();
  osss_parallel_behaviour(const osss_parallel_behaviour& beh);
  osss_parallel_behaviour& operator=(const osss_parallel_behaviour& beh);
};

} // namespace osssi
} // namespace osss

#endif
