#ifndef RPCCSCSegment_RPCCSCSegmentProducer_h
#define RPCCSCSegment_RPCCSCSegmentProducer_h

/** \class RPCCSCSegmentProducer 
 * Produces a collection of RPCCSCSegment's in endcap muon CSCs. 
 *
 */

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "RecoLocalMuon/RPCRecHit/src/CSCStationIndex.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"

class RPCCSCSegmentBuilder; 

class RPCCSCSegmentProducer : public edm::stream::EDProducer<> {
public:
    /// Constructor
    explicit RPCCSCSegmentProducer(const edm::ParameterSet&);
    /// Destructor
    ~RPCCSCSegmentProducer() override;
    /// Produce the RPCCSCSegment collection
    void produce(edm::Event&, const edm::EventSetup&) override;

private:
    int iev; // events through
    RPCCSCSegmentBuilder* segmentBuilder_;
    edm::EDGetTokenT<CSCRecHit2DCollection> m_token;
    edm::EDGetTokenT<RPCRecHitCollection> rpc_token;
};

#endif
