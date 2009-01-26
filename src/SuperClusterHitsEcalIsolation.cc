#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "MyAnalysis/IsolationTools/interface/SuperClusterHitsEcalIsolation.h"

SuperClusterHitsEcalIsolation::SuperClusterHitsEcalIsolation(const EcalRecHitCollection *ebRecHits,
                                                             const EcalRecHitCollection *eeRecHits) {

  m_ebRecHits = ebRecHits;
  m_eeRecHits = eeRecHits;
  
}

float SuperClusterHitsEcalIsolation::getSum(const edm::Event & iEvent, const edm::EventSetup & iSetup, const reco::GsfElectron *gsfEle) {

  m_gsfEle = gsfEle;

  reco::SuperClusterRef sc = m_gsfEle->get<reco::SuperClusterRef>();
  math::XYZPoint caloPosition = sc->position();
  const GlobalPoint point(caloPosition.x(), caloPosition.y() , caloPosition.z());

  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);
  const CaloGeometry* caloGeom = pG.product(); 
  const CaloSubdetectorGeometry* barrelgeom = caloGeom->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
  const CaloSubdetectorGeometry* endcapgeom = caloGeom->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);

  float sum = 0.0;
  // sum all the rechits in the circle
  // not considering rechits belonging to sc
  const  std::vector<DetId> scHitsByDetId = sc->getHitsByDetId();
  if( fabs(sc->eta()) < 1.479 ) {
    sum = collect(point, barrelgeom, scHitsByDetId, *m_ebRecHits);
  } else {
    sum = collect(point, endcapgeom, scHitsByDetId, *m_eeRecHits);
  }

  return sum;

}

float SuperClusterHitsEcalIsolation::collect( const GlobalPoint &caloPosition, const CaloSubdetectorGeometry* subdet, 
                                              const  std::vector<DetId> scHitsByDetId, const EcalRecHitCollection &hits) {
  
  float energySum = 0.0;
  
  CaloSubdetectorGeometry::DetIdSet chosen = subdet->getCells(caloPosition,m_extRadius);
  EcalRecHitCollection::const_iterator j=hits.end();
  
  for (CaloSubdetectorGeometry::DetIdSet::const_iterator i = chosen.begin(), end = chosen.end() ; i != end;  ++i) {  
    j=hits.find(*i);
    
    bool usedInSuperCluster = false;
    std::vector<DetId>::const_iterator scDetId;
    for(scDetId=scHitsByDetId.begin(); scDetId!=scHitsByDetId.end(); ++scDetId) {
      if((*i)==(*scDetId)) {
        usedInSuperCluster=true;
        break;
      }
    }

    if(j != hits.end() && !usedInSuperCluster){

      double energy = j->energy();
      energySum += energy;
      
    }
  }

  return energySum;

}

