//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// J. Comput. Phys. 274 (2014) 841-882
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// $Id$
//
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmDNAChemistry.hh"
#include "G4PhysicsConstructorRegistry.hh"
#include "CommandLineParser.hh"

//Added by MB
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonPhysics.hh"
//#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"

//For de-excitation MB
#include "G4EmProcessOptions.hh"


//Decay includes are further down
#include "G4PhysicsListHelper.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4GenericIon.hh"
#include "G4DNAGenericIonsManager.hh"

// Warning : the following is needed in order to use EM Physics builders
// e+
#include "G4Positron.hh"
using namespace G4DNAPARSER;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
  double currentDefaultCut   = 1.*nanometer;

  // fixe lower limit for cut
  G4ProductionCutsTable::GetProductionCutsTable()->
      SetEnergyRange(100*eV, 1*GeV);

  SetDefaultCutValue(currentDefaultCut);
  SetVerboseLevel(1);

  // EM physics  
  RegisterConstructor("G4EmDNAPhysics");

  //diegoif(CommandLineParser::GetParser()->GetCommandIfActive("-chemOFF")==0)
  //diego{
    RegisterConstructor("G4EmDNAChemistry");
  //diego}
  //RadioactiveDecay physics. Added by MB
  RegisterPhysics(new G4RadioactiveDecayPhysics);
  RegisterPhysics(new G4IonPhysics);
 // RegisterPhysics(new G4StoppingPhysics); //Nicole: I don't think this is needed
  RegisterPhysics(new G4DecayPhysics); //Nicole: I don't think this is needed
  //RegisterPhysics(new G4EmPenelopePhysics);

//Added MB
  G4EmProcessOptions emOptions;
 emOptions.SetFluo(true); // To activate deexcitation processes and fluorescence
 emOptions.SetAuger(true); // To activate Auger effect if deexcitation is activated
 emOptions.SetPIXE(true); // To activate Particle Induced X-Ray Emission (PIXE)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::RegisterConstructor(const G4String& name)
{
  RegisterPhysics(G4PhysicsConstructorRegistry::Instance()->
      GetPhysicsConstructor(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"

void PhysicsList::ConstructGeneral()
{


  G4int VerboseLevel=0;
// Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() )
    {
      G4ParticleDefinition* particle = particleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (theDecayProcess->IsApplicable(*particle) && !particle->IsShortLived())
        {
          pmanager ->AddProcess(theDecayProcess);
          // set ordering for PostStepDoIt and AtRestDoIt
          pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
          pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }

  // Declare radioactive decay to the GenericIon in the IonTable.
  const G4IonTable *theIonTable =
    G4ParticleTable::GetParticleTable()->GetIonTable();
  G4RadioactiveDecay *theRadioactiveDecay = new G4RadioactiveDecay();

  for (G4int i=0; i<theIonTable->Entries(); i++)
    {
      G4String particleName = theIonTable->GetParticle(i)->GetParticleName();
      G4String particleType = theIonTable->GetParticle(i)->GetParticleType();

      if (particleName == "GenericIon")
        {
          G4ProcessManager* pmanager =
            theIonTable->GetParticle(i)->GetProcessManager();
          pmanager->SetVerboseLevel(VerboseLevel);
          pmanager ->AddProcess(theRadioactiveDecay);
          pmanager ->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
          pmanager ->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
        }
    }


}

/*
void PhysicsList::ConstructParticle()
{
// bosons
  G4Gamma::Gamma();

// leptons
  G4Electron::Electron();
/// additional leptons from DMX example
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
  G4Positron::Positron();

// baryons
  G4Proton::Proton();

  G4GenericIon::GenericIonDefinition();

  G4DNAGenericIonsManager * genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");

}

#include "G4DNAGenericIonsManager.hh" //MB


void PhysicsList::ConstructLeptons()
{
  // leptons - from microdosimetry
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // additional leptons from DMX example
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//DNA
//ENDDNA

void PhysicsList::ConstructBarions()
{
  //  baryons
  G4Proton::ProtonDefinition();
  G4GenericIon::GenericIonDefinition();

  // Geant4 DNA new particles
  G4DNAGenericIonsManager * genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
}

void PhysicsList::ConstructProcess()
{

 // AddTransportation(); MB
 // ConstructEM(); MB
  ConstructGeneral();
}

void PhysicsList::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructBarions();
  //Feb 7, 2017 (NLA)
  //Added based on information at https://twiki.cern.ch/twiki/bin/view/Geant4/QuickMigrationGuideForGeant4V10
  G4GenericIon::GenericIonDefinition();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructBosons()
{
  // gamma
  G4Gamma::GammaDefinition();
}*/
