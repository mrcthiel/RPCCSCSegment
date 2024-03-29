/** \file RPCCSCSegment.cc
 *
 */

#include <DataFormats/RPCRecHit/interface/RPCCSCSegment.h>
#include <iostream>

namespace {
  // Get CSCDetId from one of the rechits, but then remove the layer part so it's a _chamber_ id
  inline
  DetId buildDetId(CSCDetId id) {
    return CSCDetId(id.endcap(),id.station(),id.ring(),id.chamber(), 0);
  }

}

RPCCSCSegment::RPCCSCSegment(const std::vector<const CSCRecHit2D*>& proto_segment, const RPCRecHit *rpcRecHit, LocalPoint origin, 
	LocalVector direction, const AlgebraicSymMatrix& errors, double chi2) : 
  RecSegment(buildDetId(proto_segment.front()->cscDetId())),
  theOrigin(origin), 
  theLocalDirection(direction), theCovMatrix(errors), theChi2(chi2), aME11a_duplicate(false) {

  for(unsigned int i=0; i<proto_segment.size(); ++i)
    theCSCRecHits.push_back(*proto_segment[i]);
}

RPCCSCSegment::~RPCCSCSegment() {}

std::vector<const TrackingRecHit*> RPCCSCSegment::recHits() const{
  std::vector<const TrackingRecHit*> pointersOfRecHits;
  for (std::vector<CSCRecHit2D>::const_iterator irh = theCSCRecHits.begin(); irh!=theCSCRecHits.end(); ++irh) {
    pointersOfRecHits.push_back(&(*irh));
  }
  return pointersOfRecHits;
}

std::vector<TrackingRecHit*> RPCCSCSegment::recHits() {
  
  std::vector<TrackingRecHit*> pointersOfRecHits;
  for (std::vector<CSCRecHit2D>::iterator irh = theCSCRecHits.begin(); irh!=theCSCRecHits.end(); ++irh) {
    pointersOfRecHits.push_back(&(*irh));
  }
  return pointersOfRecHits;
}

LocalError RPCCSCSegment::localPositionError() const {
  return LocalError(theCovMatrix[2][2], theCovMatrix[2][3], theCovMatrix[3][3]);
}

LocalError RPCCSCSegment::localDirectionError() const {
  return LocalError(theCovMatrix[0][0], theCovMatrix[0][1], theCovMatrix[1][1]); 
}


AlgebraicVector RPCCSCSegment::parameters() const {
  // For consistency with DT and what we require for the TrackingRecHit interface,
  // the order of the parameters in the returned vector should be (dx/dz, dy/dz, x, z)
  
  AlgebraicVector result(4);

  result[0] = theLocalDirection.x()/theLocalDirection.z();
  result[1] = theLocalDirection.y()/theLocalDirection.z();    
  result[2] = theOrigin.x();
  result[3] = theOrigin.y();

  return result;
}

AlgebraicMatrix createStaticMatrix()
{
  AlgebraicMatrix m( 4, 5, 0);
  m[0][1] = 1;
  m[1][2] = 1;
  m[2][3] = 1;
  m[3][4] = 1;
  return m;
}

static const AlgebraicMatrix theProjectionMatrix = createStaticMatrix();

AlgebraicMatrix RPCCSCSegment::projectionMatrix() const {
  return theProjectionMatrix;
}

void RPCCSCSegment::setDuplicateSegments(std::vector<RPCCSCSegment*> & duplicates){
  theDuplicateSegments.clear();
  for(unsigned int i=0; i<duplicates.size(); ++i){
    theDuplicateSegments.push_back(*duplicates[i]);
    //avoid copying duplicates of duplicates of duplicates...
    theDuplicateSegments.back().theDuplicateSegments.resize(0);
 }
}

