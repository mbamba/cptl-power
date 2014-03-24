package = cptl
version = 1.0
tarname = cptl-yEd
distdir = $(tarname)-$(version)

builddir = build
srcdir = src
datadir = data 

init:
	mkdir -p ${builddir}/graphs/constructions/schematic

clean:
	rm -rf ${builddir}

schematic2astl:
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/Line.graphml > build/graphs/constructions/astl/Line.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/Node.graphml > build/graphs/constructions/astl/Node.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/Breaker.graphml > build/graphs/constructions/astl/Breaker.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/RTU.graphml > build/graphs/constructions/astl/RTU.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/PotentialTransformer.graphml > build/graphs/constructions/astl/PotentialTransformer.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/CurrentTransformer.graphml > build/graphs/constructions/astl/CurrentTransformer.graphml	
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/BreakerControl.graphml > build/graphs/constructions/astl/BreakerControl.graphml	
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/BreakerInstallation.graphml > build/graphs/constructions/astl/BreakerInstallation.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/RTUToBreakerInstallation.graphml > build/graphs/constructions/astl/RTUToBreakerInstallation.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/BreakerControlInstallation.graphml > build/graphs/constructions/astl/BreakerControlInstallation.graphml
	xsltproc src/schematic2astl.xslt data/graphs/constructions/schematic/RTUBreakerControlInstallation.graphml > build/graphs/constructions/astl/RTUBreakerControlInstallation.graphml

astl2neo4j:
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/Line.graphml > build/graphs/constructions/cypher/Line.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/Node.graphml > build/graphs/constructions/cypher/Node.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/Breaker.graphml > build/graphs/constructions/cypher/Breaker.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/RTU.graphml > build/graphs/constructions/cypher/RTU.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/PotentialTransformer.graphml > build/graphs/constructions/cypher/PotentialTransformer.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/CurrentTransformer.graphml > build/graphs/constructions/cypher/CurrentTransformer.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/BreakerControl.graphml > build/graphs/constructions/cypher/BreakerControl.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/BreakerInstallation.graphml > build/graphs/constructions/cypher/BreakerInstallation.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/RTUToBreakerInstallation.graphml > build/graphs/constructions/cypher/RTUToBreakerInstallation.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/BreakerControlInstallation.graphml > build/graphs/constructions/cypher/BreakerControlInstallation.db
	xsltproc src/astl2neo4j.xslt build/graphs/constructions/astl/RTUBreakerControlInstallation.graphml > build/graphs/constructions/cypher/RTUBreakerControlInstallation.db
