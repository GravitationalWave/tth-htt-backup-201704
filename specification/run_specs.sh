#!/bin/bash

cd $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/specification
git pull
python run_specs.py
