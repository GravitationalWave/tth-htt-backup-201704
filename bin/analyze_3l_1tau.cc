
#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h" // edm::readPSetsFrom()
#include "FWCore/ParameterSet/interface/FileInPath.h" // edm::FileInPath
#include "FWCore/Utilities/interface/Exception.h" // cms::Exception
#include "PhysicsTools/FWLite/interface/TFileService.h" // fwlite::TFileService
#include "DataFormats/FWLite/interface/InputSource.h" // fwlite::InputSource
#include "DataFormats/FWLite/interface/OutputFiles.h" // fwlite::OutputFiles
#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector
#include "DataFormats/Math/interface/deltaR.h" // deltaR

#include <Rtypes.h> // Int_t, Long64_t, Double_t
#include <TChain.h> // TChain
#include <TTree.h> // TTree
#include <TBenchmark.h> // TBenchmark
#include <TString.h> // TString, Form

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h" // RecoHadTau
#include "tthAnalysis/HiggsToTauTau/interface/GenLepton.h" // GenLepton
#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTau.h" // GenHadTau
#include "tthAnalysis/HiggsToTauTau/interface/TMVAInterface.h" // TMVAInterface
#include "tthAnalysis/HiggsToTauTau/interface/mvaInputVariables.h" // auxiliary functions for computing input variables of the MVA used for signal extraction in the 3l_1tau category
#include "tthAnalysis/HiggsToTauTau/interface/JetToTauFakeRateInterface.h" // JetToTauFakeRateInterface
#include "tthAnalysis/HiggsToTauTau/interface/KeyTypes.h"
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronReader.h" // RecoElectronReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonReader.h" // RecoMuonReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTauReader.h" // GenHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionCleaner.h" // RecoElectronCollectionCleaner, RecoMuonCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleaner
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorLoose.h" // RecoHadTauCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorFakeable.h" // RecoHadTauCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/ElectronHistManager.h" // ElectronHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MuonHistManager.h" // MuonHistManager
#include "tthAnalysis/HiggsToTauTau/interface/HadTauHistManager.h" // HadTauHistManager
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManager.h" // JetHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MEtHistManager.h" // MEtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/EvtHistManager_3l_1tau.h" // EvtHistManager_3l_1tau
#include "tthAnalysis/HiggsToTauTau/interface/GenEvtHistManager.h" // GenEvtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // getLeptonType, kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // isHigherPt, isMatched
#include "tthAnalysis/HiggsToTauTau/interface/hadTauGenMatchingAuxFunctions.h" // getHadTauGenMatch_definitions_3tau, getHadTauGenMatch
#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath, create_hltPaths, hltPaths_setBranchAddresses, hltPaths_isTriggered, hltPaths_delete
#include "tthAnalysis/HiggsToTauTau/interface/data_to_MC_corrections.h"
#include "tthAnalysis/HiggsToTauTau/interface/lutAuxFunctions.h" // loadTH2, get_sf_from_TH2
#include "tthAnalysis/HiggsToTauTau/interface/cutFlowTable.h" // cutFlowTableType

#include <boost/range/algorithm/copy.hpp> // boost::copy()
#include <boost/range/adaptor/map.hpp> // boost::adaptors::map_keys

#include <iostream> // std::cerr, std::fixed
#include <iomanip> // std::setprecision(), std::setw()
#include <string> // std::string
#include <vector> // std::vector<>
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <algorithm> // std::sort(), std::find()
#include <fstream> // std::ofstream
#include <assert.h> // assert
#include <array> // std::array<>

#define EPS 1E-2

typedef math::PtEtaPhiMLorentzVector LV;
typedef std::vector<std::string> vstring;

const int hadTauSelection_antiElectron = 1; // vLoose
const int hadTauSelection_antiMuon = 1; // Loose 

/**
 * @brief Auxiliary function used for sorting jets by decreasing btagging CSV value
 * @param Given pair of jets
 * @return True, if first jet has higher CSV; false if second jet has higher CSV
 */
bool isHigherCSV(const RecoJet* jet1, const RecoJet* jet2)
{
  return (jet1->BtagCSV_ > jet2->BtagCSV_);
}

/**
 * @brief Encapsulated input information of an event
 *        (run, lumi, event numbers; MET) needed by MEM
 * @todo Drop it into a separate file
 */
struct
EventSpecifics_Out
{
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;
  Double_t met_pt;
  Double_t met_phi;

  TBranch * branch_run = 0;
  TBranch * branch_lumi = 0;
  TBranch * branch_evt = 0;
  TBranch * branch_met_pt = 0;
  TBranch * branch_met_phi = 0;

  void
  setBranches(TTree * t)
  {
    branch_run     = t -> Branch("run",     &run,     "run/i");
    branch_lumi    = t -> Branch("lumi",    &lumi,    "lumi/i");
    branch_evt     = t -> Branch("evt",     &evt,     "evt/l");
    branch_met_pt  = t -> Branch("met_pt",  &met_pt,  "met_pt/D");
    branch_met_phi = t -> Branch("met_phi", &met_phi, "met_phi/D");
  }
};

/**
 * @brief MVA-specific variables
 * @note Not needed by MEM per se, but still necessary in BDT training
 *       (the root file containing all events passing the cuts might
 *        be used as an input in separate BDT training routines)
 */
struct
EventMVAIO_Out
{
  Double_t max2LeptonEta;
  Double_t MT_met_lep1;
  Double_t nJet25_Recl;
  Double_t mindr_lep1_jet;
  Double_t mindr_lep2_jet;
  Double_t lep1_cone_pt;
  Double_t lep3_cone_pt;
  Double_t avg_dr_jet;
  Double_t mhtJet25_Recl;
  Double_t mvaOutput_3l_ttV;
  Double_t mvaOutput_3l_ttbar;

  TBranch * branch_max2LeptonEta;
  TBranch * branch_MT_met_lep1;
  TBranch * branch_nJet25_Recl;
  TBranch * branch_mindr_lep1_jet;
  TBranch * branch_mindr_lep2_jet;
  TBranch * branch_lep1_cone_pt;
  TBranch * branch_lep3_cone_pt;
  TBranch * branch_avg_dr_jet;
  TBranch * branch_mhtJet25_Recl;
  TBranch * branch_mvaOutput_3l_ttV;
  TBranch * branch_mvaOutput_3l_ttbar;

  void
  setBranches(TTree * t)
  {
    branch_max2LeptonEta      = t -> Branch("max2LeptonEta",
                                            &max2LeptonEta,      "max2LeptonEta/D");
    branch_MT_met_lep1        = t -> Branch("MT_met_lep1",
                                            &MT_met_lep1,        "MT_met_lep1/D");
    branch_nJet25_Recl        = t -> Branch("nJet25_Recl",
                                            &nJet25_Recl,        "nJet25_Recl/D");
    branch_mindr_lep1_jet     = t -> Branch("mindr_lep1_jet",
                                            &mindr_lep1_jet,     "mindr_lep1_jet/D");
    branch_mindr_lep2_jet     = t -> Branch("mindr_lep2_jet",
                                            &mindr_lep2_jet,     "mindr_lep2_jet/D");
    branch_lep1_cone_pt       = t -> Branch("lep1_cone_pt",
                                            &lep1_cone_pt,       "lep1_cone_pt/D");
    branch_lep3_cone_pt       = t -> Branch("lep3_cone_pt",
                                            &lep3_cone_pt,       "lep3_cone_pt/D");
    branch_avg_dr_jet         = t -> Branch("avg_dr_jet",
                                            &avg_dr_jet,         "avg_dr_jet/D");
    branch_mhtJet25_Recl      = t -> Branch("mhtJet25_Recl",
                                            &mhtJet25_Recl,      "mhtJet25_Recl/D");
    branch_mvaOutput_3l_ttV   = t -> Branch("mvaOutput_3l_ttV",
                                            &mvaOutput_3l_ttV,   "mvaOutput_3l_ttV/D");
    branch_mvaOutput_3l_ttbar = t -> Branch("mvaOutput_3l_ttbar",
                                            &mvaOutput_3l_ttbar, "mvaOutput_3l_ttbar/D");
  }

  void
  setValues(const std::map<std::string, Double_t> & mvaInputs,
            Double_t mvaOutput_3l_ttV,
            Double_t mvaOutput_3l_ttbar)
  {
    max2LeptonEta  = mvaInputs.at("max(abs(LepGood_eta[iF_Recl[0]]),abs(LepGood_eta[iF_Recl[1]]))");
    MT_met_lep1    = mvaInputs.at("MT_met_lep1");
    nJet25_Recl    = mvaInputs.at("nJet25_Recl");
    mindr_lep1_jet = mvaInputs.at("mindr_lep1_jet");
    mindr_lep2_jet = mvaInputs.at("mindr_lep2_jet");
    lep1_cone_pt   = mvaInputs.at("LepGood_conePt[iF_Recl[0]]");
    lep3_cone_pt   = mvaInputs.at("LepGood_conePt[iF_Recl[2]]");
    avg_dr_jet     = mvaInputs.at("avg_dr_jet");
    mhtJet25_Recl  = mvaInputs.at("mhtJet25_Recl");
    this -> mvaOutput_3l_ttV   = mvaOutput_3l_ttV;
    this -> mvaOutput_3l_ttbar = mvaOutput_3l_ttbar;
  }
};

/**
 * @brief Encapsulated input information of an event
 *        (particle collection) needed by MEM
 * @todo Drop it into a separate file
 */
