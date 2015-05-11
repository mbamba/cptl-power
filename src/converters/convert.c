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

#include "../converters/convert.h"
#include <graph.h>
#include <mpc.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>


//-- OUTPUT FORMAT SERIALIZERS
//---- General Purpose Methods

StatusCode output_interpretation(void* view, string recognizer_type, FILE* output_file) {
  StatusCode status = OK;
  int num_vertices, i;
  struct v_attribute_entry *v_attributes;

  View* v = (View*)view;
  
  // Iterate through the vertices
  // In the future, specify the fields we want
  num_vertices = v->vertex_count;  //graph_vertex_count(v->graph);
  fprintf( output_file, "%2s\t%30s\t%30s\n", "id", "name", "rdfs:type" );
  for (i=0; i < num_vertices; i++) {
    HASH_FIND_INT(v->v_interpretation, &i, v_attributes);
    if ( NULL == v_attributes ) {
      fprintf( stderr, "No interpretation for vertex %d\n", i );
      exit(NO_INTERPRETATION_ERROR);
    }
    fprintf( output_file, "%2d\t%30s\t%30s\n", i, v_attributes->name, v_attributes->rdfs_type );
  }

  fprintf( output_file, "%2s\t%2s\t%30s\n", "source_id", "target_id", "rdfs:type" );
  
  // Iterate through the edges
  struct vertex_adjacency_list *vertex_adjacency_list;
  struct e_attribute_entry *e_attr_entry;
  struct v_attribute_entry *v_attr_entry;
  int source_id, target_id;
  struct v_attribute_entry *source_v_attr_entry, *target_v_attr_entry;

  for (i=0; i < num_vertices; i++) {
    source_id = i;

    HASH_FIND_INT(v->e_interpretation, &source_id, vertex_adjacency_list);
    if ( NULL == vertex_adjacency_list ) {
      fprintf( stderr, "No adjacency list for source vertex: %d\n", source_id);
      exit(NO_INTERPRETATION_ERROR);
    }

    HASH_FIND_INT(v->v_interpretation, &source_id, source_v_attr_entry);
    if ( source_v_attr_entry == NULL ) {
      fprintf( stderr, "No interpretation for vertex %d\n", source_id );
      exit(NO_INTERPRETATION_ERROR);
    }

    // Loop through all the target vertices
    e_attr_entry = vertex_adjacency_list->targets;
    while ( e_attr_entry != NULL ) {
      target_id = e_attr_entry->target_id;

      fprintf(output_file, "%9d\t%9d\t%30s\n", source_id, target_id,
	      e_attr_entry->rdfs_type);
      e_attr_entry = e_attr_entry->next;
    }
  }
  
  return status;
}

StatusCode output_view(View *view,
		       FormatType format_type,
		       ViewType view_type,
		       FILE *output_file) {
  StatusCode status = OK;

  if ( JSON_NODE_LINK == format_type ) {
    if ( PROCESS_COMS_VIEW == view_type ) {
      status = output_json_node_link_process_communications_view(view, output_file);
    } else if ( PROCESS_TREE_VIEW == view_type ) {
      status = output_json_node_link_process_tree_view(view, output_file);
    } else if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = output_json_node_link_substation_network_view(view, output_file);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = output_json_node_link_substation_yard_view(view, output_file);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
      print_usage();
      status = UNKNOWN_VIEW_TYPE;
      exit(status);
    }
    
  } else if ( RDF_TURTLE == format_type ) {
    if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = output_rdf_turtle_substation_network_view(view, output_file);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = output_rdf_turtle_substation_yard_view(view, output_file);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
    }
  } else {
    fprintf(stderr, "Unknown view type: %d\n", view_type);
    print_usage();
    status = UNKNOWN_FORMAT_TYPE;
    exit(status);
  }
  return status;
}

//------ Process Communications View 
StatusCode output_json_node_link_process_communications_view(View *view, FILE* output_file) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  output_json_node_link_process_communications_view\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

//------ Process Tree View
StatusCode output_json_node_link_process_tree_view(View *view, FILE* output_file) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  output_json_node_link_process_tree_view\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

