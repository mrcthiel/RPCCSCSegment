#ifndef RPCCSCSegment_RPCCSCSegAlgoPreClustering_h
#define RPCCSCSegment_RPCCSCSegAlgoPreClustering_h
/**
 * \file RPCCSCSegAlgoPreClustering.h
 *
 *  \authors: S. Stoynev  - NU
 *            I. Bloch    - FNAL
 *            E. James    - FNAL
 *
 * See header file for description.
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/RPCRecHit/interface/RPCCSCSegment.h>
#include <vector>

class CSCChamber;

class RPCCSCSegAlgoPreClustering {

 public:

  typedef std::vector<const CSCRecHit2D*> ChamberHitContainer;

  /// constructor
  explicit RPCCSCSegAlgoPreClustering(const edm::ParameterSet& ps);

  /// destructor
  ~RPCCSCSegAlgoPreClustering();

  /// clusterize
  std::vector< std::vector<const CSCRecHit2D*> > clusterHits( const CSCChamber* aChamber, const ChamberHitContainer& rechits);
 private:

  bool    debug;
  double  dXclusBoxMax;
  double  dYclusBoxMax;

  float mean_x, mean_y, err_x, err_y;
  const CSCChamber* theChamber;

};
#endif