bool RPCCSCSegment::testSharesAllInSpecificRecHits( const std::vector<CSCRecHit2D>& specificRecHits_1,
						 const std::vector<CSCRecHit2D>& specificRecHits_2,
						 CSCRecHit2D::SharedInputType sharesInput) const{
  const std::vector<CSCRecHit2D> * rhContainer_1 = &specificRecHits_1;
  const std::vector<CSCRecHit2D> * rhContainer_2 = &specificRecHits_2;
  if(specificRecHits_1.size()>specificRecHits_2.size()){
    rhContainer_2 = &specificRecHits_1;
    rhContainer_1 = &specificRecHits_2;
  }
  //
  bool shareConditionPassed = true;
  for ( std::vector<CSCRecHit2D>::const_iterator itRH = rhContainer_1->begin();
	itRH != rhContainer_1->end(); ++itRH) {
    const  CSCRecHit2D *firstRecHit = &(*itRH);
    bool sharedHit = false;
    for ( std::vector<CSCRecHit2D>::const_iterator itRH2 = rhContainer_2->begin();
	  itRH2 != rhContainer_2->end(); ++itRH2) {
      if(itRH2->sharesInput(firstRecHit,sharesInput)){
	sharedHit = true;
	break;
      }
    }
    if(!sharedHit){
      shareConditionPassed = false;
      break;
    }
  }
  return shareConditionPassed;
}

//bool RPCCSCSegment::sharesRecHits(RPCCSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType sharesInput){
  // 2 tracks through a chamber leave 4 rechits per layer (2 strips x 2 wire groups) 
  // this function finds segments sharing wires or strips (first the rechits by sharesInput() )
  // there could probably be more complicated cases with partial sharing (but this needs studies)
  //
  //return testSharesAllInSpecificRecHits( theCSCRecHits , anotherSegment.specificRecHits(), sharesInput);  
//}

bool RPCCSCSegment::sharesRecHits(const RPCCSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType sharesInput) const {
  return testSharesAllInSpecificRecHits( theCSCRecHits , anotherSegment.specificRecHits(), sharesInput);  
}

//
bool RPCCSCSegment::sharesRecHits(const RPCCSCSegment  & anotherSegment) const {
  if(testSharesAllInSpecificRecHits( theCSCRecHits , anotherSegment.specificRecHits(), CSCRecHit2D::someWires) &&
     testSharesAllInSpecificRecHits( theCSCRecHits , anotherSegment.specificRecHits(), CSCRecHit2D::someStrips)){
    return true;
  }
  else{
    return false;
  }
}
//

float RPCCSCSegment::time() const {
  float averageTime=0;
  std::vector<float> wireTimes;
  for (std::vector<CSCRecHit2D>::const_iterator itRH = theCSCRecHits.begin();
       itRH != theCSCRecHits.end(); ++itRH) {
    const  CSCRecHit2D *recHit = &(*itRH);
    averageTime+=recHit->tpeak();
    averageTime+=recHit->wireTime();
    wireTimes.push_back(recHit->wireTime());
  }
  averageTime=averageTime/(2*theCSCRecHits.size());

  //The wire times have a long tail that has to be pruned.  The strip times (tpeak) are fine
  bool modified=true;
  while(modified) {
    modified=false;
    double maxDiff=-1;
    std::vector<float>::iterator maxHit;
    for (std::vector<float>::iterator itWT=wireTimes.begin();
	 itWT!=wireTimes.end(); ++itWT) {
      float diff=fabs(*itWT-averageTime);
      if (diff>maxDiff) {
	maxDiff=diff;
	maxHit=itWT;
      }
    }
    if (maxDiff>26) {
      int N=theCSCRecHits.size()+wireTimes.size();
      averageTime=(averageTime*N-(*maxHit))/(N-1);
      wireTimes.erase(maxHit);
      modified=true;
    }
  }
  return averageTime;
}

//
void RPCCSCSegment::print() const {
  std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const RPCCSCSegment& seg) {
  os << "RPCCSCSegment: local pos = " << seg.localPosition() << 
    " posErr = (" << sqrt(seg.localPositionError().xx())<<","<<sqrt(seg.localPositionError().yy())<<
    "0,)\n"<<
    "            dir = " << seg.localDirection() <<
    " dirErr = (" << sqrt(seg.localDirectionError().xx())<<","<<sqrt(seg.localDirectionError().yy())<<
    "0,)\n"<<
    "            chi2/ndf = " << seg.chi2()/double(seg.degreesOfFreedom()) << 
    " #rechits = " << seg.specificRecHits().size()<<
    " ME1/1a duplicates : "<<seg.duplicateSegments().size();
  return os;  
}

/*
const CSCChamber* RPCCSCSegment::chamber() const { return theChamber; }
*/
