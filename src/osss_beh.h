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

#ifndef OSSS_BEH_INCLUDED
#define OSSS_BEH_INCLUDED

#include "osss_clock_reset_if.h"
#include "osss_behaviour_options.h"

namespace osss {

// forward declarations
class osss_parallel_behaviour_list;
class osss_sequential_behaviour_list;
class osss_pipeline_behaviour_list;
class osss_pipeline_stage;

namespace osssi {
  class osss_state_transition;
  class osss_state_transition_list;
  
  template<typename T, unsigned int depth, bool multiple_writer_check>
  class osss_shared_variable_base;

  class osss_composite_behaviour;
  class osss_parallel_behaviour;
  class osss_sequential_behaviour;
  class osss_pipeline_behaviour;
  class osss_fsm_behaviour;
}

enum osss_behaviour_node_type 
{
  OSSS_LEAF_BEHAVIOUR,
  OSSS_SEQ_BEHAVIOUR,
  OSSS_PAR_BEHAVIOUR,
  OSSS_PIPE_BEHAVIOUR,
  OSSS_FSM_BEHAVIOUR
};

typedef sc_core::sc_module_name osss_behaviour_name;

class osss_behaviour : 
  public sc_core::sc_module,
  public osssi::osss_clock_reset_if
{

  template<typename T, unsigned int depth, bool multiple_writer_check>
  friend class osssi::osss_shared_variable_base;
  friend class osss_pipeline_stage;

  friend class osssi::osss_parallel_behaviour;
  friend class osssi::osss_sequential_behaviour;
  friend class osssi::osss_pipeline_behaviour;
  friend class osssi::osss_fsm_behaviour;

public:
  typedef sc_core::sc_module base_type;

  osss_behaviour();
  osss_behaviour(osss_behaviour_name name);

  virtual ~osss_behaviour();

  virtual void main();

  virtual const char* name() const { return base_type::name(); }
  virtual const char* kind() const { return "osss_behaviour"; }

  const char* type() const;

  osss_behaviour_node_type node_type() const;

  // operator used inside osss_par
  osss_parallel_behaviour_list& operator|( const osss_behaviour& ) const;
  // operator used inside osss_seq
  osss_sequential_behaviour_list& operator,( const osss_behaviour& ) const;

  void set_sensitivity(const sc_core::sc_event& event);
  void set_sensitivity(sc_core::sc_port_base& port_base);
  void set_sensitivity(sc_core::sc_interface& interface_p);
  void set_sensitivity(sc_core::sc_export_base& export_base);
  void set_sensitivity(sc_core::sc_event_finder& event_finder);
  
  bool has_sensitivity() const
    { return m_behaviour_options.has_sensitivity(); }

  void clear_sensitivity();
 
  void print_graph(const std::string& file_name, bool with_channels);

private:
  void print_behaviour(std::ofstream& ostr, 
                       const osss_behaviour* beh, 
                       unsigned int& indent,
                       bool with_channels);

  void print_channels(std::ofstream& ostr,
                      const osss_behaviour* beh,
                      unsigned int& indent);

protected: 
  void osss_par(const osss_behaviour& );
  void osss_par(const osss_parallel_behaviour_list& );

  void osss_seq_(const osss_behaviour& );
  void osss_seq_(const osss_sequential_behaviour_list& );

  void osss_pipe(const osss_pipeline_stage& );
  void osss_pipe(unsigned int count, const osss_pipeline_stage& );
  void osss_pipe(const osss_pipeline_behaviour_list& );
  void osss_pipe(unsigned int count, const osss_pipeline_behaviour_list& );
 
  virtual void init();
  virtual void final();
 
  virtual void pipe_pre();
  virtual void pipe_post();
  virtual bool pipe_stop_condition() const;
 
  void osss_fsm(const osssi::osss_state_transition& );
  void osss_fsm(const osssi::osss_state_transition_list& );
  
  void wait()
    { wait( osss_global_port_registry::get_clock_period() ); }

  void wait(int n)
    { wait( sc_core::sc_time(n * osss_global_port_registry::get_clock_period()) ); }

  void wait_delta()
    { sc_core::wait(sc_core::SC_ZERO_TIME); }

  void wait_delta(int n)
  {
    // CAUTION: This might be a simulation performance bottleneck !!!
    for(int i=0; i<n; ++i)
      sc_core::wait(sc_core::SC_ZERO_TIME);
  }

  // pass through to sc_core::sc_module since these wait statements overwrite 
  // the static sensitivity list
  void wait (const sc_event &e) 
    { base_type::wait(e); }

  void wait (sc_event_or_list &el) 
    { base_type::wait(el); }

  void wait (sc_event_and_list &el) 
    { base_type::wait(el); }

  void wait (const sc_time &t) 
    { base_type::wait(t); }

  void wait (double v, sc_time_unit tu) 
    { base_type::wait(v, tu); }

  void wait (const sc_time &t, const sc_event &e) 
    { base_type::wait(t, e); }

  void wait (double v, sc_time_unit tu, const sc_event &e) 
    { base_type::wait(v, tu, e); }

  void wait (const sc_time &t, sc_event_or_list &el) 
    { base_type::wait(t, el); }

  void wait (double v, sc_time_unit tu, sc_event_or_list &el) 
    { base_type::wait(v, tu, el); }

  void wait (const sc_time &t, sc_event_and_list &el) 
    { base_type::wait(t, el); }

  void wait (double v, sc_time_unit tu, sc_event_and_list &el) 
    { base_type::wait(v, tu, el); }
 
  SC_HAS_PROCESS(osss_behaviour);

  void pipeline_step();
  virtual void before_end_of_elaboration();
  virtual void end_of_elaboration();
   
private:
   sc_spawn_options* get_spawn_options()
     { return m_behaviour_options.get_spawn_options(); }

private:
  osssi::osss_composite_behaviour* m_composite_beh;
  osss_behaviour_node_type         m_behaviour_type;
  std::vector<osss_behaviour*>     m_behaviour_list;

  osssi::osss_behaviour_options    m_behaviour_options;
  bool                             m_sensitivity_cleared;

private:
  osss_behaviour(const osss_behaviour& beh);
  osss_behaviour& operator=(const osss_behaviour& beh);
};

} // namespace osss

