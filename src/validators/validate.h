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
typedef char string[STRING_SIZE];

typedef enum recognizer_type {
  JSON_NODE_LINK,
  SUBSTATION_NETWORK_VIEW,
  SUBSTATION_YARD_VIEW,
} RecognizerType;

typedef enum status_codes {
  OK = 0, 
  INVALID_ARGS = 1,
  PARSE_ERROR = 2,
  UNKNOWN_RECOGNIZER_TYPE = 3
} StatusCode;


StatusCode use_substation_yard_view_recognizer(const string input_filepath, const string grammar_filepath);
StatusCode use_json_node_link_recognizer(const string input_filepath, const string grammar_filepath);
StatusCode use_recognizer(const string recognizer_type, const string input_filepath);
StatusCode print_usage();

#endif
