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

#ifndef OSSS_STATE_INCLUDED
#define OSSS_STATE_INCLUDED

namespace osss {

// forward declaration
namespace osssi {
  class osss_state_pair;
}

//------------------------------------------------------------------------------
// FSM construct
//------------------------------------------------------------------------------

class osss_state :
  public osss_behaviour
{
public:

  osss_state();
  osss_state(osss_behaviour_name name);

  void enter();
 
  virtual void enter_action() { /* the default enter action does nothing */ }

  void exit();
 
  virtual void exit_action() { /* the default exit action does nothing */ }

  virtual const char* kind () const
    { return "osss_state"; }

  bool initial() const;
  void initial(bool i);

  bool end() const;
  void end(bool e);

  bool active() const ;

  // this operator defines a state successor relation
  //   "s1 >> s2" means: state s2 follows state s1
  osssi::osss_state_pair& operator>> (const osss_state& state_ref);
  osssi::osss_state_pair& operator>> (const osss_state* state_ptr);
 
protected:
  bool m_initial;
  bool m_end;

  bool m_active;

private:

  osss_state(const osss_state& state);
  osss_state& operator=(const osss_state& state);
};

#define OSSS_STATE(user_state_name) \
  struct user_state_name : ::osss::osss_state

#define STATE_CTOR(user_state_name) \
  typedef user_state_name OSSS_CURRENT_USER_BEHAVIOUR; \
  user_state_name( ::osss::osss_behaviour_name )

//------------------------------------------------------------------------------
// initial state
//------------------------------------------------------------------------------

class osss_initial_state :
  public osss_state
{
public:

  osss_initial_state();
  osss_initial_state(osss_behaviour_name name);
  
  virtual const char* kind () const
    { return "osss_initial_state"; }

protected:

private:

  osss_initial_state(const osss_initial_state& init_state);
  osss_initial_state& operator=(const osss_initial_state& init_state);
};

#define OSSS_INITIAL_STATE(user_state_name) \
  struct user_state_name : ::osss::osss_initial_state

#define INITIAL_STATE_CTOR(user_state_name) \
  typedef user_state_name OSSS_CURRENT_USER_BEHAVIOUR; \
  user_state_name( ::osss::osss_behaviour_name )

//------------------------------------------------------------------------------
// end state
//------------------------------------------------------------------------------

class osss_end_state :
  public osss_state
{
public:

  osss_end_state();
  osss_end_state(osss_behaviour_name name);
  
  virtual const char* kind () const
    { return "osss_end_state"; }

protected:

private:

  osss_end_state(const osss_end_state& end_state);
  osss_end_state& operator=(const osss_end_state& end_state);
};

#define OSSS_END_STATE(user_state_name) \
  struct user_state_name : ::osss::osss_end_state

#define END_STATE_CTOR(user_state_name) \
  typedef user_state_name OSSS_CURRENT_USER_BEHAVIOUR; \
  user_state_name( ::osss::osss_behaviour_name )

} // namespace osss

#endif