#define OSSS_BEHAVIOUR(user_behaviour_name) \
  struct user_behaviour_name : ::osss::osss_behaviour

#define BEHAVIOUR_CTOR(user_behaviour_name) \
  typedef user_behaviour_name OSSS_CURRENT_USER_BEHAVIOUR; \
  user_behaviour_name( ::osss::osss_behaviour_name )

// the OSSS_HAS_BEHAVIOUR macro call must be followed by a ;
#define OSSS_HAS_BEHAVIOUR(user_behaviour_name) \
  typedef user_behaviour_name OSSS_CURRENT_USER_BEHAVIOUR

//
// This is a dirty hack to get all arguments of osss_seq
// in parentheses. It is required for the correct evaluation
// of the operator,(...) used to build the osss_sequential_behaviour_list
//
#define osss_seq( ... ) osss_seq_( ( __VA_ARGS__ ) )

/*
#define osss_cond( cond ) \
   ::osss::osssi::osss_condition_wrapper< \
     typeof(sc_bind(&OSSS_CURRENT_USER_BEHAVIOUR::cond, this)) > \
       (sc_bind(&OSSS_CURRENT_USER_BEHAVIOUR::cond, this))
*/

#define osss_cond( cond ) \
   ::osss::osssi::osss_condition_wrapper<typeof(sc_bind(&cond, this))> \
       (sc_bind(&cond, this), #cond)

#define OSSS_DECLARE_STATE(statetype, statename) \
   statename = new statetype( #statename );

#define OSSS_OVERWRITE_STATE(statetype, statename) \
   if (statename != NULL) \
      delete statename; \
   statename = new statetype( #statename );

#endif