struct
EventObject_Out
{
  enum ParticleType_Out { kLepton, kJet, kHadronicTau };

  Double_t pt;
  Double_t eta;
  Double_t phi;
  Double_t mass;
  Int_t    charge;
  Int_t    decayMode;

  TBranch * branch_pt = 0;
  TBranch * branch_eta = 0;
  TBranch * branch_phi = 0;
  TBranch * branch_mass = 0;
  TBranch * branch_charge = 0;
  TBranch * branch_decayMode = 0;

  void
  setBranches(TTree * t,
              const std::string & s,
              ParticleType_Out particleType)
  {
    branch_pt     = t -> Branch(Form("%s_pt", s.c_str()),
                                &pt,   Form("%s_pt/D", s.c_str()));
    branch_eta    = t -> Branch(Form("%s_eta", s.c_str()),
                                &eta,  Form("%s_eta/D", s.c_str()));
    branch_phi    = t -> Branch(Form("%s_phi", s.c_str()),
                                &phi,  Form("%s_phi/D", s.c_str()));
    branch_mass   = t -> Branch(Form("%s_mass", s.c_str()),
                                &mass, Form("%s_mass/D", s.c_str()));
    if(particleType != ParticleType_Out::kJet)
      branch_charge = t -> Branch(Form("%s_charge", s.c_str()),
                                  &charge, Form("%s_charge/I", s.c_str()));
    if(particleType == ParticleType_Out::kHadronicTau)
      branch_decayMode = t -> Branch(Form("%s_decayMode", s.c_str()),
                                     &decayMode, Form("%s_decayMode/I", s.c_str()));
  }

  void
  setValues(const GenParticle * p)
  {
    pt = p -> pt_;
    eta = p -> eta_;
    phi = p -> phi_;
    mass = p -> mass_;
  }

  void
  setValues(const GenLepton * p)
  {
    setValues(static_cast<const GenParticle *>(p));
    charge = p -> charge_;
  }

  void
  setValues(const RecoHadTau * p)
  {
    setValues(static_cast<const GenParticle *>(p));
    decayMode = p -> decayMode_;
    charge = p -> charge_;
  }
};
typedef struct EventObject_Out EO_O;
 
/**
 * @brief Produce datacard and control plots for 3l_1tau categories.
 */
int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_SUCCESS;
  }

  std::cout << "<analyze_3l_1tau>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("analyze_3l_1tau");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("analyze_3l_1tau") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfg_analyze = cfg.getParameter<edm::ParameterSet>("analyze_3l_1tau");

  std::string treeName = cfg_analyze.getParameter<std::string>("treeName");

  std::string process_string = cfg_analyze.getParameter<std::string>("process");
  bool isSignal = ( process_string == "signal" ) ? true : false;

  std::string era_string = cfg_analyze.getParameter<std::string>("era");
  int era = -1;
  if      ( era_string == "2015" ) era = kEra_2015;
  else if ( era_string == "2016" ) era = kEra_2016;
  else throw cms::Exception("analyze_3l_1tau") 
    << "Invalid Configuration parameter 'era' = " << era_string << " !!\n";

  vstring triggerNames_1e = cfg_analyze.getParameter<vstring>("triggers_1e");
  std::vector<hltPath*> triggers_1e = create_hltPaths(triggerNames_1e);
  bool use_triggers_1e = cfg_analyze.getParameter<bool>("use_triggers_1e");
  vstring triggerNames_2e = cfg_analyze.getParameter<vstring>("triggers_2e");
  std::vector<hltPath*> triggers_2e = create_hltPaths(triggerNames_2e);
  bool use_triggers_2e = cfg_analyze.getParameter<bool>("use_triggers_2e");
  vstring triggerNames_1mu = cfg_analyze.getParameter<vstring>("triggers_1mu");
  std::vector<hltPath*> triggers_1mu = create_hltPaths(triggerNames_1mu);
  bool use_triggers_1mu = cfg_analyze.getParameter<bool>("use_triggers_1mu");
  vstring triggerNames_2mu = cfg_analyze.getParameter<vstring>("triggers_2mu");
  std::vector<hltPath*> triggers_2mu = create_hltPaths(triggerNames_2mu);
  bool use_triggers_2mu = cfg_analyze.getParameter<bool>("use_triggers_2mu");
  vstring triggerNames_1e1mu = cfg_analyze.getParameter<vstring>("triggers_1e1mu");
  std::vector<hltPath*> triggers_1e1mu = create_hltPaths(triggerNames_1e1mu);
  bool use_triggers_1e1mu = cfg_analyze.getParameter<bool>("use_triggers_1e1mu");

  bool apply_offline_e_trigger_cuts_1e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e");
  bool apply_offline_e_trigger_cuts_2e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2e");
  bool apply_offline_e_trigger_cuts_1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1mu");
  bool apply_offline_e_trigger_cuts_2mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2mu");
  bool apply_offline_e_trigger_cuts_1e1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e1mu");

  enum { kLoose, kFakeable, kTight };
  std::string leptonSelection_string = cfg_analyze.getParameter<std::string>("leptonSelection");
  int leptonSelection = -1;
  if      ( leptonSelection_string == "Loose"    ) leptonSelection = kLoose;
  else if ( leptonSelection_string == "Fakeable" ) leptonSelection = kFakeable;
  else if ( leptonSelection_string == "Tight"    ) leptonSelection = kTight;
  else throw cms::Exception("analyze_3l_1tau") 
    << "Invalid Configuration parameter 'leptonSelection' = " << leptonSelection_string << " !!\n";

  TString hadTauSelection_string = cfg_analyze.getParameter<std::string>("hadTauSelection").data();
  TObjArray* hadTauSelection_parts = hadTauSelection_string.Tokenize("|");
  assert(hadTauSelection_parts->GetEntries() >= 1);
  std::string hadTauSelection_part1 = (dynamic_cast<TObjString*>(hadTauSelection_parts->At(0)))->GetString().Data();
  int hadTauSelection = -1;
  if      ( hadTauSelection_part1 == "Loose"                                                     ) hadTauSelection = kLoose;
  else if ( hadTauSelection_part1 == "Fakeable" || hadTauSelection_part1 == "Fakeable_mcClosure" ) hadTauSelection = kFakeable;
  else if ( hadTauSelection_part1 == "Tight"                                                     ) hadTauSelection = kTight;
  else throw cms::Exception("analyze_3l_1tau") 
    << "Invalid Configuration parameter 'hadTauSelection' = " << hadTauSelection_string << " !!\n";
  std::string hadTauSelection_part2 = ( hadTauSelection_parts->GetEntries() == 2 ) ? (dynamic_cast<TObjString*>(hadTauSelection_parts->At(1)))->GetString().Data() : "";
  delete hadTauSelection_parts;
  
  std::string hadTauGenMatchSelection_string = cfg_analyze.getParameter<std::string>("hadTauGenMatch");
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions = getHadTauGenMatch_definitions_1tau();
  int hadTauGenMatchSelection = getHadTauGenMatch_int(hadTauGenMatch_definitions, hadTauGenMatchSelection_string);
  bool apply_hadTauGenMatching = cfg_analyze.getParameter<bool>("apply_hadTauGenMatching");
  if ( hadTauGenMatchSelection_string != "all" && !apply_hadTauGenMatching ) {
    throw cms::Exception("analyze_3l_1tau") 
      << "Invalid combination of Configuration parameters 'hadTauGenMatch' = " << hadTauGenMatchSelection_string << ", 'apply_hadTauGenMatching' = " << apply_hadTauGenMatching << " !!\n";
  }

  enum { kOS, kSS };
  std::string chargeSelection_string = cfg_analyze.getParameter<std::string>("chargeSelection");
  int chargeSelection = -1;
  if      ( chargeSelection_string == "OS" ) chargeSelection = kOS;
  else if ( chargeSelection_string == "SS" ) chargeSelection = kSS;
  else throw cms::Exception("analyze_3l_1tau") 
    << "Invalid Configuration parameter 'chargeSelection' = " << chargeSelection_string << " !!\n";

  std::vector<TFile*> inputFilesToClose;

  TH2* lutFakeRate_e = 0;
  TH2* lutFakeRate_mu = 0;
  if ( leptonSelection == kFakeable ) {
    edm::ParameterSet cfg_leptonFakeRate = cfg_analyze.getParameter<edm::ParameterSet>("leptonFakeRateLooseToTightWeight");
    std::string inputFileName = cfg_leptonFakeRate.getParameter<std::string>("inputFileName");
    std::string histogramName_e = cfg_leptonFakeRate.getParameter<std::string>("histogramName_e");
    std::string histogramName_mu = cfg_leptonFakeRate.getParameter<std::string>("histogramName_mu");
    TFile* inputFile = openFile(edm::FileInPath(inputFileName));
    lutFakeRate_e = loadTH2(inputFile, histogramName_e);
    lutFakeRate_mu = loadTH2(inputFile, histogramName_mu);
    inputFilesToClose.push_back(inputFile);
  }

  bool isMC = cfg_analyze.getParameter<bool>("isMC"); 
  std::string central_or_shift = cfg_analyze.getParameter<std::string>("central_or_shift");
  double lumiScale = ( process_string != "data_obs" ) ? cfg_analyze.getParameter<double>("lumiScale") : 1.;

  std::string jet_btagWeight_branch = ( isMC ) ? "Jet_bTagWeight" : "";

  int jetPt_option = RecoJetReader::kJetPt_central;
  int hadTauPt_option = RecoHadTauReader::kHadTauPt_central;
  int jetToTauFakeRate_option = kFRjt_central;
  int eToTauFakeRate_option = kFRet_central;
  int muToTauFakeRate_option = kFRmt_central;
  if ( isMC && central_or_shift != "central" ) {
    TString central_or_shift_tstring = central_or_shift.data();
    std::string shiftUp_or_Down = "";
    if      ( central_or_shift_tstring.EndsWith("Up")   ) shiftUp_or_Down = "Up";
    else if ( central_or_shift_tstring.EndsWith("Down") ) shiftUp_or_Down = "Down";
    else throw cms::Exception("analyze_3l_1tau")
      << "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
    if      ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HF")       ) jet_btagWeight_branch = "Jet_bTagWeightHF" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HFStats1") ) jet_btagWeight_branch = "Jet_bTagWeightHFStats1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HFStats2") ) jet_btagWeight_branch = "Jet_bTagWeightHFStats2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LF")       ) jet_btagWeight_branch = "Jet_bTagWeightLF" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LFStats1") ) jet_btagWeight_branch = "Jet_bTagWeightLFStats1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LFStats2") ) jet_btagWeight_branch = "Jet_bTagWeightLFStats2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_cErr1")    ) jet_btagWeight_branch = "Jet_bTagWeightcErr1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_cErr2")    ) jet_btagWeight_branch = "Jet_bTagWeightcErr2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_JES") ) {
      jet_btagWeight_branch = "Jet_bTagWeightJES" + shiftUp_or_Down;
      if      ( shiftUp_or_Down == "Up"   ) jetPt_option = RecoJetReader::kJetPt_jecUp;
      else if ( shiftUp_or_Down == "Down" ) jetPt_option = RecoJetReader::kJetPt_jecDown;
      else assert(0);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_tauES") ) {
      if      ( shiftUp_or_Down == "Up"   ) hadTauPt_option = RecoHadTauReader::kHadTauPt_shiftUp;
      else if ( shiftUp_or_Down == "Down" ) hadTauPt_option = RecoHadTauReader::kHadTauPt_shiftDown;
      else assert(0);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_FRjt") ) {
      if      ( central_or_shift_tstring.EndsWith("normUp")    ) jetToTauFakeRate_option = kFRjt_normUp;
      else if ( central_or_shift_tstring.EndsWith("normDown")  ) jetToTauFakeRate_option = kFRjt_normDown;
      else if ( central_or_shift_tstring.EndsWith("shapeUp")   ) jetToTauFakeRate_option = kFRjt_shapeUp;
      else if ( central_or_shift_tstring.EndsWith("shapeDown") ) jetToTauFakeRate_option = kFRjt_shapeDown;
      else assert(0);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_FRet") ) {
      if      ( shiftUp_or_Down == "Up"   ) eToTauFakeRate_option = kFRet_shiftUp;
      else if ( shiftUp_or_Down == "Down" ) eToTauFakeRate_option = kFRet_shiftDown;
      else assert(0);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_FRmt") ) {
      if      ( shiftUp_or_Down == "Up"   ) muToTauFakeRate_option = kFRmt_shiftUp;
      else if ( shiftUp_or_Down == "Down" ) muToTauFakeRate_option = kFRmt_shiftDown;
      else assert(0);
    } else throw cms::Exception("analyze_3l_1tau")
	<< "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
  }

  JetToTauFakeRateInterface* jetToTauFakeRateInterface = 0;
  bool applyJetToTauFakeRateWeight = cfg_analyze.getParameter<bool>("applyJetToTauFakeRateWeight");
  if ( applyJetToTauFakeRateWeight ) {
    edm::ParameterSet cfg_jetToTauFakeRateWeight = cfg_analyze.getParameter<edm::ParameterSet>("jetToTauFakeRateWeight");
    cfg_jetToTauFakeRateWeight.addParameter<std::string>("hadTauSelection", hadTauSelection_part2);
    jetToTauFakeRateInterface = new JetToTauFakeRateInterface(cfg_jetToTauFakeRateWeight, jetToTauFakeRate_option);
  }
  
  std::string selEventsFileName_input = cfg_analyze.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;
  if ( selEventsFileName_input != "" ) {
    edm::ParameterSet cfgRunLumiEventSelector;
    cfgRunLumiEventSelector.addParameter<std::string>("inputFileName", selEventsFileName_input);
    cfgRunLumiEventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfgRunLumiEventSelector);
  }

  bool fillGenEvtHistograms = cfg_analyze.getParameter<bool>("fillGenEvtHistograms");
  
  std::string selEventsFileName_output = cfg_analyze.getParameter<std::string>("selEventsFileName_output");

  std::string selEventsTFileName = cfg_analyze.getParameter<std::string>("selEventsTFileName");
  const bool writeSelEventsFile = selEventsTFileName != "";

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << std::endl;
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TChain* inputTree = new TChain(treeName.data());
  for ( std::vector<std::string>::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    std::cout << "input Tree: adding file = " << (*inputFileName) << std::endl;
    inputTree->AddFile(inputFileName->data());
  }
  
  if ( !(inputTree->GetListOfFiles()->GetEntries() >= 1) ) {
    throw cms::Exception("analyze_3l_1tau") 
      << "Failed to identify input Tree !!\n";
  }
  
  // CV: need to call TChain::LoadTree before processing first event 
  //     in order to prevent ROOT causing a segmentation violation,
  //     cf. http://root.cern.ch/phpBB3/viewtopic.php?t=10062
  inputTree->LoadTree(0);

