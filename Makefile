#
#  copyright (c) 2015, Gabriel A. Weaver, Information Trust Institute
#    at the University of Illinois, Urbana-Champaign.
#
#  This file is part of the Cyber-Physical Topology Language for the
#    Power domain.
#
#  This code is free software:  you can redistribute it and/or modify it
#    under the terms of the GNU General Public License as published by the
#    Free Software Foundation, either version 3 of the License, or 
#    (at your option) any later version.
#
#  The Cyber-Physical Topology Language (CPTL):  Electrical Power 
#    is distributed in the hope that it will be useful, but 
#    WITHOUT ANY WARRANTY; without even the implied warranty of 
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#    along with this program.  If not, see http://www.gnu.org/licenses/
#

CC = gcc
STND=-ansi

init:
	mkdir build

lcthw_test: init
	gcc -Wall -g -DNDEBUG lib/lcthw/dbg_test.c -o build/dbg_test	
	gcc -Wall -g -DNDEBUG -Ilib lib/lcthw/darray.c lib/lcthw/darray_test.c -o build/darray_test
	gcc -Wall -g -DNDEBUG -Ilib lib/bstrlib/bstrlib.c lib/lcthw/darray.c lib/lcthw/hashmap.c lib/lcthw/hashmap_test.c -o build/hashmap_test
	./build/dbg_test test
	./build/darray_test
	./build/hashmap_test

graph_test: init
	gcc -Wall -g -Ilib lib/graph.c lib/graph_test.c -o build/graph_test
	./build/graph_test

tests: init lcthw_test graph_test

validators: 
	gcc -g -Ilib -DVALIDATOR lib/mpc.c src/validators/validate.c -o build/validate
	chmod u+x build/validate

converters: 
	gcc -g -Ilib -DCONVERTER lib/mpc.c lib/lcthw/hashmap.c lib/lcthw/darray.c lib/bstrlib/bstrlib.c lib/graph.c src/validators/validate.c src/converters/convert.c -o build/convert

clean:
	rm -rf build
