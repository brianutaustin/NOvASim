/// novaPhysics.cc
/// Author: Benton Pahlka <pahlka@physics.utexas.edu>
/// Copyright: 2008 (C) NEMO3/SuperNEMO - University of Texas at Austin

#include "novaPhysics.hh"
//#include "G4MuNuclearInteraction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ios.hh"
#include "G4OpWLS.hh"
#include "G4SystemOfUnits.hh"

novaPhysics::novaPhysics():  G4VUserPhysicsList()
{
// Here used the default cut value you have typed in

  defaultCutValue = .1*mm;

  SetVerboseLevel(1);
}

novaPhysics::~novaPhysics(){}

void novaPhysics::ConstructParticle()
{
// Here are constructed all particles you have chosen

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructAllShortLiveds();
}

// In this method, static member functions should be called for ALL particles to be used.

void novaPhysics::ConstructBosons()
{
  G4Geantino::GeantinoDefinition();
  G4Gamma::GammaDefinition();
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void novaPhysics::ConstructLeptons()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();
}

void novaPhysics::ConstructMesons(){}
void novaPhysics::ConstructBaryons(){

  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();

  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

}
void novaPhysics::ConstructAllShortLiveds(){}
void novaPhysics::ConstructProcess()
{
// Transportation, electromagnetic and general processes

  AddTransportation();
  ConstructEM();
  ConstructGeneral();
  ConstructOp();
}
// Here are respective header files for chosen processes

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4SynchrotronRadiation.hh"
#include "G4ComptonScattering.hh"
//#include "G4PolarizedComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
//#include "G4MultipleScattering.hh"
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
//#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
// The next two processes are always included.

#include "G4hIonisation.hh"
//#include "G4MultipleScattering.hh"
void novaPhysics::ConstructEM()
{
  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

// The next processes are always added.

    if ((!particle->IsShortLived()) &&
	(particle->GetPDGCharge() != 0.0 )&&
	(particle->GetParticleName() != "e-") &&
	(particle->GetParticleName() != "e+") &&
	(particle->GetParticleName() != "mu-") &&
	(particle->GetParticleName() != "mu+") &&
	(particle->GetParticleName() != "chargedgeantino")){
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
    }
// Each if clause corresponds to a row in the PhysicsTable
    /*original values:
      G4MuIonisation(),ordInActive,2,2
      G4MuBremsstrahling(),ordInActive,orInActive,3
      G4MuPairProduction(),ordInActive,ordInActive,4
    */


    if (particleName == "mu-") {
      pmanager->AddProcess(new G4MuIonisation(),-1,1,1);
    }

    if (particleName == "mu+") {
      pmanager->AddProcess(new G4MuIonisation(),-1,1,1);
    }

    if (particleName == "mu-") {
      pmanager->AddProcess(new G4MuPairProduction(),-1,2,2);
      //      pmanager->AddProcess(new G4MuNuclearInteraction("muNucl"),-1,4,4);
    }

    if (particleName == "mu+") {
      pmanager->AddProcess(new G4MuPairProduction(),-1,2,2);
      //      pmanager->AddProcess(new G4MuNuclearInteraction("muNucl"),-1,4,4);
    }

    /*
    if (particleName == "mu-") {
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,3,3);
    }
    if (particleName == "mu+") {
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,3,3);
    }
    */

    if (particleName == "e-") {
      pmanager->AddProcess(new G4eIonisation(),ordInActive,2,2);
    }
    if (particleName == "e-") {
      pmanager->AddProcess(new G4eBremsstrahlung(),ordInActive,ordInActive,3);
    }
    if (particleName == "e-") {
      pmanager->AddProcess(new G4eplusAnnihilation(),ordDefault,ordInActive,4);
    }
    if (particleName == "e-") {
      pmanager->AddProcess(new G4SynchrotronRadiation(),ordInActive,ordInActive,ordDefault);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(new G4eIonisation(),ordInActive,2,2);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(new G4eBremsstrahlung(),ordInActive,ordInActive,3);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(new G4eplusAnnihilation(),ordDefault,ordInActive,4);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(new G4SynchrotronRadiation(),ordInActive,ordInActive,ordDefault);
    }
    if (particleName == "gamma") {
      pmanager->AddProcess(new G4ComptonScattering(),ordInActive,ordInActive,ordDefault);
    }
    if (particleName == "gamma") {
      pmanager->AddProcess(new G4GammaConversion(),ordInActive,ordInActive,ordDefault);
    }
    if (particleName == "gamma") {
      pmanager->AddProcess(new G4PhotoElectricEffect(),ordInActive,ordInActive,ordDefault);
    }
    G4eMultipleScattering *MSC=new G4eMultipleScattering("msc");
    //    MSC->SetFacrange(.005);

    if (particleName == "e-") {
      pmanager->AddProcess(MSC,ordInActive,1,1);
    }
    if (particleName == "e+") {
      pmanager->AddProcess(MSC,ordInActive,1,1);
    }


    if (particleName == "mu-") {
      pmanager->AddProcess(MSC,ordInActive,1,1);
    }
    if (particleName == "mu+") {
      pmanager->AddProcess(MSC,ordInActive,1,1);
    }


  }
}

void novaPhysics::ConstructOp()
{

  G4Scintillation *theScintillationProcess = new G4Scintillation("Scintillation");
  G4OpAbsorption *theAbsorptionProcess     = new G4OpAbsorption();
  G4OpRayleigh *theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess *theBoundaryProcess  = new G4OpBoundaryProcess();
  G4OpWLS *theWLSProcess = new G4OpWLS();

  G4int verbosity = 0;

  theScintillationProcess->SetVerboseLevel(verbosity);

  theAbsorptionProcess->SetVerboseLevel(verbosity);
  theRayleighScatteringProcess->SetVerboseLevel(verbosity);
  theBoundaryProcess->SetVerboseLevel(verbosity);
  theWLSProcess->SetVerboseLevel(0);

  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);

  G4OpticalSurfaceModel themodel = unified;
//  theBoundaryProcess->SetModel(themodel);

  G4ProcessManager * pManager = 0;
  pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  pManager->AddDiscreteProcess(theAbsorptionProcess);
  pManager->AddDiscreteProcess(theRayleighScatteringProcess);
  pManager->AddDiscreteProcess(theBoundaryProcess);
  pManager->AddDiscreteProcess(theWLSProcess);

  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }
  }
}

#include "G4Decay.hh"

void novaPhysics::ConstructGeneral()
{
  //Here you should specify muon decay if you want to simulate that
}

void novaPhysics::SetCuts()
{
// defaultCutValue you have typed in is used

  if (verboseLevel >1){
    G4cout << "opticalPhysics::SetCuts:";
  }
  SetCutsWithDefault();
  //  SetCutValue(5*mm,"opticalphoton");
}
