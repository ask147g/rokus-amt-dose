#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction(SimpleRunAction *arun): G4VUserDetectorConstruction(), run(arun) {
	ReadSizes();
	ReadMaterials();
}


MyDetectorConstruction::~MyDetectorConstruction() {}


G4VPhysicalVolume *MyDetectorConstruction::Construct() {
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> world;
	world = BuildWorld();
	
	if (containerPlaced)
		BuildContainer(world.first);
	if (fantomPlaced)
		BuildWaterFantom(world.first);
	if (radioactiveheadPlaced)
		BuildRadioaciveHead(world.first);
	
	return world.second;
}


void MyDetectorConstruction::ReadSizes() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

    std::ifstream theFile ("data/constructData.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
	
	// world
	root_node = doc.first_node("world");
	world_sizeA = std::stof(root_node->first_node("sizeA")->value()); world_sizeA *= CLHEP::m;
	world_sizeB = std::stof(root_node->first_node("sizeB")->value()); world_sizeB *= CLHEP::m;
	world_sizeC = std::stof(root_node->first_node("sizeC")->value()); world_sizeC *= CLHEP::m;
	
	// radioactive head
    root_node = doc.first_node("radioactivehead");
	radioactiveheadPlaced = root_node->first_attribute("placed")->value();
	biasingHead = std::stof(root_node->first_node("biasingHead")->value()); biasingHead *= CLHEP::cm;
		// frame
		Rsphere = std::stof(root_node->first_node("frame")->first_node("sphere")->value()); Rsphere *= CLHEP::mm;
		
		// source
		sourceSens = root_node->first_node("source")->first_attribute("sensitive")->value();
		source_active_diameter = std::stof(root_node->first_node("source")->first_node("source_active_diameter")->value()); source_active_diameter *= CLHEP::mm;
		source_active_height = std::stof(root_node->first_node("source")->first_node("source_active_height")->value()); source_active_height *= CLHEP::mm;
		source_diameter = std::stof(root_node->first_node("source")->first_node("source_diameter")->value()); source_diameter *= CLHEP::mm;
		source_height = std::stof(root_node->first_node("source")->first_node("source_height")->value()); source_height *= CLHEP::mm;

	// container
	root_node = doc.first_node("container");
	containerPlaced = root_node->first_attribute("placed")->value();
	thikness_container = std::stof(root_node->first_node("thikness")->value()); thikness_container *= CLHEP::mm;	
	edge_container = std::stof(root_node->first_node("edge")->value()); edge_container *= CLHEP::cm;
	placement_container = std::stof(root_node->first_node("placement")->value()); placement_container *= CLHEP::cm;

	// fantom
	root_node = doc.first_node("fantom");
	fantomPlaced = root_node->first_attribute("placed")->value();
	fantomSensitive = root_node->first_attribute("sensitive")->value();
	fantom_sizeA = std::stof(root_node->first_node("sizeA")->value()); fantom_sizeA *= CLHEP::cm;
	fantom_sizeB = std::stof(root_node->first_node("sizeB")->value()); fantom_sizeB *= CLHEP::cm;
	fantom_sizeC = std::stof(root_node->first_node("sizeC")->value()); fantom_sizeC *= CLHEP::cm;
}


void MyDetectorConstruction::ReadMaterials() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

    std::ifstream theFile ("data/constructMaterial.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

}


std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MyDetectorConstruction::BuildWorld() {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

	G4Box *solidWorld = new G4Box("solidWorld", world_sizeA, world_sizeB, world_sizeC);
	
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


void MyDetectorConstruction::BuildRadioaciveHead(G4LogicalVolume *logicWorld) {
	G4double roU = 18.9*g/cm3;
	G4double roSteel12 = 7920*kg/m3;
	G4double roSteel02 = 7920*kg/m3;
	roSteel12 = 18.9*g/cm3;
	roSteel02 = 18.9*g/cm3;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *galacyMat = nist->FindOrBuildMaterial("G4_Galactic");
	galacyMat = nist->FindOrBuildMaterial("G4_Pb");

	G4Material* Uran = new G4Material("Uran",roU,1);
	G4Isotope* U5 = new G4Isotope("U235", 92, 235, 235.01*g/mole);
	G4Isotope* U8 = new G4Isotope("U238", 92, 238, 238.03*g/mole);
	G4Element* elU = new G4Element("enriched Uranium", "U", 2);
	elU->AddIsotope(U5, 10.*perCent);
	elU->AddIsotope(U8, 90.*perCent);
	Uran -> AddElement(elU, 100.*perCent);

	G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

	G4double diameter_half = (source_diameter - source_active_diameter)/4.;
	G4double height_up = (source_height-source_active_height-2*diameter_half)/4.;

	G4VSolid *innerTubSolid = new G4Tubs("innerTubSolid",
									0,
									(source_active_diameter/2.)+diameter_half,
									(source_active_height+diameter_half+height_up)/2.,
									0,
									360);

	G4VSolid *outerTubSolid = new G4Tubs("outerTubSolid",
									0,
									(source_active_diameter/2.)+2*diameter_half,
									(source_active_height+2*diameter_half+2*height_up)/2.,
									0,
									360);

	// source
	G4Tubs *sourceSolid = new G4Tubs("sourceSolid",
									0,
									source_active_diameter/2.,
									source_active_height/2.,
									0,
									360);
	
	outerTubSolid = new G4SubtractionSolid("outerTubSolid", outerTubSolid, innerTubSolid, 0, G4ThreeVector(0,0,diameter_half));
	innerTubSolid = new G4SubtractionSolid("innerTubSolid", innerTubSolid, sourceSolid, 0, G4ThreeVector(0,0,diameter_half));
	

	G4LogicalVolume *innerTubLogic = new G4LogicalVolume(innerTubSolid, steel, "innerTubLogic");
	G4LogicalVolume *outerTubLogic = new G4LogicalVolume(outerTubSolid, steel, "outerTubLogic");
	
	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.-diameter_half),
					innerTubLogic,
					"innerTubPhys",
					logicWorld,
					true, 	
					0, 
					false);

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.-2*diameter_half),
					outerTubLogic,
					"outerTubPhys",
					logicWorld,
					true, 	
					0, 
					false);

	G4LogicalVolume *sourceLogic = new G4LogicalVolume(sourceSolid, galacyMat, "sourceLogic");
	
	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.),
					sourceLogic,
					"sourcePhys",
					logicWorld,
					true, 	
					0, 
					false);

	// head
	G4VSolid *headSolid = new G4Orb("headSolid", Rsphere/2.*CLHEP::mm);

	G4VSolid *headSolidMinus = new G4Cons("headSolidMinus",
											0,
											24/2.*CLHEP::cm,
											0,
											source_active_diameter/2.,
											225/2.*CLHEP::mm+75/2.*cm,
											0,
											360);
	
	headSolid = new G4SubtractionSolid("headSolid", headSolid, headSolidMinus, 0, G4ThreeVector(0,0,-(225/2.*CLHEP::mm+75/2.*cm)));
	headSolid = new G4SubtractionSolid("headSolid", headSolid, sourceSolid, 0, G4ThreeVector(0,0,source_active_height/2));
	headSolid = new G4SubtractionSolid("headSolid", headSolid, sourceSolid, 0, G4ThreeVector(0,0,source_active_height/2));

	G4LogicalVolume *headLogic = new G4LogicalVolume(headSolid, Uran, "headLogic");

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead), 
					headLogic, 
					"headPhys",
					logicWorld, 
					true,
					0, 
					false);
}


