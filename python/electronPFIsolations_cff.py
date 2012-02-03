import FWCore.ParameterSet.Config as cms
import MyAnalysis.IsolationTools.electronPFIsoSingleType_cfi

# Charged Hadron isolation
electronPFIsoChHad = MyAnalysis.IsolationTools.electronPFIsoSingleType_cfi.electronPFIsoSingleTypeMapProd.clone()
electronPFIsoChHad.pfTypes = cms.untracked.vint32(1)

# Neutral Hadron isolation
electronPFIsoNHad = MyAnalysis.IsolationTools.electronPFIsoSingleType_cfi.electronPFIsoSingleTypeMapProd.clone()
electronPFIsoNHad.pfTypes = cms.untracked.vint32(5)

# Photon isolation
electronPFIsoPhoton = MyAnalysis.IsolationTools.electronPFIsoSingleType_cfi.electronPFIsoSingleTypeMapProd.clone()
electronPFIsoPhoton.pfTypes = cms.untracked.vint32(4)

