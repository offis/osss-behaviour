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

#ifndef OSSS_EVENT_IFS_INCLUDED
#define OSSS_EVENT_IFS_INCLUDED

namespace osss {

class osss_event_in_if : virtual public sc_core::sc_interface
{
public:
  // get the event
  virtual const osss_event* get_event() const = 0;
  virtual operator const osss_event& () const = 0;
 
  virtual sc_core::sc_event_expr<sc_event_or_list>  
  operator|(const sc_core::sc_event& ev) const = 0;
  
  virtual sc_core::sc_event_expr<sc_event_and_list> 
  operator&(const sc_core::sc_event& ev) const = 0;

protected:
  // constructor
  osss_event_in_if() {}

private:
  // disabled
  osss_event_in_if( const osss_event_in_if& );
  osss_event_in_if& operator=( const osss_event_in_if& );
};

class osss_event_inout_if : public osss_event_in_if
{
public:
  virtual void cancel() = 0;

  virtual void notify() = 0;
  virtual void notify(const sc_core::sc_time & t) = 0;
  virtual void notify(double d, sc_core::sc_time_unit t) = 0;
  virtual void notify_delayed() = 0;
  virtual void notify_delayed(const sc_core::sc_time & t) = 0;
  virtual void notify_delayed(double d, sc_core::sc_time_unit t) = 0;

protected:
  // constructor
  osss_event_inout_if() {}

private:
  // disabled
  osss_event_inout_if( const osss_event_inout_if& );
  osss_event_inout_if& operator=( const osss_event_inout_if& );
};

#define osss_event_out_if osss_event_inout_if

} // namespace osss

#endif
