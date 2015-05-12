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

StatusCode use_converter(FormatType format_type,
			 View *view,
			 ViewType view_type,
			 mpc_result_t *r);

//-- INPUT FORMAT PARSERS
//---- General Purpose Methods
StatusCode traverse_parse_tree( mpc_ast_t *a, View *view,
				StatusCode(*graph_builder)(View *view, mpc_ast_t *a) );

static StatusCode json_node_link_ast_edge_initializer(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode json_node_link_ast_edge_initializer_helper(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode json_node_link_ast_vertex_initializer(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode json_node_link_ast_vertex_initializer_helper(mpc_ast_t *a, View *view, ViewType view_type);

static StatusCode rdf_turtle_ast_edge_initializer(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode rdf_turtle_ast_edge_initializer_helper(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode rdf_turtle_ast_vertex_initializer(mpc_ast_t *a, View *view, ViewType view_type);
static StatusCode rdf_turtle_ast_vertex_initializer_helper(mpc_ast_t *a, View *view, ViewType view_type);

//---- VIEW FORMATS
//------ Process Communications View
static StatusCode json_node_link_process_communications_view_initializer(View *view, mpc_result_t *r);
static StatusCode json_node_link_process_communications_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode json_node_link_process_communications_view_edge_initializer(View *view, mpc_ast_t *a);

//------ Substation Network View
static StatusCode json_node_link_substation_network_view_initializer(View *view, mpc_result_t *r);
static StatusCode json_node_link_substation_network_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode json_node_link_substation_network_view_edge_initializer(View *view, mpc_ast_t *a);

static StatusCode rdf_turtle_substation_network_view_initializer(View *view, mpc_result_t *r);
static StatusCode rdf_turtle_substation_network_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode rdf_turtle_substation_network_view_edge_initializer(View *view, mpc_ast_t *a);


//------ Substation Yard View
static StatusCode json_node_link_substation_yard_view_initializer(View *view, mpc_result_t *r);
static StatusCode json_node_link_substation_yard_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode json_node_link_substation_yard_view_edge_initializer(View *view, mpc_ast_t *a);

static StatusCode rdf_turtle_substation_yard_view_initializer(View *view, mpc_result_t *r);
static StatusCode rdf_turtle_substation_yard_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode rdf_turtle_substation_yard_view_edge_initializer(View *view, mpc_ast_t *a);


//---- OTHER FORMATS
//------ Process Tree Output
static StatusCode pstree_output_process_tree_view_initializer(View *view, mpc_result_t *r);
static StatusCode pstree_output_process_tree_view_vertex_initializer(View *view, mpc_ast_t *a);
static StatusCode pstree_output_process_tree_view_edge_initializer(View *view, mpc_ast_t *a);


//-- OUTPUT FORMAT SERIALIZERS
//---- General Purpose Methods

StatusCode output_graph(void* view, FormatType format_type, FILE* output_file);

StatusCode output_interpretation(void *view, string recognizer_type, FILE* output_file);     

StatusCode output_view(View* view, FormatType format_type, ViewType view_type, FILE* output_file);

StatusCode output_view_to_json_node_link(View* view, FILE* output_file);

StatusCode output_view_to_rdf_turtle(View* view, FILE* output_file);


//------ Process Communications View
StatusCode output_json_node_link_process_communications_view(View* view, FILE* output_file);

//------ Process Tree View
StatusCode output_json_node_link_process_tree_view(View* view, FILE* output_file);
StatusCode output_rdf_turtle_process_tree_view(View *view, FILE* output_file);

//------ Substation Network View
StatusCode output_json_node_link_substation_network_view(View* view, FILE* output_file);
StatusCode output_rdf_turtle_substation_network_view(View* view, FILE* output_file);

//------ Substation Yard View
StatusCode output_json_node_link_substation_yard_view(View* view, FILE* output_file);
StatusCode output_rdf_turtle_substation_yard_view(View* view, FILE* output_file);

#endif
