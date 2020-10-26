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

#ifndef OSSS_MODULE_INCLUDED
#define OSSS_MODULE_INCLUDED

#include "osss_except.h"
#include "osss_signal_event_converter_channel.h"
#include <map>
#include <list>

namespace osss {
namespace osssi {

/**
 * \class osss_reset_event_finder
 *
 */
class osss_reset_event_finder 
{
 public:
  osss_reset_event_finder() :
    m_proc_name(NULL),
    m_ev_in(NULL)
    {}

  explicit osss_reset_event_finder(const char* proc_name, 
                                   const osss_event_in* ev_in) :
    m_proc_name(proc_name),
    m_ev_in(ev_in)
    {}

  const char* proc_name() const { return m_proc_name; }
  const osss_event_in* ev_in() const { return m_ev_in; }
      
 protected:
  const char*          m_proc_name;
  const osss_event_in* m_ev_in;
};

/**
 * \class osss_reset_signal_finder
 *
 */
class osss_reset_signal_finder 
{
 public:
  osss_reset_signal_finder() :
    m_proc_name(NULL),
    m_port_in(NULL),
    m_level(false)
    {}

  explicit osss_reset_signal_finder(const char* proc_name, 
                                    const sc_core::sc_in<bool>* port_in,
                                    bool level) :
    m_proc_name(proc_name),
    m_port_in(port_in),
    m_level(level)
    {}

  const char* proc_name() const { return m_proc_name; }
  const sc_core::sc_in<bool>* port_in() const { return m_port_in; }
  bool level() const { return m_level; }
      
 protected:
  const char*                 m_proc_name;
  const sc_core::sc_in<bool>* m_port_in;
  bool                        m_level;
};

} // namespace osssi

  // forward declarations
  void wait( sc_core::sc_simcontext* );
  void wait( const sc_core::sc_event&, sc_core::sc_simcontext* );
  void wait( sc_core::sc_event_or_list&, sc_core::sc_simcontext* );
  void wait( sc_core::sc_event_and_list&, sc_core::sc_simcontext* );
  void wait( const sc_core::sc_time&, sc_core::sc_simcontext* );
  void wait( const sc_core::sc_time&, const sc_core::sc_event&,
             sc_core::sc_simcontext* );
  void wait( const sc_core::sc_time&, sc_core::sc_event_or_list&,
             sc_core::sc_simcontext* );
  void wait( const sc_core::sc_time&, sc_core::sc_event_and_list&,
             sc_core::sc_simcontext* );
   
typedef sc_core::sc_module_name osss_module_name;

/**
 * \class osss_module
 *
 */
class osss_module : public sc_core::sc_module
{
 public:
  typedef std::pair<const osss_event_in_if*, sc_dt::uint64> reset_reg_t;
  typedef std::map<const char*, reset_reg_t >               reset_lookup_t;

  typedef std::list<osssi::osss_reset_event_finder*> reset_event_finder_list_t;
  typedef std::list<osssi::osss_reset_signal_finder*> reset_signal_finder_list_t;
  
  typedef std::vector<osss_signal_event_converter_channel*> 
    signal_event_converter_channel_vec_t;

  virtual const char* kind() const
    { return "osss_module"; }

 protected:
   
  // constructor
  osss_module( const char* nm ) : 
    sc_core::sc_module(nm),
    m_reset_event_finder_list(),
    m_reset_signal_finder_list(),
    m_signal_event_converter_channel_vec()
  {}

  osss_module( const std::string& nm ) : 
    sc_core::sc_module(nm),
    m_reset_event_finder_list(),
    m_reset_signal_finder_list(),
    m_signal_event_converter_channel_vec()
  {}
  
  osss_module( const osss_module_name& nm ) : 
    sc_core::sc_module(nm),
    m_reset_event_finder_list(),
    m_reset_signal_finder_list(),
    m_signal_event_converter_channel_vec()
  {}
  
  osss_module() : 
    sc_core::sc_module()
  {}

 public:
  // destructor
  virtual ~osss_module();

 protected:

  //
  // reset event registration
  //
  // TODO: - enable more than a single event registration ???
  void reset_event_is( const osss_event_in& ev_in );
  void reset_event_is( const osss_event_in_if& ev_in_if );

  //
  // signal registration
  //
  void reset_signal_is(const sc_core::sc_in<bool>& port, bool level);
  void reset_signal_is(const sc_core::sc_signal_in_if<bool>& iface, bool level);

  // TODO
  //void dont_initialize();
  
