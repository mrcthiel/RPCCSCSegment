/** \file RPCCSCSegmentProducer.cc
 *
 */

#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegmentProducer.h>
#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegmentBuilder.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/RPCRecHit/interface/RPCCSCSegmentCollection.h>
#include <DataFormats/RPCRecHit/interface/RPCCSCSegment.h>

RPCCSCSegmentProducer::RPCCSCSegmentProducer(const edm::ParameterSet& pas) : iev(0) {
	
    m_token = consumes<CSCRecHit2DCollection>( pas.getParameter<edm::InputTag>("inputObjects") );
    segmentBuilder_ = new RPCCSCSegmentBuilder(pas); // pass on the PS

  	// register what this produces
    produces<RPCCSCSegmentCollection>();
}

RPCCSCSegmentProducer::~RPCCSCSegmentProducer() {

    LogDebug("RPCCSCSegment|CSC") << "deleting RPCCSCSegmentBuilder after " << iev << " events w/csc data.";
    delete segmentBuilder_;
}

void RPCCSCSegmentProducer::produce(edm::Event& ev, const edm::EventSetup& setup) {

    LogDebug("RPCCSCSegment|CSC") << "start producing segments for " << ++iev << "th event with csc data";
	
    // find the geometry (& conditions?) for this event & cache it in the builder
  
    edm::ESHandle<CSCGeometry> h;
    setup.get<MuonGeometryRecord>().get(h);
    const CSCGeometry* pgeom = &*h;
    segmentBuilder_->setGeometry(pgeom);
	
    // get the collection of CSCRecHit2D
    edm::Handle<CSCRecHit2DCollection> cscRecHits;
    ev.getByToken( m_token, cscRecHits);

    // create empty collection of Segments
    auto oc = std::make_unique<RPCCSCSegmentCollection>();

  	// fill the collection
    segmentBuilder_->build(cscRecHits.product(), *oc); //@@ FILL oc

    // put collection in event
    ev.put(std::move(oc));
}
