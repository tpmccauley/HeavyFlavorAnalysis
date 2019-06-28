#ifndef HeavyFlavorAnalysis_SpecificDecay_BPHKx0ToKPiBuilder_h
#define HeavyFlavorAnalysis_SpecificDecay_BPHKx0ToKPiBuilder_h
/** \class BPHKx0ToKPiBuilder
 *
 *  Description: 
 *     Class to build K*0 to K+ pi- candidates
 *
 *  \author Paolo Ronchese INFN Padova
 *
 */

//----------------------
// Base Class Headers --
//----------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHRecoBuilder.h"
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHRecoCandidate.h"
#include "HeavyFlavorAnalysis/RecoDecay/interface/BPHPlusMinusCandidate.h"

#include "FWCore/Framework/interface/Event.h"

class BPHParticlePtSelect;
class BPHParticleEtaSelect;
class BPHChi2Select;
class BPHMassSelect;

//---------------
// C++ Headers --
//---------------
#include <string>
#include <vector>

//              ---------------------
//              -- Class Interface --
//              ---------------------

class BPHKx0ToKPiBuilder {

 public:

  /// Definitions
  enum buildMode { posNeg, kaonPion };

  /** Constructor
   */
  BPHKx0ToKPiBuilder( const edm::EventSetup& es,
       const BPHRecoBuilder::BPHGenericCollection* pCollection,
       const BPHRecoBuilder::BPHGenericCollection* nCollection,
       buildMode mode = posNeg );

  /** Destructor
   */
  virtual ~BPHKx0ToKPiBuilder();

  /** Operations
   */
  /// build Phi candidates
  std::vector<BPHPlusMinusConstCandPtr> build();

  /// set cuts
  void setPtMin  ( double pt  );
  void setEtaMax ( double eta );
  void setMassMin( double m   );
  void setMassMax( double m   );
  void setProbMin( double p   );
// mass constraint: foreseen but actually not implemented
  void setConstr ( double mass, double sigma );

  /// get current cuts
  double getPtMin  () const;
  double getEtaMax () const;
  double getMassMin() const;
  double getMassMax() const;
  double getProbMin() const;
// mass constraint: foreseen but actually not implemented
  double getConstrMass () const;
  double getConstrSigma() const;

 private:

  // private copy and assigment constructors
  BPHKx0ToKPiBuilder           ( const BPHKx0ToKPiBuilder& x );
  BPHKx0ToKPiBuilder& operator=( const BPHKx0ToKPiBuilder& x );

  std::string kaonName;
  std::string pionName;

  const edm::EventSetup* evSetup;
  const BPHRecoBuilder::BPHGenericCollection* posCollection;
  const BPHRecoBuilder::BPHGenericCollection* negCollection;

  double ptMin;
  double etaMax;
  double massMin;
  double massMax;
  double probMin;

// old code left for example/reference

//  BPHParticlePtSelect *  ptSel;
//  BPHParticleEtaSelect* etaSel;
//  BPHMassSelect* massSel;
//  BPHChi2Select* chi2Sel;

//  class DZSelect;
//  DZSelect* dzSel;

// mass constraint: foreseen but actually not implemented
  double cMass;
  double cSigma;

  bool updated;

  std::vector<BPHPlusMinusConstCandPtr> kx0List;

};


#endif

