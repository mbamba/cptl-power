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

FormatType get_code_for_format_type_str(const string format_type_str) {
  if ( !strncmp( "INTERPRETATION_TABLE", format_type_str, STRING_SIZE ) ) {
    return INTERPRETATION_TABLE;
  } else if ( !strncmp( "JSON_NODE_LINK", format_type_str, STRING_SIZE ) ) {
    return JSON_NODE_LINK;
  } else if ( !strncmp( "PSTREE_OUTPUT", format_type_str, STRING_SIZE ) ) {
    return PSTREE_OUTPUT;
  } else if ( !strncmp( "RDF_TURTLE", format_type_str, STRING_SIZE ) ) {
    return RDF_TURTLE;
  } else {
    StatusCode status;
    fprintf(stderr, "Unknown format type: %s\n", format_type_str);
    status = UNKNOWN_FORMAT_TYPE;
    exit(status);
  }
}

ViewType get_code_for_view_type_str(const string view_type_str) {
  if ( !strncmp( "PROCESS_COMS_VIEW", view_type_str, STRING_SIZE ) ) {
    return PROCESS_COMS_VIEW;
  } else if ( !strncmp( "PROCESS_TREE_VIEW", view_type_str, STRING_SIZE) ) {
    return PROCESS_TREE_VIEW;
  } else if ( !strncmp( "SUBSTATION_NETWORK_VIEW", view_type_str, STRING_SIZE) ) {
    return SUBSTATION_NETWORK_VIEW;
  } else if ( !strncmp( "SUBSTATION_YARD_VIEW", view_type_str, STRING_SIZE) ) {
    return SUBSTATION_YARD_VIEW;
  } else {
    StatusCode status;
    fprintf(stderr, "Unknown view type: %s\n", view_type_str);
    status = UNKNOWN_VIEW_TYPE;
    exit(status);
  }  
}

/*!
  \param The input file to validate
 */
StatusCode use_json_view_recognizer(const string input_filepath,
				    const string grammar_filepath,
				    mpc_result_t* r) {
  StatusCode status = OK;

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

  if ( ! mpc_parse_contents(input_filepath, JSONNodeLink, r) ) {
    mpc_err_print(r->error);
    mpc_err_delete(r->error);
    status = PARSE_ERROR;
    fprintf(stderr, "Error:  PARSE ERROR\n");
    fclose(view_grammar_file);
    exit(status);
  }
  
  mpc_cleanup(13, NodeTypes, LinkTypes, String, Key, Value, KeyValuePair, Dictionary, ListElement, List, NodeDictionary, NodeList, GraphDictionary, JSONNodeLink);
  fclose(view_grammar_file);
  return status;
}

StatusCode use_ttl_view_recognizer(const string input_filepath,
				   const string grammar_filepath,
				   mpc_result_t *r) {
  StatusCode status = OK;
  FILE* grammar_file = fopen(grammar_filepath, "r");

  mpc_parser_t *NodeTypes = mpc_new("node_types");
  mpc_parser_t *LinkTypes = mpc_new("link_types");
  mpc_parser_t *String = mpc_new("string");
  mpc_parser_t *NS_Prefix = mpc_new("ns_prefix");
  mpc_parser_t *URI = mpc_new("uri");
  mpc_parser_t *NS_Decl = mpc_new("ns_decl");
  mpc_parser_t *NS_Decls = mpc_new("ns_decls");
  mpc_parser_t *Subject = mpc_new("subject");
  mpc_parser_t *Predicate = mpc_new("predicate");
  mpc_parser_t *Object = mpc_new("object");
  mpc_parser_t *RDF_Triple_Block = mpc_new("rdf_triple_block");
  mpc_parser_t *RDF_Triple_List = mpc_new("rdf_triple_list");
  mpc_parser_t *RDF_Turtle_File = mpc_new("rdf_turtle_file");
  
  mpca_lang_file(MPCA_LANG_DEFAULT,
		 grammar_file, NodeTypes, LinkTypes,
		 String, NS_Prefix,
		 URI, NS_Decl, NS_Decls, Subject,
		 Predicate, Object, RDF_Triple_Block,
		 RDF_Triple_List, RDF_Turtle_File, NULL);

  if ( ! mpc_parse_contents(input_filepath, RDF_Turtle_File, r) ) {
    mpc_err_print(r->error);
    mpc_err_delete(r->error);
    status = PARSE_ERROR;
    fprintf(stderr, "Error:  PARSE ERROR\n");
    fclose(grammar_file);
    exit(status);
  }
  
  mpc_cleanup(13, RDF_Turtle_File, RDF_Triple_List, RDF_Triple_Block, Object, Predicate, Subject, NS_Decls, NS_Decl, URI, NS_Prefix, String, NodeTypes, LinkTypes);
  fclose(grammar_file);

  return status;
}

