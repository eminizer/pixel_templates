
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('reRECO',eras.Run2_2016)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
#process.Timing = cms.Service("Timing",
#        summaryOnly = cms.untracked.bool(False),
#        useJobReport = cms.untracked.bool(False)
#        )

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/Commissioning2018/L1Accept/RAW/v1/000/314/880/00000/C26314F1-5346-E811-86F1-FA163E1B0F91.root'
        #'/store/data/Run2018A/Tau/RAW/v1/000/316/700/00000/72FD5C00-A765-E811-B48F-FA163E11C363.root'
        #'/store/data/Run2018A/SingleMuon/RAW/v1/000/316/758/00000/F89B511D-8164-E811-9A74-FA163E1DCB20.root'
        #'/store/data/Run2018B/SingleMuon/RAW/v1/000/317/527/00001/A41D3A50-1E6A-E811-9C28-FA163E3E8C39.root'
        #'/store/data/Run2018B/SingleMuon/RAW/v1/000/317/661/00000/F4707766-106D-E811-B8A0-FA163E278AF5.root'
        #'/store/data/Run2018B/HLTPhysics/RAW/v1/000/318/100/00000/6A987F7F-9A74-E811-9613-FA163EFCCBD6.root'
        #'/store/data/Run2018B/MinimumBias/RAW/v1/000/317/813/00000/3056018F-E46F-E811-A858-FA163E891257.root'
        #'/store/data/Run2018B/ZeroBias/RAW/v1/000/317/696/00000/66F9795E-156E-E811-934F-FA163E3509C2.root'
        #'/store/data/Run2016B/ZeroBias1/RAW/v1/000/272/022/00000/0C831CB3-BF0D-E611-9622-02163E014561.root' #2016 IOV 1
        #'/store/data/Run2016B/SingleMuon/RAW/v2/000/273/302/00000/F8F20693-C518-E611-81E2-02163E0146BE.root' #2016 IOV 1 on  disk
        #'/store/data/Run2016D/ZeroBias/RAW/v2/000/276/811/00000/E05180E7-0A4B-E611-8146-02163E011CBB.root' #2016 IOV 2
        #'/store/data/Run2016D/JetHT/RAW/v2/000/276/811/00001/FEFDBB74-134B-E611-B659-02163E01439B.root' #2016 IOV 2 on disk
        #'/store/data/Run2016G/ZeroBias/RAW/v1/000/279/767/00000/BE55F91E-C86E-E611-8261-02163E014473.root' #2016 IOV 3
        #'/store/data/Run2016G/SingleMuon/RAW/v1/000/280/385/00001/FCA61558-EF76-E611-9077-02163E0146FA.root' #2016 IOV 3 on disk
        #'/store/data/Run2016H/ZeroBias/RAW/v1/000/283/308/00000/0AB91CC7-A093-E611-B261-FA163E921635.root' #2016 IOV 4
        '/store/data/Run2016H/DoubleMuon/RAW/v1/000/284/068/00000/FA8D247F-519F-E611-B89C-02163E01454C.root' #2016 IOV 4 on disk
),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:test.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '103X_dataRun2_Prompt_Candidate_2018_10_26_20_13_12', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '103X_dataRun2_Prompt_v3', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '104X_dataRun2_Candidate_2019_01_26_09_40_32', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v17', '')
# For custom pixel template sqlite files
process.GlobalTag.toGet = cms.VPSet (
    cms.PSet(record = cms.string('SiPixel2DTemplateDBObjectRcd'),
             label = cms.untracked.string("numerator"),
             tag = cms.string('SiPixel2DTemplateDBObject_38T_2016_ultralegacy_v4_num'),
             connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
             ),
    cms.PSet(record = cms.string('SiPixel2DTemplateDBObjectRcd'),
             label = cms.untracked.string("denominator"),
             tag = cms.string('SiPixel2DTemplateDBObject_38T_2016_ultralegacy_v1_den_unirradiated'),
             connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
             ),
    cms.PSet(record = cms.string('SiPixelTemplateDBObjectRcd'),
             tag = cms.string('SiPixelTemplateDBObject_38T_2016_ultralegacy_v4'),
             connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
             )
    )

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
#process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)
#from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
#associatePatAlgosToolsTask(process)










process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.L1Reco_step,
    process.reconstruction_step,
    process.RECOoutput_step
    )

# end of insert to cmsDriver script


#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
