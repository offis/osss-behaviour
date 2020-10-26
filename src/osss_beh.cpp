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

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

#include "osss_composite_beh.h"
#include "osss_beh.h"
#include "osss_sequential_beh.h"
#include "osss_parallel_beh.h"
#include "osss_pipeline_beh.h"
#include "osss_behaviour_list.h"
#include "osss_condition_wrapper.h"
#include "osss_state.h"
#include "osss_state_pair.h"
#include "osss_fsm_beh.h"

#include "osss_string_utils.h"

namespace osss {

  osss_behaviour::osss_behaviour() :
    sc_core::sc_module(),
    osssi::osss_clock_reset_if(),
    m_composite_beh(NULL),
    m_behaviour_type(OSSS_LEAF_BEHAVIOUR),
    m_behaviour_list(),
    m_behaviour_options(),
    m_sensitivity_cleared(false)
  {}

  osss_behaviour::osss_behaviour(osss_behaviour_name name) :
    sc_core::sc_module( name ),
    osssi::osss_clock_reset_if(),
    m_composite_beh(NULL),
    m_behaviour_type(OSSS_LEAF_BEHAVIOUR),
    m_behaviour_list(),
    m_behaviour_options(),
    m_sensitivity_cleared(false)
  {}

  osss_behaviour::~osss_behaviour()
  {
    if (m_composite_beh)
    {
      delete m_composite_beh;
      m_composite_beh = NULL;
    }
  }

  const char* osss_behaviour::type () const
  {
    switch(m_behaviour_type)
    {
      case OSSS_LEAF_BEHAVIOUR : 
        return "OSSS_LEAF_BEHAVIOUR";
      case OSSS_SEQ_BEHAVIOUR :
        return "OSSS_SEQ_BEHAVIOUR";
      case OSSS_PAR_BEHAVIOUR :
        return "OSSS_PAR_BEHAVIOUR";
      case OSSS_PIPE_BEHAVIOUR :
        return "OSSS_PIPE_BEHAVIOUR";
      case OSSS_FSM_BEHAVIOUR :
        return "OSSS_FSM_BEHAVIOUR";
      default:
        return "OSSS_UNKNOWN_BEHAVIOUR";
    }
  };
  
  osss_behaviour_node_type osss_behaviour::node_type() const
  {
    return m_behaviour_type;
  }

  void osss_behaviour::main()
  {
    switch(m_behaviour_type)
    {
      case OSSS_SEQ_BEHAVIOUR  :
      case OSSS_PAR_BEHAVIOUR  :
      case OSSS_PIPE_BEHAVIOUR :
      case OSSS_FSM_BEHAVIOUR  :
      {
        m_composite_beh->main();
        break;
      }
      case OSSS_LEAF_BEHAVIOUR :
      default :
      {
        std::cout << "Warning: Did you forget to specify the main method"
                  << " of leaf behaviour " << this->name() << "?"
                  << std::endl;
        break;
      }
    }
  }

