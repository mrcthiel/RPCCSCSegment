import FWCore.ParameterSet.Config as cms

from RecoLocalMuon.RPCCSCSegment.RPCCSCSegmentAlgorithmRU_cfi import *

rpccscSegments = cms.EDProducer("RPCCSCSegmentProducer",
    # Define input
    inputObjects = cms.InputTag("csc2DRecHits"),
    # Choice of the building algo: 1 SK, 2 TC, 3 DF, 4 ST, 5 RU, ...
    algo_type = cms.int32(1),
    # std::vector<edm::ParameterSet>
    algo_psets = cms.VPSet(
        cms.PSet(
            RPCCSCSegAlgoRU
        )

     )
)


