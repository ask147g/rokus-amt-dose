#include "run.hh"

SimpleRunAction::SimpleRunAction() : G4UserRunAction() {
  auto analysisManager = G4AnalysisManager::Instance();
  
  G4int id = analysisManager->CreateH1("detector1", "Gamma", 100, 0, 2000);
  G4int id2 = analysisManager->CreateH1("detector2", "Electron", 100, 0, 1000);
  analysisManager->SetH1Title(id, "Gamma");
  analysisManager -> SetH1Title(id2, "Electron");
  analysisManager -> SetH1XAxisTitle(id, "kine, keV");
  analysisManager -> SetH1XAxisTitle(id2, "kine, keV");
  analysisManager -> SetH1YAxisTitle(id, "N, particle");
  analysisManager -> SetH1YAxisTitle(id2, "N, particle");
}

SimpleRunAction::~SimpleRunAction() {}

void SimpleRunAction::BeginOfRunAction(const G4Run* aRun) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("output.root");
  }

void SimpleRunAction::EndOfRunAction(const G4Run* run) {
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents != 89810) return;
  G4cout << Edep*pow(10, 11)/(mass+0.) << " rad/sec " << nofEvents << G4endl;

  std::ofstream out("result.txt", std::ios_base::app);
  if (out.is_open()) {
    out << Edep*pow(10, 11)/(mass+0.) << " rad/sec " << nofEvents << std::endl;
  }

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

void SimpleRunAction::GetValue(G4double edep) {Edep += edep;}
