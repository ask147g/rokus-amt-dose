#include <rapidxml/rapidxml.hpp>

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
	do std::cin >> t;
	while (t < 0 || t > 8);
	
	TypeCalculations::SetType(t);
	return types.GetTypeCalc();
}

int main(int argc, char** argv) {
	CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
	CLHEP::HepRandom::setTheSeed((unsigned)clock());
	
	#ifdef G4MULTITHREADED
		G4MTRunManager *runManager = new G4MTRunManager();
		runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores()); // add max
	#else
		G4RunManager *runManager = new G4RunManager();
	#endif

	int ttype = SetTypeCalculations();

	G4VUserPhysicsList *physicsList = new MyPhysicsList();
	physicsList->SetVerboseLevel(0);
  	runManager->SetUserInitialization(physicsList);

	SourceActivity *Activity = new SourceActivity();
	float Bq = Activity->GetActivity();
	int particles = Bq/Activity->GetIncreasing()*Activity->GetTime();
	std::cout << "Particles:" << particles << std::endl;
	
	SimpleRunAction *run = new SimpleRunAction();
	run->SetDecreasingActivity(Activity->GetIncreasing());
	run->SetTime(Activity->GetTime());
	delete Activity;
	run->SetNumEvents(particles);
	MyDetectorConstruction *construct = new MyDetectorConstruction(run);
	MyActionInitialization *action = new MyActionInitialization(run);
	runManager->SetUserInitialization(action);
	runManager->SetUserInitialization(construct);
	runManager->SetVerboseLevel(0);
	runManager->Initialize();

	if (ttype == 0 || ttype == 2 || ttype == 3 || ttype == 5 || ttype == 6 || ttype == 8)
		runManager->BeamOn(particles);
		runManager->BeamOn(particles);
	
	if (ttype == 1 || ttype == 4 || ttype == 7) {
		G4UIExecutive *ui = new G4UIExecutive(argc,argv);

		G4VisManager *visManager = new G4VisExecutive();
		visManager->Initialize();

		G4UImanager *UImanager = G4UImanager::GetUIpointer();

		UImanager->ApplyCommand("/control/execute macro/vis.mac");

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> *root_node;

    	std::ifstream theFile ("data/constructData.xml");
    	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    	buffer.push_back('\0');
    	doc.parse<0>(&buffer[0]);
		std::string string;
		bool is;
		if (ttype == 1) {
			root_node = doc.first_node("radioactivehead")->first_node("source");
			string = root_node->first_attribute("cap2")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/cap1.mac");


			string = root_node->first_attribute("cap2")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/cap2.mac");


			root_node = doc.first_node("container");
			string = root_node->first_attribute("placed")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/container.mac");
		}
		if (ttype == 4) {
			root_node = doc.first_node("radioactivehead")->first_node("source");
			string = root_node->first_attribute("cap2")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/cap1.mac");


			string = root_node->first_attribute("cap2")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/cap2.mac");


			root_node = doc.first_node("container");
			string = root_node->first_attribute("placed")->value();
			std::istringstream(string) >> std::boolalpha >> is;
			if (is) UImanager->ApplyCommand("/control/execute macro/container.mac");
		}
		if (ttype == 7) {
			UImanager->ApplyCommand("/control/execute macro/container.mac");
		}
		ui->SessionStart();
	}
	delete runManager;
	
	return 0;
}
