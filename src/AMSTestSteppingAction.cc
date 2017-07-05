#include "AMSTestSteppingAction.hh"
#include "AMSTestRunAction.hh"
#include "AMSTestAnalysis.hh"
#include "G4StepPoint.hh"
#include "G4Neutron.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestSteppingAction::AMSTestSteppingAction(AMSTestRunAction * const runAction) :
  hId(runAction->GetSpectrumHID()),
  analysisManager(G4AnalysisManager::Instance())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestSteppingAction::~AMSTestSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestSteppingAction::UserSteppingAction(const G4Step *step) {
  // if stepping from any volume to the detector volume
  G4VPhysicalVolume *preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume *postVolume = step->GetPostStepPoint()->GetPhysicalVolume();
  if(preVolume==postVolume || !postVolume || postVolume->GetName()!="Detector")
    return;

  G4Track const *track = step->GetTrack();
  G4ParticleDefinition const *pDef = track->GetParticleDefinition();
  if(pDef != G4Neutron::Neutron())
    return;

  const G4double kinE = track->GetKineticEnergy();
  analysisManager->FillH1(hId,kinE);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
