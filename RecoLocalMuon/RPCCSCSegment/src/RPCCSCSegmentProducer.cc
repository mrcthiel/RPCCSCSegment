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
    rpc_token = consumes<RPCRecHitCollection>( pas.getParameter<edm::InputTag>("rpcRecHitTag") );

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

    edm::ESHandle<RPCGeometry> rpcGeo;
    setup.get<MuonGeometryRecord>().get(rpcGeo);
    const RPCGeometry* rgeom = &*rpcGeo;

//    segmentBuilder_->setGeometry(pgeom);
    segmentBuilder_->setGeometry(pgeom, rgeom);
	


    // get the collection of CSCRecHit2D
    edm::Handle<CSCRecHit2DCollection> cscRecHits;
    ev.getByToken( m_token, cscRecHits);

    // get the collection of RPCRecHit
    edm::Handle<RPCRecHitCollection> pRPCrecHits;
    ev.getByToken( rpc_token, pRPCrecHits);

    // create empty collection of Segments
    auto oc = std::make_unique<RPCCSCSegmentCollection>();

    // create CSC-RPC association

        using namespace edm;
        std::map<CSCStationIndex,std::set<RPCDetId>> rollstore_temp;
        rollstore_temp.clear();

        for (TrackingGeometry::DetContainer::const_iterator it=rgeom->dets().begin();it<rgeom->dets().end();it++){
                if(dynamic_cast< const RPCChamber* >( *it ) != nullptr ){
                        auto ch = dynamic_cast< const RPCChamber* >( *it );
                        std::vector< const RPCRoll*> roles = (ch->rolls());
                        for(std::vector<const RPCRoll*>::const_iterator r = roles.begin();r != roles.end(); ++r){
                                RPCDetId rpcId = (*r)->id();
                                int region=rpcId.region();
                                if(region!=0){
                                        int station=rpcId.station();
                                        int ring=rpcId.ring();
                                        int cscring=ring;
                                        int cscstation=station;
                                        RPCGeomServ rpcsrv(rpcId);
                                        int rpcsegment = rpcsrv.segment();
                                        int cscchamber = rpcsegment; //FIX THIS ACCORDING TO RPCGeomServ::segment()Definition
                                        if((station==2||station==3)&&ring==3){//Adding Ring 3 of RPC to the CSC Ring 2
                                                cscring = 2;
                                        }
                                        CSCStationIndex ind(region,cscstation,cscring,cscchamber);
                                        std::set<RPCDetId> myrolls;
                                        if (rollstore_temp.find(ind)!=rollstore_temp.end()) myrolls=rollstore_temp[ind];
                                        myrolls.insert(rpcId);
                                        rollstore_temp[ind]=myrolls;
                                }
                        }
                }
        }
        const std::map<CSCStationIndex,std::set<RPCDetId>> rollstore = rollstore_temp;


  	// fill the collection
//    segmentBuilder_->build(cscRecHits.product(), *oc); //@@ FILL oc
    segmentBuilder_->build(cscRecHits.product(), pRPCrecHits.product(), rollstore_temp, *oc); //@@ FILL oc

    // put collection in event
    ev.put(std::move(oc));
}
