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


//-- OUTPUT METHODS
StatusCode output_interpretation(void* view, string recognizer_type, FILE* output_file) {
  StatusCode status = OK;
  int num_vertices, i;
  struct v_attribute_entry *v_attributes;

  View* v = (View*)view;
  
  // Iterate through the vertices
  // In the future, specify the fields we want
  num_vertices = graph_vertex_count(v->graph);
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

StatusCode output_view(void* view, string recognizer_type, FILE* output_file) {
  StatusCode status = OK;
  if ( !strncmp( recognizer_type, "JSON_NODE_LINK", STRING_SIZE ) ) {
    status = output_view_to_json_node_link((View*)view, output_file);
  } else if ( ! strncmp( recognizer_type, "TTL", STRING_SIZE) ) {
    status = output_view_to_rdf_turtle((View*)view, output_file);
  }
  return status;
}

//---- BASE NODE LINK VIEW
//------ JSON Format
StatusCode output_view_to_json_node_link(View* view, FILE* output_file) {

  StatusCode status = OK;  
  int i;
  int num_vertices;
  struct v_attribute_entry *v_attributes;
  
  fprintf(output_file, "{\n");

  // Print Nodes
  fprintf(output_file, "\t\"nodes\":\n");
  fprintf(output_file, "\t   [\n");

  num_vertices = graph_vertex_count(view->graph);
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
  num_vertices = graph_vertex_count(view->graph);

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
				StatusCode(*graph_builder)(mpc_ast_t *a, View* view) ) {
  StatusCode status = OK;
  int i;
  
  graph_builder(a, view);
  for ( i = 0; i < a->children_num; i++) {
    status = traverse_parse_tree(a->children[i], view, graph_builder);
  }
  return status;
}

//---- BASE NODE LINK VIEW
//------ JSON Format
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
}

static StatusCode rdf_turtle_edge_initializer(mpc_ast_t *a, View *view) {
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

static StatusCode rdf_turtle_vertex_initializer(mpc_ast_t *a, View *view) {
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

// CLI METHODS
#if defined CONVERTER
StatusCode print_usage() {
  fprintf(stderr, "Usage:  convert <input_filepath> <input_recognizer_type> <output_recognizer_type>\n");
  return OK;
}

int main(int argc, char **argv) {

  int status = OK;
  string input_filepath, input_recognizer_type, output_recognizer_type;
  mpc_result_t r;
  Graph graph;
  
  if ( argc >= 4 ) {
    strncpy(input_filepath, argv[1], STRING_SIZE);
    strncpy(input_recognizer_type, argv[2], STRING_SIZE);
    strncpy(output_recognizer_type, argv[3], STRING_SIZE);
    
    // Parse the input into an AST
    status = use_recognizer(input_recognizer_type, input_filepath, &r);

    View view;
    // Figure out the correct input format
    view.vertex_count = 0;

    // IMPORTANT
    view.v_inverse_interpretation = NULL;
    view.v_interpretation = NULL;
    view.e_interpretation = NULL;
      
    if ( !strncmp( input_recognizer_type, "JSON_NODE_LINK", STRING_SIZE) ) {
      status = traverse_parse_tree(r.output, &view, json_node_link_vertex_initializer);
      view.graph = graph_create(view.vertex_count);
      status = traverse_parse_tree(r.output, &view, json_node_link_edge_initializer);
    } else if ( !strncmp( input_recognizer_type, "TTL", STRING_SIZE) ) {
      status = traverse_parse_tree(r.output, &view, rdf_turtle_vertex_initializer);
      view.graph = graph_create(view.vertex_count);
      status = traverse_parse_tree(r.output, &view, rdf_turtle_edge_initializer);
    } else {
      print_usage();
      status = INVALID_ARGS;
      exit(status);
    }
    
    // Figure out the correct output format
    if ( !strncmp( output_recognizer_type, "JSON_NODE_LINK", STRING_SIZE) ) {
      status = output_view(&view, output_recognizer_type, stdout);
    } else if ( !strncmp( output_recognizer_type, "TTL", STRING_SIZE) ) {
      status = output_view(&view, output_recognizer_type, stdout);
    } else if ( !strncmp( output_recognizer_type, "INTERPRETATION_TABLE", STRING_SIZE) ) {
      status = output_interpretation(&view, output_recognizer_type, stdout);
    } else {
      print_usage();
      status = INVALID_ARGS;
      exit(status);
    }
    
  } else {
    print_usage();
    status = INVALID_ARGS;
    exit(status);
  }
  
  return status;
}
#endif
