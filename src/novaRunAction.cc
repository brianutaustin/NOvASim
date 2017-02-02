//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// Taken from
// $Id: novaRunAction.cc,v 1.1 2003/11/04 09:02:27 gcosmo Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
// 


#include "novaRunAction.hh"
#include "Randomize.hh"
#include <time.h>
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH2I.h"
#include "TH1D.h"
#include "TAxis.h"
#include "fstream"
#include "iostream" 

novaRunAction::novaRunAction()
{}


novaRunAction::~novaRunAction()
{}


void novaRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //start the timer clock to calculate run times
  start=time(NULL);
  //create the ROOT tree for Run Statistics
  theRunTree=new TTree("theRunTree","Tallies of Photon Data");
  RunBranch=theRunTree->Branch("RunStatistics",&Stats.IncidentE,"IncidentE/F:Deposit/F:Generated/I:WLS:Absorbed:BulkAbs:Escaped:Failed:Detected");
  //set the random seed to the CPU clock
  G4long seed=time(0);
  CLHEP::HepRandom::setTheSeed(seed);
  
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer();
      UI->ApplyCommand("/vis/scene/notifyHandlers");
    } 
}


void novaRunAction::EndOfRunAction(const G4Run*aRun)
{
  //update the temp root file
  TFile *f=new TFile("temp.root","update");
  theRunTree->Write();
  //  MakePMTHisto(f);
  //  MakeFiberHisto(f);
  f->Close();

  if (G4VVisManager::GetConcreteInstance())
    {
     G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }
  
  //display run time and write to file Rntime.out
  time_t end=time(NULL);
  G4int elapsed=end-start;
  G4cout<<"Run Completed in "<<elapsed/3600<<":"<<(elapsed%3600)/60<<":"<<((elapsed%3600)%60)<<G4endl;
  ofstream timeout;
  timeout.open("Runtime.out",std::ios::app);
  timeout<<"Run "<<aRun->GetRunID()<<": "<<elapsed/3600<<":"<<(elapsed%3600)/60<<":"<<((elapsed%3600)%60)<<G4endl;
  timeout.close();
  
}

void novaRunAction::UpdateStatistics(RunTally aRunTally)
{
  Stats=aRunTally;
  theRunTree->Fill();
}

void novaRunAction::MakePMTHisto(TFile *f){
  
  G4int numkeys=f->GetNkeys();
  TTree *theTree=(TTree*)f->Get("theRunTree");
  G4int max=(G4int)theTree->GetMaximum("Detected");
  if(max<1) return;
  TH2I *histo=new TH2I("AllPMTsLego","Hits per PMT per Event",max,0,max,NumPMTs,0,NumPMTs);
  
  for (G4int i=0;i<=numkeys;i++){
    Tally OnePhoton;
    char EventName[12];
    sprintf(EventName,"Event%d",i);
    theTree=(TTree*)f->Get(EventName);
    if(!theTree) break;
    
    theTree->SetBranchAddress(EventName,&OnePhoton);
    G4int Sum[20];
    for (G4int j=0;j<NumPMTs;j++){
      Sum[j]=0;
    }
    G4int numphotons=(G4int)theTree->GetEntries();
    for(G4int j=0;j<numphotons;j++){
      theTree->GetEntry(j);
      Sum[OnePhoton.PMTnumber]++;
    }
    
    for(G4int j=0;j<NumPMTs;j++){
      histo->Fill(Sum[j],j);
      Sum[j]=0;
    }
  }
  for (G4int i=0;i<NumPMTs;i++){
    char histname[20];
    sprintf(histname,"PMT%d",i);
    TH1D *proj=histo->ProjectionX(histname,i+1,i+1);
    proj->SetTitle("Photoelectrons per Event");
    proj->Write();
  }
  histo->SetFillColor(kGreen);
  histo->SetOption("Lego1");
  TAxis*yax=histo->GetYaxis();
  yax->SetNdivisions(NumPMTs,kTRUE);
  histo->SetBarWidth(.15);
  histo->Write();
  
}

//************************************
//  Make the histograms for the fibers
//************************************

void novaRunAction::MakeFiberHisto(TFile *f){
  
  // Make the 2D Histogram of hits per fiber

  G4int numkeys=f->GetNkeys();
  TTree *theTree=(TTree*)f->Get("theRunTree");
  G4int max=(G4int)theTree->GetMaximum("Detected");
  if(max<1) return;
  TH2I *fiberhisto=new TH2I("AllFibersLego","Hits per Fiber per Event",max,0,max,NumPMTs/2,0,NumPMTs/2);
  
  for (G4int i=0;i<=numkeys;i++){
    Tally OnePhoton;
    char EventName[12];
    sprintf(EventName,"Event%d",i);
    theTree=(TTree*)f->Get(EventName);
    if(!theTree) break;
    
    theTree->SetBranchAddress(EventName,&OnePhoton);
    G4int Sum[20];
    for (G4int j=0;j<NumPMTs;j++){
      Sum[j]=0;
    }
    G4int numphotons=(G4int)theTree->GetEntries();
    for(G4int j=0;j<numphotons;j++){
      theTree->GetEntry(j);
      Sum[OnePhoton.PMTnumber]++;

    }

    G4int FibSum[20];
    //    G4cout << "NumFibers = " << NumFibers << G4endl;
    /*    
    for (G4int k=0; k<NumPMTs/2; k++){
      G4cout << "k: " << k << G4endl;
      FibSum[k] = Sum[2*k] + Sum[(2*k)+1];
      FibSum[k] = Sum[2*k] + Sum[(2*k)+1] + Sum[(2*k)+2] + Sum[(2*k)+3] ;
    }
    */
    FibSum[0] = Sum[1] + Sum[2];
    FibSum[1] = Sum[11] + Sum[12];
    FibSum[2] = Sum[3] + Sum[4];
    FibSum[3] = Sum[13] + Sum[14];
    FibSum[4] = Sum[5] + Sum[6];
    FibSum[5] = Sum[15] + Sum[16];
    FibSum[6] = Sum[7] + Sum[8];
    FibSum[7] = Sum[17] + Sum[18];
    FibSum[8] = Sum[9] + Sum[10];
    FibSum[9] = Sum[19] + Sum[20];


    for(G4int j=0;j<NumPMTs/2;j++){
      fiberhisto->Fill(FibSum[j],j);
      FibSum[j]=0;
    }
  }

  for (G4int i=0;i<NumPMTs/2;i++){
    char histname[20];
    sprintf(histname,"fiber%d",i);
    TH1D *proj=fiberhisto->ProjectionX(histname,i+1,i+1);
    proj->SetTitle("Photoelectrons per Event");
    proj->Write();
  }

  fiberhisto->SetFillColor(kGreen);
  fiberhisto->SetOption("Lego1");
  TAxis*yax=fiberhisto->GetYaxis();
  yax->SetNdivisions(NumPMTs/2,kTRUE);
  fiberhisto->SetBarWidth(.15);
  fiberhisto->Write();
  
}
