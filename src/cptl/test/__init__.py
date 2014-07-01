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

from cptl import NMapDAO
import networkx as nx
import unittest

"""
Test all implementations of an NMapDAO here.  Currently, we
  have a file-based XML Nmap source though others might
  also be provided in the future.
"""

class TestNMapDAO(unittest.TestCase):

    nmap_input_data_path_1 = "data/test/nmap/nmap-scan.1.xml"
    nmap_input_data_path_2 = "data/test/nmap/nmap-scan.2.xml"

    urn_prefix = "urn:nmap"
    host_id_prefix = "HOST_"

    namespace_1 = "egCorp"
    networks_1 = "corporate"
    network_1 = "core"
    networks_urn_1 = ":".join([ urn_prefix, namespace_1, networks_1 ])
    network_urn_1 = ".".join([ networks_urn_1, network_1 ])

    namespace_2 = "egCorp"
    networks_2 = "remote"
    network_2 = "edge"
    networks_urn_2 = ":".join([ urn_prefix, namespace_2, networks_2 ])
    network_urn_2 = ".".join([ networks_urn_2, network_2 ])

    nmDAO = None

    def setUp(self):
        self.nmDAO_1 = NMapDAO.create( self.nmap_input_data_path_1,\
                                           "NMAP_XML",\
                                           self.network_urn_1,\
                                           self.host_id_prefix )
        self.nmDAO_2 = NMapDAO.create( self.nmap_input_data_path_2,\
                                           "NMAP_XML",\
                                           self.network_urn_2,\
                                           self.host_id_prefix )

    """
    Parse the input data source (nmap XML) to an internal representation
      (networkx Graph).  Eventually we may want to load into a 
      proper database.
    """
    def testInitializeCPTLGraph(self):
        graph_1 = self.nmDAO_1.getCPTLGraph()
        self.assertEqual( graph_1.number_of_nodes(), 1 )
        self.assertEqual( graph_1.number_of_edges(), 0 )

        graph_2 = self.nmDAO_2.getCPTLGraph()
        self.assertEqual( graph_2.number_of_nodes(), 8)

    def testGetAssetTypeURNs(self):
        asset_type_urns = NMapDAO.getAssetTypeURNs()
        self.assertEqual(len(asset_type_urns), 2)

    def testGetAssetAttributeTypeURNs(self):
        # N.B.:  We don't have any versions of networks in this DAO
        asset_attr_type_urns = NMapDAO.getAssetAttributeTypeURNs( "urn:nmap:NAMESPACE:NETWORKS.NETWORK" )
        self.assertEqual(len(asset_attr_type_urns), 2)

        asset_attr_type_urns = NMapDAO.getAssetAttributeTypeURNs( "urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST" )
        self.assertEqual(len(asset_attr_type_urns), 5)
        
    def testGetCPTLGraph(self):
        output_format = "JSON"
        json_cptl = self.nmDAO_1.getCPTLGraph(output_format)
        print json_cptl

        json_cptl = self.nmDAO_2.getCPTLGraph(output_format)
        print json_cptl
        
if __name__ == "__main__":
    unittest.main()

