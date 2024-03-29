#ifndef detectorRadDecay_h
#define detectorRadDecay_h 1

#include <vector>

#include "G4VSensitiveDetector.hh"
#include "G4RootAnalysisManager.hh"
#include "G4AnalysisManager.hh"

#include "run.hh"

class G4Step;
class G4HCofThisEvent;

class DetectorRadDecay : public G4VSensitiveDetector {
public:
  DetectorRadDecay(G4String name);
  virtual ~DetectorRadDecay();

  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
private:
  std::vector <G4double> gamma_data;
  std::vector <G4double> electron_data;
  std::string detector;
};

#endif