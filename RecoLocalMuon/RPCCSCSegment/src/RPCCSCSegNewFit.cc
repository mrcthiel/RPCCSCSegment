//
// This code is based on GEMCSCSegFit code
//

#include "RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegNewFit.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
// rpc includes

#include "Geometry/CSCGeometry/interface/CSCLayer.h"
// rpc includes

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

void RPCCSCSegNewFit::fit(void) {
  if ( fitdone() ) return; // don't redo fit unnecessarily
  fitlsq();
}

void RPCCSCSegNewFit::fitlsq(void) {

  SMatrix4 M; // 4x4, init to 0
  SVector4 B; // 4x1, init to 0; 

  std::vector<const TrackingRecHit*>::const_iterator ih = hits_.begin();

  // Loop over the TrackingRecHits and make small (2x2) matrices used to fill the blockdiagonal covariance matrix E^-1
  for (ih = hits_.begin(); ih != hits_.end(); ++ih) 
    {
      const TrackingRecHit& hit = (**ih);
      GlobalPoint gp;
      DetId d = DetId(hit.rawId());
      if(d.subdetId() == MuonSubdetId::RPC) // incluir a RPC aqui!  - feito
	{
//		RPCDetId rpcRecHit_id = hit.rpcId();
		RPCDetId rpcRecHit_id = hit.rawId();
		const RPCRoll* rollasociated = rgeom_->roll(rpcRecHit_id); // o problema agora é o rgeom_-> vou ter que carregar
		const BoundPlane & RPCSurface = rollasociated->surface();
		gp = RPCSurface.toGlobal(LocalPoint(hit.localPosition().x(),hit.localPosition().y(),hit.localPosition().z()));
	}

      else if(d.subdetId() == MuonSubdetId::CSC) 
	{
//	  const CSCLayer* layer = cscchamber(CSCDetId(d))->layer(CSCDetId(d).layer()); // trocar aqui - acho que vou precisar da geometria tambem... - talvez nao - feito?!
//        const CSCLayer* csclayer = geom_->layer(id); RecoLocalMuon/CSCSegment/test/CSCSegmentVisualise.cc
// RecoLocalMuon/CSCSegment/src/CSCSegmentBuilder.cc
      DetId hitId = hit.geographicalId();
	  const CSCDetId cscDetId(hitId.rawId());
	  //const CSCLayer* layer = cscDetId.layer(); vou ter que trazer a geometria aqui
	  const CSCLayer* layer = cscgeom_->layer(cscDetId);
	  gp = layer->toGlobal(hit.localPosition());
	}

      LocalPoint lp = cscChamber_->toLocal(gp);//hit.localPosition();//refcscchamber()->toLocal(gp); // trocar aqui - feito
      
      // Local position of hit w.r.t. chamber
      double u = lp.x();
      double v = lp.y();
      double z = lp.z();
      
      // Covariance matrix of local errors 
      SMatrixSym2 IC; // 2x2, init to 0
      
      IC(0,0) = hit.localPositionError().xx();
      IC(1,1) = hit.localPositionError().yy();
      //@@ NOT SURE WHICH OFF-DIAGONAL ELEMENT MUST BE DEFINED BUT (1,0) WORKS
      //@@ (and SMatrix enforces symmetry)
      IC(1,0) = hit.localPositionError().xy();
      // IC(0,1) = IC(1,0);

      
      // Invert covariance matrix (and trap if it fails!)
      bool ok = IC.Invert();
      if ( !ok ) 
	{
	  return; // MATRIX INVERSION FAILED ... QUIT VOID FUNCTION
	}

      // M = (AT E A)
      // B = (AT E m)
      // for now fill only with sum of blockdiagonal 
      // elements of (E^-1)_i = IC_i for hit i
      M(0,0) += IC(0,0);
      M(0,1) += IC(0,1);
      M(0,2) += IC(0,0) * z;
      M(0,3) += IC(0,1) * z;
      B(0)   += u * IC(0,0) + v * IC(0,1);
      
      M(1,0) += IC(1,0);
      M(1,1) += IC(1,1);
      M(1,2) += IC(1,0) * z;
      M(1,3) += IC(1,1) * z;
      B(1)   += u * IC(1,0) + v * IC(1,1);
      
      M(2,0) += IC(0,0) * z;
      M(2,1) += IC(0,1) * z;
      M(2,2) += IC(0,0) * z * z;
      M(2,3) += IC(0,1) * z * z;
      B(2)   += ( u * IC(0,0) + v * IC(0,1) ) * z;
      
      M(3,0) += IC(1,0) * z;
      M(3,1) += IC(1,1) * z;
      M(3,2) += IC(1,0) * z * z;
      M(3,3) += IC(1,1) * z * z;
      B(3)   += ( u * IC(1,0) + v * IC(1,1) ) * z;
      
    } // End Loop over the TrackingRecHits to make the block matrices to be filled in M and B
  
  SVector4 p;
  bool ok = M.Invert();
  if (!ok )
    {
      return; // MATRIX INVERSION FAILED ... QUIT VOID FUNCTION 
    }
  else 
    {
      p = M * B;
    }

  // fill member variables  (note origin has local z = 0)
  //  intercept_
  intercept_ = LocalPoint(p(0), p(1), 0.);
  
  // localdir_ - set so segment points outwards from IP
  uslope_ = p(2);
  vslope_ = p(3);
  setOutFromIP();
  
  // calculate chi2 of fit
  setChi2( );

  // flag fit has been done
  fitdone_ = true;

}



