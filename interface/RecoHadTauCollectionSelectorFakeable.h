#ifndef tthAnalysis_HiggsToTauTau_RecoHadTauSelectorFakeable_h
#define tthAnalysis_HiggsToTauTau_RecoHadTauSelectorFakeable_h

#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h" // RecoHadTau

#include <Rtypes.h> // Int_t, Double_t

#include <string>
#include <map>

class RecoHadTauSelectorFakeable
{
 public:
  RecoHadTauSelectorFakeable(int index = -1, bool debug = false);
  ~RecoHadTauSelectorFakeable() {}

  /**
   * @brief Check if hadronic tau given as function argument passes selection criteria for "fakeable" tau candidates, 
   *   used to estimate the background contribution arising from jets that are misidentified as hadronic tau decays
   * @return True if hadronic tau passes selection; false otherwise
   */
  bool operator()(const RecoHadTau& hadTau) const;

  friend class RecoHadTauCollectionSelectorFakeable;
 protected: 
  Double_t min_pt_;            ///< lower cut threshold on pT
  Double_t max_absEta_;        ///< upper cut threshold on absolute value of eta
  Double_t max_dz_;            ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  Int_t min_decayModeFinding_; ///< lower cut threshold on decayModeFinding discriminator
  Int_t min_id_mva_dR03_;      ///< lower cut threshold on MVA-based tau id computed with dR=0.3 isolation cone
  Double_t min_raw_mva_dR03_;  ///< lower cut threshold on raw output of MVA-based tau id computed with dR=0.3 isolation cone
  Int_t min_id_mva_dR05_;      ///< lower cut threshold on MVA-based tau id computed with dR=0.5 isolation cone
  Double_t min_raw_mva_dR05_;  ///< lower cut threshold on raw output of MVA-based tau id computed with dR=0.5 isolation cone
  Int_t min_id_cut_dR03_;      ///< lower cut threshold on cut-based tau id computed with dR=0.3 isolation cone
  Double_t max_raw_cut_dR03_;  ///< upper cut threshold on raw isolation pT-sum of cut-based tau id computed with dR=0.3 isolation cone
  Int_t min_id_cut_dR05_;      ///< lower cut threshold on cut-based tau id computed with dR=0.5 isolation cone
  Double_t max_raw_cut_dR05_;  ///< upper cut threshold on raw isolation pT-sum of cut-based tau id computed with dR=0.5 isolation cone
  Int_t min_antiElectron_;     ///< lower cut threshold on discriminator against electrons
  Int_t min_antiMuon_;         ///< lower cut threshold on discriminator against muons
};

class RecoHadTauCollectionSelectorFakeable
{
 public:
  RecoHadTauCollectionSelectorFakeable(int index = -1, bool debug = false)
    : selIndex_(index)
    , selector_(index, debug)
  {}
  ~RecoHadTauCollectionSelectorFakeable() {}

  void set_min_pt(double min_pt) { selector_.min_pt_ = min_pt; }

  void set_min_antiElectron(int min_antiElectron) { selector_.min_antiElectron_ = min_antiElectron; }
  void set_min_antiMuon(int min_antiMuon) { selector_.min_antiMuon_ = min_antiMuon; }

  std::vector<const RecoHadTau*> operator()(const std::vector<const RecoHadTau*>& hadTaus) const
  {
    std::vector<const RecoHadTau*> selHadTaus;
    int idx = 0;
    for ( typename std::vector<const RecoHadTau*>::const_iterator hadTau = hadTaus.begin();
	  hadTau != hadTaus.end(); ++hadTau ) {
      if ( (idx == selIndex_ || selIndex_ == -1) && selector_(**hadTau) ) {
	selHadTaus.push_back(*hadTau);
      }
      ++idx;
    }
    return selHadTaus;
  }
  
 protected: 
  int selIndex_;
  RecoHadTauSelectorFakeable selector_;
};

#endif // tthAnalysis_HiggsToTauTau_RecoHadTauSelectorFakeable_h
