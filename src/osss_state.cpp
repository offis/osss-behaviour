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

#include "osss_clock_reset_if.h"
#include "osss_composite_beh.h"
#include "osss_beh.h"
#include "osss_condition_wrapper.h"
#include "osss_state.h"
#include "osss_state_pair.h"

namespace osss {

osss_state::osss_state() :
  osss_behaviour(),
  m_initial(false),
  m_end(false),
  m_active(false)
{}

osss_state::osss_state(osss_behaviour_name name) :
  osss_behaviour( name ),
  m_initial(false),
  m_end(false),
  m_active(false)
{}

void osss_state::enter() 
{ 
  m_active = true;
  enter_action();
  main();
}

void osss_state::exit()
{
  exit_action();
  m_active = false;
}

bool osss_state::initial() const { return m_initial; }

void osss_state::initial(bool i) { m_initial = i; }

bool osss_state::end() const { return m_end; }

void osss_state::end(bool e) { m_initial = e; }

bool osss_state::active() const { return m_active; }

osssi::osss_state_pair& osss_state::operator>> (const osss_state& state_ref)
{
  // TODO: garbage collection
  osssi::osss_state_pair* state_pair = new osssi::osss_state_pair(); 
  state_pair->set_first(*this);
  state_pair->set_second(state_ref);
  return *state_pair;
}

osssi::osss_state_pair& osss_state::operator>> (const osss_state* state_ptr)
{
  return this->operator>>(*state_ptr);
}

//----------------------------------------------------------
// OSSS initial state
//----------------------------------------------------------

osss_initial_state::osss_initial_state() :
  osss_state()
{
  m_initial = true;
}

osss_initial_state::osss_initial_state(osss_behaviour_name name) :
  osss_state(name)
{
  m_initial = true;
}

//----------------------------------------------------------
// OSSS end state
//----------------------------------------------------------

osss_end_state::osss_end_state() :
  osss_state()
{
  m_end = true;
}

osss_end_state::osss_end_state(osss_behaviour_name name) :
  osss_state(name)
{
  m_end = true;
}

}
