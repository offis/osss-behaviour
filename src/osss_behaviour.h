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

#ifndef OSSS_BEHAVIOUR_H
#define OSSS_BEHAVIOUR_H

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>
#include <stdarg.h>

#define OSSS_SEQUENTIAL_BEHAVIOUR_SIMPLE

#include "osss_tracing.h"
#include "osss_beh.h"
#include "osss_static_macros.h"
#include "osss_seq.h"
#include "osss_fork_join.h"
#include "osss_par.h"
#include "osss_condition_wrapper.h"
#include "osss_state.h"
#include "osss_state_pair.h"
#include "osss_state_macros.h"
#include "osss_sequential_beh.h"
#include "osss_parallel_beh.h"
#include "osss_pipeline_beh.h"
#include "osss_pipeline_macros.h"
#include "osss_behaviour_list.h"
#include "osss_port.h"
#include "osss_event.h"
#include "osss_event_channel.h"
#include "osss_event_ports.h"
#include "osss_array.h"
#include "osss_shared_variable.h"
#include "osss_shared_variable_ports.h"
#include "osss_shared_array_variable_ports.h"
#include "osss_handshake_channel.h"
#include "osss_double_handshake_channel.h"
#include "osss_unchecked_signal.h"
#include "osss_wait.h"
#include "osss_prim_channel.h"
#include "osss_interface.h"
#include "osss_module.h"
#include "osss_timing.h"

#endif
