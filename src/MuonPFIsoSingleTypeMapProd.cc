#include <memory>
#include "Math/VectorUtil.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "TMath.h"

class MuonPFIsoSingleTypeMapProd : public edm::EDProducer {
public:
  explicit MuonPFIsoSingleTypeMapProd(const edm::ParameterSet&);
  ~MuonPFIsoSingleTypeMapProd();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag vtxLabel_;
  edm::InputTag muonLabel_;
  edm::InputTag pfLabel_;
  std::vector<int> pfTypes_;
  double deltaR_;
  double directional_;

};



MuonPFIsoSingleTypeMapProd::MuonPFIsoSingleTypeMapProd(const edm::ParameterSet& iConfig) :
  vtxLabel_(iConfig.getUntrackedParameter<edm::InputTag>("vtxLabel")),
  muonLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonLabel")),
  pfLabel_(iConfig.getUntrackedParameter<edm::InputTag>("pfLabel")),
  pfTypes_(iConfig.getUntrackedParameter<std::vector<int> >("pfTypes")),
  deltaR_(iConfig.getUntrackedParameter<double>("deltaR")),
  directional_(iConfig.getUntrackedParameter<bool>("directional")) {

  produces<edm::ValueMap<float> >().setBranchAlias("pfMuIso");
}

void MuonPFIsoSingleTypeMapProd::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::MuonCollection> muH;
  iEvent.getByLabel(muonLabel_,muH);

  edm::Handle<reco::PFCandidateCollection> pfH;
  iEvent.getByLabel(pfLabel_,pfH);

  edm::Handle<reco::VertexCollection> vtxH;
  iEvent.getByLabel(vtxLabel_,vtxH);

  std::vector<float> isoV;
  std::auto_ptr<edm::ValueMap<float> > isoM(new edm::ValueMap<float> ());
  edm::ValueMap<float>::Filler isoF(*isoM);

  for(size_t i=0; i<muH->size();++i) {
    const reco::Muon &mu = muH->at(i);

    Double_t zLepton = 0.0;
    if(mu.track().isNonnull()) zLepton = mu.track()->dz(vtxH->at(0).position());

    Double_t ptSum =0.;  
    math::XYZVector isoAngleSum;
    std::vector<math::XYZVector> coneParticles;

    for (size_t j=0; j<pfH->size();j++) {   
      const reco::PFCandidate &pf = pfH->at(j);
            
      // consider only the types requested
      bool skip=true;
      for(size_t t=0; t<pfTypes_.size(); t++) {
        if(pf.particleId() == pfTypes_[t]) {
          skip=false;
          break;
        }
      }
      if(skip) continue;

      // exclude muon
      if(pf.trackRef().isNonnull() && mu.track().isNonnull() &&
         pf.trackRef() == mu.track()) continue;


      // pt cut applied to neutrals
      // if(!pf.trackRef().isNonnull() && pf.pt() <= 1.0) continue;
            
      // ignore the pf candidate if it is too far away in Z
      //             Double_t deltaZ = 0.0;
      //             if(pf.trackRef().isNonnull()) {
      //                 deltaZ = fabs(pf.trackRef()->dz(vtxH->at(0).position()) - zLepton);
      //             }
      //             if (deltaZ >= 0.1) continue;
            
      // dR Veto for Gamma: no-one in EB, dR > 0.08 in EE
      Double_t dr = ROOT::Math::VectorUtil::DeltaR(mu.momentum(), pf.momentum());
      if (pf.particleId() == reco::PFCandidate::gamma && fabs(mu.eta()>1.479) 
          && dr < 0.0) continue;

      // add the pf pt if it is inside the extRadius 
      if ( dr < deltaR_ ) {

        // scalar sum
        ptSum += pf.pt();

        // directional sum
        math::XYZVector transverse( pf.eta() - mu.eta()
                                    , reco::deltaPhi(pf.phi(), mu.phi())
                                    , 0);
        transverse *= pf.pt() / transverse.rho();
        if (transverse.rho() > 0) {
          isoAngleSum += transverse;
          coneParticles.push_back(transverse);
        }
      }

    }
    if (directional_) {
      double directionalPT = 0;
      for (unsigned int iPtcl = 0; iPtcl < coneParticles.size(); ++iPtcl)
        directionalPT += pow(TMath::ACos( coneParticles[iPtcl].Dot(isoAngleSum) / coneParticles[iPtcl].rho() / isoAngleSum.rho() ),2) * coneParticles[iPtcl].rho();
      isoV.push_back(directionalPT);
    } else isoV.push_back(ptSum);
  }

  isoF.insert(muH,isoV.begin(),isoV.end());

  isoF.fill();
  iEvent.put(isoM);

}

MuonPFIsoSingleTypeMapProd::~MuonPFIsoSingleTypeMapProd() { }
void MuonPFIsoSingleTypeMapProd::beginJob() { }
void MuonPFIsoSingleTypeMapProd::endJob() { }
DEFINE_FWK_MODULE(MuonPFIsoSingleTypeMapProd);