//------ Substation Network View
StatusCode output_json_node_link_substation_network_view(View *view, FILE* output_file) {
  StatusCode status = OK;  
  int i;
  int num_vertices;
  struct v_attribute_entry *v_attributes;
  View* v = (View*)view;
  
  fprintf(output_file, "{\n");

  // Print Nodes
  fprintf(output_file, "\t\"nodes\":\n");
  fprintf(output_file, "\t   [\n");

  num_vertices = v->vertex_count; //graph_vertex_count(view->graph);
  for (i=0; i < num_vertices; i++) {
    HASH_FIND_INT(view->v_interpretation, &i, v_attributes);
    if ( NULL == v_attributes ) {
      fprintf( stderr, "No interpretation for vertex %d\n", i );
      exit(NO_INTERPRETATION_ERROR);
    }
    fprintf(output_file, "\t\t{\"name\":\"%s\",\n", v_attributes->name);
    fprintf(output_file, "\t\t \"rdfs:type\":\"%s\"}", v_attributes->rdfs_type);

    if (i != num_vertices - 1) {
      fprintf(output_file, ",\n");
    }
    fprintf(output_file, "\n");
  }
  fprintf(output_file, "\t   ],\n");
  
  // Print Link Dictionary
  fprintf(output_file, "\t\"links\":\n");
  fprintf(output_file, "\t   [\n");

  struct vertex_adjacency_list *vertex_adjacency_list;
  struct e_attribute_entry *e_attr_entry;
  struct v_attribute_entry *v_attr_entry;
  int source_id, target_id;
  struct v_attribute_entry *source_v_attr_entry, *target_v_attr_entry;
  
  for (i=0; i < num_vertices; i++) {
    source_id = i;
    
    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    if ( NULL == vertex_adjacency_list ) {
      fprintf( stderr, "No adjacency list for source vertex: %d\n", source_id);
      exit(NO_INTERPRETATION_ERROR);
    }

    HASH_FIND_INT(view->v_interpretation, &source_id, source_v_attr_entry);
    if ( source_v_attr_entry == NULL ) {
      fprintf( stderr, "No interpretation for vertex %d\n", source_id );
      exit(NO_INTERPRETATION_ERROR);
    }

    // Loop through all the target vertices
    e_attr_entry = vertex_adjacency_list->targets;
    while ( e_attr_entry != NULL ) {
      target_id = e_attr_entry->target_id;

      // Resolve the names
      HASH_FIND_INT(view->v_interpretation, &target_id, target_v_attr_entry);
      if ( target_v_attr_entry == NULL ) {
	fprintf( stderr, "No interpretation for vertex %d\n", target_id );
	exit(NO_INTERPRETATION_ERROR);
      }
      
      fprintf(output_file, "\t\t{\"source\":\"%s\",\n", source_v_attr_entry->name);
      fprintf(output_file, "\t\t \"target\":\"%s\",\n", target_v_attr_entry->name);
      fprintf(output_file, "\t\t \"relation\":\"%s\"}", e_attr_entry->rdfs_type);

      e_attr_entry = e_attr_entry->next;
      fprintf(output_file, ",\n\n");
    }
  }

  fprintf(output_file, "\t   ]\n");  
  fprintf(output_file, "}\n");
  return status;
}

StatusCode output_rdf_turtle_substation_network_view(View *view, FILE* output_file) {
  StatusCode status = OK;

  struct v_attribute_entry *v_attr_entry;

  int i;
  int num_vertices;
  View* v = (View*)view;
  
  num_vertices = v->vertex_count; //graph_vertex_count(view->graph);

  // Not sure how to output the namespaces well just yet.
  // Iterate through the adjacency list
  struct vertex_adjacency_list *vertex_adjacency_list;
  struct e_attribute_entry *e_attr_entry;
  int source_id, target_id;
  struct v_attribute_entry *source_v_attr_entry, *target_v_attr_entry;
  
  for (i=0; i < num_vertices; i++) {
    source_id = i;

    // Get the adjacency list for the source vertex
    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    if ( NULL == vertex_adjacency_list ) {
      fprintf( stderr, "No adjacency list for source vertex: %d\n", source_id);
      exit(NO_INTERPRETATION_ERROR);
    }

    // Get the attribute information for the source vertex
    HASH_FIND_INT(view->v_interpretation, &source_id, source_v_attr_entry);
    if ( source_v_attr_entry == NULL ) {
      fprintf( stderr, "No interpretation for vertex %d\n", source_id );
      exit(NO_INTERPRETATION_ERROR);
    }


    // Output the desired information
    fprintf(output_file, "\t%s\n", source_v_attr_entry->name);
    fprintf(output_file, "\t\t%s \"%s\" ;\n", "rdfs:type", source_v_attr_entry->rdfs_type);
    e_attr_entry = vertex_adjacency_list->targets;
    while ( e_attr_entry != NULL ) {
      target_id = e_attr_entry->target_id;
      HASH_FIND_INT(view->v_interpretation, &target_id, target_v_attr_entry);
      if ( target_v_attr_entry == NULL ) {
	fprintf( stderr, "No interpretation for vertex %d\n", target_id );
	exit(NO_INTERPRETATION_ERROR);
      }
      
      fprintf(output_file, "\t\t%s %s ;\n", e_attr_entry->rdfs_type, target_v_attr_entry->name);
      e_attr_entry = e_attr_entry->next;
    }
    fprintf(output_file, "\t\t.\n\n");
  }
  return status;
}

//------ Substation Yard View
StatusCode output_json_node_link_substation_yard_view(View *view, FILE* output_file) {
  StatusCode status = OK;  
  status = output_json_node_link_substation_network_view(view, output_file);
  return status;
}

StatusCode output_rdf_turtle_substation_yard_view(View *view, FILE* output_file) {
  StatusCode status = OK;
  status = output_rdf_turtle_substation_network_view(view, output_file);
  return status;
}

