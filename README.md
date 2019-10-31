# SiPixel Template/GenError Uploader
====================================

This code produces (1D and 2D) pixel template and generr sqlite db files by uploading template files and associating each detID with a template.

Checkout Instructions & Environment setup
=========================================

Currently not yet integrated into CMSSW, so this is very kludge-y and does not retain a working clone of this repo. Sorry!
This code just adds a few files to CondTools in CMSSW, so until it's incorporated this will be a bit nasty. 

```Shell
setenv SCRAM_ARCH slc6_amd64_gcc493
cmsrel CMSSW_8_0_0
cd CMSSW_8_0_0/src/
cmsenv
git cms-addpkg CondTools
git cms-addpkg Configuration
git clone https://github.com/eminizer/pixel_templates
cp -r pixel_templates/ CondTools/* CondTools/
rmrf pixel_templates/CondTools 
scram b -j 20
```

Input Files
===========

1) Template and GenError Files
* These are produced by Prof. Morris Swartz at Johns Hopkins University
* Template files should be located by default in /CondTools/SiPixel/data/, GenErr files in /CondTools/SiPixel/generr_data (this can be changed via a command line option, see below)
* Template files should be named in the format "template_summary_zp????.out", GenErrs files as "generr_summary_zp????.out" (this is hardcoded currently so if it changes edits to the scripts will need to be made)

2) Map File
* This is a .csv file that maps detector locations to templates
* You can make this from a spreadsheet if you want
* An example of the format can be found at /CondTools/SiPixel/data/template_1D_IOV12/IOV12_map.csv
* The location of this file can be changed via a command line option, but it's currently set to the example above

Running
=======

Example commands
The output of these commands prints some useful information, including the location and template assignments for each detID, so it could be good to pipe it to a file if you want to double check.

1) Producing Template SQLite files
```Shell
cd /CondTools/SiPixel/test
cmsRun SiPixelTemplateDBObjectUploader_cfg.py MagField=3.8 Year=2016 Version=2 Append=bugfix
```
This will produce an SQLLite file called "SiPixelTemplateDBObject_38T_2016_v2_bugfix.db"

2) Producing GenError SQLite files
```Shell
cd /CondTools/SiPixel/test
cmsRun SiPixelGenErrorDBObjectUploader_cfg.py MagField=3.8 Year=2016 Version=2 Append=bugfix
```
This will produce an SQLLite file called "SiPixelGenErrorDBObject_38T_2016_v2_bugfix.db"

Command Line Options
====================

1) MagField: Magnetic field value in Tesla

2) Year: Current year for versioning

3) Version: Version number (don't set this arbitrarily, ask if you don't know which to use)

4) Append: Anything to add to the end of the SQLite filenames, like "bugfix"

5) Map: Full path to the Map file mentioned above (this one does have a default, so if you've changed something, use this option)

6) GlobalTag: GlobalTag for data run (default is set for this one too, so change it if necessary)

7) Delimiter: The delimiter used in the .csv map file (default is a comma, ",")

8) Quotechar: The quotechar used in the .csv map file (default is double quotes, """)

9) (Template/GenErr)FilePath: The path to the directory holding the template/generr files (these are currently set to the defaults mentioned above)

Work to be Done
===============

1) Submit a pull request to CMSSW

2) Incorporate some sort of visualization of template/detID assignments

Maintenance
===========

This work has been maintained to this point by Maggie Eminizer at Johns Hopkins University (margaret.eminizer@gmail.com) and represents a service work contribution to the CMS Collaboration.