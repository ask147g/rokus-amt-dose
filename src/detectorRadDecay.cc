#include "detectorRadDecay.hh"

DetectorRadDecay::DetectorRadDecay(G4String name) : G4VSensitiveDetector(name), detector(name) {}

DetectorRadDecay::~DetectorRadDecay() {
  std::pair<std::string, std::string> output = std::make_pair("output/spectrum/"+detector+"_gamma.csv", "output/spectrum/"+detector+"_electron.csv");
  std::ofstream gamma_out(output.first, std::ios_base::out | std::ios_base::trunc);
    if (gamma_out.is_open()) {
      for (std::vector<G4double>::iterator i = gamma_data.begin(); i != gamma_data.end(); ++i)
        gamma_out << *i << std::endl;
      gamma_out.close();
    }
  
  std::ofstream electron_out(output.second, std::ios_base::out | std::ios_base::trunc);
    if (electron_out.is_open()) {
      for (std::vector<G4double>::iterator i = electron_data.begin(); i != electron_data.end(); ++i)
        electron_out << *i << std::endl;
      electron_out.close();
    }
}

void DetectorRadDecay::Initialize(G4HCofThisEvent*) {}

G4bool DetectorRadDecay::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4double kine = step->GetTrack()->GetKineticEnergy();
  G4String name = step->GetTrack()->GetParticleDefinition()->G4ParticleDefinition::GetParticleName();
  if (kine == 0) return true;


  // change container or with huge amount of particles gonna be a crush
  if (name == "gamma") {
    gamma_data.push_back(kine/CLHEP::keV);
  }

  if (name == "e-") {
    electron_data.push_back(kine/CLHEP::keV);
  }
  return true;
}

void DetectorRadDecay::EndOfEvent(G4HCofThisEvent*) {}