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
// This code is based on the OSCI SystemC sc_in<...> port implementation.
//

#ifndef OSSS_EVENT_PORTS_INCLUDED
#define OSSS_EVENT_PORTS_INCLUDED

#include "osss_event_ifs.h"

namespace osss {

//forward declaration
class osss_event_inout;

// ----------------------------------------------------------------------------
//  CLASS : osss_event_in
//
//  The osss_event_in input port class.
// ----------------------------------------------------------------------------

class osss_event_in : 
  public sc_core::sc_port<osss_event_in_if,1,SC_ONE_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef osss_event_in_if                       if_type;
    typedef sc_core::sc_port<if_type, 1,
                             SC_ONE_OR_MORE_BOUND> base_type;
    typedef osss_event_in                          this_type;

    typedef if_type                                in_if_type;
    typedef base_type                              in_port_type;
    typedef osss_event_inout_if                    inout_if_type;
    typedef sc_core::sc_port<inout_if_type, 1, 
                             SC_ONE_OR_MORE_BOUND> inout_port_type;

public:

    // constructors

    osss_event_in()
	: base_type()
	{}

    explicit osss_event_in( const char* name_ )
	: base_type( name_ )
	{}

    explicit osss_event_in( const in_if_type& interface_ )
        : base_type( CCAST<in_if_type&>( interface_ ) )
        {}

    osss_event_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) )
	{}

    explicit osss_event_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    explicit osss_event_in( inout_port_type& parent_ )
	: base_type()
	{ sc_core::sc_port_base::bind( parent_ ); }

    osss_event_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ )
	{ sc_core::sc_port_base::bind( parent_ ); }

    osss_event_in( this_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor

    virtual ~osss_event_in()
      {}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_core::sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_core::sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_core::sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_core::sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    const osss_event* get_event() const
      { return (*this)->get_event(); }

    operator const osss_event& () const
      { return *((*this)->get_event()); }

    sc_core::sc_event_expr<sc_event_or_list>   
      operator|(const sc_core::sc_event& ev) const
      { return (*this)->operator|(ev); }

    sc_core::sc_event_expr<sc_event_or_list>   
      operator|(const osss_event_in& ev_in) const
      { return (*this)->operator|(*(ev_in.get_event())); }

    sc_core::sc_event_expr<sc_event_or_list> 
      operator|(const osss_event_inout& ev_inout) const;
  
    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const sc_core::sc_event& ev) const
      { return (*this)->operator&(ev); }

    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const osss_event_in& ev_in) const
      { return (*this)->operator&(*(ev_in.get_event())); }

    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const osss_event_inout& ev_inout) const;

    virtual const char* kind() const
        { return "osss_event_in"; }

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_core::sc_interface& );
    virtual int vbind( sc_core::sc_port_base& );

private:

    // disabled
    osss_event_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// called by pbind (for internal use only)

inline
int
osss_event_in::vbind( sc_core::sc_interface& interface_ )
{
    return sc_core::sc_port_b<if_type>::vbind( interface_ );
}

inline
int
osss_event_in::vbind( sc_core::sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_core::sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_core::sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : osss_event_inout
//
//  The osss_event input/output port class.
// ----------------------------------------------------------------------------

class osss_event_inout : 
public sc_core::sc_port<osss_event_inout_if,1,SC_ONE_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef osss_event_inout_if                    if_type;
    typedef sc_core::sc_port<if_type, 1, 
                             SC_ONE_OR_MORE_BOUND> base_type;
    typedef osss_event_inout                       this_type;

    typedef osss_event_in_if                       in_if_type;
    typedef sc_core::sc_port<in_if_type, 1, 
                             SC_ONE_OR_MORE_BOUND> in_port_type;
    typedef if_type                                inout_if_type;
    typedef base_type                              inout_port_type;

public:

    // constructors

    osss_event_inout()
	: base_type()
	{}

    explicit osss_event_inout( const char* name_ )
	: base_type( name_ )
	{}

    explicit osss_event_inout( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    osss_event_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit osss_event_inout( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    osss_event_inout( this_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor

    virtual ~osss_event_inout() 
        {}


    // interface access shortcut methods

    const osss_event* get_event() const
      { return (*this)->get_event(); }

    operator const osss_event& () const
      { return *((*this)->get_event()); }

    sc_core::sc_event_expr<sc_event_or_list> 
      operator|(const sc_core::sc_event& ev) const
      { return (*this)->operator|(ev); }

    sc_core::sc_event_expr<sc_event_or_list> 
      operator|(const osss_event_in& ev_in) const
      { return (*this)->operator|(*(ev_in.get_event())); }

    sc_core::sc_event_expr<sc_event_or_list>   
      operator|(const osss_event_inout& ev_inout) const
      { return (*this)->operator|(*(ev_inout.get_event())); }
  
    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const sc_core::sc_event& ev) const
      { return (*this)->operator&(ev); }

    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const osss_event_in& ev_in) const
      { return (*this)->operator&(*(ev_in.get_event())); }

    sc_core::sc_event_expr<sc_event_and_list> 
      operator&(const osss_event_inout& ev_inout) const
      { return (*this)->operator&(*(ev_inout.get_event())); }

    void cancel()
      { (*this)->cancel(); }

    void notify()
      { (*this)->notify(); }

    void notify(const sc_core::sc_time & t)
      { (*this)->notify(t); }
    
    void notify(double d, sc_core::sc_time_unit t)
      { (*this)->notify(d, t); }

    void notify_delayed()
      { (*this)->notify_delayed(); }
    
    void notify_delayed(const sc_core::sc_time & t)
      { (*this)->notify_delayed(t); }

    void notify_delayed(double d, sc_core::sc_time_unit t)
      { (*this)->notify_delayed(d, t); }


    virtual const char* kind() const
        { return "osss_event_inout"; }

private:

    // disabled
    osss_event_inout( const this_type& );
};


// ----------------------------------------------------------------------------
//  CLASS : osss_event_out
//
//  The osss_event output port class.
// ----------------------------------------------------------------------------

// osss_event_out can also read from its port, hence no difference with 
// osss_event_inout.
// For debugging reasons, a class is provided instead of a define.

class osss_event_out : public osss_event_inout
{
public:

    // typedefs

    typedef osss_event_out             this_type;
    typedef osss_event_inout           base_type;

    typedef base_type::in_if_type      in_if_type;
    typedef base_type::in_port_type    in_port_type;
    typedef base_type::inout_if_type   inout_if_type;
    typedef base_type::inout_port_type inout_port_type;

public:

    // constructors

    osss_event_out()
	: base_type()
	{}

    explicit osss_event_out( const char* name_ )
	: base_type( name_ )
	{}

    explicit osss_event_out( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    osss_event_out( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit osss_event_out( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_out( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    osss_event_out( this_type& parent_ )
	: base_type( parent_ )
	{}

    osss_event_out( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~osss_event_out()
	{}

    virtual const char* kind() const
        { return "osss_event_out"; }

private:

    // disabled
    osss_event_out( const this_type& );
};

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_core::sc_event_expr<sc_event_or_list>   
osss_event_in::operator|(const osss_event_inout& ev_inout) const
{ return (*this)->operator|(*(ev_inout.get_event())); }

inline
sc_core::sc_event_expr<sc_event_and_list> 
osss_event_in::operator&(const osss_event_inout& ev_inout) const
{ return (*this)->operator&(*(ev_inout.get_event())); }

} // namespace osss

#endif
