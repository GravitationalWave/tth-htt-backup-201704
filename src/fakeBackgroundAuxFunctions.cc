#include "tthAnalysis/HiggsToTauTau/interface/fakeBackgroundAuxFunctions.h"

double getWeight_2L(double prob_fake_lead, bool passesTight_lead,
		    double prob_fake_sublead, bool passesTight_sublead)
{
  double p1 = prob_fake_lead/(1. - prob_fake_lead);
  double p2 = prob_fake_sublead/(1. - prob_fake_sublead);
  double weight = 0.;
  if      ( !passesTight_lead &&  passesTight_sublead ) weight =  p1;
  else if (  passesTight_lead && !passesTight_sublead ) weight =  p2;
  else if ( !passesTight_lead && !passesTight_sublead ) weight = -p1*p2;
  return weight;
}

double getWeight_3L(double prob_fake_lead, bool passesTight_lead,
		    double prob_fake_sublead, bool passesTight_sublead,
		    double prob_fake_third, bool passesTight_third) 
{
  double p1 = prob_fake_lead/(1. - prob_fake_lead);
  double p2 = prob_fake_sublead/(1. - prob_fake_sublead);
  double p3 = prob_fake_third/(1. - prob_fake_third);
  double weight = 0.;
  if      ( !passesTight_lead &&  passesTight_sublead &&  passesTight_third ) weight =  p1;
  else if (  passesTight_lead && !passesTight_sublead &&  passesTight_third ) weight =  p2;
  else if (  passesTight_lead &&  passesTight_sublead && !passesTight_third ) weight =  p3;
  else if ( !passesTight_lead && !passesTight_sublead &&  passesTight_third ) weight = -p1*p2;
  else if ( !passesTight_lead &&  passesTight_sublead && !passesTight_third ) weight = -p1*p3;
  else if (  passesTight_lead && !passesTight_sublead && !passesTight_third ) weight = -p2*p3;
  else if ( !passesTight_lead && !passesTight_sublead && !passesTight_third ) weight =  p1*p2*p3;
  return weight;
}

double getWeight_4L(double prob_fake_lead, bool passesTight_lead,
		    double prob_fake_sublead, bool passesTight_sublead,
		    double prob_fake_third, bool passesTight_third,
		    double prob_fake_fourth, bool passesTight_fourth) 
{
  double p1 = prob_fake_lead/(1. - prob_fake_lead);
  double p2 = prob_fake_sublead/(1. - prob_fake_sublead);
  double p3 = prob_fake_third/(1. - prob_fake_third);
  double p4 = prob_fake_fourth/(1. - prob_fake_fourth);
  double weight = 0.;
  if      ( !passesTight_lead &&  passesTight_sublead &&  passesTight_third &&  passesTight_fourth ) weight =  p1;
  else if (  passesTight_lead && !passesTight_sublead &&  passesTight_third &&  passesTight_fourth ) weight =  p2;
  else if (  passesTight_lead &&  passesTight_sublead && !passesTight_third &&  passesTight_fourth ) weight =  p3;
  else if (  passesTight_lead &&  passesTight_sublead &&  passesTight_third && !passesTight_fourth ) weight =  p4;
  else if ( !passesTight_lead && !passesTight_sublead &&  passesTight_third &&  passesTight_fourth ) weight = -p1*p2;
  else if ( !passesTight_lead &&  passesTight_sublead && !passesTight_third &&  passesTight_fourth ) weight = -p1*p3;
  else if ( !passesTight_lead &&  passesTight_sublead &&  passesTight_third && !passesTight_fourth ) weight = -p1*p4;
  else if (  passesTight_lead && !passesTight_sublead && !passesTight_third &&  passesTight_fourth ) weight = -p2*p3;
  else if (  passesTight_lead && !passesTight_sublead &&  passesTight_third && !passesTight_fourth ) weight = -p2*p4;
  else if (  passesTight_lead &&  passesTight_sublead && !passesTight_third && !passesTight_fourth ) weight = -p3*p4;
  else if ( !passesTight_lead && !passesTight_sublead && !passesTight_third &&  passesTight_fourth ) weight =  p1*p2*p3;
  else if ( !passesTight_lead && !passesTight_sublead &&  passesTight_third && !passesTight_fourth ) weight =  p1*p2*p4;
  else if ( !passesTight_lead &&  passesTight_sublead && !passesTight_third && !passesTight_fourth ) weight =  p1*p3*p4;
  else if (  passesTight_lead && !passesTight_sublead && !passesTight_third && !passesTight_fourth ) weight =  p2*p3*p4;
  else if ( !passesTight_lead && !passesTight_sublead && !passesTight_third && !passesTight_fourth ) weight = -p1*p2*p3*p4;
  return weight;
}
