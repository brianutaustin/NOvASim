#include "novaPrimaryGeneratorAction.hh"
#include "novaEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4EventManager.hh"
#include "novaEventAction.hh"


//The commented out commands are to use this module with a G4ParticleGun 
G4UImanager* UI = G4UImanager::GetUIpointer();
novaPrimaryGeneratorAction::novaPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  //  UI->ApplyCommand("/control/execute setgun.mac");
    particleSource=new G4GeneralParticleSource();
    UI->ApplyCommand("/control/execute setsource.mac");
}

novaPrimaryGeneratorAction::~novaPrimaryGeneratorAction()
{
  // delete particleGun;
    delete particleSource;
}

void novaPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleSource->GeneratePrimaryVertex(anEvent);
  G4EventManager *evtman=G4EventManager::GetEventManager();
  novaEventAction *evtac=(novaEventAction*)evtman->GetUserEventAction();
  evtac->SetIncidentE(particleSource->GetParticleEnergy());
  //particleGun->GeneratePrimaryVertex(anEvent);
}


