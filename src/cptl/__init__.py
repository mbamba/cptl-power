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
import libxml2
import networkx as nx
from networkx.readwrite import json_graph

class NMapDAO():

    # Identifier for the group of networks (e.g. enterprise name)
    xml_nmap_path = None
    cptl_nmap_graph = None
    host_id_prefix = None

    """
    Factory method for the nmap DAO.  For now we take a path to 
      an NMap XML file as input.
    @return an NMapDAO object.
    """
    @staticmethod
    def create(nmap_input_data_path,\
                   nmap_input_data_format,\
                   network_urn,\
                   host_id_prefix):
        nmDAO = NMapDAO()
        try: 
            nmDAO.host_id_prefix = host_id_prefix
            nmDAO.initializeCPTLGraph(nmap_input_data_path,\
                                          nmap_input_data_format,\
                                          network_urn)
        except Exception as e:
            print e
        return nmDAO

    """
    Given a path to the XML output of nmap, initialize 
    the nmap_graph for this instance.  
    @param input_type is NMAP_XML or CPTL_NMAP_XML
    @param input_data_path  Path to XML output of NMap Scan
      self.nmap_graph is initialized
    """
    def initializeCPTLGraph(self, input_data_path, input_data_format, network_urn):
        self.cptl_nmap_graph = nx.Graph()
        self.cptl_nmap_graph.graph['network_urn'] = network_urn

        if "CPTL_NMAP_XML" == input_data_format:
            raise Exception("CPTL_NMAP_XML parser not yet implemented!")
        elif "NMAP_XML" == input_data_format:
            self._initializeCPTLGraphFromNMAPXML(input_data_path)

    def _initializeCPTLGraphFromNMAPXML(self, input_data_path):
        nmap_xml_doc = libxml2.parseFile(input_data_path)

        # Parse information for the scanned network
        context = nmap_xml_doc.xpathNewContext()
        finished_element = context.xpathEval("//finished")[0]
        self.cptl_nmap_graph.graph['timestr'] = finished_element.prop("timestr")
        self.cptl_nmap_graph.graph['summary'] = finished_element.prop("summary")
        
        # Parse information for the hosts
        host_elements = context.xpathEval("//host")
        ref2idx = {}

        i = 1
        for host_element in host_elements:
            host_id = self.host_id_prefix + str(i)
            host_ref = self.cptl_nmap_graph.graph['network_urn'] + ":" + host_id
            self.cptl_nmap_graph.add_node(i, urn_id=host_ref)
            

            #-- now add attribute information
            self.cptl_nmap_graph.node[i]['asset_type_urn'] =\
                "urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST"
            
            child = host_element.children
            while child is not None:
                if "element" == child.type and "address" == child.name:
                    addrtype = child.prop("addrtype")
                    if "ipv4" == addrtype:
                        self.cptl_nmap_graph.node[i]['urn-cptl-HOST-ipv4'] = child.prop("addr")
                    elif "ipv6" == addrtype:
                        self.cptl_nmap_graph.node[i]['urn-cptl-HOST-ipv6'] = child.prop("addr")
                    elif "mac" == addrtype:
                        self.cptl_nmap_graph.node[i]['urn-cptl-HOST-mac'] = child.prop("addr")
                elif "element" == child.type and "hostnames" == child.name:
                    grandchild = child.children
                    while grandchild is not None:
                        if "element" == grandchild.type and "hostname" == grandchild.name:
                            hostname = grandchild.prop("name")
                            self.cptl_nmap_graph.node[i]['hostname'] = hostname
                            self.cptl_nmap_graph.node[i]['urn_id'] =\
                                self.cptl_nmap_graph.graph['network_urn'] + ":" + hostname
                            break
                        grandchild = grandchild.next
                child = child.next
            i = i + 1

        # No information to parse for the edges
        nmap_xml_doc.freeDoc()
        context.xpathFreeContext()

    """
    def initializeCPTLGraphFromCPTLXML(self, input_data_path):
        cptl_xml_doc = libxml2.parseFile(input_data_path)

        # Parse information for the scanned network
        context = cptl_xml_doc.xpathNewContext()
        graph_element = context.xpathEval("//graph")[0]
        graph_ref = graph_element.prop("id")
        if (self.cptl_graph['graph_ref'] != graph_ref):
            throw Exception("Graph refs don't match!")
            
        data_timestr = context.xpathEval("//graph/data[@key='timestr']")[0]
        self.cptl_graph['timestr'] = data_timestr.getContent()
        data_summary = context.xpathEval("//graph/data[@key='summary']")[0]
        self.cptl_graph['summary'] = data_summary.getContent()

        # Parse information for the hosts
        host_elements = context.xpathEval("//host")
        i = 1
        for host_element in host_elements:
            host_id = self.host_id_prefix + str(i)
            host_ref = self.cptl_graph['graph_ref'] + "." + host_id
            self.cptl_graph.add_node(host_ref)
            i = i + 1
            
        # Parse information for the edges
        edge_elements = context.xpathEval("//edge")
        i = 1
        for edge_element in edge_elements:
            source_host_id = edge_element.prop("source")
            target_host_id = edge_element.prop("target")
            source_host_ref = self.cptl_graph['graph_ref'] + "." + source_host_id
            target_host_ref = self.cptl_graph['graph_ref'] + "." + target_host_id
            self.cptl_graph.add_edge(source_host_ref, target_host_ref)
            i = i + 1

        cptl_xml_doc.freeDoc()
        context.xpathFreeContext()
    """

    """
    Retrieve all available types of assets for the given nmap scan.
      eventually incorporate a context parameter.
    @return A list of references 
    """
    @staticmethod
    def getAssetTypeURNs():
        asset_type_urns = [ ("urn:nmap:NAMESPACE:NETWORKS.NETWORK",\
                                 "urn:nmap:egCorp:corporate.core",\
                                 "A network"), 
                            ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST",\
                                 "urn:nmap:egCorp:corporate.core:HOST_1",\
                                 "A host within the network") ]
        return asset_type_urns

    """
    Given an asset type, retrieve the attributes associated with that
      type.
    @param asset_type The type of asset for which to retrieve attributes
    @return a list of type attribute tuples (attribute_ref, attribute description)
    """
    @staticmethod
    def getAssetAttributeTypeURNs(asset_type_urn):
        asset_attr_type_urns = []
        if "urn:nmap:NAMESPACE:NETWORKS.NETWORK" == asset_type_urn:
            asset_attr_type_urns = [ ("urn:nmap:NAMESPACE:NETWORKS.NETWORK@timestr", "The time of the network scan"),
                                 ("urn:nmap:NAMESPACE:NETWORKS.NETWORK@summary", "The summary of the nmap scan") ]
        elif "urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST" == asset_type_urn:
            asset_attr_type_urns = [ ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST#status", "The status of a network host"),
                                 ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST#address", "The address of a network host"),
                                 ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST#hostnames", "The hostnames of a network host"),
                                 ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST#ports", "The ports open on a network host"),
                                 ("urn:nmap:NAMESPACE:NETWORKS.NETWORK:HOST#os", "The OS of a network host") ]
        else:
            raise Exception("Unknown asset type: " + asset_type_urn)

        return asset_attr_type_urns

    def getCPTLGraph(self, output_format=None):
        result = None
        if None == output_format:
            result = self.cptl_nmap_graph
        elif "JSON" == output_format:
            result = json_graph.dumps(self.cptl_nmap_graph)
        else:
            raise Exception("Output format: " + output_format + "not yet supported")
        return result

    """
    Retrieve all instances of an asset of a given type
    @param asset_type_ref The type of asset to retrieve
    @return A list of asset references

    def getAssetRefs(self, asset_type_urn):
        asset_urns = []
        if "urn:nmap:network" == asset_type_ref:
            network_asset_urn = self.cptl_nmap_graph.graph['graph_ref']
            asset_refs.append(network_asset_urn)
        elif "urn:nmap:network.host" == asset_type_ref:
            asset_refs = self.cptl_nmap_graph.nodes()
        else:
            raise Exception("Asset reference retrieval not yet implemented for: " , asset_type_ref)
        return asset_refs
    """
