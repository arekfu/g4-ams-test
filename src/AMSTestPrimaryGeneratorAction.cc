#include "AMSTestPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SingleParticleSource.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4ParticleTable.hh"
#include "G4Proton.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestPrimaryGeneratorAction::AMSTestPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   sps(new G4SingleParticleSource)
{
  G4ParticleDefinition *particle
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  sps->SetParticleDefinition(particle);

  G4SPSEneDistribution *eneDist = sps->GetEneDist();
  eneDist->SetEnergyDisType("Mono");
  eneDist->SetMonoEnergy(3.*GeV);

  G4SPSAngDistribution *angDist = sps->GetAngDist();
  angDist->DefineAngRefAxes("angref1", G4ThreeVector(0., 1., 0.));
  angDist->DefineAngRefAxes("angref2", G4ThreeVector(1., 0., 0.));
  angDist->SetAngDistType("beam1d");

  G4SPSPosDistribution *posDist = sps->GetPosDist();
  posDist->SetPosDisType("Point");
  posDist->SetCentreCoords(G4ThreeVector());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestPrimaryGeneratorAction::~AMSTestPrimaryGeneratorAction()
{
  delete sps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  sps->GeneratePrimaryVertex(anEvent);
}