void RPCCSCSegNewFit::setChi2(void) {
  
  double chsq = 0.;
  //bool rpc = false; // incluir a RPC aqui

  std::vector<const TrackingRecHit*>::const_iterator ih;
  for (ih = hits_.begin(); ih != hits_.end(); ++ih) {

    const TrackingRecHit& hit = (**ih);
    GlobalPoint gp;
    DetId d = DetId(hit.rawId());
    if(d.subdetId() == MuonSubdetId::RPC) {  // incluir a RPC a partir daqui - feito
//		RPCDetId rpcRecHit_id = hit.rpcId();
		RPCDetId rpcRecHit_id = hit.rawId();
		const RPCRoll* rollasociated = rgeom_->roll(rpcRecHit_id); // o problema agora é o rgeom_-> vou ter que carregar
		const BoundPlane & RPCSurface = rollasociated->surface();
		gp = RPCSurface.toGlobal(LocalPoint(hit.localPosition().x(),hit.localPosition().y(),hit.localPosition().z()));
        //rpc = true;
    }
    else if(d.subdetId() == MuonSubdetId::CSC) {
//      const CSCLayer* layer = cscchamber(CSCDetId(d))->layer(CSCDetId(d).layer()); // trocar aqui  - Feito?!
//      gp = layer->toGlobal(hit.localPosition());

      DetId hitId = hit.geographicalId();
	  const CSCDetId cscDetId(hitId.rawId());

	  //const CSCLayer* layer = cscDetId.layer();
	  const CSCLayer* layer = cscgeom_->layer(cscDetId);
	  gp = layer->toGlobal(hit.localPosition());
      //rpc = false;
    }
    LocalPoint lp = cscChamber_->toLocal(gp);//hit.localPosition();//refcscchamber()->toLocal(gp); // trocar aqui  - feito?!
    
    double u = lp.x();
    double v = lp.y();
    double z = lp.z();
    
    double du = intercept_.x() + uslope_ * z - u;
    double dv = intercept_.y() + vslope_ * z - v;
    

    SMatrixSym2 IC; // 2x2, init to 0

    IC(0,0) = hit.localPositionError().xx();
    //    IC(0,1) = hit.localPositionError().xy();
    IC(1,0) = hit.localPositionError().xy();
    IC(1,1) = hit.localPositionError().yy();
    //    IC(1,0) = IC(0,1);

    // Invert covariance matrix
    bool ok = IC.Invert();
    if (!ok ){
      return; // MATRIX INVERSION FAILED ... QUIT VOID FUNCTION
    }
    chsq += du*du*IC(0,0) + 2.*du*dv*IC(0,1) + dv*dv*IC(1,1);

  }

  // fill member variables
  chi2_ = chsq;
  ndof_ = 2.*hits_.size() - 4;

}


void RPCCSCSegNewFit::setOutFromIP() {
  // Set direction of segment to point from IP outwards
  // (Incorrect for particles not coming from IP, of course.)
  
  double dxdz = uslope_;
  double dydz = vslope_;
  double dz   = 1./sqrt(1. + dxdz*dxdz + dydz*dydz);
  double dx   = dz*dxdz;
  double dy   = dz*dydz;
  LocalVector localDir(dx,dy,dz);

  // localDir sometimes needs a sign flip 
  // Examine its direction and origin in global z: to point outward
  // the localDir should always have same sign as global z...
  
  //double globalZpos    = ( refcscchamber()->toGlobal( intercept_ ) ).z(); // trocar aqui
  //double globalZdir    = ( refcscchamber()->toGlobal( localDir  ) ).z(); // trocar aqui
  double globalZpos    = ( cscChamber_->toGlobal( intercept_ ) ).z(); // trocar aqui - Feito?!
  double globalZdir    = ( cscChamber_->toGlobal( localDir  ) ).z(); // trocar aqui - Feito?!

  double directionSign = globalZpos * globalZdir;
  localdir_ = (directionSign * localDir ).unit();
}


