#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction(SimpleRunAction *arun): G4VUserDetectorConstruction(), run(arun) {}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
	//G4SDManager* SDman = G4SDManager::GetSDMpointer();

	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> world;
	world = BuildWorld();
	
	BuildWaterFantom(world.first, SDman);
	BuildRadioaciveHead(world.first, SDman, false);
	
	return world.second;
}

std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MyDetectorConstruction::BuildWorld() {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

	G4Box *solidWorld = new G4Box("solidWorld", 6*m, 6*m, 6*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
								G4ThreeVector(0, 0, 0),
								logicWorld,
								"physWorld",
								0,
								false,
								0,
								true);

	return std::make_pair(logicWorld, physWorld);
}

void MyDetectorConstruction::BuildRadioaciveHead(G4LogicalVolume *logicWorld, G4SDManager *SDman, G4bool isDet) {
	G4double roU = 18.9*g/cm3;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *galacyMat = nist->FindOrBuildMaterial("G4_Galactic");

	G4Material* Uran = new G4Material("Uran",roU,1);
	G4Isotope* U5 = new G4Isotope("U235", 92, 235, 235.01*g/mole);
	G4Isotope* U8 = new G4Isotope("U238", 92, 238, 238.03*g/mole);
	G4Element* elU = new G4Element("enriched Uranium", "U", 2);
	elU->AddIsotope(U5, 90.*perCent);
	elU->AddIsotope(U8, 10.*perCent);
	Uran -> AddElement(elU, 100.*perCent);

	// source
	G4Tubs *sourceSolid = new G4Tubs("sourceSolid",
									0,
									21/2.*CLHEP::mm,
									24.5/2.*CLHEP::mm,
									0,
									360);

	G4LogicalVolume *sourceLogic = new G4LogicalVolume(sourceSolid, galacyMat, "sourceLogic");
	
	G4VPhysicalVolume *sourcePhys = new G4PVPlacement(0,
								G4ThreeVector(0, 0, biasingHead+24.5/2*CLHEP::mm),
								sourceLogic,
								"sourcePhys",
								logicWorld,
								true, 	
								0, 
								false);

	// head
	G4VSolid *headSolid = new G4Orb("headSolid", 420/2*CLHEP::mm);

	G4VSolid *headSolidMinus = new G4Cons("headSolidMinus",
											0,
											120/2*CLHEP::mm,
											0,
											21/2.*CLHEP::mm,
											225/2.*CLHEP::mm,
											0,
											360);
	
	headSolid = new G4SubtractionSolid("headSolid", headSolid, headSolidMinus, 0, G4ThreeVector(0,0,-225/2.*CLHEP::mm));
	headSolid = new G4SubtractionSolid("headSolid", headSolid, sourceSolid, 0, G4ThreeVector(0,0,24.5/2*CLHEP::mm));

	G4LogicalVolume *headLogic = new G4LogicalVolume(headSolid, Uran, "headLogic");

	G4VPhysicalVolume *headPhys = new G4PVPlacement(0,
								G4ThreeVector(0, 0, biasingHead), 
								headLogic, 
								"headPhys",
								logicWorld, 
								true,
								0, 
								false);

	G4VSensitiveDetector* sdRadDecay = new DetectorRadDecay("detectorRadDecay");
	SetSensitiveDetector("sourceLogic", sdRadDecay, true);
	SDman->AddNewDetector(sdRadDecay);
}

void MyDetectorConstruction::BuildWaterFantom(G4LogicalVolume *logicWorld, G4SDManager *SDman) {
	G4double containerSize = 0.1*m;
	G4double containerPlacememnt = -biasingHead+840*cm+225/2.*mm+containerSize;
	G4double containerThickness = 1*mm;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat2 = nist->FindOrBuildMaterial("G4_Si");
	G4Material *PbMat = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");

	// Water fantom
	G4Box *fantomWorld = new G4Box("fantomWorld",
					(containerSize/2),
					(containerSize/2),
					(containerSize/2));
	
	
	G4LogicalVolume *fantomLogic = new G4LogicalVolume(fantomWorld, worldMat2, "fantomLogic");

	G4VPhysicalVolume *fantomPhys = new G4PVPlacement(0,
								G4ThreeVector(0, 0, -(containerPlacememnt+containerSize/2)), 
								fantomLogic, 
								"fantomPhys",
								logicWorld, 
								true,
								0, 
								false);

	// Container Pb
	G4VSolid *PbWorld = new G4Box("PbWorld",
					containerSize/2+containerThickness,
					containerSize/2+containerThickness,
					containerSize/2+containerThickness);
	
	// Container Al
	G4VSolid *AlWorld = new G4Box("AlWorld",
					containerSize/2+2*containerThickness,
					containerSize/2+2*containerThickness,
					containerSize/2+2*containerThickness);

	AlWorld = new G4SubtractionSolid("AlWorld", AlWorld, PbWorld, 0, G4ThreeVector(0,0,0));
	PbWorld = new G4SubtractionSolid("PbWorld", PbWorld, fantomWorld, 0, G4ThreeVector(0,0,0));

	G4LogicalVolume *PbLogic = new G4LogicalVolume(PbWorld, PbMat, "PbLogic");

	G4VPhysicalVolume *PbPhys = new G4PVPlacement(0,
								G4ThreeVector(0, 0, -(containerPlacememnt+containerSize/2)), 
								PbLogic, 
								"PbPhys",
								logicWorld, 
								true,
								0, 
								false);

	G4LogicalVolume *AlLogic = new G4LogicalVolume(AlWorld, AlMat, "AlLogic");

	G4VPhysicalVolume *AlPhys = new G4PVPlacement(0,
								G4ThreeVector(0, 0, -(containerPlacememnt+containerSize/2)), 
								AlLogic, 
								"AlPhys",
								logicWorld, 
								true,
								0, 
								false);
	
	const std::vector<G4String> part = {"e-", "gamma"};
	G4SDParticleFilter* Filter = new G4SDParticleFilter("Filter",part);
	
	G4VSensitiveDetector* sd = new MySensitiveDetector("detectorField", run);
	SetSensitiveDetector("fantomLogic", sd, true);
	sd->SetFilter(Filter);
	SDman->AddNewDetector(sd);
}

void MyDetectorConstruction::ConstructSDandField() {
	const std::vector<G4String> part = {"e-", "gamma"};
	G4SDParticleFilter* Filter = new G4SDParticleFilter("Filter",part);
	
	G4VSensitiveDetector* sd = new MySensitiveDetector("detectorField", run);
	SetSensitiveDetector("fantomLogic", sd, true);
	sd->SetFilter(Filter);
	SDman->AddNewDetector(sd);
}