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

#ifndef OSSS_TIMING_INCLUDED
#define OSSS_TIMING_INCLUDED

namespace osss {

// -----------------------------------------------------------------------------
// OSSS internal class declarations. Not to be used directly in an OSSS model!
// -----------------------------------------------------------------------------

// Helper struct which stores the source location of the user's timing annotations
// (for meaningful error messages).
struct osss_timing_annotation_source_info
{
  osss_timing_annotation_source_info( 
    std::string fileName,
    unsigned int lineNumber,
    sc_core::sc_object* obj
  );

  std::ostream& print(std::ostream &os) const;
  
  sc_core::sc_time    m_timeOfCreation;
  std::string         m_fileName;
  unsigned int        m_lineNumber;
  sc_core::sc_object  *m_obj;
};

// The implementation of the RET (Required Execution time) as described below
class osss_required_time 
{
public:

  osss_required_time( osss_timing_annotation_source_info const &otasi,
                      sc_core::sc_time                   const &duration);

  osss_required_time( osss_timing_annotation_source_info const &otasi,
                      double                                    duration);

  ~osss_required_time();

private:
    
  osss_timing_annotation_source_info const  m_otasi;
  sc_core::sc_time                          m_duration;

};

// The implementation of EET (Estimated Execution Time) as described below
class osss_waste_of_time 
{
public:
  
  osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                     sc_core::sc_time                   const &duration);

  osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                     double                                    duration);

  ~osss_waste_of_time();

private:
  osss_timing_annotation_source_info const  m_otasi;
  sc_core::sc_time                          m_timeOfCreation;
  sc_core::sc_time                          m_duration;
};

// =============================================================================
// OSSS-internal method bodies
// =============================================================================

inline
osss_timing_annotation_source_info::
osss_timing_annotation_source_info( std::string fileName,
                                    unsigned int lineNumber,
                                    sc_core::sc_object* obj )
  : m_timeOfCreation(sc_core::sc_time_stamp()),
    m_fileName(fileName),
    m_lineNumber(lineNumber),
    m_obj(obj)
{
  if (m_obj == NULL)
  {
    std::cerr << "OSSS internal error: illegal object during "
              << "the creation of an osss_timing_annotation_source_info. "
              << fileName << ':' << lineNumber << std::endl;
  }
}

inline
std::ostream&
osss_timing_annotation_source_info::print(std::ostream &os) const
{
  os << '[';
  os << m_obj->name() << " (" << m_obj->kind() << ")";
  os << ", " << m_fileName << ':' << m_lineNumber
     << ", created : "
     << m_timeOfCreation << ']';
  return os;
}

// -----------------------------------------------------------------------------

inline
osss_required_time::
osss_required_time( osss_timing_annotation_source_info const &otasi,
                    sc_core::sc_time                   const &duration )
  : m_otasi(otasi),
    m_duration(duration)
{}

inline
osss_required_time::
osss_required_time( osss_timing_annotation_source_info const &otasi,
                    double                                    duration )
  : m_otasi(otasi),
    m_duration( duration * osss_global_port_registry::get_clock_period() )
{}

inline
osss_required_time::
~osss_required_time() 
{ 
  /*some computation to calculate the deadline and if the deadline 
    is already exceeded */
    sc_core::sc_time now = sc_core::sc_time_stamp(),
    deadline = m_otasi.m_timeOfCreation + m_duration;
  
  /*information about an deadline violation, or otherwise a waste of time*/
  if (now > deadline)
  {
    std::cout << "OSSS_RET violation ";
    m_otasi.print(std::cout);
    std::cout << " : duration: " << m_duration
              << ", deadline: " << deadline
              << ", now: " << now
              << std::endl;
  }
}

// -----------------------------------------------------------------------------

inline
osss_waste_of_time::
osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                   sc_core::sc_time                   const &duration)
  :  m_otasi(otasi),
     m_duration(duration)
{}

inline
osss_waste_of_time::
osss_waste_of_time(osss_timing_annotation_source_info const &otasi,
                   double                                    duration)
  :  m_otasi(otasi),
     m_duration( duration * osss_global_port_registry::get_clock_period() )
{}

inline
osss_waste_of_time::
~osss_waste_of_time() 
{ 
  osss::wait(m_duration);
}

// =============================================================================
// EET and RET user macros
// =============================================================================

/**
 * Estimated execution time (EET). Defines the time it takes for a certain block
 * to execute. [Ideally these times could be determined by a tool and annotated]
 * The EET is intended for simulation only. It will have no effect on the 
 * generated code. The aim is to give the behaviour code a meaningful (and more 
 * or accurate) timing behaviour. It is by no way sufficient to write real-time 
 * systems
 * Nesting EETs will result in a duration which is the sum of all sub-durations.
 */
#define OSSS_EET(duration)                                                    \
   ::osss::osss_waste_of_time                                                 \
   OSSS_INTERNAL__estimated_execution_time(                                   \
     (::osss::osss_timing_annotation_source_info (                            \
        std::string(__FILE__),                                                \
        __LINE__,                                                             \
       this )                                                                 \
     ),                                                                       \
     duration);
   
/**                          
 * Required execution time. This can be seen as a kind of constraint. It 
 * specifies the time it takes for a certain block of code to execute. If the
 * code executes faster than the given duration nothing happens.
 * The execution times of the code within the RET block can be
 * specified via EETs or RETs. 
 * If the code executes slower than the given duration an error will be issued.
 * RETs are intended to have an effect on the generated code.
 * Nesting RETs is allowed. The resulting duration is a) the specified outer
 * duration in the case there is no violation or b) the sum of the inner RETs
 * (and an error message) in the case the inner RETs are greater than the outer
 * duration.
 */
#define OSSS_RET(duration)                                                    \
   ::osss::osss_required_time                                                 \
   OSSS_INTERNAL__required_execution_time(                                    \
     (::osss::osss_timing_annotation_source_info (                            \
        std::string(__FILE__),                                                \
        __LINE__,                                                             \
        this )                                                                \
     ),                                                                       \
    duration);

} // namespace osss

#endif
