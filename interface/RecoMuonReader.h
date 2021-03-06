#ifndef tthAnalysis_HiggsToTauTau_RecoMuonReader_h
#define tthAnalysis_HiggsToTauTau_RecoMuonReader_h

#include "tthAnalysis/HiggsToTauTau/interface/RecoMuon.h" // RecoMuon
#include "tthAnalysis/HiggsToTauTau/interface/RecoLeptonReader.h" // RecoLeptonReader

#include <Rtypes.h> // Int_t
#include <TTree.h> // TTree

#include <string>
#include <vector>
#include <map>

class RecoMuonReader
{
 public:
  RecoMuonReader(int era);
  RecoMuonReader(int era, const std::string& branchName_num, const std::string& branchName_obj); 
  ~RecoMuonReader();

  /**
   * @brief Call tree->SetBranchAddress for all lepton branches specific to RecoMuons
   */
  void setBranchAddresses(TTree* tree);

  /**
   * @brief Read branches from tree and use information to fill collection of RecoMuon objects
   * @return Collection of RecoMuon objects
   */
  std::vector<RecoMuon> read() const;
  
 protected: 
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void setBranchNames();

  int era_;
  std::string branchName_num_;
  std::string branchName_obj_;

  RecoLeptonReader* leptonReader_;

  std::string branchName_looseIdPOG_; 
  std::string branchName_mediumIdPOG_;
#ifdef DPT_DIV_PT
  std::string branchName_dpt_div_pt_;
#endif
  std::string branchName_segmentCompatibility_;

  Int_t* looseIdPOG_;
  Int_t* mediumIdPOG_;
#ifdef DPT_DIV_PT
  Float_t* dpt_div_pt_;
#endif
  Float_t* segmentCompatibility_;

  // CV: make sure that only one RecoMuonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoMuonReader*> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_RecoMuonReader_h

