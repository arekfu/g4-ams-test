#include "AMSTestDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "AMSTestDetectorConstructionMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestDetectorConstruction::AMSTestDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true),
  objAttributes(0),
  sphereR(300*cm)
{
  theMessenger = new AMSTestDetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestDetectorConstruction::~AMSTestDetectorConstruction()
{
  delete objAttributes;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* AMSTestDetectorConstruction::Construct()
{
  G4double detectorR  = sphereR + 1*cm;
  G4double worldR  = detectorR + 1*cm;

  G4NistManager* nist = G4NistManager::Instance();
  G4Material *void_mat_294 =
    G4NistManager::Instance()->BuildMaterialWithNewDensity("HotGalactic", "G4_Galactic", 1e-30*g/cm3, 294.0*kelvin);
  G4Material* fe_mat = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* fe_mat_294 =
    G4NistManager::Instance()->BuildMaterialWithNewDensity("Fe", "G4_Fe", fe_mat->GetDensity(), 294.0*kelvin);

  G4Sphere* solidWorld =
    new G4Sphere("World",
                 0., worldR, 0., twopi, 0., pi);

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        void_mat_294,         //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps

  // detector sphere
  G4Sphere *solidDetector =
    new G4Sphere("Detector", 0., detectorR, 0., twopi, 0., pi);

  G4LogicalVolume* logicDetector =
    new G4LogicalVolume(solidDetector,           //its solid
                        fe_mat_294,         //its material
                        "Detector");             //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicDetector,            //its logical volume
                    "Detector",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps

  // iron sphere
  G4Sphere *solidSphere =
    new G4Sphere("Sphere", 0., sphereR, 0., twopi, 0., pi);

  G4LogicalVolume* logicSphere =
    new G4LogicalVolume(solidSphere,           //its solid
                        fe_mat_294,         //its material
                        "Sphere");             //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicSphere,            //its logical volume
                    "Sphere",               //its name
                    logicDetector,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps

  // Visualization attributes
  //
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  objAttributes = new G4VisAttributes( G4Colour(1.,1.,1., 0.2));
  objAttributes -> SetVisibility(true);
  objAttributes -> SetForceSolid(true);
  logicSphere->SetVisAttributes(objAttributes);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //always return the physical World
  //
  return physWorld;
}
