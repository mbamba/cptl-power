#!/bin/bash

for substation in capital-city cypress-creek haverbrook north-haverbrook odgenville paris shelbyville springfield
do
    ./build/validate JSON_NODE_LINK SUBSTATION_NETWORK_VIEW data/substation_network/json/$substation.json >> /tmp/errors.validate-network.log

    ./build/validate JSON_NODE_LINK SUBSTATION_YARD_VIEW data/substation_yard/json/$substation.json >> /tmp/errors.validate-yard.log  
done

