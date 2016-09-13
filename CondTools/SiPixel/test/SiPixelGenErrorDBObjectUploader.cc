#include "CondTools/SiPixel/test/SiPixelGenErrorDBObjectUploader.h"
#include "CondFormats/DataRecord/interface/SiPixelGenErrorDBObjectRcd.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include <fstream>
#include <stdio.h>
#include <iostream>

SiPixelGenErrorDBObjectUploader::SiPixelGenErrorDBObjectUploader(const edm::ParameterSet& iConfig):
	theGenErrorCalibrations( iConfig.getParameter<vstring>("siPixelGenErrorCalibrations") ),
	theGenErrorBaseString( iConfig.getParameter<std::string>("theGenErrorBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ),
	theMagField( iConfig.getParameter<double>("MagField") ),
	thegenerrids( iConfig.getParameter<std::vector<uint32_t> >("generrorIds") )
{
}


SiPixelGenErrorDBObjectUploader::~SiPixelGenErrorDBObjectUploader()
{
}

void 
SiPixelGenErrorDBObjectUploader::beginJob()
{
}

void
SiPixelGenErrorDBObjectUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
{

	// set new modules list:
	   bool newmodule[3][44][8]; // populate all as false:
	   for(int lay=0; lay<3; ++lay) {
	      for(int lad=0; lad<44; ++lad) {
		    for(int mod=0; mod<8; ++mod) {
		       newmodule[lay][lad][mod] = false;
		    }
		}
	    }
	  // change the lucky few to true 
	  newmodule[0][3][5] = true;
	  newmodule[0][3][6] = true;
	  newmodule[0][2][4] = true;
	  newmodule[0][0][4] = true;
	  newmodule[0][0][6] = true;
	  newmodule[0][18][4] = true;
	  newmodule[0][18][5] = true;
	  newmodule[0][16][5] = true;
	  newmodule[0][15][6] = true;
	  newmodule[1][7][7] = true;
	  newmodule[1][6][4] = true;
	  newmodule[1][6][5] = true;
	  newmodule[1][5][4] = true;
	  newmodule[1][4][5] = true;
	  newmodule[1][4][7] = true;
	  newmodule[1][3][5] = true;
	  newmodule[1][2][5] = true;
	  newmodule[1][1][5] = true;
	  newmodule[1][0][5] = true;
	  newmodule[1][0][6] = true;
	  newmodule[1][30][5] = true;
	  newmodule[1][30][6] = true;
	  newmodule[1][31][5] = true;
	  newmodule[1][31][6] = true;
	  newmodule[1][31][7] = true;
	  newmodule[1][28][4] = true;
	  newmodule[1][28][5] = true;
	  newmodule[1][3][1] = true;
	  newmodule[2][9][7] = true;
	  newmodule[2][8][4] = true;
	  newmodule[2][8][7] = true;
	  newmodule[2][5][4] = true;
	  newmodule[2][3][7] = true;
	  newmodule[2][43][5] = true;
	  newmodule[2][42][6] = true;
	  newmodule[2][42][7] = true;
	  newmodule[2][40][5] = true;
	  newmodule[2][37][4] = true;
	  newmodule[2][33][7] = true;
	  newmodule[2][8][5] = true;
	  newmodule[2][0][4] = true;
	  newmodule[2][0][7] = true;
	  newmodule[2][7][1] = true;
	  newmodule[2][2][3] = true;
	  newmodule[2][0][3] = true;
	  newmodule[2][0][2] = true;
	  newmodule[2][0][0] = true;
	  newmodule[2][42][2] = true;
	//end list

	//--- Make the POOL-ORA object to store the database object
	SiPixelGenErrorDBObject* obj = new SiPixelGenErrorDBObject;

	// Local variables 
	const char *tempfile;
	int m;
	
	// Set the number of GenErrors to be passed to the dbobject
	obj->setNumOfTempl(theGenErrorCalibrations.size());
	// Set the version of the GenError dbobject - this is an external parameter
	obj->setVersion(theVersion);

	// Open the GenError file(s) 
	for(m=0; m< obj->numOfTempl(); ++m){
		edm::FileInPath file( theGenErrorCalibrations[m].c_str() );
		tempfile = (file.fullPath()).c_str();
		std::ifstream in_file(tempfile, std::ios::in);
		if(in_file.is_open()){
			edm::LogInfo("GenError Info") << "Opened GenError File: " << file.fullPath().c_str();

			// Local variables 
			char title_char[80], c;
			SiPixelGenErrorDBObject::char2float temp;
			float tempstore;
			int iter,j;
			
			// GenErrors contain a header char - we must be clever about storing this
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
			edm::LogError("SiPixelGenErrorDBObjectUploader") << "Error opening File" << tempfile;
		}
	}
	
	//Get the event setup
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );

	//Make a local list of GenError IDs
	short generrids[55];
	for(int k = 0; k < 55; k++){
	generrids[k] = (short) thegenerrids[k];
	}
	short thisID = 0000;

	//Loop over detector elements and put the GenError IDs in place
	for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
			if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0){

			// Here is the actual looping step over all DetIds:				
			DetId detid=(*it)->geographicalId();
			unsigned int layer=0, ladder=0, disk=0, side=0, blade=0, panel=0, module=0;
					
			// Now we sort them into the Barrel and Endcap:
			//Barrel pixels first
			if(detid.subdetId() == 1) {
				std::cout << "--- IN THE BARREL ---\n";

				//Get the layer, ladder, and module corresponding to this DetID
				PXBDetId pdetId = PXBDetId(detid);
				layer=pdetId.layer();
				ladder=pdetId.ladder();
				module=pdetId.module();

				//Assign template IDs
				if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)){
					//If this is a new module use the exceptions
				   	if (newmodule[layer-1][ladder-1][module-1]) {
					 	if (layer == 1) { thisID=generrids[52]; } 
					    else if (layer == 2) { thisID=generrids[53]; } 
					    else if (layer == 3) { thisID=generrids[54]; } 
				   	}
				   	//Otherwise use the defaults
				   	else {
						if (layer == 1) {
							if (module == 1) { thisID=generrids[0]; } 
							else if (module == 2) { thisID=generrids[1]; } 
							else if (module == 3) { thisID=generrids[2]; } 
							else if (module == 4) { thisID=generrids[3]; } 
							else if (module == 5) { thisID=generrids[4]; } 
							else if (module == 6) { thisID=generrids[5]; } 
							else if (module == 7) { thisID=generrids[6]; } 
							else if (module == 8) { thisID=generrids[7]; } 
						}
						else if (layer == 2) {
							if (module == 1) { thisID=generrids[8]; } 
							else if (module == 2) { thisID=generrids[9]; } 
							else if (module == 3) { thisID=generrids[10]; } 
							else if (module == 4) { thisID=generrids[11]; } 
							else if (module == 5) { thisID=generrids[12]; } 
							else if (module == 6) { thisID=generrids[13]; } 
							else if (module == 7) { thisID=generrids[14]; } 
							else if (module == 8) { thisID=generrids[15]; } 
						}
						else if (layer == 3) {
							if (module == 1) { thisID=generrids[16]; } 
							else if (module == 2) { thisID=generrids[17]; } 
							else if (module == 3) { thisID=generrids[18]; } 
							else if (module == 4) { thisID=generrids[19]; } 
							else if (module == 5) { thisID=generrids[20]; } 
							else if (module == 6) { thisID=generrids[21]; } 
							else if (module == 7) { thisID=generrids[22]; } 
							else if (module == 8) { thisID=generrids[23]; } 
					   	}
					}

					if (thisID==0000 || ( ! (*obj).putGenErrorID( detid.rawId(),thisID ) ))
						std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";
					// ----- debug:
					std::cout<<"This is a barrel element with: layer "<<layer<<", ladder "<<ladder<<" and module "<<module<<".\n"; //Uncomment to read out exact position of each element.
					// -----
				}
			}
			//Now endcaps
			if(detid.subdetId() == 2) {
				std::cout << "--- IN AN ENDCAP ---\n";

				//Get the DetID's disk, blade, side, panel, and module
				PXFDetId pdetId = PXFDetId(detid);
				disk=pdetId.disk(); //1,2,3
			    blade=pdetId.blade(); //1-24
			    side=pdetId.side(); //size=1 for -z, 2 for +z
			    panel=pdetId.panel(); //panel=1,2	
		       	module=pdetId.module(); // plaquette=1-4

				//Assign IDs
				if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)){
					if (side ==1 ){
						if (disk == 1){
							if(panel == 1){
								if(module == 1){ thisID=generrids[24]; } 
								else if(module == 2){ thisID=generrids[25]; } 
								else if(module == 3){ thisID=generrids[26]; } 
								else if(module == 4){ thisID=generrids[27]; } 
							}
							else if(panel == 2){
								if(module == 1){ thisID=generrids[40]; } 
								else if(module == 2){ thisID=generrids[41]; } 
								else if(module == 3){ thisID=generrids[42]; } 
							}
						}
						else if (disk == 2){
							if(panel == 1){
								if(module == 1){ thisID=generrids[28]; } 
								else if(module == 2){ thisID=generrids[29]; } 
								else if(module == 3){ thisID=generrids[30]; } 
								else if(module == 4){ thisID=generrids[31]; } 
							}	
							else if(panel == 2){
								if(module == 1){ thisID=generrids[43]; } 
								else if(module == 2){ thisID=generrids[44]; } 
								else if(module == 3){ thisID=generrids[45]; } 
							}
						}
					}						
					else if (side ==2 ){
						if (disk == 1){
							if(panel == 1){
								if(module == 1){ thisID=generrids[32]; } 
								else if(module == 2){ thisID=generrids[33]; } 
								else if(module == 3){ thisID=generrids[34]; } 
								else if(module == 4){ thisID=generrids[35]; } 
							}
							else if(panel == 2){
								if(module == 1){ thisID=generrids[46]; } 
								else if(module == 2){ thisID=generrids[47]; } 
								else if(module == 3){ thisID=generrids[48]; } 
							}
						}
						else if (disk == 2){
							if(panel == 1){
								if(module == 1){ thisID=generrids[36]; } 
								else if(module == 2){ thisID=generrids[37]; } 
								else if(module == 3){ thisID=generrids[38]; } 
								else if(module == 4){ thisID=generrids[39]; } 
							}	
							else if(panel == 2){
								if(module == 1){ thisID=generrids[49]; } 
								else if(module == 2){ thisID=generrids[50]; } 
								else if(module == 3){ thisID=generrids[51]; } 
							}
						}
					}

					if (thisID==0000 || ( ! (*obj).putGenErrorID( detid.rawId(),thisID ) ) )
						std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
					// ----- debug:
					std::cout<<"This is an endcap element with: side "<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n"; //Uncomment to read out exact position of each element.
					// -----
				}
			}

			//Print out the assignment of this DetID
			short mapnum;
			std::cout<<"checking map:\n";
			mapnum = (*obj).getGenErrorID( detid.rawId());
			std::cout<<"The DetID: "<<detid.rawId()<<" is mapped to the template: "<<mapnum<<".\n\n";

		}
	}

	//--- Create a new IOV
	edm::Service<cond::service::PoolDBOutputService> poolDbService;
	if( !poolDbService.isAvailable() ) // Die if not available
		throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	if(poolDbService->isNewTagRequest("SiPixelGenErrorDBObjectRcd"))
		poolDbService->writeOne( obj, poolDbService->beginOfTime(), "SiPixelGenErrorDBObjectRcd");
	else
		poolDbService->writeOne( obj, poolDbService->currentTime(), "SiPixelGenErrorDBObjectRcd");
}

void 
SiPixelGenErrorDBObjectUploader::endJob()
{
}