//---- BASE NODE LINK VIEW
//------ JSON Format
StatusCode output_view_to_json_node_link(View* view, FILE* output_file) {
  StatusCode status = OK;  
  int i;
  int num_vertices;
  struct v_attribute_entry *v_attributes;
  View* v = (View*)view;
  
  fprintf(output_file, "{\n");

  // Print Nodes
  fprintf(output_file, "\t\"nodes\":\n");
  fprintf(output_file, "\t   [\n");

  num_vertices = v->vertex_count; //graph_vertex_count(view->graph);
  for (i=0; i < num_vertices; i++) {
    HASH_FIND_INT(view->v_interpretation, &i, v_attributes);
    if ( NULL == v_attributes ) {
      fprintf( stderr, "No interpretation for vertex %d\n", i );
      exit(NO_INTERPRETATION_ERROR);
    }
    fprintf(output_file, "\t\t{\"name\":\"%s\",\n", v_attributes->name);
    fprintf(output_file, "\t\t \"rdfs:type\":\"%s\"}", v_attributes->rdfs_type);

    if (i != num_vertices - 1) {
      fprintf(output_file, ",\n");
    }
    fprintf(output_file, "\n");
  }
  fprintf(output_file, "\t   ],\n");
  
  // Print Link Dictionary
  fprintf(output_file, "\t\"links\":\n");
  fprintf(output_file, "\t   [\n");

  struct vertex_adjacency_list *vertex_adjacency_list;
  struct e_attribute_entry *e_attr_entry;
  struct v_attribute_entry *v_attr_entry;
  int source_id, target_id;
  struct v_attribute_entry *source_v_attr_entry, *target_v_attr_entry;
  
  for (i=0; i < num_vertices; i++) {
    source_id = i;
    
    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    if ( NULL == vertex_adjacency_list ) {
      fprintf( stderr, "No adjacency list for source vertex: %d\n", source_id);
      exit(NO_INTERPRETATION_ERROR);
    }

    HASH_FIND_INT(view->v_interpretation, &source_id, source_v_attr_entry);
    if ( source_v_attr_entry == NULL ) {
      fprintf( stderr, "No interpretation for vertex %d\n", source_id );
      exit(NO_INTERPRETATION_ERROR);
    }

    // Loop through all the target vertices
    e_attr_entry = vertex_adjacency_list->targets;
    while ( e_attr_entry != NULL ) {
      target_id = e_attr_entry->target_id;

      // Resolve the names
      HASH_FIND_INT(view->v_interpretation, &target_id, target_v_attr_entry);
      if ( target_v_attr_entry == NULL ) {
	fprintf( stderr, "No interpretation for vertex %d\n", target_id );
	exit(NO_INTERPRETATION_ERROR);
      }
      
      fprintf(output_file, "\t\t{\"source\":\"%s\",\n", source_v_attr_entry->name);
      fprintf(output_file, "\t\t \"target\":\"%s\",\n", target_v_attr_entry->name);
      fprintf(output_file, "\t\t \"relation\":\"%s\"}", e_attr_entry->rdfs_type);

      e_attr_entry = e_attr_entry->next;
      fprintf(output_file, ",\n\n");
    }
  }

  fprintf(output_file, "\t   ]\n");  
  fprintf(output_file, "}\n");
  return status;
}

//------ RDF Turtle Format
StatusCode output_view_to_rdf_turtle(View *view, FILE* output_file) {

  StatusCode status = OK;

  struct v_attribute_entry *v_attr_entry;

  int i;
  int num_vertices;
  View* v = (View*)view;
  
  num_vertices = v->vertex_count; //graph_vertex_count(view->graph);

  // Not sure how to output the namespaces well just yet.
  // Iterate through the adjacency list
  struct vertex_adjacency_list *vertex_adjacency_list;
  struct e_attribute_entry *e_attr_entry;
  int source_id, target_id;
  struct v_attribute_entry *source_v_attr_entry, *target_v_attr_entry;
  
  for (i=0; i < num_vertices; i++) {
    source_id = i;

    // Get the adjacency list for the source vertex
    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    if ( NULL == vertex_adjacency_list ) {
      fprintf( stderr, "No adjacency list for source vertex: %d\n", source_id);
      exit(NO_INTERPRETATION_ERROR);
    }

    // Get the attribute information for the source vertex
    HASH_FIND_INT(view->v_interpretation, &source_id, source_v_attr_entry);
    if ( source_v_attr_entry == NULL ) {
      fprintf( stderr, "No interpretation for vertex %d\n", source_id );
      exit(NO_INTERPRETATION_ERROR);
    }


    // Output the desired information
    fprintf(output_file, "\t%s\n", source_v_attr_entry->name);
    fprintf(output_file, "\t\t%s %s ;\n", "rdfs:type", source_v_attr_entry->rdfs_type);
    e_attr_entry = vertex_adjacency_list->targets;
    while ( e_attr_entry != NULL ) {
      target_id = e_attr_entry->target_id;
      HASH_FIND_INT(view->v_interpretation, &target_id, target_v_attr_entry);
      if ( target_v_attr_entry == NULL ) {
	fprintf( stderr, "No interpretation for vertex %d\n", target_id );
	exit(NO_INTERPRETATION_ERROR);
      }
      
      fprintf(output_file, "\t\t%s %s ;\n", e_attr_entry->rdfs_type, target_v_attr_entry->name);
      e_attr_entry = e_attr_entry->next;
    }
    fprintf(output_file, "\t\t.\n\n");
  }
  return status;
}

