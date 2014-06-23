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
import unittest

"""
Test all implementations of an NMapDAO here.  Currently, we
  have a file-based XML Nmap source though others might
  also be provided in the future.
"""
class TestNMapDAO(unittest.TestCase):

    xml_nmap_path = "data/test/nmap/nmap.input.xml"
    xml_nmap_path2 = "data/test/nmap/nmap-scan.1.xml"
    network_group = "egCorp"
    network_group2 = "han"
    network_id = "simple"
    network_id2 = "simple"
    nmDAO = None

    def setUp(self):
        self.nmDAO = NMapDAO.create(self.xml_nmap_path,
                                    self.network_group,
                                    self.network_id)

    def testGetAssetTypes(self):
        asset_type_refs = self.nmDAO.getAssetTypes()
        self.assertEquals(len(asset_type_refs), 6)

    def testGetAssetRefs(self):
        
        # Test refs for the entire network
        asset_type_ref = "urn:nmap:network"
        actual_asset_refs = self.nmDAO.getAssetRefs(asset_type_ref)
        expected_asset_refs = ["urn:egCorp:simple"]
        self.assertEqual( len(actual_asset_refs), len(expected_asset_refs) )
        for i, actual_asset_ref in enumerate(actual_asset_refs):
            self.assertEqual(actual_asset_ref, expected_asset_refs[i])

        # Test refs for host references
        asset_type_ref = "urn:nmap:network.host"
        actual_asset_refs = self.nmDAO.getAssetRefs(asset_type_ref)
        expected_asset_refs = ["urn:egCorp:simple.HOST_1"]
        self.assertEqual( len(actual_asset_refs), len(expected_asset_refs) )
        for i, actual_asset_ref in enumerate(actual_asset_refs):
            self.assertEqual(actual_asset_ref, expected_asset_refs[i])

        # Test refs for host status
        asset_type_ref = "urn:nmap:network.host#status"
        actual_asset_refs = self.nmDAO.getAssetRefs(asset_type_ref)
        expected_asset_refs = ["urn:egCorp:simple.HOST_1#status"]
        self.assertEqual( len(actual_asset_refs), len(expected_asset_refs) )
        for i, actual_asset_ref in enumerate(actual_asset_refs):
            self.assertEqual(actual_asset_ref, expected_asset_refs[i])

        # Test refs for host os
        asset_type_ref = "urn:nmap:network.host#os"
        actual_asset_refs = self.nmDAO.getAssetRefs(asset_type_ref)        
        expected_asset_refs = ["urn:egCorp:simple.HOST_1#os"]
        self.assertEqual( len(actual_asset_refs), len(expected_asset_refs) )
        for i, actual_asset_ref in enumerate(actual_asset_refs):
            self.assertEqual(actual_asset_ref, expected_asset_refs[i])

    def testGetAsset(self):

        # Test refs for host
        asset_type = "urn:nmap:network.host"
        asset_ref = "urn:egCorp:simple.HOST_1"
        asset_vars = [ (asset_type, asset_ref) ]
        actual_assets = self.nmDAO.getAsset(asset_vars)
        print "HEYU!"
        print actual_assets

        # Test refs for host status
        asset_type = "urn:nmap:network.host#status"
        asset_ref = "urn:egCorp:simple.HOST_1#status"
        asset_vars = [ (asset_type, asset_ref) ]
        actual_assets = self.nmDAO.getAsset(asset_vars)
        print actual_assets

        # Test refs for host os
        asset_type = "urn:nmap:network.host#os"
        asset_ref = "urn:egCorp:simple.HOST_1#os"
        asset_vars = [ (asset_type, asset_ref) ]
        actual_assets = self.nmDAO.getAsset(asset_vars)
        print actual_assets

    def testGetAssetAsCPTLXML(self):
        # Test refs for the network
        asset_type = "urn:nmap:network"
        asset_ref = "urn:egCorp:simple"
        asset_vars = [ (asset_type, asset_ref) ]
        asset_to_cptlxml = self.nmDAO.getAssetAsCPTLXML(asset_vars)
        
    def testGetAssetAsCPTLXML2(self):
        nmDAO_1 = NMapDAO.create(self.xml_nmap_path2,
                                 self.network_group2,
                                 self.network_id2)
        asset_type = "urn:nmap:network"
        asset_ref = "urn:han:simple"
        asset_vars = [ (asset_type, asset_ref) ]
        asset_to_cptlxml = nmDAO_1.getAssetAsCPTLXML(asset_vars)
        f = open("/tmp/nmap-scan.1.cptl.graphml","w")
        f.write(asset_to_cptlxml.serialize("UTF-8"))
        f.close()
        
if __name__ == "__main__":
    unittest.main()
