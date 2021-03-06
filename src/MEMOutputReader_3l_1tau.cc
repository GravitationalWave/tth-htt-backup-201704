#include "tthAnalysis/HiggsToTauTau/interface/MEMOutputReader_3l_1tau.h" // MEMOutputReader_3l_1tau

#include "FWCore/Utilities/interface/Exception.h"

#include <TString.h> // Form

#include <assert.h> // assert

std::map<std::string, int> MEMOutputReader_3l_1tau::numInstances_;
std::map<std::string, MEMOutputReader_3l_1tau*> MEMOutputReader_3l_1tau::instances_;

MEMOutputReader_3l_1tau::MEMOutputReader_3l_1tau(const std::string& branchName_num, const std::string& branchName_obj)
  : max_nMEMOutputs_(100)
  , branchName_num_(branchName_num)
  , branchName_obj_(branchName_obj)
  , run_(0)
  , lumi_(0)
  , evt_(0)
  , leadLepton_eta_(0)
  , leadLepton_phi_(0)
  , subleadLepton_eta_(0)
  , subleadLepton_phi_(0)
  , thirdLepton_eta_(0)
  , thirdLepton_phi_(0)
  , hadTau_eta_(0)
  , hadTau_phi_(0)
  , weight_ttH_(0)
  , weight_ttZ_(0)
  , weight_ttH_hww_(0)
  , LR_(0)
  , cpuTime_(0)
  , realTime_(0)
  , isValid_(0)
  , errorFlag_(0)
{
  setBranchNames();
}

MEMOutputReader_3l_1tau::~MEMOutputReader_3l_1tau()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if ( numInstances_[branchName_obj_] == 0 ) {
    MEMOutputReader_3l_1tau* gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->run_;
    delete[] gInstance->lumi_;
    delete[] gInstance->evt_;
    delete[] gInstance->leadLepton_eta_;
    delete[] gInstance->leadLepton_phi_;
    delete[] gInstance->subleadLepton_eta_;
    delete[] gInstance->subleadLepton_phi_;
    delete[] gInstance->thirdLepton_eta_;
    delete[] gInstance->thirdLepton_phi_;
    delete[] gInstance->hadTau_eta_;
    delete[] gInstance->hadTau_phi_;
    delete[] gInstance->weight_ttH_;
    delete[] gInstance->weight_ttZ_;
    delete[] gInstance->weight_ttH_hww_;
    delete[] gInstance->LR_;
    delete[] gInstance->cpuTime_;
    delete[] gInstance->realTime_;
    delete[] gInstance->isValid_;
    delete[] gInstance->errorFlag_;
    instances_[branchName_obj_] = 0;
  }
}

