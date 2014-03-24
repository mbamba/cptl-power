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
  <xsl:output method="text"/>
  
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

  <xsl:template match="graphml:graphml">
    <xsl:text>CREATE </xsl:text>
    <xsl:for-each select="child::graphml:graph/graphml:node">
      <xsl:call-template name="output_node"/>
    </xsl:for-each>
    <xsl:for-each select="child::graphml:graph/graphml:edge">
      <xsl:call-template name="output_edge"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="output_node">
    <xsl:variable name="class"><xsl:value-of select="child::graphml:data[./@key=$astlNodeClassKey]"/></xsl:variable>
    <xsl:variable name="ns"><xsl:value-of select="child::graphml:data[./@key=$astlNodeNamespaceKey]"/></xsl:variable>
    <xsl:text>(</xsl:text>
      <xsl:call-template name="output_node_var_and_labels">
	<xsl:with-param name="id"><xsl:value-of select="./@id"/></xsl:with-param>
	<xsl:with-param name="class"><xsl:value-of select="$class"/></xsl:with-param>
      </xsl:call-template>

      <xsl:text> {</xsl:text>

      <xsl:if test="$ns != ''">
	<xsl:text>ns:'</xsl:text><xsl:value-of select="$ns"/><xsl:text>', </xsl:text>
      </xsl:if>

      <xsl:choose>
	<xsl:when test="$class and contains(./@id, 'VariableName')">
	  <xsl:text>value:'</xsl:text><xsl:value-of select="$class"/><xsl:text>', </xsl:text>
	  <xsl:text>class:'VariableName'</xsl:text>
	</xsl:when>
	<xsl:when test="$class">
	  <xsl:text>class:'</xsl:text><xsl:value-of select="$class"/><xsl:text>'</xsl:text>
	</xsl:when>
      </xsl:choose>
      <xsl:text>}</xsl:text>
    <xsl:text>),
 </xsl:text>
  </xsl:template>

  <xsl:template name="output_node_var_and_labels">
    <xsl:param name="id"></xsl:param>
    <xsl:param name="class"></xsl:param>

    <xsl:choose>
      <xsl:when test="contains($id, 'VariableName')">
	<xsl:variable name="var"><xsl:value-of select="substring-after($id, '::')"/></xsl:variable>
	<xsl:text>vn_</xsl:text><xsl:value-of select="translate($var,'::','_')"/><xsl:text>:VariableName</xsl:text>
      </xsl:when>
      <xsl:when test="contains($id, 'VariableType')">
	<xsl:variable name="var"><xsl:value-of select="substring-after($id, '::')"/></xsl:variable>
	<xsl:text>vt_</xsl:text><xsl:value-of select="translate($var,'::','_')"/><xsl:text>:VariableType</xsl:text>
      </xsl:when>
      <xsl:when test="contains($id, 'Variable')">
	<xsl:variable name="var"><xsl:value-of select="substring-after($id, '::')"/></xsl:variable>
	<xsl:text>v_</xsl:text><xsl:value-of select="translate($var,'::','_')"/><xsl:text>:Variable</xsl:text>
      </xsl:when>
      <xsl:otherwise>
	<xsl:value-of select="$id"/><xsl:text>:</xsl:text><xsl:value-of select="$class"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="output_edge">
    <xsl:variable name="source">
      <xsl:call-template name="output_node_var_and_labels"><xsl:with-param name="id"><xsl:value-of select="./@source"/></xsl:with-param></xsl:call-template>
    </xsl:variable>

    <xsl:variable name="relation_name"><xsl:value-of select="child::graphml:data[./@key=$astlEdgeRelationKey]"/></xsl:variable>
    <xsl:variable name="relation_namespace"><xsl:value-of select="child::graphml:data[./@key=$astlEdgeNamespaceKey]"/></xsl:variable>
    <xsl:variable name="relation">
      <xsl:text>-[:</xsl:text>
      <xsl:value-of select="$relation_name"/>
      <xsl:text> {ns:'</xsl:text><xsl:value-of select="$relation_namespace"/>
      <xsl:text disable-output-escaping='yes'>'}]-&gt;</xsl:text>
    </xsl:variable>

    <xsl:variable name="target">
      <xsl:call-template name="output_node_var_and_labels"><xsl:with-param name="id"><xsl:value-of select="./@target"/></xsl:with-param></xsl:call-template>
    </xsl:variable>

    <xsl:variable name="edge">
      <xsl:text>(</xsl:text><xsl:value-of select="substring-before($source,':')"/><xsl:text>)</xsl:text>
      <xsl:value-of select="$relation"/>
      <xsl:text>(</xsl:text><xsl:value-of select="substring-before($target,':')"/><xsl:text>)</xsl:text>
    </xsl:variable>

    <xsl:value-of select="normalize-space($edge)"/><xsl:text>,
</xsl:text>
    
  </xsl:template>

</xsl:stylesheet>
