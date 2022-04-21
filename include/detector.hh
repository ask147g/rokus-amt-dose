#ifndef detector_h
#define detector_h 1

#include "G4VSensitiveDetector.hh"

#include "run.hh"

class G4Step;
class G4HCofThisEvent;

class MySensitiveDetector : public G4VSensitiveDetector {
public:
  MySensitiveDetector(G4String name, SimpleRunAction *arun);
  virtual ~MySensitiveDetector();
  
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
private:
  SimpleRunAction *run;
  G4bool isMass = false;
  G4int counter;
};

#endif