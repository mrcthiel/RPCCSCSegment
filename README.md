# RPCCSCSegment

This is a first version of the RPCCSCSegment. For now the RPCCSCSegment is just a copy of CSCSegment.

To run the code:

export SCRAM_ARCH=slc7_amd64_gcc700 
cmsrel CMSSW_10_6_0_patch2 
cd CMSSW_10_6_0_patch2/src/  
cmsenv 
git clone git@github.com:mrcthiel/RPCCSCSegment.git . 
cmsRun run_CSC_and_RPC_rechits_and_segments_cfg.py  


