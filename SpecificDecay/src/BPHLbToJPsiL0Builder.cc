/*
 *  See header file for a description of this class.
 *
 *  \author Paolo Ronchese INFN Padova
 *
 */

//-----------------------
// This Class' Header --
//-----------------------
#include "HeavyFlavorAnalysis/SpecificDecay/interface/BPHLbToJPsiL0Builder.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHRecoBuilder.h"
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHPlusMinusCandidate.h"
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHRecoCandidate.h"
#include "HeavyFlavorAnalysis/SpecificDecay/interface/BPHMassSelect.h"
#include "HeavyFlavorAnalysis/SpecificDecay/interface/BPHChi2Select.h"
#include "HeavyFlavorAnalysis/SpecificDecay/interface/BPHMassFitSelect.h"
#include "HeavyFlavorAnalysis/SpecificDecay/interface/BPHParticleMasses.h"

#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"

//---------------
// C++ Headers --
//---------------
using namespace std;

//-------------------
// Initializations --
//-------------------


//----------------
// Constructors --
//----------------
BPHLbToJPsiL0Builder::BPHLbToJPsiL0Builder( const edm::EventSetup& es,
    const std::vector<BPHPlusMinusConstCandPtr>& jpsiCollection,
    const std::vector<BPHPlusMinusConstCandPtr>&   l0Collection ):
  jPsiName(    "JPsi" ),
    l0Name( "Lambda0" ),
  evSetup( &es ),
  jCollection( &jpsiCollection ),
  lCollection( &  l0Collection ) {
  jpsiSel = new BPHMassSelect   ( 2.80, 3.40 );
   ml0Sel = new BPHMassFitSelect( 0.00, 3.00 );
  massSel = new BPHMassSelect   ( 3.50, 8.00 );
  probMin = 0.02;
  mFitSel = new BPHMassFitSelect( jPsiName,
                                  BPHParticleMasses::jPsiMass,
                                  BPHParticleMasses::jPsiMWidth,
                                  5.00, 6.00 );	
  massConstr = true;
  minPDiff = 1.0e-4;
  updated = false;
}

//--------------
// Destructor --
//--------------
BPHLbToJPsiL0Builder::~BPHLbToJPsiL0Builder() {
  delete jpsiSel;
  delete  ml0Sel;
  delete massSel;
  delete mFitSel;
}

//--------------
// Operations --
//--------------
/// build Bs candidates
vector<BPHRecoConstCandPtr> BPHLbToJPsiL0Builder::build() {

  if ( updated ) return lbList;

  lbList.clear();
  dMap.clear();

  BPHRecoBuilder bLb( *evSetup );
  bLb.setMinPDiffererence( minPDiff );
  bLb.add( jPsiName, *jCollection );
  bLb.add(   l0Name, *lCollection );

  bLb.filter( jPsiName, *jpsiSel );
  bLb.filter(   l0Name,  *ml0Sel );

  bLb.filter( *massSel );

  vector<BPHRecoConstCandPtr> tmpList = BPHRecoCandidate::build( bLb );
//
//  Apply kinematic constraint on the JPsi mass.
//
  int iLb;
  int nLb = tmpList.size();
  lbList.reserve( nLb );
  for ( iLb = 0; iLb < nLb; ++iLb ) {
    BPHRecoConstCandPtr ctmp = tmpList[iLb];
    BPHRecoCandidate* cptr = const_cast<BPHRecoCandidate*>( ctmp->clone() );
    BPHRecoConstCandPtr cand( cptr );
    cptr->setIndependentFit( l0Name );    // fit for Lambda0 reconstruction
                                          // independent from other particles
    BPHRecoCandidate* lptr = const_cast<BPHRecoCandidate*>(
                      cptr->getComp( l0Name ).get() );
    lptr->setConstraint( BPHParticleMasses::lambda0Mass,
                         BPHParticleMasses::lambda0MSigma );
    cptr->resetKinematicFit();
    if ( !mFitSel->accept( *cptr ) ) continue;
    const RefCountedKinematicVertex tdv = cptr->topDecayVertex();
    if ( tdv.get() == 0 ) continue;
    if ( !tdv->vertexIsValid() ) continue;
    reco::Vertex vtx( *tdv );
    if ( TMath::Prob( vtx.chi2(), lround( vtx.ndof() ) ) < probMin ) continue;
    dMap[cand->getComp( jPsiName ).get()] = ctmp->getComp( jPsiName ).get();
    dMap[cand->getComp(   l0Name ).get()] = ctmp->getComp(   l0Name ).get();
    lbList.push_back( cand );
  }
  updated = true;

  return lbList;

}

/// get original daughters map
const std::map<const BPHRecoCandidate*,
               const BPHRecoCandidate*>&
               BPHLbToJPsiL0Builder::daughMap() const {
  return dMap;
}

/// set cuts
void BPHLbToJPsiL0Builder::setJPsiMassMin( double m ) {
  updated = false;
  jpsiSel->setMassMin( m );
  return;
}


void BPHLbToJPsiL0Builder::setJPsiMassMax( double m ) {
  updated = false;
  jpsiSel->setMassMax( m );
  return;
}


void BPHLbToJPsiL0Builder::setLambda0MassMin( double m ) {
  updated = false;
  ml0Sel->setMassMin( m );
  return;
}


void BPHLbToJPsiL0Builder::setLambda0MassMax( double m ) {
  updated = false;
  ml0Sel->setMassMax( m );
  return;
}


void BPHLbToJPsiL0Builder::setMassMin( double m ) {
  updated = false;
  massSel->setMassMin( m );
  return;
}


void BPHLbToJPsiL0Builder::setMassMax( double m ) {
  updated = false;
  massSel->setMassMax( m );
  return;
}


void BPHLbToJPsiL0Builder::setProbMin( double p ) {
  updated = false;
  probMin = p;
  return;
}


void BPHLbToJPsiL0Builder::setMassFitMin( double m ) {
  updated = false;
  mFitSel->setMassMin( m );
  return;
}


void BPHLbToJPsiL0Builder::setMassFitMax( double m ) {
  updated = false;
  mFitSel->setMassMax( m );
  return;
}


void BPHLbToJPsiL0Builder::setConstr( bool flag ) {
  updated = false;
  massConstr = flag;
  return;
}

/// get current cuts
double BPHLbToJPsiL0Builder::getJPsiMassMin() const {
  return jpsiSel->getMassMin();
}


double BPHLbToJPsiL0Builder::getJPsiMassMax() const {
  return jpsiSel->getMassMax();
}


double BPHLbToJPsiL0Builder::getLambda0MassMin() const {
  return ml0Sel->getMassMin();
}


double BPHLbToJPsiL0Builder::getLambda0MassMax() const {
  return ml0Sel->getMassMax();
}


double BPHLbToJPsiL0Builder::getMassMin() const {
  return massSel->getMassMin();
}


double BPHLbToJPsiL0Builder::getMassMax() const {
  return massSel->getMassMax();
}


double BPHLbToJPsiL0Builder::getProbMin() const {
  return probMin;
}


double BPHLbToJPsiL0Builder::getMassFitMin() const {
  return mFitSel->getMassMin();
}


double BPHLbToJPsiL0Builder::getMassFitMax() const {
  return mFitSel->getMassMax();
}


bool BPHLbToJPsiL0Builder::getConstr() const {
  return massConstr;
}