  // Former static sensitivity for SC_THREADs and SC_CTHREADs
  // Now we forbid these "old" waits by mapping them on dynamic
  // sensitivities using a used-defined clock period

  void wait()
    { this->wait( osss_global_port_registry::get_clock_period() ); }

  void wait(int n)
    { this->wait( 
      sc_core::sc_time(n * osss_global_port_registry::get_clock_period()) ); }

  // dynamic sensitivity for SC_THREADs and SC_CTHREADs

  void wait( const sc_core::sc_event& e )
    { ::osss::wait( e, simcontext() ); }
   
  void wait( sc_core::sc_event_or_list& el )
    { ::osss::wait( el, simcontext() ); }
 
  void wait( sc_core::sc_event_and_list& el )
    { ::osss::wait( el, simcontext() ); }

  void wait( const sc_core::sc_time& t )
    { ::osss::wait( t, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu )
    { this->wait(sc_core::sc_time(v, tu)); }

  void wait( const sc_core::sc_time& t, const sc_core::sc_event& e )
    { ::osss::wait( t, e, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu, const sc_core::sc_event& e )
    { this->wait(sc_core::sc_time(v, tu), e); }

  void wait( const sc_core::sc_time& t, sc_core::sc_event_or_list& el );
 
  void wait( double v, sc_core::sc_time_unit tu, sc_core::sc_event_or_list& el )
    { this->wait(sc_core::sc_time(v, tu), el); }

  void wait( const sc_core::sc_time& t, sc_core::sc_event_and_list& el )
    { ::osss::wait( t, el, simcontext() ); }

  void wait( double v, sc_core::sc_time_unit tu, sc_core::sc_event_and_list& el )
    { this->wait(sc_core::sc_time(v, tu), el); }

  virtual void end_of_elaboration();
  
 public:
  static reset_lookup_t reset_lookup;

 protected:
  reset_event_finder_list_t            m_reset_event_finder_list;
  reset_signal_finder_list_t           m_reset_signal_finder_list;
  signal_event_converter_channel_vec_t m_signal_event_converter_channel_vec;

 private:
  // disabled
  osss_module( const osss_module& );
};

// -----------------------------------------------------------------------------
// helper classes that are used in conjunction with boost_bind to
// register the user-defined thread
// -----------------------------------------------------------------------------

namespace osssi {

class osss_main_routine_runner
{
 public:
  virtual void run_user_code() = 0;
  virtual ~osss_main_routine_runner() {}
};

template<class T>
class osss_main_routine_wrapper : public osss_main_routine_runner
{
 public:
  explicit osss_main_routine_wrapper(T object) :
    m_object(object)
  {}
  
  virtual ~osss_main_routine_wrapper() {}

  virtual void run_user_code() { m_object(); }

 protected:
  T m_object;
 
 private:
  osss_main_routine_wrapper();
};

extern
void handle_reset( const osss_module::reset_lookup_t::iterator& rlookup_iter );

// this is the main routine that embedds the user-defined main mathod
// and implements the reset behaviour
extern
void osss_module_main_routine(osss_main_routine_runner& runner);

} // namespace osssi

// -----------------------------------------------------------------------------
// SOME MACROS TO SIMPLIFY SYNTAX:
// -----------------------------------------------------------------------------

#define OSSS_MODULE(user_module_name)                                         \
    struct user_module_name : ::osss::osss_module

#define OSSS_CTOR(user_module_name)                                           \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    user_module_name( ::osss::osss_module_name )

// the OSSS_HAS_PROCESS macro call must be followed by a ;
#define OSSS_HAS_PROCESS(user_module_name)                                    \
    typedef user_module_name SC_CURRENT_USER_MODULE

#define OSSS_THREAD( user_main )                                              \
  {                                                                           \
    ::osss::osssi::osss_main_routine_runner* mr =                             \
      new ::osss::osssi::osss_main_routine_wrapper<                           \
        typeof(sc_bind(& SC_CURRENT_USER_MODULE :: user_main, this))>         \
      (sc_bind(& SC_CURRENT_USER_MODULE :: user_main, this));                 \
     sc_core::sc_spawn(                                                       \
      sc_bind(&::osss::osssi::osss_module_main_routine, sc_ref(*mr)));        \
  }

// ----------------------------------------------------------------------------
//  TYPEDEFS for osss_channel (hierachical channels in SpecC)
// ----------------------------------------------------------------------------

typedef osss_module      osss_channel;
typedef osss_module_name osss_channel_name;

} // namespace osss

#endif
