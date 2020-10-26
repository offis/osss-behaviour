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

#ifndef OSSS_STATIC_MACROS_INCLUDED
#define OSSS_STATIC_MACROS_INCLUDED

#define osss_declare_static_method_process(handle, name, classname, objectname) \
  {                                                                             \
    ::sc_core::sc_process_handle handle =                                       \
         sc_core::sc_get_curr_simcontext()->create_method_process(              \
           #name,  false, SC_MAKE_FUNC_PTR( classname, main ),                  \
           &objectname, 0 );                                                    \
         this->sensitive << handle;                                             \
         this->sensitive_pos << handle;                                         \
         this->sensitive_neg << handle;                                         \
  }

#define osss_declare_static_thread_process(handle, name, classname, objectname) \
  {                                                                             \
    ::sc_core::sc_process_handle handle =                                       \
          sc_core::sc_get_curr_simcontext()->create_thread_process(             \
             #name,  false, SC_MAKE_FUNC_PTR( classname, main ),                \
             &objectname, 0 );                                                  \
         this->sensitive << handle;                                             \
         this->sensitive_pos << handle;                                         \
         this->sensitive_neg << handle;                                         \
  }

#define osss_declare_static_cthread_process(handle, name, classname, objectname, edge) \
  {                                                                                    \
    ::sc_core::sc_process_handle handle =                                              \
          sc_core::sc_get_curr_simcontext()->create_cthread_process(                   \
             #name,  false, SC_MAKE_FUNC_PTR( classname, main ),                       \
             &objectname, 0 );                                                         \
         this->sensitive.operator() ( handle, edge );                                  \
  }

#define OSSS_STATIC_METHOD(classname, objectname)                \
     osss_declare_static_method_process( objectname ## _handle,  \
                                         objectname ## _main,    \
                                         classname,              \
                                         objectname )

#define OSSS_STATIC_THREAD(classname, objectname)                \
     osss_declare_static_thread_process( objectname ## _handle,  \
                                         objectname ## _main,    \
                                         classname,              \
                                         objectname )

#define OSSS_STATIC_CTHREAD(classname, objectname, edge)          \
     osss_declare_static_cthread_process( objectname ## _handle,  \
                                          objectname ## _main,    \
                                          classname,              \
                                          objectname,             \
                                          edge )

#endif
