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

#ifndef OSSS_SIGNAL_EVENT_CONVERTER_CHANNEL_INCLUDED
#define OSSS_SIGNAL_EVENT_CONVERTER_CHANNEL_INCLUDED

#include "osss_event_ifs.h"

namespace osss {

class osss_signal_event_converter_channel :
  public osss_event_in_if,
  public osss::osss_event
{
  public :
    typedef osss::osss_event base_type;

  SC_HAS_PROCESS(osss_signal_event_converter_channel);

  explicit osss_signal_event_converter_channel( 
    const sc_core::sc_signal_in_if<bool>& iface,
    bool level,
    sc_core::sc_module_name name = 
    sc_core::sc_module_name(
      sc_core::sc_gen_unique_name("osss_signal_event_converter_channel")) ) :
    osss::osss_event(name)
  {
    SC_METHOD(converter_proc);
    dont_initialize();
    if(level)
      sensitive << iface.posedge_event();
    else
      sensitive << iface.negedge_event();
  }
   
  virtual ~osss_signal_event_converter_channel() {}

  virtual const char * kind() const 
    { return "osss_signal_event_converter_channel"; }

  virtual const osss_event* get_event() const 
    { return this; };

  virtual operator const osss_event& () const
    { return *this; }

  virtual sc_core::sc_event_expr<sc_event_or_list> 
    operator|(const sc_core::sc_event& ev) const
    { return base_type::operator|(ev); }
  
  virtual sc_core::sc_event_expr<sc_event_and_list>
    operator&(const sc_core::sc_event& ev) const
    { return base_type::operator&(ev); }

 protected:
  void converter_proc()
  {
    base_type::notify();
  }
   
 private:
  osss_signal_event_converter_channel();

  osss_signal_event_converter_channel
    ( const osss_signal_event_converter_channel& );

  osss_signal_event_converter_channel& operator=
    ( const osss_signal_event_converter_channel& );    
};

} // namespace osss

#endif