AlgebraicSymMatrix RPCCSCSegNewFit::covarianceMatrix() {
  
  SMatrixSym16 weights = weightMatrix();
  SMatrix16by4 A = derivativeMatrix();

  bool ok;
  SMatrixSym4 result =  ROOT::Math::SimilarityT(A, weights);
  ok = result.Invert(); // inverts in place
  if ( !ok ) {
    AlgebraicSymMatrix emptymatrix(4, 0. );
    return emptymatrix; // return empty matrix if matrix inversion failed
  }  
  // reorder components to match TrackingRecHit interface (GEMCSCSegment isa TrackingRecHit)
  // i.e. slopes first, then positions 
  AlgebraicSymMatrix flipped = flipErrors( result );
    
  return flipped;
}


RPCCSCSegNewFit::SMatrixSym16 RPCCSCSegNewFit::weightMatrix() {
  
  bool ok = true;
  SMatrixSym16 matrix = ROOT::Math::SMatrixIdentity(); 
  int row = 0;
  
  for (std::vector<const TrackingRecHit*>::const_iterator it = hits_.begin(); it != hits_.end(); ++it) 
    {
   
      const TrackingRecHit& hit = (**it);
      matrix(row, row)   = scaleXError()*hit.localPositionError().xx();
      matrix(row, row+1) = hit.localPositionError().xy();
      ++row;
      matrix(row, row-1) = hit.localPositionError().xy();
      matrix(row, row)   = hit.localPositionError().yy();
      ++row;
    }

  ok = matrix.Invert(); // invert in place
  if ( !ok ) 
    {
      SMatrixSym16 emptymatrix = ROOT::Math::SMatrixIdentity();
      return emptymatrix; // return (empty) identity matrix if matrix inversion failed 
    }
  return matrix;
}


RPCCSCSegNewFit::SMatrix16by4 RPCCSCSegNewFit::derivativeMatrix() {
  
  SMatrix16by4 matrix; // 16x4, init to 0
  int row = 0;
  
  for(std::vector<const TrackingRecHit*>::const_iterator it = hits_.begin(); it != hits_.end(); ++it) {
    
    const TrackingRecHit& hit = (**it);
    GlobalPoint gp;
    DetId d = DetId(hit.rawId());
      if(d.subdetId() == MuonSubdetId::RPC) // incluir a RPC aqui!  - feito
	{
		RPCDetId rpcRecHit_id = hit.rawId();
		const RPCRoll* rollasociated = rgeom_->roll(rpcRecHit_id); // o problema agora é o rgeom_-> vou ter que carregar
		const BoundPlane & RPCSurface = rollasociated->surface();
		gp = RPCSurface.toGlobal(LocalPoint(hit.localPosition().x(),hit.localPosition().y(),hit.localPosition().z()));
	}
      else if(d.subdetId() == MuonSubdetId::CSC) 
	{
      DetId hitId = hit.geographicalId();
	  const CSCDetId cscDetId(hitId.rawId());
	  const CSCLayer* layer = cscgeom_->layer(cscDetId);
	  gp = layer->toGlobal(hit.localPosition());
	}

    LocalPoint lp = cscChamber_->toLocal(gp);//refcscchamber()->toLocal(gp);
    float z = lp.z();

    matrix(row, 0) = 1.;
    matrix(row, 2) = z;
    ++row;
    matrix(row, 1) = 1.;
    matrix(row, 3) = z;
    ++row;
  }
  return matrix;
}


AlgebraicSymMatrix RPCCSCSegNewFit::flipErrors( const SMatrixSym4& a ) { 
    
  // The GEMCSCSegment needs the error matrix re-arranged to match
  // parameters in order (uz, vz, u0, v0) 
  // where uz, vz = slopes, u0, v0 = intercepts
    
  AlgebraicSymMatrix hold(4, 0. ); 
      
  for ( short j=0; j!=4; ++j) {
    for (short i=0; i!=4; ++i) {
      hold(i+1,j+1) = a(i,j); // SMatrix counts from 0, AlgebraicMatrix from 1
    }
  }


  // errors on slopes into upper left 
  hold(1,1) = a(2,2); 
  hold(1,2) = a(2,3); 
  hold(2,1) = a(3,2); 
  hold(2,2) = a(3,3); 
    
  // errors on positions into lower right 
  hold(3,3) = a(0,0); 
  hold(3,4) = a(0,1); 
  hold(4,3) = a(1,0); 
  hold(4,4) = a(1,1); 
    
  // must also interchange off-diagonal elements of off-diagonal 2x2 submatrices
  hold(4,1) = a(1,2);
  hold(3,2) = a(0,3);
  hold(2,3) = a(3,0); // = a(0,3)
  hold(1,4) = a(2,1); // = a(1,2)

  return hold;
}
