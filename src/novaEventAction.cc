#include "G4SteppingManager.hh"
#include "novaSteppingAction.hh"
#include "G4Run.hh"
#include "novaEventAction.hh"
#include "G4RunManager.hh"
#include "novaRunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "fstream"
#include "iostream"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"
 
novaEventAction::novaEventAction()
{}
 
novaEventAction::~novaEventAction()
{}

 
void novaEventAction::BeginOfEventAction(const G4Event* evt)
{
  //initialize all counters  
  
  Statistics.Deposit=0;
  Statistics.Generated=0;
  Statistics.WLS=0;
  Statistics.Absorbed=0;
  Statistics.BulkAbs=0;
  Statistics.Escaped=0;
  Statistics.Failed=0;
  Statistics.Detected=0;
  //create the ROOT tree to store the data
  char branchname[20];
  sprintf(branchname,"Event%d",evt->GetEventID());
  EventTree=new TTree(branchname,branchname);
  EventBranch=EventTree->Branch(branchname,&BranchFiller.BirthX,"BirthX/F:BirthY/F:BirthZ/F:BirthLambda:DeathLambda/F:DistBefore:DistAfter:ReflectBeforeWLS/I:ReflectAfterWLS:TotalInternalReflections/I:WLSZ/F:WLSCount/I:PMTnumber:ExitAngle/F:TotTrackLen/F:DeathX/F:DeathY/F:DeathZ/F:DeathR/F");
}

 
void novaEventAction::EndOfEventAction(const G4Event* evt)
{
  
  //  G4int event_id = evt->GetEventID();
  // get number of stored trajectories and calculate fates statistics
  //
  G4double WLSfrac=0,Absfrac=0,Bulkfrac=0,Escfrac=0,Failfrac=0;
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  G4double efficiency=0;
  if (Statistics.Generated!=0){
    efficiency=(1.0*Statistics.Detected)/(1.0*Statistics.Generated);
    Absfrac=(1.0*Statistics.Absorbed)/(1.0*Statistics.Generated);
    Bulkfrac=(1.0*Statistics.BulkAbs)/(1.0*Statistics.Generated);
    Escfrac=(1.0*Statistics.Escaped)/(1.0*Statistics.Generated);
    Failfrac=(1.0*Statistics.Failed)/(1.0*Statistics.Generated);
    WLSfrac=(1.0*Statistics.WLS)/(1.0*Statistics.Generated);
    
  }
  
  // output fates
  G4cout <<"Run "<<G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()<< " >>> Event " << evt->GetEventID() << G4endl;
  G4cout<<"Incident Energy:    "<<Statistics.IncidentE/MeV<<G4endl;
  G4cout<<"Energy Deposited:   "<<Statistics.Deposit/MeV<<G4endl;
  G4cout<<"Photons Generated:  "<<Statistics.Generated<<G4endl;
  G4cout<<"Photons WL Shifted: "<<Statistics.WLS<<" "<<WLSfrac<<G4endl;
  G4cout<<"Photons Absorbed:   "<<Statistics.Absorbed<<" "<<Absfrac<<G4endl;
  G4cout<<"Bulk Absorbed:      "<<Statistics.BulkAbs<<" "<<Bulkfrac<<G4endl;
  G4cout<<"Photons Escaped:    "<<Statistics.Escaped<<" "<<Escfrac<<G4endl;
  G4cout<<"PMT Misses:         "<<Statistics.Failed<<" "<<Failfrac<<G4endl;
  G4cout<<"Photons Collected:  "<<Statistics.Detected<<" "<<efficiency<<G4endl;
 
  // extract the trajectories and draw them
  //
  
  if (G4VVisManager::GetConcreteInstance())
    {
     for (G4int i=0; i<n_trajectories; i++) 
       { 
	 G4Trajectory* trj = (G4Trajectory*)
	   ((*(evt->GetTrajectoryContainer()))[i]);
	  if (trj->GetParticleName()="opticalphoton")	  
	    //	 if (trj->GetParticleName()!="opticalphoton")	  

	   trj->DrawTrajectory();
	 
       }
    }
  //Write the event statistics to the temp root file
  TFile f("temp.root","update");
  EventTree->Write();
  //Send the fates to the run tree 
  novaRunAction *runac=(novaRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  runac->UpdateStatistics(Statistics);
}
  

void novaEventAction::CountDetected(Tally PhotonTally)
{ 
  //every detected photon is one count on the event root tree
  Statistics.Detected++;
  BranchFiller=PhotonTally;
  EventTree->Fill();
}

