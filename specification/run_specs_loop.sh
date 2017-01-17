#!/bin/bash

export store_results_dir=$HOME/old_results

while true
do
  rm -rf $store_results_dir
  cp -a $HOME/tmp $store_results_dir

  cd $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/specification
  git pull
  python run_specs.py > $HOME/tmp/summary.txt
done
