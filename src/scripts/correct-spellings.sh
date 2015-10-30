#!/bin/bash

for substation in capital-city cypress-creek haverbrook north-haverbrook odgenville paris shelbyville springfield
do
    # Correct misspellings of Odgenville and Cypress
#    sed -e 's/ogdenville/odgenville/g' -e 's/cyprus/cypress/g' data/substation_network/ttl/$substation.ttl > data/substation_network/ttl/$substation.tmp.ttl

    # Correct the use of dash and dollar sign
#    sed -e 's/-BRK-/$BRK$/g' -e 's/-ND-/$ND$/g' -e 's/-TX-/$TX$/g' data/substation_network/ttl/$substation.tmp.ttl > data/substation_network/ttl/$substation.v2.ttl

    # Correct misspellings of Odgenville and Cypress    
     sed -e 's/ogdenville/odgenville/g' -e 's/cyprus/cypress/g' data/substation_yard/ttl/$substation.ttl > data/substation_yard/ttl/$substation.tmp.ttl

    # Correct the use of dash and dollar sign
    sed -e 's/-BRK-/$BRK$/g' -e 's/-ND-/$ND$/g' -e 's/-TX-/$TX$/g' data/substation_yard/ttl/$substation.tmp.ttl > data/substation_yard/ttl/$substation.v2.ttl
    
done    