void MyDetectorConstruction::BuildContainer(G4LogicalVolume *logicWorld) {
	G4double containerPlacememnt = -biasingHead+placement_container+225*mm-edge_container/2.;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *PbMat = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");

	G4Box *fantomWorld = new G4Box("fantomWorld",
					(edge_container/2),
					(edge_container/2),
					(edge_container/2));

	// Container Pb
	G4VSolid *PbWorld = new G4Box("PbWorld",
					edge_container/2+thikness_container,
					edge_container/2+thikness_container,
					edge_container/2+thikness_container);
	
	// Container Al
	G4VSolid *AlWorld = new G4Box("AlWorld",
					edge_container/2+2*thikness_container,
					edge_container/2+2*thikness_container,
					edge_container/2+2*thikness_container);

	AlWorld = new G4SubtractionSolid("AlWorld", AlWorld, PbWorld, 0, G4ThreeVector(0,0,0));
	PbWorld = new G4SubtractionSolid("PbWorld", PbWorld, fantomWorld, 0, G4ThreeVector(0,0,0));

	G4LogicalVolume *PbLogic = new G4LogicalVolume(PbWorld, PbMat, "PbLogic");

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, -(containerPlacememnt+edge_container/2)), 
					PbLogic, 
					"PbPhys",
					logicWorld, 
					true,
					0, 
					false);

	G4LogicalVolume *AlLogic = new G4LogicalVolume(AlWorld, AlMat, "AlLogic");

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, -(containerPlacememnt+edge_container/2)), 
					AlLogic, 
					"AlPhys",
					logicWorld, 
					true,
					0, 
					false);
}


void MyDetectorConstruction::BuildWaterFantom(G4LogicalVolume *logicWorld) {
	G4double containerPlacememnt = -biasingHead+placement_container+225*mm-fantom_sizeC/2.;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *SiMat = nist->FindOrBuildMaterial("G4_Si");

	// fantom
	G4Box *fantomWorld = new G4Box("fantomWorld",
					(fantom_sizeA/2),
					(fantom_sizeB/2),
					(fantom_sizeC/2));
	
	G4LogicalVolume *fantomLogic = new G4LogicalVolume(fantomWorld, SiMat, "fantomLogic");
	
	new G4PVPlacement(0,
					G4ThreeVector(0, 0, -(containerPlacememnt+fantom_sizeC/2)), 
					fantomLogic, 
					"fantomPhys",
					logicWorld, 
					true,
					0, 
					false);
}


void MyDetectorConstruction::ConstructSDandField() {
	const std::vector<G4String> part = {"e-", "gamma"};
	G4SDParticleFilter* Filter = new G4SDParticleFilter("Filter",part);
	
	if (fantomPlaced) {
		G4VSensitiveDetector* sd = new MySensitiveDetector("detectorField", run);
		SetSensitiveDetector("fantomLogic", sd, true);
		sd->SetFilter(Filter);
		SDman->AddNewDetector(sd);
	}

	if (fantomPlaced) {
		G4VSensitiveDetector* sdRadDecay = new DetectorRadDecay("fantomRadDecay");
		SetSensitiveDetector("fantomLogic", sdRadDecay, true);
		sdRadDecay->SetFilter(Filter);
		SDman->AddNewDetector(sdRadDecay);
	}

	if (radioactiveheadPlaced && sourceSens) {
		G4VSensitiveDetector* sdRadDecay2 = new DetectorRadDecay("sourceRadDecay");
		SetSensitiveDetector("sourceLogic", sdRadDecay2, true);
		sdRadDecay2->SetFilter(Filter);
		SDman->AddNewDetector(sdRadDecay2);
	}
}