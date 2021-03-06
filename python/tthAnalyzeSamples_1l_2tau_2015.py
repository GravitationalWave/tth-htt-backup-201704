from collections import OrderedDict as OD

from tthAnalyzeSamples_2015 import samples_2015

for sample_name, sample_info in samples_2015.items():
  if sample_info["type"] == "mc":
    sample_info["triggers"] = [ "1e", "1mu" ]
  if sample_name in [
      "/DoubleMuon/Run2015C_25ns-16Dec2015-v1/MINIAOD",
      "/DoubleMuon/Run2015D-16Dec2015-v1/MINIAOD",
      "/DoubleEG/Run2015C_25ns-16Dec2015-v1/MINIAOD",
      "/DoubleEG/Run2015D-16Dec2015-v2/MINIAOD",
      "/MuonEG/Run2015C_25ns-16Dec2015-v1/MINIAOD", 
      "/MuonEG/Run2015D-16Dec2015-v1/MINIAOD" ]:
    sample_info["use_it"] = False

