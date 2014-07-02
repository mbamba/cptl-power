CPTL_ROOT=.
CONFIG_DIR=$(CPTL_ROOT)/config
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
	python2 $(SRC_DIR)/run_test_suite.py

usage:
	@echo "-------------------------"
	@echo "target:  description     "
	@echo "-------------------------"
	@echo $(clean_desc)
	@echo $(init_desc)
	@echo $(test_desc)
	@echo $(validate_desc)

validate_desc="validate: validate test data\n"
validate:
	xmllint --noout --dtdvalid data/validation/nmap/nmap.dtd data/test/nmap/nmap-scan.1.xml
	xmllint --noout --dtdvalid data/validation/nmap/nmap.dtd data/test/nmap/nmap-scan.2.xml
	xmllint --noout --relaxng data/validation/cptl.rng data/test/nmap/nmap-scan.1.cptl.graphml
	xmllint --noout --relaxng data/validation/cptl.rng data/test/nmap/nmap-scan.2.cptl.graphml


