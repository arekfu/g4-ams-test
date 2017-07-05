#include "AMSTestRunAction.hh"
#include "AMSTestPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "AMSTestAnalysis.hh"
#include "AMSTestDetectorConstruction.hh"

#include <vector>

AMSTestRunAction::AMSTestRunAction(const G4String &fname) :
  G4UserRunAction(),
  fileName(fname)
{
  // Create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create directories
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Creating histograms
  const G4double eMin = 1e-11 * MeV;
  const G4double eMax = 3000.0 * MeV;
  const G4int nBins = 100;
  std::vector<G4double> vEdges(nBins+1);
  for(G4int i=0; i<nBins+1; ++i) {
    vEdges[i] = eMin * std::pow(eMax/eMin, ((double)i)/((double)nBins));
  }
  hId = analysisManager->CreateH1("spectrum","neutron spectrum entering the detector", vEdges, "MeV");

  // default file name
  if(fileName.isNull())
    fileName = "amstest";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AMSTestRunAction::~AMSTestRunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  analysisManager->OpenFile(fileName+".root");

#ifdef HAS_BOOST_DATE_TIME
  if(isMaster) {
    startTime = bpt::microsec_clock::local_time();
    G4cout << "Run starts at " << startTime << G4endl;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestRunAction::EndOfRunAction(const G4Run *run)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if(analysisManager->GetH1(hId)) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }

    G4cout << " neutron energy: mean = "
      << G4BestUnit(analysisManager->GetH1(hId)->mean(), "Energy")
      << " rms = "
      << G4BestUnit(analysisManager->GetH1(hId)->rms(),  "Energy") << G4endl;
  }

  // scale the histograms appropriately
  if(isMaster) {
    const G4double nEvents = run->GetNumberOfEvent();
    const G4double factor=1.0/nEvents;
    G4cout << "normalization: " << factor << G4endl;
    analysisManager->ScaleH1(hId, factor);

    dumpHistogramToDAT(hId,   fileName + ".dat");
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();

#ifdef HAS_BOOST_DATE_TIME
  if(isMaster) {
    endTime = bpt::microsec_clock::local_time();
    G4cout << "Run ends at " << endTime << G4endl;
    G4cout << "Elapsed time " << endTime - startTime << G4endl;
  }
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AMSTestRunAction::dumpHistogramToDAT(const G4int hID, const G4String &fname) const {
  // Dump the content of the histogram in a format readable by xmgrace
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Writing data file " << fname << G4endl;
  std::ofstream oFile(fname);
  tools::histo::h1d *h = analysisManager->GetH1(hID);
  const G4int nBins = h->axis().bins();
  for(G4int i=0; i<nBins; ++i) {
    oFile << h->axis().bin_lower_edge(i) << '\t' << h->bin_height(i) << '\t'
      << h->bin_error(i) << '\n';
  }
  oFile << h->axis().bin_upper_edge(nBins-1) << "\t0.\t0.\n";
  oFile.close();
}
