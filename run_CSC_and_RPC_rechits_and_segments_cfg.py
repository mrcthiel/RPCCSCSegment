# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --filein file:PPD-PhaseIITDRSpring19DR-00002_step1.root --fileout file:PPD-PhaseIITDRSpring19DR-00002.root --mc --eventcontent AODSIM --runUnscheduled --customise SLHCUpgradeSimulations/Configuration/aging.customise_aging_1000,Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --conditions 106X_upgrade2023_realistic_v3 --step RAW2DIGI,L1Reco,RECO --nThreads 8 --geometry Extended2023D41 --era Phase2C8_timing_layer_bar --python_filename PPD-PhaseIITDRSpring19DR-00002_2_cfg.py --no_exec -n 64
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C8_timing_layer_bar_cff import Phase2C8_timing_layer_bar

process = cms.Process('RECO',Phase2C8_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/60000/23509529-CA46-0D44-91B3-CA5B0CD03AB5.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:64'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(31457280),
    fileName = cms.untracked.string('file:CSC_and_RPC.root'),
    outputCommands = process.AODSIMEventContent.outputCommands
)

process.AODSIMoutput.outputCommands = cms.untracked.vstring('drop *')
process.AODSIMoutput.outputCommands = cms.untracked.vstring('keep *_*csc*_*_*','keep *_*rpcRecHits*_*_*')


# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2023_realistic_v3', '')

#process.load('RecoLocalMuon.RPCCSCSegment.RPCCSCSegmentAlgorithmRU_cfi')
process.load('RecoLocalMuon.RPCCSCSegment.rpccscSegments_cfi')

# Path and EndPath def
process.unpack = cms.Path(process.muonCSCDigis)
process.reco = cms.Path(process.rpcRecHits * process.csc2DRecHits * process.cscSegments * process.rpccscSegments)
process.endjob = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.unpack, process.reco, process.endjob, process.AODSIMoutput_step)