//-- AST METHODS
StatusCode traverse_parse_tree( mpc_ast_t *a,
				View *view,
				StatusCode(*graph_builder)(View* view, mpc_ast_t *a) ) {
  StatusCode status = OK;
  int i;
  
  graph_builder(view, a);
  for ( i = 0; i < a->children_num; i++) {
    status = traverse_parse_tree(a->children[i], view, graph_builder);
  }
  return status;
}

//---- BASE NODE LINK VIEW
//------ JSON Format
/*
static StatusCode json_node_link_edge_initializer(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  status = json_node_link_edge_initializer_helper(a, view);
  return status;
}

static StatusCode json_node_link_edge_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;

  struct v_attribute_entry *source_attr_entry, *target_attr_entry;
  string relation_name;
  int source_id, target_id;

  if ( !strncmp("link_dictionary", a->tag, 15) ) {
    int rc;

    mpc_ast_t * source_name_value_node = a->children[3]->children[1];
    mpc_ast_t * target_name_value_node = a->children[7]->children[1];
    mpc_ast_t * relation_name_value_node = a->children[11]->children[1];

    HASH_FIND_STR(view->v_inverse_interpretation, source_name_value_node->contents, source_attr_entry);
    if ( source_attr_entry == NULL ) {
      fprintf(stderr, "Edge Initializer:  Could not find vertex with name %s\n", source_name_value_node->contents);
      exit(NO_INTERPRETATION_ERROR);
    }

    HASH_FIND_STR(view->v_inverse_interpretation, target_name_value_node->contents, target_attr_entry);
    if ( target_attr_entry == NULL ) {
      fprintf(stderr, "Edge Initializer 2:  Could not find vertex with name %s\n", target_name_value_node->contents);
      exit(NO_INTERPRETATION_ERROR);
    }

    source_id = source_attr_entry->id;
    target_id = target_attr_entry->id;
    
    struct vertex_adjacency_list *vertex_adjacency_list = NULL;
    struct e_attribute_entry *e_attr_entry = NULL;

    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    e_attr_entry = vertex_adjacency_list->targets;
    
    if ( NULL == e_attr_entry ) {
      e_attr_entry = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry->target_id = target_id;
      strncpy(e_attr_entry->rdfs_type, relation_name_value_node->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
      vertex_adjacency_list->targets = e_attr_entry;
    } else {
      while ( e_attr_entry->next != NULL ) {
	e_attr_entry = e_attr_entry->next;
      }
      e_attr_entry->next = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry = e_attr_entry->next;
      e_attr_entry->target_id = target_id;
      strncpy(e_attr_entry->rdfs_type, relation_name_value_node->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
    }
    
  }
  return status;
  }*/

/*static StatusCode rdf_turtle_edge_initializer(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  status = rdf_turtle_edge_initializer_helper(a, view);
  return status;
}

static StatusCode rdf_turtle_edge_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;

  struct v_attribute_entry *source_attr_entry, *target_attr_entry;
  string relation_name;
  int source_id, target_id;

  if ( !strncmp("subject", a->tag, 7) ) {
    // Set the current source ID 
    mpc_ast_t * source_name_value_node = a;
    struct v_attribute_entry *source_attr_entry = NULL;
    
    HASH_FIND_STR( view->v_inverse_interpretation, source_name_value_node->contents, source_attr_entry);
    if ( source_attr_entry == NULL ) {
      fprintf(stderr, "Edge Initializer:  Could not find vertex with name %s\n", source_name_value_node->contents);
      exit(NO_INTERPRETATION_ERROR);
    }
    view->current_source_id = source_attr_entry->id;
  } else if ( !strncmp("predicate", a->tag, 9) ) {
    //-- retrieve the vertex adjacency list for the current source ID
    struct vertex_adjacency_list *vertex_adjacency_list = NULL;
    struct e_attribute_entry *e_attr_entry = NULL;
    HASH_FIND_INT(view->e_interpretation, &view->current_source_id, vertex_adjacency_list);
    e_attr_entry = vertex_adjacency_list->targets;

    //-- Iterate until e_attr_entry->next is NULL
    if ( NULL == e_attr_entry ) {
      e_attr_entry = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      strncpy(e_attr_entry->rdfs_type, a->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
      vertex_adjacency_list->targets = e_attr_entry;
    } else {
      while ( e_attr_entry->next != NULL ) {
	e_attr_entry = e_attr_entry->next;
      }
      e_attr_entry->next = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry = e_attr_entry->next;
      strncpy(e_attr_entry->rdfs_type, a->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
    }
    
  } else if ( !strncmp("object", a->tag, 6) ) {
    //-- retrieve the vertex adjacency list for the current source ID
    struct vertex_adjacency_list *vertex_adjacency_list = NULL;
    struct e_attribute_entry *e_attr_entry = NULL;
    struct v_attribute_entry *target_attr_entry = NULL;
    mpc_ast_t * target_name_value_node = a;

    HASH_FIND_INT(view->e_interpretation, &view->current_source_id, vertex_adjacency_list);
    e_attr_entry = vertex_adjacency_list->targets;

    // Iterate until e_attr_entry->next is NULL
    while ( e_attr_entry->next != NULL ) {
      e_attr_entry = e_attr_entry->next;
    }
    HASH_FIND_STR( view->v_inverse_interpretation, target_name_value_node->contents, target_attr_entry);
    e_attr_entry->target_id = target_attr_entry->id;
  }
  
  return status;
}
*/

