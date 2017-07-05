#include "AMSTestActionInitialization.hh"
#include "AMSTestPrimaryGeneratorAction.hh"
#include "AMSTestRunAction.hh"
#include "AMSTestSteppingAction.hh"
#include "G4String.hh"
#include "AMSTestDetectorConstruction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestActionInitialization::AMSTestActionInitialization(const G4String &file) :
  G4VUserActionInitialization(),
  outputFile(file)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestActionInitialization::~AMSTestActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestActionInitialization::BuildForMaster() const
{
  SetUserAction(new AMSTestRunAction(outputFile));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestActionInitialization::Build() const
{
  SetUserAction(new AMSTestPrimaryGeneratorAction);
  AMSTestRunAction * const runAction = new AMSTestRunAction(outputFile);
  SetUserAction(runAction);
  SetUserAction(new AMSTestSteppingAction(runAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
