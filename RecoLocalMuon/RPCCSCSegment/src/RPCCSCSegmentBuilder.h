#ifndef RPCCSCSegment_RPCCSCSegmentBuilder_h
#define RPCCSCSegment_RPCCSCSegmentBuilder_h

/** \class RPCCSCSegmentBuilder 
 * Algorithm to build RPCCSCSegment's from CSCRecHit2D collection
 * by implementing a 'build' function required by RPCCSCSegmentProducer.
 *
 * Implementation notes: <BR>
 * Configured via the Producer's ParameterSet. <BR>
 * Presume this might become an abstract base class one day. <BR>
 *
 * \author M. Sani
 *
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/RPCRecHit/interface/RPCCSCSegmentCollection.h>
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "RecoLocalMuon/RPCRecHit/src/CSCStationIndex.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCGeometry;
class RPCCSCSegmentAlgorithm;

class RPCCSCSegmentBuilder {
public:
   
    /** Configure the algorithm via ctor.
     * Receives ParameterSet percolated down from EDProducer
     * which owns this Builder.
     */
    explicit RPCCSCSegmentBuilder(const edm::ParameterSet&);
    /// Destructor
    ~RPCCSCSegmentBuilder();

    /** Find rechits in each CSCChamber, build RPCCSCSegment's in each chamber,
     *  and fill into output collection.
     */
//    void build(const CSCRecHit2DCollection* rechits, RPCCSCSegmentCollection& oc);
    void build(const CSCRecHit2DCollection* rechits, const RPCRecHitCollection* rpcrechits, const std::map<CSCStationIndex,std::set<RPCDetId>> rollstore, RPCCSCSegmentCollection& oc);

    /** Cache pointer to geometry _for current event_
     */
//    void setGeometry(const CSCGeometry* geom);
    void setGeometry(const CSCGeometry* geom, const RPCGeometry* rgeom);

private:

    const CSCGeometry* geom_;
    const RPCGeometry* rgeom_;
    std::map<std::string, std::unique_ptr<RPCCSCSegmentAlgorithm>> algoMap;
};

#endif
