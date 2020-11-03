#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegmentProducer.h>
#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegmentBuilderPluginFactory.h>

#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegAlgoRU.h>


DEFINE_FWK_MODULE(RPCCSCSegmentProducer);
DEFINE_EDM_PLUGIN(RPCCSCSegmentBuilderPluginFactory, RPCCSCSegAlgoRU, "RPCCSCSegAlgoRU");

