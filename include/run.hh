#ifndef run_h
#define run_h 1

#include <fstream>
#include "math.h"

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RootAnalysisManager.hh"
#include "g4root.hh"

#include "setActivity.hh"

class G4Run;

class SimpleRunAction : public G4UserRunAction {
public:
  SimpleRunAction();
  virtual ~SimpleRunAction();

  void GetValue(G4double edep);

  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  void SetVolumeMass(G4double massin) {mass = massin;}
  void SetDecreasingActivity(double dec) {decreasingActivity = dec;};
  void SetNumEvents(double num) {numEvents = num;};
  void SetDistance();
private:
  G4double mass;
  G4double Edep = 0;
  double decreasingActivity = 1;
  double numEvents = 0;
  G4double placement_container = 0;
};

#endif