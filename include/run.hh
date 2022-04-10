#ifndef RUN.HH
#define RUN.HH

#include <fstream>
#include "math.h"

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RootAnalysisManager.hh"
#include "G4AnalysisManager.hh"

class G4Run;

class SimpleRunAction : public G4UserRunAction {
public:
  SimpleRunAction();
  virtual ~SimpleRunAction();

  void GetValue(G4double edep);

  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  void SetVolumeMass(G4double massin) {mass = massin;}
private:
  G4double mass;
  G4double Edep = 0;
};

#endif