#ifndef construction_h
#define construction_h 1

#include <utility>
#include <map>
#include <fstream>

#include <rapidxml/rapidxml.hpp>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SDParticleFilter.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "detector.hh"
#include "detectorRadDecay.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
	MyDetectorConstruction(SimpleRunAction *arun);
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

private:
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	SimpleRunAction *run;
	
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> BuildWorld();
	void BuildRadioaciveHead(G4LogicalVolume *logicWorld);
	void BuildWaterFantom(G4LogicalVolume *logicWorld);
	void BuildContainer(G4LogicalVolume *logicWorld);

	void ReadSizes();
	void ReadMaterials();
	void SetDistance();
	virtual void ConstructSDandField();
	// SIZES
	// world
	G4double world_sizeA;
	G4double world_sizeB;
	G4double world_sizeC;
	// radioactive head
	G4double biasingHead = 0*CLHEP::cm;
	G4bool radioactiveheadPlaced;
	G4bool sourceSens;
		// frame
		G4double Rsphere = 0 * CLHEP::cm;
		G4double biasEdge_frame;
		G4double biasing_frame;
		G4double edge_frame;

		// source
		G4double source_active_diameter = 0 * CLHEP::mm;
		G4double source_active_height = 0 * CLHEP::mm;
		G4double source_diameter = 0 * CLHEP::mm;
		G4double source_height = 0 * CLHEP::mm;

	// container
	G4bool containerPlaced;
	G4double thikness_container = 0 * CLHEP::mm;
	G4double edge_container = 0 * CLHEP::mm;
	G4double placement_container = 0 * CLHEP::mm;

	// fantom
	G4bool fantomPlaced;
	G4bool fantomSensitive;
	G4double fantom_sizeA;
	G4double fantom_sizeB;
	G4double fantom_sizeC;

	// MATERIALS
	// uran
	G4double ro_uran;
	std::vector<G4int> Z_uran;
	std::vector<G4int> A_uran;
	std::vector<G4double> mass_uran;
	std::vector<G4double> perCent_uran;

	// steel
	// steel12
	G4double ro_steel12;
	std::map<std::string, G4double> mat_steel12;
	// steel02
	G4double ro_steel02;
	std::map<std::string, G4double> mat_steel02;

};

#endif