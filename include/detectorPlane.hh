#ifndef detectorPlane_h
#define detectorPlane_h 1

#include "G4VSensitiveDetector.hh"

#include "run.hh"

class G4Step;
class G4HCofThisEvent;

class PlaneDetector : public G4VSensitiveDetector {
public:
  PlaneDetector(G4String name, SimpleRunAction *arun);
  virtual ~PlaneDetector();
  
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
private:
  SimpleRunAction *run;
  G4bool isMass = false;
};

#endif