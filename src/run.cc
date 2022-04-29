#include "run.hh"

SimpleRunAction::SimpleRunAction() : G4UserRunAction() {
	int typeCalc = TypeCalculations::GetTypeCalc();
  if (typeCalc == 0) SetDistanceType0();
  if (typeCalc == 1) SetDistanceType0();
	if ((typeCalc) == 2) SetDistance();
}

SimpleRunAction::~SimpleRunAction() {}

void SimpleRunAction::BeginOfRunAction(const G4Run* aRun) {
  for (G4int x = 0; x < planeAmount; ++x) {
    for (G4int y = 0; y <planeAmount; ++y) {
      PlaneXYEdep.insert({std::make_pair(x*planeStep/10., y*planeStep/10.), 0.}); // cm
    }
  }

  std::ofstream out("planeResult.csv", std::ios_base::out | std::ios_base::trunc);
  out.close();
}

void SimpleRunAction::EndOfRunAction(const G4Run* run) {
  Edep = 100*Edep*decreasingActivity/(mass+0.);
  if (TypeCalculations::GetTypeCalc() >= 0 && TypeCalculations::GetTypeCalc() <= 3)
    ResOutputOneContainer(run);
  if (TypeCalculations::GetTypeCalc() >= 3 && TypeCalculations::GetTypeCalc() <= 5)
    ResOutputPlane(run);
}

void SimpleRunAction::GetValue(G4double edep) {Edep += edep;}

void SimpleRunAction::SetDistance() {
  std::ifstream dist;
	dist.open("macro/distance.txt", std::ios::in);
	if (dist) {
		while (1) {
			if (dist.eof()) break;
			dist >> placement_container;
		}
	}
}

void SimpleRunAction::SetDistanceType0() {
  rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

  std::ifstream theFile ("data/constructData.xml");
  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');
  doc.parse<0>(&buffer[0]);
  root_node = doc.first_node("container");
  placement_container = std::stof(root_node->first_node("placement")->value());
}

void SimpleRunAction::ResOutputOneContainer(const G4Run* run) {
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents != numEvents) return;
  G4cout << Edep << " rad/sec " << placement_container << " cm " << nofEvents << G4endl;  // 100 from Sv to rad

  std::ofstream out("result.csv", std::ios_base::app);
  if (out.is_open()) {
    out << Edep << " " << placement_container << std::endl;  // 100 from Sv to rad
    out.close();
  }
}

void SimpleRunAction::ResOutputPlane(const G4Run* run) {
  std::ofstream out("planeResult.csv", std::ios_base::app);
  if (out.is_open()) {
    std::map<std::pair<G4double, G4double>, G4double>::iterator it_Edep = PlaneXYEdep.begin();
    for (it_Edep; it_Edep != PlaneXYEdep.end(); ++it_Edep) {
      out << (it_Edep->first).first << " " << (it_Edep->first).second << " " << it_Edep->second << std::endl;
    }
  out.close();
  }
}

void SimpleRunAction::PlaneEdep(G4double edep, G4int copy) {
  PlaneXYEdep.find(GetXY(copy))->second += 100*edep*decreasingActivity/(mass+0.);
}

std::pair<G4double, G4double> SimpleRunAction::GetXY(G4int copy) {
  G4int yy = ceil(copy/planeAmount);
  G4int xx = copy % planeAmount;
  return std::make_pair(planeStep*xx/10., planeStep*yy/10.);  // cm
}

void SimpleRunAction::SetParametersPlane(G4int amount, G4double step) {
  planeAmount = amount;
  planeStep = step;
}