#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(G4double bias) : biasingHead(bias) {
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("geantino");
	
	sourceCo = new G4SingleParticleSource();
	sourceCo->SetParticleDefinition(particle);
	G4SPSEneDistribution* energy = sourceCo->GetEneDist();
	energy->SetEnergyDisType("Mono");
	energy->SetMonoEnergy(0*CLHEP::keV);
	G4SPSPosDistribution* position = sourceCo->GetPosDist();
	position->SetPosDisType("Volume");
	position->SetPosDisShape("Cylinder");
	position->SetPosRot1(G4ThreeVector(0*CLHEP::degree, 0*CLHEP::degree, 90*CLHEP::degree));
	position->SetPosRot2(G4ThreeVector(0*CLHEP::degree, 90*CLHEP::degree, 0*CLHEP::degree));
	position->SetHalfZ(24.5/2.*CLHEP::mm);
	position->SetRadius(21/2.*CLHEP::mm);
	position->SetCentreCoords(G4ThreeVector(0.0*CLHEP::cm, 0.0*CLHEP::cm, biasingHead+24.5/2*CLHEP::mm));
	G4SPSAngDistribution* angular = sourceCo->GetAngDist();
	angular->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
	
	sourceCo->SetNumberOfParticles(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator() {delete sourceCo;}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
    
    if(particle == G4Geantino::Geantino()) {
        G4int Z = 27;
		G4int A = 60;
        
        G4double charge = 0*CLHEP::eplus;
        G4double energy = 0*CLHEP::keV;
        
        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);
        sourceCo->SetParticleDefinition(ion);
        sourceCo->SetParticleCharge(charge);
    }
    	sourceCo->GeneratePrimaryVertex(anEvent);
}