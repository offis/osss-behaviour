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

#include "osss_tracing.h"

#include "osss_clock_reset_if.h"
#include "osss_composite_beh.h"
#include "osss_beh.h"
#include "osss_composite_beh.h"
#include "osss_event_list.h"
#include "osss_pipeline_beh.h"

namespace osss {

namespace osssi {

//--------------------------------------------------------------------
// osss pipeline behaviour
//--------------------------------------------------------------------
osss_pipeline_behaviour::osss_pipeline_behaviour(osss_behaviour* beh_ptr) :
  osss_composite_behaviour(beh_ptr),
  m_stop(false),
  m_cycle_count(0),
  m_step_event()
{}

void osss_pipeline_behaviour::evaluate_stop_condition()
{ 
  if (!m_stop)
    m_stop = this->behaviour()->pipe_stop_condition();
}

sc_core::sc_event& osss_pipeline_behaviour::step_event() 
{ 
  return m_step_event; 
}

void
osss_pipeline_behaviour::main()
{
  // call initialization action
  this->behaviour()->init();

  // FORK
  std::vector<sc_core::sc_process_handle> forkees;
  for(unsigned int i=0; i<m_behaviour_list.size(); ++i)
  {
    forkees.push_back( 
      sc_core::sc_spawn(
        sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, 
                dynamic_cast<osss_pipeline_stage*>(m_behaviour_list[i])),
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

void osss_pipeline_behaviour::step_proc()
{
  // build ready & complete event list
  osssi::osss_event_and_list 
    ready_event_list(
      dynamic_cast<osss_pipeline_stage*>(m_behaviour_list[0])->
      ready_event());
  
  osssi::osss_event_and_list 
    complete_event_list(
      dynamic_cast<osss_pipeline_stage*>(m_behaviour_list[0])->
      complete_event());
  for(unsigned int i=1; i<m_behaviour_list.size(); ++i)
  {
    ready_event_list.add_event(
      dynamic_cast<osss_pipeline_stage*>(m_behaviour_list[i])->
      ready_event());
    
    complete_event_list.add_event(
      dynamic_cast<osss_pipeline_stage*>(m_behaviour_list[i])->
      complete_event());
  }

  while(true)
  {
    // (re)init internals
    m_stop = false;
    m_cycle_count = 0;
    
    //wait for all pipeline stages to be ready for the first execution
    wait(ready_event_list);
    
    // all pipeline stages are ready to run -> go for it!
    while(true)
    {
      // call pipeline pre-action
      this->behaviour()->pipe_pre();

      // evaluate stop condition
      this->evaluate_stop_condition(); 
      if(!is_stop())
      {
        m_cycle_count++;
      }

      // all pipeline stages perform a single execution step 
      // (i.e. are executed once)
      m_step_event.notify(sc_core::SC_ZERO_TIME);

      // wait for completion of all pipeline stages
      wait(complete_event_list);
      
      // call pipeline post-action
      this->behaviour()->pipe_post();
    
      // check whether all pipeline stages have finished during their last
      // execution cycle
      bool finished = true;
      std::vector<osss_behaviour*>::iterator it;
      for(it = m_behaviour_list.begin(); 
          it != m_behaviour_list.end(); 
          it++)
      {
        if(!dynamic_cast<osss_pipeline_stage*>(*it)->is_finished())
        {
          finished = false;
          break;
        }
      }

      // when all pipeline stages have finished: let them die and quit
      if(finished)
      {
        for(it = m_behaviour_list.begin(); 
            it != m_behaviour_list.end(); 
            it++)
          dynamic_cast<osss_pipeline_stage*>(*it)->die();
      
        // final step event
        m_step_event.notify(sc_core::SC_ZERO_TIME);
      
        // wait for all pipeline stage death (i.e. their last complete_event)
        wait(complete_event_list);
        
        break; // inner while loop
      }
    }
  }
}

bool osss_pipeline_behaviour::is_stop() const 
{ return m_stop; }

unsigned int osss_pipeline_behaviour::get_cycle_count() const
{ return m_cycle_count; }

} // namespace osssi

//--------------------------------------------------------------------
// osss pipeline stage
//--------------------------------------------------------------------

osss_pipeline_stage::osss_pipeline_stage() : 
  osss_behaviour(),
  m_stage_nr(-1),
  m_num_cycles(0),
  m_pipeline_behaviour(NULL),
  m_ready_event(),
  m_complete_event(),
  m_finished(false),
  m_die(false)
{}

osss_pipeline_stage::osss_pipeline_stage(osss_behaviour_name name) :
  osss_behaviour( name ),
  m_stage_nr(-1),
  m_num_cycles(0),
  m_pipeline_behaviour(NULL),
  m_ready_event(),
  m_complete_event(),
  m_finished(false),
  m_die(false)
{}

void osss_pipeline_stage::_main_wrapper()
{
  // (re)init
  m_finished = false;
  m_die = false;

  unsigned int cycle_counter = 0;
  unsigned int num_cycles = m_num_cycles;

  // notify that we are ready to go!
  m_ready_event.notify(sc_core::SC_ZERO_TIME);

  // wait m_stage_nr step events until execution of main starts
  for(int i=0; i<m_stage_nr; ++i)
  {
    wait(m_pipeline_behaviour->step_event());
    // do nothing
    m_complete_event.notify(sc_core::SC_ZERO_TIME);
  }

  // run user's main until num_cycles step events have passed
  while(true)
  {
    //OSSS_PRINT_TIME_STAMP();
    wait(m_pipeline_behaviour->step_event());

    // call user-defined main behaviour
    main();
       
    cycle_counter++;

    if (num_cycles > 0)
    {
      // use cycle count as stop condition
      if (cycle_counter >= num_cycles)
        m_finished = true;
    }
    else
    {
      // use user defined stop_condition method
      if (m_pipeline_behaviour->is_stop())
      {
        // we are the primal stage -> quit immediately
        if (m_stage_nr == 0)
        {
          m_finished = true;
        }
        // keep on running for the same amount of cycles as the primal stage did
        else
        {
          num_cycles = m_pipeline_behaviour->get_cycle_count() + 1;
        }
      }
    }
    m_complete_event.notify(sc_core::SC_ZERO_TIME);
    if (m_finished)
      break;
  }
  
  // execution of this pipeline stage is over:
  // wait for leaving _main_wrapper
  while(true)
  {
    wait(m_pipeline_behaviour->step_event());
    m_complete_event.notify(sc_core::SC_ZERO_TIME);
    if (m_die)
      break;
  }
}

void osss_pipeline_stage::stage_nr(int nr) { m_stage_nr = nr; }

void osss_pipeline_stage::num_cycles(unsigned int nr) { m_num_cycles = nr; }

sc_core::sc_event& osss_pipeline_stage::ready_event()
{
  return m_ready_event;
}

sc_core::sc_event& osss_pipeline_stage::complete_event() 
{ 
  return m_complete_event; 
}

bool osss_pipeline_stage::is_finished() const { return m_finished; }

bool osss_pipeline_stage::is_scheduled() const { return m_stage_nr != -1; }

void osss_pipeline_stage::die() { m_die = true; }

void osss_pipeline_stage::end_of_elaboration()
{
  // call end_of_elaboration of osss::osss_behaviour to force
  // the propagation of sensitivities
  osss::osss_behaviour::end_of_elaboration();

  // check whether the parent behaviour of a pipeline stage
  // is a pipeline behaviour
  sc_core::sc_object* parent_obj = this->get_parent_object();
  osss_behaviour* beh = dynamic_cast<osss_behaviour*>(parent_obj);
  if (beh && (beh->node_type() == OSSS_PIPE_BEHAVIOUR))
  {
    osssi::osss_pipeline_behaviour* pipe_beh = 
      dynamic_cast<osssi::osss_pipeline_behaviour*>(beh->m_composite_beh);
    if(pipe_beh)
    {
      m_pipeline_behaviour = pipe_beh;
    }
  }
  else
  {
    std::cerr << "Error: parent behaviour of pipeline stage "
              << this->name() << " is not a pipeline behaviour!" 
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

} // namespace osss

