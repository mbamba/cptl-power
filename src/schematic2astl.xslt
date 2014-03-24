<!-- 
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
-->
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:graphml="http://graphml.graphdrawing.org/xmlns"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:y="http://www.yworks.com/xml/graphml" 
	xmlns:yed="http://www.yworks.com/xml/yed/3" 
	version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="xml" indent="yes"/>

  <!-- Set up some consts -->
  <xsl:variable name="VARIABLE_LABEL">astl:Variable</xsl:variable>

  <!-- Set up the data keys in the Schematic source -->
  <xsl:variable name="schematicResourcesKey"><xsl:value-of select="/graphml:graphml/graphml:key[./@yfiles.type='resources']/@id"/></xsl:variable>
  <xsl:variable name="schematicNodeGraphicsKey"><xsl:value-of select="/graphml:graphml/graphml:key[./@yfiles.type='nodegraphics' and ./@for='node']/@id"/></xsl:variable>
  <xsl:variable name="schematicEdgeGraphicsKey"><xsl:value-of select="/graphml:graphml/graphml:key[./@yfiles.type='edgegraphics' and ./@for='edge']/@id"/></xsl:variable>

  <!-- Set up the data keys in the ASTL Target -->
  <xsl:variable name="astlResourcesKey">d0</xsl:variable>
  <xsl:variable name="astlPortGraphicsKey">d1</xsl:variable>
  <xsl:variable name="astlPortGeometryKey">d2</xsl:variable>
  <xsl:variable name="astlPortUserDataKey">d3</xsl:variable>
  <xsl:variable name="astlNodeNamespaceKey">d4</xsl:variable>
  <xsl:variable name="astlNodeClassKey">d5</xsl:variable>
  <xsl:variable name="astlNodeURLKey">d6</xsl:variable>
  <xsl:variable name="astlNodeDescriptionKey">d7</xsl:variable>
  <xsl:variable name="astlNodeGraphicsKey">d8</xsl:variable>
  <xsl:variable name="astlGraphDescriptionKey">d9</xsl:variable>
  <xsl:variable name="astlEdgeNamespaceKey">d10</xsl:variable>
  <xsl:variable name="astlEdgeURLKey">d11</xsl:variable>
  <xsl:variable name="astlEdgeDescriptionKey">d12</xsl:variable>
  <xsl:variable name="astlEdgeRelationKey">d13</xsl:variable>
  <xsl:variable name="astlEdgeGraphicsKey">d14</xsl:variable>
  

  <!-- Graphml -->
  <xsl:template match="graphml:graphml">
    <xsl:element name="graphml" namespace="http://graphml.graphdrawing.org/xmlns"> 
      <!--<xsl:attribute name="xmlns" value="http://graphml.graphdrawing.org/xmlns"/>-->
      <!--<xsl:attribute name="xmlns:xsi" value="http://www.w3.org/2001/XMLSchema-instance"/>
      <xsl:attribute name="xmlns:y" value="http://www.yworks.com/xml/graphml"/>
      <xsl:attribute name="xmlns:yed" value="http://www.yworks.com/xml/yed/3"/>-->
      
      <graphml:key for="graphml" id="{$astlResourcesKey}" yfiles.type="resources"/>
      <graphml:key for="port" id="{$astlPortGraphicsKey}" yfiles.type="portgraphics"/>
      <graphml:key for="port" id="{$astlPortGeometryKey}" yfiles.type="portgeometry"/>
      <graphml:key for="port" id="{$astlPortUserDataKey}" yfiles.type="portuserdata"/>
      <graphml:key attr.name="namespace" attr.type="string" for="node" id="{$astlNodeNamespaceKey}"/>
      <graphml:key attr.name="class" attr.type="string" for="node" id="{$astlNodeClassKey}"/>
      <graphml:key attr.name="url" attr.type="string" for="node" id="{$astlNodeURLKey}"/>
      <graphml:key attr.name="description" attr.type="string" for="node" id="{$astlNodeDescriptionKey}"/>
      <graphml:key for="node" id="{$astlNodeGraphicsKey}" yfiles.type="nodegraphics"/>
      <graphml:key attr.name="Description" attr.type="string" for="graph" id="{$astlGraphDescriptionKey}"/>
      <graphml:key attr.name="namespace" attr.type="string" for="edge" id="{$astlEdgeNamespaceKey}"/>
      <graphml:key attr.name="url" attr.type="string" for="edge" id="{$astlEdgeURLKey}"/>
      <graphml:key attr.name="description" attr.type="string" for="edge" id="${astlEdgeDescriptionKey}"/>
      <graphml:key attr.name="relation" attr.type="string" for="edge" id="{$astlEdgeRelationKey}"/>
      <graphml:key for="edge" id="{$astlEdgeGraphicsKey}" yfiles.type="edgegraphics"/>
      
      <xsl:element name="graph" namespace="http://graphml.graphdrawing.org/xmlns">
	<xsl:apply-templates select="child::graphml:graph/graphml:node"/>
	<xsl:apply-templates select="child::graphml:graph/graphml:edge"/>
      </xsl:element>
    </xsl:element>    
  </xsl:template>

  <!-- NODE 
       for each folder node in the schematic view, I want to create a node, Variable pair.  I then create a type and name node.
    -->
  <xsl:template match="graphml:node">

    <!-- CREATE THE NODE OF THE CONTAINER -->
    <xsl:element name="node" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="id">astl:VariableType::<xsl:value-of select="normalize-space(./@id)"/></xsl:attribute>

      <xsl:variable name="nodeLabel">
	<xsl:choose>
	  <xsl:when test="./@yfiles.foldertype='folder' or ./@yfiles.foldertype='group'">
	    <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ProxyAutoBoundsNode/y:Realizers/y:GroupNode/y:NodeLabel)"/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ShapeNode/y:NodeLabel)"/>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>

      <xsl:call-template name="getNodeClass"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
      <xsl:call-template name="getNodeNamespace"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
      <xsl:call-template name="getImageNode"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
    </xsl:element>

    <!-- FOR EACH CHILD NODE IN THE CONTAINER -->
    <xsl:if test="./@yfiles.foldertype='folder' or ./@yfiles.foldertype='group'">
      <xsl:variable name="parentNodeId">astl:VariableType::<xsl:value-of select="normalize-space(./@id)"/></xsl:variable>

      <!-- CREATE A VARIABLE NODE AND EDGE FOR EACH SUBGRAPH CHILD -->
      <xsl:for-each select="child::graphml:graph/graphml:node">
	  <xsl:variable name="variable_node_id"><xsl:value-of select="concat(concat('astl:Variable', '::'), normalize-space(./@id))"/></xsl:variable>
	  <xsl:variable name="variable_name_node_id"><xsl:value-of select="concat(concat('astl:VariableName', '::'), normalize-space(./@id))"/></xsl:variable>

	  <xsl:variable name="variable_name_node_label">
	  <xsl:choose>
	    <xsl:when test="child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ShapeNode/y:NodeLabel">
	      <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ShapeNode/y:NodeLabel)"/>
            </xsl:when>
	    <xsl:when test="child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ProxyAutoBoundsNode/y:Realizers/y:GroupNode/y:NodeLabel">
	      <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ProxyAutoBoundsNode/y:Realizers/y:GroupNode/y:NodeLabel)"/>
	    </xsl:when>
	  </xsl:choose>
	  </xsl:variable>
	  <xsl:variable name="variable_name"><xsl:value-of select="substring-after($variable_name_node_label, ' ')"/></xsl:variable>

	  <!-- Variable Node -->
	  <xsl:element name="node" namespace="http://graphml.graphdrawing.org/xmlns">
	    <xsl:choose>
	      <xsl:when test="contains($variable_node_id, ' ')">
		<xsl:attribute name="id"><xsl:value-of select="substring-before($variable_node_id, ' ')"/></xsl:attribute>
	      </xsl:when>
	      <xsl:otherwise>
		<xsl:attribute name="id"><xsl:value-of select="$variable_node_id"/></xsl:attribute>
	      </xsl:otherwise>
	    </xsl:choose>
	    <xsl:call-template name="getNodeClass"><xsl:with-param name="nodeLabel">astl:Variable</xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getNodeNamespace"><xsl:with-param name="nodeLabel">astl:Variable</xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getImageNode"><xsl:with-param name="nodeLabel">astl:Variable</xsl:with-param></xsl:call-template>
	  </xsl:element>

	  <!-- Variable Name Node -->
	  <xsl:element name="node" namespace="http://graphml.graphdrawing.org/xmlns">
	    <xsl:attribute name="id"><xsl:value-of select="$variable_name_node_id"/></xsl:attribute>
	    <xsl:value-of select="$variable_name_node_label"/>
	    <xsl:call-template name="getNodeNamespace"><xsl:with-param name="nodeLabel">astl:VariableName</xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getNodeName"><xsl:with-param name="variable_name"><xsl:value-of select="$variable_name"/></xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getImageNode"><xsl:with-param name="nodeLabel"><xsl:value-of select="$variable_name_node_label"/></xsl:with-param></xsl:call-template>
	  </xsl:element>

	  <!-- Variable Type Node
	  <xsl:element name="node" namespace="http://graphml.graphdrawing.org/xmlns">
	    <xsl:attribute name="id"><xsl:value-of select="$variable_node_type_id"/></xsl:attribute>
	    <xsl:call-template name="getNodeClass"><xsl:with-param name="nodeLabel"><xsl:value-of select="$VARIABLE_LABEL"/></xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getNodeNamespace"><xsl:with-param name="nodeLabel"><xsl:value-of select="$VARIABLE_LABEL"/></xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getImageNode"><xsl:with-param name="nodeLabel"><xsl:value-of select="$VARIABLE_LABEL"/></xsl:with-param></xsl:call-template>
	  </xsl:element>-->


	  <xsl:element name="edge" namespace="http://graphml.graphdrawing.org/xmlns">
	    <xsl:attribute name="source"><xsl:value-of select="$parentNodeId"/></xsl:attribute>
	    <xsl:attribute name="target"><xsl:value-of select="$variable_node_id"/></xsl:attribute>

	    <xsl:call-template name="getEdgeRelation"><xsl:with-param name="edgeLabel">astl:hasVariable</xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getEdgeNamespace"><xsl:with-param name="edgeLabel">astl:hasVariable</xsl:with-param></xsl:call-template>
	  </xsl:element>

	  <xsl:element name="edge" namespace="http://graphml.graphdrawing.org/xmlns">
	    <xsl:attribute name="source"><xsl:value-of select="$variable_node_id"/></xsl:attribute>
	    <xsl:attribute name="target"><xsl:value-of select="$variable_name_node_id"/></xsl:attribute>
	    <xsl:call-template name="getEdgeRelation"><xsl:with-param name="edgeLabel">astl:hasName</xsl:with-param></xsl:call-template>
	    <xsl:call-template name="getEdgeNamespace"><xsl:with-param name="edgeLabel">astl:hasName</xsl:with-param></xsl:call-template>
	  </xsl:element>
      </xsl:for-each>

      <xsl:apply-templates select="child::graphml:graph"/>
    </xsl:if>

    <xsl:if test="../../@yfiles.foldertype='folder' or ../../@yfiles.foldertype='group'">
      <xsl:element name="edge" namespace="http://graphml.graphdrawing.org/xmlns">
	<xsl:attribute name="source"><xsl:value-of select="concat(concat($VARIABLE_LABEL, '::'), ./@id)"/></xsl:attribute>
	<xsl:attribute name="target">astl:VariableType::<xsl:value-of select="./@id"/></xsl:attribute>
	<xsl:call-template name="getEdgeRelation"><xsl:with-param name="edgeLabel">astl:hasType</xsl:with-param></xsl:call-template>
	<xsl:call-template name="getEdgeNamespace"><xsl:with-param name="edgeLabel">astl:hasType</xsl:with-param></xsl:call-template>      
      </xsl:element>
    </xsl:if>

  </xsl:template>

  <!-- EDGE -->
  <xsl:template match="graphml:edge">
    <xsl:element name="edge" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="source"><xsl:call-template name="getNodeId"><xsl:with-param name="base_id"><xsl:value-of select="./@source"/></xsl:with-param></xsl:call-template></xsl:attribute>
      <xsl:attribute name="target"><xsl:call-template name="getNodeId"><xsl:with-param name="base_id"><xsl:value-of select="./@target"/></xsl:with-param></xsl:call-template></xsl:attribute>
      
      <xsl:call-template name="getExistingEdgeRelation"/>
      <xsl:call-template name="getExistingEdgeNamespace"/>
    </xsl:element>
  </xsl:template>

  <!-- GRAPH -->
  <xsl:template match="graphml:graph"><xsl:apply-templates/></xsl:template>  

  <!-- KEY -->
  <xsl:template match="graphml:key"/>

  <!-- DATA -->
  <xsl:template match="graphml:data"/>

  <!-- NAMED TEMPLATES -->
  <xsl:template name="getNodeId">
    <xsl:param name="base_id"></xsl:param>

    <!--<xsl:variable name="nodeLabel">
      <xsl:for-each select="//graphml:node[./@id = $base_id]">
	<xsl:choose>
	  <xsl:when test="./@yfiles.foldertype='folder' or ./@yfiles.foldertype='group'">
	    <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ProxyAutoBoundsNode/y:Realizers/y:GroupNode/y:NodeLabel)"/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicNodeGraphicsKey]/y:ShapeNode/y:NodeLabel)"/>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:for-each>
    </xsl:variable>

    <xsl:call-template name="getNodeNamespaceHelper"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
    <xsl:text>:</xsl:text>
    <xsl:call-template name="getNodeClassHelper"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
    <xsl:text>::</xsl:text>-->
    <xsl:text>astl:Variable::</xsl:text><xsl:value-of select="$base_id"/>

  </xsl:template>

  <xsl:template name="getImageNode">
    <xsl:param name="nodeLabel"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="graphml:key"><xsl:value-of select="$astlNodeGraphicsKey"/></xsl:attribute>

      <xsl:choose>
	<xsl:when test="contains($nodeLabel, 'cptl:power:') or contains($nodeLabel, 'cptl:substation')">
	  <y:ImageNode>
            <y:Geometry height="30.0" width="30.0"/>
            <y:Fill color="#CCCCFF" transparent="false"/>
            <y:BorderStyle color="#003300" type="line" width="1.0"/>
            <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" hasText="false" height="4.0" modelName="sandwich" modelPosition="s" textColor="#000000" visible="true" width="4.0"/>
	  </y:ImageNode>	
	</xsl:when>

	<xsl:when test="contains($nodeLabel, 'String')">
	  <y:ImageNode>
            <y:Geometry height="30.0" width="30.0"/>
            <y:Fill color="#CCCCFF" transparent="false"/>
            <y:BorderStyle color="#003300" type="line" width="1.0"/>
            <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" hasText="false" height="4.0" modelName="sandwich" modelPosition="s" textColor="#000000" visible="true" width="4.0"/>
	    <y:Shape type="rectangle"/>
	  </y:ImageNode>	
	</xsl:when>
	
	<xsl:when test="contains($nodeLabel, 'astl:Variable')">
          <y:ShapeNode>
            <y:Geometry height="30.0" width="30.0"/>
            <y:Fill color="#FF6600" transparent="false"/>
            <y:BorderStyle color="#993300" type="line" width="1.0"/>
            <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="18.1328125" modelName="custom" textColor="#000000" visible="true" width="51.185546875" x="-10.5927734375" y="5.93359375">Variable<y:LabelModel>
		<y:SmartNodeLabelModel distance="4.0"/>
              </y:LabelModel>
              <y:ModelParameter>
		<y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
              </y:ModelParameter>
            </y:NodeLabel>
            <y:Shape type="rectangle"/>
          </y:ShapeNode>	  
        </xsl:when>
      </xsl:choose>
      
    </xsl:element>
  </xsl:template>

  <xsl:template name="getNodeClass">
    <xsl:param name="nodeLabel"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="key"><xsl:value-of select="$astlNodeClassKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:call-template name="getNodeClassHelper"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getNodeClassHelper">
    <xsl:param name="nodeLabel"></xsl:param>
    <xsl:choose>      
      <xsl:when test="contains($nodeLabel, 'String')">
	<xsl:variable name="class">String</xsl:variable>
	<xsl:value-of select="$class"/>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'cptl:power')">
	<xsl:variable name="preclass"><xsl:value-of select="substring-after($nodeLabel, 'cptl:power:')"/></xsl:variable>
	<xsl:choose>
	  <xsl:when test="contains($preclass, ' ')">
	    <xsl:value-of select="substring-before($preclass, ' ')"/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="$preclass"/>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'cptl:substation')">
	<xsl:variable name="preclass"><xsl:value-of select="substring-after($nodeLabel, 'cptl:substation:')"/></xsl:variable>
	<xsl:choose>
	  <xsl:when test="contains($preclass, ' ')">
	    <xsl:value-of select="substring-before($preclass, ' ')"/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="$preclass"/>
	  </xsl:otherwise>
	</xsl:choose>
	</xsl:when>
      <xsl:when test="contains($nodeLabel, 'astl:VariableName')">
	<xsl:variable name="class"><xsl:value-of select="substring-after($nodeLabel, 'astl:')"/></xsl:variable>
	<xsl:value-of select="$class"/>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'astl:Variable')">
	<xsl:variable name="class"><xsl:value-of select="substring-after($nodeLabel, 'astl:')"/></xsl:variable>
	<xsl:value-of select="$class"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="getNodeName">
    <xsl:param name="variable_name"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="key"><xsl:value-of select="$astlNodeClassKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:value-of select="$variable_name"/>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getNodeNamespace">
    <xsl:param name="nodeLabel"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">    
      <xsl:attribute name="key"><xsl:value-of select="$astlNodeNamespaceKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:call-template name="getNodeNamespaceHelper"><xsl:with-param name="nodeLabel"><xsl:value-of select="$nodeLabel"/></xsl:with-param></xsl:call-template>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getNodeNamespaceHelper">
    <xsl:param name="nodeLabel"></xsl:param>
    <xsl:choose>
      <xsl:when test="contains($nodeLabel, 'String')">
	<xsl:text>xsd</xsl:text>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'cptl:power')">
	<xsl:text>cptl:power</xsl:text>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'cptl:substation')">
	<xsl:text>cptl:substation</xsl:text>
      </xsl:when>
      <xsl:when test="contains($nodeLabel, 'astl:Variable') or contains($nodeLabel, 'astl:Variable')">
	<xsl:text>astl</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="getEdgeRelation">
    <xsl:param name="edgeLabel"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="key"><xsl:value-of select="$astlEdgeRelationKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:choose>      
	<xsl:when test="contains($edgeLabel, 'astl:')">
	  <xsl:variable name="relation"><xsl:value-of select="substring-after($edgeLabel, 'astl:')"/></xsl:variable>
	  <xsl:value-of select="$relation"/>
	</xsl:when>
      </xsl:choose>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getEdgeNamespace">
    <xsl:param name="edgeLabel"></xsl:param>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">    
      <xsl:attribute name="key"><xsl:value-of select="$astlEdgeNamespaceKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:choose>
	<xsl:when test="contains($edgeLabel, 'String')">
	  <xsl:text>xsd</xsl:text>
	</xsl:when>
	<xsl:when test="contains($edgeLabel, 'cptl:power')">
	  <xsl:text>cptl:power</xsl:text>
	</xsl:when>
	<xsl:when test="contains($edgeLabel, 'cptl:substation')">
	  <xsl:text>cptl:substation</xsl:text>
	</xsl:when>
	<xsl:when test="contains($edgeLabel, 'astl:')">
	  <xsl:text>astl</xsl:text>
	</xsl:when>
      </xsl:choose>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getExistingEdgeRelation">
    <xsl:variable name="relation"><xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicEdgeGraphicsKey]/y:PolyLineEdge/y:EdgeLabel)"/></xsl:variable>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="key"><xsl:value-of select="$astlEdgeRelationKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:value-of select="substring-after($relation,':')"/>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template name="getExistingEdgeNamespace">
    <xsl:variable name="relation"><xsl:value-of select="normalize-space(child::graphml:data[./@key=$schematicEdgeGraphicsKey]/y:PolyLineEdge/y:EdgeLabel)"/></xsl:variable>

    <xsl:element name="data" namespace="http://graphml.graphdrawing.org/xmlns">
      <xsl:attribute name="key"><xsl:value-of select="$astlEdgeNamespaceKey"/></xsl:attribute>
      <xsl:text disable-output-escaping="yes">&lt;![CDATA[</xsl:text>
      <xsl:value-of select="substring-before($relation,':')"/>
      <xsl:text disable-output-escaping="yes">]]&gt;</xsl:text>
    </xsl:element>
  </xsl:template>

  <xsl:template match="node() | @*">
    <xsl:copy>
      <xsl:apply-templates select="node() | @*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
