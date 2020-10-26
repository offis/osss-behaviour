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

#ifndef OSSS_PIPELINE_MACROS_INCLUDED
#define OSSS_PIPELINE_MACROS_INCLUDED

#ifdef USE_OSSS_HAS_PIPELINE_STAGES_MACRO

#define _OSSS_HAS_PIPELINE_STAGES0()

#define _OSSS_HAS_PIPELINE_STAGES1( st1 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished()) \
      { \
        st1.die(); \
        break; \
      } \
      wait(st1.complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES2( st1, st2 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished()) \
      { \
        st1.die(); \
        st2.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES3( st1, st2, st3 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished() && \
          st3.is_finished()) \
      { \
        st1.die(); \
        st2.die(); \
        st3.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event() & \
           st3 .complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES4( st1, st2, st3, st4 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished() && \
          st3.is_finished() && \
          st4.is_finished()) \
      { \
        st1.die(); \
        st2.die(); \
        st3.die(); \
        st4.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event() & \
           st3 .complete_event() & \
           st4 .complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES5( st1, st2, st3, st4, st5 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished() && \
          st3.is_finished() && \
          st4.is_finished() && \
          st5.is_finished()) \
      { \
        st1.die(); \
        st2.die(); \
        st3.die(); \
        st4.die(); \
        st5.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event() & \
           st3 .complete_event() & \
           st4 .complete_event() & \
           st5 .complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES6( st1, st2, st3, st4, st5, st6 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished() && \
          st3.is_finished() && \
          st4.is_finished() && \
          st5.is_finished() && \
          st6.is_finished()) \
      { \
        st1.die(); \
        st2.die(); \
        st3.die(); \
        st4.die(); \
        st5.die(); \
        st6.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event() & \
           st3 .complete_event() & \
           st4 .complete_event() & \
           st5 .complete_event() & \
           st6 .complete_event()); \
      m_step_event.notify(); \
    } \
  }

#define _OSSS_HAS_PIPELINE_STAGES7( st1, st2, st3, st4, st5, st6, st7 ) \
  virtual void _step_proc() \
  { \
    while(true) \
    { \
      if (st1.is_finished() && \
          st2.is_finished() && \
          st3.is_finished() && \
          st4.is_finished() && \
          st5.is_finished() && \
          st6.is_finished() && \
          st7.is_finished()) \
      { \
        if ( OSSS_PIPELINE_BEHAVIOUR_DEBUG ) \
        { \
          std::cout << "@ " << sc_core::sc_time_stamp() \
                    << " (" << sc_core::sc_delta_count() << ") : " \
                    << __PRETTY_FUNCTION__ << " finished." << std::endl; \
        } \
        st1.die(); \
        st2.die(); \
        st3.die(); \
        st4.die(); \
        st5.die(); \
        st6.die(); \
        st7.die(); \
        break; \
      } \
      wait(st1 .complete_event() & \
           st2 .complete_event() & \
           st3 .complete_event() & \
           st4 .complete_event() & \
           st5 .complete_event() & \
           st6 .complete_event() & \
           st7 .complete_event()); \
      if( OSSS_PIPELINE_BEHAVIOUR_DEBUG ) \
      { \
        std::cout << std::endl; \
        std::cout << "@ " << sc_core::sc_time_stamp() \
                  << " (" << sc_core::sc_delta_count() << ") : " \
                  << __PRETTY_FUNCTION__ << " step event!" << std::endl; \
        std::cout << std::endl; \
      } \
      m_step_event.notify(); \
    } \
  }

//
// ... and so on ...
//

#define OSSS_HAS_PIPELINE_STAGES(classname, N, ...) \
  typedef classname _this_type; \
  _OSSS_HAS_PIPELINE_STAGES##N( __VA_ARGS__ )

#endif

//--------------------------------------------------------------------

#define _OSSS_PIPE0()

#define _OSSS_PIPE1( count, st1 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  OSSS_FORK \
   sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)) \
  OSSS_JOIN

#define _OSSS_PIPE2( count, st1, st2 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2))  \
  OSSS_JOIN

#define _OSSS_PIPE3( count, st1, st2, st3 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  st3 .num_cycles( count ); \
  st3 .stage_nr(2); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st3))  \
  OSSS_JOIN 

