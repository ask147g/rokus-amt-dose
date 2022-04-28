#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, SimpleRunAction *arun) : G4VSensitiveDetector(name), run(arun) {}

MySensitiveDetector::~MySensitiveDetector() {
  delete run;
}

void MySensitiveDetector::Initialize(G4HCofThisEvent*) {}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
  if (!isMass) {
    G4double mass = step->GetTrack()->GetVolume()->GetLogicalVolume()->GetMass();
    run->SetVolumeMass(mass/CLHEP::kg);
    isMass = true;
  }
  run->GetValue(step->GetTotalEnergyDeposit()/CLHEP::joule);
  return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*) {}