/*
static StatusCode json_node_link_vertex_initializer(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  status = json_node_link_vertex_initializer_helper(a, view);
  return status;
}

static StatusCode json_node_link_vertex_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  
  if ( !strncmp("node_dictionary", a->tag, 15) ) {
    int rc;

    mpc_ast_t * name_value_node = a->children[3]->children[1];
    mpc_ast_t * rdfs_type_value_node = a->children[7]->children[1];

    string name, rdfs_type;
    strncpy(name, name_value_node->contents, STRING_SIZE);
    strncpy(rdfs_type, rdfs_type_value_node->contents, STRING_SIZE);

    int vertex_id;
    vertex_id = view->vertex_count;

    struct v_attribute_entry *v_attr_entry;
    struct v_attribute_entry *v_inverse_attr_entry;
    
    v_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));
    v_inverse_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));

    // Initialize the vertex attribute maps
    v_attr_entry->id = view->vertex_count;
    v_inverse_attr_entry->id = view->vertex_count;
    
    strncpy(v_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    strncpy(v_inverse_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    
    strncpy(v_attr_entry->name, name, STRING_SIZE);
    strncpy(v_inverse_attr_entry->name, name, STRING_SIZE);
    
    HASH_ADD_INT( view->v_interpretation, id, v_attr_entry);
    HASH_ADD_STR( view->v_inverse_interpretation, name, v_inverse_attr_entry);

    // Initialize the vertex adjacency list
    struct vertex_adjacency_list *vertex_adjacency_list;
    vertex_adjacency_list = (struct vertex_adjacency_list*)malloc(sizeof(struct vertex_adjacency_list));
    vertex_adjacency_list->source_id = vertex_id;
    vertex_adjacency_list->targets = NULL;
    HASH_ADD_INT( view->e_interpretation, source_id, vertex_adjacency_list );
    
    view->vertex_count = view->vertex_count + 1;
  } 
  return status;
}
*/

/*static StatusCode rdf_turtle_vertex_initializer(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  status = rdf_turtle_vertex_initializer_helper(a, view);
  return status;
}

static StatusCode rdf_turtle_vertex_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;

  if ( !strncmp("rdf_triple_block", a->tag, 16) ) {
    int rc;

    mpc_ast_t * name_value_node = a->children[0];
    mpc_ast_t * rdfs_type_value_node = a->children[2];

    string name, rdfs_type;
    strncpy(name, name_value_node->contents, STRING_SIZE);
    strncpy(rdfs_type, rdfs_type_value_node->contents, STRING_SIZE);

    int vertex_id;
    vertex_id = view->vertex_count;

    struct v_attribute_entry *v_attr_entry;
    struct v_attribute_entry *v_inverse_attr_entry;

    v_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));
    v_inverse_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));

    // Initialize the vertex attribute maps
    v_attr_entry->id = view->vertex_count;
    v_inverse_attr_entry->id = view->vertex_count;
    
    strncpy(v_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    strncpy(v_inverse_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    
    strncpy(v_attr_entry->name, name, STRING_SIZE);
    strncpy(v_inverse_attr_entry->name, name, STRING_SIZE);
    
    HASH_ADD_INT( view->v_interpretation, id, v_attr_entry);
    HASH_ADD_STR( view->v_inverse_interpretation, name, v_inverse_attr_entry);

    // Initialize the vertex adjacency list
    struct vertex_adjacency_list *vertex_adjacency_list;
    vertex_adjacency_list = (struct vertex_adjacency_list*)malloc(sizeof(struct vertex_adjacency_list));
    vertex_adjacency_list->source_id = vertex_id;
    vertex_adjacency_list->targets = NULL;
    HASH_ADD_INT( view->e_interpretation, source_id, vertex_adjacency_list );
    
    view->vertex_count = view->vertex_count + 1;
  }
  return status;
}
*/
//-- Process Tree Output

