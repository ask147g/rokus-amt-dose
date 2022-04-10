#include "detectorRadDecay.hh"

DetectorRadDecay::DetectorRadDecay(G4String name) : G4VSensitiveDetector(name) {}

DetectorRadDecay::~DetectorRadDecay() {}

void DetectorRadDecay::Initialize(G4HCofThisEvent*) {}

G4bool DetectorRadDecay::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  G4double kine = step->GetTrack()->GetKineticEnergy();
  G4String name = step->GetTrack()->GetParticleDefinition()->G4ParticleDefinition::GetParticleName();

  if (name == "gamma") analysisManager->FillH1(0, kine/CLHEP::keV);
  if (name == "e-") analysisManager->FillH1(1, kine/CLHEP::keV);
  return true;
}

void DetectorRadDecay::EndOfEvent(G4HCofThisEvent*) {}