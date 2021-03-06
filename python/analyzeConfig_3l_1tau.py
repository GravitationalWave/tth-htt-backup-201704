import logging, re

from tthAnalysis.HiggsToTauTau.analyzeConfig_new import *
from tthAnalysis.HiggsToTauTau.jobTools import create_if_not_exists
from tthAnalysis.HiggsToTauTau.analysisTools import initDict, getKey, create_cfg, createFile, generateInputFileList

def get_lepton_and_hadTau_selection_and_frWeight(lepton_and_hadTau_selection, lepton_and_hadTau_frWeight):
  lepton_and_hadTau_selection_and_frWeight = lepton_and_hadTau_selection
  if lepton_and_hadTau_selection.startswith("Fakeable"):
    if lepton_and_hadTau_frWeight == "enabled":
      lepton_and_hadTau_selection_and_frWeight += "_wFakeRateWeights"
    elif lepton_and_hadTau_frWeight == "disabled":
      lepton_and_hadTau_selection_and_frWeight += "_woFakeRateWeights"
  lepton_and_hadTau_selection_and_frWeight = lepton_and_hadTau_selection_and_frWeight.replace("|", "_")    
  return lepton_and_hadTau_selection_and_frWeight

def getHistogramDir(lepton_selection, hadTau_selection, hadTau_frWeight, charge_selection):
  hadTau_selection_part1 = hadTau_selection
  for separator in [ "|" ]:
    if hadTau_selection_part1.find(separator) != -1:
      hadTau_selection_part1 = hadTau_selection_part1[:hadTau_selection_part1.find(separator)]
  histogramDir = "3l_1tau_%s_lep%s_tau%s" % (charge_selection, lepton_selection, hadTau_selection_part1)
  if hadTau_selection_part1.find("Fakeable") != -1:
    if hadTau_frWeight == "enabled":
      histogramDir += "_wFakeRateWeights"
    elif hadTau_frWeight == "disabled":
      histogramDir += "_woFakeRateWeights"
  return histogramDir

