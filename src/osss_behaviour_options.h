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

#ifndef OSSS_BEHAVIOUR_OPTIONS_INCLUDED
#define OSSS_BEHAVIOUR_OPTIONS_INCLUDED

namespace osss {

namespace osssi {

class osss_behaviour_options 
{
 public:
  osss_behaviour_options() :                  
    m_dont_initialize(false), 
    m_stack_size(0)
  {}

  osss_behaviour_options( const osss_behaviour_options& bops )
  {
    *this = bops; // use assignment operator
  }
  
  osss_behaviour_options& operator=( const osss_behaviour_options& bops )
  {
    // check for self-assignment
    if (&bops == this)
      return *this;
	
    // copy everything except m_spawn_options
    this->m_dont_initialize = bops.m_dont_initialize;
    this->m_sensitive_events = bops.m_sensitive_events;
    this->m_sensitive_event_finders = bops.m_sensitive_event_finders; 
    this->m_sensitive_interfaces = bops.m_sensitive_interfaces;
    this->m_sensitive_port_bases = bops.m_sensitive_port_bases;
    this->m_stack_size = bops.m_stack_size;
  
    return *this;
  }

  void dont_initialize() 
    { m_dont_initialize = true; }
  
  void set_stack_size(int stack_size) 
    { m_stack_size = stack_size; }
  
  void set_sensitivity(const sc_event* event) 
    { m_sensitive_events.push_back(event); }
  void set_sensitivity(sc_port_base* port_base)
    { m_sensitive_port_bases.push_back(port_base); }
  void set_sensitivity(sc_interface* interface_p) 
    { m_sensitive_interfaces.push_back(interface_p); }
  void set_sensitivity(sc_export_base* export_base) 
    { m_sensitive_interfaces.push_back(export_base->get_interface()); }
  void set_sensitivity(sc_event_finder* event_finder) 
    { m_sensitive_event_finders.push_back(event_finder); }

  bool has_sensitivity() const
  { 
    return !(m_sensitive_events.empty() &&
             m_sensitive_event_finders.empty() &&
             m_sensitive_interfaces.empty() &&
             m_sensitive_port_bases.empty());
  }

  void clear_sensitivity()
  {
    m_sensitive_events.clear();
    m_sensitive_event_finders.clear(); 
    m_sensitive_interfaces.clear();
    m_sensitive_port_bases.clear();
  }

  void clear_all()
  {
    m_dont_initialize = false;
    this->clear_sensitivity();
    m_stack_size = 0;
  }

  sc_spawn_options* get_spawn_options()
  {
    if (m_dont_initialize)
      m_spawn_options.dont_initialize();

    m_spawn_options.set_stack_size(m_stack_size);
    
    if (this->has_sensitivity())
    {
      for(std::vector<const sc_event*>::iterator it = 
            m_sensitive_events.begin();
          it != m_sensitive_events.end();
          ++it)
        m_spawn_options.set_sensitivity(*it);

      for(std::vector<sc_event_finder*>::iterator it = 
            m_sensitive_event_finders.begin();
          it != m_sensitive_event_finders.end();
          ++it)
        m_spawn_options.set_sensitivity(*it);

      for(std::vector<sc_interface*>::iterator it = 
            m_sensitive_interfaces.begin();
          it != m_sensitive_interfaces.end();
          ++it)
        m_spawn_options.set_sensitivity(*it); 
    
      for(std::vector<sc_port_base*>::iterator it = 
            m_sensitive_port_bases.begin();
          it != m_sensitive_port_bases.end();
          ++it)
        m_spawn_options.set_sensitivity(*it); 
    }

    return &m_spawn_options;
  }

 protected:
  bool                          m_dont_initialize;         
  std::vector<const sc_event*>  m_sensitive_events;
  std::vector<sc_event_finder*> m_sensitive_event_finders; 
  std::vector<sc_interface*>    m_sensitive_interfaces;
  std::vector<sc_port_base*>    m_sensitive_port_bases;
  int                           m_stack_size;

  sc_spawn_options              m_spawn_options;
};

} // namespace osssi

} // namespace osss

#endif
