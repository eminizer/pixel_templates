#include "CondTools/SiPixel/test/SiPixel2DTemplateDBObjectUploader.h"
#include "CondFormats/DataRecord/interface/SiPixel2DTemplateDBObjectRcd.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include <fstream>
#include <stdio.h>
#include <iostream>

SiPixel2DTemplateDBObjectUploader::SiPixel2DTemplateDBObjectUploader(const edm::ParameterSet& iConfig):
	theTemplateCalibrations( iConfig.getParameter<vstring>("siPixelTemplateCalibrations") ),
	theTemplateBaseString( iConfig.getParameter<std::string>("theTemplateBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ),
	theMagField( iConfig.getParameter<double>("MagField") ),
	theBarrelLocations( iConfig.getParameter<std::vector<std::string> >("barrelLocations") ),
	theEndcapLocations( iConfig.getParameter<std::vector<std::string> >("endcapLocations") ),
	theBarrelTemplateIds( iConfig.getParameter<std::vector<uint32_t> >("barrelTemplateIds") ),
	theEndcapTemplateIds( iConfig.getParameter<std::vector<uint32_t> >("endcapTemplateIds") ),
	useVectorIndices( iConfig.getUntrackedParameter<bool>("useVectorIndices",false) )
{
}


SiPixel2DTemplateDBObjectUploader::~SiPixel2DTemplateDBObjectUploader()
{
}

void 
SiPixel2DTemplateDBObjectUploader::beginJob()
{
}

void
SiPixel2DTemplateDBObjectUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
{

	//--- Make the POOL-ORA object to store the database object
	SiPixel2DTemplateDBObject* obj = new SiPixel2DTemplateDBObject;

	// Local variables 
	const char *tempfile;
	int m;
	
	// Set the number of templates to be passed to the dbobject
	obj->setNumOfTempl(theTemplateCalibrations.size());
	// Set the version of the template dbobject - this is an external parameter
	obj->setVersion(theVersion);

	// Open the template file(s) 
	for(m=0; m< obj->numOfTempl(); ++m){
		edm::FileInPath file( theTemplateCalibrations[m].c_str() );
		tempfile = (file.fullPath()).c_str();
		std::ifstream in_file(tempfile, std::ios::in);
		if(in_file.is_open()){
			edm::LogInfo("Template Info") << "Opened Template File: " << file.fullPath().c_str();
			
			// Local variables 
			char title_char[80], c;
			SiPixel2DTemplateDBObject::char2float temp;
			float tempstore;
			int iter,j;
			
			// Templates contain a header char - we must be clever about storing this
			for (iter = 0; (c=in_file.get()) != '\n'; ++iter) {
				if(iter < 79) {title_char[iter] = c;}
			}
			if(iter > 78) {iter=78;}
			title_char[iter+1] ='\n';
			for(j=0; j<80; j+=4) {
				temp.c[0] = title_char[j];
				temp.c[1] = title_char[j+1];
				temp.c[2] = title_char[j+2];
				temp.c[3] = title_char[j+3];
				obj->push_back(temp.f);
				obj->setMaxIndex(obj->maxIndex()+1);
			}
			
			// Fill the dbobject
			in_file >> tempstore;
			while(!in_file.eof()) {
				obj->setMaxIndex(obj->maxIndex()+1);
				obj->push_back(tempstore);
				in_file >> tempstore;
			}
			
			in_file.close();
		}
		else {
			// If file didn't open, report this
			edm::LogError("SiPixel2DTemplateDBObjectUploader") << "Error opening File" << tempfile;
		}
	}
	
	//get the event setup
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );

	// Use the TrackerTopology class for layer/disk etc. number
	edm::ESHandle<TrackerTopology> tTopoHandle;
	es.get<TrackerTopologyRcd>().get(tTopoHandle);
	const TrackerTopology* tTopo = tTopoHandle.product();

	// This tells if we are using Phase I geometry (may be needed for commented out Phase 1 variables)
	//bool phase = pDD->isThere(GeomDetEnumerators::P1PXB) && pDD->isThere(GeomDetEnumerators::P1PXEC);

	//Loop over the detector elements and put template IDs in place
	for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
		if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0){

			// Here is the actual looping step over all DetIds:				
			DetId detid=(*it)->geographicalId();
			unsigned int layer=0, ladder=0, disk=0, side=0, blade=0, panel=0, module=0;
			// Some extra variables from Janos, that can be used for Phase 1 - comment in if needed
			// unsigned int shl=0, sec=0, half=0, flipped=0, ring=0;
			short thisID = 10000;
			unsigned int iter;
					
			// Now we sort them into the Barrel and Endcap:
			//Barrel Pixels first
			if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
				std::cout << "--- IN THE BARREL ---\n";

				//Get the layer, ladder, and module corresponding to this detID
				layer  = tTopo->pxbLayer(detid.rawId());
				ladder = tTopo->pxbLadder(detid.rawId());
				module = tTopo->pxbModule(detid.rawId());
				/*
				// Comment these in if needed
				PixelBarrelName pbn(detid, tTopo, phase);
				shl    = pbn.shell();
				sec    = pbn.sectorName();
				half   = pbn.isHalfModule();
				// This tells if we are on a flipped ladder (in the inner radius, closer to beam)
				flipped = (phase ? layer==4 : layer%2) ? ladder%2==0 : ladder%2==1;
				*/
				if (useVectorIndices) { --layer; --ladder; --module; }

				//Assign template IDs
				//Loop over all the barrel locations
				for (iter=0;iter<theBarrelLocations.size();++iter) {
					//get the string of this barrel location
					std::string loc_string = theBarrelLocations[iter];
					//find where the delimiters are
					unsigned int first_delim_pos = loc_string.find("_");
					unsigned int second_delim_pos = loc_string.find("_",first_delim_pos+1);
					//get the layer, ladder, and module as unsigned ints
					unsigned int checklayer = (unsigned int)stoi(loc_string.substr(0,first_delim_pos));
					unsigned int checkladder = (unsigned int)stoi(loc_string.substr(first_delim_pos+1,second_delim_pos-first_delim_pos-1));
					unsigned int checkmodule = (unsigned int)stoi(loc_string.substr(second_delim_pos+1,5));
					//check them against the desired layer, ladder, and module
					if (ladder==checkladder && layer==checklayer && module==checkmodule)
						//if they match, set the template ID
						thisID=(short)theBarrelTemplateIds[iter];
				}

				if (thisID==10000 || ( ! (*obj).putTemplateID( detid.rawId(),thisID ) ) )
				  std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";	
				// ----- debug:
				std::cout<<"This is a barrel element with: layer "<<layer<<", ladder "<<ladder<<" and module "<<module<<".\n"; //Uncomment to read out exact position of each element.
				// -----
			}
			//Now endcaps
			if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)){
				std::cout << "--- IN AN ENDCAP ---\n";

				//Get the DetId's disk, blade, side, panel, and module
				disk   = tTopo->pxfDisk(detid.rawId()); //1,2,3
				blade  = tTopo->pxfBlade(detid.rawId()); //1-56 (Ring 1 is 1-22, Ring 2 is 23-56)
				side   = tTopo->pxfSide(detid.rawId()); //side=1 for -z, 2 for +z
				panel  = tTopo->pxfPanel(detid.rawId()); //panel=1,2	
				/*
				// Comment these in if needed
				PixelEndcapName pen(detid, tTopo, phase);
				shl    = pen.halfCylinder();
				ring   = pen.ringName(); //1,2 This is for Phase I
				*/
				if (useVectorIndices) { 
				  --disk; --blade; --side; --panel; 
				}

				//Assign IDs

				//Loop over all the endcap locations
				for (iter=0;iter<theEndcapLocations.size();++iter) {
					//get the string of this barrel location
					std::string loc_string = theEndcapLocations[iter];
					//find where the delimiters are
					unsigned int first_delim_pos = loc_string.find("_");
					unsigned int second_delim_pos = loc_string.find("_",first_delim_pos+1);
					unsigned int third_delim_pos = loc_string.find("_",second_delim_pos+1);
					//get the disk, blade, side, panel, and module as unsigned ints
					unsigned int checkdisk = (unsigned int)stoi(loc_string.substr(0,first_delim_pos));
					unsigned int checkblade = (unsigned int)stoi(loc_string.substr(first_delim_pos+1,second_delim_pos-first_delim_pos-1));
					unsigned int checkside = (unsigned int)stoi(loc_string.substr(second_delim_pos+1,third_delim_pos-second_delim_pos-1));
					unsigned int checkpanel = (unsigned int)stoi(loc_string.substr(third_delim_pos+1,5));
					//check them against the desired disk, blade, side, panel, and module
					if (disk==checkdisk && blade==checkblade && side==checkside && panel==checkpanel)
						//if they match, set the template ID
						thisID=(short)theEndcapTemplateIds[iter];
				}

				if (thisID == 10000 || ( ! (*obj).putTemplateID( detid.rawId(),thisID ) ) )
					std::cout << " Could not fill endcap det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", and panel "<<panel<<".\n";
				// ----- debug:
				std::cout<<"This is an endcap element with: side "<<side<<", disk "<<disk<<", blade "<<blade<<", and panel "<<panel<<".\n"; //Uncomment to read out exact position of each element.
				// -----
			}

			//Print out the assignment of this detID
			short mapnum;
			std::cout<<"checking map:\n";
			mapnum = (*obj).getTemplateID( detid.rawId());
			std::cout<<"The DetID: "<<detid.rawId()<<" is mapped to the template: "<<mapnum<<".\n\n";

		}
	}

	//--- Create a new IOV
	edm::Service<cond::service::PoolDBOutputService> poolDbService;
	if( !poolDbService.isAvailable() ) // Die if not available
		throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	if(poolDbService->isNewTagRequest("SiPixel2DTemplateDBObjectRcd"))
		poolDbService->writeOne( obj, poolDbService->beginOfTime(), "SiPixel2DTemplateDBObjectRcd");
	else
		poolDbService->writeOne( obj, poolDbService->currentTime(), "SiPixel2DTemplateDBObjectRcd");
}

void 
SiPixel2DTemplateDBObjectUploader::endJob()
{
}