class analyzeConfig_3l_1tau(analyzeConfig):
  """Configuration metadata needed to run analysis in a single go.

  Sets up a folder structure by defining full path names; no directory creation is delegated here.

  Args specific to analyzeConfig_3l_1tau:
    lepton_selection: either `Tight`, `Loose` or `Fakeable`

  See $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/python/analyzeConfig.py
  for documentation of further Args.

  """
  def __init__(self, outputDir, executable_analyze, cfgFile_analyze, samples, hadTau_selection, applyFakeRateWeights, charge_selections, central_or_shifts,
               max_files_per_job, era, use_lumi, lumi, debug, running_method, num_parallel_jobs, 
               executable_addBackgrounds, executable_addBackgroundJetToTauFakes, histograms_to_fit, select_rle_output = False, executable_prep_dcard="prepareDatacard",
               select_root_output = False):
    analyzeConfig.__init__(self, outputDir, executable_analyze, "3l_1tau", central_or_shifts,
      max_files_per_job, era, use_lumi, lumi, debug, running_method, num_parallel_jobs, 
      histograms_to_fit)

    self.samples = samples

    ##self.lepton_and_hadTau_selections = [ "Tight", "Fakeable", "Fakeable_mcClosure" ]
    self.lepton_and_hadTau_selections = [ "Tight", "Fakeable" ]
    self.lepton_and_hadTau_frWeights = [ "enabled", "disabled" ]
    self.hadTau_selection_part2 = hadTau_selection
    self.applyFakeRateWeights = applyFakeRateWeights

    self.lepton_genMatches = [ "3l0j", "2l1j", "1l2j", "0l3j" ]
    self.hadTau_genMatches = [ "1t0e0m0j", "0t1e0m0j", "0t0e1m0j", "0t0e0m1j" ]

    self.apply_leptonGenMatching = None
    self.apply_hadTauGenMatching = None
    self.lepton_and_hadTau_genMatches_nonfakes = []
    self.lepton_and_hadTau_genMatches_fakes = []
    if self.applyFakeRateWeights == "4L":
      self.apply_leptonGenMatching = True
      self.apply_hadTauGenMatching = True
      for lepton_genMatch in self.lepton_genMatches:
        for hadTau_genMatch in self.hadTau_genMatches:
          lepton_and_hadTau_genMatch = "&".join([ lepton_genMatch, hadTau_genMatch ])
          if lepton_genMatch.endswith("0j") and hadTau_genMatch.endswith("0j"):
            self.lepton_and_hadTau_genMatches_nonfakes.append(lepton_and_hadTau_genMatch)
          else:
            self.lepton_and_hadTau_genMatches_fakes.append(lepton_and_hadTau_genMatch)
    elif applyFakeRateWeights == "3lepton":
      self.apply_leptonGenMatching = True
      self.apply_hadTauGenMatching = False
      for lepton_genMatch in self.lepton_genMatches:
        if lepton_genMatch.endswith("0j"):
          self.lepton_and_hadTau_genMatches_nonfakes.append(lepton_genMatch)
        else:
          self.lepton_and_hadTau_genMatches_fakes.append(lepton_genMatch)        
    elif applyFakeRateWeights == "1tau":
      self.apply_leptonGenMatching = False
      self.apply_hadTauGenMatching = True
      for hadTau_genMatch in self.hadTau_genMatches:
        if hadTau_genMatch.endswith("0j"):
          self.lepton_and_hadTau_genMatches_nonfakes.append(hadTau_genMatch)
        else:
          self.lepton_and_hadTau_genMatches_fakes.append(hadTau_genMatch)
    else:
      raise ValueError("Invalid Configuration parameter 'applyFakeRateWeights' = %s !!" % applyFakeRateWeights)

    self.charge_selections = charge_selections
    
    self.executable_addBackgrounds = executable_addBackgrounds
    self.executable_addFakes = executable_addBackgroundJetToTauFakes

    self.nonfake_backgrounds = [ "TT", "TTW", "TTZ", "EWK", "Rares" ]
    
    self.cfgFile_analyze = os.path.join(self.workingDir, cfgFile_analyze)
    self.prep_dcard_processesToCopy = [ "data_obs" ] + self.nonfake_backgrounds + [ "fakes_data", "fakes_mc" ]
    self.histogramDir_prep_dcard = "3l_1tau_OS_lepTight_tauTight"
    self.histogramDir_prep_dcard_SS = "3l_1tau_SS_lepTight_tauTight"
    ##self.make_plots_backgrounds = self.nonfake_backgrounds + [ "fakes_data" ]
    self.make_plots_backgrounds = [ "TTW", "TTZ", "EWK", "Rares", "fakes_data" ]
    self.cfgFile_make_plots_mcClosure = os.path.join(self.workingDir, "makePlots_mcClosure_cfg.py")

    self.select_rle_output = select_rle_output
    self.select_root_output = select_root_output

    self.isBDTtraining = False

  def set_BDT_training(self):
    """Run analysis with loose selection criteria for leptons and hadronic taus,
       for the purpose of preparing event list files for BDT training.
    """
    self.lepton_and_hadTau_selections = [ "forBDTtraining" ]
    self.lepton_and_hadTau_frWeights = [ "disabled" ]
    self.isBDTtraining = True

  def createCfg_analyze(self, jobOptions):
    """Create python configuration file for the analyze_3l_1tau executable (analysis code)

    Args:
      inputFiles: list of input files (Ntuples)
      outputFile: output file of the job -- a ROOT file containing histogram
      process: either `TT`, `TTW`, `TTZ`, `EWK`, `Rares`, `data_obs`, `ttH_hww`, `ttH_hzz` or `ttH_htt`
      is_mc: flag indicating whether job runs on MC (True) or data (False)
      lumi_scale: event weight (= xsection * luminosity / number of events)
      central_or_shift: either 'central' or one of the systematic uncertainties defined in $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/bin/analyze_3l_1tau.cc
    """  
    lines = []
    ##lines.append("process.fwliteInput.fileNames = cms.vstring(%s)" % [ os.path.basename(inputFile) for inputFile in jobOptions['ntupleFiles'] ])
    lines.append("process.fwliteInput.fileNames = cms.vstring(%s)" % jobOptions['ntupleFiles'])
    lines.append("process.fwliteOutput.fileName = cms.string('%s')" % os.path.basename(jobOptions['histogramFile']))
    lines.append("process.analyze_3l_1tau.process = cms.string('%s')" % jobOptions['sample_category'])
    hadTau_frWeight = None
    if jobOptions['applyFakeRateWeights'] == "disabled":
      hadTau_frWeight = "disabled"
    else:
      hadTau_frWeight = "enabled"
    histogramDir = getHistogramDir(jobOptions['lepton_selection'], jobOptions['hadTau_selection'], hadTau_frWeight, jobOptions['charge_selection'])
    lines.append("process.analyze_3l_1tau.histogramDir = cms.string('%s')" % histogramDir)
    lines.append("process.analyze_3l_1tau.era = cms.string('%s')" % self.era)
    lines.append("process.analyze_3l_1tau.triggers_1e = cms.vstring(%s)" % self.triggers_1e)
    lines.append("process.analyze_3l_1tau.use_triggers_1e = cms.bool(%s)" % ("1e" in jobOptions['triggers']))
    lines.append("process.analyze_3l_1tau.triggers_2e = cms.vstring(%s)" % self.triggers_2e)
    lines.append("process.analyze_3l_1tau.use_triggers_2e = cms.bool(%s)" % ("2e" in jobOptions['triggers']))
    lines.append("process.analyze_3l_1tau.triggers_1mu = cms.vstring(%s)" % self.triggers_1mu)
    lines.append("process.analyze_3l_1tau.use_triggers_1mu = cms.bool(%s)" % ("1mu" in jobOptions['triggers']))
    lines.append("process.analyze_3l_1tau.triggers_2mu = cms.vstring(%s)" % self.triggers_2mu)
    lines.append("process.analyze_3l_1tau.use_triggers_2mu = cms.bool(%s)" % ("2mu" in jobOptions['triggers']))
    lines.append("process.analyze_3l_1tau.triggers_1e1mu = cms.vstring(%s)" % self.triggers_1e1mu)
    lines.append("process.analyze_3l_1tau.use_triggers_1e1mu = cms.bool(%s)" % ("1e1mu" in jobOptions['triggers']))
    lines.append("process.analyze_3l_1tau.leptonSelection = cms.string('%s')" % jobOptions['lepton_selection'])
    lines.append("process.analyze_3l_1tau.apply_leptonGenMatching = cms.bool(%s)" % (jobOptions['apply_leptonGenMatching'] and jobOptions['is_mc']))
    lines.append("process.analyze_3l_1tau.hadTauSelection = cms.string('%s')" % jobOptions['hadTau_selection'])
    lines.append("process.analyze_3l_1tau.apply_hadTauGenMatching = cms.bool(%s)" % (jobOptions['apply_hadTauGenMatching'] and jobOptions['is_mc']))
    lines.append("process.analyze_3l_1tau.applyFakeRateWeights = cms.string('%s')" % jobOptions['applyFakeRateWeights'])
    if jobOptions['hadTau_selection'].find("Fakeable") != -1 and jobOptions['applyFakeRateWeights'] in [ "4L", "1tau" ]:
      fitFunctionName = None
      if self.era == "2015":
        lines.append("process.analyze_3l_1tau.hadTauFakeRateWeight.inputFileName = cms.string('tthAnalysis/HiggsToTauTau/data/FR_tau_2015.root')")
        # CV: use data/MC corrections determined for dR03mvaLoose discriminator,
        #     as the event statistics in 2015 data is too low to determine data/MC corrections for tighter working-points
        fitFunctionName = "jetToTauFakeRate/dR03mvaLoose/$etaBin/fitFunction_data_div_mc_hadTaus_pt"        
      elif self.era == "2016":
        lines.append("process.analyze_3l_1tau.hadTauFakeRateWeight.inputFileName = cms.string('tthAnalysis/HiggsToTauTau/data/FR_tau_2016.root')")
        # CV: use data/MC corrections determined for dR03mvaMedium discriminator for 2016 data
        fitFunctionName = "jetToTauFakeRate/dR03mvaMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt"   
      else:
        raise ValueError("Invalid parameter 'era' = %s !!" % self.era)
      lines.append("process.analyze_3l_1tau.hadTauFakeRateWeight.lead.fitFunctionName = cms.string('%s')" % fitFunctionName)
    if jobOptions['hadTau_selection'].find("mcClosure") != -1:
      lines.append("process.analyze_3l_1tau.hadTauFakeRateWeight.applyFitFunction_lead = cms.bool(False)")
    lines.append("process.analyze_3l_1tau.chargeSelection = cms.string('%s')" % jobOptions['charge_selection'])
    lines.append("process.analyze_3l_1tau.isMC = cms.bool(%s)" % jobOptions['is_mc'])
    lines.append("process.analyze_3l_1tau.central_or_shift = cms.string('%s')" % jobOptions['central_or_shift'])
    lines.append("process.analyze_3l_1tau.lumiScale = cms.double(%f)" % jobOptions['lumi_scale'])
    lines.append("process.analyze_3l_1tau.apply_genWeight = cms.bool(%s)" % jobOptions['apply_genWeight'])
    lines.append("process.analyze_3l_1tau.apply_trigger_bits = cms.bool(%s)" % jobOptions['apply_trigger_bits'])
    lines.append("process.analyze_3l_1tau.selEventsFileName_output = cms.string('%s')" % jobOptions['rleOutputFile'])
    lines.append("process.analyze_3l_1tau.selEventsTFileName = cms.string('%s')" % jobOptions['rootOutputFile'])
    create_cfg(self.cfgFile_analyze, jobOptions['cfgFile_modified'], lines)

  def createCfg_makePlots_mcClosure(self, jobOptions):
    """Fills the template of python configuration file for making control plots

    Args:
      histogramFile: name of the input ROOT file 
    """
    lines = []
    lines.append("process.fwliteInput.fileNames = cms.vstring('%s')" % jobOptions['inputFile'])
    lines.append("process.makePlots_mcClosure.outputFileName = cms.string('%s')" % jobOptions['outputFile'])
    lines.append("process.makePlots_mcClosure.processesBackground = cms.vstring(%s)" % self.make_plots_backgrounds)
    lines.append("process.makePlots_mcClosure.processSignal = cms.string('%s')" % self.make_plots_signal)
    lines.append("process.makePlots_mcClosure.categories = cms.VPSet(")
    lines.append("  cms.PSet(")
    lines.append("    signal = cms.string('%s')," % self.histogramDir_prep_dcard)
    lines.append("    sideband = cms.string('%s')," % self.histogramDir_prep_dcard.replace("Tight", "Fakeable_mcClosure_wFakeRateWeights"))
    lines.append("    label = cms.string('%s')" % self.channel)
    lines.append("  )")
    lines.append(")")
    lines.append("process.makePlots_mcClosure.distributions = cms.VPSet(")
    lines.append("  cms.PSet(")
    lines.append("    histogramName = cms.string('sel/evt/$PROCESS/numJets'),")
    lines.append("    xAxisTitle = cms.string('jet Multiplicity'),")
    lines.append("    yAxisTitle = cms.string('N')")
    lines.append("  ),")
    lines.append("  cms.PSet(")
    lines.append("    histogramName = cms.string('sel/evt/$PROCESS/mvaDiscr_3l'),")
    lines.append("    xAxisTitle = cms.string('MVA Discriminant'),")
    lines.append("    yAxisTitle = cms.string('N')")
    lines.append("  ),")
    lines.append("  cms.PSet(")
    lines.append("    histogramName = cms.string('sel/evt/$PROCESS/mTauTauVis'),")
    lines.append("    xAxisTitle = cms.string('m_{#tau#tau}^{vis} [GeV]'),")
    lines.append("    yAxisTitle = cms.string('dN/dm_{#tau#tau}^{vis} [1/GeV]')")
    lines.append("  )")
    lines.append(")")
    create_cfg(self.cfgFile_make_plots_mcClosure, jobOptions['cfgFile_modified'], lines)
    
  def create(self):
    """Creates all necessary config files and runs the complete analysis workfow -- either locally or on the batch system
    """

    for sample_name, sample_info in self.samples.items():
      if not sample_info["use_it"] or sample_info["sample_category"] in [ "additional_signal_overlap", "background_data_estimate" ]:
        continue
      process_name = sample_info["process_name_specific"]
      for lepton_and_hadTau_selection in self.lepton_and_hadTau_selections:
        for lepton_and_hadTau_frWeight in self.lepton_and_hadTau_frWeights:
          if lepton_and_hadTau_frWeight == "enabled" and not lepton_and_hadTau_selection.startswith("Fakeable"):
            continue
          lepton_and_hadTau_selection_and_frWeight = get_lepton_and_hadTau_selection_and_frWeight(lepton_and_hadTau_selection, lepton_and_hadTau_frWeight)
          for charge_selection in self.charge_selections:
            key_dir = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)  
            for dir_type in [ DKEY_CFGS, DKEY_HIST, DKEY_LOGS, DKEY_ROOT, DKEY_RLES ]:
              initDict(self.dirs, [ key_dir, dir_type ])
              self.dirs[key_dir][dir_type] = os.path.join(self.outputDir, dir_type, self.channel,
                "_".join([ lepton_and_hadTau_selection_and_frWeight, charge_selection ]), process_name)
    for dir_type in [ DKEY_SCRIPTS, DKEY_DCRD, DKEY_PLOT ]:
      initDict(self.dirs, [ dir_type ])
      self.dirs[dir_type] = os.path.join(self.outputDir, dir_type, self.channel)
    ##print "self.dirs = ", self.dirs

    for key in self.dirs.keys():
      if type(self.dirs[key]) == dict:
        for dir_type in self.dirs[key].keys():
          create_if_not_exists(self.dirs[key][dir_type])
      else:
        create_if_not_exists(self.dirs[key])

    for lepton_and_hadTau_selection in self.lepton_and_hadTau_selections:
      lepton_selection = lepton_and_hadTau_selection
      if self.applyFakeRateWeights == "1tau":
        lepton_selection = "Tight"
      hadTau_selection = lepton_and_hadTau_selection
      if self.applyFakeRateWeights == "3lepton":
        hadTau_selection = "Tight"
      hadTau_selection = "|".join([ hadTau_selection, self.hadTau_selection_part2 ])

      if lepton_and_hadTau_selection == "forBDTtraining":
        lepton_selection = "Loose"
        hadTau_selection = "Tight|dR03mvaVVLoose"
      
      for lepton_and_hadTau_frWeight in self.lepton_and_hadTau_frWeights:
        if lepton_and_hadTau_frWeight == "enabled" and not lepton_and_hadTau_selection.startswith("Fakeable"):
          continue
        if lepton_and_hadTau_frWeight == "disabled" and not lepton_and_hadTau_selection in [ "Tight", "forBDTtraining" ]:
          continue
        lepton_and_hadTau_selection_and_frWeight = get_lepton_and_hadTau_selection_and_frWeight(lepton_and_hadTau_selection, lepton_and_hadTau_frWeight)

        for charge_selection in self.charge_selections:

          for sample_name, sample_info in self.samples.items():
            if not sample_info["use_it"] or sample_info["sample_category"] in [ "additional_signal_overlap", "background_data_estimate" ]:
              continue
            process_name = sample_info["process_name_specific"]
            logging.info("Creating configuration files to run '%s' for sample %s" % (self.executable_analyze, process_name))  
            
            sample_category = sample_info["sample_category"]
            is_mc = (sample_info["type"] == "mc")
            is_signal = (sample_category == "signal")

            for central_or_shift in self.central_or_shifts:

              inputFileList = generateInputFileList(sample_name, sample_info, self.max_files_per_job, self.debug)
              for jobId in inputFileList.keys():
                if central_or_shift != "central" and not (lepton_and_hadTau_selection.startswith("Tight") and charge_selection == "OS"):
                  continue
                if central_or_shift != "central" and not is_mc:
                  continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttH") and sample_category != "signal":
                  continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttW") and sample_category != "TTW":
                  continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttZ") and sample_category != "TTZ":
                  continue

                # build config files for executing analysis code
                key_dir = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                key_analyze_job = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId)
                ntupleFiles = inputFileList[jobId]
                if len(ntupleFiles) == 0:
                  print "Warning: ntupleFiles['%s'] = %s --> skipping job !!" % (key_job, ntupleFiles)
                  continue
                rootOutputFile = ""
                if self.select_root_output:
                  rootOutputFile = os.path.join(self.dirs[key_dir][DKEY_ROOT], "out_%s_%s_%s_%s_%s_%i.root" % \
                    (self.channel, process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId))
                  key_file_woJobId = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift)
                  if key_file_woJobId not in self.rootOutputAux:
                    self.rootOutputAux[key_file_woJobId] = [ re.sub('_\d+\.root', '.root', rootOutputFile),
                                                             re.sub('\d+\.root', '*.root', rootOutputFile) ]
                self.jobOptions_analyze[key_analyze_job] = {
                  'ntupleFiles' : ntupleFiles,
                  'cfgFile_modified' : os.path.join(self.dirs[key_dir][DKEY_CFGS], "analyze_%s_%s_%s_%s_%s_%i_cfg.py" % \
                    (self.channel, process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId)),
                  'histogramFile' : os.path.join(self.dirs[key_dir][DKEY_HIST], "%s_%s_%s_%s_%i.root" % \
                    (process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId)),
                  'logFile' : os.path.join(self.dirs[key_dir][DKEY_LOGS], "analyze_%s_%s_%s_%s_%s_%i.log" % \
                    (self.channel, process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId)),
                  'rleOutputFile' : os.path.join(self.dirs[key_dir][DKEY_RLES], "rle_%s_%s_%s_%s_%s_%i.txt" % \
                    (self.channel, process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection, central_or_shift, jobId)) if self.select_rle_output else "",
                  'rootOutputFile' : rootOutputFile,
                  'sample_category' : sample_category,
                  'triggers' : sample_info["triggers"],
                  'lepton_selection' : lepton_selection,
                  'apply_leptonGenMatching' : self.apply_leptonGenMatching,                  
                  'hadTau_selection' :  hadTau_selection,
                  'apply_hadTauGenMatching' : self.apply_hadTauGenMatching,
                  'charge_selection' : charge_selection,
                  'applyFakeRateWeights' : self.applyFakeRateWeights if not (lepton_selection == "Tight" and hadTau_selection.find("Tight") != -1) else "disabled",
                  'is_mc' : is_mc,
                  'central_or_shift' : central_or_shift,
                  'lumi_scale' : 1. if not (self.use_lumi and is_mc) else sample_info["xsection"] * self.lumi / sample_info["nof_events"],
                  'apply_genWeight' : sample_info["genWeight"] if (is_mc and "genWeight" in sample_info.keys()) else False,
                  'apply_trigger_bits' : (is_mc and (self.era == "2015" or (self.era == "2016" and sample_info["reHLT"]))) or not is_mc
                }

                applyFakeRateWeights = self.applyFakeRateWeights
                if lepton_and_hadTau_frWeight == "disabled":
                  applyFakeRateWeights = "disabled"
                self.createCfg_analyze(self.jobOptions_analyze[key_analyze_job])

                # initialize input and output file names for hadd_stage1
                key_hadd_stage1 = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                if not key_hadd_stage1 in self.inputFiles_hadd_stage1.keys():
                  self.inputFiles_hadd_stage1[key_hadd_stage1] = []
                self.inputFiles_hadd_stage1[key_hadd_stage1].append(self.jobOptions_analyze[key_analyze_job]['histogramFile'])
                self.outputFile_hadd_stage1[key_hadd_stage1] = os.path.join(self.outputDir, DKEY_HIST, "histograms_harvested_stage1_%s_%s_%s_%s.root" % \
                    (self.channel, process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection))

            if is_mc:
              logging.info("Creating configuration files to run 'addBackgrounds' for sample %s" % process_name)

              sample_categories = [ sample_category ]
              if is_signal:
                sample_categories = [ "signal", "ttH", "ttH_htt", "ttH_hww", "ttH_hzz" ]
              for sample_category in sample_categories:
                # sum non-fake contributions for each MC sample separately
                # input processes: TT3l0j; TTW3l0j,...
                # output processes: TT; ...
                key_hadd_stage1 = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                key_addBackgrounds_job = getKey(process_name, sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                processes_input = None
                if sample_category in [ "signal" ]:
                  lepton_and_hadTau_genMatches = []
                  lepton_and_hadTau_genMatches.extend(self.lepton_and_hadTau_genMatches_nonfakes)
                  lepton_and_hadTau_genMatches.extend(self.lepton_and_hadTau_genMatches_fakes)
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in lepton_and_hadTau_genMatches ]
                elif sample_category in [ "ttH" ]:
                  processes_input = []
                  processes_input.extend([ "%s%s" % ("ttH_htt", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_nonfakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hww", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_nonfakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hzz", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_nonfakes ])                
                else:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.lepton_and_hadTau_genMatches_nonfakes ]
                if processes_input:  
                  self.jobOptions_addBackgrounds[key_addBackgrounds_job] = {
                    'inputFile' : self.outputFile_hadd_stage1[key_hadd_stage1],
                    'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "addBackgrounds_%s_%s_%s_%s_%s_cfg.py" % \
                      (self.channel, process_name, sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
                    'outputFile' : os.path.join(self.outputDir, DKEY_HIST, "addBackgrounds_%s_%s_%s_%s_%s.root" % \
                      (self.channel, process_name, sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
                    'categories' : [ getHistogramDir(lepton_selection, hadTau_selection, lepton_and_hadTau_frWeight, charge_selection) ],
                    'processes_input' : processes_input,
                    'process_output' : sample_category
                  }
                  self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds[key_addBackgrounds_job])
                      
                  # initialize input and output file names for hadd_stage1_5
                  key_hadd_stage1_5 = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
                  if not key_hadd_stage1_5 in self.inputFiles_hadd_stage1_5.keys():
                    self.inputFiles_hadd_stage1_5[key_hadd_stage1_5] = []
                  self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.jobOptions_addBackgrounds[key_addBackgrounds_job]['outputFile'])
                  self.outputFile_hadd_stage1_5[key_hadd_stage1_5] = os.path.join(self.outputDir, DKEY_HIST, "histograms_harvested_stage1_5_%s_%s_%s.root" % \
                    (self.channel, lepton_and_hadTau_selection_and_frWeight, charge_selection))
  
                # sum fake contributions for each MC sample separately
                # input processes: TT2l1j, TT1l2j, TT0l3j; TTW2l1j,...
                # output processes: fakes_TT; ...
                key_hadd_stage1 = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                key_addBackgrounds_job = getKey(process_name, "%s_fake" % sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)
                processes_input = None
                if sample_category in [ "signal" ]:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.lepton_and_hadTau_genMatches_fakes ]
                elif sample_category in [ "ttH" ]:
                  processes_input = []
                  processes_input.extend([ "%s%s" % ("ttH_htt", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_fakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hww", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_fakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hzz", genMatch) for genMatch in self.lepton_and_hadTau_genMatches_fakes ])                
                else:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.lepton_and_hadTau_genMatches_fakes ]
                if processes_input:  
                  self.jobOptions_addBackgrounds[key_addBackgrounds_job] = {
                    'inputFile' : self.outputFile_hadd_stage1[key_hadd_stage1],
                    'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "addBackgrounds_%s_fakes_%s_%s_%s_%s_cfg.py" % \
                      (self.channel, process_name, sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
                    'outputFile' : os.path.join(self.outputDir, DKEY_HIST, "addBackgrounds_%s_fakes_%s_%s_%s_%s.root" % \
                      (self.channel, process_name, sample_category, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
                    'categories' : [ getHistogramDir(lepton_selection, hadTau_selection, lepton_and_hadTau_frWeight, charge_selection) ],
                    'processes_input' : processes_input,
                    'process_output' : "%s_fake" % sample_category
                  }
                  self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds[key_addBackgrounds_job])
                      
                  # initialize input and output file names for hadd_stage1_5
                  key_hadd_stage1_5 = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
                  if not key_hadd_stage1_5 in self.inputFiles_hadd_stage1_5.keys():
                    self.inputFiles_hadd_stage1_5[key_hadd_stage1_5] = []
                  self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.jobOptions_addBackgrounds[key_addBackgrounds_job]['outputFile'])
                  self.outputFile_hadd_stage1_5[key_hadd_stage1_5] = os.path.join(self.outputDir, DKEY_HIST, "histograms_harvested_stage1_5_%s_%s_%s.root" % \
                    (self.channel, lepton_and_hadTau_selection_and_frWeight, charge_selection))

            # add output files of hadd_stage1 for data to list of input files for hadd_stage1_5
            if not is_mc:
              key_hadd_stage1 = getKey(process_name, lepton_and_hadTau_selection_and_frWeight, charge_selection)
              key_hadd_stage1_5 = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
              self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.outputFile_hadd_stage1[key_hadd_stage1])

          # sum fake contributions for the total of all MC sample
          # input processes: TT1t0e0m1j, TT0t1e0m1j, TT0t0e1m1j, TT0t0e0m2j; TTW1t0e0m1j,...
          # output process: fakes_mc
          key_addBackgrounds_job = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
          sample_categories = []
          sample_categories.extend(self.nonfake_backgrounds)
          sample_categories.extend([ "signal" ])
          processes_input = []
          for sample_category in sample_categories:
            processes_input.append("%s_fake" % sample_category)            
          self.jobOptions_addBackgrounds[key_addBackgrounds_job] = {
            'inputFile' : self.outputFile_hadd_stage1_5[key_hadd_stage1_5],
            'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "addBackgrounds_%s_fakes_mc_%s_%s_cfg.py" % \
              (self.channel, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
            'outputFile' : os.path.join(self.outputDir, DKEY_HIST, "addBackgrounds_%s_fakes_mc_%s_%s.root" % \
              (self.channel, lepton_and_hadTau_selection_and_frWeight, charge_selection)),
            'categories' : [ getHistogramDir(lepton_selection, hadTau_selection, lepton_and_hadTau_frWeight, charge_selection) ],
            'processes_input' : processes_input,
            'process_output' : "fakes_mc"
          }
          self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds[key_addBackgrounds_job])

          # initialize input and output file names for hadd_stage2
          key_hadd_stage2 = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
          if not key_hadd_stage2 in self.inputFiles_hadd_stage2.keys():
            self.inputFiles_hadd_stage2[key_hadd_stage2] = []
          if lepton_and_hadTau_selection == "Tight":
            self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.jobOptions_addBackgrounds[key_addBackgrounds_job]['outputFile'])
          key_hadd_stage1_5 = getKey(lepton_and_hadTau_selection_and_frWeight, charge_selection)
          self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.outputFile_hadd_stage1_5[key_hadd_stage1_5])
          self.outputFile_hadd_stage2[key_hadd_stage2] = os.path.join(self.outputDir, DKEY_HIST, "histograms_harvested_stage2_%s_%s_%s.root" % \
            (self.channel, lepton_and_hadTau_selection_and_frWeight, charge_selection))

    if self.isBDTtraining:
      if self.is_sbatch:
        logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable_analyze)
        self.sbatchFile_analyze = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_analyze_%s.py" % self.channel)
        self.createScript_sbatch()        
      logging.info("Creating Makefile")
      lines_makefile = []
      self.addToMakefile_analyze(lines_makefile)
      self.createMakefile(lines_makefile)      
      logging.info("Done")      
      return

    logging.info("Creating configuration files to run 'addBackgroundFakes'")
    for charge_selection in self.charge_selections:
      key_addFakes_job = getKey("fakes_data", charge_selection)
      key_hadd_stage1_5 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Fakeable", "enabled"), charge_selection)
      category_sideband = None
      if self.applyFakeRateWeights == "3lepton":
        category_sideband = "3l_1tau_%s_lepFakeable_tauTight" % charge_selection
      elif self.applyFakeRateWeights == "4L":
        category_sideband = "3l_1tau_%s_lepFakeable_tauFakeable_wFakeRateWeights" % charge_selection
      elif self.applyFakeRateWeights == "1tau":
        category_sideband = "3l_1tau_%s_lepTight_tauFakeable_wFakeRateWeights" % charge_selection 
      else:
        raise ValueError("Invalid Configuration parameter 'applyFakeRateWeights' = %s !!" % applyFakeRateWeights)
      self.jobOptions_addFakes[key_addFakes_job] = {
        'inputFile' : self.outputFile_hadd_stage1_5[key_hadd_stage1_5],
        'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "addBackgroundLeptonFakes_%s_%s_cfg.py" % \
          (self.channel, charge_selection)),
        'outputFile' : os.path.join(self.outputDir, DKEY_HIST, "addBackgroundLeptonFakes_%s_%s.root" % \
          (self.channel, charge_selection)),
        'category_signal' : "3l_1tau_%s_lepTight_tauTight" % charge_selection,
        'category_sideband' : category_sideband
      }
      self.createCfg_addFakes(self.jobOptions_addFakes[key_addFakes_job])
      key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), charge_selection)
      self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.jobOptions_addFakes[key_addFakes_job]['outputFile'])

    logging.info("Creating configuration files to run 'prepareDatacards'")
    for histogramToFit in self.histograms_to_fit:
      key_prep_dcard_job = getKey(histogramToFit)
      key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")                      
      self.jobOptions_prep_dcard[key_prep_dcard_job] = {
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "prepareDatacards_%s_%s_cfg.py" % (self.channel, histogramToFit)),
        'datacardFile' : os.path.join(self.outputDir, DKEY_DCRD, "prepareDatacards_%s_%s.root" % (self.channel, histogramToFit)),
        'histogramDir' : self.histogramDir_prep_dcard,
        'histogramToFit' : histogramToFit,
        'label' : None
      }                            
      self.createCfg_prep_dcard(self.jobOptions_prep_dcard[key_prep_dcard_job])
      if "SS" in self.charge_selections:
        key_prep_dcard_job = getKey(histogramToFit, "SS")
        key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), "SS")                      
        self.jobOptions_prep_dcard[key_prep_dcard_job] = {
          'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
          'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "prepareDatacards_%s_SS_%s_cfg.py" % (self.channel, histogramToFit)),
          'datacardFile' : os.path.join(self.outputDir, DKEY_DCRD, "prepareDatacards_%s_SS_%s.root" % (self.channel, histogramToFit)),
          'histogramDir' : self.histogramDir_prep_dcard_SS,
          'histogramToFit' : histogramToFit,
          'label' : 'SS'
        }                            
        self.createCfg_prep_dcard(self.jobOptions_prep_dcard[key_prep_dcard_job])

    logging.info("Creating configuration files to run 'makePlots'")
    key_makePlots_job = getKey("OS")
    key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")                            
    self.jobOptions_make_plots[key_makePlots_job] = {
      'executable' : self.executable_make_plots,
      'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
      'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "makePlots_%s_cfg.py" % self.channel),
      'outputFile' : os.path.join(self.outputDir, DKEY_PLOT, self.channel, "makePlots_%s.png" % self.channel),
      'histogramDir' : self.histogramDir_prep_dcard,
      'label' : None,
      'make_plots_backgrounds' : self.make_plots_backgrounds
    }
    self.createCfg_makePlots(self.jobOptions_make_plots[key_makePlots_job])
    if "SS" in self.charge_selections:
      key_makePlots_job = getKey("SS")
      key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), "SS")                        
      self.jobOptions_make_plots[key_makePlots_job] = {
        'executable' : self.executable_make_plots,
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "makePlots_%s_SS_cfg.py" % self.channel),
        'outputFile' : os.path.join(self.outputDir, DKEY_PLOT, self.channel, "makePlots_%s_SS.png" % self.channel),                       
        'histogramDir' : self.histogramDir_prep_dcard_SS,
        'label' : "SS",
        'make_plots_backgrounds' : self.make_plots_backgrounds
      }
      self.createCfg_makePlots(self.jobOptions_make_plots[key_makePlots_job])
    if "Fakeable_mcClosure" in self.lepton_and_hadTau_selections:
      key_makePlots_job = getKey("OS")
      key_hadd_stage2 = getKey(get_lepton_and_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")
      self.jobOptions_make_plots[key_makePlots_job] = {
        'executable' : self.executable_make_plots_mcClosure,
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.outputDir, DKEY_CFGS, "makePlots_mcClosure_%s_cfg.py" % self.channel),
        'outputFile' : os.path.join(self.outputDir, DKEY_PLOT, self.channel, "makePlots_mcClosure_%s.png" % self.channel)
      }
      self.createCfg_makePlots_mcClosure(self.jobOptions_make_plots[key_makePlots_job])
                
    if self.is_sbatch:
      logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable_analyze)
      self.sbatchFile_analyze = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_analyze_%s.py" % self.channel)
      self.createScript_sbatch()

    logging.info("Creating Makefile")
    lines_makefile = []
    self.addToMakefile_analyze(lines_makefile)
    self.addToMakefile_hadd_stage1(lines_makefile)
    self.addToMakefile_backgrounds_from_data(lines_makefile)
    self.addToMakefile_hadd_stage2(lines_makefile)
    self.addToMakefile_prep_dcard(lines_makefile)
    self.addToMakefile_make_plots(lines_makefile)
    self.createMakefile(lines_makefile)
  
    logging.info("Done")

