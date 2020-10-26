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

#ifndef OSSS_EVENT_LIST_INCLUDED
#define OSSS_EVENT_LIST_INCLUDED

namespace osss {
namespace osssi {

// ----------------------------------------------------------------------------
//  CLASS : osss_event_or_list
//
//  OR list of events.
// ----------------------------------------------------------------------------
class osss_event_or_list : public sc_core::sc_event_or_list
{
    friend class sc_core::sc_event;
    
public:

    osss_event_or_list( const sc_core::sc_event& e) :
      sc_core::sc_event_or_list(e)
    {}

    void add_event(const sc_core::sc_event& e)
    {
      push_back( e );
    }

private:

    // disabled
    osss_event_or_list();
    osss_event_or_list( const osss_event_or_list& );
    osss_event_or_list& operator = ( const osss_event_or_list& );
};

// ----------------------------------------------------------------------------
//  CLASS : osss_event_and_list
//
//  AND list of events.
// ----------------------------------------------------------------------------
class osss_event_and_list : public sc_core::sc_event_and_list
{
    friend class sc_core::sc_event;
    
public:

    osss_event_and_list( const sc_core::sc_event& e) :
      sc_core::sc_event_and_list(e)
    {}

    void add_event(const sc_core::sc_event& e)
    {
      push_back( e );
    }

private:

    // disabled
    osss_event_and_list();
    osss_event_and_list( const osss_event_and_list& );
    osss_event_and_list& operator = ( const osss_event_and_list& );
};

} // namespace osssi
} // namespace osss

#endif
