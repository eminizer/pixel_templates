import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

options = opts.VarParsing ('standard')

options.register('MagField',
    			 None,
    			 opts.VarParsing.multiplicity.singleton,
    			 opts.VarParsing.varType.float,
    			 'Magnetic field value in Tesla')
options.register('Version',
    			 None,
    			 opts.VarParsing.multiplicity.singleton,
    			 opts.VarParsing.varType.string,
    			 'Template DB object version')
options.register('TemplateFilePath',
    			 'CondTools/SiPixel/data',
    			 opts.VarParsing.multiplicity.singleton,
    			 opts.VarParsing.varType.string,
    			 'Location of template files')
options.register('GlobalTag',
    			 '80X_dataRun2_Prompt_v8',
    			 opts.VarParsing.multiplicity.singleton,
    			 opts.VarParsing.varType.string,
    			 'Global tag for this run')
options.parseArguments()

MagFieldValue = 10.*options.MagField #code needs it in deciTesla
print '\nMagField = %f deciTesla \n'%(MagFieldValue)
version = options.Version
print'\nVersion = %s \n'%(version)
magfieldstrsplit = str(options.MagField).split('.')
MagFieldString = magfieldstrsplit[0]
if len(magfieldstrsplit)>1 :
	MagFieldString+='p'+magfieldstrsplit[1]

process = cms.Process("SiPixelTemplateDBUpload")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = options.GlobalTag

prefix = options.TemplateFilePath+'/template_summary_'
suffix = ".out"
files_to_upload = cms.vstring(
	prefix + "zp9410" + suffix,
	prefix + "zp9510" + suffix,
	prefix + "zp5611" + suffix,
	prefix + "zp5711" + suffix,
	prefix + "zp7612" + suffix,
	prefix + "zp7712" + suffix,
	prefix + "zp6483" + suffix,
	prefix + "zp4884" + suffix,
	prefix + "zp4885" + suffix,
	prefix + "zp1006" + suffix,)
theTemplateIds = cms.vuint32(
							# IN THE BARREL:
						9510, # Position 0: Layer 1, Mod 1
						9510, # Position 1: Layer 1, Mod 2
						9510, # Position 2: Layer 1, Mod 3
						9510, # Position 3: Layer 1, Mod 4
						9410, # Position 4: Layer 1, Mod 5
						9410, # Position 5: Layer 1, Mod 6
						9410, # Position 6: Layer 1, Mod 7
						9410, # Position 7: Layer 1, Mod 8
						5711, # Position 8: Layer 2, Mod 1
						5711, # Position 9: Layer 2, Mod 2
						5711, # Position 10: Layer 2, Mod 3
						5711, # Position 11: Layer 2, Mod 4
						5611, # Position 12: Layer 2, Mod 5
						5611, # Position 13: Layer 2, Mod 6
						5611, # Position 14: Layer 2, Mod 7
						5611, # Position 15: Layer 2, Mod 8
						7712, # Position 16: Layer 3, Mod 1
						7712, # Position 17:Layer 3, Mod 2
						7712, # Position 18:Layer 3, Mod 3
						7712, # Position 19:Layer 3, Mod 4
						7612, # Position 20:Layer 3, Mod 5
						7612, # Position 21:Layer 3, Mod 6
						7612, # Position 22:Layer 3, Mod 7
						7612, # Position 23:Layer 3, Mod 8
							# IN THE ENDCAPS:
						6483, # Position 24: Side 1, Disk 1, Panel 1, Mod 1
						4884, # Position 25: Side 1, Disk 1, Panel 1, Mod 2
						4885, # Position 26: Side 1, Disk 1, Panel 1, Mod 3
						4885, # Position 27: Side 1, Disk 1, Panel 1, Mod 4
						6483, # Position 28: Side 1, Disk 1, Panel 2, Mod 1
						4884, # Position 29: Side 1, Disk 1, Panel 2, Mod 2
						4885, # Position 30: Side 1, Disk 1, Panel 2, Mod 3
						6483, # Position 31: Side 1, Disk 2, Panel 1, Mod 1
						4884, # Position 32: Side 1, Disk 2, Panel 1, Mod 2
						4885, # Position 33: Side 1, Disk 2, Panel 1, Mod 3
						4885, # Position 34: Side 1, Disk 2, Panel 1, Mod 4
						6483, # Position 35: Side 1, Disk 2, Panel 2, Mod 1
						4884, # Position 36: Side 1, Disk 2, Panel 2, Mod 2
						4885, # Position 37: Side 1, Disk 2, Panel 2, Mod 3
						6483, # Position 38: Side 2, Disk 1, Panel 1, Mod 1
						4884, # Position 39: Side 2, Disk 1, Panel 1, Mod 2
						4885, # Position 40: Side 2, Disk 1, Panel 1, Mod 3
						4885, # Position 41: Side 2, Disk 1, Panel 1, Mod 4
						6483, # Position 42: Side 2, Disk 1, Panel 2, Mod 1
						4884, # Position 43: Side 2, Disk 1, Panel 2, Mod 2
						4885, # Position 44: Side 2, Disk 1, Panel 2, Mod 3
						6483, # Position 45: Side 2, Disk 2, Panel 1, Mod 1
						4884, # Position 46: Side 2, Disk 2, Panel 1, Mod 2
						4885, # Position 47: Side 2, Disk 2, Panel 1, Mod 3
						4885, # Position 48: Side 2, Disk 2, Panel 1, Mod 4
						6483, # Position 49: Side 2, Disk 2, Panel 2, Mod 1
						4884, # Position 50: Side 2, Disk 2, Panel 2, Mod 2
						4885, # Position 51: Side 2, Disk 2, Panel 2, Mod 3
							# NEW MODULES:
						5616,  # Position 52: Will fill list of "replaced" pixels, layer 1
						6417,  # Position 53: Will fill list of "replaced" pixels, layer 2
						4818  # Position 54: Will fill list of "replaced" pixels, layer 3
	)

template_base = 'SiPixelTemplateDBObject' + MagFieldString + 'T'

print '\nUploading %s%s with record SiPixelTemplateDBObjectRcd in file siPixelTemplates%sT.db\n' % (template_base,version,MagFieldString)

process.source = cms.Source("EmptyIOVSource",
                            timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(1),
                            lastValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          DBParameters = cms.PSet(messageLevel = cms.untracked.int32(0),
    															  authenticationPath = cms.untracked.string('.')
    															  ),
                                          timetype = cms.untracked.string('runnumber'),
                                          connect = cms.string('sqlite_file:siPixel1DTemplatesNM' + MagFieldString + 'T.db'),
                                          toPut = cms.VPSet(cms.PSet(record = cms.string('SiPixelTemplateDBObjectRcd'),
    																 tag = cms.string(template_base + version)
																	 )
                                          				    )
                                          )
process.uploader = cms.EDAnalyzer("SiPixelTemplateDBObjectUploader",
                                  siPixelTemplateCalibrations = files_to_upload,
                                  theTemplateBaseString = cms.string(template_base),
                                  Version = cms.double("3.0"),
                                  MagField = cms.double(MagFieldValue),
                                  templateIds = theTemplateIds
								 )
process.myprint = cms.OutputModule("AsciiOutputModule")
process.p = cms.Path(process.uploader)
process.CondDBCommon.connect = 'sqlite_file:siPixel1DTemplatesNM' + MagFieldString + 'T.db'
process.CondDBCommon.DBParameters.messageLevel = 0
process.CondDBCommon.DBParameters.authenticationPath = './'
