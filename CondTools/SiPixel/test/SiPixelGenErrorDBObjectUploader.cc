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
	
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );

	short generrids[53];
	for(int k = 0; k < 53; k++){
	generrids[k] = (short) thegenerrids[k];
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
						if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[52] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
					    if (layer == 2) {
						if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[53] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
					    if (layer == 3) {
						if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[54] ) )
						std::cout << " Could not fill barrel layer "<<layer<<", ladder "<<ladder<<", module "<<module<<"\n";
						}
				   }
				   else {
					if (layer == 1) {
						if (module == 1) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[0] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";						
						}
						if (module == 2) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[1] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 3) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[2] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 4) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[3] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 5) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[4] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 6) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[5] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 7) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[6] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 8) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[7] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
					
					}
					if (layer == 2) {
						if (module == 1) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[8] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";		
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 2) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[9] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 3) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[10] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 4) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[11] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 5) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[12] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 6) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[13] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 7) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[14] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";											
						}
						if (module == 8) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[15] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
					
					}
					if (layer == 3) {
						if (module == 1) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[16] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 2) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[17] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 3) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[18] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 4) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[19] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 5) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[20] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 6) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[21] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";	
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 7) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[22] ) )
							//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
							std::cout << " Could not fill barrel layer "<<layer<<", module "<<module<<"\n";										
						}
						if (module == 8) {
							if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[23] ) )
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

				//short temp123abc = (short) thegenerrids[1];
				if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)){
					if (side ==1 ){
						if (disk == 1){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[24] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[25] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[26] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[27] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[40] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[41] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[42] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
						if (disk == 2){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[28] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[29] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[30] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[31] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}	
							if(panel == 2){
								if(module == 1){
										if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[43] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[44] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[45] ) )
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
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[32] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[33] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[34] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[35] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[46] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[47] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[48] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}
						}
						if (disk == 2){
							if(panel == 1){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[36] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[37] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[38] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 4){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[39] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
							}	
							if(panel == 2){
								if(module == 1){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[49] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 2){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[50] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									std::cout << " Could not fill barrel det unit"<<side<<", disk "<<disk<<", blade "<<blade<<", panel "<<panel<<" and module "<<module<<".\n";
								}
								if(module == 3){
									if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[51] ) )
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
			mapnum = (*obj).getGenErrorID( detid.rawId());
			std::cout<<"The DetID: "<<detid.rawId()<<" is mapped to the template: "<<mapnum<<".\n\n";

			//else 
				//if ( ! (*obj).putGenErrorID( detid.rawId(),generrids[1] ) )
									//edm::LogInfo("Template Info") << " Could not fill barrel det unit";
									//std::cout << "ERROR! OH NO!\n";
		}
	}
	//for(std::map<unsigned int,short>::const_iterator it=templMap.begin(); it!=templMap.end();++it)
		//std::cout<< "Map:\n"<< "DetId: "<< it->first << " GenErrorID: "<< it->second <<"\n";

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

