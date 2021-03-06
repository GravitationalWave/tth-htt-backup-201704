import subprocess
import time
import os
from tthAnalysis.HiggsToTauTau.jobTools import run_cmd
from tthAnalysis.HiggsToTauTau.sbatchManager import sbatchManager

from config import config

def check_that_histograms_are_valid_with_missing_input_histogram_spec():


    # Prepare

    missing_histogram = "%(fixtures_dir)s/histogram_THIS_DOES_NOT_EXIST.root" % config

    histograms = [
        "%(fixtures_dir)s/histogram_1.root" % config,
        missing_histogram
    ]

    # Run task

    command = 'python %(cmssw_base)s/src/tthAnalysis/HiggsToTauTau/scripts/check_that_histograms_are_valid.py' % config
    command_arguments = " ".join(histograms)
    command_with_arguments = command + " " + command_arguments + "; echo EXIT_STATUS_WAS: $?;"

    result = run_cmd(command_with_arguments)

    # Check result

    if result.find('ERROR: root input file is missing: %s' % missing_histogram) == -1:
        raise Exception('Ouput must contain error information what file was missing')

    if result.find('EXIT_STATUS_WAS: 1') == -1:
        raise Exception('Exit status must be 1 if file was missing')
