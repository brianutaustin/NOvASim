#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Event.hh"
#include "novaTrackingAction.hh"
#include "G4VProcess.hh"
#include "G4TrackVector.hh"
#include "novaTrackInformation.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"

novaTrackingAction::novaTrackingAction()
{}
novaTrackingAction::~novaTrackingAction()
{}
void novaTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //attach a new TrackInformation to photons at scintillation
  G4String partname=aTrack->GetDefinition()->GetParticleName();
  if(partname=="opticalphoton"&&aTrack->GetUserInformation()==0){
    //    if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation"){
      G4Track* theTrack=(G4Track*)aTrack;
      theTrack->SetUserInformation(new novaTrackInformation(aTrack));
      //    }
  }
}
void novaTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  //attach a new TrackInformation to WLS photons, add parent info
   G4String partname=aTrack->GetDefinition()->GetParticleName();
   if(partname=="opticalphoton"){
     G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
     if(secondaries){
       size_t nSeco = secondaries->size();
       if(nSeco>0){
	 for(size_t i=0;i<nSeco;i++){
	   G4String cproc=(*secondaries)[i]->GetCreatorProcess()
	                   ->GetProcessName();
	   if(cproc=="OpWLS") {	     
	     (*secondaries)[i]->SetUserInformation(new novaTrackInformation(aTrack));
	   }
	 }
       }
     }
   }
   
}
