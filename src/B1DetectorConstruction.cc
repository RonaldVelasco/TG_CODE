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
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  
   //     
  // Definicion de parametros para cilindros
  //  
 G4Material* mat_int = nist -> FindOrBuildMaterial("G4_POLYSTYRENE");

   G4double innerRadius =17.9*mm;
   G4double outerRadius =19.4*mm;
   G4double hz =10.5*mm; 
   G4double startAngle =0.*deg;
   G4double spanningAngle =360.*deg;


    // Monedita de agua

    

     G4Material* mat_int_pozo = nist -> FindOrBuildMaterial("G4_WATER");

   G4double innerRadius2 =0*mm;
   G4double outerRadius2 =17.9*mm;
   G4double hz2 =4*mm; 
   G4double startAngle2 =0.*deg;
   G4double spanningAngle2 =360.*deg;

   G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
   rotationMatrix -> rotateX(90.*deg);

   
  
   //moneda_Agua_solido
   G4Tubs* moneda_agua = new G4Tubs ("moneda_agua", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);

   //moneda_agua 1

   G4Tubs* moneda_agua1 = new G4Tubs ("moneda_agua1", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua1 = new G4LogicalVolume (moneda_agua1,
                                                             mat_int_pozo,
                                                             "moneda_agua1");
   G4VisAttributes* simpleLayerVisAtt1 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua1 -> SetVisAttributes(simpleLayerVisAtt1);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(38.8*mm,-5*mm,38.8*mm),                    //at position
                     logic_moneda_agua1,             //its logical volume
                     "moneda_agua1",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking

   //moneda_agua 2

   G4Tubs* moneda_agua2 = new G4Tubs ("moneda_agua2", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua2 = new G4LogicalVolume (moneda_agua2,
                                                             mat_int_pozo,
                                                             "moneda_agua2");

   G4VisAttributes* simpleLayerVisAtt2 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua2 -> SetVisAttributes(simpleLayerVisAtt2);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(0,-5*mm,38.8*mm),                    //at position
                     logic_moneda_agua2,             //its logical volume
                     "moneda_agua2",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking
   //moneda_agua 3


   G4Tubs* moneda_agua3 = new G4Tubs ("moneda_agua3", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua3 = new G4LogicalVolume (moneda_agua3,
                                                             mat_int_pozo,
                                                             "moneda_agua3");

   G4VisAttributes* simpleLayerVisAtt3 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua3 -> SetVisAttributes(simpleLayerVisAtt3);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(-38.8*mm,-5*mm,38.8*mm),                    //at position
                     logic_moneda_agua3,             //its logical volume
                     "moneda_agua3",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking
   //moneda_agua 4


   G4Tubs* moneda_agua4 = new G4Tubs ("moneda_agua4", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua4 = new G4LogicalVolume (moneda_agua4,
                                                             mat_int_pozo,
                                                             "moneda_agua4");

   G4VisAttributes* simpleLayerVisAtt4 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua4 -> SetVisAttributes(simpleLayerVisAtt4);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(38.8*mm,-5*mm,0),                    //at position
                     logic_moneda_agua4,             //its logical volume
                     "moneda_agua4",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking
   //moneda_agua 5

   G4Tubs* moneda_agua5 = new G4Tubs ("moneda_agua5", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua5 = new G4LogicalVolume (moneda_agua5,
                                                             mat_int_pozo,
                                                             "moneda_agua5");

   G4VisAttributes* simpleLayerVisAtt5 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua5 -> SetVisAttributes(simpleLayerVisAtt5);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(0,-5*mm,0),                    //at position
                     logic_moneda_agua5,             //its logical volume
                     "moneda_agua5",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking
   //moneda_agua 6

   G4Tubs* moneda_agua6 = new G4Tubs ("moneda_agua6", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua6 = new G4LogicalVolume (moneda_agua6,
                                                             mat_int_pozo,
                                                             "moneda_agua6");

   G4VisAttributes* simpleLayerVisAtt6 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua6 -> SetVisAttributes(simpleLayerVisAtt6);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(-38.8*mm,-5*mm,0),                    //at position
                     logic_moneda_agua6,             //its logical volume
                     "moneda_agua6",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking
   //moneda_agua 7


   G4Tubs* moneda_agua7 = new G4Tubs ("moneda_agua7", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua7 = new G4LogicalVolume (moneda_agua7,
                                                             mat_int_pozo,
                                                             "moneda_agua7");

   G4VisAttributes* simpleLayerVisAtt7 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua7 -> SetVisAttributes(simpleLayerVisAtt7);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(38.8*mm,-5*mm,-38.8*mm),                    //at position
                     logic_moneda_agua7,             //its logical volume
                     "moneda_agua7",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking

   //moneda_Agua 8

   G4Tubs* moneda_agua8 = new G4Tubs ("moneda_agua8", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
   G4LogicalVolume* logic_moneda_agua8 = new G4LogicalVolume (moneda_agua8,
                                                             mat_int_pozo,
                                                             "moneda_agua8");

   G4VisAttributes* simpleLayerVisAtt8 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua8 -> SetVisAttributes(simpleLayerVisAtt8);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(0,-5*mm,-38.8*mm),                    //at position
                     logic_moneda_agua8,             //its logical volume
                     "moneda_agua8",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking


   //moneda_Agua 9

   G4Tubs* moneda_agua9 = new G4Tubs ("moneda_agua9", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);

   G4LogicalVolume* logic_moneda_agua9 = new G4LogicalVolume (moneda_agua9,
                                                             mat_int_pozo,
                                                             "moneda_agua9");

   G4VisAttributes* simpleLayerVisAtt9 = new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));//amarillo(R,G,B, Transparencia)  G4VisAttributes* simpleLayer(
   logic_moneda_agua9 -> SetVisAttributes(simpleLayerVisAtt9);

	
   new G4PVPlacement(rotationMatrix,                       //no rotation
                     G4ThreeVector(-38.8*mm,-5*mm,-38.8*mm),                    //at position
                     logic_moneda_agua9,             //its logical volume
                     "moneda_agua9",                //its name
                     logicEnv,                //its mother  volume
                     false,                   //no boolean operation
                     0,                       //copy number
                     checkOverlaps);          //overlaps checking



    

   //Tapas

   G4double innerRadius1 =0.*mm;
   G4double outerRadius1 =19.4*mm;
   G4double hz1 =0.75*mm; 
   G4double startAngle1 =0.*deg;
   G4double spanningAngle1 =360.*deg;
 




  //Cilindro 1


   G4Tubs* solidpozo1 = new G4Tubs ("pozo1", innerRadius, outerRadius, hz, startAngle, spanningAngle);

   

   // CIlindro 2

   G4Tubs* solidpozo2 = new G4Tubs ("pozo2", innerRadius, outerRadius, hz, startAngle, spanningAngle);
 

   // Cilindro 3

   G4Tubs* solidpozo3 = new G4Tubs ("pozo3", innerRadius, outerRadius, hz, startAngle, spanningAngle);
   G4LogicalVolume* logicalcilindro3= new G4LogicalVolume (solidpozo3,
							 mat_int,
							 "pozo3");

     			new G4PVPlacement(rotationMatrix,
					      G4ThreeVector(-38.8*mm,0,38.8*mm),
					      logicalcilindro3,
					      "pozo3",
					      logicEnv,
					      false,
					      0,
					      checkOverlaps);

   // Cilindro 4

   G4Tubs* solidpozo4 = new G4Tubs ("pozo4", innerRadius, outerRadius, hz, startAngle, spanningAngle);
 

   // Cilindro 5

   G4Tubs* solidpozo5 = new G4Tubs ("pozo5", innerRadius, outerRadius, hz, startAngle, spanningAngle);


   // Cilindro 6

   G4Tubs* solidpozo6 = new G4Tubs ("pozo6", innerRadius, outerRadius, hz, startAngle, spanningAngle);

   // Cilindro 7

   G4Tubs* solidpozo7 = new G4Tubs ("pozo7", innerRadius, outerRadius, hz, startAngle, spanningAngle);


   //Cilindro 8

   G4Tubs* solidpozo8 = new G4Tubs ("pozo8", innerRadius, outerRadius, hz, startAngle, spanningAngle);


   // Cilindro 9

   G4Tubs* solidpozo9 = new G4Tubs ("pozo9", innerRadius, outerRadius, hz, startAngle, spanningAngle);

   
  //     
  // Tapas
  //

 //tapa1

 G4Tubs* tapa1 = new G4Tubs("tapa_inf1", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 1

   G4UnionSolid* pozoT1 = new G4UnionSolid ("pozototal1",solidpozo1 , tapa1,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo1 = new G4LogicalVolume (pozoT1,
						     mat_int,
						     "pozototal1");


 				new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(38.8*mm,0,38.8*mm),
						  logicpozo1,
						  "pozototal1",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);
 
 //tapa2

 G4Tubs* tapa2 = new G4Tubs("tapa_inf2", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 2

 G4UnionSolid* pozoT2 = new G4UnionSolid ("pozototal2",solidpozo2 , tapa2,0,G4ThreeVector(0,0,-9.75*mm));

 
G4LogicalVolume* logicpozo2 = new G4LogicalVolume (pozoT2,
						     mat_int,
						     "pozototal2");


 				new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(0,0,38.8*mm),
						  logicpozo2,
						  "pozototal2",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);

 

 //tapa3

 G4Tubs* tapa3 = new G4Tubs("tapa_inf3", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo3

  G4UnionSolid* pozoT3 = new G4UnionSolid ("pozototal3",solidpozo3 , tapa3,0,G4ThreeVector(0,0,-9.75*mm));

 
G4LogicalVolume* logicpozo3 = new G4LogicalVolume (pozoT3,
						     mat_int,
						     "pozototal3");


 				new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(-38.8*mm,0,38.8*mm),
						  logicpozo3,
						  "pozototal3",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);
 
 //tapa4

 G4Tubs* tapa4 = new G4Tubs("tapa_inf4", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 4

  G4UnionSolid* pozoT4 = new G4UnionSolid ("pozototal4",solidpozo4 , tapa4,0,G4ThreeVector(0,0,-9.75*mm));

 
G4LogicalVolume* logicpozo4 = new G4LogicalVolume (pozoT4,
						     mat_int,
						     "pozototal4");


 				new G4PVPlacement(rotationMatrix,
						  G4ThreeVector(38.8*mm,0,0),
						  logicpozo4,
						  "pozototal4",
						  logicEnv,
						  false,
						  0,
						  checkOverlaps);

 
 //tapa5

 G4Tubs* tapa5 = new G4Tubs("tapa_inf5", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);


 // pozo 5


  G4UnionSolid* pozoT5 = new G4UnionSolid ("pozototal5",solidpozo5 , tapa5,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo5 = new G4LogicalVolume (pozoT5,
						     mat_int,
						     "pozototal5");
 new G4PVPlacement(rotationMatrix,
 						  G4ThreeVector(),
  						  logicpozo5,
  						  "pozototal5",
 						  logicEnv,
 						  false,
 						  0,
  						  checkOverlaps);

  //tapa6

 G4Tubs* tapa6 = new G4Tubs("tapa_inf6", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 6

 
  G4UnionSolid* pozoT6 = new G4UnionSolid ("pozototal6",solidpozo6 , tapa6,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo6 = new G4LogicalVolume (pozoT6,
						     mat_int,
						     "pozototal6");
 new G4PVPlacement(rotationMatrix,
 						  G4ThreeVector(-38.8*mm,0,0),
  						  logicpozo6,
  						  "pozototal6",
 						  logicEnv,
 						  false,
 						  0,
  						  checkOverlaps);
   //tapa7

 G4Tubs* tapa7 = new G4Tubs("tapa_inf7", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 7

 
  G4UnionSolid* pozoT7 = new G4UnionSolid ("pozototal7",solidpozo7 , tapa7,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo7 = new G4LogicalVolume (pozoT7,
						     mat_int,
						     "pozototal7");
 new G4PVPlacement(rotationMatrix,
 						  G4ThreeVector(38.8*mm,0,-38.8*mm),
  						  logicpozo7,
  						  "pozototal7",
 						  logicEnv,
 						  false,
 						  0,
  						  checkOverlaps);
    //tapa8

 G4Tubs* tapa8 = new G4Tubs("tapa_inf8", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

 // pozo 8

   G4UnionSolid* pozoT8 = new G4UnionSolid ("pozototal8",solidpozo8 , tapa8,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo8 = new G4LogicalVolume (pozoT8,
						     mat_int,
						     "pozototal8");
 new G4PVPlacement(rotationMatrix,
 						  G4ThreeVector(0,0,-38.8*mm),
  						  logicpozo8,
  						  "pozototal8",
 						  logicEnv,
 						  false,
 						  0,
  						  checkOverlaps);

     //tapa9

 G4Tubs* tapa9 = new G4Tubs("tapa_inf9", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

// pozo 9

   G4UnionSolid* pozoT9 = new G4UnionSolid ("pozototal9",solidpozo9 , tapa9,0,G4ThreeVector(0,0,-9.75*mm));
 
 G4LogicalVolume* logicpozo9 = new G4LogicalVolume (pozoT9,
						     mat_int,
						     "pozototal9");
 new G4PVPlacement(rotationMatrix,
 						  G4ThreeVector(-38.8*mm,0,-38.8*mm),
  						  logicpozo9,
  						  "pozototal9",
 						  logicEnv,
 						  false,
 						  0,
  						  checkOverlaps);
 

                
  // Set Shape2 as scoring volume
  //
  
  fScoringVolume = logic_moneda_agua5;

 // fScoringVolume = logicpozo1;

  //
  //always return the physical World
  //
  
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
