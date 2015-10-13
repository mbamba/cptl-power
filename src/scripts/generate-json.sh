#!/bin/bash

mkdir -p data/substation_network/json
mkdir -p data/substation_yard/json

for substation in capital-city cyprus-creek haverbrook north-haverbrook ogdenville paris shelbyville springfield
do
#    ./build/convert RDF_TURTLE JSON_NODE_LINK SUBSTATION_NETWORK_VIEW data/substation_network/$substation.ttl > data/substation_network/json/$substation.json | tee /tmp/errors.convert.network.log

    ./build/convert RDF_TURTLE JSON_NODE_LINK SUBSTATION_YARD_VIEW data/substation_yard/$substation.ttl > data/substation_yard/json/$substation.json 
done

