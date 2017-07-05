#ifndef AMSTestRunAction_h
#define AMSTestRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4String.hh"
#include "globals.hh"
#include "AMSTestAnalysis.hh"

#ifdef HAS_BOOST_DATE_TIME
#include <boost/date_time/posix_time/posix_time.hpp>
namespace bpt = boost::posix_time;
#endif

class G4Run;

/// Run action class

class AMSTestRunAction : public G4UserRunAction
{
  public:
    AMSTestRunAction(const G4String &fname = "");
    virtual ~AMSTestRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    G4int   GetSpectrumHID() { return hId; }
  protected:
    G4String fileName;
    G4int hId;
#ifdef HAS_BOOST_DATE_TIME
    bpt::ptime startTime, endTime;
#endif
    void dumpHistogramToDAT(const G4int hID, const G4String &fname) const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

