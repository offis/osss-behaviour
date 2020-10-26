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
// i_receive.sc: receiver interface for one-way handshake
//
// author: Rainer Doemer
//
// modifications: (most recent first)
//
// 10/24/02 RD	replaced "signal" with "handshake" to avoid confusion
// 02/18/02 RD	applied naming convention, integrated with distribution
// 02/06/02 RD	generalized capabilities, fixed usage rules
// 01/31/02 RD	separated interface into send an receive interfaces
// 01/23/02 RD	brush up, moved into separate file "handshake.sc"
// 12/27/01 RD	initial version
//
//
// interface rules:
//
// - a connected thread acts as a receiver
// - a call to receive() lets the receiver wait for a handshake from the sender
// - if a handshake is present at the time of receive(), the call to receive()
//   will immediately return
// - if no handshake is present at the time of receive(), the calling thread
//   is suspended until the sender sends the handshake; then, the receiver
//   will resume its execution
// - calling receive() may suspend the calling thread indefinitely

#ifndef OSSS_RECEIVE_IF_INCLUDE
#define OSSS_RECEIVE_IF_INCLUDE

namespace osss {

class osss_receive_if : public virtual sc_core::sc_interface
{
 public:
  osss_receive_if() {}
  virtual ~osss_receive_if() {}

  virtual void receive() = 0;
};

} //namespace osss

#endif
