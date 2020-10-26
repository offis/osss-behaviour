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

#ifndef OSSS_PIPELINE_BEH_INCLUDED
#define OSSS_PIPELINE_BEH_INCLUDED

#include "osss_composite_beh.h"

namespace osss {

//forward declarations
class osss_pipeline_stage;
class osss_pipeline_behaviour_list;

namespace osssi {

//forward declaration
template<typename T, unsigned int depth, bool multiple_writer_check>
class osss_shared_variable_base;

//--------------------------------------------------------------------
// osss pipeline behaviour
//--------------------------------------------------------------------
class osss_pipeline_behaviour : public osss_composite_behaviour
{
 public:

  explicit osss_pipeline_behaviour(osss_behaviour* beh_ptr);

  virtual const char* kind () const 
    { return "osss_pipeline_behaviour"; }

  virtual void main();

  sc_core::sc_event& step_event();

  void step_proc();

  bool is_stop() const;
  unsigned int get_cycle_count() const;
 
protected:
  void evaluate_stop_condition();

  bool              m_stop;
  unsigned int      m_cycle_count;
  sc_core::sc_event m_step_event;

private:
  osss_pipeline_behaviour();
  osss_pipeline_behaviour(const osss_pipeline_behaviour&);
  osss_pipeline_behaviour& operator=(const osss_pipeline_behaviour&);

};

} // namespace osssi

//--------------------------------------------------------------------
// osss pipeline stage
//--------------------------------------------------------------------

class osss_pipeline_stage : public osss_behaviour
{
 public:

  osss_pipeline_stage();
  osss_pipeline_stage(osss_behaviour_name name);

  void _main_wrapper();

  virtual const char* kind () const
    { return "osss_pipeline_stage"; }

  osss_pipeline_behaviour_list& operator >> ( const osss_pipeline_stage& ) const;

  void stage_nr(int nr);
  void num_cycles(unsigned int nr);
  sc_core::sc_event& ready_event();
  sc_core::sc_event& complete_event();

  bool is_finished() const;
  bool is_scheduled() const;

  void die();
 
 protected:

  virtual void end_of_elaboration();

 protected:
  int m_stage_nr; // -1 = pipeline stage has not been scheduled for execution
  unsigned int m_num_cycles;
  osssi::osss_pipeline_behaviour* m_pipeline_behaviour;

  sc_core::sc_event m_ready_event;
  sc_core::sc_event m_complete_event;
  bool m_finished;
  bool m_die;

};

#define OSSS_PIPELINE_STAGE(user_behaviour_name) \
  struct user_behaviour_name : ::osss::osss_pipeline_stage

#define PIPELINE_STAGE_CTOR(user_behaviour_name) \
  typedef user_behaviour_name OSSS_CURRENT_USER_BEHAVIOUR; \
  user_behaviour_name( ::osss::osss_behaviour_name )

} // namespace osss

//#include "osss_pipeline_beh.tpp"

#endif
