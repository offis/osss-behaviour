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
// This code is based on the SpecC implementation of:
//
// i_receiver.sc: type-less receiver interface
//
// author: Rainer Doemer
//
// modifications: (most recent first)
//
// 02/13/02 RD	applied naming convention, integrated with distribution
// 02/05/02 RD	parameterized name of the queue
// 02/04/02 RD	refined usage rules
// 02/01/02 RD	defined specific features and inserted actual code
// 01/31/02 RD	initial version
//
//
// interface rules:
//
// - a connected thread acts as a receiver
// - a call to receive() receives a packet of data from a connected channel
// - data packets are typeless (i.e. an array of bytes in the memory)
//   and may vary in size for separate calls to receive()
// - calling receive() may suspend the calling thread indefinitely

#ifndef OSSS_RECEIVER_IF_INCLUDE
#define OSSS_RECEIVER_IF_INCLUDE

namespace osss {

template<typename T>
class osss_receiver_if : public virtual sc_core::sc_interface
{
 public:
  osss_receiver_if() {}
  virtual ~osss_receiver_if() {}

  virtual void receive(T&) = 0;
};

} //namespace osss

#endif
