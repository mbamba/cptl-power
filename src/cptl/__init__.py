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

class NMapDAO():

    # Identifier for the group of networks (e.g. enterprise name)
    network_group = None
    network_id = None
    xml_nmap_path = None
    error_code = None
    nmap_xml_doc = None
    nmap_ns = None
    graphml_ns = None

    host_id_prefix = "HOST_"

    """
    Factory method for the nmap DAO.  For now we take a path to 
      an NMap XML file as input.
    @param xml_nmap_path 
    @param network_group  ID for the group of networks (e.g. enterprise name)
    @param network_id     ID for a network being scanned
    @return an NMapDAO object.
    """
    @staticmethod
    def create(xml_nmap_path, network_group, network_id):
        nmDAO = NMapDAO()
        try: 
            nmDAO.network_group = network_group
            nmDAO.network_id = network_id
            nmDAO.xml_nmap_path = xml_nmap_path
            nmDAO.nmap_xml_doc = libxml2.parseFile(xml_nmap_path)
        except Exception as e:
            print e
        return nmDAO
    
    """
    Retrieve all available types of assets for the given nmap scan.
      eventually incorporate a context parameter.
    @return A list of references 
    """
    def getAssetTypes(self):
        self.error_code = 0
        refs = [ ("urn:nmap:network.host", "A host within the network"),
                 ("urn:nmap:network.host#status", "The status of a network host"),
                 ("urn:nmap:network.host#address", "The address of a network host"),
                 ("urn:nmap:network.host#hostnames", "The hostnames of a network host"),
                 ("urn:nmap:network.host#ports", "The ports open on a network host"),
                 ("urn:nmap:network.host#os", "The OS of a network host") ]
        return refs

    """
    Retrieve all instances of an asset of a given type
    @param asset_type_ref The type of asset to retrieve
    @return A list of asset references
    """
    def getAssetRefs(self, asset_type_ref):
        asset_refs = []
        if "urn:nmap:network" == asset_type_ref:
            network_asset_urn = "urn:" + self.network_group + ":" + self.network_id
            asset_refs.append(network_asset_urn)
        elif "urn:nmap:network.host" == asset_type_ref:
            asset_refs = self._getNetworkHostRefs()
        elif "urn:nmap:network.host#status" == asset_type_ref or\
                "urn:nmap:network.host#address" == asset_type_ref or\
                "urn:nmap:network.host#hostnames" == asset_type_ref or\
                "urn:nmap:network.host#ports" == asset_type_ref or\
                "urn:nmap:network.host#os" == asset_type_ref:
            asset_refs = self._getNetworkHostPropertyRefs(asset_type_ref)
        else:
            raise Exception("Asset reference retrieval not yet implemented for: " , asset_type_ref)
        return asset_refs

    def getAsset(self, asset_vars):
        assets = []

        for asset_type_ref, asset_ref in asset_vars:     
            if "urn:nmap:network.host" == asset_type_ref or\
                    "urn:nmap:network.host#status" == asset_type_ref or\
                    "urn:nmap:network.host#address" == asset_type_ref or\
                    "urn:nmap:network.host#hostnames" == asset_type_ref or\
                    "urn:nmap:network.host#ports" == asset_type_ref or\
                    "urn:nmap:network.host#os" == asset_type_ref:
                hosts = self._getNetworkHostRefs()
                host_id = asset_ref.split(".")[1]  # Assume host id aligned with doc order
                host_id = host_id.split("#")[0]
                host_id = int(host_id.split("_")[1])
                asset_xml = self._getNetworkHostPropertyNMapXML(asset_type_ref, host_id)
                assets.append( (asset_type_ref, asset_ref, asset_xml) )
            else:
                raise Exception("Asset retrieval not yet implemented for: ", asset_type_ref)
        return assets

    def getAssetAsCPTLXML(self, asset_vars):
        assets = []
        
        asset_type = asset_vars[0][0]
        asset_ref = asset_vars[0][1]
        if "urn:nmap:network" != asset_type:
            raise Exception("unimplemented asset serialization to CPTL for type: " + asset_type)
        cptl_doc = libxml2.newDoc("1.0")
        graphml_element = cptl_doc.newChild(None, "graphml", None)
        self.graphml_ns = graphml_element.newNs("http://graphml.graphdrawing.org/xmlns", "graphml")
        self.nmap_ns = graphml_element.newNs("http://www.nmap.org/docs/2/nmap.dtd", "nmap")
        
        # Set up the keys
        for key_label in ["address", "hostnames", "label", "os", "ports", "status", "type"]:
            self._createCPTLXMLKey(key_label, graphml_element, "node")
        
        # Now create the graph
        graph_elet = graphml_element.newChild(None, "graph", None)
        graph_elet.setProp("id", asset_ref)
        graph_elet.setProp("edgedefault", "undirected")
        
        # Add some time information
        graph_time_elet = graph_elet.newChild(None, "data", None)
        graph_time_elet.setProp("key", "timestr")
        graph_time_elet.setContent("Fri Sep 9 18:33:54 2011 (HARDCODED!)")

        # Now add the Nodes
        host_type = "urn:nmap:network.host"
        host_refs = self.getAssetRefs(host_type)
        for host_ref in host_refs:
            self._createCPTLXMLHostElet(host_type, host_ref, graph_elet)
            
        return cptl_doc
        
    def _createCPTLXMLKey(self, key_label, parent_elet, attr_type):
        key_elet = parent_elet.newChild(None, "key", None)
        key_elet.setProp("id", key_label)
        key_elet.setProp("for", attr_type)
        key_elet.setProp("attr.name", key_label)
        
    def _createCPTLXMLHostElet(self, host_type, host_ref, parent_elet):
        node_elet = parent_elet.newChild(None, "node", None)
        node_elet.setProp("id", host_ref)
        
        node_type_elet = node_elet.newChild(None, "data", None)
        node_type_elet.setProp("key", "type")
        node_type_elet.setContent("network:Host")
        
        for key_label in ["os", "address", "status", "ports", "hostnames"]:
            host_property_type = host_type + "#" + key_label
            data_elet = node_elet.newChild(None, "data", None)
            data_elet.setProp("key", key_label)
            asset_vars = [ (host_property_type, host_ref) ]
            asset_xml = self.getAsset(asset_vars)[0][2]
            if asset_xml != None:
                data_elet.addChild(asset_xml)

    """
    @return A list of assets of type 
    """
    def _getNetworkHostRefs(self):
        host_refs = []
        host_elements = self._getNetworkHostsNMapXML()
        
        i = 1
        for host in host_elements:
            host_id = self.host_id_prefix + str(i)
            host_asset_urn = "urn:" + self.network_group + ":" + self.network_id + "." + host_id
            host_refs.append(host_asset_urn)
            i = i + 1
        return host_refs

    """
    @return A list of properties of type "urn:nmap:network.host"
    """
    def _getNetworkHostPropertyRefs(self, asset_type_ref):
        host_property_refs = []
        host_refs = self._getNetworkHostRefs()
        property = asset_type_ref.split("#")[1]   # A little sloppy here
        for host_ref in host_refs:
            host_property_ref = host_ref + "#" + property
            host_property_refs.append(host_property_ref)
        return host_property_refs

    def _getNetworkHostsNMapXML(self):
        self.nmap_xml_doc = libxml2.parseFile(self.xml_nmap_path)
        context = self.nmap_xml_doc.xpathNewContext()
        host_elements = context.xpathEval("//host")
        self.nmap_xml_doc.freeDoc()
        context.xpathFreeContext()
        return host_elements

    def _getNetworkHostPropertyNMapXML(self, asset_type_ref, host_id):
        self.nmap_xml_doc = libxml2.parseFile(self.xml_nmap_path)
        context = self.nmap_xml_doc.xpathNewContext()
        xpath = self._getXPathForAssetType(asset_type_ref, host_id)
        results = context.xpathEval(xpath)
        if len(results) > 0:
            host_property_element = context.xpathEval(xpath)[0].copyNode(1)
            host_property_element.setNs(self.nmap_ns)
            print [xpath, host_property_element]
        else:
            host_property_element = None
        self.nmap_xml_doc.freeDoc()
        context.xpathFreeContext()
        return host_property_element
        
    def _getXPathForAssetType(self, asset_type_ref, host_id):
        xpath = None
        if "urn:nmap:network" == asset_type_ref:
            xpath = "/"
        elif "urn:nmap:network.host" == asset_type_ref:
            xpath = "//host"
        elif "urn:nmap:network.host#status" == asset_type_ref or\
                "urn:nmap:network.host#address" == asset_type_ref or\
                "urn:nmap:network.host#hostnames" == asset_type_ref or\
                "urn:nmap:network.host#ports" == asset_type_ref or\
                "urn:nmap:network.host#os" == asset_type_ref:
            property = asset_type_ref.split("#")[1]
            xpath = "//host[" + str(host_id) + "]/" + property
        else:
            raise Exception("XPath for asset type not yet implemented for: ", asset_type_ref)
        return xpath

    """
    Retrieve all instances of an asset given by the scan
    @param asset_type_urn    A urn reference to an asset provided by NMap
    @param additional_params  A query object
    @return A list of references to the hosts found within the scan

    def getAssets(self, asset_type_urn, additional_params):
        if "urn:nmap:network.host" == nmap_urn:
            host_asset_urns = getNetworkHosts()
            return host_asset_urns
        elif "urn:nmap:network.host#status" == nmap_urn:
            host_asset_urn = additional_params['urn:nmap:network.host']
            return getNetworkHostProperty(host_asset_urn, asset_type_urn)
        else:
            raise Exception("Unrecognized asset exception", asset_urn)



    @return a list of properties contained within "urn:nmap:network.host"

    def getNetworkHostPropeties(self, host_asset_ref, asset_type_ref):
        asset_type_refs = self.getAssetTypeRefs()

        if not (asset_type_ref in asset_type_refs):
            return []
        elif "urn:nmap:network.host#status" == asset_type_ref:
            properties = "Awesome JSON"
        elif "urn:nmap:network.host#address" == asset_type_ref:
            properties = "Awesome JSON"
        else:
            raise Exception("Not yet implemented", asset_type_ref)
        return properties


    """