//--- create output root file from selected events if needed
  TFile * selEventsTFile = 0;
  TTree * selEventsTTree = 0;
  EventSpecifics_Out eventSpecificsOut;
  std::array<EventObject_Out, 3> leptonsOut {{  EO_O(), EO_O(), EO_O() }};
  std::array<EventObject_Out, 2> jetsOut {{ EO_O(), EO_O() }};
  EventObject_Out htauOut;
  EventMVAIO_Out MVAOut;

  if(writeSelEventsFile)
  {
    selEventsTFile = new TFile(selEventsTFileName.c_str(), "recreate");
    selEventsTTree = new TTree("tree", "Selected events created for MEM");
    eventSpecificsOut.setBranches(selEventsTTree);
    for(std::size_t i = 0; i < leptonsOut.size(); ++i)
      leptonsOut[i].setBranches(selEventsTTree,
                                std::string(TString::Format("lepton%lu", i + 1)),
                                EventObject_Out::ParticleType_Out::kLepton);
    for(std::size_t i = 0; i < jetsOut.size(); ++i)
      jetsOut[i].setBranches(selEventsTTree,
                             std::string(TString::Format("jet%lu", i + 1)),
                             EventObject_Out::ParticleType_Out::kJet);
    htauOut.setBranches(selEventsTTree, "htau1", EventObject_Out::ParticleType_Out::kHadronicTau);
    MVAOut.setBranches(selEventsTTree);
  }

  std::cout << "input Tree contains " << inputTree->GetEntries()
            << " Entries in " << inputTree->GetListOfFiles()->GetEntries()
            << " files." << std::endl;

//--- declare event-level variables
  RUN_TYPE run;
  inputTree->SetBranchAddress(RUN_KEY, &run);
  LUMI_TYPE lumi;
  inputTree->SetBranchAddress(LUMI_KEY, &lumi);
  EVT_TYPE event;
  inputTree->SetBranchAddress(EVT_KEY, &event);
  GENHIGGSDECAYMODE_TYPE genHiggsDecayMode;
  if ( isSignal ) {
    inputTree->SetBranchAddress(GENHIGGSDECAYMODE_KEY, &genHiggsDecayMode);
  }

  hltPaths_setBranchAddresses(inputTree, triggers_1e);
  hltPaths_setBranchAddresses(inputTree, triggers_2e);
  hltPaths_setBranchAddresses(inputTree, triggers_1mu);
  hltPaths_setBranchAddresses(inputTree, triggers_2mu);
  hltPaths_setBranchAddresses(inputTree, triggers_1e1mu);

  PUWEIGHT_TYPE pileupWeight;
  if ( isMC ) {
    inputTree->SetBranchAddress(PUWEIGHT_KEY, &pileupWeight);
  }

  MET_PT_TYPE met_pt;
  inputTree->SetBranchAddress(MET_PT_KEY, &met_pt);
  MET_ETA_TYPE met_eta;
  inputTree->SetBranchAddress(MET_ETA_KEY, &met_eta);
  MET_PHI_TYPE met_phi;
  inputTree->SetBranchAddress(MET_PHI_KEY, &met_phi);

//--- declare particle collections
  RecoMuonReader* muonReader = new RecoMuonReader(era, "nselLeptons", "selLeptons");
  muonReader->setBranchAddresses(inputTree);
  RecoMuonCollectionGenMatcher muonGenMatcher;
  RecoMuonCollectionSelectorLoose preselMuonSelector;
  RecoMuonCollectionSelectorFakeable fakeableMuonSelector(era);
  RecoMuonCollectionSelectorTight tightMuonSelector(era, -1, run_lumi_eventSelector != 0);
  tightMuonSelector.disable_tightCharge_cut();

  RecoElectronReader* electronReader = new RecoElectronReader("nselLeptons", "selLeptons");
  electronReader->setBranchAddresses(inputTree);
  RecoElectronCollectionGenMatcher electronGenMatcher;
  RecoElectronCollectionCleaner electronCleaner(0.3);
  RecoElectronCollectionSelectorLoose preselElectronSelector;
  RecoElectronCollectionSelectorFakeable fakeableElectronSelector(era);
  RecoElectronCollectionSelectorTight tightElectronSelector(era, -1, run_lumi_eventSelector != 0);
  tightElectronSelector.disable_tightCharge_cut();

  RecoHadTauReader* hadTauReader = new RecoHadTauReader("nTauGood", "TauGood");
  hadTauReader->setHadTauPt_central_or_shift(hadTauPt_option);
  hadTauReader->setBranchAddresses(inputTree);
  RecoHadTauCollectionGenMatcher hadTauGenMatcher;
  RecoHadTauCollectionCleaner hadTauCleaner(0.3);
  RecoHadTauCollectionSelectorLoose preselHadTauSelector;
  preselHadTauSelector.set_min_antiElectron(hadTauSelection_antiElectron);
  preselHadTauSelector.set_min_antiMuon(hadTauSelection_antiMuon);
  RecoHadTauCollectionSelectorFakeable fakeableHadTauSelector;
  fakeableHadTauSelector.set_min_antiElectron(hadTauSelection_antiElectron);
  fakeableHadTauSelector.set_min_antiMuon(hadTauSelection_antiMuon);
  RecoHadTauCollectionSelectorTight tightHadTauSelector;
  if ( hadTauSelection_part2 != "" ) tightHadTauSelector.set(hadTauSelection_part2);
  tightHadTauSelector.set_min_antiElectron(hadTauSelection_antiElectron);
  tightHadTauSelector.set_min_antiMuon(hadTauSelection_antiMuon);
    
  RecoJetReader* jetReader = new RecoJetReader("nJet", "Jet");
  jetReader->setJetPt_central_or_shift(jetPt_option);
  jetReader->setBranchName_BtagWeight(jet_btagWeight_branch);
  jetReader->setBranchAddresses(inputTree);
  RecoJetCollectionGenMatcher jetGenMatcher;
  RecoJetCollectionCleaner jetCleaner(0.5);
  RecoJetCollectionSelector jetSelector;  
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose(era);
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium(era);

  GenLeptonReader* genLeptonReader = 0;
  GenHadTauReader* genHadTauReader = 0;
  GenJetReader* genJetReader = 0;
  if ( isMC ) {
    genLeptonReader = new GenLeptonReader("nGenLep", "GenLep");
    genLeptonReader->setBranchAddresses(inputTree);
    genHadTauReader = new GenHadTauReader("nGenHadTaus", "GenHadTaus");
    genHadTauReader->setBranchAddresses(inputTree);
    genJetReader = new GenJetReader("nGenJet", "GenJet");
    genJetReader->setBranchAddresses(inputTree);
  }

