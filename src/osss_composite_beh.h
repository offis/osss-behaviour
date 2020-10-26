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

#ifndef OSSS_COMPOSITE_BEH_INCLUDED
#define OSSS_COMPOSITE_BEH_INCLUDED

namespace osss {

// forward declaration
class osss_behaviour;

namespace osssi {

class osss_composite_behaviour
{
public:

  explicit osss_composite_behaviour(osss_behaviour* beh_ptr) :
    m_behaviour(beh_ptr),
    m_behaviour_list()
  {}

  virtual ~osss_composite_behaviour() {}
  
  virtual void main() = 0;

  std::vector<osss_behaviour*>& behaviour_list()
    { return m_behaviour_list; }

  osss_behaviour* behaviour() { return m_behaviour; }

protected:
  osss_behaviour*              m_behaviour;
  std::vector<osss_behaviour*> m_behaviour_list;
  
private:
  osss_composite_behaviour();
  osss_composite_behaviour(const osss_composite_behaviour&);
  osss_composite_behaviour& operator=(const osss_composite_behaviour&);
};

} // namespace osssi

} // namespace osss

#endif
