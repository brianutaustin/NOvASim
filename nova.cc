#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "Randomize.hh"
#include "G4UIterminal.hh"
#include "novaSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
// Your choice of User Interface driver
#include "G4UIGAG.hh"
// Detector geometry generated by nova's GGE
#include "novaGeometry.hh"
// Physics List generated by nova's GPE
#include "novaPhysics.hh"
// nova's default PrimaryGeneratorAction
#include "novaPrimaryGeneratorAction.hh"
#include "novaRunAction.hh"
#include "novaEventAction.hh"
#include "novaTrackingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#ifdef G4VIS_USE
#include "novaVisManager.hh"
#endif
int main(int argc,char** argv)
{
 
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  
  // set mandatory initialization classes
  novaGeometry* OptGeom=new novaGeometry;
  runManager->SetUserInitialization(OptGeom);
  runManager->SetUserInitialization(new novaPhysics);
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new novaVisManager;
  visManager->Initialize();
#endif
  // set mandatory user action class
  runManager->SetUserAction(new novaPrimaryGeneratorAction);
  // set nova action classes 
  runManager->SetUserAction(new novaRunAction);
  runManager->SetUserAction(new novaEventAction);
  runManager->SetUserAction(new novaSteppingAction);
  runManager->SetUserAction(new novaTrackingAction);
  // Initialize G4 kernel
  runManager->Initialize();
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();
  //execute visualization macro
//  UI->ApplyCommand("/control/execute vrml.mac");
  //turn tracking verbose on 
  //  UI->ApplyCommand("/tracking/verbose 1");



  G4UIsession * session = new G4UIterminal();
  if(argc==1)
    //starts G4Terminal
    session->SessionStart();
  
  else if(argc==3){
    //automatically executes the specified number of runs and saves to output
    char writefile[40],startcommand[40];
    sprintf(startcommand,"/run/beamOn %s",argv[1]);
      UI->ApplyCommand(startcommand);
      sprintf(writefile,"/control/shell mv temp.root output/%s.root",argv[2]);
      UI->ApplyCommand(writefile);
      G4cout<<"Output saved to file output/"<<argv[2]<<".root"<<G4endl;
  }
  else if(argc==4){
    //loops over the specified parameter
    G4String type=argv[3];
    char writefile[40],startcommand[40],command[40],fname[40];
    if(type=="a"){
      //moves the center of the particle generator down the length of the strip
      for(int i=0;i<15;i++){
	sprintf(command,"/gps/pos/centre 0.0 5.0 %d cm",-350+50*i);
	UI->ApplyCommand(command);
	sprintf(startcommand,"/run/beamOn %s",argv[1]);
	UI->ApplyCommand(startcommand);
	sprintf(fname,"%s_%d",argv[2],-750+50*i);
	sprintf(writefile,"/control/shell mv temp.root output/%s.root",fname);
	UI->ApplyCommand(writefile);
	G4cout<<"Output saved to file output/"<<fname<<".root"<<G4endl;
      }
    }
/*    else if (type=="b"){
      //changes the RC of the Ti02
      G4LogicalVolume *StripMod=OptGeom->GetLWorld()->GetDaughter(0)->GetLogicalVolume();
      G4int NoDaughts=StripMod->GetNoDaughters();
      G4LogicalVolume *Paint=StripMod->GetDaughter(NoDaughts-1)->GetLogicalVolume();
      G4LogicalSkinSurface *PaintSurf=G4LogicalSkinSurface::GetSurface(Paint);
      G4OpticalSurface *op_paint=(G4OpticalSurface*)(PaintSurf->GetSurfaceProperty());
      G4MaterialPropertiesTable *paintMPT=op_paint->GetMaterialPropertiesTable();
      G4MaterialPropertyVector *Reflectivity=paintMPT->GetProperty("REFLECTIVITY");
      for(G4int i=0;i<4;i++){
	Reflectivity->RemoveElement(0);
	Reflectivity->RemoveElement(100);
	Reflectivity->AddElement(0,.89+.02*i);
	Reflectivity->AddElement(100,.89+.02*i);
	sprintf(startcommand,"/run/beamOn %s",argv[1]);
	UI->ApplyCommand(startcommand);
	sprintf(fname,"%s_%d",argv[2],89+2*i);
	sprintf(writefile,"/control/shell mv temp.root output/%s.root",fname);
	UI->ApplyCommand(writefile);
	G4cout<<"Output saved to file output/"<<fname<<".root"<<G4endl;
      }
    }
    else if (type=="c"){
      //changes the refractive index of the epoxy
      G4LogicalVolume *StripMod=OptGeom->GetLWorld()->GetDaughter(0)->GetLogicalVolume();
      
      G4LogicalVolume *Epoxy=StripMod->GetDaughter(0)->GetLogicalVolume();
      G4MaterialPropertiesTable *EProps=Epoxy->GetMaterial()->GetMaterialPropertiesTable();
      for(G4int i=0;i<4;i++){
	EProps->RemoveProperty("RINDEX");
	G4double E[2]={0,100},L[2]={1.45+i*.05,1.45+i*.05};
	EProps->AddProperty("RINDEX",E,L,2);
	sprintf(startcommand,"/run/beamOn %s",argv[1]);
	UI->ApplyCommand(startcommand);
	sprintf(fname,"%s_%d",argv[2],145+5*i);
	sprintf(writefile,"/control/shell mv temp.root output/%s.root",fname);
	UI->ApplyCommand(writefile);
	G4cout<<"Output saved to file output/"<<fname<<".root"<<G4endl;
      }
    }
    
    else if (type=="d"){
      //changes the concentration of the WLS dye
      G4LogicalVolume *StripMod=OptGeom->GetLWorld()->GetDaughter(0)->GetLogicalVolume();
      G4LogicalVolume *fiber=StripMod->GetDaughter(0)->GetLogicalVolume();
      G4MaterialPropertiesTable *CoreMPT=fiber->GetDaughter(0)->GetLogicalVolume()->GetMaterial()->GetMaterialPropertiesTable();
      //G4MaterialPropertiesTable *CladMPT=fiber->GetDaughter(1)->GetLogicalVolume()->GetMaterial()->GetMaterialPropertiesTable();
      for(G4int i=0;i<4;i++){
	CoreMPT->RemoveProperty("WLSABSLENGTH");
	//CladMPT->RemoveProperty("ABSLENGTH");
	G4double E[40],L[40],lambda,length;
	G4int NumEntries=0;
	ifstream Read;
	Read.open("WLSabsorb.cfg");
	while(!Read.eof()){
	  G4String filler;
	  Read>>lambda>>filler>>length;
	  E[NumEntries]=(1240/lambda)*eV;
	  L[NumEntries]=length*mm*200/(150+25*i);
	  NumEntries++;
	}
	Read.close();
	CoreMPT->AddProperty("WLSABSLENGTH",E,L,NumEntries);
	//CladMPT->AddProperty("ABSLENGTH",E,L,NumEntries);
	sprintf(startcommand,"/run/beamOn %s",argv[1]);
	UI->ApplyCommand(startcommand);
	sprintf(fname,"%s_%d",argv[2],150+25*i);
	sprintf(writefile,"/control/shell mv temp.root output/%s.root",fname);
	UI->ApplyCommand(writefile);
	G4cout<<"Output saved to file output/"<<fname<<".root"<<G4endl;
      }
      
    }*/
  }
  delete session;
  // job termination
  //#ifdef G4VIS_USE
  //delete visManager;
  //#endif
  delete runManager;
  return 0;
}

