package = cptl
version = 1.0
tarname = cptl-yEd
distdir = $(tarname)-$(version)

builddir = build
srcdir = src
datadir = data 

init:
	mkdir -p ${builddir}/graphs/constructions/schematic
	mkdir -p ${builddir}/graphs/constructions/asl
	mkdir -p ${builddir}/graphs/constructions/cypher

clean:
	rm -rf ${builddir}

schematic2asl:
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/Line.graphml > build/graphs/constructions/asl/Line.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/Node.graphml > build/graphs/constructions/asl/Node.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/Breaker.graphml > build/graphs/constructions/asl/Breaker.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/RTU.graphml > build/graphs/constructions/asl/RTU.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/PotentialTransformer.graphml > build/graphs/constructions/asl/PotentialTransformer.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/CurrentTransformer.graphml > build/graphs/constructions/asl/CurrentTransformer.graphml	
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/BreakerControl.graphml > build/graphs/constructions/asl/BreakerControl.graphml	
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/BreakerInstallation.graphml > build/graphs/constructions/asl/BreakerInstallation.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/RTUToBreakerInstallation.graphml > build/graphs/constructions/asl/RTUToBreakerInstallation.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/BreakerControlInstallation.graphml > build/graphs/constructions/asl/BreakerControlInstallation.graphml
	xsltproc src/schematic2asl.xslt data/graphs/constructions/schematic/RTUBreakerControlInstallation.graphml > build/graphs/constructions/asl/RTUBreakerControlInstallation.graphml

asl2neo4j:
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/Line.graphml > build/graphs/constructions/cypher/Line.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/Node.graphml > build/graphs/constructions/cypher/Node.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/Breaker.graphml > build/graphs/constructions/cypher/Breaker.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/RTU.graphml > build/graphs/constructions/cypher/RTU.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/PotentialTransformer.graphml > build/graphs/constructions/cypher/PotentialTransformer.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/CurrentTransformer.graphml > build/graphs/constructions/cypher/CurrentTransformer.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/BreakerControl.graphml > build/graphs/constructions/cypher/BreakerControl.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/BreakerInstallation.graphml > build/graphs/constructions/cypher/BreakerInstallation.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/RTUToBreakerInstallation.graphml > build/graphs/constructions/cypher/RTUToBreakerInstallation.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/BreakerControlInstallation.graphml > build/graphs/constructions/cypher/BreakerControlInstallation.db
	xsltproc src/asl2neo4j.xslt build/graphs/constructions/asl/RTUBreakerControlInstallation.graphml > build/graphs/constructions/cypher/RTUBreakerControlInstallation.db
