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
// $Id: novaEventAction.hh,v 1.1 2003/11/04 09:02:26 gcosmo Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
 
#ifndef novaEventAction_h
#define novaEventAction_h 1
#include "G4ParticleGun.hh"
#include "G4UserEventAction.hh"
#include "TH1F.h"
#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TCanvas.h"
class G4Event;
//a Tally is attached to each photon to store relevant statistics
struct Tally {
  float BirthX;
  float BirthY;
  float BirthZ;
  float BirthLambda;
  float DeathLambda;
  float DistBefore;//Distance traveled before WLShifting
  float DistAfter;
  int   ReflectBeforeWLS;//Number of Reflections before WL shifting
  int   ReflectAfterWLS;//Number of Reflections before WL shifting
  int TotalInternalReflections;

  float WLSZ;
  //  float WLSLambda;
  int   WLSCount;//number of WL shifts per photon
  int   PMTnumber;
  float ExitAngle;

  float TotTrackLen;
  float DeathX;
  float DeathY;
  float DeathZ;
  float DeathR;

  inline int operator ==(const Tally& right) const
    {return (this==&right);}
};
//This struct carries statistics for the whole Run
struct RunTally {
  float  IncidentE;
  float  Deposit;
  int    Generated;
  int    WLS;
  int    Absorbed;
  int    BulkAbs;
  int    Escaped;
  int    Failed;
  int    Detected;
  inline int operator ==(const RunTally& right) const
    {return (this==&right);}
};

class novaEventAction : public G4UserEventAction
{
  public:
    novaEventAction();
   ~novaEventAction();
  
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  //Keeps track of the total number of generated photons
  void AddGenerated(G4int numgenerated){Statistics.Generated+=numgenerated;}
  //Keeps track of the total energy deposited in the scintillator
  void AddEdep(G4double edep){Statistics.Deposit+=edep;}
  
  void CountDetected(Tally);
  void CountWLS(){Statistics.WLS++;}
  void CountAbsorbed(){Statistics.Absorbed++;}
  void CountBulkAbs(){Statistics.BulkAbs++;}
  void CountEscaped(){Statistics.Escaped++;}
  void CountFailed(){Statistics.Failed++;}
    
  //Keeps track of the incident energy of the primary particle
  void SetIncidentE(G4double ince){Statistics.IncidentE=ince;}
  
private:
  
  TTree *EventTree;
  TBranch *EventBranch;
  Tally BranchFiller;
  RunTally Statistics;
  
  
};


#endif

    
