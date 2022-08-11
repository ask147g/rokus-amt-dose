#ifndef run_h
#define run_h 1

#include <fstream>
#include "math.h"

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

#include "setActivity.hh"
#include "typeCalc.hh"

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
  void SetTime(double time) {this->time = time;};
  void SetNumEvents(double num) {numEvents = num;};
  void PlaneEdep(G4double edep, G4int copy);
  void ClosetEdepCalc(G4double edep, G4int copy);
  void SetParametersPlane(G4int amount, G4double step);
  void SetParametersCloset(G4int amount);
private:
  G4double mass;
  G4double Edep = 0;
  double decreasingActivity = 1;
  double time = 1;
  double numEvents = 0;
  G4double placement_container = 0;
  G4double planeBiasing;
  G4int planeAmount = 0;
  G4double planeStep;

  G4int closetAmount = 0;

  std::map<std::pair<G4double, G4double>, G4double > PlaneXYEdep;
  std::map<G4int, G4double > ClosetEdep;
  G4double SetDistance();
  void SetDistanceType0();

  void ResOutputOneContainer(const G4Run* run);
  void ResOutputPlane(const G4Run* run);
  void ResOutputPlaneDistance(const G4Run* run);
  void ResOutputCloset(const G4Run* run);

  std::pair<G4double, G4double> GetXY(G4int copy);
};

#endif