static StatusCode process_tree_vertex_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;

  if ( ! strncmp(">", a->tag, 1) ||
       ! strncmp("tree", a->tag, 4) ||
       ! strncmp("leaf_node", a->tag, 9) ) {
    int rc;

    
    mpc_ast_t * pid_value_node = a->children[2];
    mpc_ast_t * username_value_node = a->children[3];
    mpc_ast_t * command_value_node = a->children[4];

    string name, rdfs_type, username, command;
    strncpy(name, pid_value_node->contents, STRING_SIZE);
    strncpy(rdfs_type, "devos:Process", STRING_SIZE);
    strncpy(username, username_value_node->contents, STRING_SIZE);
    strncpy(command, command_value_node->contents, STRING_SIZE);
    
    int vertex_id;
    vertex_id = view->vertex_count;
    
    struct v_attribute_entry *v_attr_entry;
    struct v_attribute_entry *v_inverse_attr_entry;

    v_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));
    v_inverse_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));

    // Initialize the vertex attribute maps
    v_attr_entry->id = view->vertex_count;
    v_inverse_attr_entry->id = view->vertex_count;

    strncpy(v_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    strncpy(v_inverse_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);

    strncpy(v_attr_entry->name, name, STRING_SIZE);
    strncpy(v_inverse_attr_entry->name, name, STRING_SIZE);

    strncpy(v_attr_entry->devos_username, username, STRING_SIZE);
    strncpy(v_inverse_attr_entry->devos_username, username, STRING_SIZE);

    strncpy(v_attr_entry->devos_command, command, STRING_SIZE);
    strncpy(v_inverse_attr_entry->devos_command, command, STRING_SIZE);

    HASH_ADD_INT( view->v_interpretation, id, v_attr_entry);
    HASH_ADD_STR( view->v_inverse_interpretation, name, v_inverse_attr_entry);

    // Initialize the vertex adjacency list
    struct vertex_adjacency_list *vertex_adjacency_list;
    vertex_adjacency_list = (struct vertex_adjacency_list*)malloc(sizeof(struct vertex_adjacency_list));
    vertex_adjacency_list->source_id = vertex_id;
    vertex_adjacency_list->targets = NULL;
    HASH_ADD_INT( view->e_interpretation, source_id, vertex_adjacency_list );

    view->vertex_count = view->vertex_count + 1;
  }
  return status;
}

/*static StatusCode process_tree_edge_initializer(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  status = process_tree_edge_initializer_helper(a, view);
  return status;
}

static StatusCode process_tree_edge_initializer_helper(mpc_ast_t *a, View *view) {
  StatusCode status = OK;
  int i;

  string relation_name;
  int source_id, target_id;

  //-- SOURCE NODE
  mpc_ast_t * source_name_value_node = a->children[2];
  struct vertex_adjacency_list *vertex_adjacency_list = NULL;  
  struct v_attribute_entry *source_attr_entry = NULL;
  struct v_attribute_entry *target_attr_entry = NULL;
  
  HASH_FIND_STR( view->v_inverse_interpretation, source_name_value_node->contents, source_attr_entry );
  if ( source_attr_entry == NULL ) {
    fprintf(stderr, "Edge initializer:  Could not find vertex with name %s\n", source_name_value_node->contents);
    exit(NO_INTERPRETATION_ERROR);
  }
  view->current_source_id = source_attr_entry->id;
  HASH_FIND_INT(view->e_interpretation, &view->current_source_id, vertex_adjacency_list);
  
  //-- TARGET NODE
  for ( i = 0; i < a->children_num; i++) {
    struct e_attribute_entry *e_attr_entry = NULL;
    struct v_attribute_entry *target_attr_entry = NULL;
    mpc_ast_t * child_target_node = a->children[i];

    //---- only process children that are either subtrees or leaves
    if ( strncmp("tree", child_target_node->tag, 4) &&
	 strncmp("leaf_node", child_target_node->tag, 9) ) {
      continue;
    }

    mpc_ast_t * target_name_value_node = child_target_node->children[2];
    HASH_FIND_STR( view->v_inverse_interpretation, target_name_value_node->contents, target_attr_entry);
    //---- loop through all the target vertices
    e_attr_entry = vertex_adjacency_list->targets;
    if ( NULL == e_attr_entry ) {
      e_attr_entry = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry->target_id = target_attr_entry->id;
      strncpy(e_attr_entry->rdfs_type, "devos:hasChildProcess", STRING_SIZE);
      e_attr_entry->next = NULL;
      vertex_adjacency_list->targets = e_attr_entry;
    } else {
      while ( e_attr_entry->next != NULL ) {
	e_attr_entry = e_attr_entry->next;
      }
      e_attr_entry->next = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry = e_attr_entry->next;
      e_attr_entry->target_id = target_attr_entry->id;
      strncpy(e_attr_entry->rdfs_type, "devos:hasChildProcess", STRING_SIZE);
      e_attr_entry->next = NULL;
    }
    status = process_tree_edge_initializer(a->children[i], view);
  }
  return status;
  }*/

//---- VIEW FORMATS
//------ Process Communications View
StatusCode json_node_link_process_communications_view_initializer(View *view,
								  mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       json_node_link_process_communications_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status = traverse_parse_tree(r->output, view,
			       json_node_link_process_communications_view_edge_initializer);
  return status;
}

