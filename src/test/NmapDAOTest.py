"""
copyright (c) 2014, Gabriel A. Weaver, Coordinated Science Laboratory 
at the University of Illinois at Urbana-Champaign.

This file is part of the Cyber-Physical Topology Language (CPTL):
Electrical Power distribution.

The code is free software:   you can redistribute 
it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

The Cyber-Physical Topology Language (CPTL): Electrical Power
is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program.  If not, see http://www.gnu.org/licenses/
"""

import unittest

"""
Test all implementations of an NMapDAO here.  Currently, we
  have a file-based XML Nmap source though others might
  also be provided in the future.
"""
class TestNMapDAO(unittest.TestCase):

    xml_nmap_path = "../../data/test/nmap/nmap.input.xml"

    """
    Tests the constructor
    """
    def testConstructor(self):
        nmDAO = XMLNMapDAO(xml_nmap_path)
        self.assertEquals(nmDAO.getErrorCode(), 0)
        
    # Write setters and getters for all fields
    def testGetStatus(self):
        nmDAO = XMLNMapDAO(xml_nmap_path)
        actual_status_str = nmDAO.getStatus()
        expected_status_str = "<nmap:status state=\"up\" reason=\"echo-reply\" reason_ttl=\"bogus\"/>"
        self.assertEqual(actual_status_str, expected_status_str)
        
    def testGetAddress(self):
        nmDAO = XMLNMapDAO(xml_nmap_path)
        actual_address_str = nmDAO.getAddress()
        expected_address_str = "<nmap:address addr=\"74.207.244.221\" addrtype=\"ipv4\"/>"
        self.assertEqual(actual_address_str, expected_address_str)

    def testGetHostNames(self):
        return None

    def testGetPorts(self):
        return None

    def testGetOS(self):
        return None


    # Serialization of Information
    """
    Get the fields available via this interface (e.g. address, hostnames, ports, os)
    """
    def testGetFields(self):

    """
    Serialize the nmap information to a CPTL graph
    """
    def testToCPTL(self):
        nmDAO = XMLNMapDAO(xml_nmap_path)
        actual_cptl = nmDAO.toCPTL()
        expected_cptl = open("../../data/nmap/nmap.cptl.graphml",'r') // and parse
        self.assertEqual(actual_cptl, expected_cptl)
        
if __name__ == "__main__":
    unittest.main()