#define _OSSS_PIPE4( count, st1, st2, st3, st4 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  st3 .num_cycles( count ); \
  st3 .stage_nr(2); \
  st4 .num_cycles( count ); \
  st4 .stage_nr(3); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st3)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st4))  \
  OSSS_JOIN

#define _OSSS_PIPE5( count, st1, st2, st3, st4, st5 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  st3 .num_cycles( count ); \
  st3 .stage_nr(2); \
  st4 .num_cycles( count ); \
  st4 .stage_nr(3); \
  st5 .num_cycles( count ); \
  st5 .stage_nr(4); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st3)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st4)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st5))  \
  OSSS_JOIN 

#define _OSSS_PIPE6( count, st1, st2, st3, st4, st5, st6 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  st3 .num_cycles( count ); \
  st3 .stage_nr(2); \
  st4 .num_cycles( count ); \
  st4 .stage_nr(3); \
  st5 .num_cycles( count ); \
  st5 .stage_nr(4); \
  st6 .num_cycles( count ); \
  st6 .stage_nr(5); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st3)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st4)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st5)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st6))  \
  OSSS_JOIN

#define _OSSS_PIPE7( count, st1, st2, st3, st4, st5, st6, st7 ) \
  st1 .num_cycles( count ); \
  st1 .stage_nr(0); \
  st2 .num_cycles( count ); \
  st2 .stage_nr(1); \
  st3 .num_cycles( count ); \
  st3 .stage_nr(2); \
  st4 .num_cycles( count ); \
  st4 .stage_nr(3); \
  st5 .num_cycles( count ); \
  st5 .stage_nr(4); \
  st6 .num_cycles( count ); \
  st6 .stage_nr(5); \
  st7 .num_cycles( count ); \
  st7 .stage_nr(6); \
  OSSS_FORK \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st1)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st2)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st3)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st4)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st5)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st6)), \
    sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, &st7))  \
  OSSS_JOIN
  
//
// ... and so on ...
//

#define OSSS_PIPE(N, ...) \
  _OSSS_PIPE##N( 0, __VA_ARGS__ )

#define OSSS_PIPE_COUNT(count, N, ...) \
  _OSSS_PIPE##N(count, __VA_ARGS__ )

namespace osss {

namespace osssi {

inline void osss_pipe_helper( unsigned int count, unsigned int amount, va_list argument_list )
{
  // FORK
  std::vector<sc_core::sc_process_handle> forkees;
  for(unsigned int i=0; i<amount; ++i)
  {
    void* vp = va_arg(argument_list, void*);
    ::osss::osss_pipeline_stage* pst = 
        (::osss::osss_pipeline_stage*)(vp);
    pst->num_cycles(count);
    pst->stage_nr(i);
    forkees.push_back( 
      sc_core::sc_spawn(sc_bind(&::osss::osss_pipeline_stage::_main_wrapper, pst)));    
  }

  // JOIN
  sc_core::sc_join join;
  for ( unsigned int i=0; i<amount; ++i)
    join.add_process(forkees[i]);
  sc_core::sc_process_handle cph = sc_core::sc_get_current_process_handle();
  if (cph.proc_kind() == sc_core::SC_THREAD_PROC_)
    join.wait();
  else if (cph.proc_kind() == sc_core::SC_CTHREAD_PROC_)
    join.wait_clocked();
}

} // namespace osssi

// same as OSSS_PIPE_COUNT(count, N, ...) macro above using ellipse (...) 
// operator
//
// needs to be called ::osss::osss_pipe_count(10, 3, &b1, &b2, &b3);
// the address (&) operator is needed to get a raw void* 
//
inline void osss_pipe_count( unsigned int count, unsigned int amount, ... )
{
  va_list argument_list;
  va_start(argument_list, amount);
  osssi::osss_pipe_helper(count, amount, argument_list);
  va_end(argument_list);
}

// same as OSSS_PIPE(N, ...) macro above using ellipse (...) operator
//
// needs to be called ::osss::osss_pipe(3, &b1, &b2, &b3);
// the address (&) operator is needed to get a raw void* 
//
inline void osss_pipe( unsigned int amount, ... )
{
  va_list argument_list;
  va_start(argument_list, amount);
  osssi::osss_pipe_helper(0, amount, argument_list);
  va_end(argument_list);
}

} // namespace osss

#endif
