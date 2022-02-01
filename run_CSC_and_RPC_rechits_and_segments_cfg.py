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
#    fileNames = cms.untracked.vstring('file:/eos/cms/store/user/mthiel/23509529-CA46-0D44-91B3-CA5B0CD03AB5.root'),
    fileNames = cms.untracked.vstring('file:/eos/cms/store/user/mthiel/23509529-CA46-0D44-91B3-CA5B0CD03AB5.root','file:/eos/cms/store/user/mthiel/3D6E0381-37F8-CA44-99D2-F0F07A27AB2A.root','file:/eos/cms/store/user/mthiel/41A8FF37-7D2E-7249-B152-FE89164826F2.root','file:/eos/cms/store/user/mthiel/4B69420D-7BBC-BC46-8FF8-4090BB22E52F.root','file:/eos/cms/store/user/mthiel/560F7499-5721-654B-BC83-3AE76477D8F1.root','file:/eos/cms/store/user/mthiel/6EA092BE-94A5-AD49-80DB-6A73ADBD033E.root','file:/eos/cms/store/user/mthiel/77DD67E9-434B-8242-B08C-C99FBC95F477.root','file:/eos/cms/store/user/mthiel/7E51FB46-8F9C-614D-830E-1DC00A69B753.root','file:/eos/cms/store/user/mthiel/7EABBE9A-4C49-BA4C-87C9-8E7E245174B3.root','file:/eos/cms/store/user/mthiel/7F4AF09D-D268-C74E-B06C-259D965857BE.root','file:/eos/cms/store/user/mthiel/8659033B-7DE7-C448-B7D6-5DBC29F502A9.root','file:/eos/cms/store/user/mthiel/871278EB-3E2F-4342-B4CF-B7BDBF38975A.root','file:/eos/cms/store/user/mthiel/918902A9-9C86-A04C-B4F1-EADCD16A7E22.root','file:/eos/cms/store/user/mthiel/97A0EE28-28BF-7249-9A3F-772D6B7A98A3.root','file:/eos/cms/store/user/mthiel/A87AA119-9C11-3A49-B726-D87465854A19.root','file:/eos/cms/store/user/mthiel/AA5992F4-5D03-BF41-8CA6-0C0F5B38EC1E.root','file:/eos/cms/store/user/mthiel/B43C1523-B66F-944D-A2E9-128CB4576CF8.root','file:/eos/cms/store/user/mthiel/BBDD1FC4-DC6F-1F4C-B777-72D247A6FC1F.root','file:/eos/cms/store/user/mthiel/BDCB64F7-4BC6-D342-B4E7-F9451C5656D0.root','file:/eos/cms/store/user/mthiel/D696E7CC-7F02-CA4C-83C9-DD51FBCD3A48.root','file:/eos/cms/store/user/mthiel/DBDE8ECA-27FA-3D49-AE4A-8ACC528A853A.root','file:/eos/cms/store/user/mthiel/DD924933-8A75-8645-84F5-D408236797C1.root','file:/eos/cms/store/user/mthiel/DEE59424-11F1-B34E-A151-77D6642AA834.root','file:/eos/cms/store/user/mthiel/E0D5C6A5-B855-D14F-9124-0B2C9B28D0EA.root'),


#    fileNames = cms.untracked.vstring('/store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/60000/23509529-CA46-0D44-91B3-CA5B0CD03AB5.root'),
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

