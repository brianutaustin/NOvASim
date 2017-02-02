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
// Taken after
// $Id: novaRunAction.hh,v 1.1 2003/11/04 09:02:27 gcosmo Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
// 

#ifndef novaRunAction_h
#define novaRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TROOT.h"
#include "TTree.h"
#include "novaEventAction.hh"
#include "TFile.h"


class G4Run;

class novaRunAction : public G4UserRunAction
{
  public:
    novaRunAction();
   ~novaRunAction();

  public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  //adds the photon fates from an event to the run tree
  void UpdateStatistics(RunTally);
  //makes the individual histrograms for each fiber at the end of the run
  void MakeFiberHisto(TFile *f);
  //makes the individual histrograms for each PMT at the end of the run
  void MakePMTHisto(TFile *f);
  //sets the number of PMTs for MakePMTHisto, used by novaGeometry
  void SetNumPMTs(G4int PMTcount){NumPMTs=PMTcount;}
  //sets the number of fibers for MakeFiberHisto, used by opticalGeometry
  void SetNumFibers(G4int numfibers){NumFibers=numfibers;}
  private:
  G4int NumPMTs;
  G4int NumFibers;
  RunTally Stats;
  TTree *theRunTree;
  TBranch *RunBranch;
  time_t start;
};

#endif





