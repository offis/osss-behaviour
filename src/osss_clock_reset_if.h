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

#ifndef OSSS_CLOCK_RESET_IF_INCLUDED
#define OSSS_CLOCK_RESET_IF_INCLUDED

#include "osss_global_port_registry.h"

namespace osss {
namespace osssi {

class osss_clock_reset_if
{
 public:

  osss_clock_reset_if() : 
    clock("clock_port"),
    reset("reset_port"),
    m_clock_port_bound(false),
    m_reset_port_bound(false)
  {}

  virtual ~osss_clock_reset_if() {}
  
  void clock_port(const sc_core::sc_signal_in_if<bool>& sif)
  {
    clock(sif);
    m_clock_port_bound = true;
  }
  
  void clock_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& port)
  {
    clock(port);
    m_clock_port_bound = true;
  }

  void clock_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& port)
  {
    clock(port);
    m_clock_port_bound = true;
  }

  void reset_port(const sc_core::sc_signal_in_if<bool>& sif)
  {
    reset(sif);
    m_reset_port_bound = true;
  }

  void reset_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& port)
  {
    reset(port);
    m_reset_port_bound = true;
  }

  void reset_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& port)
  {
    reset(port);
    m_reset_port_bound = true;
  }

  virtual const char * kind() const = 0;
  virtual const char * name() const = 0;

 protected:

  void check_clock_reset_port()
  {
    if (! m_clock_port_bound)
    {
      if (osss_global_port_registry::is_clock_port_registered())
        this->clock_port(osss_global_port_registry::get_clock_port());
      else
      {
        std::cerr << "Error: Clock port " << clock.name()
                  << " of " << name()
                  << " (" << kind() << ") is not bound." 
                  << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    if(! m_reset_port_bound)
    {
      if (osss_global_port_registry::is_reset_port_registered())
        this->reset_port(osss_global_port_registry::get_reset_port());
      else
      {
        std::cerr << "Error: Reset port " << reset.name()
                  << " of " << name()
                  << " (" << kind() << ") is not bound."
                  << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  sc_core::sc_in<bool>  clock;
  sc_core::sc_in<bool>  reset;

 private:

  osss_clock_reset_if(const osss_clock_reset_if& clk_rst_if);
  osss_clock_reset_if& operator=(const osss_clock_reset_if& clk_rst_if);

  bool m_clock_port_bound;
  bool m_reset_port_bound;
};

} // namespace osssi

} // namespace osss

#endif
