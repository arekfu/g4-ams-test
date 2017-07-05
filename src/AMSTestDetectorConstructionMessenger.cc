#include "AMSTestDetectorConstructionMessenger.hh"
#include "AMSTestDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


AMSTestDetectorConstructionMessenger::AMSTestDetectorConstructionMessenger(AMSTestDetectorConstruction * const dc) :
  G4UImessenger(),
  detectorConstruction(dc)
{
  amsDirectory = new G4UIdirectory("/AMS/");
  amsDirectory->SetGuidance("UI commands specific to this application.");

  geomCmd = new G4UIcmdWithADoubleAndUnit("/AMS/setSphereRadius",this);
  geomCmd->SetGuidance("Select radius of the calculation sphere.");
  geomCmd->SetParameterName("radius",false);
  geomCmd->SetUnitCategory("length");
  geomCmd->AvailableForStates(G4State_PreInit);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestDetectorConstructionMessenger::~AMSTestDetectorConstructionMessenger()
{
  delete geomCmd;
  delete amsDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == geomCmd ) {
    const G4double newRadius = geomCmd->GetNewDoubleValue(newValue);
    G4cout << "Setting sphere radius to " << newRadius << G4endl;
    detectorConstruction->SetSphereRadius(newRadius);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
