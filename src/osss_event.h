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

#ifndef OSSS_EVENT_INCLUDED
#define OSSS_EVENT_INCLUDED

namespace osss {

class osss_event : 
  public sc_core::sc_event,
  public sc_core::sc_module
{
public:

  SC_HAS_PROCESS( osss_event );

  osss_event( sc_core::sc_module_name name = 
              sc_core::sc_module_name(
                sc_core::sc_gen_unique_name("osss_event")) ) :
    sc_core::sc_event(),
    sc_core::sc_module(name),
    m_last_notified(0),
    m_scheduled_time()
  {
    SC_METHOD( register_event );
    sensitive << *this;
    dont_initialize();
  }
  
  ~osss_event() {}
  
  inline virtual const char* kind() const { return "osss_event"; }

  void cancel()
    { sc_core::sc_event::cancel(); }

  void notify()
    { sc_core::sc_event::notify(); }

  void notify( const sc_core::sc_time& t )
    { sc_core::sc_event::notify(t); }

  void notify( double val, sc_core::sc_time_unit tu )
    { this->notify(sc_core::sc_time(val, tu)); }

  void notify_delayed()
    { sc_core::sc_event::notify_delayed(); }
  
  void notify_delayed( const sc_core::sc_time& t )
    { sc_core::sc_event::notify_delayed(t); }

  void notify_delayed( double val, sc_core::sc_time_unit tu )
    { this->notify_delayed(sc_core::sc_time(val, tu)); }

  sc_dt::uint64 last_notified() const
    { return m_last_notified; }

  const sc_core::sc_time& scheduled_time() const
    { return m_scheduled_time; }

  osss_event& ref() const
    { return *(const_cast<osss_event*>(this)); }

  const osss_event& const_ref() const
    { return *this; }

 protected:
  void register_event()
  {
    m_last_notified = sc_core::sc_delta_count();
    m_scheduled_time = sc_core::sc_time_stamp();
  }

 protected:
  sc_dt::uint64    m_last_notified;
  sc_core::sc_time m_scheduled_time;

 private:
  // disabled
  osss_event( const osss_event& );
  osss_event& operator=( const osss_event& );
};

} // namespace osss

#endif