StatusCode json_node_link_process_communications_view_vertex_initializer(View *view,
									 mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  json_node_link_process_communications_view_vertex_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

StatusCode json_node_link_process_communications_view_edge_initializer(View *view,
								       mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  json_node_link_process_communications_view_edge_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

//------ Substation Network View
StatusCode json_node_link_substation_network_view_initializer(View *view,
							      mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       json_node_link_substation_network_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status= traverse_parse_tree(r->output, view,
			      json_node_link_substation_network_view_edge_initializer);
  return status;
}


StatusCode json_node_link_substation_network_view_vertex_initializer(View *view,
								     mpc_ast_t *a) {
  StatusCode status = OK;
  
  if ( !strncmp("node_dictionary", a->tag, 15) ) {
    int rc;

    mpc_ast_t * name_value_node = a->children[3]->children[1];
    mpc_ast_t * rdfs_type_value_node = a->children[7]->children[1];

    string name, rdfs_type;
    strncpy(name, name_value_node->contents, STRING_SIZE);
    strncpy(rdfs_type, rdfs_type_value_node->contents, STRING_SIZE);

    int vertex_id;
    vertex_id = view->vertex_count;

    struct v_attribute_entry *v_attr_entry;
    struct v_attribute_entry *v_inverse_attr_entry;
    
    v_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));
    v_inverse_attr_entry = (struct v_attribute_entry*)malloc(sizeof(struct v_attribute_entry));

    v_attr_entry->id = view->vertex_count;
    v_inverse_attr_entry->id = view->vertex_count;
    
    strncpy(v_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    strncpy(v_inverse_attr_entry->rdfs_type, rdfs_type, STRING_SIZE);
    
    strncpy(v_attr_entry->name, name, STRING_SIZE);
    strncpy(v_inverse_attr_entry->name, name, STRING_SIZE);
    
    HASH_ADD_INT( view->v_interpretation, id, v_attr_entry);
    HASH_ADD_STR( view->v_inverse_interpretation, name, v_inverse_attr_entry);

    // Initialize the vertex adjacency list
    struct vertex_adjacency_list *vertex_adjacency_list;
    vertex_adjacency_list = (struct vertex_adjacency_list*)malloc(sizeof(struct vertex_adjacency_list));
    vertex_adjacency_list->source_id = vertex_id;
    vertex_adjacency_list->targets = NULL;
    HASH_ADD_INT( view->e_interpretation, source_id, vertex_adjacency_list );
    
    view->vertex_count = view->vertex_count + 1;
  } 
  return status;  
}

StatusCode json_node_link_substation_network_view_edge_initializer(View *view,
								   mpc_ast_t *a) {
  StatusCode status = OK;

  struct v_attribute_entry *source_attr_entry, *target_attr_entry;
  string relation_name;
  int source_id, target_id;

  if ( !strncmp("link_dictionary", a->tag, 15) ) {
    int rc;

    mpc_ast_t * source_name_value_node = a->children[3]->children[1];
    mpc_ast_t * target_name_value_node = a->children[7]->children[1];
    mpc_ast_t * relation_name_value_node = a->children[11]->children[1];

    HASH_FIND_STR(view->v_inverse_interpretation, source_name_value_node->contents, source_attr_entry);
    if ( source_attr_entry == NULL ) {
      fprintf(stderr, "Edge Initializer:  Could not find source vertex with name %s\n", source_name_value_node->contents);
      exit(NO_INTERPRETATION_ERROR);
    }

    HASH_FIND_STR(view->v_inverse_interpretation, target_name_value_node->contents, target_attr_entry);
    if ( target_attr_entry == NULL ) {
      fprintf(stderr, "Edge Initializer:  Could not find target vertex with name %s\n", target_name_value_node->contents);
      exit(NO_INTERPRETATION_ERROR);
    }

    source_id = source_attr_entry->id;
    target_id = target_attr_entry->id;

    struct vertex_adjacency_list *vertex_adjacency_list = NULL;
    struct e_attribute_entry *e_attr_entry = NULL;

    HASH_FIND_INT(view->e_interpretation, &source_id, vertex_adjacency_list);
    e_attr_entry = vertex_adjacency_list->targets;

    if ( NULL == e_attr_entry ) {
      e_attr_entry = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry->target_id = target_id;
      strncpy(e_attr_entry->rdfs_type, relation_name_value_node->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
      vertex_adjacency_list->targets = e_attr_entry;
    } else {
      while ( e_attr_entry->next != NULL ) {
	e_attr_entry = e_attr_entry->next;
      }
      e_attr_entry->next = (struct e_attribute_entry*)malloc(sizeof(struct e_attribute_entry));
      e_attr_entry = e_attr_entry->next;
      e_attr_entry->target_id = target_id;
      strncpy(e_attr_entry->rdfs_type, relation_name_value_node->contents, STRING_SIZE);
      e_attr_entry->next = NULL;
    }
  }
  return status;
}

StatusCode rdf_turtle_substation_network_view_initializer(View *view,
							  mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       rdf_turtle_substation_network_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status = traverse_parse_tree(r->output, view,
			      rdf_turtle_substation_network_view_edge_initializer);
  return status;
}

StatusCode rdf_turtle_substation_network_view_vertex_initializer(View *view,
								 mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  rdf_turtle_substation_network_view_vertex_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

StatusCode rdf_turtle_substation_network_view_edge_initializer(View *view,
							       mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  rdf_turtle_substation_network_view_edge_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);  
}

//------ Substation Yard View
StatusCode json_node_link_substation_yard_view_initializer(View *view,
							   mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       json_node_link_substation_yard_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status= traverse_parse_tree(r->output, view,
			      json_node_link_substation_yard_view_edge_initializer);
  return status;
}

StatusCode json_node_link_substation_yard_view_vertex_initializer(View *view,
								  mpc_ast_t *a) {
  StatusCode status = OK;
  status = json_node_link_substation_network_view_vertex_initializer(view, a);
  return status;
}

StatusCode json_node_link_substation_yard_view_edge_initializer(View *view,
								mpc_ast_t *a) {
  StatusCode status = OK;
  status = json_node_link_substation_network_view_edge_initializer(view, a);
  return status;
}

StatusCode rdf_turtle_substation_yard_view_initializer(View *view,
						       mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       rdf_turtle_substation_yard_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status= traverse_parse_tree(r->output, view,
			      rdf_turtle_substation_yard_view_edge_initializer);
  return status;
}

StatusCode rdf_turtle_substation_yard_view_vertex_initializer(View *view,
							      mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  rdf_turtle_substation_yard_view_vertex_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);
}

StatusCode rdf_turtle_substation_yard_view_edge_initializer(View *view,
							    mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  rdf_turtle_substation_yard_view_edge_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);  
}

