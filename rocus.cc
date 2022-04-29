#include "G4MTRunManager.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "action.hh"
#include "physics.hh"
#include "run.hh"
#include "setActivity.hh"
#include "typeCalc.hh"

int TypeCalculations::typeCalc = 0;

int SetTypeCalculations() {
	TypeCalculations types = TypeCalculations();
	types.TypeMessenger();
	int t;
	do {
		std::cin >> t;
	} while (t < 0 || t > 8);
	
	TypeCalculations::SetType(t);
	return types.GetTypeCalc();
}

int main(int argc, char** argv) {
	CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
	CLHEP::HepRandom::setTheSeed((unsigned)clock());
	
	#ifdef G4MULTITHREADED
		G4MTRunManager *runManager = new G4MTRunManager();
		runManager->SetNumberOfThreads(8); // add max
	#else
		G4RunManager *runManager = new G4RunManager();
	#endif

	int ttype = SetTypeCalculations();

	G4VUserPhysicsList *physicsList = new MyPhysicsList();
	physicsList->SetVerboseLevel(0);
  	runManager->SetUserInitialization(physicsList);

	SourceActivity *Activity = new SourceActivity();
	float Bq = Activity->GetActivity();
	int particles = Bq/Activity->GetIncreasing();
	
	SimpleRunAction *run = new SimpleRunAction();
	run->SetDecreasingActivity(Activity->GetIncreasing());
	delete Activity;
	run->SetNumEvents(particles);
	MyDetectorConstruction *construct = new MyDetectorConstruction(run);
	MyActionInitialization *action = new MyActionInitialization(run);
	runManager->SetUserInitialization(action);
	runManager->SetUserInitialization(construct);
	runManager->SetVerboseLevel(0);
	runManager->Initialize();

	if (ttype == 0 || ttype == 2 || ttype == 3 || ttype == 5)
		runManager->BeamOn(particles);
	
	if (ttype == 1 || ttype == 4) {
		G4UIExecutive *ui = new G4UIExecutive(argc,argv);

		G4VisManager *visManager = new G4VisExecutive();
		visManager->Initialize();

		G4UImanager *UImanager = G4UImanager::GetUIpointer();

		UImanager->ApplyCommand("/control/execute vis.mac");
		//UImanager->ApplyCommand("/control/execute plot.mac");

		ui->SessionStart();
	}
	delete runManager;
	
	return 0;
}
