import FWCore.ParameterSet.Config as cms
import MyAnalysis.IsolationTools.muonPFIsoSingleType_cfi

# Charged Hadron isolation
muonPFIsoChHad = MyAnalysis.IsolationTools.muonPFIsoSingleType_cfi.muonPFIsoSingleTypeMapProd.clone()
muonPFIsoChHad.pfTypes = cms.untracked.vint32(1)

# Neutral Hadron isolation
muonPFIsoNHad = MyAnalysis.IsolationTools.muonPFIsoSingleType_cfi.muonPFIsoSingleTypeMapProd.clone()
muonPFIsoNHad.pfTypes = cms.untracked.vint32(5)

# Photon isolation
muonPFIsoPhoton = MyAnalysis.IsolationTools.muonPFIsoSingleType_cfi.muonPFIsoSingleTypeMapProd.clone()
muonPFIsoPhoton.pfTypes = cms.untracked.vint32(4)

