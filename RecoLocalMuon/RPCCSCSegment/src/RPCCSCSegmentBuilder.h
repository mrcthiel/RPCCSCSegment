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
    void build(const CSCRecHit2DCollection* rechits, RPCCSCSegmentCollection& oc);

    /** Cache pointer to geometry _for current event_
     */
    void setGeometry(const CSCGeometry* geom);

private:

    const CSCGeometry* geom_;
    std::map<std::string, std::unique_ptr<RPCCSCSegmentAlgorithm>> algoMap;
};

#endif
