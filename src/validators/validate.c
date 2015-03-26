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

#include "mpc.h"
#include "validate.h"

#include <stdlib.h>
#include <string.h>



//! Validate the input as a Substation Yard View
/*!
  \param The input file to validate
 */
StatusCode use_view_recognizer(const string input_filepath,
			       const string grammar_filepath) {
  StatusCode status = OK;
  mpc_result_t r;

  mpc_parser_t *NodeTypes = mpc_new("node_types");
  mpc_parser_t *LinkTypes = mpc_new("link_types");
  mpc_parser_t *String = mpc_new("string");
  mpc_parser_t *Key = mpc_new("key");
  mpc_parser_t *Value = mpc_new("value");
  mpc_parser_t *KeyValuePair = mpc_new("key_value_pair");
  mpc_parser_t *Dictionary = mpc_new("dictionary");
  mpc_parser_t *ListElement = mpc_new("list_element");
  mpc_parser_t *List = mpc_new("list");
  mpc_parser_t *LinkDictionary = mpc_new("link_dictionary");
  mpc_parser_t *LinkList = mpc_new("link_list");
  mpc_parser_t *NodeDictionary = mpc_new("node_dictionary");
  mpc_parser_t *NodeList = mpc_new("node_list");
  mpc_parser_t *GraphDictionary = mpc_new("graph_dictionary");
  mpc_parser_t *JSONNodeLink = mpc_new("json_node_link");

  FILE* view_grammar_file = fopen(grammar_filepath, "r");
  
  mpca_lang_file(MPCA_LANG_DEFAULT,
		 view_grammar_file, 
		 NodeTypes, LinkTypes, String, Key, Value, KeyValuePair, Dictionary, ListElement, List, LinkDictionary, LinkList, NodeDictionary, NodeList, GraphDictionary, JSONNodeLink, NULL);

  if ( mpc_parse_contents(input_filepath, JSONNodeLink, &r) ) {
    mpc_ast_print(r.output);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
    status = PARSE_ERROR;
    fprintf(stderr, "Error:  PARSE ERROR\n");
    fclose(view_grammar_file);
    exit(status);
  }
  
  mpc_cleanup(13, NodeTypes, LinkTypes, String, Key, Value, KeyValuePair, Dictionary, ListElement, List, NodeDictionary, NodeList, GraphDictionary, JSONNodeLink);
  fclose(view_grammar_file);
  return status;
}

//! Validate the input as a JSON Node Link Graph
/*!
  \param The input file to validate
 */
StatusCode use_json_node_link_recognizer(const string input_filepath,
					 const string grammar_filepath) {

  StatusCode status = OK;
  mpc_result_t r;
    
  mpc_parser_t *String = mpc_new("string");
  mpc_parser_t *Key = mpc_new("key");
  mpc_parser_t *Value = mpc_new("value");
  mpc_parser_t *KeyValuePair = mpc_new("key_value_pair");
  mpc_parser_t *Dictionary = mpc_new("dictionary");
  mpc_parser_t *ListElement = mpc_new("list_element");
  mpc_parser_t *List = mpc_new("list");
  mpc_parser_t *LinkDictionary = mpc_new("link_dictionary");
  mpc_parser_t *LinkList = mpc_new("link_list");
  mpc_parser_t *NodeDictionary = mpc_new("node_dictionary");
  mpc_parser_t *NodeList = mpc_new("node_list");
  mpc_parser_t *GraphDictionary = mpc_new("graph_dictionary");
  mpc_parser_t *JSONNodeLink = mpc_new("json_node_link");

  FILE* grammar_file = fopen(grammar_filepath, "r");
  mpca_lang_file(MPCA_LANG_DEFAULT,
		 grammar_file, 
		 String, Key, Value, KeyValuePair, Dictionary, ListElement, List, LinkDictionary, LinkList, NodeDictionary, NodeList, GraphDictionary, JSONNodeLink, NULL);

  if ( mpc_parse_contents(input_filepath, JSONNodeLink, &r) ) {
    mpc_ast_print(r.output);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
    status = PARSE_ERROR;
    fprintf(stderr, "Error:  PARSE ERROR\n");
    fclose(grammar_file);
    exit(status);
  }
  
  mpc_cleanup(11, String, Key, Value, KeyValuePair, Dictionary, ListElement, List, NodeDictionary, NodeList, GraphDictionary, JSONNodeLink);
  fclose(grammar_file);
  return status;
}

StatusCode use_recognizer(const string recognizer_type, const string input_filepath) {
  StatusCode status = OK;
  if ( !strncmp( recognizer_type, "JSON_NODE_LINK", STRING_SIZE ) ) {
    status = use_json_node_link_recognizer(input_filepath, "src/grammars/json_node_link.grammar");
  } else if ( !strncmp( recognizer_type, "SUBSTATION_YARD_VIEW", STRING_SIZE ) ) {
    status = use_view_recognizer(input_filepath, "src/grammars/substation_yard.grammar");
  } else if ( !strncmp( recognizer_type, "SUBSTATION_NETWORK_VIEW", STRING_SIZE ) ) {
    status = use_view_recognizer(input_filepath, "src/grammars/substation_network.grammar");    
  } else {
    fprintf(stderr, "Unknown recognizer type: %s\n", recognizer_type);
    print_usage();
    status = UNKNOWN_RECOGNIZER_TYPE;
    exit(status);
  }
  return status;
}

StatusCode print_usage() {
  fprintf(stderr, "Usage:  validate <recognizer_type> <input_filepath>\n");
  return OK;
}

int main(int argc, char **argv) {

  int status = OK;
  string input_filepath, recognizer_type;
  
  if ( argc == 3 ) {
    strncpy(recognizer_type, argv[1], STRING_SIZE);
    strncpy(input_filepath, argv[2], STRING_SIZE);
    status = use_recognizer(recognizer_type, input_filepath);
  } else {
    print_usage();
    status = INVALID_ARGS;
    exit(status);
  }
  return status;
}
