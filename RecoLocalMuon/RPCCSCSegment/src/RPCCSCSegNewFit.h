#ifndef RPCCSCSegment_RPCCSCSegNewFit_h
#define RPCCSCSegment_RPCCSCSegNewFit_h

//
// This code is based on GEMCSCSegFit code
//

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/MuonDetId/interface/RPCDetId.h>

#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/RPCGeometry/interface/RPCGeomServ.h>
#include <Geometry/RPCGeometry/interface/RPCGeometry.h>

class RPCCSCSegNewFit {

public:

// TYPES

  // 16 x 16 Symmetric
  typedef ROOT::Math::SMatrix<double,16,16,ROOT::Math::MatRepSym<double,16> > SMatrixSym16;

  // 16 x 4
  typedef ROOT::Math::SMatrix<double,16,4 > SMatrix16by4;

  // 4 x 4 General + Symmetric
  typedef ROOT::Math::SMatrix<double, 4 > SMatrix4;
  typedef ROOT::Math::SMatrix<double,4,4,ROOT::Math::MatRepSym<double,4> > SMatrixSym4;

  // 2 x 2 Symmetric
  typedef ROOT::Math::SMatrix<double,2,2,ROOT::Math::MatRepSym<double,2> > SMatrixSym2;

  // 4-dim vector
  typedef ROOT::Math::SVector<double,4> SVector4;


  
 RPCCSCSegNewFit(const std::vector<const TrackingRecHit*> hits,const RPCGeometry* rgeom, const CSCGeometry* cscgeom,const CSCChamber* cscChamber) : hits_( hits ), rgeom_(rgeom), cscgeom_(cscgeom), cscChamber_(cscChamber), scaleXError_( 1.0 ), fitdone_( false ) // usar apenas os hits? - não
    {



    } 


  virtual ~RPCCSCSegNewFit() {}

  // Least-squares fit
  void fit( void ); 

  AlgebraicSymMatrix covarianceMatrix(void);

  void setScaleXError ( double factor ) { scaleXError_ = factor; }

  std::vector<const TrackingRecHit*> hits(void) const { return hits_; }
  const RPCGeometry* rgeom(void) const { return rgeom_; }
  const CSCGeometry* cscgeom(void) const { return cscgeom_; }
  const CSCChamber* cscChamber(void) const { return cscChamber_; }

  double scaleXError(void) const { return scaleXError_; }  
  

  size_t nhits(void) const { return hits_.size(); }
  double chi2(void) const { return chi2_; }
  int ndof(void) const { return ndof_; }
  LocalPoint intercept() const { return intercept_;}
  LocalVector localdir() const { return localdir_;}  
  bool fitdone() const { return fitdone_; }
  
  private:  
  
  // PRIVATE FUNCTIONS
  void fitlsq(void);  // least-squares fit for 3-6 hits  
  void setChi2(void); // fill chi2_ & ndof_ @@ FKA fillChiSquared()


 protected:

  // PROTECTED FUNCTIONS - derived class needs access

 // Set segment direction 'out' from IP
  void setOutFromIP(void); // fill localdir_  @@ FKA fillLocalDirection()
  SMatrixSym16 weightMatrix(void);
  SMatrix16by4 derivativeMatrix(void);
  AlgebraicSymMatrix flipErrors(const SMatrixSym4&);

  
  // PROTECTED MEMBER VARIABLES - derived class needs access


  std::vector<const TrackingRecHit*> hits_;      //@@ FKA protoSegment
  const RPCGeometry* 				 rgeom_;
  const CSCGeometry* 				 cscgeom_;
  const CSCChamber* 				 cscChamber_;  
  float                              uslope_;    //@@ FKA protoSlope_u
  float                              vslope_;    //@@ FKA protoSlope_v
  LocalPoint                         intercept_; //@@ FKA protoIntercept		
  LocalVector                        localdir_;  //@@ FKA protoDirection
  double                             chi2_;      //@@ FKA protoChi2
  int                                ndof_;      //@@ FKA protoNDF, which was double!!
  double                             scaleXError_;  
  bool                               fitdone_;  
  // order needs to be the same here when filled by the constructor

};
  
#endif  
  
  
