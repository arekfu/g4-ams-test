#ifndef AMSTestSteppingAction_h
#define AMSTestSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "AMSTestAnalysis.hh"

class G4Step;
class AMSTestRunAction;

class AMSTestSteppingAction : public G4UserSteppingAction
{
  public:
    AMSTestSteppingAction(AMSTestRunAction * const runAction);
    virtual ~AMSTestSteppingAction();

    virtual void UserSteppingAction(const G4Step *step);
  protected:
    const G4int hId;
    G4AnalysisManager* analysisManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

