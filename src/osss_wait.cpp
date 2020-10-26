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

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

#include "osss_global_port_registry.h"
#include "osss_event.h"
#include "osss_event_ports.h"
#include "osss_module.h"

namespace osss {

void wait( const sc_core::sc_event& e,  
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() )
{ 
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  assert(ph.valid());
    
  osss_module::reset_lookup_t::iterator rlookup_iter = 
    osss_module::reset_lookup.find(ph.name());
  if (rlookup_iter != osss_module::reset_lookup.end())
  {
    const sc_core::sc_event& reset_ev = 
      rlookup_iter->second.first->get_event()->const_ref();
    sc_core::wait( e | reset_ev, sc );
    ::osss::osssi::handle_reset(rlookup_iter);
  }
  else
    sc_core::wait( e, sc );
}

void wait( sc_core::sc_event_or_list& el,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ 
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  assert(ph.valid());
    
  osss_module::reset_lookup_t::iterator rlookup_iter = 
    osss_module::reset_lookup.find(ph.name());
  if (rlookup_iter != osss_module::reset_lookup.end())
  {
    const sc_core::sc_event& reset_ev = 
      rlookup_iter->second.first->get_event()->const_ref();
    sc_core::wait( el | reset_ev, sc );
    ::osss::osssi::handle_reset(rlookup_iter);
  }
  else
    sc_core::wait( el, sc );
}

// Problematic since an sc_event_and_list can not be OR-connected
// with the reset event
void wait(sc_core::sc_event_and_list& evl, 
          sc_core::sc_simcontext* sc = sc_get_curr_simcontext())
{ 
  static bool warning = true;
  if (warning)
  {
    sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
    assert(ph.valid());
    
    osss_module::reset_lookup_t::iterator rlookup_iter = 
      osss_module::reset_lookup.find(ph.name());
    if (rlookup_iter != osss_module::reset_lookup.end())
    {
      std::cout << "Warning: wait of type " << __PRETTY_FUNCTION__
                << " supresses all reset events of OSSS_THREADs."
                << " Do not use it!"
                << std::endl;
      warning = false;
    }
  }
  sc_core::wait(evl, sc); 
}

void wait( const sc_core::sc_time& t,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ 
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  assert(ph.valid());
    
  osss_module::reset_lookup_t::iterator rlookup_iter = 
    osss_module::reset_lookup.find(ph.name());
  if (rlookup_iter != osss_module::reset_lookup.end())
  {
    const sc_core::sc_event& reset_ev = 
      rlookup_iter->second.first->get_event()->const_ref();
    sc_core::wait( t, reset_ev, sc );
    ::osss::osssi::handle_reset(rlookup_iter);
  }
  else
    sc_core::wait( t, sc );
}

void wait( const sc_core::sc_time& t, const sc_core::sc_event& e,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() )
{ 
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  assert(ph.valid());
    
  osss_module::reset_lookup_t::iterator rlookup_iter = 
    osss_module::reset_lookup.find(ph.name());
  if (rlookup_iter != osss_module::reset_lookup.end())
  {
    const sc_core::sc_event& reset_ev = 
      rlookup_iter->second.first->get_event()->const_ref();
    sc_core::wait( t, e | reset_ev, sc );
    ::osss::osssi::handle_reset(rlookup_iter);
  }
  else
    sc_core::wait( t, e, sc );
}

void wait( const sc_core::sc_time& t, sc_core::sc_event_or_list& el,
           sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext() )
{ 
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  assert(ph.valid());
    
  osss_module::reset_lookup_t::iterator rlookup_iter = 
    osss_module::reset_lookup.find(ph.name());
  if (rlookup_iter != osss_module::reset_lookup.end())
  {
    const sc_core::sc_event& reset_ev = 
      rlookup_iter->second.first->get_event()->const_ref();
    sc_core::wait( t, el | reset_ev, sc );
    ::osss::osssi::handle_reset(rlookup_iter);
  }
  else
    sc_core::wait( t, el, sc );
}

// Problematic since an sc_event_and_list can not be OR-connected
// with the reset event
void wait(const sc_core::sc_time& t, sc_core::sc_event_and_list& evl, 
          sc_core::sc_simcontext* sc = sc_core::sc_get_curr_simcontext())
{ 
  static bool warning = true;
  if(warning)
  {
    sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
    assert(ph.valid());
    
    osss_module::reset_lookup_t::iterator rlookup_iter = 
      osss_module::reset_lookup.find(ph.name());
    if (rlookup_iter != osss_module::reset_lookup.end())
    {
      std::cout << "Warning: wait of type " << __PRETTY_FUNCTION__
                << " supresses all reset events of OSSS_THREADs."
                << " Do not use it!"
                << std::endl;
      warning = false;
    }
  }
  sc_core::wait(t, evl, sc); 
}

} // namespace osss

