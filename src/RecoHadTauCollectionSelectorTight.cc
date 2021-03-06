#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauSelectorTight

#include <cmath> // fabs

RecoHadTauSelectorTight::RecoHadTauSelectorTight(int era, int index, bool debug)
  : RecoHadTauSelectorBase(era, index, debug)
{ 
  min_pt_ = 20.;
  max_absEta_ = 2.3;
  max_dz_ = 0.2;
  min_decayModeFinding_ = 1;
  set("dR03mvaTight"); 
  min_antiElectron_ = -1000;
  min_antiMuon_ = -1000;
}