//---- Process Communications View
StatusCode json_node_link_process_communications_view_recognizer(const string input_filepath,
								 mpc_result_t *r) {
  StatusCode status;
  status = OK;
  
  status = use_json_view_recognizer(input_filepath,
				    "src/grammars/json/process_communications.grammar", r);
  return status;
}

//---- Process Tree Output
StatusCode json_node_link_process_tree_view_recognizer(const string input_filepath, mpc_result_t *r) {
  StatusCode status;
  status = OK;

  status = use_json_view_recognizer(input_filepath,
				    "src/grammars/json/process_tree.grammar", r);  
  return status;
}

StatusCode pstree_output_process_tree_view_recognizer(const string input_filepath, mpc_result_t *r) {
  StatusCode status;
  status = OK;

  string grammar_filepath = "src/grammars/unix/pstree.output.grammar";
  
  mpc_parser_t *String = mpc_new("string");
  mpc_parser_t *Pid = mpc_new("pid");
  mpc_parser_t *Username = mpc_new("username");
  mpc_parser_t *Command = mpc_new("command");
  mpc_parser_t *Indent = mpc_new("indent");
  mpc_parser_t *InternalNode = mpc_new("internal_node");
  mpc_parser_t *LeafNode = mpc_new("leaf_node");
  mpc_parser_t *Node = mpc_new("node");
  mpc_parser_t *Children = mpc_new("children");
  mpc_parser_t *Tree = mpc_new("tree");
  
  FILE* output_grammar_file = fopen(grammar_filepath, "r");

  mpca_lang_file(MPCA_LANG_DEFAULT,
		 output_grammar_file,
		 String, Pid, Username, Command, Indent, InternalNode, LeafNode, Node, Children, Tree, NULL); 

  if ( ! mpc_parse_contents(input_filepath, Tree, r) ) {
    mpc_err_print(r->error);
    mpc_err_delete(r->error);
    status = PARSE_ERROR;
    fprintf(stderr, "Error: PARSE ERROR UTOH!\n");
    fclose(output_grammar_file);
    exit(status);
  }

  mpc_cleanup(10, String, Pid, Username, Command, Indent, InternalNode, LeafNode, Node, Children, Tree);
  fclose(output_grammar_file);

  return status;
}

//---- Substation Network View
StatusCode json_node_link_substation_network_view_recognizer(const string input_filepath,
							     mpc_result_t *r) {
  StatusCode status;
  status = OK;

  status = use_json_view_recognizer(input_filepath,
				    "src/grammars/json/substation_network.grammar", r);  
  return status;
}

StatusCode rdf_turtle_substation_network_view_recognizer(const string input_filepath,
							 mpc_result_t *r) {
  StatusCode status;
  status = OK;

  status = use_ttl_view_recognizer(input_filepath,
				   "src/grammars/ttl/substation_network.grammar", r);  
  return status;
}

