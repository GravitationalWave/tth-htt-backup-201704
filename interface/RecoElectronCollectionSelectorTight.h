#ifndef tthAnalysis_HiggsToTauTau_RecoElectronCollectionSelectorTight_h
#define tthAnalysis_HiggsToTauTau_RecoElectronCollectionSelectorTight_h

#include "tthAnalysis/HiggsToTauTau/interface/RecoElectron.h" // RecoElectron

#include <Rtypes.h> // Int_t, Double_t

#include <string>
#include <map>

class RecoElectronSelectorTight
{
 public:
  RecoElectronSelectorTight(int index = -1, bool debug = false);
  ~RecoElectronSelectorTight() {}

  /**
   * @brief Set cut thresholds
   */

  // enable/disable cuts on electron ID variables to mimic electron ID cuts applied by single electron trigger 
  void enable_offline_e_trigger_cuts()  { apply_offline_e_trigger_cuts_ = true; }
  void disable_offline_e_trigger_cuts() { apply_offline_e_trigger_cuts_ = false; }

  // enable/disable tight charge cut
  void enable_tightCharge_cut() { apply_tightCharge_ = true; }
  void disable_tightCharge_cut() { apply_tightCharge_ = false; }

  // enable/disable photon conversion veto
  void enable_conversionVeto() { apply_conversionVeto_ = true; }
  void disable_conversionVeto() { apply_conversionVeto_ = false; }

  /**
   * @brief Check if electron given as function argument passes "tight" electron selection, defined in Table 13 of AN-2015/321
   * @return True if electron passes selection; false otherwise
   */
  bool operator()(const RecoElectron& electron) const;

 protected: 
  bool apply_offline_e_trigger_cuts_;
  bool debug_;

  Double_t min_pt_;                   ///< lower cut threshold on pT
  Double_t max_absEta_;               ///< upper cut threshold on absolute value of eta
  Double_t max_dxy_;                  ///< upper cut threshold on d_{xy}, distance in the transverse plane w.r.t PV
  Double_t max_dz_;                   ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  Double_t max_relIso_;               ///< upper cut threshold on relative isolation
  Double_t max_sip3d_;                ///< upper cut threshold on significance of IP
//--- define cuts that dependent on eta
//    format: central region (|eta| < 0.8) / transition region (0.8 < |eta| < 1.479) / forward region (|eta| > 1.479)
  typedef std::vector<Double_t> vDouble_t;
  vDouble_t min_mvaRawPOG_;           ///< upper cut threshold on EGamma POG electron MVA value
  vDouble_t binning_absEta_;          ///< eta values separating central, transition and forward region (0.8, 1.479)
  Double_t min_pt_trig_;              ///< lower pT threshold for applying shower shape cuts (to mimic selection applied on trigger level)
  vDouble_t max_sigmaEtaEta_trig_;    ///< upper cut threshold on second shower moment in eta-direction 
  vDouble_t max_HoE_trig_;            ///< upper cut threshold on ratio of energy deposits in hadronic/electromagnetic section of calorimeter
  vDouble_t max_deltaEta_trig_;       ///< upper cut threshold on difference in eta between impact position of track and electron cluster
  vDouble_t max_deltaPhi_trig_;       ///< upper cut threshold on difference in phi between impact position of track and electron cluster
  Double_t min_OoEminusOoP_trig_;     ///< lower cut threshold on difference between calorimeter energy and track momentum (1/E - 1/P)
  vDouble_t max_OoEminusOoP_trig_;    ///< upper cut threshold on difference between calorimeter energy and track momentum (1/E - 1/P)
//-------------------------------------------------------------------------------
  Double_t max_jetBtagCSV_;           ///< upper cut threshold on CSV b-tagging discriminator value of nearby jet
  bool apply_tightCharge_;            ///< apply (True) or do not apply (False) tight charge cut
  bool apply_conversionVeto_;         ///< apply (True) or do not apply (False) conversion veto
  Int_t max_nLostHits_;               ///< upper cut threshold on lost hits in the innermost layer of the tracker (electrons with lost_hits equal to cut threshold pass) 
  Double_t min_mvaTTH_;               ///< lower cut threshold on lepton MVA of ttH multilepton analysis
};

class RecoElectronCollectionSelectorTight
{
 public:
  RecoElectronCollectionSelectorTight(int index = -1, bool debug = false)
    : selIndex_(index)
    , selector_(index, debug)
  {}
  ~RecoElectronCollectionSelectorTight() {}

  // enable/disable cuts on electron ID variables to mimic electron ID cuts applied by single electron trigger 
  void enable_offline_e_trigger_cuts() { selector_.enable_offline_e_trigger_cuts(); }
  void disable_offline_e_trigger_cuts() { selector_.disable_offline_e_trigger_cuts(); }

  // enable/disable tight charge cut
  void enable_tightCharge_cut() { selector_.enable_tightCharge_cut(); }
  void disable_tightCharge_cut() { selector_.disable_tightCharge_cut(); }

  // enable/disable photon conversion veto
  void enable_conversionVeto() { selector_.enable_conversionVeto(); }
  void disable_conversionVeto() { selector_.disable_conversionVeto(); }

  std::vector<const RecoElectron*> operator()(const std::vector<const RecoElectron*>& electrons) const
  {
    std::vector<const RecoElectron*> selElectrons;
    int idx = 0;
    for ( typename std::vector<const RecoElectron*>::const_iterator electron = electrons.begin();
	  electron != electrons.end(); ++electron ) {
      if ( (idx == selIndex_ || selIndex_ == -1) && selector_(**electron) ) {
	selElectrons.push_back(*electron);
	++idx;
      }
    }
    return selElectrons;
  }
  
 protected: 
  int selIndex_;
  RecoElectronSelectorTight selector_;
};

#endif // tthAnalysis_HiggsToTauTau_RecoElectronCollectionSelectorTight_h