//---- OTHER FORMATS
//------ Process Tree Output
StatusCode pstree_output_process_tree_view_initializer(View *view,
						       mpc_result_t *r) {
  StatusCode status = OK;
  status = traverse_parse_tree(r->output, view,
			       pstree_output_process_tree_view_vertex_initializer);
  view->graph = graph_create(view->vertex_count);
  status= traverse_parse_tree(r->output, view,
			      pstree_output_process_tree_view_edge_initializer);
  return status;
}

StatusCode pstree_output_process_tree_view_vertex_initializer(View *view,
							      mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  ps_tree_output_process_tree_view_vertex_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);    
}

StatusCode pstree_output_process_tree_view_edge_initializer(View *view,
							    mpc_ast_t *a) {
  StatusCode status = OK;
  fprintf(stderr, "Not yet implemented:  ps_tree_output_process_tree_view_edge_initializer\n");
  status = NOT_YET_IMPLEMENTED;
  exit(status);  
}

StatusCode use_converter(FormatType format_type,
			 View *view,
			 ViewType view_type,
			 mpc_result_t *r) {
  StatusCode status = OK;
  
  if ( JSON_NODE_LINK == format_type ) {
    if ( PROCESS_COMS_VIEW == view_type ) {
      status = json_node_link_process_communications_view_initializer(view, r);
    } else if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = json_node_link_substation_network_view_initializer(view, r);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = json_node_link_substation_yard_view_initializer(view, r);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
      print_usage();
      status = UNKNOWN_VIEW_TYPE;
      exit(status);
    }
    
  } else if ( RDF_TURTLE == format_type ) {
    if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = rdf_turtle_substation_network_view_initializer(view, r);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = rdf_turtle_substation_yard_view_initializer(view, r);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
      print_usage();
      status = UNKNOWN_VIEW_TYPE;
      exit(status);
    }
    
  } else if ( INTERPRETATION_TABLE == format_type ) {
    fprintf(stderr, "Not yet implemented:  View Initializer for Interpretation Table\n");
    status = NOT_YET_IMPLEMENTED;
    exit(status);
    
  } else if ( PSTREE_OUTPUT == format_type ) {
    if ( PROCESS_TREE_VIEW == view_type ) {
      status = pstree_output_process_tree_view_initializer(view, r);
    } else {
      fprintf(stderr, "Not Yet Implemented:  View Initializer for Process Tree Output that isn't a Process Tree View");
      status = NOT_YET_IMPLEMENTED;
      exit(status);
    }
    
  } else {
    fprintf(stderr, "Unknown view type: %d\n", view_type);
    print_usage();
    status = UNKNOWN_FORMAT_TYPE;
    exit(status);
  }
  
  return status;
}

// CLI METHODS
#if defined CONVERTER
StatusCode print_usage() {
  fprintf(stderr, "Usage:  convert <input_format_type> <output_format_type> <view_type> <input_filepath>\n");
  return OK;
}

int main(int argc, char **argv) {

  int status = OK;
  string input_format_type_str, output_format_type_str, view_type_str, input_filepath;
  int input_format_type, output_format_type, view_type;
  mpc_result_t r;
  Graph graph;
  
  if ( argc >= 5 ) {
    strncpy(input_format_type_str, argv[1], STRING_SIZE);
    strncpy(output_format_type_str, argv[2], STRING_SIZE);
    strncpy(view_type_str, argv[3], STRING_SIZE);
    strncpy(input_filepath, argv[4], STRING_SIZE);

    input_format_type = get_code_for_format_type_str(input_format_type_str);
    output_format_type = get_code_for_format_type_str(output_format_type_str);
    view_type = get_code_for_view_type_str(view_type_str);
    
    status = use_recognizer(input_format_type, view_type, input_filepath, &r);
    
    View view;
    // Figure out the correct input format
    view.vertex_count = 0;

    // IMPORTANT
    view.v_inverse_interpretation = NULL;
    view.v_interpretation = NULL;
    view.e_interpretation = NULL;

    use_converter(input_format_type, &view, view_type, &r);
    output_view(&view, output_format_type, view_type, stdout);
    
  } else {
    print_usage();
    status = INVALID_ARGS;
    exit(status);
  }
  
  return status;
}
#endif
