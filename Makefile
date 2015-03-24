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

tests: init
	gcc -g -Ilib lib/mpc.c lib/ptest.c tests/src/test.c tests/src/core.c -o build/tests

validators: init
	gcc -g -Ilib lib/mpc.c src/validators/validate.c -o build/validate
	chmod u+x build/validate

clean:
	rm -rf build
