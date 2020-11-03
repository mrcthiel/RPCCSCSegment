#ifndef RPCCSCSegment_RPCCSCSegmentBuilderPluginFactory_h
#define RPCCSCSegment_RPCCSCSegmentBuilderPluginFactory_h

/** \class RPCCSCSegmentBuilderPluginFactory
 *  Plugin factory for concrete RPCCSCSegmentBuilder algorithms
 *
 * \author M. Sani
 * 
 */

#include <FWCore/PluginManager/interface/PluginFactory.h>
#include <RecoLocalMuon/RPCCSCSegment/src/RPCCSCSegmentAlgorithm.h>

typedef edmplugin::PluginFactory<RPCCSCSegmentAlgorithm *(const edm::ParameterSet&)> RPCCSCSegmentBuilderPluginFactory;

#endif
