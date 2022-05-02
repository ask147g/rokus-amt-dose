#include "detectorPlane.hh"

PlaneDetector::PlaneDetector(G4String name, SimpleRunAction *arun) : G4VSensitiveDetector(name), run(arun) {}

PlaneDetector::~PlaneDetector() {}

void PlaneDetector::Initialize(G4HCofThisEvent*) {}

G4bool PlaneDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
  if (!isMass) {
    G4double mass = step->GetTrack()->GetVolume()->GetLogicalVolume()->GetMass();
    run->SetVolumeMass(mass/CLHEP::kg);
    isMass = true;
  }
  if (TypeCalculations::GetTypeCalc() >= 3 && TypeCalculations::GetTypeCalc() <= 5)
    run->PlaneEdep(step->GetTotalEnergyDeposit()/CLHEP::joule, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber());
  else
    run->ClosetEdepCalc(step->GetTotalEnergyDeposit()/CLHEP::joule, step->GetPreStepPoint()->GetTouchable()->GetCopyNumber());

  return true;
}

void PlaneDetector::EndOfEvent(G4HCofThisEvent*) {}
