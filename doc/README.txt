INTRODUCTION
We are interested in reasoning about how the topology (connectivity)
of assets within the electrical power grid affects physical, economic,
and security measurements.  We therefore introduce a domain-specific
language whose intent is to allow us to analyze these different
aspects of power systems.  When applied to the power grid, this
language will allow us to analytically evalute the design and ongoing
state of generation, substation, and distribution networks relative to
architectural, physical, and security tradeoffs.

Within the course of this small tutorial, we will demonstrate how this
language may be used for a variety of tasks including (1) dependency
analysis, (2) reachability analysis, (3) network inventory generation,
and (4) network diagramming.

REQUIREMENTS
1.  xsltproc:  We use XSLT to transform various views of the
               cyber-physical system.
2.  yEd:       We use yEd (http://www.yworks.com/en/products_yed_about.html) 
               to create schematics for cyber-physical constructs.
3.  Neo4j:     We use Neo4j (http://www.neo4j.org/) to query different 
               cyber-physical constructs.

GETTING STARTED
Practitioners have a wide variety of views of a transmission
substation, each of which encode a wide variety of relations at a
variety of different levels of granularity.  For example, a
transmission substation may have a one-line diagram and a network
diagram associated with it.

h i g h  l e v e l  v i e w
We interpret each of these diagrams as a visual representation of a
database of relations that may be queried and used in reasoning about
the state of the cyber-physical systems within the power grid.  One
can compose these relations in a large variety of ways and in this
sense, the language is extensible.  On the other hand, certain
combinations of relations are more common than others within power
system design and we can represent these as a construct.  Moreover,
the relations within a graph may yield certain behavioral properties
that we may capture within a set of functions.

For this release, we focus on the ability to create diagrams of
constructs within cyber-physical systems and query the relations
contained therein.  Already we will see that this allows us to perform
basic dependency analyses, generate inventories of networked assets
(both cyber and physical), and diagram networks.  



d e t a i l e d  e x a m p l e 
Although there are a wide variety of components within a power control
network, we now demonstrate the capabilities of our language by
constructing a module to encode a subsequently analyze a protection
scheme for a transmission substation.  Specifically we focus on
constructs for various breaker control schemes within a substation.
By the end of this guide, you will understand how to do the following:

1.  Graphically define constructs for various breaker control schemes.
2.  Use a graph database to analyze dependencies within those
      constructs and generate inventories of assets.

We are now going to define various constructs needed for breaker
control schemes.  There are a variety of different ways that one can
configure a recloser control.  We are now going to discuss each of
these alternatives.  
1.  A Remote Terminal Unit (RTU) is connected to a breaker.
2.  A Breaker Control is connected to a breaker.
3.  An RTU is connected to a Breaker Control, the Breaker Control
     is subsequently connected to the breaker.  

Each of these may be constructed from other components within our
catalog.






