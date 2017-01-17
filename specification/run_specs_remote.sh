#!/bin/bash

git add .
git commit -a --message="save"
git push origin master

ssh margusp@yo <<"EOF"
cd /home/margusp/VHbbNtuples_7_6_x/CMSSW_7_6_3/src/tthAnalysis/HiggsToTauTau/specification
bash run_specs.sh
EOF
