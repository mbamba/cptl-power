#include "mpc.h"
#include "validate.h"

#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {

  string input_filepath = argv[1];
  mpc_result_t r;
  mpc_parser_t *String = mpc_new("string");

  FILE scratch_grammar = fopen("tests/src/grammars/simple.grammar", "r");
  mpca_lang_file(MPCA_LANG_DEFAULT,
		 scratch_grammar, String, NULL);

  if ( mpc_parse_contents( input_filepath, String, &r) ) {
    mpc_ast_print(r.output);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }
  
  return 0;
    
}