  void osss_behaviour::set_sensitivity(const sc_core::sc_event& event)
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true; 
      m_behaviour_options.set_sensitivity(&event);
    }
  }

  void osss_behaviour::set_sensitivity(sc_core::sc_port_base& port_base)
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true;
      m_behaviour_options.set_sensitivity(&port_base);
    }
  }

  void osss_behaviour::set_sensitivity(sc_core::sc_interface& interface_p)
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true;
      m_behaviour_options.set_sensitivity(&interface_p); 
    }
  }

  void osss_behaviour::set_sensitivity(sc_core::sc_export_base& export_base)
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true;
      m_behaviour_options.set_sensitivity(&export_base); 
    }
  }

  void osss_behaviour::set_sensitivity(sc_core::sc_event_finder& event_finder)
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true;
      m_behaviour_options.set_sensitivity(&event_finder); 
    }
  }

  void osss_behaviour::clear_sensitivity()
  { 
    if(!sc_core::sc_is_running())
    {
      m_sensitivity_cleared = true;
      m_behaviour_options.clear_sensitivity(); 
    }
  }

  void osss_behaviour::pipeline_step()
  {
    if (osssi::osss_pipeline_behaviour* pbeh = 
        dynamic_cast<osssi::osss_pipeline_behaviour*>(m_composite_beh))
      pbeh->step_proc();
    else
    {
      std::cerr << "Error: Behaviour " << this->name() 
               << " should be a pipeline behaviour!" << std::endl;
      sc_stop();
    }
  }

  void osss_behaviour::init()  { /* does nothing by default */ }
  void osss_behaviour::final() { /* does nothing by default */ }
 
  void osss_behaviour::pipe_pre()  { /* does nothing by default */ }
  void osss_behaviour::pipe_post() { /* does nothing by default */ }

  bool osss_behaviour::pipe_stop_condition() const
  {
    if (m_behaviour_type == OSSS_PIPE_BEHAVIOUR)
      return false; /* by default an unconstrained pipe is an endless loop */
    else
    {
      std::cerr << "Error: Called stop_condition() in an non-"
                << "pipeline behaviour: " 
                << this->name() << " [" << this->type() << "]."
                << std::endl;
      exit(1);
    }
  }

  void osss_behaviour::before_end_of_elaboration()
  {
    // perform clock and reset port bindings, unless it 
    // has already been performed by the user
    osssi::osss_clock_reset_if::check_clock_reset_port();
  }

  void osss_behaviour::end_of_elaboration()
  {
    //
    // inherit sensitivities from parent behaviour, unless
    // the user has manually cleared the sensitivity list of this behaviour
    //
    if (!m_sensitivity_cleared)
    {
      sc_core::sc_object* obj = get_parent_object();
      if(osss_behaviour* beh = dynamic_cast<osss_behaviour*>(obj))
      {
        // get sensitivity from parent behaviour and make it ours
        m_behaviour_options = beh->m_behaviour_options;
      }
      else if(/*sc_core::sc_module* mod =*/ dynamic_cast<sc_core::sc_module*>(obj))
      {
        // TODO: What if parent object is a module?
        //       Provide special osss_module to get access to the module
        //       processes sensitivity list?
      }
    }

    //
    // Design rule checks
    //
    std::vector<sc_core::sc_object*> objs(this->get_child_objects());
    std::vector<osss_behaviour*> forbidden_sub_behs;
    switch(m_behaviour_type)
    {
      case OSSS_LEAF_BEHAVIOUR : 
      {
        // check: does not conatin any sub-behaviours
        for(unsigned int i=0; i<objs.size(); ++i)
        {
          osss_behaviour* beh = dynamic_cast<osss_behaviour*>(objs[i]);
          if(beh)
            forbidden_sub_behs.push_back(beh);
        }
        break;
      }
      case OSSS_SEQ_BEHAVIOUR :
      case OSSS_PAR_BEHAVIOUR :
      {
        // check: contains no state or pipeline stage behaviours
        for(unsigned int i=0; i<objs.size(); ++i)
        {
          if(osss_state* state = dynamic_cast<osss_state*>(objs[i]))
          {
            forbidden_sub_behs.push_back(state);
          }
          else if(osss_pipeline_stage* pipe_s = 
                  dynamic_cast<osss_pipeline_stage*>(objs[i]))
          {
            forbidden_sub_behs.push_back(pipe_s);
          } 
        }
        break;
      }
      case OSSS_PIPE_BEHAVIOUR :
      {
        // check: contains only pipeline stage behaviours
        for(unsigned int i=0; i<objs.size(); ++i)
        {
          if(osss_behaviour* beh = dynamic_cast<osss_behaviour*>(objs[i]))
          {
            osss_pipeline_stage* pipe_st = 
              dynamic_cast<osss_pipeline_stage*>(beh);
            if(pipe_st == NULL)
              forbidden_sub_behs.push_back(beh);
          }
        }
        break;
      }
      case OSSS_FSM_BEHAVIOUR :
      {
        // check: contains only state behaviours AND
        //        a single initial state AND
        //        at least a singel end state

        // build vector of all contained states
        std::vector<osss::osss_state*> state_vec;
        osssi::osss_fsm_behaviour* fsm_beh = 
          dynamic_cast<osssi::osss_fsm_behaviour*>(m_composite_beh);
        for(unsigned int i=0; i<objs.size(); ++i)
        {
          if(osss_behaviour* beh = dynamic_cast<osss_behaviour*>(objs[i]))
          {
            osss::osss_state* state = dynamic_cast<osss::osss_state*>(beh);
            if (state != NULL)
              state_vec.push_back(state);
            else
              forbidden_sub_behs.push_back(beh);
          }
        }
    
        // search state vector for a single initial state and 
        // make it the active state after end of elaboration
        for(unsigned int i=0; i<state_vec.size(); ++i)
        {
          if((fsm_beh->m_active_state == NULL) && 
             state_vec[i]->initial())
            fsm_beh->m_active_state = state_vec[i];
          else if((fsm_beh->m_active_state != NULL) && 
                  state_vec[i]->initial())
          {
            std::cerr << "Error: Behaviour " 
                      << this->name() << " ["
                      << this->type() << "] "
                      << " contains more than a single initial state!"
                      << std::endl;
            exit(1);
          }
        }
        if (fsm_beh->m_active_state == NULL)
        {
          std::cerr << "Error: Behaviour " 
                    << this->name() << " ["
                    << this->type() << "] "
                    << " contains no initial state!"
                    << std::endl;
          exit(1);
        }

        // search state vector for at least a single end state
        bool end_state_found = false;
        for(unsigned int i=0; i<state_vec.size(); ++i)
        {
          if(state_vec[i]->end())
          {
            end_state_found = true;
            break;
          }
        }
        if(!end_state_found)
        {
          std::cerr << "Error: Behaviour " 
                    << this->name() << " ["
                    << this->type() << "] "
                    << " contains no end state!"
                    << std::endl;
          exit(1);
        }
        break;
      }
      default:
        break;
    }

    // print warning and show all forbidden sub-behaviour that have
    // been found
    if (forbidden_sub_behs.size() != 0)
    {
      std::cout << "Warning: Behaviour " << this->type() << " "
                << this->name() << " ["
                << this->type() << "] "
                << " contains the following forbidden sub-behaviours:"
                << std::endl;
      for(unsigned int i=0; i<forbidden_sub_behs.size(); ++i)
      {
        std::cout << "  " << forbidden_sub_behs[i]->name() 
                  << "[" << forbidden_sub_behs[i]->type() << "]" 
                  << std::endl;
      }
    }
  }

  //------------------------------------------------------------------
  // build dot graph
  //------------------------------------------------------------------
  void osss_behaviour::print_graph(const std::string& file_name, 
                                   bool with_channels = false)
  {
    std::ofstream ostr;
    ostr.open (file_name.c_str());

    ostr << "digraph " << this->basename()
         << "_graph {" << std::endl
         << "graph [center = true rankdir = LR];" << std::endl
         << "compound = true;" << std::endl;
    

    unsigned int indent = 1;
    this->print_behaviour( ostr, this, indent, with_channels );
    
    ostr << "}" << std::endl; // write digraph closing bracket

    ostr.close();
  }

