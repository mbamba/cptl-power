#!/bin/bash

mkdir -p data/substation_network/json

for substation in capital-city cyprus-creek haverbrook north-haverbrook ogdenville paris shelbyville springfield
do
#    ./build/convert RDF_TURTLE JSON_NODE_LINK SUBSTATION_NETWORK_VIEW data/substation_network/$substation.ttl > data/substation_network/json/$substation.json | tee /tmp/errors.convert.log

    ./build/validate JSON_NODE_LINK SUBSTATION_NETWORK_VIEW data/substation_network/json/$substation.json >> /tmp/errors.validate.log
done

