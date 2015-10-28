#!/bin/bash

mkdir -p data/substation_network/json
mkdir -p data/substation_yard/json

for substation in capital-city cyprus-creek haverbrook north-haverbrook ogdenville paris shelbyville springfield
do
    #./build/convert RDF_TURTLE JSON_NODE_LINK SUBSTATION_NETWORK_VIEW data/substation_network/ttl/$substation.ttl > data/substation_network/json/$substation.tmp.json | tee /tmp/errors.convert.network.log 
    #sed -e "s/'//g" data/substation_network/json/$substation.tmp.json > data/substation_network/json/$substation.json
    #rm data/substation_network/json/*.tmp.json
    
    ./build/convert RDF_TURTLE JSON_NODE_LINK SUBSTATION_YARD_VIEW data/substation_yard/ttl/$substation.ttl > data/substation_yard/json/$substation.json 
done

