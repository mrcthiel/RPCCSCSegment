#ifndef RPCRecHit_RPCCSCSegment_h
#define RPCRecHit_RPCCSCSegment_h

/** \class RPCCSCSegment
 *  Describes a reconstructed track segment in the 6 layers of a CSC chamber. 
 *  This is 4-dimensional since it has an origin (x,y) and a direction (x,y)
 *  in the local coordinate system of the chamber.
 *
 *  \author Matteo Sani
 *  \author Rick Wilkinson
 *  \author Tim Cox
 */

#include <DataFormats/TrackingRecHit/interface/RecSegment.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/RPCRecHit/interface/RPCRecHit.h>
#include <iosfwd>

class CSCDetId;

class RPCCSCSegment final : public RecSegment {

public:

    /// Default constructor
    RPCCSCSegment() : theChi2(0.), aME11a_duplicate(false) {}
	
    /// Constructor
//    RPCCSCSegment(const std::vector<const CSCRecHit2D*>& proto_segment, LocalPoint origin, LocalVector direction, const AlgebraicSymMatrix& errors, double chi2);

    RPCCSCSegment(const std::vector<const CSCRecHit2D*>& proto_segment, const RPCRecHit* rpcRecHit, LocalPoint origin, LocalVector direction, const AlgebraicSymMatrix& errors, double chi2);

  
    /// Destructor
    ~RPCCSCSegment() override;

    //--- Base class interface
    RPCCSCSegment* clone() const override { return new RPCCSCSegment(*this); }

    LocalPoint localPosition() const override { return theOrigin; }
    LocalError localPositionError() const override ;
	
    LocalVector localDirection() const override { return theLocalDirection; }
    LocalError localDirectionError() const override ;

    /// Parameters of the segment, for the track fit in the order (dx/dz, dy/dz, x, y )
    AlgebraicVector parameters() const override;

    /// Covariance matrix of parameters()
    AlgebraicSymMatrix parametersError() const override { return theCovMatrix; }

    /// The projection matrix relates the trajectory state parameters to the segment parameters().
    AlgebraicMatrix projectionMatrix() const override;

    std::vector<const TrackingRecHit*> recHits() const override;

    std::vector<TrackingRecHit*> recHits() override;

    double chi2() const override { return theChi2; };

    int dimension() const override { return 4; }

    int degreesOfFreedom() const override { return 2*nRecHits() - 4;}	 

    //--- Extension of the interface
        
    const std::vector<CSCRecHit2D>& specificRecHits() const { return theCSCRecHits; }

    int nRecHits() const { return theCSCRecHits.size(); }        

    CSCDetId cscDetId() const { return  geographicalId(); }

    void setDuplicateSegments(std::vector<RPCCSCSegment*>& duplicates);

    bool isME11a_duplicate() const { return (!theDuplicateSegments.empty() ? true : false); }
    // a copy of the duplicated segments (ME1/1a only) 
    const std::vector< RPCCSCSegment> & duplicateSegments() const { return theDuplicateSegments; } 
    
    bool testSharesAllInSpecificRecHits( const std::vector<CSCRecHit2D>& specificRecHits_1,
					 const std::vector<CSCRecHit2D>& specificRecHits_2,
					 CSCRecHit2D::SharedInputType) const;
    
    //bool sharesRecHits(RPCCSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType);
    // checks if ALL the rechits share the specific input (allWires, allStrips or all)
    bool sharesRecHits(const RPCCSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType sharesInput) const;
    // checks if ALL the rechits share SOME wire AND SOME strip input
    bool sharesRecHits(const RPCCSCSegment  & anotherSegment) const;

    float time() const;
    
    void print() const;		
    
 private:
    
    std::vector<CSCRecHit2D> theCSCRecHits;
    LocalPoint theOrigin;   // in chamber frame - the GeomDet local coordinate system
    LocalVector theLocalDirection; // in chamber frame - the GeomDet local coordinate system
    AlgebraicSymMatrix theCovMatrix; // the covariance matrix
    double theChi2;
    bool aME11a_duplicate;
    std::vector<RPCCSCSegment> theDuplicateSegments;// ME1/1a only
};

std::ostream& operator<<(std::ostream& os, const RPCCSCSegment& seg);

#endif // RPCRecHit_RPCCSCSegment_h
