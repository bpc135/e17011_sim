//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "e17011_simDetectorConstruction.hh"
#include "e17011_simDetectorMessenger.hh"
#include "e17011_simDetectorSD.hh"
#include "e17011_simAnalysisManager.hh"
#include "G4UImanager.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RotationMatrix.hh"
#include "Materials.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "e17011_simMaterial.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include <sstream>

//CADMesh
//#include "CADMesh.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
e17011_simDetectorConstruction::e17011_simDetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0),
 solidGeThickDetector(0),logicGeThickDetector(0),physiGeThickDetector(0), 
 solidGeThinDetector(0),logicGeThinDetector(0),physiGeThinDetector(0), 
 GeThickDetectorMater(0), GeThinDetectorMater(0), fWorldLength(0)
{
  //  G4cout<<"e17011_simDetectorConstruction"<<G4endl;
  detectorMessenger = new e17011_simDetectorMessenger(this);
  DefineMaterials();

  fGeThickDetectorThickness = 1.0*cm; // 2.0*cm; //Hiro's detector
  fGeThinDetectorThickness = 1.5*mm;
  fGeThickDetectorRadius = 4.5*cm; // 3.0 * cm; //3.0*cm;Hiro's detector
  fGeThinDetectorRadius = 4.5 * cm;
  fSpacing = 5*mm;

  fUseGeThinDetector = false;
  fUseGeThickDetector= false;
  fUseGeThickDetectorCryo= false;
  fUseGeThickDetectorCryoEndCap= false;

  fUseSega = true;
  fUseClover = false;

  //BC - use LaBr3 array
  //these should be set to true but for now I am setting them to false to do some testing
  
  fUseLaBr3 = true;
  fUseLaBr3Frame = true;
  
  //BC - use 3Hen
  fUse3Hen = false;
  
  //BC - use MTC
  fUseMTC = false;
  
  //BC - use LENDA
  fUseLENDA = false;
  
  //BC - 76Ge test
  fUseGetest = false;
  
  //BC - EXO test
  fUseEXOtest = false;
  
  //BC - EJ204 Scint
  //return to true once testing is over
  fUseEJ204Scint = true;
  
  //BC - SiDSSD
  fUseSiDSSD = false;

  //BC - Beam Pipe
  fUsePipe = true;
 
  //BC - PSPMT
  //return to true once testing is over
  fUsePSPMT = true;

  //BC - CeBr3 Scint
  //return to true once testing is over
  fUseCeBr3Scint = true;

  //--------- Sizes of the principal geometrical components (solids)  ---------
  fWorldLength = fGeThickDetectorThickness + 50*cm;
  fWorldRadius = fGeThickDetectorRadius + 50.*cm;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
e17011_simDetectorConstruction::~e17011_simDetectorConstruction()
{
  // G4cout<<"~gedssdDetectorConstruction"<<G4endl;
  delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void e17011_simDetectorConstruction::DefineMaterials()
{
  // G4cout<<"e17011_simDetectorConstruction DefineMaterials"<<G4endl;
  //--------- Material definition ---------

  //   Materials* set_of_materials = new Materials();

  //   // define all the materials
  //   set_of_materials->defineAllMaterials();










  materialsManager = new e17011_simMaterial();
  // Lead
  //   materialsManager->AddMaterial("Lead","Pb",11.3*g/cm3,"");
  //   //Germanium detector
  //   materialsManager->AddMaterial("Germanium","Ge",5.323*g/cm3,""); 
  //   //CsI
  //   materialsManager->AddMaterial("CsI","Cs-I",4.51*g/cm3,"");
  //   //Be
  //   materialsManager->AddMaterial("Beryllium","Be",1.85*g/cm3,"");
  //   //Si
  //   materialsManager->AddMaterial("Silicon","Si",2.33*g/cm3,"");


  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
  DefaultMater = materialsManager->GetMaterial("vacuum");
  //TargetMater  = materialsManager->GetMaterial("CsI");
  GeThickDetectorMater = materialsManager->GetMaterial("Ge");
  GeThinDetectorMater = materialsManager->GetMaterial("Ge");
  GeThickDetectorCryoMater = materialsManager->GetMaterial("Al");
  SegaMater = materialsManager->GetMaterial("Ge");
  SegaCryoMater = materialsManager->GetMaterial("Al");
  SegaVacuumMater = materialsManager->GetMaterial("vacuum");
  CloverCrystalMater = materialsManager->GetMaterial("Ge");
  CloverCryoMater = materialsManager->GetMaterial("Al");
  VacuumMater = materialsManager->GetMaterial("vacuum");
  BCSdssdMater = materialsManager->GetMaterial("Si");
  BCSPlasticMater = materialsManager->GetMaterial("polyethylene");
  //BC - define material for LaBr3
  LaBr3ScintMater = materialsManager->GetMaterial("LaBr3");
  LaBr3CoverMater = materialsManager->GetMaterial("Al");
  //BC - define material for frame (doing this in case it is not just Al at some point)
  LaBr3FrameMater = materialsManager->GetMaterial("Al");
  // BC - define material for 3Hen
  HDPE_3Hen_Mater = materialsManager->GetMaterial("polyethylene");
  Stainless_3Hen_Mater = materialsManager->GetMaterial("StainlessSteel");
  He3_10atm_3Hen_Mater = materialsManager->GetMaterial("He3_10atm");
  tape_35mm_Mater = materialsManager->GetMaterial("CellulousAcetate");
  //BC - LENDA material
  BC408_Mater = materialsManager->GetMaterial("BC408");
  H6410_Mater = materialsManager->GetMaterial("Al");
  //BC - Getest material
  Getest_Mater = materialsManager->GetMaterial("Ge");
  //BC - EXOtest material
  EXOtest_Mater = materialsManager->GetMaterial("LiquidXeEXO");
  //EXOtest_Mater = materialsManager->GetMaterial("Ge");
  //BC - EJ204 Scintillator material
  EJ204Scint_Mater = materialsManager->GetMaterial("EJ204");
  //BC - PSPMT material... for now just the EJ204 material
  // PSPMT_Mater = materialsManager->GetMaterial("EJ204");
  //PSPMT_Mater = materialsManager->GetMaterial("Al");
  PSPMT_Mater = materialsManager->GetMaterial("StainlessSteel");  //trying stainless
  PSPMTWindow_Mater = materialsManager->GetMaterial("Borosilicate glass");
  PSPMTCathode_Mater = materialsManager->GetMaterial("Bialkali");
  //BC - SiDSSD just made out of Si
  SiDSSD_Mater = materialsManager->GetMaterial("Si");
  //BC - CeBr3 Scintillator material
  CeBr3Scint_Mater = materialsManager->GetMaterial("CeBr3");
  
  

  //std::cout << "I am in detector construction" < std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* e17011_simDetectorConstruction::Construct()
{
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  //--------- Sizes of the principal geometrical components (solids)  ---------
  //G4cout<<"gedssdDetectorConstruction Contruct"<<G4endl;
 
  fWorldLength = fGeThickDetectorThickness + 400*cm;
  fWorldRadius = fGeThickDetectorRadius + 1500.*cm;
   
  //------------------------------ 
  // World
  //------------------------------ 

  solidWorld= new G4Tubs("world",0.,fWorldRadius,fWorldLength/2.,0.,twopi);
  logicWorld= new G4LogicalVolume( solidWorld, DefaultMater, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
	                        			 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // no field specific to volume




  // using tigress ----------
  //   G4SDManager *SDman = G4SDManager::GetSDMpointer();
  //   DetectionSystemTigress *myTigress = new DetectionSystemTigress(SDman,12,0,false);
  //   myTigress->Build(logicWorld);
  // ------------------------



				 
  //------------------------------
  // Ge Double Sided Strip Detector
  // There are two GeDSSD in the simulation
  // One is a thick 1.5 cm detector
  // the second is a thin 1.5 mm detector
  // The thick detector will be used to determine
  // the ease of detecting an impanted ion and decay
  // in the same detector.
  // If it proves too difficult a thin implantation Ge detector
  // can be used in front of the thick detector.
  //------------------------------

  //------------------------------ 
  // Ge Double Sided Strip Detector
  // create the thick Ge detector
  // pixelation will occur during at end of event during sensitive detector readout
  //------------------------------
  
  //position of the thin detector is in front of the thick detector
  fGeThinDetectorPos = G4ThreeVector(0,0,-(fGeThickDetectorThickness/2.)-(fGeThinDetectorThickness/2.)-fSpacing);
  
  solidGeThinDetector = new G4Tubs("GeThinDetector",0,fGeThinDetectorRadius,fGeThinDetectorThickness/2.,0.,twopi);
  logicGeThinDetector = new G4LogicalVolume(solidGeThinDetector ,GeThinDetectorMater, "GeThinDetector",0,0,0);  

  //only place the detector in the world if the user has interactively requested it
  if(fUseGeThinDetector){
    physiGeThinDetector = new G4PVPlacement(0,                  // no rotation
					                                  fGeThinDetectorPos, // at (x,y,z)
					                                  logicGeThinDetector,    // its logical volume				  
					                                  "GeThinDetector",       // its name
					                                  logicGeThickDetectorCryo,      // its mother  volume
					                                  false,              // no boolean operations
					                                  0);                 // no particular field 
  }

  fGeThickDetectorCryoPos = G4ThreeVector(0.5*cm,0,0); //e11503
  // fGeThickDetectorCryoPos = G4ThreeVector(0,0,0);
  fGeThickDetectorCryoOutRad = 9.6393*cm; //diagram from Ethan
  //fGeThickDetectorCryoInnRad = 7.0*cm; //estimated from Ethan's diagram (in worst case cryo is thick solid Al)
  fGeThickDetectorCryoInnRad = 6.6*cm;
  fGeThickDetectorCryoThickness = 8.89*cm;//measured via tape measure// 3.0 cm used for radioxenon
  solidGeThickDetectorCryo = new G4Tubs("GeThickDetectorCryo", fGeThickDetectorCryoInnRad, fGeThickDetectorCryoOutRad, fGeThickDetectorCryoThickness/2.,0.,twopi);
  logicGeThickDetectorCryo = new G4LogicalVolume(solidGeThickDetectorCryo, GeThickDetectorCryoMater,  "GeThickDetectorCryo",0,0,0);
  //logicGeThickDetectorCryo = new G4LogicalVolume(solidGeThickDetectorCryo,GeThickDetectorCryoMater,
  //	 "GeThickDetectorCryo",-0.5,1.0,0);//e11503

  fGeCryoEndCapOutRad = fGeThickDetectorCryoInnRad;
  fGeCryoEndCapThickness = 1.143*mm; //use 0.1 cm for radioxenon

  //fGeCryoEndCapPos1 = G4ThreeVector(0,0,fGeThickDetectorCryoThickness/2. - fGeCryoEndCapThickness/2.);
  //fGeCryoEndCapPos2 = G4ThreeVector(0,0,-fGeThickDetectorCryoThickness/2. + fGeCryoEndCapThickness/2.);

  fGeCryoEndCapPos1 = G4ThreeVector(0.5*cm,0,fGeThickDetectorCryoThickness/2. - fGeCryoEndCapThickness/2.);//e11503
  fGeCryoEndCapPos2 = G4ThreeVector(0.5*cm,0,-fGeThickDetectorCryoThickness/2. + fGeCryoEndCapThickness/2.);//e11503
 
  solidGeCryoEndCap = new G4Tubs("GeCryoEndCap",0.,fGeCryoEndCapOutRad,fGeCryoEndCapThickness/2.,0.,twopi);
  logicGeCryoEndCap = new G4LogicalVolume(solidGeCryoEndCap,GeThickDetectorCryoMater,"GeCryoEndCap",0,0,0);
  // logicGeCryoEndCap = new G4LogicalVolume(solidGeCryoEndCap,GeThickDetectorCryoMater,"GeCryoEndCap",-0.5,1.0,0);//e11503

  if(fUseGeThickDetectorCryoEndCap){
    physiGeCryoEndCap = new G4PVPlacement(0,
					  fGeCryoEndCapPos1,
					  logicGeCryoEndCap,
					  "GeCryoEndCap",
					  logicWorld,
					  false,
					  0);
    physiGeCryoEndCap = new G4PVPlacement(0,
					  fGeCryoEndCapPos2,
					  logicGeCryoEndCap,
					  "GeCryoEndCap",
					  logicWorld,
					  false,
					  1);
  }
  // GeDSSD for e11503 runs 49/50-detector was off center
  fGeThickDetectorPos = G4ThreeVector(0.5*cm,0,0);
  //position of thick detector is at origin of world
  //fGeThickDetectorPos = G4ThreeVector(0,0,0);
  
  solidGeThickDetector = new G4Tubs("GeThickDetector",0,fGeThickDetectorRadius,fGeThickDetectorThickness/2.,0.,twopi);
  logicGeThickDetector = new G4LogicalVolume(solidGeThickDetector ,GeThickDetectorMater, "GeThickDetector",0,0,0);  
  //only place the detector in the world if the user has interactively requested it
  // logicGeThickDetector = new G4LogicalVolume(solidGeThickDetector ,GeThickDetectorMater, "GeThickDetector",-0.50,1.0,0);  //e11503
  if(fUseGeThickDetector){
              physiGeThickDetector = new G4PVPlacement(0,              // no rotation
					    fGeThickDetectorPos, // at (x,y,z)
					    logicGeThickDetector,    // its logical volume				  
					    "GeThickDetector",       // its name
					    logicWorld/*GeThickDetectorCryo*/,      // its mother  volume
					    false,           // no boolean operations
					    0);              // no particular field 
  }

  if(fUseGeThickDetectorCryo){
              physiGeThickDetectorCryo = new G4PVPlacement(0,
						  fGeThickDetectorCryoPos,
						  logicGeThickDetectorCryo,
						  "GeThickDetectorCryo",
						  logicWorld,
						  false,
						  0);
  }

  //   //creation of BCS beam pipe
  /*solidBCSpipe = new G4Tubs("BCSpipe",0,7.7*cm,50.8*cm/2.,0., twopi);
   logicBCSpipe = new G4LogicalVolume(solidBCSpipe,SegaCryoMater,"BCSpipe",0,0,0);
   physiBCSpipe = new G4PVPlacement(0,
 				   G4ThreeVector(0,0,0),
 				   logicBCSpipe,
 				   "BCSpipe",
 				   logicWorld,
 				   false,
 				   0);
//   //creation of BCS beam vac
   /*solidBCSvac = new G4Tubs("BCSvac",0,7.40*cm,50.8*cm/2.,0., twopi);
   logicBCSvac = new G4LogicalVolume(solidBCSvac,SegaVacuumMater,"BCSvac",0,0,0);
   physiBCSvac = new G4PVPlacement(0,
 				   G4ThreeVector(0,0,0),
 				   logicBCSvac,
 				   "BCSvac",
 				   logicBCSpipe,
 				   false,
 				   0);

   //end of beamline
   solidBeamEndCap = new G4Tubs("BeamEndCap",0,7.8*cm, 1.0*cm,0.,twopi);
   logicBeamEndCap = new G4LogicalVolume(solidBeamEndCap, SegaCryoMater,"BeamEndCap", 0,0,0);
   physiBeamEndCap = new G4PVPlacement(0,
 				   G4ThreeVector(0,0,50.8*cm/2.+1.0*cm),
 				   logicBeamEndCap,
 				   "BCSvac",
 				   logicWorld,
 				   false,
 				   0);

   //creation of plastic blocks behind GeDSSD position
   fBCSPlasticlength=0.85*cm // 1.0*/;
  /*fBCSPlasticSpacing=3.175*cm;
   solidBCSPlastic = new G4Box("BCSPlastic",fBCSPlasticlength, fBCSPlasticlength, 6.6675*cm/2.);
   logicBCSPlastic = new G4LogicalVolume(solidBCSPlastic,BCSPlasticMater,"BCSPlastic",0,0,0);
   //need to create four of them, one centered at each corner of where DSSD would be
   physiBCSPlastic = new G4PVPlacement(0,
				   G4ThreeVector(fBCSPlasticSpacing-fBCSPlasticlength,fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   0);

   physiBCSPlastic = new G4PVPlacement(0,
				   G4ThreeVector(fBCSPlasticSpacing-fBCSPlasticlength,fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   4);

   physiBCSPlastic = new G4PVPlacement(0,
				   G4ThreeVector(fBCSPlasticSpacing+fBCSPlasticlength,fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   5);


   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(fBCSPlasticSpacing+fBCSPlasticlength,-fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   1);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(fBCSPlasticSpacing-fBCSPlasticlength,-fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   6);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(fBCSPlasticSpacing-fBCSPlasticlength,-fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   7);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing+fBCSPlasticlength,-fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   2);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing+fBCSPlasticlength,-fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   8);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing-fBCSPlasticlength,-fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   9);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing-fBCSPlasticlength,fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
 				   3);

   physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing+fBCSPlasticlength,fBCSPlasticSpacing-fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
				   10);
 physiBCSPlastic = new G4PVPlacement(0,
 				   G4ThreeVector(-fBCSPlasticSpacing+fBCSPlasticlength,fBCSPlasticSpacing+fBCSPlasticlength,6.6675*cm/2.),
 				   logicBCSPlastic,
 				   "BCSPlastic",
 				   logicBCSvac,
 				   false,
				   11);*/
 

 /* //   // creation of BCS detector
   //solidBCSdssd = new G4Box("BCSdssd",4.*cm/2, 4.*cm/2., 1.*cm/2.);
   solidBCSdssd = new G4Box("BCSdssd",7.9375*cm/2, 7.9375*cm/2., 1.*cm/2.);
   //   //solidBCSdssd = new G4Tubs("BCSdssd",0 , 1.*cm , 0.05*cm , 0. , twopi);
   logicBCSdssd = new G4LogicalVolume(solidBCSdssd,BCSdssdMater,"BCSdssd",0,0,0);
   physiBCSdssd = new G4PVPlacement(0,
 				   G4ThreeVector(0,0,0),
 				   logicBCSdssd,
 				   "BCSdssd",
 				   logicWorld,
 				   false,
 				   0);*///possibly don't actually want DSSD in there, just beam pipe
   //create a PIN in front of source
   // solidPIN = new G4Box("PIN",5.*cm/2, 5.*cm/2., 0.05*cm/2.);
   //logicPIN = new G4LogicalVolume(solidPIN,BCSdssdMater,"PIN",0,0,0);
   // physiPIN = new G4PVPlacement(0,
   //				   G4ThreeVector(0,0,-0.5*cm),
   //			   logicPIN,
   //			   "PIN",
   //			   logicWorld,
   //			   false,
   //			0);


  // creation of the SeGA detectors.  From outside to inside the SeGA detector consists of a cryostat
  // followed by a vacuum region, detector holder, vacuum region, crystal, active area of the crystal, and central contact.
  // All positions and thickness are derived from PFM MCNP input file
  fsegacryolength = 8.56*cm; //initial values
  // fsegacryothick  = 0.15*cm;
  //fsegacryooutrad = 4.35*cm;
  //trying to match from Ragnar's sim
  fsegacryothick =0.10*cm;
  fsegacryooutrad = 4.325*cm;

  //fsegacryooutrad = 4.385*cm;//to accomodate increased crystal thickness
  //fsegacryothick  = 0.25*cm; //try thicker cryo
  //fsegacryolength = 8.5*cm +fsegacryothick  ;
  //fsegacryothick  = 0.30*cm;
  //fsegacryooutrad = 4.535*cm; //diagram
  //fsegacryooutrad = 4.50*cm; 
  G4cout << " radius " << fsegacryooutrad << G4endl;
  solidSegaCryo = new G4Tubs("SegaCryo",0,fsegacryooutrad,fsegacryolength/2. + 0.05*mm,0.,twopi);
  logicSegaCryo = new G4LogicalVolume(solidSegaCryo,SegaCryoMater,"SegaCryo",0,0,0);

  //fsegavac2length = 8.5*cm; //initial
  fsegavac2length = fsegacryolength - fsegacryothick;
  // fsegavac2thick = 0.485*cm;
  //fsegavac2thick = 0.45*cm;//to accomodate thicker crys without changing radius
  //fsegavac2thick = 0.6*cm-fsegacryothick;//adjust for thicker cup by making this thinner
  fsegavac2thick =0.445*cm;//like Ragnar's sim
  G4double foutrad = fsegacryooutrad - fsegacryothick;
  G4cout << " radius1 " << foutrad << G4endl;
  fSegaVac2Pos = G4ThreeVector(0,0,-(fsegacryolength/2. - fsegavac2length/2.));
  solidSegaVac2 = new G4Tubs("SegaVac2",0,foutrad,fsegavac2length/2.,0.,twopi);
  logicSegaVac2 = new G4LogicalVolume(solidSegaVac2,SegaVacuumMater,"SegaVac2",0,0,0);
  physiSegaVac2 = new G4PVPlacement(0,
				    fSegaVac2Pos,
				    logicSegaVac2,
				    "SegaVac2",
				    logicSegaCryo,
				    false,
				    0);

  // fsegadetcuplength = 8.1*cm; //inital value
  fsegadetcuplength = fsegavac2length - fsegavac2thick;
  fsegadetcupthick = 0.098*cm;//inital value
  // fsegadetcupthick = 0.125*cm;
  //fsegadetcupthick = 0.25*cm;

  //fsegadetcupthick =0.1*cm;//like Ragnar's sim
  foutrad = foutrad - fsegavac2thick;
  G4cout << " radius2 " << foutrad << G4endl;
  fSegaDetCupPos = G4ThreeVector(0,0,-(fsegavac2length/2. - fsegadetcuplength/2.));
  solidSegaDetCup = new G4Tubs("SegaDetCup",0,foutrad,fsegadetcuplength/2.,0.,twopi);
  logicSegaDetCup = new G4LogicalVolume(solidSegaDetCup,SegaCryoMater,"SegaDetCup",0,0,0);
  physiSegaDetCup = new G4PVPlacement(0,
				      fSegaDetCupPos,
				      logicSegaDetCup,
				      "SegaDetCup",
				      logicSegaVac2,
				      false,
				      0);

  // fsegavac1length = 8.0*cm; //initial value
  fsegavac1length = fsegadetcuplength - fsegadetcupthick;
  fsegavac1thick = 0.152*cm;//initial value


  //fsegavac1thick = 0.125*cm;//adjust for thicker cup by making this thinner
  // fsegavac1thick = 0.293*cm-0.113*cm-0.05*cm;//like Ragnar's sim-subtracting .113 to account for making crystal 3.5 cm thick, where it's 3.387 in Ragnar's sim and for thicker inner cryo
  foutrad = foutrad - fsegadetcupthick;
  G4cout << " radius3 " << foutrad << G4endl;
  fSegaVac1Pos = G4ThreeVector(0,0,-(fsegadetcuplength/2. - fsegavac1length/2.));
  solidSegaVac1 = new G4Tubs("SegaVac1",0,foutrad,fsegavac1length/2.,0.,twopi);
  logicSegaVac1 = new G4LogicalVolume(solidSegaVac1,SegaVacuumMater,"SegaVac1",0,0,0);
  physiSegaVac1 = new G4PVPlacement(0,
				    fSegaVac1Pos,
				    logicSegaVac1,
				    "SegaVac1",
				    logicSegaDetCup,
				    false,
				    0);

  //  fsegacrystallength = 8.0*cm; //initial value
  fsegacrystallength = fsegavac1length-fsegavac1thick;
  // fsegacrystalthick = 0.1*cm;//initial value
  //fsegacrystalthick = 0.06*cm;
  // fsegacrystalthick = 0.03*cm;//like Ragnar's sim  //original in nrl sim
  fsegacrystalthick = 0.01*cm;
  
  //  fsegacrystalthick = 0.09*cm; //try thicker 
  foutrad = foutrad - fsegavac1thick;
  G4cout << " radius4 " << foutrad << G4endl;
  fSegaCrystalPos = G4ThreeVector(0,0,-(fsegavac1length/2. - fsegacrystallength/2.));
  solidSegaCrystal = new G4Tubs("SegaCrystal",0,foutrad,fsegacrystallength/2.,0.,twopi);
  logicSegaCrystal = new G4LogicalVolume(solidSegaCrystal,SegaMater,"SegaCrystal",0,0,0);
  physiSegaCrystal = new G4PVPlacement(0,
				       fSegaCrystalPos,
				       logicSegaCrystal,
				       "SegaCrystal",
				       logicSegaVac1,
				       false,
				       0);

  // fsegaactivelength = 7.9*cm; //initial value
  fsegaactivelength = fsegacrystallength - fsegacrystalthick;
  // fsegaactivethick =3.0*cm-fsegacrystalthick-0.2*cm;//0.4 accounts for additional dead layer
  // fsegaactivethick = 2.9*cm;//to match crystal dimensions in Mueller01
  //fsegaactivethick = 2.5*cm;
  fsegaactivethick = 2.865*cm;//initial values
  //fsegaactivethick = 2.75*cm;

  
  foutrad = foutrad - fsegacrystalthick;
  G4cout << " radius5 " << foutrad << G4endl;
  fSegaActivePos = G4ThreeVector(0,0,-(fsegacrystallength/2. - fsegaactivelength/2.));
  solidSegaActiveArea = new G4Tubs("SegaActiveArea",0,foutrad,fsegaactivelength/2.,0.,twopi);
  logicSegaActiveArea = new G4LogicalVolume(solidSegaActiveArea,SegaMater,"SegaActiveArea",0,0,0);
  physiSegaActiveArea = new G4PVPlacement(0,
					  fSegaActivePos,
					  logicSegaActiveArea,
					  "SegaActiveArea",
					  logicSegaCrystal,
					  false,
					  0);

  //adding a second dead layer like Ragnar
  fsegaDLinnerlength = 0.875*fsegacrystallength;//just make it around central contact
  //  fsegaDLinnerlength = 7.0*cm;//just make it around central contact original value
  //fsegaDLinnerthick = 0.2*cm;//like Ragnar's sim
  fsegaDLinnerthick = 0.175*cm;
  // fsegaDLinnerthick = 0.5*cm;

  foutrad = foutrad - fsegaactivethick;
  G4cout << " radius5 " << foutrad << G4endl;
  fSegaDLinnerPos = G4ThreeVector(0,0,-(fsegaactivelength/2. - fsegaDLinnerlength/2.));
  solidSegaDLinner = new G4Tubs("SegaInnerDeadLayer",0,foutrad,fsegaDLinnerlength/2.,0.,twopi);
  logicSegaDLinner = new G4LogicalVolume(solidSegaDLinner,SegaMater,"SegaCrystal",0,0,0);
  physiSegaDLinner = new G4PVPlacement(0,
				       fSegaDLinnerPos,
				       logicSegaDLinner,
				       "SegaInnerDeadLayer",
				       logicSegaActiveArea,
				       false,
				       0);
 
  // fsegacentralconlength = 7.0*cm; //initial value
  fsegacentralconlength = fsegaDLinnerlength - fsegaDLinnerthick;
  fsegacentralconthick = 0.5*cm;
  // foutrad = foutrad - fsegaactivethick;
  foutrad = foutrad -fsegaDLinnerthick;//changes with addition of new dead layer
  G4cout << " radius6 " << foutrad << G4endl;
  //fSegaCentralConPos = G4ThreeVector(0,0,-(fsegaactivelength/2. - fsegacentralconlength/2.));
  fSegaCentralConPos = G4ThreeVector(0,0,-(fsegaDLinnerlength/2. - fsegacentralconlength/2.));//changes with addition of new dead layer
  solidSegaCentralCon = new G4Tubs("SegaCentralCon",0,foutrad,fsegacentralconlength/2.,0.,twopi);
  logicSegaCentralCon = new G4LogicalVolume(solidSegaCentralCon,SegaVacuumMater,"SegaCentralCon",0,0,0);
  physiSegaCentralCon = new G4PVPlacement(0,
					  fSegaCentralConPos,
					  logicSegaCentralCon,
					  "SegaCentralCon",
					  //  logicSegaActiveArea,
					  logicSegaDLinner,//also changes with addition of new dead layer
					  false,
					  0);

  //place Sega detectors into a ring around GeDSSD if requested by user
  if(fUseSega){

    G4int nsega = 8;
    G4double dtheta = twopi/nsega;
    G4double theta = 0;
    //G4double radius = fsegacryooutrad + 8.65*cm;//SLD
    G4double radius = 13.19*cm;//keep centers constant regardless of how I change SeGA internal details
    //G4double radius = fsegacryooutrad + 8.84*cm; // stand drawing
    //G4double radius = fsegacryooutrad + 9.0*cm; //update based on Chris picture 2/20/13
    //G4double radius = fsegacryooutrad + 7.8*cm; // for BCS mode
    //fGeThickDetectorRadius + fsegacryooutrad + 1.0*cm;  //use 1.5 for 60 mm detector with 13th Ge, use 0.55 without 13th, use 1 *cm with 90 mm detecotr and 6 sega
    for(G4int n=0; n < nsega; n++){
      
      //theta = dtheta * (n+0.5);
      theta = pi/2.0 - dtheta * (n+0.5);
      G4double fSegaxpos = radius*cos(theta);
      G4double fSegaypos = radius*sin(theta); 
      // G4double fSegazpos = fsegacryolength/2.; //try to match the service descrip
      //  G4double fSegazpos = 11.13*cm;//measured from set up e11503 runs 49/50
      //G4double fSegazpos = 9.75*cm;  //e14039
      //fSegazpos += 0.3*cm;  //try moving back by 0.25 cm - e14057
      //  G4double fSegazpos =fsegacryolength/2.+ 0.60*cm; //try to match the runs from the end of e14057,e14039 campaign

      //  G4double fSegazpos =fsegacryolength/2.+ 3.25*cm; //try to match the runs from the end of e14057,e14039 campaign THIS ONE WORKS!!
      G4double fSegazpos = 9.8*cm;  //e14057
      //G4double fSegazpos = 10.7*cm;  //e14039

      //add spacers in downstream ring - e14057
      //while we did add spacers in, the simulation makes it seem like the effect was to put them in 
      //line with all other SeGA detectors, not further out (the spacers were added so SeGA detectors 
      //would not touch the LaBr3 frame)
      // if(n == 2 || n == 5){
      // 	// 1/16" spacers
      // 	fSegazpos += 0.15875*cm;
      // }
      // if(n == 3 || n == 4){
      // 	// 1/8" spacers
      // 	fSegazpos += 0.3175*cm;
      // }
      
      //   G4double fSegazpos = fsegacryolength/2. + 6.805*cm;//adjust for the slightly longer cryo length to make uniform cryo on all sides
      G4RotationMatrix *rotatesega = new G4RotationMatrix();
      rotatesega->rotateX(180.*deg);
      physiSegaCryo = new G4PVPlacement(rotatesega,
					G4ThreeVector(fSegaxpos,fSegaypos,fSegazpos),
					logicSegaCryo,
					"SegaCryo",
					logicWorld,
					false,
					n);
    }
    
    //half of sega
    for(G4int n=nsega; n < nsega+nsega; n++){
      
      //theta = dtheta * (0.5+n-nsega);
      theta = pi/2.0 + dtheta * (0.5+n-nsega);
      G4double fSegaxpos = radius*cos(theta);
      G4double fSegaypos = radius*sin(theta); 
      //   G4double fSegazpos =-fsegacryolength/2.- 0.25*cm; //try to match the service descrip
      //    G4double fSegazpos = -9.48*cm;//measured from set up e11503 runs 49/50
      //G4double fSegazpos = -9.75*cm;  //e14039
      //fSegazpos -= 0.3*cm;  //try moving back by 0.25 cm - e14057
      // G4double fSegazpos =-fsegacryolength/2.- 0.60*cm; //try to match the runs from the end of e14057,e14039 campaign

      // G4double fSegazpos =-fsegacryolength/2.- 3.250*cm; //try to match the runs from the end of e14057,e14039 campaign THIS ONE WORKS!!
      G4double fSegazpos = -9.78*cm;  //e14057
      // G4double fSegazpos = -10.7*cm; //e14039

      // G4double fSegazpos = -8.78*cm; //based on SeGA mech design
      //  G4double fSegazpos = -fsegacryolength/2. -4.455*cm;
      physiSegaCryo = new G4PVPlacement(0,
					G4ThreeVector(fSegaxpos,fSegaypos,fSegazpos),
					logicSegaCryo,
					"SegaCryo",
					logicWorld,
					false,
					n);
    }
   

    //   G4double fSegaxpos = 0;
    //   G4double fSegaypos = 0; 
    //   G4double fSegazpos = -8.0*cm; 
    //   physiSegaCryo = new G4PVPlacement(0,
    // 				    G4ThreeVector(fSegaxpos,fSegaypos,fSegazpos),
    // 				    logicSegaCryo,
    // 				    "SegaCryo",
    // 				    logicWorld,
    // 				    false,
    // 				    12);
  }

  


  //---------------------
  // Creation of a compact clover array

  // base clover geometry on detectors from Yale 

  // length - , width - , height - ,

  // Cryostat will be a simple shell of Al 2 mm thick all around sides of detector and 1 mm in front
  G4cout << " TIGRESS1 " << G4endl;

  G4double CloverCrystalLength = 80.0*mm;
  //G4double CloverCrystalRadius = 25.0*mm; //union of cylinder and box geom
  G4double CloverCrystalRadius = 22.5*mm; // simple cyclinder geom

  // A clover crystal will be constructed from two boolean solids.
  // Boolean1 will be a large simple cylinder of Ge
  // Boolean2 will be a retangular box
  // The crystal will be constructed from the intersection of the cylinder with the box providing a cylinder with flat edges.

  //start Clover construction with a simple cylinder
  solidCloverBool1 = new G4Tubs("CloverBool1",0,CloverCrystalRadius,CloverCrystalLength/2.,0,twopi);
  
  // continue with box
  G4double CloverCrystalBoxx = 45.*mm;
  G4double CloverCrystalBoxy = 45.*mm;
  G4double CloverCrystalBoxz = CloverCrystalLength;
  solidCloverBool2 = new G4Box("CloverBool2",CloverCrystalBoxx/2.,CloverCrystalBoxy/2.,CloverCrystalBoxz/2.);

  //solidCloverCrystal = new G4IntersectionSolid("CloverCrystal",solidCloverBool2,solidCloverBool1);


  //per V.W. clover is  a collection of simple cylinders not as drawn on documentation
  solidCloverCrystal = new G4Tubs("",0,CloverCrystalRadius,CloverCrystalLength/2.,0,twopi);
  logicCloverCrystal = new G4LogicalVolume(solidCloverCrystal,CloverCrystalMater,"CloverCrystal",0,0,0);

  // create the central core
  G4double CloverCoreRadius = 5.0*mm;
  G4double CloverCoreLength = CloverCrystalLength;
  solidCloverCore = new G4Tubs("CloverCore",0,CloverCoreRadius,CloverCoreLength/2.,0,twopi);
  logicCloverCore = new G4LogicalVolume(solidCloverCore,VacuumMater,"CloverCore",0,0,0);
  G4ThreeVector CloverCorePos = G4ThreeVector(0,0,0);
  physiCloverCore = new G4PVPlacement(0,
				      CloverCorePos,
				      logicCloverCore,
				      "CloverCore",
				      logicCloverCrystal,
				      false,
				      0);

  //create clover cryostat
  G4double CloverCryox = 101.0*mm;
  G4double CloverCryoy = 101.0*mm;
  G4double CloverCryoz = CloverCrystalLength + 20.0*mm;

  //test non standard dimensions
  // CloverCryoy = 151.0*mm;
  // CloverCryoz += 100.0*mm;

  solidCloverCryo = new G4Box("CloverCryo",CloverCryox/2.,CloverCryoy/2.,CloverCryoz/2.);
  logicCloverCryo = new G4LogicalVolume(solidCloverCryo,CloverCryoMater, "CloverCryo",0,0,0);

  //create interior vacuum region in cryostat.  Assume wall thickness of 2mm Al for sides and 1 mm Al for face
  G4double CloverCryoVacx = CloverCryox - 4.0*mm;
  G4double CloverCryoVacy = CloverCryoy - 4.0*mm;
  G4double CloverCryoVacz = CloverCryoz - 2.0*mm;
  
  solidCloverCryoVac = new G4Box("CloverCryoVac",CloverCryoVacx/2.,CloverCryoVacy/2.,CloverCryoVacz/2.);
  logicCloverCryoVac = new G4LogicalVolume(solidCloverCryoVac,VacuumMater,"CloverCryoVac",0,0,0);

  //place the vacuum into the cryostat
  CloverCryoVacuumPos = G4ThreeVector(0,0,0);
  physiCloverCryoVac = new G4PVPlacement(0,
					 CloverCryoVacuumPos,
					 logicCloverCryoVac,
					 "CloverCryoVac",
					 logicCloverCryo,
					 false,
					 0);

  //place four crystals into the vacuum region of the cryostat
  CloverCrystalPos = G4ThreeVector(CloverCrystalBoxx/2., CloverCrystalBoxy/2.,0);
  physiCloverCrystal = new G4PVPlacement(0,
					 CloverCrystalPos,
					 logicCloverCrystal,
					 "CloverCrystal",
					 logicCloverCryoVac,
					 false,
					 0);

  CloverCrystalPos = G4ThreeVector(-CloverCrystalBoxx/2., CloverCrystalBoxy/2.,0);
  physiCloverCrystal = new G4PVPlacement(0,
					 CloverCrystalPos,
					 logicCloverCrystal,
					 "CloverCrystal",
					 logicCloverCryoVac,
					 false,
					 1);

  CloverCrystalPos = G4ThreeVector(-CloverCrystalBoxx/2., -CloverCrystalBoxy/2.,0);
  physiCloverCrystal = new G4PVPlacement(0,
					 CloverCrystalPos,
					 logicCloverCrystal,
					 "CloverCrystal",
					 logicCloverCryoVac,
					 false,
					 2);

  CloverCrystalPos = G4ThreeVector(CloverCrystalBoxx/2., -CloverCrystalBoxy/2.,0);
  physiCloverCrystal = new G4PVPlacement(0,
					 CloverCrystalPos,
					 logicCloverCrystal,
					 "CloverCrystal",
					 logicCloverCryoVac,
					 false,
					 3);



  G4cout << " TIGRESS2 " << G4endl;

  
  if(fUseClover){

    //place nine clover detectors and the GeDSSD

    //G4RotationMatrix *rotatetemp = new G4RotationMatrix();
    //rotatetemp->rotateY(90.*deg);

    
    // first clover directly behind the GeDSSD
    CloverCryoPos = G4ThreeVector(0,0,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.5*cm);
    //CloverCryoPos = G4ThreeVector(CloverCryox/2.,CloverCryoy/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.5*cm);
    // physiCloverCryo = new G4PVPlacement(0,
    // 					CloverCryoPos,
    // 					logicCloverCryo,
    // 					"CloverCryo",
    // 					logicWorld,
    // 					false,
    // 					0);

    //squared geometry
//     CloverCryoPos = G4ThreeVector(-CloverCryox/2.,CloverCryoy/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.5*cm);
//     physiCloverCryo = new G4PVPlacement(0,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 1);

//     CloverCryoPos = G4ThreeVector(-CloverCryox/2.,-CloverCryoy/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.5*cm);
//     physiCloverCryo = new G4PVPlacement(0,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 3);
 
//     CloverCryoPos = G4ThreeVector(CloverCryox/2.,-CloverCryoy/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.5*cm);
//     physiCloverCryo = new G4PVPlacement(0,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 2);
    //end square geometry

//     /**** explode geometry ****/
//     G4RotationMatrix *rotateCloverex1 = new G4RotationMatrix();
//     rotateCloverex1->rotateX(-6.*deg);
//     G4RotationMatrix *rotateCloverex2 = new G4RotationMatrix();
//     rotateCloverex2->rotateX(6.*deg);
//     G4RotationMatrix *rotateCloverey1 = new G4RotationMatrix();
//     rotateCloverey1->rotateY(-6.*deg);
//     G4RotationMatrix *rotateCloverey2 = new G4RotationMatrix();
//     rotateCloverey2->rotateY(6.*deg);
    
//     CloverCryoPos = G4ThreeVector(0,CloverCryoy/2.+ CloverCryoz/2.+1.2*cm,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
//     physiCloverCryo = new G4PVPlacement(rotateCloverex1,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 1);

//     CloverCryoPos = G4ThreeVector(0,-CloverCryoy/2.- CloverCryoz/2.-1.2*cm,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
//     physiCloverCryo = new G4PVPlacement(rotateCloverex2,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 3);
 
//     CloverCryoPos = G4ThreeVector(CloverCryox/2.+ CloverCryoz/2.+1.2*cm,0,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
//     physiCloverCryo = new G4PVPlacement(rotateCloverey1,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 2);

//     CloverCryoPos = G4ThreeVector(-CloverCryox/2.-CloverCryoz/2.-1.2*cm,0,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
//     physiCloverCryo = new G4PVPlacement(rotateCloverey2,
// 					 CloverCryoPos,
// 					 logicCloverCryo,
// 					 "CloverCryo",
// 					 logicWorld,
// 					 false,
// 					 4);
//     /**** end explode geometry ****/  




    /**** cross ****/
    G4RotationMatrix *rotateCloverx = new G4RotationMatrix();
    rotateCloverx->rotateX(90.*deg);
    G4RotationMatrix *rotateClovery = new G4RotationMatrix();
    rotateClovery->rotateY(90.*deg);
    G4RotationMatrix *rotateCloverz = new G4RotationMatrix();
    
    
    // CloverCryoPos = G4ThreeVector(0,CloverCryoy/2.+ CloverCryoz/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
    // physiCloverCryo = new G4PVPlacement(rotateCloverx,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 1);

    // CloverCryoPos = G4ThreeVector(0,-CloverCryoy/2.- CloverCryoz/2.,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
    // physiCloverCryo = new G4PVPlacement(rotateCloverx,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 3);
 
    // CloverCryoPos = G4ThreeVector(CloverCryox/2.+ CloverCryoz/2.,0,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
    // physiCloverCryo = new G4PVPlacement(rotateClovery,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 2);

    // CloverCryoPos = G4ThreeVector(-CloverCryox/2.-CloverCryoz/2.,0,CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm);
    // physiCloverCryo = new G4PVPlacement(rotateClovery,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 4);
    // /**** end cross ****/  

    // //CloverCryoPos = G4ThreeVector(0,CloverCryoy/2.+ CloverCryoz/2.,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // CloverCryoPos = G4ThreeVector(0,CloverCryoy/2.+ CloverCryoz/2.+1*mm,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // physiCloverCryo = new G4PVPlacement(rotateCloverx,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 5);

    // // CloverCryoPos = G4ThreeVector(0,-CloverCryoy/2.- CloverCryoz/2.,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // CloverCryoPos = G4ThreeVector(0,-CloverCryoy/2.- CloverCryoz/2.-1*mm,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // physiCloverCryo = new G4PVPlacement(rotateCloverx,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 7);

    // // CloverCryoPos = G4ThreeVector(CloverCryox/2.+CloverCryoz/2,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // //CloverCryoPos = G4ThreeVector(1*mm+CloverCryox/2.+CloverCryoz/2,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // //CloverCryoPos = G4ThreeVector(CloverCryoz/2.,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // CloverCryoPos = G4ThreeVector(2.*cm + CloverCryoz/2.,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // physiCloverCryo = new G4PVPlacement(rotateClovery,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 6);

    // //CloverCryoPos = G4ThreeVector(-CloverCryox/2.-CloverCryoz/2,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // //CloverCryoPos = G4ThreeVector(-1*mm-CloverCryox/2.-CloverCryoz/2,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // // CloverCryoPos = G4ThreeVector(- CloverCryoz/2.,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // CloverCryoPos = G4ThreeVector(-2.*cm - CloverCryoz/2.,0,-(CloverCryoz/2. + fGeThickDetectorCryoThickness/2. + 0.8*cm));
    // physiCloverCryo = new G4PVPlacement(rotateClovery,
    // 					 CloverCryoPos,
    // 					 logicCloverCryo,
    // 					 "CloverCryo",
    // 					 logicWorld,
    // 					 false,
    // 					 8);

    G4double distclover = CloverCryox/2./tan(22.5*deg) + CloverCryoz/2.;
    G4RotationMatrix *rotatetempx[8];
    G4RotationMatrix *rotatetempy[8];
    G4RotationMatrix *rotatetempz[8];
    for(int ii = 0; ii < 8; ii++){
      rotatetempx[ii] = new G4RotationMatrix();
      rotatetempx[ii]->rotateX(45.0*deg*ii);
      rotatetempy[ii] = new G4RotationMatrix();
      rotatetempy[ii]->rotateY(45.0*deg*ii);
      rotatetempz[ii] = new G4RotationMatrix();
      rotatetempz[ii]->rotateZ(45.0*deg*ii);
    }

    //x-y plane
    for(int ii = 0; ii < 8; ii++){
      CloverCryoPos = G4ThreeVector( distclover*cos(45.0*deg*ii), distclover*sin(45*deg*ii) , 0 );

      physiCloverCryo = new G4PVPlacement(rotatetempz[ii],
					  CloverCryoPos,
					  logicCloverCryo,
					  "CloverCryo",
					  logicWorld,
					  false,
					  ii);
    }
    //y-z plane
    int detnum = 8;
    for(int ii = 0; ii < 8; ii++){
      CloverCryoPos = G4ThreeVector( 0, distclover*sin(45*deg*ii) , distclover*cos(45.0*deg*ii) );
      if(ii != 2|| ii != 6){
	physiCloverCryo = new G4PVPlacement(rotatetempx[ii],
					    CloverCryoPos,
					    logicCloverCryo,
					    "CloverCryo",
					    logicWorld,
					    false,
					    detnum);
	detnum++;
      }
    }
    //x-z plane
    detnum = 14;
    for(int ii = 0; ii < 8; ii++){
      CloverCryoPos = G4ThreeVector( distclover*cos(45.0*deg*ii), 0 ,distclover*sin(45*deg*ii) );
      if(ii != 0 || ii != 2|| ii != 4 || ii != 6){
	physiCloverCryo = new G4PVPlacement(rotatetempy[ii],
					    CloverCryoPos,
					    logicCloverCryo,
					    "CloverCryo",
					    logicWorld,
					    false,
					    detnum);
	detnum++;
      }
    }


  }
  
  //BC - create beam pipe used for LaBr3 array
  
  //Al beam pipe:  4.5 inch outer diameter (radius = 5.715 cm), 0.125 inch thickness (0.3175 cm)

  //fPipeOutRad = 5.715*cm;
  fPipeOutRad = 5.7975*cm;
  //fPipeOutRad = 5.87375*cm;
  //fPipeOutRad = 6.0325*cm;
  //fPipeOutRad = 6.19125*cm;
  //fPipeOutRad = 6.35*cm;
  fPipeInRad = 5.3975*cm;
  fPipeLength = 50.8*cm;
  //fPipeLength = 39.6875*cm;

  if(fUsePipe){
    //solidBCSpipe = new G4Tubs("BCSpipe",5.3975*cm,5.715*cm,50.8*cm/2.,0., twopi);
    //solidBCSpipe = new G4Tubs("BCSpipe",5.3975*cm,5.87375*cm,50.8*cm/2.,0., twopi);
    // solidBCSpipe = new G4Tubs("BCSpipe",5.3975*cm,6.0325*cm,50.8*cm/2.,0., twopi);
    solidBCSpipe = new G4Tubs("BCSpipe",fPipeInRad,fPipeOutRad,fPipeLength/2.,0., twopi);
    logicBCSpipe = new G4LogicalVolume(solidBCSpipe,SegaCryoMater,"BCSpipe",0,0,0); //This is Al beam pipe!
    physiBCSpipe = new G4PVPlacement(0,
				     G4ThreeVector(0,0,0),
				     logicBCSpipe,
				     "BCSpipe",
				     logicWorld,
				     false,
				     0,
				     true);
    
    //Stainless Steel beam pipe:  4.25 inch outer diamter, 0.065 inch thickness (0.1651 cm)
    /*solidBCSpipe = new G4Tubs("BCSpipe",5.2324*cm,5.3975*cm,50.8*cm/2.,0., twopi);
      logicBCSpipe = new G4LogicalVolume(solidBCSpipe,Stainless_3Hen_Mater,"BCSpipe",0,0,0); //This is Stainless beam pipe!
      physiBCSpipe = new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
      logicBCSpipe,
      "BCSpipe",
      logicWorld,
      false,
      0);*/
  }

  //BC - create LaBr3 array
  
  //initial values using measurements with a ruler
  
  
  flabr3AlCoverlength  = 3.175*cm;
  flabr3AlCoverthick = 0.1*cm;
  flabr3AlCoveroutrad = 1.984375*cm;  //Diameter = 3.96875 cm or 1.5625 in
  flabr3AlCoverinrad = flabr3AlCoveroutrad - flabr3AlCoverthick;
  flabr3cryslength = flabr3AlCoverlength - 0.1*cm; 
  flabr3crysthick =flabr3AlCoverthick - 0.1*cm;
  flabr3crysoutrad = flabr3AlCoverinrad;//-0.05*m't use exact same place in space
  flabr3PMToutrad = 3.175*cm;  //1.25 in radius, 2.5 in diameter
  flabr3PMTlength = 17.145*cm; // 6.75 in length
  
  
  G4cout << " radius " << flabr3crysoutrad << G4endl;
  solidLaBr3AlCover = new G4Tubs("LaBr3AlCover",0,flabr3AlCoveroutrad,flabr3AlCoverlength/2.,0.,twopi);
  logicLaBr3AlCover = new G4LogicalVolume(solidLaBr3AlCover,LaBr3CoverMater,"LaBr3AlCover",0,0,0);

  
  solidLaBr3Crys = new G4Tubs("LaBr3Crys",0,flabr3crysoutrad,flabr3cryslength/2.,0.,twopi);
  logicLaBr3Crys = new G4LogicalVolume(solidLaBr3Crys,LaBr3ScintMater,"LaBr3Crys",0,0,0);
  
  solidLaBr3PMT = new G4Tubs("LaBr3PMT",0,flabr3PMToutrad,flabr3PMTlength/2.,0.,twopi);
  logicLaBr3PMT = new G4LogicalVolume(solidLaBr3PMT,LaBr3CoverMater,"LaBr3PMT",0,0,0);

  solidLaBr3PMTvac = new G4Tubs("LaBr3PMTvac",0,(flabr3PMToutrad-0.175*cm),(flabr3PMTlength-0.175*cm)/2.,0.,twopi);
  logicLaBr3PMTvac = new G4LogicalVolume(solidLaBr3PMT,VacuumMater,"LaBr3PMTvac",0,0,0);


  physiLaBr3Crys = new G4PVPlacement(0,
				//G4ThreeVector(0,0,-(flabr3AlCoverlength-flabr3AlCoverthick)),
				G4ThreeVector(0,0,0),
				logicLaBr3Crys,
				"LaBr3Crys",
				logicLaBr3AlCover,
				false,
				0);
				
  physiLaBr3PMT = new G4PVPlacement(0,
        G4ThreeVector(0,0,-(flabr3AlCoverlength/2+flabr3PMTlength/2)),
        logicLaBr3PMT,
        "LaBr3PMT",
        logicLaBr3AlCover,
        false,
        0);

  physiLaBr3PMTvac = new G4PVPlacement(0,
        G4ThreeVector(0,0,-(flabr3AlCoverlength/2+flabr3PMTlength/2)),
        logicLaBr3PMT,
        "LaBr3PMTvac",
        logicLaBr3AlCover,
        false,
        0);
  
  
  if(fUseLaBr3){
  
    G4cout << " LaBr3 array " << G4endl;
    
    G4int nLaBr3 = 8; //e14057
    G4double dtheta = twopi/nLaBr3;
    G4double theta = 0;
    
    //assume beam 6 inch beam pipe (diameter).  r = 3 in = 7.62 cm
    
    //G4double radius = 7.62*cm + flabr3AlCoverlength/2;//keep centers constant regardless of how I change LaBr3 internal details
    //G4double radius = 5.08*cm + flabr3AlCoverlength/2; //r = 2 in = 5.08 cm
    //G4double radius = 5.3975*cm + flabr3AlCoverlength/2; //r = 2.125 in = 5.3975 cm  -- Stainless beam pipe
    // G4double radius = 5.715*cm + flabr3AlCoverlength/2 + 0.2*cm; //r = 2.25 in = 5.715 cm  -- Al beam pipe
    G4double radius = fPipeOutRad + flabr3AlCoverlength/2 + 0.2*cm; //r = 2.25 in = 5.715 cm  -- Al beam pipe
    //G4double radius = fsegacryooutrad + 8.84*cm; // stand drawing
    //G4double radius = fsegacryooutrad + 9.0*cm; //update based on Chris picture 2/20/13
    //G4double radius = fsegacryooutrad + 7.8*cm; // for BCS mode
    //fGeThickDetectorRadius + fsegacryooutrad + 1.0*cm;  //use 1.5 for 60 mm detector with 13th Ge, use 0.55 without 13th, use 1 *cm with 90 mm detecotr and 6 sega
    
    //place the LaBr3 crystal inside the Al cover
   
    //upstream half of LaBr3 array - e14057  (negative z positions)
    
    for(G4int n=0; n < nLaBr3; n++){
    
      //test
      //if(n >= (nLaBr3 - 2)){
      //  continue;
      //}
      
      //theta = dtheta * (n+0.25);
      //theta = dtheta * n;
      theta = dtheta * (n+0.5);
      //theta = dtheta * (n-0.25);
      G4double fLaBr3xpos = radius*cos(theta);
      G4double fLaBr3ypos = radius*sin(theta); 
      //G4double fLaBr3zpos = 0*cm;
      //G4double fLaBr3zpos = 2.75*cm;
      //G4double fLaBr3zpos = 2.413*cm;
      //G4double fLaBr3zpos = -2.301875*cm; //1.25 inches - e14057
      G4double fLaBr3zpos = -2.38125*cm; //1.25 inches - e14057
      G4double offsetRotAngleX = (atan(radius/fLaBr3zpos)*360./twopi)*deg;
      if(((theta/deg) > 180) && ((theta/deg) <= 360)){
        offsetRotAngleX = 180*deg - offsetRotAngleX;
      }
      G4RotationMatrix *rotatelabr3 = new G4RotationMatrix();
      rotatelabr3->rotateX(90.*deg);
      //rotatelabr3->rotateY(90.*deg);
      //rotatelabr3->rotateZ(theta);
      //rotatelabr3->rotateX(90*deg + offsetRotAngleX);
      //rotatelabr3->rotateX(offsetRotAngleX);
      rotatelabr3->rotateY(theta + 90.*deg);
      
      //rotatelabr3->inverse();

      G4int detnum_up = 2*n;
      
      physiLaBr3AlCover = new G4PVPlacement(rotatelabr3,
          G4ThreeVector(fLaBr3xpos,fLaBr3ypos,fLaBr3zpos),
					logicLaBr3AlCover,
					"LaBr3AlCover",
          logicWorld,
          false,
          detnum_up,
          true);
    }
    
    //downstream half of LaBr3 array - e14057 (positive z positions)
    
    //for(G4int n=nLaBr3; n < (2 * nLaBr3); n++){
    for(G4int n=0; n < (nLaBr3-1); n++){ //e16032 layout
    
      //testing for number of detectors required for offset arrays to achieve 
      //efficiency of centralized array
      //if(n > (nLaBr3 + 3)){
      //  break;
      //}
      //more testing 
      //if(((n-nLaBr3+1)%3) != 0){
      //  continue;
      //}
      
      //test
      //if(n >= (2*nLaBr3 - 3)){
      //  continue;
      //}

      //theta = dtheta * (n  + 0.75);
      //theta = dtheta * (n+0.5);
      //theta = dtheta*2 * n - dtheta;  //e14057
      theta = dtheta*(nLaBr3-n)+1*dtheta;
      //theta = dtheta * ((n - nLaBr3-7) + 0.75);  //set so that we don't run into 90 or 270, which is 
                                               //where tangent has problems
      G4double fLaBr3xpos = radius*cos(theta);
      G4double fLaBr3ypos = radius*sin(theta); 
      //G4double fLaBr3zpos = 0*cm;
      //G4double fLaBr3zpos = -2.413*cm;
      //G4double fLaBr3zpos = 2.301875*cm; //1.25 inches - e14057
      G4double fLaBr3zpos = 2.38125*cm; //e14057... frame width = 10.95375 cm, detector middle is 1/16" + 
                                        //
      G4double offsetRotAngleX = (atan(radius/fLaBr3zpos)*360./twopi)*deg;
      if(((theta/deg) < 180) || ((theta/deg) > 360)){
        offsetRotAngleX = 180*deg + offsetRotAngleX;
      }else{
        offsetRotAngleX = -1. * offsetRotAngleX;
      }
      G4RotationMatrix *rotatelabr3 = new G4RotationMatrix();
      rotatelabr3->rotateX(90.*deg);
      //rotatelabr3->rotateY(90.*deg);
      //rotatelabr3->rotateX((90. + (atan((fLaBr3zpos/cm)/(radius/cm))*twopi/360))*deg);
      //rotatelabr3->rotateX(90.*deg + offsetRotAngle);
      //rotatelabr3->rotateX(offsetRotAngleX);
      rotatelabr3->rotateY(theta + 90.*deg);
      //std::cout << "fLaBr3zpos/radius = " << ((fLaBr3zpos/cm)/(radius/cm)) << std::endl;
      //std::cout << "atan(fLaBr3zpos/radius) = " << atan((fLaBr3zpos/cm)/(radius/cm)) << std::endl;
      
      //rotatelabr3->inverse();
      G4int detnum_down = 2*n+1;
      physiLaBr3AlCover = new G4PVPlacement(rotatelabr3,
          G4ThreeVector(fLaBr3xpos,fLaBr3ypos,fLaBr3zpos),
    					logicLaBr3AlCover,
    					"LaBr3AlCover",
          logicWorld,
          false,
	  detnum_down,//e14057 numbering
          true);  //last true for collision detection (check if volumes touch)
    }
  
  }  //end LaBr3 array
  
  
  //BC - create LaBr3 frame
  flabr3framethick = 1.27*cm;       //0.5 inches
  //flabr3framelength = 11.684*cm;    //4.6 inches
  flabr3framelength = 10.95375*cm;    //4.3125 inches
  flabr3frameinrad = 15.24*cm;      //6 inches
  flabr3frameoutrad = flabr3frameinrad + flabr3framethick;
  
  solidLaBr3Frame = new G4Tubs("LaBr3Frame",flabr3frameinrad,flabr3frameoutrad,flabr3framelength/2.,0.,twopi);
  logicLaBr3Frame = new G4LogicalVolume(solidLaBr3Frame,LaBr3FrameMater,"LaBr3Frame",0,0,0);
  
  
  if(fUseLaBr3Frame){
  
    physiLaBr3Frame = new G4PVPlacement(0,
					G4ThreeVector(0,0,0),
					logicLaBr3Frame,
					"LaBr3Frame",
					logicWorld,
					false,
					0,
					true);
  
  } //end labr3 frame
  


  //BC - create 3Hen
  f3HenRmin = 4.5*cm;
  f3HenRmax = 32*cm;
  f3HenR1 = 8.5*cm;
  f3HenR2 = 14*cm;
  f3HenR3 = 19.5*cm;
  f3HenR4 = 25.5*cm;
  f3HenTubeCoveroutrad1in = 2.54/2*cm;
  f3HenTubeCoverinnerrad1in = f3HenTubeCoveroutrad1in - 1*mm;
  f3HenTubeoutrad1in = f3HenTubeCoverinnerrad1in;
  f3HenTubeCoveroutrad2in = 2.54*cm;
  f3HenTubeCoverinnerrad2in = f3HenTubeCoveroutrad2in - 1*mm;
  f3HenTubeoutrad2in = f3HenTubeCoverinnerrad2in;
  f3HenLength = 61*cm;
  
  //boolean to use only a view angle
  G4bool useViewAngle = true;
  
  G4double display3HenAngleStart = 0;
  G4double display3HenSize = 0;
  G4double display3HenAngleEnd = 0;
  
  G4Tubs* halfsolid3HenTube1inStart;
  G4LogicalVolume* halflogic3HenTube1inStart;
  G4Tubs* halfsolid3HenTubeCover1inStart;
  G4LogicalVolume* halflogic3HenTubeCover1inStart;
  G4Tubs* halfsolid3HenTubeCover2inStart;
  G4LogicalVolume* halflogic3HenTubeCover2inStart;
  G4Tubs* halfsolid3HenTube2inStart;
  G4LogicalVolume* halflogic3HenTube2inStart;
  
  G4Tubs* halfsolid3HenTube1inEnd;
  G4LogicalVolume* halflogic3HenTube1inEnd;
  G4Tubs* halfsolid3HenTubeCover1inEnd;
  G4LogicalVolume* halflogic3HenTubeCover1inEnd;
  G4Tubs* halfsolid3HenTubeCover2inEnd;
  G4LogicalVolume* halflogic3HenTubeCover2inEnd;
  G4Tubs* halfsolid3HenTube2inEnd;
  G4LogicalVolume* halflogic3HenTube2inEnd;
  
  G4VPhysicalVolume* halfphysi3HenTube1inStart;
  G4VPhysicalVolume* halfphysi3HenTube1inEnd;
  G4VPhysicalVolume* halfphysi3HenTube2inStart;
  G4VPhysicalVolume* halfphysi3HenTube2inEnd;
  G4VPhysicalVolume* halfphysi3HenTubeCover1inStart;
  G4VPhysicalVolume* halfphysi3HenTubeCover1inEnd;
  G4VPhysicalVolume* halfphysi3HenTubeCover2inStart;
  G4VPhysicalVolume* halfphysi3HenTubeCover2inEnd;
  
  if(useViewAngle){
    //choose starting and ending angles such that from the start, the detector 
    //is created counter-clockwise to the end
    display3HenAngleStart = twopi/4;
    display3HenSize = 3 * twopi/4;
    display3HenAngleEnd = twopi;
    
    solid3HenHDPE = new G4Tubs("3HenHDPE",f3HenRmin,f3HenRmax,(f3HenLength-1*mm)/2.,display3HenAngleStart,display3HenSize);
    logic3HenHDPE = new G4LogicalVolume(solid3HenHDPE,HDPE_3Hen_Mater,"3HenHDPE",0,0,0);
  
    solid3HenTubeCover1in = new G4Tubs("3HenTubeCover1in",0,f3HenTubeCoveroutrad1in,f3HenLength/2.,0.,twopi);
    logic3HenTubeCover1in = new G4LogicalVolume(solid3HenTubeCover1in,Stainless_3Hen_Mater,"3HenTubeCover1in",0,0,0);
  
    solid3HenTube1in = new G4Tubs("3HenTube1in",0,f3HenTubeoutrad1in,(f3HenLength-2*mm)/2.,0.,twopi); //shorter than cover
    logic3HenTube1in = new G4LogicalVolume(solid3HenTube1in,He3_10atm_3Hen_Mater,"3HenTube1in",0,0,0);
  
    solid3HenTubeCover2in = new G4Tubs("3HenTubeCover2in",0,f3HenTubeCoveroutrad2in,f3HenLength/2.,0.,twopi);
    logic3HenTubeCover2in = new G4LogicalVolume(solid3HenTubeCover2in,Stainless_3Hen_Mater,"3HenTubeCover2in",0,0,0);
  
    solid3HenTube2in = new G4Tubs("3HenTube2in",0,f3HenTubeoutrad2in,(f3HenLength-2*mm)/2.,0.,twopi); //shorter than cover
    logic3HenTube2in = new G4LogicalVolume(solid3HenTube2in,He3_10atm_3Hen_Mater,"3HenTube2in",0,0,0);
    
    //need half tubes for the boundaries (meaning should only use multiples of pi/2, probably)
    //at start angles
    halfsolid3HenTube1inStart = new G4Tubs("half3HenTube1inStart",0,f3HenTubeoutrad1in,(f3HenLength-2*mm)/2., display3HenAngleStart, twopi/2); //shorter than cover
    halflogic3HenTube1inStart = new G4LogicalVolume(halfsolid3HenTube1inStart, He3_10atm_3Hen_Mater,  "half3HenTube1inStart",0,0,0);
    
    halfsolid3HenTubeCover1inStart = new G4Tubs("half3HenTubeCover1inStart",0, f3HenTubeCoveroutrad1in, f3HenLength/2.,display3HenAngleStart, twopi/2);
    halflogic3HenTubeCover1inStart = new G4LogicalVolume(halfsolid3HenTubeCover1inStart, Stainless_3Hen_Mater,"half3HenTubeCover1inStart",0,0,0);
    
    halfsolid3HenTubeCover2inStart = new G4Tubs("half3HenTubeCover2inStart",0,f3HenTubeCoveroutrad2in, f3HenLength/2.,display3HenAngleStart, twopi/2);
    halflogic3HenTubeCover2inStart = new G4LogicalVolume(halfsolid3HenTubeCover2inStart, Stainless_3Hen_Mater,"half3HenTubeCover2inStart",0,0,0);
  
    halfsolid3HenTube2inStart = new G4Tubs("half3HenTube2inStart",0,f3HenTubeoutrad2in,(f3HenLength-2*mm)/2., display3HenAngleStart, twopi/2); //shorter than cover
    halflogic3HenTube2inStart = new G4LogicalVolume(halfsolid3HenTube2inStart, He3_10atm_3Hen_Mater,"half3HenTube2inStart",0,0,0);
    
    //at end angles
    halfsolid3HenTube1inEnd = new G4Tubs("half3HenTube1inEnd",0,f3HenTubeoutrad1in,(f3HenLength-2*mm)/2., (display3HenAngleEnd-twopi/2), twopi/2); //shorter than cover
    halflogic3HenTube1inEnd = new G4LogicalVolume(halfsolid3HenTube1inEnd, He3_10atm_3Hen_Mater,  "half3HenTube1inEnd",0,0,0);
    
    halfsolid3HenTubeCover1inEnd = new G4Tubs("half3HenTubeCover1inEnd",0, f3HenTubeCoveroutrad1in, f3HenLength/2.,(display3HenAngleEnd-twopi/2), twopi/2);
    halflogic3HenTubeCover1inEnd = new G4LogicalVolume(halfsolid3HenTubeCover1inEnd, Stainless_3Hen_Mater,"half3HenTubeCover1inEnd",0,0,0);
    
    halfsolid3HenTubeCover2inEnd = new G4Tubs("half3HenTubeCover2inEnd",0,f3HenTubeCoveroutrad2in, f3HenLength/2.,(display3HenAngleEnd-twopi/2), twopi/2);
    halflogic3HenTubeCover2inEnd = new G4LogicalVolume(halfsolid3HenTubeCover2inEnd, Stainless_3Hen_Mater,"half3HenTubeCover2inEnd",0,0,0);
  
    halfsolid3HenTube2inEnd = new G4Tubs("half3HenTube2inEnd",0,f3HenTubeoutrad2in,(f3HenLength-2*mm)/2.,(display3HenAngleEnd-twopi/2), twopi/2); //shorter than cover
    halflogic3HenTube2inEnd = new G4LogicalVolume(halfsolid3HenTube2inEnd, He3_10atm_3Hen_Mater,"half3HenTube2inEnd",0,0,0);
    
    
    halfphysi3HenTube1inStart = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				halflogic3HenTube1inStart,
				"half3HenTube1inStart",
				halflogic3HenTubeCover1inStart,
				false,
				0);
  
    halfphysi3HenTube2inStart = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				halflogic3HenTube2inStart,
				"half3HenTube2inStart",
				halflogic3HenTubeCover2inStart,
				false,
				0);
				
		halfphysi3HenTube1inEnd = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				halflogic3HenTube1inEnd,
				"half3HenTube1inEnd",
				halflogic3HenTubeCover1inEnd,
				false,
				0);
  
    halfphysi3HenTube2inEnd = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				halflogic3HenTube2inEnd,
				"half3HenTube2inEnd",
				halflogic3HenTubeCover2inEnd,
				false,
				0);		
    
    
  
  }else{
  
    solid3HenHDPE = new G4Tubs("3HenHDPE",f3HenRmin,f3HenRmax,(f3HenLength-1*mm)/2.,0, twopi);
    logic3HenHDPE = new G4LogicalVolume(solid3HenHDPE,HDPE_3Hen_Mater,"3HenHDPE",0,0,0);

  
    solid3HenTubeCover1in = new G4Tubs("3HenTubeCover1in",0,f3HenTubeCoveroutrad1in,f3HenLength/2.,0.,twopi);
    logic3HenTubeCover1in = new G4LogicalVolume(solid3HenTubeCover1in,Stainless_3Hen_Mater,"3HenTubeCover1in",0,0,0);
  
    solid3HenTube1in = new G4Tubs("3HenTube1in",0,f3HenTubeoutrad1in,(f3HenLength-2*mm)/2.,0.,twopi); //shorter than cover
    logic3HenTube1in = new G4LogicalVolume(solid3HenTube1in,He3_10atm_3Hen_Mater,"3HenTube1in",0,0,0);
  
    solid3HenTubeCover2in = new G4Tubs("3HenTubeCover2in",0,f3HenTubeCoveroutrad2in,f3HenLength/2.,0.,twopi);
    logic3HenTubeCover2in = new G4LogicalVolume(solid3HenTubeCover2in,Stainless_3Hen_Mater,"3HenTubeCover2in",0,0,0);
  
    solid3HenTube2in = new G4Tubs("3HenTube2in",0,f3HenTubeoutrad2in,(f3HenLength-2*mm)/2.,0.,twopi); //shorter than cover
    logic3HenTube2in = new G4LogicalVolume(solid3HenTube2in,He3_10atm_3Hen_Mater,"3HenTube2in",0,0,0);
  
  }
  
  //G4double display3HenAngleStart = 0;
  
  
  
        
        
        
        

  if(fUse3Hen){
  
    G4cout << " 3Hen detector " << G4endl;
    
    physi3HenTube1in = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				logic3HenTube1in,
				"3HenTube1in",
				logic3HenTubeCover1in,
				false,
				0);
  
    physi3HenTube2in = new G4PVPlacement(0,
				G4ThreeVector(0,0,0),
				logic3HenTube2in,
				"3HenTube2in",
				logic3HenTubeCover2in,
				false,
				0);
				
    physi3HenHDPE = new G4PVPlacement(0,
        G4ThreeVector(0,0,0),
        logic3HenHDPE,
        "3HenHDPE",
        logicWorld,
        false,
        0);
    
    //set up the angles for the rings of 3He tubes
    G4double dtheta1 = twopi/16;
    G4double dtheta2 = twopi/16;
    G4double dtheta3 = twopi/16;
    G4double dtheta4 = twopi/26;
    
    G4double theta = 0;
    
    G4double f3Henxpos = 0;
    G4double f3Henypos = 0;
    G4double f3Henzpos = 0;
    
    //place the inner ring of 1 in tubes
    for(G4int n = 0; n < 16; n++){
    
      theta = dtheta1 * n;
      
      f3Henxpos = f3HenR1*cos(theta);
      f3Henypos = f3HenR1*sin(theta);
      f3Henzpos = 0;
      
      if(useViewAngle){
      
        if((theta > display3HenAngleStart) && (theta < display3HenAngleEnd)){
      
          physi3HenTubeCover1in = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    logic3HenTubeCover1in,
				    "3HenTube1in",
				    logic3HenHDPE,
				    false,
				    0);
        }else if(theta == display3HenAngleStart){
          
          halfphysi3HenTubeCover1inStart = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover1inStart,
				    "half3HenTube1inStart",
				    logic3HenHDPE,
				    false,
				    0);
          
        
        }else if(theta == fmod(display3HenAngleEnd,twopi)){
        
          halfphysi3HenTubeCover1inEnd = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover1inEnd,
				    "half3HenTube1inEnd",
				    logic3HenHDPE,
				    false,
				    0);
        
        }
       
      }else{
        physi3HenTubeCover1in = new G4PVPlacement(0,
				G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				  logic3HenTubeCover1in,
				  "3HenTube1in",
				  logic3HenHDPE,
				  false,
				  0);
       }
    }
    
    //place first ring of 2 in tubes
    for(G4int n = 0; n < 16; n++){
    
      theta = dtheta2 * (n + 0.5);
      
      f3Henxpos = f3HenR2*cos(theta);
      f3Henypos = f3HenR2*sin(theta);
      f3Henzpos = 0;
      
      if(useViewAngle){
      
        if((theta > display3HenAngleStart) && (theta < display3HenAngleEnd)){
      
          physi3HenTubeCover2in = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    logic3HenTubeCover2in,
				    "3HenTube2in",
				    logic3HenHDPE,
				    false,
				    0);
				}else if(theta == display3HenAngleStart){
          
          halfphysi3HenTubeCover2inStart = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inStart,
				    "half3HenTube2inStart",
				    logic3HenHDPE,
				    false,
				    0);
          
        
        }else if(theta == fmod(display3HenAngleEnd,twopi)){
        
          halfphysi3HenTubeCover2inEnd = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inEnd,
				    "half3HenTube2inEnd",
				    logic3HenHDPE,
				    false,
				    0);
        
        }
				
      }else{
        physi3HenTubeCover2in = new G4PVPlacement(0,
				  G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				  logic3HenTubeCover2in,
				  "3HenTube2in",
				  logic3HenHDPE,
				  false,
				  0);
      }
    }
    
    //place second ring of 2 in tubes
    for(G4int n = 0; n < 16; n++){
    
      theta = dtheta3 * n;
      
      f3Henxpos = f3HenR3*cos(theta);
      f3Henypos = f3HenR3*sin(theta);
      f3Henzpos = 0;
      
      if(useViewAngle){
      
        if((theta > display3HenAngleStart) && (theta < display3HenAngleEnd)){
      
          physi3HenTubeCover2in = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    logic3HenTubeCover2in,
				    "3HenTube2in",
				    logic3HenHDPE,
				    false,
				    0);
        }else if(theta == display3HenAngleStart){
          
          halfphysi3HenTubeCover2inStart = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inStart,
				    "half3HenTube2inStart",
				    logic3HenHDPE,
				    false,
				    0);
          
        
        }else if(theta == fmod(display3HenAngleEnd,twopi)){
        
          halfphysi3HenTubeCover2inEnd = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inEnd,
				    "half3HenTube2inEnd",
				    logic3HenHDPE,
				    false,
				    0);
        
        }
        
      }else{
        physi3HenTubeCover2in = new G4PVPlacement(0,
				  G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				  logic3HenTubeCover2in,
				  "3HenTube2in",
				  logic3HenHDPE,
				  false,
				  0);
      }
    }
    
    //place third ring of 2 in tubes
    for(G4int n = 0; n < 26; n++){
      
      theta = dtheta4 * n;
      
      f3Henxpos = f3HenR4*cos(theta);
      f3Henypos = f3HenR4*sin(theta);
      f3Henzpos = 0;
      
      if(useViewAngle){
      
        if((theta > display3HenAngleStart) && (theta < display3HenAngleEnd)){
      
          physi3HenTubeCover2in = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    logic3HenTubeCover2in,
				    "3HenTube2in",
				    logic3HenHDPE,
				    false,
				    0);
        }else if(theta == display3HenAngleStart){
          
          halfphysi3HenTubeCover2inStart = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inStart,
				    "half3HenTube2inStart",
				    logic3HenHDPE,
				    false,
				    0);
          
        
        }else if(theta == fmod(display3HenAngleEnd,twopi)){
        
          halfphysi3HenTubeCover2inEnd = new G4PVPlacement(0,
				    G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				    halflogic3HenTubeCover2inEnd,
				    "half3HenTube2inEnd",
				    logic3HenHDPE,
				    false,
				    0);
        
        }
        
      }else{
        physi3HenTubeCover2in = new G4PVPlacement(0,
				  G4ThreeVector(f3Henxpos,f3Henypos,f3Henzpos),
				  logic3HenTubeCover2in,
				  "3HenTube2in",
				  logic3HenHDPE,
				  false,
				  0);
      }
    }
    
    
  } //3Hen
  
  
  //BC - create MTC
  fMTCboxX = 60*cm;
  fMTCboxY = 120*cm;
  fMTCboxZ = 90*cm;
  fMTCboxxpos = 100*cm;
  fMTCboxypos = 0*cm;
  fMTCboxzpos = -100*cm;
  fMTCtapeWidth = 35*mm;
  fMTCtapeThickness = 5*mm;
  fMTCtapeThickness = 5*mm;
  fMTCtapeSep = 3/2*2.54*cm;
  
  fMTCtapeL1 = fMTCboxxpos - fMTCboxX/2 + fMTCtapeWidth/2;
  //fMTCtapeL2 = (fMTCboxzpos - fMTCboxZ/2)/2;
  fMTCtapeL2 = fMTCboxzpos - fMTCtapeWidth/2;
  fMTCtapeL3 = fMTCtapeSep;
  
  solidMTCbox = new G4Box("MTCbox", fMTCboxX/2, fMTCboxY/2, fMTCboxZ/2);
  logicMTCbox = new G4LogicalVolume(solidMTCbox,materialsManager->GetMaterial("Al"),"MTCbox",0,0,0);
  
  solidMTCtapeS1_1 = new G4Box("MTCtapeS1_1", fMTCtapeWidth/2, fMTCtapeThickness/2, fMTCtapeL1/2);
  logicMTCtapeS1_1 = new G4LogicalVolume(solidMTCtapeS1_1, tape_35mm_Mater, "MTCtapeS1_1",0,0,0);
  
  solidMTCtapeS1_2 = new G4Box("MTCtapeS1_2", fMTCtapeWidth/2, fMTCtapeThickness/2, fMTCtapeL1/2);
  logicMTCtapeS1_2 = new G4LogicalVolume(solidMTCtapeS1_2, tape_35mm_Mater, "MTCtapeS1_2",0,0,0);
  
  // // solidMTCtapeS2_1 = new G4Box("MTCtapeS2_1", fMTCtapeWidth/2, fMTCtapeThickness/2, fMTCtapeL2/2);
  // // logicMTCtapeS2_1 = new G4LogicalVolume(solidMTCtapeS2_1, tape_35mm_Mater, "MTCtapeS2_1",0,0,0);
  
  // solidMTCtapeS2_2 = new G4Box("MTCtapeS2_2", fMTCtapeWidth/2, fMTCtapeThickness/2, fMTCtapeL2/2);
  // logicMTCtapeS2_2 = new G4LogicalVolume(solidMTCtapeS2_2, tape_35mm_Mater, "MTCtapeS2_2",0,0,0);
  
  // solidMTCtapeS3 = new G4Box("MTCtapeS3", fMTCtapeWidth/2, fMTCtapeL3/2, fMTCtapeThickness/2);
  // logicMTCtapeS3 = new G4LogicalVolume(solidMTCtapeS3, tape_35mm_Mater, "MTCtapeS3",0,0,0);
  
  if(fUseMTC){
  
    //place the big box
    physiMTCbox = new G4PVPlacement(0,
      G4ThreeVector(fMTCboxxpos,fMTCboxypos,fMTCboxzpos),
			logicMTCbox,
			"MTCbox",
      logicWorld,
      false,
      0);
    
    //place the sections of tape relative to the box and the constants up top
    G4RotationMatrix *rotatetapeS1 = new G4RotationMatrix();
    rotatetapeS1->rotateY(90.*deg);
    
    physiMTCtapeS1_1 = new G4PVPlacement(rotatetapeS1,
      G4ThreeVector(-(fMTCboxX/2 + fMTCtapeL1/2), fMTCtapeSep/2, 0),
			logicMTCtapeS1_1,
			"MTCtapeS1_1",
      logicMTCbox,
      false,
      0);
      
    physiMTCtapeS1_2 = new G4PVPlacement(rotatetapeS1,
      G4ThreeVector(-(fMTCboxX/2 + fMTCtapeL1/2), -fMTCtapeSep/2, 0),
			logicMTCtapeS1_2,
			"MTCtapeS1_2",
      logicMTCbox,
      false,
      0);
      
    //G4double tapeS2zpos = -fMTCboxzpos/2 + 3*fMTCtapeWidth/4;
    G4double tapeS2zpos = fMTCtapeL2/2 + fMTCtapeWidth/2;
    // physiMTCtapeS2_1 = new G4PVPlacement(0,
    //   G4ThreeVector(-fMTCboxxpos, fMTCtapeSep/2, -tapeS2zpos),
    // 			logicMTCtapeS2_1,
    // 			"MTCtapeS2_1",
    //   logicMTCbox,
    //   false,
    //   0);
      
    // physiMTCtapeS2_2 = new G4PVPlacement(0,
    //   G4ThreeVector(-fMTCboxxpos, -fMTCtapeSep/2, -tapeS2zpos),
    // 			logicMTCtapeS2_2,
    // 			"MTCtapeS2_2",
    //   logicMTCbox,
    //   false,
    //   0);
      
      
    // physiMTCtapeS3 = new G4PVPlacement(0,
    //   G4ThreeVector(-fMTCboxxpos, -fMTCboxypos, -fMTCboxzpos),
    // 			logicMTCtapeS3,
    // 			"MTCtapeS3",
    //   logicMTCbox,
    //   false,
    //   0);
    
    
      
  } //MTC
  
  
  //BC - create LENDA
  
  //LENDA:  http://arxiv.org/pdf/1111.4011v2.pdf
  fscintHeight = 300*mm;
  fscintWidth = 45*mm;
  fscintLength = 25*mm;
  //H6410:  http://www.hamamatsu.com/us/en/product/category/3100/3002/H6410/index.html
  fH6410Diamater = 60*mm;
  fH6410Height = 200*mm;
  fLENDAradius = 1*m;
  //setting, but not going to use the internal PMT values for now
  fH6410PMTDiamter = 51*mm;
  fH6410PMTHeight = 100*mm;
  
  //LENDA covers 45 degrees with 12 detectors or 90 with 24
  solidLENDAscintbar = new G4Box("LENDAscintbar", fscintWidth/2, fscintHeight/2, fscintLength/2);
  logicLENDAscintbar = new G4LogicalVolume(solidLENDAscintbar,BC408_Mater,"LENDAscintbar",0,0,0);
  
  //place the PMTs on the scintillator bar
  solidH6410_1 = new G4Tubs("H6410_1",0,fH6410Diamater/2,fH6410Height/2.,0.,twopi);
  logicH6410_1 = new G4LogicalVolume(solidH6410_1,H6410_Mater,"H6410_1",0,0,0);
  
  solidH6410_2 = new G4Tubs("H6410_2",0,fH6410Diamater/2,fH6410Height/2.,0.,twopi);
  logicH6410_2 = new G4LogicalVolume(solidH6410_2,H6410_Mater,"H6410_2",0,0,0);
  
  G4RotationMatrix *rotateH6410 = new G4RotationMatrix();
  rotateH6410->rotateX(90.*deg);
  
  physiH6410_1 = new G4PVPlacement(rotateH6410,
		G4ThreeVector(0,fscintHeight/2 + fH6410Height/2,0),
		logicH6410_1,
		"H6410_1",
		logicLENDAscintbar,
		false,
		0);
		
  physiH6410_2 = new G4PVPlacement(rotateH6410,
		G4ThreeVector(0,-(fscintHeight/2 + fH6410Height/2),0),
		logicH6410_2,
		"H6410_2",
		logicLENDAscintbar,
		false,
		0);
		
  
  
  
  if(fUseLENDA){
  
    G4int numLENDAbars = 12;
    
    G4double dtheta = twopi/8/numLENDAbars;  //spread over 45 deg.
    
    G4double theta = 0;
    G4double LENDAxpos = 0;
    G4double LENDAypos = 0;
    G4double LENDAzpos = 0;
    
    for(int n = 0; n < numLENDAbars; n++){
    
      theta = twopi/2 + dtheta * n;
      
      LENDAxpos = fLENDAradius * sin(theta);
      LENDAzpos = fLENDAradius * cos(theta);
      
      G4RotationMatrix *rotateLENDAscintbar = new G4RotationMatrix();
      rotateLENDAscintbar->rotateY(-theta);
      
      physiLENDAscintbar = new G4PVPlacement(rotateLENDAscintbar,
        G4ThreeVector(LENDAxpos,LENDAypos,LENDAzpos),
			  logicLENDAscintbar,
			  "LENDAscintbar",
        logicWorld,
        false,
        n);
    
    }
  
  }
  
  //BC - create Getest
  fGetestHeight = 3*mm;
  fGetestWidth = 3*mm;
  fGetestDepth = 3*mm;
  
  solidGetest = new G4Box("Getest", fGetestWidth/2, fGetestHeight/2, fGetestDepth/2);
  logicGetest = new G4LogicalVolume(solidGetest,Getest_Mater,"Getest",0,0,0);
  
  if(fUseGetest){
  
    physiGetest = new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
			logicGetest,
			"Getest",
      logicWorld,
      false,
      0);
  
  }
  
  
  //BC - create EXOtest
  //tests of 100 mm, 10 mm, 5 mm, 3 mm, and 2 mm
  fEXOtestHeight = 1000*mm;
  fEXOtestWidth = 1000*mm;
  fEXOtestDepth = 1000*mm;
  
  solidEXOtest = new G4Box("EXOtest", fEXOtestWidth/2, fEXOtestHeight/2, fEXOtestDepth/2);
  logicEXOtest = new G4LogicalVolume(solidEXOtest,EXOtest_Mater,"EXOtest",0,0,0);
  
  if(fUseEXOtest){
  
    physiEXOtest = new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
			logicEXOtest,
			"EXOtest",
      logicWorld,
      false,
      0);
  
  }
  
  //inner detector work
  //first, set variables for global use of any inner detector (independent of type)

  //setting a variable for x and y offset of inner detector positions
  G4double innerDet_xpos = 0*mm;
  G4double innerDet_ypos = -1.0*mm;
  
  //BC - create EJ204 Scintillator
  //see M. Alshudifat et al. Physics Procedia (2014)

  fEJ204ScintHeight = 52*mm;
  fEJ204ScintWidth = 52*mm;
  fEJ204ScintDepth = 10*mm;
  fEJ204ScintDepth += 4*mm;

  solidEJ204Scint = new G4Box("EJ204Scint", fEJ204ScintWidth/2.0, fEJ204ScintHeight/2.0, fEJ204ScintDepth/2.0);
  logicEJ204Scint = new G4LogicalVolume(solidEJ204Scint,EJ204Scint_Mater,"EJ204Scint",0,0,0);
  
  //fEJ204Scint_zpos = -2.278125*cm;
  //fEJ204Scint_zpos = -2.02*cm;
  fEJ204Scint_zpos = -2.25*cm + (fEJ204ScintDepth -10*mm)/2.0;
  G4cout << "fEJ204Scint_zpos = " << fEJ204Scint_zpos << G4endl;
  //fEJ204Scint_zpos += 0.02*cm;
  //fEJ204Scint_zpos = -2.0*cm;

  if(fUseEJ204Scint){
  
    physiEJ204Scint = new G4PVPlacement(0,
      G4ThreeVector(innerDet_xpos,innerDet_ypos,fEJ204Scint_zpos), //adjacent to EJ204Scint
			logicEJ204Scint,
			"EJ204Scint",
      logicWorld,
      false,
      0,
      true);
  
  }


  //BC - create CeBr3 Scintillator
  //see Berkeley Nucleonics data sheet

  //setting a variable for x and y offset of inner detector positions
  innerDet_xpos = 0*mm;
  innerDet_ypos = 0*mm;

  fCeBr3ScintHeight = 49*mm;
  fCeBr3ScintWidth = 49*mm;
  fCeBr3ScintDepth = 3*mm;
  fCeBr3ScintDepth += 0*mm;

  solidCeBr3Scint = new G4Box("CeBr3Scint", fCeBr3ScintWidth/2.0, fCeBr3ScintHeight/2.0, fCeBr3ScintDepth/2.0);
  logicCeBr3Scint = new G4LogicalVolume(solidCeBr3Scint,CeBr3Scint_Mater,"CeBr3Scint",0,0,0);
  
  //fCeBr3Scint_zpos = -2.278125*cm;
  //fCeBr3Scint_zpos = -2.02*cm;
  fCeBr3Scint_zpos = -2.25*cm + (fCeBr3ScintDepth -3*mm)/2.0;
  G4cout << "fCeBr3Scint_zpos = " << fCeBr3Scint_zpos << G4endl;
  //fCeBr3Scint_zpos += 0.02*cm;
  //fCeBr3Scint_zpos = -2.0*cm;

  if(fUseCeBr3Scint){
  
    physiCeBr3Scint = new G4PVPlacement(0,
      G4ThreeVector(innerDet_xpos,innerDet_ypos,fCeBr3Scint_zpos), //adjacent to CeBr3Scint
			logicCeBr3Scint,
			"CeBr3Scint",
      logicWorld,
      false,
      0,
      true);
  
  }


  
  //BC-create PSPMT
  fPSPMTWindowHeight = 52*mm;
  fPSPMTWindowWidth = 52*mm;
  fPSPMTWindowDepth = 2.5*mm;
  
  fPSPMTCathodeHeight = 52*mm;
  fPSPMTCathodeWidth = 52*mm;
  fPSPMTCathodeDepth = 10*nm;

  fPSPMTHeight = 52*mm;
  fPSPMTWidth = 52*mm;
  fPSPMTDepth = 27.4*mm - fPSPMTCathodeDepth - fPSPMTWindowDepth; //25.8999
  fPSPMTDepth += 0*mm;
  
  solidPSPMT = new G4Box("PSPMT", fPSPMTWidth/2.0, fPSPMTHeight/2.0, fPSPMTDepth/2.0);
  logicPSPMT = new G4LogicalVolume(solidPSPMT,PSPMT_Mater,"PSPMT",0,0,0);  
  
  //fPSPMT_zpos = fEJ204Scint_zpos + fEJ204ScintDepth/2.0 + fPSPMTDepth/2.0 + fPSPMTWindowDepth + fPSPMTCathodeDepth;
  fPSPMT_zpos = fCeBr3Scint_zpos + fCeBr3ScintDepth/2.0 + fPSPMTDepth/2.0 + fPSPMTWindowDepth + fPSPMTCathodeDepth;
  fPSPMTWindow_zpos = fPSPMT_zpos - fPSPMTDepth/2.0 - fPSPMTCathodeDepth - fPSPMTWindowDepth/2.0;
  fPSPMTCathode_zpos = fPSPMT_zpos - fPSPMTDepth/2.0 - fPSPMTCathodeDepth/2.0;
  //making PSPMT window and cathode children of the mother volume PSPMT
  // fPSPMTWindow_zpos = -fPSPMTDepth/2.0 - fPSPMTCathodeDepth - fPSPMTWindowDepth/2.0;
  // fPSPMTCathode_zpos = -fPSPMTDepth/2.0 - fPSPMTCathodeDepth/2.0;

  G4cout << "fPSPMT_zpos = " << fPSPMT_zpos << G4endl;
  G4cout << "fPSPMTWindow_zpos = " << fPSPMTWindow_zpos << G4endl;
  G4cout << "fPSPMTCathode_zpos = " << fPSPMTCathode_zpos << G4endl;
  
  if(fUsePSPMT){
    
    physiPSPMT = new G4PVPlacement(0,
				   //G4ThreeVector(0,0,(-0.278125*cm)), //0.47625 upstream of center of upstream ring (so -2.301875 - 0.47625)
				   G4ThreeVector(innerDet_xpos,innerDet_ypos,fPSPMT_zpos),
				   logicPSPMT,
				   "PSPMT",
      logicWorld,
      false,
      0,
      true);
  
  }

  fPSPMTVacHeight = fPSPMTHeight - 3*mm;
  fPSPMTVacWidth = fPSPMTWidth - 3*mm; 
  fPSPMTVacDepth = fPSPMTDepth - 2.9*mm;

  fPSPMTVacPos = G4ThreeVector(0,0,0);
  solidPSPMTVac = new G4Box("PSPMTVac", fPSPMTVacWidth/2.0, fPSPMTVacHeight/2.0, fPSPMTVacDepth/2.0);
  logicPSPMTVac = new G4LogicalVolume(solidPSPMTVac,VacuumMater,"PSPMTVac",0,0,0);

  if(fUsePSPMT){
    physiPSPMTVac = new G4PVPlacement(0,
				      fPSPMTVacPos,
				      logicPSPMTVac,
				      "PSPMTVac",
				      logicPSPMT,
				      false,
				      0);
  }

  solidPSPMTWindow = new G4Box("PSPMTWindow", fPSPMTWindowWidth/2.0, fPSPMTWindowHeight/2.0, fPSPMTWindowDepth/2.0);
  logicPSPMTWindow = new G4LogicalVolume(solidPSPMTWindow, PSPMTWindow_Mater, "PSPMTWindow",0,0,0);
  solidPSPMTCathode = new G4Box("PSPMTCathode", fPSPMTCathodeWidth/2.0, fPSPMTCathodeHeight/2.0, fPSPMTCathodeDepth/2.0);
  logicPSPMTCathode = new G4LogicalVolume(solidPSPMTCathode, PSPMTCathode_Mater, "PSPMTCathode",0,0,0);

  if(fUsePSPMT){
    physiPSPMTWindow = new G4PVPlacement(0,
					 G4ThreeVector(innerDet_xpos,innerDet_ypos,fPSPMTWindow_zpos),
					 logicPSPMTWindow,
					 "PSPMTWindow",
					 logicWorld,
					 false,
					 0,
					 true);
    
    physiPSPMTCathode = new G4PVPlacement(0,
					  G4ThreeVector(innerDet_xpos,innerDet_ypos,fPSPMTCathode_zpos),
					  logicPSPMTCathode,
					  "PSPMTCathode",
					  logicWorld,
					  false,
					  0,
					  true);
  }
  
  //BC - create SiDSSD 
  fSiDSSDHeight = 52*mm;
  fSiDSSDWidth = 52*mm;
  fSiDSSDDepth = 1.0*mm;
  fSiDSSDDepth += 1.7*mm;
  
  solidSiDSSD = new G4Box("SiDSSD", fSiDSSDWidth/2, fSiDSSDHeight/2, fSiDSSDDepth/2);
  logicSiDSSD = new G4LogicalVolume(solidSiDSSD,SiDSSD_Mater,"SiDSSD",0,0,0);

  fSiDSSD_zpos = fEJ204Scint_zpos - fEJ204ScintDepth/2.0 - fSiDSSDDepth/2.0 - 0.3*cm;
  
  if(fUseSiDSSD){
  
    physiSiDSSD = new G4PVPlacement(0,
      G4ThreeVector(innerDet_xpos,innerDet_ypos,fSiDSSD_zpos), //half a cm upstream of scint
			logicSiDSSD,
			"SiDSSD",
      logicWorld,
      false,
      0,
      true);
  
  }

  G4cout << "Front face of EJ204 Scint = " << (fEJ204Scint_zpos - fEJ204ScintDepth/2.0) << G4endl;
  G4cout << "Front face of CeBr3 Scint = " << (fCeBr3Scint_zpos - fCeBr3ScintDepth/2.0) << G4endl;
  G4cout << "Back face of SiDSSD = " << (fSiDSSD_zpos + fSiDSSDDepth/2.0) << G4endl;
  
  
  

  /*
  //BC - cadmesh test
  G4ThreeVector offset = G4ThreeVector(0, 0, 0);
  CADMesh *mesh = new CADMesh("/projects/decay/crider/SNL_figure_detector_systems/test_stl/example021.stl", "STL", cm, offset, false);
  G4VSolid *cad_solid;
  G4LogicalVolume * cad_logical;
  G4VPhysicalVolume * cad_physical;
  cad_solid = mesh->TessellatedMesh();
  cad_logical = new G4LogicalVolume(cad_solid, H6410_Mater, "cad_logical", 0, 0, 0);
  cad_physical = new G4PVPlacement(0, G4ThreeVector(), cad_logical,
                                     "cad_physical", logicWorld, false, 0);

  */

  //---------------------



  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 
  G4cout << "sensitive 1 " << G4endl;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  //G4String detectorSDname = "e17011_sim/DetectorSD";
  G4String detectorSDname = "GeThickDetectorCollection";
  e17011_simDetectorSD* aDetectorSD = new e17011_simDetectorSD( detectorSDname );
  SDman->AddNewDetector( aDetectorSD );
  //logicTarget->SetSensitiveDetector( aDetectorSD );
  logicGeThickDetector->SetSensitiveDetector( aDetectorSD );
  G4cout << "sensitive 2 " << G4endl;

  detectorSDname = "GeThinDetectorCollection";
  e17011_simDetectorSD* athinDetectorSD = new e17011_simDetectorSD( detectorSDname );
  SDman->AddNewDetector( athinDetectorSD );
  logicGeThinDetector->SetSensitiveDetector( athinDetectorSD );

  //add Sega as sensitive detector
  detectorSDname = "SegaCollection";
  e17011_simDetectorSD* aSegaDetectorSD = new e17011_simDetectorSD( detectorSDname );
  SDman->AddNewDetector( aSegaDetectorSD );
  logicSegaActiveArea->SetSensitiveDetector( aSegaDetectorSD );
  G4cout << "sensitive 3 " << G4endl;

  //add Clover as sensistive detector
  detectorSDname = "CloverCollection";
  e17011_simDetectorSD* aCloverDetectorSD = new e17011_simDetectorSD( detectorSDname);
  SDman->AddNewDetector( aCloverDetectorSD);
  logicCloverCrystal->SetSensitiveDetector( aCloverDetectorSD);
  
  //BC - add LaBr3 array as sensitive detector
  detectorSDname = "LaBr3Collection";
  e17011_simDetectorSD* aLaBr3DetectorSD = new e17011_simDetectorSD(detectorSDname);
  SDman->AddNewDetector(aLaBr3DetectorSD);
  logicLaBr3Crys->SetSensitiveDetector(aLaBr3DetectorSD);
  G4cout << "sensitive 3.1 " << G4endl;
  
  //BC - add Getest as sensitive detector
  detectorSDname = "GetestCollection";
  G4cout << "sensitive 3.11 " << G4endl;
  e17011_simDetectorSD* aGetestDetectorSD = new e17011_simDetectorSD(detectorSDname);
  G4cout << "sensitive 3.12 " << G4endl;
  SDman->AddNewDetector(aGetestDetectorSD);
  G4cout << "sensitive 3.13 " << G4endl;
  logicGetest->SetSensitiveDetector(aGetestDetectorSD);
  G4cout << "sensitive 3.2 " << G4endl;
  
  //BC - add EXOtest as sensitive detector
  detectorSDname = "EXOtestCollection";
  e17011_simDetectorSD* aEXOtestDetectorSD = new e17011_simDetectorSD(detectorSDname);
  SDman->AddNewDetector(aEXOtestDetectorSD);
  logicEXOtest->SetSensitiveDetector(aEXOtestDetectorSD);
  G4cout << "sensitive 3.3 " << G4endl;

  //BC - add PSPMT+EJ204 as sensitive detector
  detectorSDname = "EJ204ScintCollection";
  e17011_simDetectorSD* aEJ204ScintDetectorSD = new e17011_simDetectorSD(detectorSDname);
  SDman->AddNewDetector(aEJ204ScintDetectorSD);
  logicEJ204Scint->SetSensitiveDetector(aEJ204ScintDetectorSD);
  G4cout << "sensitive 3.4 " << G4endl;

  //BC - add PSPMT+CeBr3 as sensitive detector
  detectorSDname = "CeBr3ScintCollection";
  e17011_simDetectorSD* aCeBr3ScintDetectorSD = new e17011_simDetectorSD(detectorSDname);
  SDman->AddNewDetector(aCeBr3ScintDetectorSD);
  logicCeBr3Scint->SetSensitiveDetector(aCeBr3ScintDetectorSD);
  G4cout << "sensitive 3.5 " << G4endl;
  
  
  

  //
  //-------------------------------------------------
  // regions
  //
  //  if(targetRegion) delete targetRegion;
  // if(detectorRegion) delete detectorRegion;
  //targetRegion = new G4Region("Target"); //snl
  GeThickDetectorRegion   = new G4Region("GeThickDetector");
  //targetRegion->AddRootLogicalVolume(logicTarget);//snl
  GeThickDetectorRegion->AddRootLogicalVolume(logicGeThickDetector);

  //--------- Visualization attributes -------------------------------
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  //G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //logicTarget ->SetVisAttributes(TargetVisAtt);
  G4VisAttributes* DetectorVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,.0));
  logicGeThickDetector->SetVisAttributes(DetectorVisAtt);

  G4VisAttributes* DetectorVisAtt1 = new G4VisAttributes(G4Colour::Blue());
  //logicBCSpipe->SetVisAttributes(G4VisAttributes::Invisible);
  //logicBCSvac->SetVisAttributes(G4VisAttributes::Invisible);
  logicSegaCryo->SetVisAttributes(DetectorVisAtt1);
  logicGeThickDetector->SetVisAttributes(DetectorVisAtt1);
  logicCloverCryo->SetVisAttributes(G4Colour::Gray());
  logicCloverCrystal->SetVisAttributes(G4Colour::Green());

  logicGeCryoEndCap->SetVisAttributes(G4Colour::Green());
  //logicBCSdssd->SetVisAttributes(G4Colour::Green());
  
  //BC - LaBr3 array visualization attributes 
  
  //G4VisAttributes *labr3CrysVisAtt = new G4VisAttributes(G4Color(0.,0.,1.));
  //labr3CrysVisAtt -> SetForceWireframe(true);
  //labr3CrysVissAtt -> SetForceSolid(true);
  logicLaBr3Crys -> SetVisAttributes(G4Colour::Red());
  logicLaBr3PMT -> SetVisAttributes(G4Colour::Yellow());
  logicLaBr3AlCover -> SetVisAttributes(G4Colour::Green());
  logicLaBr3PMTvac ->SetVisAttributes(G4Colour::Blue());
  
  //BC - LaBr3 frame visualization attributes
  logicLaBr3Frame -> SetVisAttributes(G4Colour::White());
  
  //BC - 3Hen detector visualization attributes
  logic3HenHDPE -> SetVisAttributes(G4Colour::White());
  logic3HenTube1in -> SetVisAttributes(G4Colour::Green());
  logic3HenTube2in -> SetVisAttributes(G4Colour::Green());
  logic3HenTubeCover1in -> SetVisAttributes(G4Colour::Red());
  logic3HenTubeCover2in -> SetVisAttributes(G4Colour::Red());
  halflogic3HenTube1inStart -> SetVisAttributes(G4Colour::Green());
  halflogic3HenTube2inStart -> SetVisAttributes(G4Colour::Green());
  halflogic3HenTubeCover1inStart -> SetVisAttributes(G4Colour::Red());
  halflogic3HenTubeCover2inStart -> SetVisAttributes(G4Colour::Red());
  halflogic3HenTube1inEnd -> SetVisAttributes(G4Colour::Green());
  halflogic3HenTube2inEnd -> SetVisAttributes(G4Colour::Green());
  halflogic3HenTubeCover1inEnd -> SetVisAttributes(G4Colour::Red());
  halflogic3HenTubeCover2inEnd -> SetVisAttributes(G4Colour::Red());
  
  //BC - MTC visualization attributes
  //logicMTCbox -> SetVisAttributes(G4Colour::Brown());
  logicMTCtapeS1_1 -> SetVisAttributes(G4Colour::Blue());
  logicMTCtapeS1_2 -> SetVisAttributes(G4Colour::Blue());
  //logicMTCtapeS2_1 -> SetVisAttributes(G4Colour::Blue());
  // logicMTCtapeS2_2 -> SetVisAttributes(G4Colour::Blue());
  // logicMTCtapeS3 -> SetVisAttributes(G4Colour::Blue());
  
  //BC - LENDA visualization attributes
  logicH6410_1 -> SetVisAttributes(G4Colour::Blue());
  logicH6410_2 -> SetVisAttributes(G4Colour::Blue());
  logicLENDAscintbar -> SetVisAttributes(G4Colour::White());
  
  //BC - Getest vis attributes
  logicGetest -> SetVisAttributes(G4Colour::Gray());
  
  //BC - EXOtest vis attributes
  logicEXOtest -> SetVisAttributes(G4Colour::Gray());
  
  //BC - EJ204Scint vis attributes
  logicEJ204Scint -> SetVisAttributes(G4Colour::Cyan());
  
  //BC - PSPMT vis attributes
  logicPSPMT -> SetVisAttributes(G4Colour::Red());
  logicPSPMTWindow -> SetVisAttributes(G4Colour::Yellow());
  logicPSPMTCathode -> SetVisAttributes(G4Colour::Green());
  
  //BC - SiDSSD vis attributes
  logicSiDSSD -> SetVisAttributes(G4Colour::Blue());
  

  //------------ set the incident position ------
  G4cout << "sensitive 4 " << G4endl;

  // get the pointer to the User Interface manager 
    
  //   G4UImanager* UI = G4UImanager::GetUIpointer();  
  //   //      UI->ApplyCommand("/run/verbose 1");
  //   //      UI->ApplyCommand("/event/verbose 2");
  //   //      UI->ApplyCommand("/tracking/verbose 1");  

  //   G4double zpos = -fWorldLength/2.;
  //   G4String command = "/gps/pos/centre ";
  //   std::ostringstream os;
  //   os << zpos ; 
  //   G4String xs = os.str();
  //   UI->ApplyCommand(command+"0. 0. "+xs+" mm");
  //   UI->ApplyCommand("/gps/pos/type Point");
  //   command = "/gps/position ";
  //   //  UI->ApplyCommand(command+"0. 0. "+xs+" mm");
  //   UI->ApplyCommand("/gps/particle proton");
  //   UI->ApplyCommand("/gps/direction 0 0 1");
  //   UI->ApplyCommand("/gps/energy 100 MeV");
  //   //       

  if(ge17011_simAnalysisManager) ge17011_simAnalysisManager->DetectorInfo(this);
  G4cout << "sensitive 5 " << G4endl;
  
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// void e17011_simDetectorConstruction::setTargetMaterial(G4String materialName)
// {
//   // search the material by its name 
//   G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
//   if (pttoMaterial)
//      {TargetMater = pttoMaterial;
//       if (logicTarget) logicTarget->SetMaterial(pttoMaterial); 
//       G4cout << "\n----> The target has been changed to " << fTargetLength/cm << " cm of "
//              << materialName << G4endl;
//      }             
// }
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void e17011_simDetectorConstruction::setGeThickDetectorMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {GeThickDetectorMater = pttoMaterial;
      if (logicGeThickDetector) logicGeThickDetector->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The Thick Deetctor has been changed to" << fGeThickDetectorThickness/cm << " cm of "
             << materialName << G4endl;
     }             
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void e17011_simDetectorConstruction::setGeThinDetectorMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {GeThinDetectorMater = pttoMaterial;
      if (logicGeThinDetector) logicGeThinDetector->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The Thin Deetctor has been changed to" << fGeThinDetectorThickness/cm << " cm of "
             << materialName << G4endl;
     }             
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
