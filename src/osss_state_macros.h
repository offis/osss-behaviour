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

#ifndef OSSS_STATE_MACROS_INCLUDED
#define OSSS_STATE_MACROS_INCLUDED

namespace osss {

#define IS_INITIAL_STATE(state) \
  state.initial(true);

#define IS_END_STATE(state) \
  state.end(true);

#define OSSS_FSM \
   std::vector<osss::osss_state*> state_vec; \
   std::vector<sc_core::sc_object*> module_objs(get_child_objects()); \
   for(unsigned int i=0; i<module_objs.size(); i++) \
   { \
     osss::osss_state* state = dynamic_cast<osss::osss_state*>(module_objs[i]); \
     if (state != NULL) \
       state_vec.push_back(state); \
   } \
   osss::osss_state* active_state = NULL; \
   for(unsigned int i=0; i<state_vec.size(); i++) \
   { \
     if(active_state == NULL && state_vec[i]->initial()) \
       active_state = state_vec[i]; \
     else if(active_state != NULL && state_vec[i]->initial()) \
     { \
       std::cerr << "Error in: " << __PRETTY_FUNCTION__ \
                 << " more than a single initial state found!" << std::endl; \
       sc_core::sc_stop(); \
     } \
   } \
   if (active_state == NULL) \
   { \
     std::cerr << "Error in: " << __PRETTY_FUNCTION__ \
               << " no initial state found!" << std::endl; \
     sc_core::sc_stop(); \
   } \
   active_state->enter(); \
   while(true) 

namespace osssi {

  template<typename T>
  struct _fsm_case_helper
  {
    _fsm_case_helper(T& state) : m_state(state) {}

    inline
    bool operator == (osss_state* active_state)
    { 
      return (active_state == &m_state); 
    }

    T& m_state;
  };

  template<typename T>
  struct _fsm_case_helper<T*>
  {
    _fsm_case_helper(T* state) : m_state(state) {}

    inline
    bool operator == (osss_state* active_state)
    { 
      return (active_state == m_state); 
    }

    T* m_state;
  };

} //namespace osssi

#define fsm_case(state) \
  if ( ::osss::osssi::_fsm_case_helper< typeof(state) >(state) == active_state )

namespace osssi {

  template<typename T>
  struct _fsm_enter_state_helper
  {
    _fsm_enter_state_helper(T& state) : m_state(state) {}

    inline
    osss_state* doit (osss_state* active_state)
    { 
      active_state->exit(); m_state.enter(); active_state = &m_state;
      return active_state;
    }

    T& m_state;
  };

  template<typename T>
  struct _fsm_enter_state_helper<T*>
  {
    _fsm_enter_state_helper(T* state) : m_state(state) {}

    inline
    osss_state* doit (osss_state* active_state)
    { 
      active_state->exit(); m_state->enter(); active_state = m_state;
      return active_state;
    }

    T* m_state;
  };

} //namespace osssi

#define enter_state(state) \
   ::osss::osssi::_fsm_enter_state_helper< typeof(state) > _fesh(state); \
   active_state = _fesh.doit(active_state); \
   continue;

#define enter_end_state() \
   active_state->exit(); break;

} // namespace osss

#endif
