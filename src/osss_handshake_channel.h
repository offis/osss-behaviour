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
// c_handshake.sc: one-way handshake channel between a sender and a receiver
//
// author: Rainer Doemer
//
// modifications: (most recent first)
//
// 10/24/02 RD	replaced "signal" with "handshake" to avoid confusion
// 10/04/02 RD	added rule about safety of exceptions
// 02/18/02 RD	applied naming convention, integrated with distribution
// 02/06/02 RD	generalized capabilities, fixed usage rules
// 01/31/02 RD	separated interface into send an receive interfaces
// 01/23/02 RD	brush up, moved into separate file "handshake.sc"
// 12/27/01 RD	initial version
//
//
// interface rules:
//
// - see files i_send.sc, i_receive.sc
//
// channel rules:
//
// - this handshake channel provides safe one-way synchronization between
//   a sender and a receiver
// - a thread connected to the sender interface acts as a sender
// - a thread connected to the receiver interface acts as a receiver
// - only one sender and one receiver may use the channel at any time;
//   otherwise, the behavior is undefined
// - a call to send() sends a handshake to the receiver; if the receiver
//   is waiting at the time of the send(), it will wake up and resume
//   its execution; otherwise, the handshake is stored until the receiver
//   calls receive()
// - the behavior is undefined if send() is called successively without
//   any calls to receive()
// - a call to receive() lets the receiver wait for a handshake from the sender
// - if a handshake is present at the time of receive(), the call to receive()
//   will immediately return
// - if no handshake is present at the time of receive(), the calling thread
//   is suspended until the sender sends the handshake; then, the receiver
//   will resume its execution
// - calling send() will not suspend the calling thread
// - calling receive() may suspend the calling thread indefinitely
// - this channel is only safe with respect to exceptions, if any exceptions
//   are guaranteed to occur only for all communicating threads simultaneously;
//   the behavior is undefined, if any exceptions occur only for a subset
//   of the communicating threads
// - no restrictions exist for use of 'waitfor'
// - no restrictions exist for use of 'wait', 'notify', 'notifyone'

#ifndef OSSS_HANDSHAKE_CHANNEL_INCLUDE
#define OSSS_HANDSHAKE_CHANNEL_INCLUDE

#include "osss_send_if.h"
#include "osss_receive_if.h"

namespace osss {

class osss_handshake_channel : 
  public osss_send_if,
  public osss_receive_if,
  public sc_core::sc_prim_channel
{
 protected:
  sc_core::sc_event e;
  bool              f, w;

 public:

  osss_handshake_channel() :
    sc_prim_channel( 
      sc_core::sc_gen_unique_name("osss_handshake_channel")),
    e(),
    f(false),
    w(false)
  {}

  osss_handshake_channel(const char* name) :
    sc_prim_channel(name),
    e(),
    f(false),
    w(false)
  {}

  ~osss_handshake_channel() {}

  const char* kind() const
    { return "osss_handshake_channel"; }

  virtual void receive()
  {
    if (! f)
    {
      w = true;
      wait(e);
      w = false;
    }
    f = false;
  }

  virtual void send()
  {
    if (w)
    {
      e.notify(sc_core::SC_ZERO_TIME);
    }
    f = true;
  }
};

} // namespace osss

#endif
