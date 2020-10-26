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

osss_module::~osss_module()
{
  // delete m_signal_event_converter_channel vector
  for(signal_event_converter_channel_vec_t::iterator it =
        m_signal_event_converter_channel_vec.begin();
      it != m_signal_event_converter_channel_vec.end();
      ++it)
    delete *it;
  m_signal_event_converter_channel_vec.clear();

  // delete m_reset_event_finder_list elements
  for(reset_event_finder_list_t::iterator it = 
        m_reset_event_finder_list.begin();
      it != m_reset_event_finder_list.end();
      ++it)
    delete *it;
  m_reset_event_finder_list.clear();

  // delete m_reset_signal_finder_list elements
  for(reset_signal_finder_list_t::iterator it = 
        m_reset_signal_finder_list.begin();
      it != m_reset_signal_finder_list.end();
      ++it)
    delete *it;
  m_reset_signal_finder_list.clear();
}

void osss_module::reset_event_is( const osss_event_in& ev_in )
{
  const osss_event_in_if* ev_in_if = 
    dynamic_cast<const osss_event_in_if*>(ev_in.get_interface());
  if (ev_in_if)
    this->reset_event_is( *ev_in_if );
  else
  {
    sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
    if(ph.valid())
    {
      // port not bound until now, build reset_event_finder for reconciliation
      m_reset_event_finder_list.push_back(
        new osssi::osss_reset_event_finder(ph.name(), &ev_in) );
    }
  }
}

void osss_module::reset_event_is( const osss_event_in_if& ev_in_if )
{
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  if(ph.valid())
  {
    std::pair<reset_lookup_t::iterator, bool> res =
      osss_module::reset_lookup.insert(
        reset_lookup_t::value_type(ph.name(), reset_reg_t(&ev_in_if, 0)));

    // reset event already registered for this process name
    if(!res.second)
    {
      std::cout << "Error: OSSS Module " << name() 
                << " has more than an single reset event, which is not"
                << " supported so far." << std::endl;
      exit(1);
    }
  }
}

void osss_module::reset_signal_is(const sc_core::sc_in<bool>& port, 
                                  bool level)
{
  const sc_core::sc_signal_in_if<bool>* iface = 
    dynamic_cast<const sc_core::sc_signal_in_if<bool>*>(port.get_interface());
  if (iface)
    this->reset_signal_is( *iface, level );
  else
  {
    sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
    if(ph.valid())
    {
      // port not bound until now, build reset_signal_finder for reconciliation
      m_reset_signal_finder_list.push_back(
        new osssi::osss_reset_signal_finder(ph.name(), &port, level) );
    }
  }
}

void osss_module::reset_signal_is(const sc_core::sc_signal_in_if<bool>& iface, 
                                  bool level)
{
  sc_core::sc_process_handle ph = sc_core::sc_get_current_process_handle();
  if(ph.valid())
  {
    // build osss_signal_event_converter_channel
    osss_signal_event_converter_channel* sig_conv_ch = 
      new osss_signal_event_converter_channel(iface, level);
      
    m_signal_event_converter_channel_vec.push_back( sig_conv_ch );

    const osss_event_in_if* ev_in_if =
      dynamic_cast<const osss_event_in_if*>(sig_conv_ch);
    
    assert(ev_in_if);
    
    std::pair<reset_lookup_t::iterator, bool> res =
      osss_module::reset_lookup.insert(
        reset_lookup_t::value_type(ph.name(), reset_reg_t(ev_in_if, 0)));

    // reset event already registered for this process name
    if(!res.second)
    {
      std::cout << "Error: OSSS Module " << name() 
                << " has more than an single reset event, which is not"
                << " supported so far." << std::endl;
      exit(1);
    }
  }
}

