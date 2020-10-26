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

#ifndef OSSS_FSM_BEH_INCLUDED
#define OSSS_FSM_BEH_INCLUDED

namespace osss {
namespace osssi {

class osss_state_transition_table
{
 public:
  explicit osss_state_transition_table(osss_behaviour* beh_ptr,
                                       const osss_state_transition_list& stl) :
    m_behaviour(beh_ptr),
    m_stl(const_cast<osss_state_transition_list&>(stl))
  {}

  // check for potentially conflicting transitions
  bool check(bool print)
  {
    std::vector<int> conflict_list;
    for(int i=0; i<m_stl.size(); ++i)
    {
      for(int j=0; j<m_stl.size(); ++j)
      {
        if(i < j)
        {
          if ((m_stl.transitions().at(i)->anchor() == 
               m_stl.transitions().at(j)->anchor()) &&
              (m_stl.transitions().at(i)->target() ==
               m_stl.transitions().at(j)->target()))
          {
            conflict_list.push_back(i);
            conflict_list.push_back(j);
          }
        }
      }
    }
    if (print)
    {
      if (conflict_list.size() != 0)
      {
        std::cout << "Conflicting transitions for behaviour "
                  << m_behaviour->name() << " found in:" << std::endl;
        for(unsigned int i=0; i<conflict_list.size(); ++i)
        {
          std::cout << "line " << conflict_list[i] 
                    << " [" 
                    << m_stl.transitions().at(conflict_list[i])->anchor()->name()
                    << " -> "
                    << m_stl.transitions().at(conflict_list[i])->target()->name()
                    << "]"
                    << std::endl;
        }
        std::cout << std::endl;
      }
    }
    return (conflict_list.size() == 0);
  }

  osss_state* next_state(const osss_state* current_state)
  {
    // next state can only be evaluated during simulation
    if(sc_core::sc_is_running())
    {
      // return target state of first transition whose
      // condition evaluates to true
      for(int i=0; i<m_stl.size(); ++i)
      {
        if ((m_stl.transitions().at(i)->anchor() == current_state) &&
            ( m_stl.transitions().at(i)->condition()))
          return const_cast<osss_state*>(m_stl.transitions().at(i)->target());
      }
      // no transition found
      return const_cast<osss_state*>(current_state);
    }
    else
      return const_cast<osss_state*>(current_state);
  }

  osss_state_transition_list& get_state_transition_list() const
  {
    return m_stl;
  }

 protected:
  osss_behaviour*             m_behaviour;
  osss_state_transition_list& m_stl;

};

class osss_fsm_behaviour : public osss_composite_behaviour
{
public:

  explicit osss_fsm_behaviour(osss_behaviour* beh_ptr,
                              const osss_state_transition_list& stl) : 
    osss_composite_behaviour(beh_ptr),
    m_active_state(NULL),
    m_state_transition_table(beh_ptr, stl)
  {}

  virtual void main() 
  {
    // call initialization action
    this->behaviour()->init();

    m_active_state->enter();
    // state machine main loop
    while(true) 
    {
      wait();

      if (m_active_state->end())
      {
        m_active_state->exit(); 
        break;
      }
      else
      {
        osss_state* next_state = 
          m_state_transition_table.next_state(m_active_state);
        m_active_state->exit(); 
        next_state->enter(); 
        m_active_state = next_state;
      }
    }

    // call finalization action
    this->behaviour()->final(); 
  }

  osss::osss_state* m_active_state;

  osss_state_transition_list& get_state_transition_list() const
  {
    return m_state_transition_table.get_state_transition_list();
  }
  
private:
  osss_state_transition_table m_state_transition_table;

  osss_fsm_behaviour();
  osss_fsm_behaviour(const osss_fsm_behaviour&);
  osss_fsm_behaviour& operator=(const osss_fsm_behaviour&);
};

} // namespace osssi
} // namespace osss

#endif