//--- initialize BDTs used to discriminate ttH vs. ttV and ttH vs. ttbar 
//    in 3l category of ttH multilepton analysis
  std::string mvaFileName_3l_ttV = "tthAnalysis/HiggsToTauTau/data/3l_ttV_BDTG.weights.xml";
  std::vector<std::string> mvaInputVariables_3l_ttV;
  mvaInputVariables_3l_ttV.push_back("max(abs(LepGood_eta[iF_Recl[0]]),abs(LepGood_eta[iF_Recl[1]]))");
  mvaInputVariables_3l_ttV.push_back("MT_met_lep1");
  mvaInputVariables_3l_ttV.push_back("nJet25_Recl");
  mvaInputVariables_3l_ttV.push_back("mindr_lep1_jet");
  mvaInputVariables_3l_ttV.push_back("mindr_lep2_jet");
  mvaInputVariables_3l_ttV.push_back("LepGood_conePt[iF_Recl[0]]");
  mvaInputVariables_3l_ttV.push_back("LepGood_conePt[iF_Recl[2]]");
  TMVAInterface mva_3l_ttV(mvaFileName_3l_ttV, mvaInputVariables_3l_ttV, { "iF_Recl[0]", "iF_Recl[1]", "iF_Recl[2]" });

  std::string mvaFileName_3l_ttbar = "tthAnalysis/HiggsToTauTau/data/3l_ttbar_BDTG.weights.xml";
  std::vector<std::string> mvaInputVariables_3l_ttbar;
  mvaInputVariables_3l_ttbar.push_back("max(abs(LepGood_eta[iF_Recl[0]]),abs(LepGood_eta[iF_Recl[1]]))");
  mvaInputVariables_3l_ttbar.push_back("MT_met_lep1");
  mvaInputVariables_3l_ttbar.push_back("nJet25_Recl");
  mvaInputVariables_3l_ttbar.push_back("mhtJet25_Recl");
  mvaInputVariables_3l_ttbar.push_back("avg_dr_jet");
  mvaInputVariables_3l_ttbar.push_back("mindr_lep1_jet");
  mvaInputVariables_3l_ttbar.push_back("mindr_lep2_jet");
  TMVAInterface mva_3l_ttbar(mvaFileName_3l_ttbar, mvaInputVariables_3l_ttbar, { "iF_Recl[0]", "iF_Recl[1]", "iF_Recl[2]" });

  std::map<std::string, double> mvaInputs;

//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = new std::ofstream(selEventsFileName_output.data(), std::ios::out);

