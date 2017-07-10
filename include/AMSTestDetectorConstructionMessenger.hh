#ifndef AMSTestDetectorMessenger_h
#define AMSTestDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class AMSTestDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;


class AMSTestDetectorConstructionMessenger: public G4UImessenger
{
  public:
    AMSTestDetectorConstructionMessenger(AMSTestDetectorConstruction * const dc);
    virtual ~AMSTestDetectorConstructionMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    AMSTestDetectorConstruction*  detectorConstruction;

    G4UIdirectory*           amsDirectory;

    G4UIcmdWithADoubleAndUnit*      geomCmd;

    G4UIcmdWithAString*      matCmd;

};

#endif
