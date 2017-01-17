#!/bin/bash

git add .
git commit -a --message="save"
git push origin master

ssh margusp@yo <<"EOF"
cd $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/specification
bash run_specs.sh
EOF