//--- declare histograms
  std::string charge_and_lepton_and_hadTauSelection = Form("%s_lep%s_tau%s", chargeSelection_string.data(), leptonSelection_string.data(), hadTauSelection_part1.data());  
  ElectronHistManager preselElectronHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/electrons", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselElectronHistManager.bookHistograms(fs);
  MuonHistManager preselMuonHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/muons", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselMuonHistManager.bookHistograms(fs);
  HadTauHistManager preselHadTauHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/hadTaus", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselHadTauHistManager.bookHistograms(fs);
  JetHistManager preselJetHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/jets", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselJetHistManager.bookHistograms(fs);
  JetHistManager preselBJet_looseHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/BJets_loose", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselBJet_looseHistManager.bookHistograms(fs);
  JetHistManager preselBJet_mediumHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/BJets_medium", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselBJet_mediumHistManager.bookHistograms(fs);
  MEtHistManager preselMEtHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/met", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselMEtHistManager.bookHistograms(fs);
  EvtHistManager_3l_1tau preselEvtHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/presel/evt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  preselEvtHistManager.bookHistograms(fs);

  ElectronHistManager selElectronHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/electrons", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selElectronHistManager.bookHistograms(fs);

  MuonHistManager selMuonHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/muons", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selMuonHistManager.bookHistograms(fs);

  HadTauHistManager selHadTauHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/hadTaus", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selHadTauHistManager.bookHistograms(fs);

  JetHistManager selJetHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/jets", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selJetHistManager.bookHistograms(fs);
  JetHistManager selJetHistManager_lead(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/leadJet", charge_and_lepton_and_hadTauSelection.data()), central_or_shift, 0));
  selJetHistManager_lead.bookHistograms(fs);
  JetHistManager selJetHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/subleadJet", charge_and_lepton_and_hadTauSelection.data()), central_or_shift, 1));
  selJetHistManager_sublead.bookHistograms(fs);

  JetHistManager selBJet_looseHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/BJets_loose", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selBJet_looseHistManager.bookHistograms(fs);
  JetHistManager selBJet_looseHistManager_lead(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/leadBJet_loose", charge_and_lepton_and_hadTauSelection.data()), central_or_shift, 0));
  selBJet_looseHistManager_lead.bookHistograms(fs);
  JetHistManager selBJet_looseHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/subleadBJet_loose", charge_and_lepton_and_hadTauSelection.data()), central_or_shift, 1));
  selBJet_looseHistManager_sublead.bookHistograms(fs);
  JetHistManager selBJet_mediumHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/BJets_medium", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selBJet_mediumHistManager.bookHistograms(fs);

  MEtHistManager selMEtHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/met", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selMEtHistManager.bookHistograms(fs);

  EvtHistManager_3l_1tau selEvtHistManager(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/evt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  selEvtHistManager.bookHistograms(fs);
  std::map<std::string, EvtHistManager_3l_1tau*> selEvtHistManager_decayMode; // key = decay mode
  const std::map<std::string, GENHIGGSDECAYMODE_TYPE> decayMode_idString = {
    { "ttH_hww", static_cast<GENHIGGSDECAYMODE_TYPE>(24) },
    { "ttH_hzz", static_cast<GENHIGGSDECAYMODE_TYPE>(23) },
    { "ttH_htt", static_cast<GENHIGGSDECAYMODE_TYPE>(15) }
  };
  vstring decayModes_evt;
  decayModes_evt.reserve(decayMode_idString.size());
  boost::copy(decayMode_idString | boost::adaptors::map_keys, std::back_inserter(decayModes_evt));
  if ( isSignal ) {
    for ( vstring::const_iterator decayMode = decayModes_evt.begin();
          decayMode != decayModes_evt.end(); ++decayMode) {
      EvtHistManager_3l_1tau* selEvtHistManager_ptr = new EvtHistManager_3l_1tau(makeHistManager_cfg(decayMode->data(),
        Form("3l_1tau_%s/sel/evt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
      selEvtHistManager_ptr->bookHistograms(fs);
      selEvtHistManager_decayMode[*decayMode] = selEvtHistManager_ptr;
    }
  }
  std::map<int, EvtHistManager_3l_1tau*> selEvtHistManagers_genMatch; // key = { kGen_1t0e0m0j, kGen_0t1e0m0j, kGen_0t_0e1m0j, kGen_0t_0e0m1j }
  if ( isMC && !apply_hadTauGenMatching ) {
    for ( std::vector<hadTauGenMatchEntry>::const_iterator hadTauGenMatch_definition = hadTauGenMatch_definitions.begin();
	hadTauGenMatch_definition != hadTauGenMatch_definitions.end(); ++hadTauGenMatch_definition ) {
      if ( hadTauGenMatch_definition->idx_ == kGen_All1 || hadTauGenMatch_definition->idx_ == kGen_Undefined1 ) continue;
      std::string process_and_genMatch = process_string + hadTauGenMatch_definition->name_;
      EvtHistManager_3l_1tau* selEvtHistManager_ptr = new EvtHistManager_3l_1tau(makeHistManager_cfg(process_and_genMatch, 
        Form("3l_1tau_%s/sel/evt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
      selEvtHistManager_ptr->bookHistograms(fs);
      selEvtHistManagers_genMatch[hadTauGenMatch_definition->idx_] = selEvtHistManager_ptr;
    }
  }

  GenEvtHistManager genEvtHistManager_beforeCuts(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/unbiased/genEvt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  genEvtHistManager_beforeCuts.bookHistograms(fs);
  GenEvtHistManager genEvtHistManager_afterCuts(makeHistManager_cfg(process_string, 
    Form("3l_1tau_%s/sel/genEvt", charge_and_lepton_and_hadTauSelection.data()), central_or_shift));
  genEvtHistManager_afterCuts.bookHistograms(fs);
  
  int numEntries = inputTree->GetEntries();
  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
   cutFlowTableType cutFlowTable;
  for ( int idxEntry = 0; idxEntry < numEntries && (maxEvents == -1 || idxEntry < maxEvents); ++idxEntry ) {
    if ( idxEntry > 0 && (idxEntry % reportEvery) == 0 ) {
      std::cout << "processing Entry " << idxEntry << " (" << selectedEntries << " Entries selected)" << std::endl;
    }
    ++analyzedEntries;
    
    inputTree->GetEntry(idxEntry);

    if ( run_lumi_eventSelector && !(*run_lumi_eventSelector)(run, lumi, event) ) continue;
    cutFlowTable.update("run:ls:event selection");

//--- build collections of generator level particles (before any cuts are applied, to check distributions in unbiased event samples)
    std::vector<GenLepton> genLeptons;
    std::vector<GenLepton> genElectrons;
    std::vector<GenLepton> genMuons;
    std::vector<GenHadTau> genHadTaus;
    std::vector<GenJet> genJets;
    if ( isMC && fillGenEvtHistograms ) {
      genLeptons = genLeptonReader->read();
      for ( std::vector<GenLepton>::const_iterator genLepton = genLeptons.begin();
    	    genLepton != genLeptons.end(); ++genLepton ) {
	int abs_pdgId = std::abs(genLepton->pdgId_);
	if      ( abs_pdgId == 11 ) genElectrons.push_back(*genLepton);
	else if ( abs_pdgId == 13 ) genMuons.push_back(*genLepton);
      }
      genHadTaus = genHadTauReader->read();
      genJets = genJetReader->read();
    }

    if ( isMC ) {
      genEvtHistManager_beforeCuts.fillHistograms(genElectrons, genMuons, genHadTaus, genJets);
    }
    
    bool isTriggered_1e = hltPaths_isTriggered(triggers_1e);
    bool isTriggered_2e = hltPaths_isTriggered(triggers_2e);
    bool isTriggered_1mu = hltPaths_isTriggered(triggers_1mu);
    bool isTriggered_2mu = hltPaths_isTriggered(triggers_2mu);
    bool isTriggered_1e1mu = hltPaths_isTriggered(triggers_1e1mu);

    bool selTrigger_1e = use_triggers_1e && isTriggered_1e;
    bool selTrigger_2e = use_triggers_2e && isTriggered_2e;
    bool selTrigger_1mu = use_triggers_1mu && isTriggered_1mu;
    bool selTrigger_2mu = use_triggers_2mu && isTriggered_2mu;
    bool selTrigger_1e1mu = use_triggers_1e1mu && isTriggered_1e1mu;
    if ( !(selTrigger_1e || selTrigger_2e || selTrigger_1mu || selTrigger_2mu || selTrigger_1e1mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection." << std::endl; 
	std::cout << " (selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu 
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    }
    
//--- rank triggers by priority and ignore triggers of lower priority if a trigger of higher priority has fired for given event;
//    the ranking of the triggers is as follows: 2mu, 1e1mu, 2e, 1mu, 1e
// CV: this logic is necessary to avoid that the same event is selected multiple times when processing different primary datasets
    if ( !isMC ) {
      if ( selTrigger_1e && (isTriggered_2e || isTriggered_1mu || isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1e = " << selTrigger_1e 
		    << ", isTriggered_2e = " << isTriggered_2e 
		    << ", isTriggered_1mu = " << isTriggered_1mu 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_2e && (isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_2e = " << selTrigger_2e 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_1mu && (isTriggered_2e || isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1mu = " << selTrigger_1mu 
		    << ", isTriggered_2e = " << isTriggered_2e 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_1e1mu && isTriggered_2mu ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1e1mu = " << selTrigger_1e1mu 
		    << ", isTriggered_2mu = " << isTriggered_2mu << ")" << std::endl;
	}
	continue; 
      }
    }
    cutFlowTable.update("trigger");

    if ( (selTrigger_2mu   && !apply_offline_e_trigger_cuts_2mu)   ||
	 (selTrigger_1mu   && !apply_offline_e_trigger_cuts_1mu)   ||
	 (selTrigger_2e    && !apply_offline_e_trigger_cuts_2e)    ||
	 (selTrigger_1e1mu && !apply_offline_e_trigger_cuts_1e1mu) ||
	 (selTrigger_1e    && !apply_offline_e_trigger_cuts_1e)    ) {
      fakeableElectronSelector.disable_offline_e_trigger_cuts();
      tightElectronSelector.disable_offline_e_trigger_cuts();
    } else {
      fakeableElectronSelector.enable_offline_e_trigger_cuts();
      tightElectronSelector.enable_offline_e_trigger_cuts();
    }

//--- build collections of electrons, muons and hadronic taus;
//    resolve overlaps in order of priority: muon, electron,
    std::vector<RecoMuon> muons = muonReader->read();
    std::vector<const RecoMuon*> muon_ptrs = convert_to_ptrs(muons);
    std::vector<const RecoMuon*> cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
    std::vector<const RecoMuon*> preselMuons = preselMuonSelector(cleanedMuons);
    std::vector<const RecoMuon*> fakeableMuons = fakeableMuonSelector(preselMuons);
    std::vector<const RecoMuon*> tightMuons = tightMuonSelector(preselMuons);
    std::vector<const RecoMuon*> selMuons;
    if      ( leptonSelection == kLoose    ) selMuons = preselMuons;
    else if ( leptonSelection == kFakeable ) selMuons = fakeableMuons;
    else if ( leptonSelection == kTight    ) selMuons = tightMuons;
    else assert(0);
    //for ( size_t idxPreselMuon = 0; idxPreselMuon < preselMuons.size(); ++idxPreselMuon ) {
    //  std::cout << "preselMuon #" << idxPreselMuon << ":" << std::endl;
    //  std::cout << (*preselMuons[idxPreselMuon]);
    //}
    
    std::vector<RecoElectron> electrons = electronReader->read();
    std::vector<const RecoElectron*> electron_ptrs = convert_to_ptrs(electrons);
    std::vector<const RecoElectron*> cleanedElectrons = electronCleaner(electron_ptrs, selMuons);
    std::vector<const RecoElectron*> preselElectrons = preselElectronSelector(cleanedElectrons);
    std::vector<const RecoElectron*> fakeableElectrons = fakeableElectronSelector(preselElectrons);
    std::vector<const RecoElectron*> tightElectrons = tightElectronSelector(preselElectrons);
    std::vector<const RecoElectron*> selElectrons;
    if      ( leptonSelection == kLoose    ) selElectrons = preselElectrons;
    else if ( leptonSelection == kFakeable ) selElectrons = fakeableElectrons;
    else if ( leptonSelection == kTight    ) selElectrons = tightElectrons;
    else assert(0);
    //for ( size_t idxPreselElectron = 0; idxPreselElectron < preselElectrons.size(); ++idxPreselElectron ) {
    //  std::cout << "preselElectron #" << idxPreselElectron << ":" << std::endl;
    //  std::cout << (*preselElectrons[idxPreselElectron]);
    //}

    std::vector<RecoHadTau> hadTaus = hadTauReader->read();
    std::vector<const RecoHadTau*> hadTau_ptrs = convert_to_ptrs(hadTaus);
    std::vector<const RecoHadTau*> cleanedHadTaus = hadTauCleaner(hadTau_ptrs, selMuons, selElectrons);
    std::vector<const RecoHadTau*> preselHadTaus = preselHadTauSelector(hadTau_ptrs);
    std::vector<const RecoHadTau*> fakeableHadTaus = fakeableHadTauSelector(cleanedHadTaus);
    std::vector<const RecoHadTau*> tightHadTaus = tightHadTauSelector(cleanedHadTaus);
    std::vector<const RecoHadTau*> selHadTaus;
    if      ( hadTauSelection == kLoose    ) selHadTaus = preselHadTaus;
    else if ( hadTauSelection == kFakeable ) selHadTaus = fakeableHadTaus;
    else if ( hadTauSelection == kTight    ) selHadTaus = tightHadTaus;
    else assert(0);
    
//--- build collections of jets and select subset of jets passing b-tagging criteria
    std::vector<RecoJet> jets = jetReader->read();
    std::vector<const RecoJet*> jet_ptrs = convert_to_ptrs(jets);
    std::vector<const RecoJet*> cleanedJets = jetCleaner(jet_ptrs, selMuons, selElectrons, selHadTaus);
    std::vector<const RecoJet*> selJets = jetSelector(cleanedJets);
    std::vector<const RecoJet*> selBJets_loose = jetSelectorBtagLoose(cleanedJets);
    std::vector<const RecoJet*> selBJets_medium = jetSelectorBtagMedium(cleanedJets);

//--- build collections of generator level particles (after some cuts are applied, to safe computing time)
    if ( isMC && !fillGenEvtHistograms ) {
      genLeptons = genLeptonReader->read();
      for ( std::vector<GenLepton>::const_iterator genLepton = genLeptons.begin();
    	    genLepton != genLeptons.end(); ++genLepton ) {
    	int abs_pdgId = std::abs(genLepton->pdgId_);
    	if      ( abs_pdgId == 11 ) genElectrons.push_back(*genLepton);
    	else if ( abs_pdgId == 13 ) genMuons.push_back(*genLepton);
      }
      genHadTaus = genHadTauReader->read();
      genJets = genJetReader->read();
    }

//--- match reconstructed to generator level particles
    if ( isMC ) {
      muonGenMatcher.addGenLeptonMatch(preselMuons, genLeptons, 0.3);
      muonGenMatcher.addGenHadTauMatch(preselMuons, genHadTaus, 0.3);
      muonGenMatcher.addGenJetMatch(preselMuons, genJets, 0.5);

      electronGenMatcher.addGenLeptonMatch(preselElectrons, genLeptons, 0.3);
      electronGenMatcher.addGenHadTauMatch(preselElectrons, genHadTaus, 0.3);
      electronGenMatcher.addGenJetMatch(preselElectrons, genJets, 0.5);

      hadTauGenMatcher.addGenLeptonMatch(selHadTaus, genLeptons, 0.3);
      hadTauGenMatcher.addGenHadTauMatch(selHadTaus, genHadTaus, 0.3);
      hadTauGenMatcher.addGenJetMatch(selHadTaus, genJets, 0.5);

      jetGenMatcher.addGenLeptonMatch(selJets, genLeptons, 0.3);
      jetGenMatcher.addGenHadTauMatch(selJets, genHadTaus, 0.3);
      jetGenMatcher.addGenJetMatch(selJets, genJets, 0.5);
    }

//--- apply preselection
    std::vector<const RecoLepton*> preselLeptons;    
    preselLeptons.reserve(preselElectrons.size() + preselMuons.size());
    preselLeptons.insert(preselLeptons.end(), preselElectrons.begin(), preselElectrons.end());
    preselLeptons.insert(preselLeptons.end(), preselMuons.begin(), preselMuons.end());
    std::sort(preselLeptons.begin(), preselLeptons.end(), isHigherPt);
    // require exactly two leptons passing loose preselection criteria to avoid overlap with 3l category
    if ( !(preselLeptons.size() == 3) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS preselLeptons selection." << std::endl;
	std::cout << " (#preselLeptons = " << preselLeptons.size() << ")" << std::endl;
	for ( size_t idxPreselLepton = 0; idxPreselLepton < preselLeptons.size(); ++idxPreselLepton ) {
	  std::cout << "preselLepton #" << idxPreselLepton << ":" << std::endl;
	  std::cout << (*preselLeptons[idxPreselLepton]);
	}
      }
      continue;
    }
    cutFlowTable.update("3 presel leptons");
    const RecoLepton* preselLepton_lead = preselLeptons[0];
    int preselLepton_lead_type = getLeptonType(preselLepton_lead->pdgId_);
    const RecoLepton* preselLepton_sublead = preselLeptons[1];
    int preselLepton_sublead_type = getLeptonType(preselLepton_sublead->pdgId_);
    const RecoLepton* preselLepton_third = preselLeptons[2];
    int preselLepton_third_type = getLeptonType(preselLepton_third->pdgId_);

    // require that trigger paths match event category (with event category based on preselLeptons);
    if ( (preselElectrons.size() == 3 &&                            !(selTrigger_1e || selTrigger_2e                                                        ) ) ||
	 (preselElectrons.size() == 2 && preselMuons.size() == 1 && !(selTrigger_1e || selTrigger_2e || selTrigger_1mu ||                   selTrigger_1e1mu) ) ||
	 (preselElectrons.size() == 1 && preselMuons.size() == 2 && !(selTrigger_1e ||                  selTrigger_1mu || selTrigger_2mu || selTrigger_1e1mu) ) ||
	 (                               preselMuons.size() == 3 && !(                                  selTrigger_1mu || selTrigger_2mu                    ) ) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given preselLepton multiplicity." << std::endl; 
	std::cout << " (#preselElectrons = " << preselElectrons.size() 
		  << ", #preselMuons = " << preselMuons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu 
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    } 
    cutFlowTable.update("presel lepton trigger match");

    // apply requirement on jets (incl. b-tagged jets) and hadronic taus on preselection level
    if ( !(selJets.size() >= 2) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selJets selection." << std::endl;
	std::cout << " (#selJets = " << selJets.size() << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(">= 2 jets (1)");
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selBJets selection." << std::endl;
	std::cout << " (#selBJets_loose = " << selBJets_loose.size() << ", #selBJets_medium = " << selBJets_medium.size() << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (1)");
    if ( !(selHadTaus.size() >= 1) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selHadTaus selection." << std::endl;
	std::cout << " (#selHadTaus = " << selHadTaus.size() << ")" << std::endl;
	for ( size_t idxSelHadTau = 0; idxSelHadTau < selHadTaus.size(); ++idxSelHadTau ) {
	  std::cout << "selHadTau #" << idxSelHadTau << ":" << std::endl;
	  std::cout << (*selHadTaus[idxSelHadTau]);
	}
      }
      continue;
    }
    cutFlowTable.update(">= 1 sel tau (1)");
    const RecoHadTau* selHadTau = selHadTaus[0];

//--- compute MHT and linear MET discriminant (met_LD)
    LV met_p4(met_pt, met_eta, met_phi, 0.);
    LV mht_p4(0,0,0,0);
    for ( std::vector<const RecoJet*>::const_iterator jet = selJets.begin();
	  jet != selJets.end(); ++jet ) {
      mht_p4 += (*jet)->p4_;
    }
    for ( std::vector<const RecoLepton*>::const_iterator lepton = preselLeptons.begin();
	  lepton != preselLeptons.end(); ++lepton ) {
      mht_p4 += (*lepton)->p4_;
    }
    for ( std::vector<const RecoHadTau*>::const_iterator hadTau = selHadTaus.begin();
	  hadTau != selHadTaus.end(); ++hadTau ) {
      mht_p4 += (*hadTau)->p4_;
    }
    double met_LD = met_coef*met_p4.pt() + mht_coef*mht_p4.pt();    

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method", 
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
    double evtWeight = 1.;
    if ( isMC ) {
      evtWeight *= lumiScale;
      evtWeight *= pileupWeight;
      for ( std::vector<const RecoJet*>::const_iterator jet = selJets.begin();
	    jet != selJets.end(); ++jet ) {
	evtWeight *= (*jet)->BtagWeight_;
      }
    }    

//--- apply data/MC corrections for trigger efficiency,
//    and efficiencies for lepton to pass loose identification and isolation criteria
    if ( isMC ) {
      evtWeight *= sf_triggerEff(3,
        preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_,
        preselLepton_third_type, preselLepton_third->pt_, preselLepton_third->eta_);
      evtWeight *= sf_leptonID_and_Iso_loose(3,
        preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_,
        preselLepton_third_type, preselLepton_third->pt_, preselLepton_third->eta_);
    }

//--- compute output of BDTs used to discriminate ttH vs. ttV and ttH vs. ttbar 
//    in 3l category of ttH multilepton analysis 
    mvaInputs["max(abs(LepGood_eta[iF_Recl[0]]),abs(LepGood_eta[iF_Recl[1]]))"] = std::max(std::fabs(preselLepton_lead->eta_), std::fabs(preselLepton_sublead->eta_));
    mvaInputs["MT_met_lep1"]                = comp_MT_met_lep1(*preselLepton_lead, met_pt, met_phi);
    mvaInputs["nJet25_Recl"]                = comp_n_jet25_recl(selJets);
    mvaInputs["mindr_lep1_jet"]             = comp_mindr_lep1_jet(*preselLepton_lead, selJets);
    mvaInputs["mindr_lep2_jet"]             = comp_mindr_lep2_jet(*preselLepton_sublead, selJets);
    mvaInputs["LepGood_conePt[iF_Recl[0]]"] = comp_lep1_conePt(*preselLepton_lead);
    mvaInputs["LepGood_conePt[iF_Recl[2]]"] = comp_lep3_conePt(*preselLepton_third);
    mvaInputs["avg_dr_jet"]                 = comp_avg_dr_jet(selJets);
    mvaInputs["mhtJet25_Recl"]              = mht_p4.pt();

    int index = 1;
    for ( std::map<std::string, double>::const_iterator mvaInput = mvaInputs.begin();
	  mvaInput != mvaInputs.end(); ++mvaInput ) {
      if ( TMath::IsNaN(mvaInput->second) ) {
	std::cout << "Warning in run = " << run << ", lumi = " << lumi << ", event = " << event << ":" << std::endl; 
	std::cout << " mvaInput #" << index << " ('" << mvaInput->first << "') = " << mvaInput->second << " --> setting mvaInput value to zero !!" << std::endl; 
	mvaInputs[mvaInput->first] = 0.;
	++index;
      }
    }

    double mvaOutput_3l_ttV = mva_3l_ttV(mvaInputs);
    double mvaOutput_3l_ttbar = mva_3l_ttbar(mvaInputs);

//--- compute integer discriminant based on both BDT outputs,
//    as defined in Table 10 of AN-2016/211
    Double_t mvaDiscr_3l = -1;
    if      ( mvaOutput_3l_ttbar >  +0.3 && mvaOutput_3l_ttV >  -0.1 ) mvaDiscr_3l = 3.;
    else if ( mvaOutput_3l_ttbar <= +0.3 && mvaOutput_3l_ttV <= -0.1 ) mvaDiscr_3l = 1.;
    else                                                               mvaDiscr_3l = 2.;

    const RecoLepton* preselLepton1_OS = 0;
    const RecoLepton* preselLepton2_OS = 0;
    if ( preselLepton_lead->charge_*selHadTau->charge_ < 0. ) {
      preselLepton1_OS = preselLepton_lead;
    } 
    if ( preselLepton_sublead->charge_*selHadTau->charge_ < 0. ) {
      if ( !preselLepton1_OS ) preselLepton1_OS = preselLepton_sublead;
      else preselLepton2_OS = preselLepton_sublead;
    }
    if ( preselLepton_third->charge_*selHadTau->charge_ < 0. ) {
      if ( !preselLepton1_OS ) preselLepton1_OS = preselLepton_third;
      else if ( !preselLepton2_OS ) preselLepton2_OS = preselLepton_third;
    }
    double mTauTauVis1_presel = ( preselLepton1_OS ) ? (preselLepton1_OS->p4_ + selHadTau->p4_).mass() : -1.;
    double mTauTauVis2_presel = ( preselLepton2_OS ) ? (preselLepton2_OS->p4_ + selHadTau->p4_).mass() : -1.;
     
//--- fill histograms with events passing preselection
    preselMuonHistManager.fillHistograms(preselMuons, evtWeight);
    preselElectronHistManager.fillHistograms(preselElectrons, evtWeight);
    preselHadTauHistManager.fillHistograms(selHadTaus, evtWeight);
    preselJetHistManager.fillHistograms(selJets, evtWeight);
    selBJet_looseHistManager.fillHistograms(selBJets_loose, evtWeight);
    selBJet_mediumHistManager.fillHistograms(selBJets_medium, evtWeight);
    preselMEtHistManager.fillHistograms(met_p4, mht_p4, met_LD, evtWeight);
    preselEvtHistManager.fillHistograms(preselElectrons.size(), preselMuons.size(), selHadTaus.size(), 
      selJets.size(), selBJets_loose.size(), selBJets_medium.size(), 
      mvaOutput_3l_ttV, mvaOutput_3l_ttbar, mvaDiscr_3l, 
      mTauTauVis1_presel, mTauTauVis2_presel, evtWeight);

//--- apply final event selection 
    std::vector<const RecoLepton*> selLeptons;    
    selLeptons.reserve(selElectrons.size() + selMuons.size());
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
    selLeptons.insert(selLeptons.end(), selMuons.begin(), selMuons.end());
    std::sort(selLeptons.begin(), selLeptons.end(), isHigherPt);
    // require exactly two leptons passing tight selection criteria of final event selection 
    if ( !(selLeptons.size() == 3) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selLeptons selection." << std::endl;
	std::cout << " (#selLeptons = " << selLeptons.size() << ")" << std::endl;
	for ( size_t idxSelLepton = 0; idxSelLepton < selLeptons.size(); ++idxSelLepton ) {
	  std::cout << "selLepton #" << idxSelLepton << ":" << std::endl;
	  std::cout << (*selLeptons[idxSelLepton]);
	}
      }
      continue;
    }
    cutFlowTable.update("3 sel leptons", evtWeight);
    const RecoLepton* selLepton_lead = selLeptons[0];
    const RecoLepton* selLepton_sublead = selLeptons[1];
    const RecoLepton* selLepton_third = selLeptons[2];

    // require that trigger paths match event category (with event category based on selLeptons);
    if ( (selElectrons.size() == 3 &&                         !(selTrigger_1e || selTrigger_2e                                                        ) ) ||
	 (selElectrons.size() == 2 && selMuons.size() == 1 && !(selTrigger_1e || selTrigger_2e || selTrigger_1mu ||                   selTrigger_1e1mu) ) ||
	 (selElectrons.size() == 1 && selMuons.size() == 2 && !(selTrigger_1e ||                  selTrigger_1mu || selTrigger_2mu || selTrigger_1e1mu) ) ||
	 (                            selMuons.size() == 3 && !(                                  selTrigger_1mu || selTrigger_2mu                    ) ) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given selLepton multiplicity." << std::endl; 
	std::cout << " (#selElectrons = " << selElectrons.size() 
		  << ", #selMuons = " << selMuons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu 
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    } 
    cutFlowTable.update("sel lepton trigger match");

    // apply requirement on jets (incl. b-tagged jets) and hadronic taus on level of final event selection
    if ( !(selJets.size() >= 2) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selJets selection." << std::endl;
	std::cout << " (#selJets = " << selJets.size() << ")" << std::endl;
	for ( size_t idxSelJet = 0; idxSelJet < selJets.size(); ++idxSelJet ) {
	  std::cout << "selJet #" << idxSelJet << ":" << std::endl;
	  std::cout << (*selJets[idxSelJet]);
	}
      }
      continue;
    }
    cutFlowTable.update(">= 2 jets (2)", evtWeight);
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1)) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selBJets selection." << std::endl;
	std::cout << " (#selBJets_loose = " << selBJets_loose.size() << ", #selBJets_medium = " << selBJets_medium.size() << ")" << std::endl;
	for ( size_t idxSelBJet_loose = 0; idxSelBJet_loose < selBJets_loose.size(); ++idxSelBJet_loose ) {
	  std::cout << "selJet #" << idxSelBJet_loose << ":" << std::endl;
	  std::cout << (*selJets[idxSelBJet_loose]);
	}
	for ( size_t idxSelBJet_medium = 0; idxSelBJet_medium < selBJets_medium.size(); ++idxSelBJet_medium ) {
	  std::cout << "selJet #" << idxSelBJet_medium << ":" << std::endl;
	  std::cout << (*selJets[idxSelBJet_medium]);
	}
      }
      continue;
    }
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (2)", evtWeight);
    if ( !(selHadTaus.size() >= 1) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selHadTaus selection." << std::endl;
	std::cout << " (#selHadTaus = " << selHadTaus.size() << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(">= 1 sel tau (2)", evtWeight);

    const hadTauGenMatchEntry& hadTauGenMatch = getHadTauGenMatch(hadTauGenMatch_definitions, selHadTau);
    assert(!(hadTauGenMatch.idx_ == kGen_Undefined1 || hadTauGenMatch.idx_ == kGen_All1));

    if ( hadTauGenMatchSelection != kGen_All1 ) {
      if ( hadTauGenMatch.idx_ != hadTauGenMatchSelection ) continue;
      cutFlowTable.update("tau gen match", evtWeight);
    }
    
    bool failsLowMassVeto = false;
    for ( std::vector<const RecoLepton*>::const_iterator lepton1 = selLeptons.begin();
	  lepton1 != selLeptons.end(); ++lepton1 ) {
      for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
	    lepton2 != selLeptons.end(); ++lepton2 ) {
	if ( ((*lepton1)->p4_ + (*lepton2)->p4_).mass() < 12. ) {
	  failsLowMassVeto = true;
	}
      }
    }
    if ( failsLowMassVeto ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS low mass lepton pair veto." << std::endl;
      }
      continue;
    }
    cutFlowTable.update("m(ll) > 12 GeV", evtWeight);

    double minPt_lead = 20.;
    double minPt_sublead = selLepton_sublead->is_electron() ? 15. : 10.;
    double minPt_third = 10.;
    if ( !(selLepton_lead->pt_ > minPt_lead && selLepton_sublead->pt_ > minPt_sublead && selLepton_third->pt_ > minPt_third) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton pT selection." << std::endl;
	std::cout << " (leading selLepton pT = " << selLepton_lead->pt_ << ", minPt_lead = " << minPt_lead
		  << ", subleading selLepton pT = " << selLepton_sublead->pt_ << ", minPt_sublead = " << minPt_sublead 
		  << ", third selLepton pT = " << selLepton_third->pt_ << ", minPt_third = " << minPt_third << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("lead lepton pT > 20 GeV && sublead lepton pT > 15(e)/10(mu) GeV && third lepton pT > 10 GeV", evtWeight);
    
    int sumLeptonCharge = selLepton_lead->charge_ + selLepton_sublead->charge_ + selLepton_third->charge_;
    if ( std::abs(sumLeptonCharge) != 1 ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge_ 
		  << ", subleading selLepton charge = " << selLepton_sublead->charge_ 
		  << ", third selLepton charge = " << selLepton_third->charge_ << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("sel lepton charge");

    bool isCharge_SS = sumLeptonCharge*selHadTau->charge_ > 0;
    bool isCharge_OS = sumLeptonCharge*selHadTau->charge_ < 0;
    if ( (chargeSelection == kOS && isCharge_SS) || (chargeSelection == kSS && isCharge_OS) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton+tau charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge_ 
		  << ", subleading selLepton charge = " << selLepton_sublead->charge_ 
		  << ", third selLepton charge = " << selLepton_third->charge_ 
		  << ", selHadTau charge = " << selHadTau->charge_ << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("sel lepton+tau charge");

    bool isSameFlavor_OS = false;
    double massSameFlavor_OS = -1.;
    for ( std::vector<const RecoLepton*>::const_iterator lepton1 = selLeptons.begin();
	  lepton1 != selLeptons.end(); ++lepton1 ) {
      for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
	    lepton2 != selLeptons.end(); ++lepton2 ) {
	if ( (*lepton1)->pdgId_ == -(*lepton2)->pdgId_ ) { // pair of same flavor leptons of opposite charge
	  isSameFlavor_OS = true;
	  double mass = ((*lepton1)->p4_ + (*lepton2)->p4_).mass();
	  if ( std::fabs(mass - z_mass) < std::fabs(massSameFlavor_OS - z_mass) ) massSameFlavor_OS = mass;
	}
      }
    }

    bool failsZbosonMassVeto = isSameFlavor_OS && std::fabs(massSameFlavor_OS - z_mass) < z_window;
    if ( failsZbosonMassVeto ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS Z-boson veto." << std::endl;
      }
      continue;
    }
    cutFlowTable.update("Z-boson mass veto", evtWeight);

    double met_LD_cut = 0.;
    if      ( selJets.size() >= 4 ) met_LD_cut = -1.; // MET LD cut not applied
    else if ( isSameFlavor_OS     ) met_LD_cut =  0.3;
    else                            met_LD_cut =  0.2;
    if ( met_LD_cut > 0 && met_LD < met_LD_cut ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS MET LD selection." << std::endl;
	std::cout << " (met_LD = " << met_LD << ", met_LD_cut = " << met_LD_cut << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("met LD", evtWeight);

    if ( leptonSelection == kFakeable ) {
      if ( (tightMuons.size() + tightElectrons.size()) >= 3 ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS tightElectrons+tightMuons selection." << std::endl;
	  std::cout << " (#tightElectrons = " << tightElectrons.size() << ", #tightMuons = " << tightMuons.size() << ")" << std::endl;
	}
	continue; // CV: avoid overlap with signal region
      }
    }
    cutFlowTable.update("signal region veto", evtWeight);

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the tight identification and isolation criteria
    if ( isMC ) {
      double sf_tight_to_loose = 1.;
      if ( leptonSelection == kFakeable ) {
	sf_tight_to_loose = sf_leptonID_and_Iso_fakeable_to_loose(3, 
          preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	  preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_,
          preselLepton_third_type, preselLepton_third->pt_, preselLepton_third->eta_);
      } else if ( leptonSelection == kTight ) {
	sf_tight_to_loose = sf_leptonID_and_Iso_tight_to_loose(3, 
          preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
          preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_,
          preselLepton_third_type, preselLepton_third->pt_, preselLepton_third->eta_);
      }
      evtWeight *= sf_tight_to_loose;
    }

    if ( leptonSelection == kFakeable ) {
      TH2* lutFakeRate_lead = 0;
      if      ( std::abs(selLepton_lead->pdgId_) == 11 ) lutFakeRate_lead = lutFakeRate_e;
      else if ( std::abs(selLepton_lead->pdgId_) == 13 ) lutFakeRate_lead = lutFakeRate_mu;
      assert(lutFakeRate_lead);
      double prob_fake_lead = get_sf_from_TH2(lutFakeRate_lead, selLepton_lead->pt_, selLepton_lead->eta_);

      TH2* lutFakeRate_sublead = 0;
      if      ( std::abs(selLepton_sublead->pdgId_) == 11 ) lutFakeRate_sublead = lutFakeRate_e;
      else if ( std::abs(selLepton_sublead->pdgId_) == 13 ) lutFakeRate_sublead = lutFakeRate_mu;
      assert(lutFakeRate_sublead);
      double prob_fake_sublead = get_sf_from_TH2(lutFakeRate_sublead, selLepton_sublead->pt_, selLepton_sublead->eta_);

      TH2* lutFakeRate_third = 0;
      if      ( std::abs(selLepton_third->pdgId_) == 11 ) lutFakeRate_third = lutFakeRate_e;
      else if ( std::abs(selLepton_third->pdgId_) == 13 ) lutFakeRate_third = lutFakeRate_mu;
      assert(lutFakeRate_third);
      double prob_fake_third = get_sf_from_TH2(lutFakeRate_third, selLepton_third->pt_, selLepton_third->eta_);

      bool passesTight_lead = isMatched(*selLepton_lead, tightElectrons) || isMatched(*selLepton_lead, tightMuons);
      bool passesTight_sublead = isMatched(*selLepton_sublead, tightElectrons) || isMatched(*selLepton_sublead, tightMuons);
      bool passesTight_third = isMatched(*selLepton_third, tightElectrons) || isMatched(*selLepton_third, tightMuons);

      double p1 = prob_fake_lead/(1. - prob_fake_lead);
      double p2 = prob_fake_sublead/(1. - prob_fake_sublead);
      double p3 = prob_fake_third/(1. - prob_fake_third);
      double evtWeight_tight_to_loose = 0.;
      if      ( !passesTight_lead &&  passesTight_sublead &&  passesTight_third ) evtWeight_tight_to_loose =  p1;
      else if (  passesTight_lead && !passesTight_sublead &&  passesTight_third ) evtWeight_tight_to_loose =  p2;
      else if (  passesTight_lead &&  passesTight_sublead && !passesTight_third ) evtWeight_tight_to_loose =  p3;
      else if ( !passesTight_lead && !passesTight_sublead &&  passesTight_third ) evtWeight_tight_to_loose = -p1*p2;
      else if ( !passesTight_lead &&  passesTight_sublead && !passesTight_third ) evtWeight_tight_to_loose = -p1*p3;
      else if (  passesTight_lead && !passesTight_sublead && !passesTight_third ) evtWeight_tight_to_loose = -p2*p3;
      else if ( !passesTight_lead && !passesTight_sublead && !passesTight_third ) evtWeight_tight_to_loose =  p1*p2*p3;
      evtWeight *= evtWeight_tight_to_loose;
    }

    if ( hadTauSelection == kFakeable ) {
      if ( tightHadTaus.size() >= 1 ) continue; // CV: avoid overlap with signal region
      cutFlowTable.update("signal region veto", evtWeight);
    }

    if ( applyJetToTauFakeRateWeight ) {
      assert(jetToTauFakeRateInterface);
      
      double prob_fake = jetToTauFakeRateInterface->getWeight_lead(selHadTau->pt_, selHadTau->absEta_);
      
      double p1 = prob_fake/(1. - prob_fake);
      double evtWeight_tight_to_loose = p1;
      evtWeight *= evtWeight_tight_to_loose;
    }

//--- apply data/MC corrections for e->tau and mu->tau fake-rates
    if ( isMC ) {
      if ( selHadTau->genLepton_ ) {
	int abs_pdgId = std::abs(selHadTau->genLepton_->pdgId_);
	if      ( abs_pdgId == 11 ) evtWeight *= sf_eToTauFakeRate_2016(selHadTau->pt_, selHadTau->absEta_, hadTauSelection_antiElectron, eToTauFakeRate_option);
	else if ( abs_pdgId == 13 ) evtWeight *= sf_muToTauFakeRate_2016(selHadTau->pt_, selHadTau->absEta_, hadTauSelection_antiMuon, muToTauFakeRate_option);
      }
    }    
    
//--- fill histograms with events passing final selection 
    selMuonHistManager.fillHistograms(selMuons, evtWeight);
    selElectronHistManager.fillHistograms(selElectrons, evtWeight);
    selHadTauHistManager.fillHistograms(selHadTaus, evtWeight);
    selJetHistManager.fillHistograms(selJets, evtWeight);
    selJetHistManager_lead.fillHistograms(selJets, evtWeight);
    selJetHistManager_sublead.fillHistograms(selJets, evtWeight);
    selBJet_looseHistManager.fillHistograms(selBJets_loose, evtWeight);
    selBJet_looseHistManager_lead.fillHistograms(selBJets_loose, evtWeight);
    selBJet_looseHistManager_sublead.fillHistograms(selBJets_loose, evtWeight);
    selBJet_mediumHistManager.fillHistograms(selBJets_medium, evtWeight);
    selMEtHistManager.fillHistograms(met_p4, mht_p4, met_LD, evtWeight);
    selEvtHistManager.fillHistograms(selElectrons.size(), selMuons.size(), selHadTaus.size(), 
      selJets.size(), selBJets_loose.size(), selBJets_medium.size(), 
      mvaOutput_3l_ttV, mvaOutput_3l_ttbar, mvaDiscr_3l, 
      mTauTauVis1_presel, mTauTauVis2_presel, evtWeight);
    if ( isSignal ) {
      for ( const auto & kv: decayMode_idString ) {
        if ( std::fabs(genHiggsDecayMode - kv.second) < EPS ) {
          selEvtHistManager_decayMode[kv.first]->fillHistograms(selElectrons.size(), selMuons.size(), selHadTaus.size(), 
            selJets.size(), selBJets_loose.size(), selBJets_medium.size(), 
            mvaOutput_3l_ttV, mvaOutput_3l_ttbar, mvaDiscr_3l, 
            mTauTauVis1_presel, mTauTauVis2_presel, evtWeight);
          break;
        }
      }
    }
    if ( isMC && !apply_hadTauGenMatching ) {
      EvtHistManager_3l_1tau* selEvtHistManager_genMatch = selEvtHistManagers_genMatch[hadTauGenMatch.idx_];
      assert(selEvtHistManager_genMatch);
      selEvtHistManager_genMatch->fillHistograms(selElectrons.size(), selMuons.size(), selHadTaus.size(), 
        selJets.size(), selBJets_loose.size(), selBJets_medium.size(), 
        mvaOutput_3l_ttV, mvaOutput_3l_ttbar, mvaDiscr_3l, 
        mTauTauVis1_presel, mTauTauVis2_presel, evtWeight);
    }

    if ( isMC ) {
      genEvtHistManager_afterCuts.fillHistograms(genElectrons, genMuons, genHadTaus, genJets);
    }
    
    if ( writeSelEventsFile ) {
      // KE: unique merge loose and medium B-jets, and hadronic jets
      std::sort(selBJets_medium.begin(), selBJets_medium.end(), isHigherPt);
      std::sort(selBJets_loose.begin(), selBJets_loose.end(), isHigherPt);
      std::sort(selJets.begin(), selJets.end(), isHigherCSV); // optional: sort by pT
      std::vector<const RecoJet *> selBJetsMerged(selBJets_medium);
      auto unique_push_back = [&selBJetsMerged](const std::vector<const RecoJet *> & v) -> void
      {
        for(const RecoJet * j: v)
          if(std::find(selBJetsMerged.begin(), selBJetsMerged.end(), j) == selBJetsMerged.end())
            selBJetsMerged.push_back(j);
      };
      unique_push_back(selBJets_loose);
      unique_push_back(selJets);
      if(selBJetsMerged.size() < 2)
      {
        std::cerr << "Error: merged b-jets contains less than two jets\n";
        assert(0);
      }

      eventSpecificsOut.run  = run;
      eventSpecificsOut.lumi = lumi;
      eventSpecificsOut.evt  = event;

      eventSpecificsOut.met_pt  = met_pt;
      eventSpecificsOut.met_phi = met_phi;

      for(std::size_t i = 0; i < 3; ++i)
        leptonsOut[i].setValues(selLeptons[i]);
      for(std::size_t i = 0; i < 2; ++i)
        jetsOut[i].setValues(selBJetsMerged[i]);
      htauOut.setValues(selHadTau);
      MVAOut.setValues(mvaInputs, mvaOutput_3l_ttV, mvaOutput_3l_ttbar);

      selEventsTTree -> Fill();
    }

    (*selEventsFile) << run << ":" << lumi << ":" << event << std::endl;

    ++selectedEntries;
    selectedEntries_weighted += evtWeight;
  }

  std::cout << "num. Entries = " << numEntries << std::endl;
  std::cout << " analyzed = " << analyzedEntries << std::endl;
  std::cout << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")" << std::endl;

  for ( std::vector<TFile*>::iterator inputFile = inputFilesToClose.begin();
	inputFile != inputFilesToClose.end(); ++inputFile ) {
    delete (*inputFile);
  }  

  std::cout << "cut-flow table" << std::endl;
  cutFlowTable.print(std::cout);
  std::cout << std::endl;

  if(writeSelEventsFile)
  {
    selEventsTFile -> Write();
    delete selEventsTFile;
  }

  delete jetToTauFakeRateInterface;
  
  delete run_lumi_eventSelector;

  delete selEventsFile;

  delete muonReader;
  delete electronReader;
  delete hadTauReader;
  delete jetReader;
  delete genLeptonReader;
  delete genHadTauReader;
  delete genJetReader;

  hltPaths_delete(triggers_1e);
  hltPaths_delete(triggers_2e);
  hltPaths_delete(triggers_1mu);
  hltPaths_delete(triggers_2mu);
  hltPaths_delete(triggers_1e1mu);

  clock.Show("analyze_3l_1tau");

  return EXIT_SUCCESS;
}

