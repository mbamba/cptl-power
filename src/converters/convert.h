/*
  copyright (c) 2015, Gabriel A. Weaver, Information Trust Institute
    at the University of Illinois, Urbana-Champaign.

  This file is part of the Cyber-Physical Topology Language for the
    Power domain.

  This code is free software:  you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version.

  The Cyber-Physical Topology Language (CPTL):  Electrical Power 
    is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

  You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/
 */
#ifndef _CONVERT_H_

#include "../validators/validate.h"
#include <graph.h>
#include <mpc.h>


//-- OUTPUT METHODS
// View Formats:  JSON_NODE_LINK or RDF_TURTLE
StatusCode output_view(void* view, string view_format_type, FILE* output_file);

// Graph Formats:  NODE-LINK
StatusCode output_graph(void* view, string graph_format_type, FILE* output_file);              

// Interpretation Formats:  TABULAR
StatusCode output_interpretation(void *view, string recognizer_type, FILE* output_file);     


//---- BASE NODE LINK VIEW
//------ JSON Format
StatusCode output_view_to_json_node_link(View* view, FILE* output_file);

//------ RDF Turtle Format
StatusCode output_view_to_rdf_turtle(View* view, FILE* output_file);
					     

//-- AST METHODS
StatusCode traverse_parse_tree( mpc_ast_t *a, View *view,
				StatusCode(*graph_builder)(mpc_ast_t *a, View* view) );
//---- BASE NODE LINK VIEW
//------ JSON Format
static StatusCode json_node_link_edge_initializer(mpc_ast_t *a, View *view);
static StatusCode json_node_link_edge_initializer_helper(mpc_ast_t *a, View *view);
static StatusCode json_node_link_vertex_initializer(mpc_ast_t *a, View *view);
static StatusCode json_node_link_vertex_initializer_helper(mpc_ast_t *a, View *view);

//------ TTL Format
static StatusCode rdf_turtle_edge_initializer(mpc_ast_t *a, View *view);
static StatusCode rdf_turtle_edge_initializer_helper(mpc_ast_t *a, View *view);
static StatusCode rdf_turtle_vertex_initializer(mpc_ast_t *a, View *view);
static StatusCode rdf_turtle_vertex_initializer_helper(mpc_ast_t *a, View *view);

#endif
