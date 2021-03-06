#ifndef tthAnalysis_HiggsToTauTau_GenJetReader_h
#define tthAnalysis_HiggsToTauTau_GenJetReader_h

#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet

#include <Rtypes.h> // Int_t, Double_t
#include <TTree.h> // TTree

#include <string>
#include <vector>
#include <map>

class GenJetReader
{
 public:
  GenJetReader();
  GenJetReader(const std::string& branchName_num, const std::string& branchName_obj); 
  ~GenJetReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenJet branches
   */
  void setBranchAddresses(TTree* tree);

  /**
   * @brief Read branches from tree and use information to fill collection of GenJet objects
   * @return Collection of GenJet objects
   */
  std::vector<GenJet> read() const;
  
 protected: 
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void setBranchNames();

  const int max_nJets_;
  std::string branchName_num_;
  std::string branchName_obj_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;

  Int_t nJets_;
  Float_t* jet_pt_;
  Float_t* jet_eta_;
  Float_t* jet_phi_;
  Float_t* jet_mass_;

  // CV: make sure that only one RecoJetReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenJetReader*> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_GenJetReader_h

