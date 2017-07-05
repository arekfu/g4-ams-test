#ifndef AMSTestPrimaryGeneratorAction_h
#define AMSTestPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4SingleParticleSource;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines an ion (F18), at rest, randomly distribued within a zone
/// in a patient defined in GeneratePrimaries(). Ion F18 can be changed
/// with the G4ParticleGun commands (see run2.mac).

class AMSTestPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    AMSTestPrimaryGeneratorAction();
    virtual ~AMSTestPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);
  protected:

  private:
    G4SingleParticleSource *sps;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