#define mk_indent(ostr, indent) \
  for(unsigned int i_of_mk_indent_=0; \
      i_of_mk_indent_<indent; \
      ++i_of_mk_indent_) \
    ostr << "  "; \
  ostr

  void osss_behaviour::print_behaviour(std::ofstream& ostr,
                                       const osss_behaviour* beh,
                                       unsigned int& indent,
                                       bool with_channels)
  {
    // open new subgraph
    mk_indent(ostr, indent) << "subgraph " << "cluster_"
                            << str_replace(".", "_", beh->name())
                            << " {"
                            << std::endl;
    if(with_channels)
      print_channels(ostr, beh, indent);
    
    switch(beh->m_behaviour_type)
    {
      case OSSS_LEAF_BEHAVIOUR : 
      {
        // we have reached a leaf: just label it with the behaviour name
        mk_indent(ostr, indent+1) << "label = \""
                                  << beh->basename();
        // check for pipeline stage
        if(dynamic_cast<const osss_pipeline_stage*>(beh))
          ostr << " [PIPELINE_STAGE]";
        else if (const osss_state* st = dynamic_cast<const osss_state*>(beh))
        {
          if (st->initial())
            ostr << " [INITIAL_STATE]";
          else if (st->end())
            ostr << " [END_STATE]";
          else
            ostr << " [STATE]";
        }
        ostr <<  "\";" << std::endl;
        mk_indent(ostr, indent+1) << "node [style = solid, shape = ellipse];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << str_replace(".", "_", beh->name()) 
                                  << " [label = \"main\"];"
                                  << std::endl;
        break;
      }
      case OSSS_SEQ_BEHAVIOUR :
      {
        mk_indent(ostr, indent+1) << "label = \""
                                  << beh->basename() << " [SEQ]\";"
                                  << std::endl;
        mk_indent(ostr, indent+1) << "node [style = dashed, shape = ellipse];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << str_replace(".", "_", beh->name()) 
                                  << " [label = \"main\"];"
                                  << std::endl;
        
        // recusively descend into sub-behaviours
        std::vector<osss_behaviour*>& seq_behs = 
          beh->m_composite_beh->behaviour_list();
        indent++;
        for(unsigned int i=0; i<seq_behs.size(); ++i)
          print_behaviour( ostr, seq_behs[i], indent, with_channels );
        indent--;
        
        // draw sequence of sequential behaviours
        mk_indent(ostr, indent+1) << "edge [style = solid, color = black];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << "";
        for(unsigned int i=0; i<seq_behs.size(); ++i)
        {
          ostr << str_replace(".", "_", seq_behs[i]->name());
          if(i<(seq_behs.size()-1))
            ostr << " -> ";
        }
        ostr << std::endl;
        break;
      }
      case OSSS_PAR_BEHAVIOUR :
      {
        mk_indent(ostr, indent+1) << "label = \""
                                  << beh->basename() << " [PAR]\";"
                                  << std::endl;
        mk_indent(ostr, indent+1) << "node [style = dashed, shape = ellipse];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << str_replace(".", "_", beh->name()) 
                                  << " [label = \"main\"];"
                                  << std::endl;

        // recusively descend into sub-behaviours
        std::vector<osss_behaviour*>& par_behs = 
          beh->m_composite_beh->behaviour_list();
        indent++;
        for(unsigned int i=0; i<par_behs.size(); ++i)
          print_behaviour( ostr, par_behs[i], indent, with_channels );
        indent--;
        
        break;
      }
      case OSSS_PIPE_BEHAVIOUR :
      {
        mk_indent(ostr, indent+1) << "label = \""
                                  << beh->basename() << " [PIPE]\";"
                                  << std::endl;
        mk_indent(ostr, indent+1) << "node [style = dashed, shape = ellipse];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << str_replace(".", "_", beh->name()) 
                                  << " [label = \"main\"];"
                                  << std::endl;

        // recusively descend into sub-behaviours
        std::vector<osss_behaviour*>& pipe_behs = 
          beh->m_composite_beh->behaviour_list();
        indent++;
        for(unsigned int i=0; i<pipe_behs.size(); ++i)
          print_behaviour( ostr, pipe_behs[i], indent, with_channels );
        indent--;
        
        // draw sequence of pipeline behaviours
        mk_indent(ostr, indent+1) << "edge [style = solid, color = black];" 
                                  << std::endl;
        if (pipe_behs.size() > 1)
        {
          for(unsigned int i=0; i<pipe_behs.size()-1; ++i)
          {
            mk_indent(ostr, indent+1) << str_replace(".", "_", 
                                                     pipe_behs[i]->name())
                                      << " -> "
                                      << str_replace(".", "_", 
                                                     pipe_behs[i+1]->name())
                                      << " [ltail = " 
                                      << "cluster_" 
                                      << str_replace(".", "_", pipe_behs[i]->name())
                                      << ", lhead = "
                                      << "cluster_" 
                                      << str_replace(".", "_", pipe_behs[i+1]->name())
                                      << "];"
                                      << std::endl;
          }
        }
        break;
      }
      case OSSS_FSM_BEHAVIOUR :
      {
        mk_indent(ostr, indent+1) << "label = \""
                                  << beh->basename() << " [FSM]\";"
                                  << std::endl;
        mk_indent(ostr, indent+1) << "node [style = dashed, shape = ellipse];" 
                                  << std::endl;
        mk_indent(ostr, indent+1) << str_replace(".", "_", beh->name()) 
                                  << " [label = \"main\"];"
                                  << std::endl;

        // recusively descend into sub-behaviours
        std::vector<osss_behaviour*>& fsm_behs = 
          beh->m_composite_beh->behaviour_list();
        indent++;
        for(unsigned int i=0; i<fsm_behs.size(); ++i)
          print_behaviour( ostr, fsm_behs[i], indent, with_channels );
        indent--;
                
        osssi::osss_fsm_behaviour* fsm_beh =
          dynamic_cast<osssi::osss_fsm_behaviour*>(beh->m_composite_beh);
        osssi::osss_state_transition_list& stl = 
          fsm_beh->get_state_transition_list();
        std::vector<osssi::osss_state_transition*>& trans = stl.transitions();
        // draw sequences of fsm behaviours
        mk_indent(ostr, indent+1) << "edge [style = solid, color = blue];" 
                                  << std::endl;
        for(unsigned int i=0; i<trans.size(); ++i)
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", 
                                                   trans[i]->anchor()->name())
                                    << " -> "
                                    << str_replace(".", "_", 
                                                   trans[i]->target()->name())
                                    << " [label = \"" 
                                    << trans[i]->name() << "\", fontcolor = blue]"
                                    << std::endl;
        }
        break;
      }
      default:
        break;
    }

    // close subgraph
    mk_indent(ostr, indent) << "}" << std::endl;
  }

  void osss_behaviour::print_channels(std::ofstream& ostr,
                                      const osss_behaviour* beh,
                                      unsigned int& indent)
  {
    std::vector<sc_core::sc_object*> channels;
    std::vector<sc_core::sc_object*> child_objs(beh->get_child_objects());
    for(unsigned int i=0; i<child_objs.size(); ++i)
    {
      // check for primitive channels
      if(sc_core::sc_prim_channel* prim_ch = 
         dynamic_cast<sc_core::sc_prim_channel*>(child_objs[i]))
      {
        // osss event
        if(std::string(prim_ch->kind()) == std::string("osss_event"))
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", prim_ch->name()) 
                                    << " [style = solid, shape = diamond,"
                                    << " label = \""
                                    << prim_ch->basename() 
                                    << "\"];"
                                    << std::endl;
          channels.push_back(prim_ch);
        }
        // non piped osss shared variable
        else if(std::string(prim_ch->kind()) == std::string("osss_shared_variable"))
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", prim_ch->name())
                                    << " [style = solid, shape = hexagon," 
                                    << " label = \""
                                    << prim_ch->basename() 
                                    << "\"];"
                                    << std::endl;
          channels.push_back(prim_ch);
        }
        // used-defined primitive channel
        else
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", prim_ch->name()) 
                                    << " [style = solid, shape = octagon," 
                                    << " label = \""
                                    << prim_ch->basename();
          if (std::string(prim_ch->kind()) == std::string("sc_prim_channel"))
            ostr << " [primitive]\"];";
          else
            ostr << "\\n[" << prim_ch->kind() << "]\"];";
          ostr << std::endl;
          channels.push_back(prim_ch);
        }
      }
      // check for modules
      else if(sc_core::sc_module* module =
        dynamic_cast<sc_core::sc_module*>(child_objs[i]))
      {
        // piped osss shared variable
        if(std::string(module->kind()) == std::string("osss_shared_variable"))
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", module->name())
                                    << " [style = solid, shape = hexagon," 
                                    << " label = \""
                                    << module->basename()
                                    << " [piped]\"];"
                                    << std::endl;
          channels.push_back(module);
        }
        // module with at least a single method interface 
        //  -> user defined hierarchical channel
        else if(dynamic_cast<sc_core::sc_interface*>(module))
        {
          mk_indent(ostr, indent+1) << str_replace(".", "_", module->name())
                                    << " [style = solid, shape = octagon," 
                                    << " label = \""
                                    << module->basename()
                                    << " [hierarchical]\"];"
                                    << std::endl;
          channels.push_back(module);
        }
      }
    }
    if(channels.size() > 1)
    {
      mk_indent(ostr, indent+1) << "edge [style = invis];" << std::endl;
      for(unsigned int i=0; i<channels.size()-1; ++i)
      {
        mk_indent(ostr, indent+1) << str_replace(".", "_", channels[i]->name())
                                  << " -> "
                                  << str_replace(".", "_", channels[i+1]->name());
      }
    }
  }


