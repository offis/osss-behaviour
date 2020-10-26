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

#ifndef OSSS_STATE_PAIR_INCLUDED
#define OSSS_STATE_PAIR_INCLUDED

namespace osss {

//forward declaration
class osss_state;
class osss_behaviour;

namespace osssi {

//forward declaration
class osss_state_transition;

class osss_condition_evaluater
{
 public:
  osss_condition_evaluater() : eval_result(false) {}

  osss_condition_evaluater(const std::string& name) :
    eval_result(false),
    m_name(name)
  {}

  virtual ~osss_condition_evaluater() {}

  virtual void evaluate() = 0;
  bool eval_result;
  const std::string& name() const { return m_name; }

 protected:
  std::string m_name;
};

template<typename T>
class osss_condition_helper : public osss_condition_evaluater
{
 public:
  osss_condition_helper( T object, 
                         const std::string& name ) :
    osss_condition_evaluater(name),
    m_object(object)
  {}

  virtual void evaluate() { eval_result = m_object(); }

  protected:
    T m_object;
};

class osss_state_pair
{
 public:
  osss_state_pair() : m_state_pair() {}

  void set_first(const osss_state& state) { m_state_pair.first = &state; }
  void set_second(const osss_state& state) { m_state_pair.second = &state; }
  
  const osss_state* get_first() const { return m_state_pair.first; }
  const osss_state* get_second() const { return m_state_pair.second; }

  template<typename T>
  osss_state_transition& operator,(osss_condition_wrapper<T> cw) const;

 private:
  std::pair<const osss_state*, const osss_state*> m_state_pair;

 private:
  osss_state_pair(const osss_state_pair&);
  osss_state_pair& operator=(const osss_state_pair&);
};

class osss_state_transition_list
{
  friend class osss_state_transition;
  friend class ::osss::osss_behaviour;

 public:
  int size() const { return m_state_transitions.size(); }
  void push_back(const osss_state_transition& st_)
  {
    osss_state_transition* st = 
      const_cast<osss_state_transition*>(&st_);
    m_state_transitions.push_back(st);
  }

  std::vector<osss_state_transition*>& transitions()
  {
    return m_state_transitions;
  }

  osss_state_transition_list& operator&&(const osss_state_transition& st)
  {
    push_back(st);
    return *this;
  }

 protected:
  explicit osss_state_transition_list(const osss_state_transition& stt_)
  {
    osss_state_transition* stt =
      const_cast<osss_state_transition*>(&stt_);
    m_state_transitions.push_back(stt);
  }

  std::vector<osss_state_transition*> m_state_transitions;

 private:
  osss_state_transition_list();
  osss_state_transition_list(const osss_state_transition_list&);
  osss_state_transition_list& operator=(const osss_state_transition_list&);
};

class osss_state_transition
{
 public:
  explicit osss_state_transition(osss_state_pair* sp,
                                 osss_condition_evaluater* ce) :
    m_state_pair(sp),
    m_condition_eval(ce)
  {}

  const osss_state* anchor() const { return m_state_pair->get_first(); }
  const osss_state* target() const { return m_state_pair->get_second(); }
  bool condition()
  {
    m_condition_eval->evaluate();
    return m_condition_eval->eval_result;
  }

  osss_state_transition_list& operator&&(const osss_state_transition& st);

  const std::string& name() const { return m_condition_eval->name(); }

 protected:
  osss_state_pair*          m_state_pair;
  osss_condition_evaluater* m_condition_eval;

 private:
  osss_state_transition();
  osss_state_transition(const osss_state_transition&);
  osss_state_transition& operator=(const osss_state_transition&);
};

inline
osss_state_transition_list& osss_state_transition::operator&&(const osss_state_transition& st)
{
  osss_state_transition_list* stl = new osss_state_transition_list(*this);
  stl->push_back(st);
  return *stl;
}

template<typename T>
inline
osss_state_transition& osss_state_pair::operator,(osss_condition_wrapper<T> cw) const
{
  osss_condition_evaluater* cond_eval = 
    new osssi::osss_condition_helper<T>(cw.m_cond_obj, cw.m_name);

  osss_state_transition* stt = 
    new osss_state_transition(const_cast<osss_state_pair*>(this), cond_eval);
    
  return *stt;
}

} // namespace osssi

} // namespace osss

#endif