void MEMOutputReader_3l_1tau::setBranchNames()
{
  if ( numInstances_[branchName_obj_] == 0 ) {
    branchName_run_ = Form("%s_%s", branchName_obj_.data(), "run");
    branchName_lumi_ = Form("%s_%s", branchName_obj_.data(), "lumi");
    branchName_evt_ = Form("%s_%s", branchName_obj_.data(), "evt");
    branchName_leadLepton_eta_ = Form("%s_%s", branchName_obj_.data(), "leadLepton_eta");
    branchName_leadLepton_phi_ = Form("%s_%s", branchName_obj_.data(), "leadLepton_phi");
    branchName_subleadLepton_eta_ = Form("%s_%s", branchName_obj_.data(), "subleadLepton_eta");
    branchName_subleadLepton_phi_ = Form("%s_%s", branchName_obj_.data(), "subleadLepton_phi");
    branchName_thirdLepton_eta_ = Form("%s_%s", branchName_obj_.data(), "thirdLepton_eta");
    branchName_thirdLepton_phi_ = Form("%s_%s", branchName_obj_.data(), "thirdLepton_phi");
    branchName_hadTau_eta_ = Form("%s_%s", branchName_obj_.data(), "hadTau_eta");
    branchName_hadTau_phi_ = Form("%s_%s", branchName_obj_.data(), "hadTau_phi");
    branchName_weight_ttH_ = Form("%s_%s", branchName_obj_.data(), "weight_ttH");
    branchName_weight_ttZ_ = Form("%s_%s", branchName_obj_.data(), "weight_ttZ");
    branchName_weight_ttH_hww_ = Form("%s_%s", branchName_obj_.data(), "weight_ttH_hww");
    branchName_LR_ = Form("%s_%s", branchName_obj_.data(), "LR");
    branchName_cpuTime_ = Form("%s_%s", branchName_obj_.data(), "cpuTime");
    branchName_realTime_ = Form("%s_%s", branchName_obj_.data(), "realTime");
    branchName_isValid_ = Form("%s_%s", branchName_obj_.data(), "isValid");
    branchName_errorFlag_ = Form("%s_%s", branchName_obj_.data(), "errorFlag");
    instances_[branchName_obj_] = this;
  } else {
    if ( branchName_num_ != instances_[branchName_obj_]->branchName_num_ ) {
      throw cms::Exception("MEMOutputReader_3l_1tau") 
	<< "Association between configuration parameters 'branchName_num' and 'branchName_obj' must be unique:"
	<< " present association 'branchName_num' = " << branchName_num_ << " with 'branchName_obj' = " << branchName_obj_ 
	<< " does not match previous association 'branchName_num' = " << instances_[branchName_obj_]->branchName_num_ << " with 'branchName_obj' = " << instances_[branchName_obj_]->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}



void MEMOutputReader_3l_1tau::setBranchAddresses(TTree* tree)
{
  if ( instances_[branchName_obj_] == this ) {
    tree->SetBranchAddress(branchName_num_.data(), &nMEMOutputs_);   
    run_ = new RUN_TYPE[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_run_.data(), run_); 
    lumi_ = new LUMI_TYPE[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_lumi_.data(), lumi_); 
    evt_ = new EVT_TYPE[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_evt_.data(), evt_); 
    leadLepton_eta_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_leadLepton_eta_.data(), leadLepton_eta_); 
    leadLepton_phi_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_leadLepton_phi_.data(), leadLepton_phi_); 
    subleadLepton_eta_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_subleadLepton_eta_.data(), subleadLepton_eta_); 
    subleadLepton_phi_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_subleadLepton_phi_.data(), subleadLepton_phi_); 
    thirdLepton_eta_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_thirdLepton_eta_.data(), thirdLepton_eta_); 
    thirdLepton_phi_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_thirdLepton_phi_.data(), thirdLepton_phi_);     
    hadTau_eta_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_hadTau_eta_.data(), hadTau_eta_); 
    hadTau_phi_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_hadTau_phi_.data(), hadTau_phi_); 
    weight_ttH_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_weight_ttH_.data(), weight_ttH_); 
    weight_ttZ_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_weight_ttZ_.data(), weight_ttZ_); 
    weight_ttH_hww_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_weight_ttH_hww_.data(), weight_ttH_hww_); 
    LR_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_LR_.data(), LR_); 
    cpuTime_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_cpuTime_.data(), cpuTime_); 
    realTime_ = new Float_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_realTime_.data(), realTime_); 
    errorFlag_ = new Int_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_errorFlag_.data(), errorFlag_); 
    isValid_ = new Int_t[max_nMEMOutputs_];
    tree->SetBranchAddress(branchName_isValid_.data(), isValid_); 
  }
}

std::vector<MEMOutput_3l_1tau> MEMOutputReader_3l_1tau::read() const
{
  MEMOutputReader_3l_1tau* gInstance = instances_[branchName_obj_];
  assert(gInstance);
  std::vector<MEMOutput_3l_1tau> memOutputs;
  Int_t nMEMOutputs = gInstance->nMEMOutputs_;
  if ( nMEMOutputs > max_nMEMOutputs_ ) {
    throw cms::Exception("MEMOutputReader_3l_1tau") 
      << "Number of MEMOutputs stored in Ntuple = " << nMEMOutputs << ", exceeds max_nMEMOutputs = " << max_nMEMOutputs_ << " !!\n";
  }
  if ( nMEMOutputs > 0 ) {
    memOutputs.reserve(nMEMOutputs);
    for ( Int_t idxMEMOutput = 0; idxMEMOutput < nMEMOutputs; ++idxMEMOutput ) {
      memOutputs.push_back(MEMOutput_3l_1tau(
	gInstance->run_[idxMEMOutput],
	gInstance->lumi_[idxMEMOutput],
	gInstance->evt_[idxMEMOutput],			     
        gInstance->leadLepton_eta_[idxMEMOutput],
        gInstance->leadLepton_phi_[idxMEMOutput],
        gInstance->subleadLepton_eta_[idxMEMOutput],
        gInstance->subleadLepton_phi_[idxMEMOutput],
	gInstance->thirdLepton_eta_[idxMEMOutput],
        gInstance->thirdLepton_phi_[idxMEMOutput],
        gInstance->hadTau_eta_[idxMEMOutput],
        gInstance->hadTau_phi_[idxMEMOutput],
        gInstance->weight_ttH_[idxMEMOutput],
        gInstance->weight_ttZ_[idxMEMOutput],
        gInstance->weight_ttH_hww_[idxMEMOutput],
        gInstance->LR_[idxMEMOutput],
        gInstance->cpuTime_[idxMEMOutput],
        gInstance->realTime_[idxMEMOutput],
        gInstance->isValid_[idxMEMOutput],	
        gInstance->errorFlag_[idxMEMOutput] ));
    }
  }
  return memOutputs;
}
