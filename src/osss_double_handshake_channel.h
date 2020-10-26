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
// This code is based on the SpecC implementation:
//
// c_double_handshake.sc: type-less 2-way handshake between sender and receiver
// (rendezvous for data transfer between a sender and a receiver)
//
// author: Rainer Doemer
//
// modifications: (most recent first)
//
// 08/10/04 RD	made receiver wait for its own 'ack' such that both
//		receiver and sender exit in the same synch. cycle
//		(otherwise a tranceiver may receive its own 'ack'
//		when calling send immediately after receive)
// 04/25/03 RD	added support of interface i_typed_tranceiver
// 04/22/03 RD	added 'const' qualifier to sender interface
// 10/04/02 RD	added rule about safety of exceptions
// 02/18/02 RD	applied naming convention, integrated with distribution
// 02/07/02 RD	clarified properties, fixed code
// 02/06/02 RD  separated interfaces for send and receive
// 01/31/02 RD	brush up, moved into separate file "double_handshake.sc"
// 12/27/01 RD	initial version
//
//
// interface rules:
//
// - see files i_sender.sc, i_receiver.sc, i_tranceiver.sc
//
// channel rules:
//
// - this double-handshake channel can be seen as a queue (see c_queue.sc)
//   with size 0; because no data can be stored in the channel, the
//   sender and receiver must meet in a rendezvous to exchange the data
// - exactly one receiver and one sender thread may use the same channel
//   instance at the same time; if used by more than one sender or receiver,
//   the behavior of the channel is undefined
// - the same channel instance may be used multiple times in order to
//   transfer multiple data packets from the sender to the receiver
// - using the tranceiver interface, the channel may be used bidirectionally
// - the sender calls send() to send a packet of data to the receiver
// - the receiver calls receive() to receive a packet of data from the sender
// - data packets may vary in size; valid sizes are in the range
//   between 0 and 2**32-1 inclusively
// - if different packet sizes are used with the same channel, the user
//   has to ensure that the data size of the sender always matches
//   the data size expected by the receiver; it is an error if the sizes
//   in a transaction don't match
// - the channel operates in rendezvous fashion; a call to send() will
//   suspend the sender until the receiver calls receive(), and vice versa;
//   when both are ready, data is transferred from the sender to the receiver
//   and both can resume their execution
// - calling send() or receive() may suspend the calling thread indefinitely
// - this channel is only safe with respect to exceptions, if any exceptions
//   are guaranteed to occur only for all communicating threads simultaneously;
//   the behavior is undefined, if any exceptions occur only for a subset
//   of the communicating threads
// - no restrictions exist for use of 'waitfor'
// - no restrictions exist for use of 'wait', 'notify', 'notifyone'

#ifndef OSSS_DOUBLE_HANDSHAKE_CHANNEL_INCLUDE
#define OSSS_DOUBLE_HANDSHAKE_CHANNEL_INCLUDE

#include "osss_transceiver_if.h"

namespace osss {

template<typename T>
class osss_double_handshake_channel :
  public virtual osss_transceiver_if<T>,
  public sc_core::sc_prim_channel
{
 protected:
  sc_core::sc_event req, ack;
  bool              v, w;
  T                 data;

 public:

  osss_double_handshake_channel() :
    sc_prim_channel( 
      sc_core::sc_gen_unique_name("osss_double_handshake_channel")),
    req(),
    ack(),
    v(false),
    w(false),
    data( T() )
  {}

  osss_double_handshake_channel(const char* name) :
    sc_prim_channel(name),
    req(),
    ack(),
    v(false),
    w(false),
    data( T() )
  {}

  ~osss_double_handshake_channel() {}

  const char* kind() const
    { return "osss_double_handshake_channel"; }

  virtual void receive(T& value)
  {
    if (!v)
    {
      w = true;
      wait(req);
      w = false;
    }
    value = data;
    v = false;
    ack.notify(sc_core::SC_ZERO_TIME);
    wait(ack);
  }

  virtual void send(const T& value)
  {
    data = value;
    v = true;
    if (w)
    {
      req.notify(sc_core::SC_ZERO_TIME);
    }
    wait(ack);
  }
};

} // namespace osss

#endif
