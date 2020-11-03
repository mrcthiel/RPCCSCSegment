#ifndef RPCCSCSegment_RPCCSCSegmentAlgorithm_h
#define RPCCSCSegment_RPCCSCSegmentAlgorithm_h

/** \class RPCCSCSegmentAlgo
 * An abstract base class for algorithmic classes used to
 * build segments in one chamber of an Endcap Muon CSC.
 *
 * Implementation notes: <BR>
 * For example, RPCCSCSegmentizerSK inherits from this class,
 * and classes ported from ORCA local reco inherit from that.
 *
 * \author M. Sani
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/RPCRecHit/interface/RPCCSCSegment.h>
#include <Geometry/CSCGeometry/interface/CSCChamber.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <vector>

class RPCCSCSegmentAlgorithm {
public:
    /// Constructor
    explicit RPCCSCSegmentAlgorithm(const edm::ParameterSet&) {};
    /// Destructor
    virtual ~RPCCSCSegmentAlgorithm() {};

    /** Run the algorithm = build the segments in this chamber
    */
    virtual std::vector<RPCCSCSegment> run(const CSCChamber* chamber, const std::vector<const CSCRecHit2D*>& rechits) = 0;  

    private:
};

#endif
