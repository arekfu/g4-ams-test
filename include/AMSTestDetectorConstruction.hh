#ifndef AMSTestDetectorConstruction_h
#define AMSTestDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4String.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VisAttributes;
class AMSTestDetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.
///
/// Crystals are positioned in Ring, with an appropriate rotation matrix.
/// Several copies of Ring are placed in the full detector.

class AMSTestDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    AMSTestDetectorConstruction();
    virtual ~AMSTestDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    void SetSphereRadius(const G4double r) { sphereR = r; }

  private:
    G4bool  fCheckOverlaps;
    G4VisAttributes *objAttributes;
    G4double sphereR;
    AMSTestDetectorConstructionMessenger *theMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