#undef mk_indent

  //------------------------------------------------------------------
  // PAR behaviour
  //------------------------------------------------------------------

  osss_parallel_behaviour_list&
  osss_behaviour::operator | ( const osss_behaviour& beh2 ) const
  {
    osss_parallel_behaviour_list* behl = 
      new osss_parallel_behaviour_list( *this, true );
    behl->push_back( beh2 );
    return *behl;
  }

  void osss_behaviour::osss_par(const osss_behaviour& beh )
  {
    this->osss_par( osss_parallel_behaviour_list(beh) );
  }
  
  void 
  osss_behaviour::osss_par(const osss_parallel_behaviour_list& bl )
  {
    if(!sc_core::sc_is_running())
    {
      if (m_composite_beh == NULL)
      {
        m_composite_beh = new osssi::osss_parallel_behaviour(this);
        m_behaviour_type = OSSS_PAR_BEHAVIOUR;
        for(int i=0; i<bl.size(); ++i)
        {
          osss_behaviour* beh = const_cast<osss_behaviour*>(bl.m_behaviours[i]);
          m_composite_beh->behaviour_list().push_back(beh);
        }
      }
      else
      {
        std::cerr << "Error: Behaviour " << this->name() 
                  << " has already been defined as a behaviour of type "
                  << this->type() << "." << std::endl;
        exit(1);
      }
    }
  }

  //------------------------------------------------------------------
  // SEQ behaviour
  //------------------------------------------------------------------

  osss_sequential_behaviour_list&
  osss_behaviour::operator , ( const osss_behaviour& beh2 ) const
  {
    osss_sequential_behaviour_list* behl = 
      new osss_sequential_behaviour_list( *this, true );
    behl->push_back( beh2 );
    return *behl;
  }

  void osss_behaviour::osss_seq_(const osss_behaviour& beh )
  {
    this->osss_seq_( osss_sequential_behaviour_list(beh) );
  }

  void osss_behaviour::osss_seq_(const osss_sequential_behaviour_list& bl )
  {
    if(!sc_core::sc_is_running())
    {
      if (m_composite_beh == NULL)
      {
        m_composite_beh = new osssi::osss_sequential_behaviour(this);
        m_behaviour_type = OSSS_SEQ_BEHAVIOUR;
        for(int i=0; i<bl.size(); ++i)
        {
          osss_behaviour* beh = const_cast<osss_behaviour*>(bl.m_behaviours[i]);
          m_composite_beh->behaviour_list().push_back(beh);
        }
      }
      else
      {
        std::cerr << "Error: Behaviour " << this->name() 
                  << " has already been defined as a behaviour of type "
                  << this->type() << "." << std::endl;
        exit(1);
      }
    }
  }

  //------------------------------------------------------------------
  // PIPE behaviour
  //------------------------------------------------------------------ 

  void osss_behaviour::osss_pipe(const osss_pipeline_stage& pst )
  {
    this->osss_pipe(0, osss_pipeline_behaviour_list(pst) );
  }
  
  void osss_behaviour::osss_pipe(unsigned int count, const osss_pipeline_stage& pst )
  {
    this->osss_pipe(count, osss_pipeline_behaviour_list(pst) );
  }

  void osss_behaviour::osss_pipe(const osss_pipeline_behaviour_list& bl )
  {
    this->osss_pipe(0, bl);
  }

  void osss_behaviour::osss_pipe(unsigned int count, 
                                 const osss_pipeline_behaviour_list& bl )
  {
    if(!sc_core::sc_is_running())
    {
      if (m_composite_beh == NULL)
      {
        m_composite_beh = new osssi::osss_pipeline_behaviour(this);
        m_behaviour_type = OSSS_PIPE_BEHAVIOUR;
        SC_THREAD(pipeline_step);

        for(int i=0; i<bl.size(); ++i)
        {
          osss_behaviour* beh = const_cast<osss_behaviour*>(bl.m_behaviours[i]);
          osss_pipeline_stage* ps = dynamic_cast<osss_pipeline_stage*>(beh);
          ps->num_cycles(count);
          ps->stage_nr(i);
          m_composite_beh->behaviour_list().push_back(ps);
        }
      }
      else
      {
        std::cerr << "Error: Behaviour " << this->name() 
                  << " has already been defined as a behaviour of type "
                  << this->type() << "." << std::endl;
        exit(1);
      }
    }
  }

  //------------------------------------------------------------------
  // FSM behaviour
  //------------------------------------------------------------------

  void osss_behaviour::osss_fsm(const osssi::osss_state_transition& st)
  {
    osss_fsm( osssi::osss_state_transition_list(st) );
  }

  void osss_behaviour::osss_fsm(const osssi::osss_state_transition_list& stl)
  {
    if(!sc_core::sc_is_running())
    {
      if (m_composite_beh == NULL)
      {
        m_composite_beh = new osssi::osss_fsm_behaviour(this, stl);
        m_behaviour_type = OSSS_FSM_BEHAVIOUR;
        
        std::vector<osss_behaviour*>& beh_list = 
          m_composite_beh->behaviour_list();
        std::vector<osssi::osss_state_transition*>& trans = 
          const_cast<osssi::osss_state_transition_list&>(stl).transitions();
        for(unsigned int i=0; i<trans.size(); ++i)
        {
          osss_state* a_st = const_cast<osss_state*>(trans[i]->anchor());
          // check whether this anchor state is already in the behaviour_list
          bool found = false;
          for(unsigned int j=0; j<beh_list.size(); ++j)
          {
            if (beh_list[j] == a_st)
            {
              found = true;
              break;
            }
          }
          if (!found)
            beh_list.push_back(a_st);
          
          osss_state* t_st = const_cast<osss_state*>(trans[i]->target());
           // check whether this target state is already in the behaviour_list
          found = false;
          for(unsigned int j=0; j<beh_list.size(); ++j)
          {
            if (beh_list[j] == t_st)
            {
              found = true;
              break;
            }
          }
          if (!found)
            beh_list.push_back(t_st);
        }
      }
      else
      {
        std::cerr << "Error: Behaviour " << this->name() 
                  << " has already been defined as a behaviour of type "
                  << this->type() << "." << std::endl;
        exit(1);
      }
    }
  }

} // namespace osss
