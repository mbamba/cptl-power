CPTL_ROOT=.
CONFIG_DIR=$(CPTL_ROOT)/config
DOC_DIR=$(CPTL_ROOT)/doc
DOC_DIR_DOXYGEN=$(DOC_DIR)/doxygen

BUILD_DIR=$(CPTL_ROOT)/build
SRC_DIR=$(CPTL_ROOT)/src

all: usage


clean_desc="clean: clean the build\n"
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DOC_DIR_DOXYGEN)

init_desc="init: initialize the build\n"
init:
	mkdir -p $(BUILD_DIR)/graphs/constructions/schematic
	mkdir -p $(BUILD_DIR)/graphs/constructions/asl
	mkdir -p $(BUILD_DIR)/graphs/constructions/cypher

test_desc="test: test the cptl dao\n"
test:
	export PYTHONPATH=/usr/local/opt/libxml2/lib/python2.7/site-packages:$PYTHONPATH
	python $(SRC_DIR)/run_test_suite.py

usage:
	@echo "-------------------------"
	@echo "target:  description     "
	@echo "-------------------------"
	@echo $(clean_desc)
	@echo $(init_desc)
	@echo $(doc_desc)
	@echo $(test_desc)
	@echo $(validate_desc)

validate_desc="validate: validate test data\n"
validate:
	xmllint --noout --dtdvalid data/validation/nmap/nmap.dtd data/test/nmap/nmap.input.xml
	xmllint --noout --relaxng data/validation/cptl.rng data/test/nmap/nmap.cptl.graphml


