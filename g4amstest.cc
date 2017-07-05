#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "Randomize.hh"

#include "AMSTestDetectorConstruction.hh"
#include "AMSTestActionInitialization.hh"

#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"

#ifdef HAS_BOOST_PROGRAM_OPTIONS
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  G4int numberOfThreads = 1;
  G4String physicsList = "FTFP_INCLXX";
  G4String macroFile;
  G4String output;

#ifdef HAS_BOOST_PROGRAM_OPTIONS
  // define the CLI options
  po::options_description runOptDesc("Run options");
  runOptDesc.add_options()
    ("help,h", "produce this help message")
    ("physics-list,l", po::value<std::string>(&physicsList)->default_value("FTFP_BERT_EMY"), "physics list to be used")
    ("threads,t", po::value<G4int>(&numberOfThreads)->default_value(1), "number of threads for the calculation")
    ("output,o", po::value<std::string>(&output), "stem of the output files")
    ;
  po::options_description hiddenOptDesc("Hidden options");
  hiddenOptDesc.add_options()
    ("input-file", po::value<std::string>(&macroFile), "macro file")
    ;

  po::options_description allOptions;
  allOptions.add(runOptDesc).add(hiddenOptDesc);

  po::positional_options_description p;
  p.add("input-file", 1);

  // Disable guessing of option names
  const int cmdstyle =
    po::command_line_style::default_style &
    ~po::command_line_style::allow_guessing;

  // Result of the option processing
  po::variables_map variablesMap;
  po::store(po::command_line_parser(argc, argv).
            style(cmdstyle).
            options(allOptions).positional(p).run(), variablesMap);
  po::notify(variablesMap);

  // -h/--help option
  if(variablesMap.count("help")) {
    G4cout
      << "Usage: g4amstest [options] <macro_file>" << G4endl
      << runOptDesc << G4endl;
    return 0;
  }

  // -o/--output option
  if(!variablesMap.count("output")) {
    output = macroFile;
  }

  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if(macroFile.isNull()) {
    ui = new G4UIExecutive(argc, argv);
  }
#else
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
#endif

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#ifdef HAS_BOOST_PROGRAM_OPTIONS
  // -t/--threads option
  if(variablesMap.count("threads")) {
    runManager->SetNumberOfThreads(numberOfThreads);
  }
#endif

#else
  G4RunManager* runManager = new G4RunManager;
#ifdef HAS_BOOST_PROGRAM_OPTIONS
  // -t/--threads option
  if(variablesMap.count("threads")) {
    G4cout << "WARNING: -t/--threads option is ignored in sequential mode" << G4endl;
  }
#endif
#endif

  // Set mandatory initialization classes
  //
  AMSTestDetectorConstruction *theDetector = new AMSTestDetectorConstruction;
  runManager->SetUserInitialization(theDetector);
  //
  G4PhysListFactory physListFactory;
  G4VModularPhysicsList* physList = physListFactory.GetReferencePhysList(physicsList);
  physList->SetDefaultCutValue(100.*um); // sufficient to get the electron production threshold below 100 keV
  physList->DumpCutValuesTable();
  runManager->SetUserInitialization(physList);

  // Set user action initialization
  //
  runManager->SetUserInitialization(new AMSTestActionInitialization(output));

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = macroFile;
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