void osss_module::end_of_elaboration()
{
  // register all "unconnected" reset events
  for(reset_event_finder_list_t::iterator it = 
        m_reset_event_finder_list.begin();
      it != m_reset_event_finder_list.end();
      ++it)
  {
    const osss_event_in_if* ev_in_if = 
      dynamic_cast<const osss_event_in_if*>((*it)->ev_in()->get_interface());

    assert(ev_in_if);

    const char* proc_name = (*it)->proc_name();

    std::pair<reset_lookup_t::iterator, bool> res =
      osss_module::reset_lookup.insert(
        reset_lookup_t::value_type(proc_name, reset_reg_t(ev_in_if, 0)));

    // reset event already registered for this process name
    if(!res.second)
    {
      std::cout << "Error: OSSS Module " << name() 
                << " has more than an single reset event, which is not"
                << " supported so far." << std::endl;
      exit(1);
    }
  }

  // register all "unconnected" reset signals
  for(reset_signal_finder_list_t::iterator it = 
        m_reset_signal_finder_list.begin();
      it != m_reset_signal_finder_list.end();
      ++it)
  {
    const sc_core::sc_signal_in_if<bool>* iface = 
      dynamic_cast<const sc_core::sc_signal_in_if<bool>*>(
        (*it)->port_in()->get_interface());

    assert(iface);

    const char* proc_name = (*it)->proc_name();
    bool level = (*it)->level();

    // build osss_signal_event_converter_channel
    osss_signal_event_converter_channel* sig_conv_ch = 
      new osss_signal_event_converter_channel(*iface, level);
      
    m_signal_event_converter_channel_vec.push_back( sig_conv_ch );

    const osss_event_in_if* ev_in_if =
      dynamic_cast<const osss_event_in_if*>(sig_conv_ch);
    
    assert(ev_in_if);
    
    std::pair<reset_lookup_t::iterator, bool> res =
      osss_module::reset_lookup.insert(
        reset_lookup_t::value_type(proc_name, reset_reg_t(ev_in_if, 0)));

    // reset event already registered for this process name
    if(!res.second)
    {
      std::cout << "Error: OSSS Module " << name() 
                << " has more than an single reset event, which is not"
                << " supported so far." << std::endl;
      exit(1);
    }
    
  }

  //------------------------------------------------
  // do some cleanup
  //------------------------------------------------

  // delete m_reset_event_finder_list elements
  for(reset_event_finder_list_t::iterator it = 
        m_reset_event_finder_list.begin();
      it != m_reset_event_finder_list.end();
      ++it)
    delete *it;
  m_reset_event_finder_list.clear();

  // delete m_reset_signal_finder_list elements
  for(reset_signal_finder_list_t::iterator it = 
        m_reset_signal_finder_list.begin();
      it != m_reset_signal_finder_list.end();
      ++it)
    delete *it;
  m_reset_signal_finder_list.clear();
}

namespace osssi {

void handle_reset( const osss_module::reset_lookup_t::iterator& rlookup_iter )
{
  sc_dt::uint64 last_notified = 
    rlookup_iter->second.first->get_event()->last_notified();
  sc_core::sc_time scheduled_time = 
    rlookup_iter->second.first->get_event()->scheduled_time();
    
/*
  std::cout << "last_notified: " << last_notified
  << " scheduled_time: " << scheduled_time
  << " m_last_reset_notification: " << m_last_reset_notification
  << std::endl;
*/

  if( (sc_core::sc_delta_count() >= last_notified) &&
      (sc_core::sc_time_stamp() >= scheduled_time) &&
      ( (last_notified > rlookup_iter->second.second) || 
        ( (last_notified == 0) && (rlookup_iter->second.second == 0) && 
          (scheduled_time != sc_core::sc_time(0, sc_core::SC_NS))
          )
        ) 
    )
  {
    rlookup_iter->second.second = sc_core::sc_delta_count();
    throw osssi::osss_reset_exception();
  }
}

// this is the main routine that embedds the user-defined main mathod
// and implements the reset behaviour
void osss_module_main_routine(osss_main_routine_runner& runner)
{
  while( true )
  {
    try 
    {
      runner.run_user_code(); // run user-defined main method
    }
    catch( ::osss::osssi::osss_reset_exception )
    {
      continue;
    }
    break;
  }
  delete &runner;
}

} // namespace osssi

osss_module::reset_lookup_t
osss_module::reset_lookup = osss_module::reset_lookup_t();

} // namespace osss