//---- Substation Yard View
StatusCode json_node_link_substation_yard_view_recognizer(const string input_filepath,
							  mpc_result_t *r) {
  StatusCode status;
  status = OK;
  
  status = use_json_view_recognizer(input_filepath,
				    "src/grammars/json/substation_yard.grammar", r);    
  return status;
}

StatusCode rdf_turtle_substation_yard_view_recognizer(const string input_filepath,
						      mpc_result_t *r) {
  StatusCode status;
  status = OK;
  
  status = use_ttl_view_recognizer(input_filepath,
				   "src/grammars/ttl/substation_yard.grammar", r); 
  return status;
}



StatusCode use_recognizer(FormatType format_type,
			  ViewType view_type,
			  const string input_filepath,
			  mpc_result_t *r) {
  StatusCode status = OK;

  if ( JSON_NODE_LINK == format_type ) {
    if ( PROCESS_COMS_VIEW == view_type ) {
      status = json_node_link_process_communications_view_recognizer(input_filepath, r);
    } else if ( PROCESS_TREE_VIEW == view_type ) {
      status = json_node_link_process_tree_view_recognizer(input_filepath, r);
    } else if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = json_node_link_substation_network_view_recognizer(input_filepath, r);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = json_node_link_substation_yard_view_recognizer(input_filepath, r);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
      print_usage();
      status = UNKNOWN_VIEW_TYPE;
      exit(status);
    }
    
  } else if ( RDF_TURTLE == format_type ) {
    if ( PROCESS_COMS_VIEW == view_type ) {
      fprintf(stderr, "Not Yet Implemented:  Recognizer for RDF Turtle, Process Communications View");
      status = NOT_YET_IMPLEMENTED;
      exit(status);
    } else if ( PROCESS_TREE_VIEW == view_type ) {
      fprintf(stderr, "Not Yet Implemented:  Recognizer for RDF Turtle, Process Tree View");
      status = NOT_YET_IMPLEMENTED;
      exit(status);
    } else if ( SUBSTATION_NETWORK_VIEW == view_type ) {
      status = rdf_turtle_substation_network_view_recognizer(input_filepath, r);
    } else if ( SUBSTATION_YARD_VIEW == view_type ) {
      status = rdf_turtle_substation_yard_view_recognizer(input_filepath, r);
    } else {
      fprintf(stderr, "Unknown view type: %d\n", view_type);
      print_usage();
      status = UNKNOWN_VIEW_TYPE;
      exit(status);
    }
    
  } else if ( INTERPRETATION_TABLE == format_type ) {
    fprintf(stderr, "Not Yet Implemented:  Recognizer for Interpretation Table");
    status = NOT_YET_IMPLEMENTED;
    exit(status);

  } else if ( PSTREE_OUTPUT == format_type ) {
    if ( PROCESS_TREE_VIEW == view_type ) {
      status = pstree_output_process_tree_view_recognizer(input_filepath, r);
    } else {
      fprintf(stderr, "Not Yet Implemented:  Recognizer for Process Tree Output that isn't a Process Tree View");
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

#if defined VALIDATOR
StatusCode print_usage() {
  fprintf(stderr, "Usage:  validate <format_type> <view_type> <input_filepath>\n");
  return OK;
}

int main(int argc, char **argv) {

  int status = OK;
  string format_type_str, view_type_str, input_filepath;
  int format_type, view_type;
  mpc_result_t r;
  
  if ( argc == 4 ) {
    
    strncpy(format_type_str, argv[1], STRING_SIZE);
    strncpy(view_type_str, argv[2], STRING_SIZE);
    strncpy(input_filepath, argv[3], STRING_SIZE);

    format_type = get_code_for_format_type_str(format_type_str);
    view_type = get_code_for_view_type_str(view_type_str);
    
    status = use_recognizer(format_type, view_type, input_filepath, &r);
    mpc_ast_print(r.output);
    mpc_ast_delete(r.output);
  } else {
    print_usage();
    status = INVALID_ARGS;
    exit(status);
  }
  return status;
}
#endif
