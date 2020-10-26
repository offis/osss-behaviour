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

#ifndef OSSS_BEHAVIOUR_LIST_INCLUDED
#define OSSS_BEHAVIOUR_LIST_INCLUDED

namespace osss {

// ----------------------------------------------------------------------------
// osss_behaviour_list
// ----------------------------------------------------------------------------

class osss_behaviour_list
{
  friend class osss_behaviour;
  friend class osss_sequential_behaviour;
  friend class osss_parallel_behaviour;
  friend class osss_pipeline_behaviour;

protected:

  void push_back( const osss_behaviour& );

  osss_behaviour_list( const osss_behaviour&,
                       bool auto_delete_ = false );

  int size() const;
  void auto_delete();

 private:

  std::vector<const osss_behaviour*> m_behaviours;
  bool                               m_auto_delete;

 private:

  // disabled
  osss_behaviour_list();
  osss_behaviour_list( const osss_behaviour_list& );
  osss_behaviour_list& operator = ( const osss_behaviour_list& );
};

inline
osss_behaviour_list::osss_behaviour_list( const osss_behaviour& beh,
                                          bool auto_delete_ ) :
  m_auto_delete( auto_delete_ )
{
  m_behaviours.push_back( &beh );
}

inline
int
osss_behaviour_list::size() const
{
  return m_behaviours.size();
}

inline
void
osss_behaviour_list::auto_delete()
{
  if( m_auto_delete ) {
    delete this;
  }
}

inline
void
osss_behaviour_list::push_back( const osss_behaviour& beh )
{
  // make sure behaviour is not already in the list
  if ( m_behaviours.size() != 0 ) {
    const osss_behaviour** l_behs = &m_behaviours[0];
    for( int i = m_behaviours.size() - 1; i >= 0; -- i ) 
    {
      if( &beh == l_behs[i] ) {
        // behaviour already in the list; ignore
        return;
      }
    }
  }
  m_behaviours.push_back( &beh );
}

// ----------------------------------------------------------------------------
// osss_parallel_behaviour_list
// ----------------------------------------------------------------------------

class osss_parallel_behaviour_list : public osss_behaviour_list
{
  friend class osss_behaviour;
  
protected:

  osss_parallel_behaviour_list( const osss_behaviour&, 
                                bool auto_delete_ = false );

public:

  osss_parallel_behaviour_list& operator | ( const osss_behaviour& );

private:

  // disabled
  osss_parallel_behaviour_list();
  osss_parallel_behaviour_list( const osss_parallel_behaviour_list& );
  osss_parallel_behaviour_list& operator = ( const osss_parallel_behaviour_list& );
};

inline
osss_parallel_behaviour_list::osss_parallel_behaviour_list( 
  const osss_behaviour& beh,
  bool auto_delete_ ) : osss_behaviour_list( beh, auto_delete_ )
{}

inline
osss_parallel_behaviour_list&
osss_parallel_behaviour_list::operator | ( const osss_behaviour& beh )
{
  push_back( beh );
  return *this;
}

// ----------------------------------------------------------------------------
// osss_sequential_behaviour_list
// ----------------------------------------------------------------------------

class osss_sequential_behaviour_list : public osss_behaviour_list
{
  friend class osss_behaviour;
  
protected:

  osss_sequential_behaviour_list( const osss_behaviour&, 
                                  bool auto_delete_ = false );

public:

  osss_sequential_behaviour_list& operator , ( const osss_behaviour& );

private:

  // disabled
  osss_sequential_behaviour_list();
  osss_sequential_behaviour_list( const osss_sequential_behaviour_list& );
  osss_sequential_behaviour_list& operator = ( const osss_sequential_behaviour_list& );
};

inline
osss_sequential_behaviour_list::osss_sequential_behaviour_list( 
  const osss_behaviour& beh,
  bool auto_delete_ ) : osss_behaviour_list( beh, auto_delete_ )
{}

inline
osss_sequential_behaviour_list&
osss_sequential_behaviour_list::operator , ( const osss_behaviour& beh )
{
  push_back( beh );
  return *this;
}

// ----------------------------------------------------------------------------
// osss_pipeline_behaviour_list
// ----------------------------------------------------------------------------

class osss_pipeline_behaviour_list : public osss_behaviour_list
{
  friend class osss_behaviour;
  friend class osss_pipeline_stage;
  
protected:

  osss_pipeline_behaviour_list( const osss_pipeline_stage&, 
                                bool auto_delete_ = false );

public:

  osss_pipeline_behaviour_list& operator >> ( const osss_pipeline_stage& );

private:

  // disabled
  osss_pipeline_behaviour_list();
  osss_pipeline_behaviour_list( const osss_pipeline_behaviour_list& );
  osss_pipeline_behaviour_list& operator = ( const osss_pipeline_behaviour_list& );
};

inline
osss_pipeline_behaviour_list::osss_pipeline_behaviour_list( 
  const osss_pipeline_stage& beh,
  bool auto_delete_ ) : osss_behaviour_list( beh, auto_delete_ )
{}

inline
osss_pipeline_behaviour_list&
osss_pipeline_behaviour_list::operator >> ( const osss_pipeline_stage& beh )
{
  push_back( beh );
  return *this;
}

// osss_pipeline_stage
inline
osss_pipeline_behaviour_list&
osss_pipeline_stage::operator >> ( const osss_pipeline_stage& beh2 ) const
{
  osss_pipeline_behaviour_list* behl = 
    new osss_pipeline_behaviour_list( *this, true );
  behl->push_back( beh2 );
  return *behl;
}

} // namespace osss

#endif


