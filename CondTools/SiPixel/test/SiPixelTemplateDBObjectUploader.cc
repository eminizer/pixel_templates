#include "CondTools/SiPixel/test/SiPixelTemplateDBObjectUploader.h"
#include "CondFormats/DataRecord/interface/SiPixelTemplateDBObjectRcd.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include <fstream>
#include <stdio.h>
#include <iostream>

SiPixelTemplateDBObjectUploader::SiPixelTemplateDBObjectUploader(const edm::ParameterSet& iConfig):
	theTemplateCalibrations( iConfig.getParameter<vstring>("siPixelTemplateCalibrations") ),
	theTemplateBaseString( iConfig.getParameter<std::string>("theTemplateBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ),
	theMagField( iConfig.getParameter<double>("MagField") ),
	theTemplIds( iConfig.getParameter<std::vector<uint32_t> >("templateIds") )
{
}


SiPixelTemplateDBObjectUploader::~SiPixelTemplateDBObjectUploader()
{
}

void 
SiPixelTemplateDBObjectUploader::beginJob()
{
}

void
SiPixelTemplateDBObjectUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
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
	// end list
	//--- Make the POOL-ORA object to store the database object
	SiPixelTemplateDBObject* obj = new SiPixelTemplateDBObject;

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
			SiPixelTemplateDBObject::char2float temp;
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
			edm::LogError("SiPixelTemplateDBObjectUploader") << "Error opening File" << tempfile;
		}
	}
	
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );

	short templids[55];
	for(int k = 0; k < 55; k++){
	templids[k] = (short) theTemplIds[k];
	}

	for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
		if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0){

			// Here is the actual looping step over all DetIds:				
			DetId detid=(*it)->geographicalId();
			unsigned int layer=0, ladder=0, disk=0, side=0, blade=0, panel=0, module=0;
					
			// Now we sort them into the Barrel and Endcap:
			if(detid.subdetId() == 1) {
				std::cout << "--- IN THE BARREL ---\n";
				PXBDetId pdetId = PXBDetId(detid);

				layer=pdetId.layer();
				ladder=pdetId.ladder();
				module=pdetId.module();

				if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)){
				   if (newmodule[layer-1][ladder-1][module-1]) {
					    if (layer == 1) {
						if ( ! (*obj).putTemplateID( detid.rawId(),templids[52] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
					    if (layer == 2) {
						if ( ! (*obj).putTemplateID( detid.rawId(),templids[53] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
					    if (layer == 3) {
						if ( ! (*obj).putTemplateID( detid.rawId(),templids[54] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
				   }
				   else {
					if (layer == 1) {
						if (module == 1) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[0] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";						
						}
						if (module == 2) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[1] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 3) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[2] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 4) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[3] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 5) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[4] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 6) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[5] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 7) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[6] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 8) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[7] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
					
					}
					if (layer == 2) {
						if (module == 1) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[8] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";		
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 2) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[9] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 3) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[10] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 4) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[11] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 5) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[12] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 6) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[13] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 7) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[14] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 8) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[15] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
					
					}
					if (layer == 3) {
						if (module == 1) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[16] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 2) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[17] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 3) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[18] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 4) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[19] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 5) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[20] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 6) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[21] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";	
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 7) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[22] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 8) {
							if ( ! (*obj).putTemplateID( detid.rawId(),templids[23] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";									
						}
					   }
					}
				// ----- debug:
				std::cout<<"This is a barrel element with: layer "<<layer<<", ladder "<<ladder<<" and module "<<module<<".\n"; //Uncomment to read out exact position of each element.
				// -----
				}
			}
			if(detid.subdetId() == 2) {

				std::cout << "--- IN AN ENDCAP ---\n";
				PXFDetId pdetId = PXFDetId(detid);
				disk=pdetId.disk(); //1,2,3
			       	blade=pdetId.blade(); //1-24
			       	side=pdetId.side(); //size=1 for -z, 2 for +z
			       	panel=pdetId.panel(); //panel=1,2	
		        	module=pdetId.module(); // plaquette

				//short temp123abc = (short) theTemplIds[1];
				if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)){
					if (side ==1 ){
						if (disk == 1){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[24] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[25] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[26] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[27] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[40] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[41] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[42] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
						if (disk == 2){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[28] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[29] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[30] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[31] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}	
							if(panel == 2){
								if(module == 1){
										if ( ! (*obj).putTemplateID( detid.rawId(),templids[43] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[44] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[45] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
					}						
					if (side ==2 ){
						if (disk == 1){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[32] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[33] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[34] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[35] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[46] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[47] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[48] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
						if (disk == 2){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[36] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[37] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[38] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[39] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}	
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[49] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[50] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putTemplateID( detid.rawId(),templids[51] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
					}
						
				// ----- debug:
				std::cout<<"This is an endcap element with: side "<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n"; //Uncomment to read out exact position of each element.
				// -----
				}
			}

			short mapnum;
			std::cout<<"checking map:\n";
			mapnum = (*obj).getTemplateID( detid.rawId());
			std::cout<<"The DetID: "<<detid.rawId()<<" is mapped to the template: "<<mapnum<<".\n\n";

			//else 
				//if ( ! (*obj).putTemplateID( detid.rawId(),templids[1] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									//std::cout << "ERROR! OH NO!\n";
		}
	}

	//--- Create a new IOV
	edm::Service<cond::service::PoolDBOutputService> poolDbService;
	if( !poolDbService.isAvailable() ) // Die if not available
		throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	if(poolDbService->isNewTagRequest("SiPixelTemplateDBObjectRcd"))
		poolDbService->writeOne( obj, poolDbService->beginOfTime(), "SiPixelTemplateDBObjectRcd");
	else
		poolDbService->writeOne( obj, poolDbService->currentTime(), "SiPixelTemplateDBObjectRcd");
}

void 
SiPixelTemplateDBObjectUploader::endJob()
{
}

