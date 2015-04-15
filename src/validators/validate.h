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

#ifndef _VALIDATE_H_

#define STRING_SIZE 50
#define MAX_STRING_SIZE 800

#include <lcthw/hashmap.h>
#include <graph.h>
#include <mpc.h>
#include <uthash.h>

typedef char string[STRING_SIZE];

struct v_attribute_entry {
  int id;
  string name;
  string rdfs_type;
  UT_hash_handle hh;
};

struct e_attribute_entry {
  int target_id;
  string rdfs_type;
  struct e_attribute_entry* next;
};

struct vertex_adjacency_list {
  int source_id;
  struct e_attribute_entry* targets;
  UT_hash_handle hh;
};

typedef struct view {
  Graph graph;
  struct v_attribute_entry *v_inverse_interpretation;
  struct v_attribute_entry *v_interpretation;
  struct vertex_adjacency_list *e_interpretation;
  int vertex_count;
  int current_source_id;  // a hack
} View;

typedef enum recognizer_type {
  JSON_NODE_LINK,
  RDF_TURTLE,
  SUBSTATION_NETWORK_VIEW,
  SUBSTATION_YARD_VIEW,
} RecognizerType;

typedef enum graph_format_type {
  NODE_LINK,
  ADJACENCY_LIST,
  ADJACENCY_MATRIX
} GraphFormatType;


typedef enum status_codes {
  OK = 0, 
  INVALID_ARGS = 1,
  PARSE_ERROR = 2,
  UNKNOWN_RECOGNIZER_TYPE = 3,
  NO_INTERPRETATION_ERROR = 4
} StatusCode;


StatusCode use_substation_yard_view_recognizer(const string input_filepath, const string grammar_filepath);
StatusCode use_json_node_link_recognizer(const string input_filepath, const string grammar_filepath, mpc_result_t *r);
StatusCode use_recognizer(const string recognizer_type, const string input_filepath, mpc_result_t *r);
StatusCode print_usage();

#endif
