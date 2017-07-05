#ifndef AMSTestActionInitialization_h
#define AMSTestActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

/// Action initialization class.
///

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AMSTestActionInitialization : public G4VUserActionInitialization
{
  public:
    AMSTestActionInitialization(const G4String &file="");
    virtual ~AMSTestActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  protected:
    G4String outputFile;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


