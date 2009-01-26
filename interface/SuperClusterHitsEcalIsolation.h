#ifndef SuperClusterHitsEcalIsolation_h
#define SuperClusterHitsEcalIsolation_h

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"


class SuperClusterHitsEcalIsolation {
 public:

  //! ctor
  SuperClusterHitsEcalIsolation() { };
  SuperClusterHitsEcalIsolation(const EcalRecHitCollection *ebRecHits,
                                const EcalRecHitCollection *eeRecHits);

  //! dtor
  ~SuperClusterHitsEcalIsolation() { }

  //! the radius of the external cone 
  void setExtRadius (float extRadius) { m_extRadius = extRadius; }

  //! add n more rows of crystals around the supercluster
  // void addCrystalStrips(int nstrips) { m_additionalStrips = nstrips; }

  //! get the sum of the energies
  float getSum(const edm::Event & iEvent, const edm::EventSetup & iSetup, const reco::GsfElectron *gsfEle);

 private:

  float collect( const GlobalPoint &caloPosition, const CaloSubdetectorGeometry* subdet,
                 const  std::vector<DetId> scHitsByDetId, const EcalRecHitCollection &hits);

  const reco::GsfElectron *m_gsfEle;
  const EcalRecHitCollection *m_ebRecHits;
  const EcalRecHitCollection *m_eeRecHits;

  float m_extRadius;

};

#endif
