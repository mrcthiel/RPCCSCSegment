#ifndef DataFormats_RPCCSCSegmentCollection_H
#define DataFormats_RPCCSCSegmentCollection_H

/** \class RPCCSCSegmentCollection
 *
 * The collection of RPCCSCSegment's. See \ref RPCCSCSegmentCollection.h for details.
 *
 *  \author Matteo Sani
 */

#include <DataFormats/MuonDetId/interface/CSCDetId.h> 
#include <DataFormats/RPCRecHit/interface/RPCCSCSegment.h>

#include <DataFormats/Common/interface/RangeMap.h>
#include <DataFormats/Common/interface/ClonePolicy.h>
#include <DataFormats/Common/interface/OwnVector.h>

typedef edm::RangeMap <CSCDetId, edm::OwnVector<RPCCSCSegment> > RPCCSCSegmentCollection;

#include <DataFormats/Common/interface/Ref.h>
typedef edm::Ref<RPCCSCSegmentCollection> RPCCSCSegmentRef;

//typedef std::vector<RPCCSCSegment> RPCCSCSegmentCollection; 
	
#endif
