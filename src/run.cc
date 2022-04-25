#include "run.hh"

SimpleRunAction::SimpleRunAction() : G4UserRunAction() {SetDistance();}

SimpleRunAction::~SimpleRunAction() {}

void SimpleRunAction::BeginOfRunAction(const G4Run* aRun) {}

void SimpleRunAction::EndOfRunAction(const G4Run* run) {
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents != numEvents) return;
  G4cout << 100*Edep*decreasingActivity/(mass+0.) << " rad/sec " << nofEvents << G4endl;  // 100 from Sv to rad

  std::ofstream out("result.csv", std::ios_base::app);
  if (out.is_open())
    out << 100*Edep*decreasingActivity/(mass+0.) << " rad/sec " << placement_container << " cm " << nofEvents << std::endl;  // 100 from Sv to rad
  out.close();
}

void SimpleRunAction::GetValue(G4double edep) {Edep += edep;}

void SimpleRunAction::SetDistance() {
  std::ifstream dist;
	dist.open("distance_macro/distance.txt", std::ios::in);
	if (dist) {
		while (1) {
			if (dist.eof()) break;
			dist >> placement_container;
		}
	}
}