
struct Tally {
  float BirthX;
  float BirthY;
  float BirthZ;
  float BirthLambda;
  float DistBefore;
  int   ReflectBefore;
  float WLSZ;
  float WLSLambda;
  float DistAfter;
  int   WLSCount;
  int   PMTnumber;
  float   ExitAngle;
};

void CompileStats(TString fname)
{
  TFile f(fname,"UPDATE");
  Int_t numevents=f.GetNkeys()-1;
  Int_t MaxExitAngle=0,MaxDist=0,MaxDistZ=0;
  Int_t MaxDetSpec=0,MaxScintSpec=0;
  Int_t MaxRefls=0,MaxWLSCount=0;
  for(int i=0;i<numevents;i++){
    Tally OnePhoton;
    char EventName[12];
    sprintf(EventName,"Event%d",i);
    TTree *theTree=(TTree*)f.Get(EventName);
    if(!theTree) break;
    //theTree->SetBranchAddress(EventName,&OnePhoton);
    if(abs(theTree->GetMaximum("ExitAngle")*180.0/3.14)>MaxExitAngle)
      MaxExitAngle=abs(theTree->GetMaximum("ExitAngle")*180.0/3.14);
    if(theTree->GetMaximum("DistBefore")>MaxDist)
      MaxDist=theTree->GetMaximum("DistBefore");
    if(abs(theTree->GetMaximum("BirthZ")-theTree->GetMaximum("WLSZ"))>MaxDistZ)
      MaxDistZ=abs(theTree->GetMaximum("BirthZ")-theTree->GetMaximum("WLSZ"));
    if(theTree->GetMaximum("WLSLambda")>MaxDetSpec)
      MaxDetSpec=theTree->GetMaximum("WLSLambda");
    if(theTree->GetMaximum("BirthLambda")>MaxScintSpec)
      MaxScintSpec=theTree->GetMaximum("BirthLambda");
    if(theTree->GetMaximum("ReflectBefore")>MaxRefls)
      MaxRefls=theTree->GetMaximum("ReflectBefore");
    if(theTree->GetMaximum("WLSCount")>MaxWLSCount)
      MaxWLSCount=theTree->GetMaximum("WLSCount");
  }
  TH1F *hExitAngle=new TH1F("hExitAngle","Exit Angle of Detected Photons",MaxExitAngle,0,MaxExitAngle);
  TH1F *hDist=new TH1F("hDist","Distance Traveled Through Scintillator",MaxDist,0,MaxDist);
  TH1F *hDistZ=new TH1F("hDistZ","Distance Traveled Along Z Axis",MaxDistZ,0,MaxDistZ);
  TH1F *hDetSpec=new TH1F("hDetSpec","Spectrum of Detected Photons",MaxDetSpec,0,MaxDetSpec);
  TH1F *hScintSpec=new TH1F("hScintSpec","Scintillation Spectrum of Detected Photons",MaxScintSpec,0,MaxScintSpec);
  TH1F *hRefls=new TH1F("hRefls","Number of Reflections Before WLS",MaxRefls,0,MaxRefls);
  TH1F *hWLSCount=new TH1F("hWLSCount","Number of WL Shifts",MaxWLSCount,0,MaxWLSCount);
  
  for(int i=0;i<numevents;i++){
    Tally OnePhoton;
    char EventName[12];
    sprintf(EventName,"Event%d",i);
    TTree *theTree=(TTree*)f.Get(EventName);
    if(!theTree) break;
    theTree->SetBranchAddress(EventName,&OnePhoton);
    Int_t numphotons=(Int_t)theTree->GetEntries();
    for (Int_t j=0;j<numphotons;j++){
      theTree->GetEntry(j);
      hExitAngle->Fill(abs(OnePhoton.ExitAngle*180.0/3.14));
      //cout<<OnePhoton.ExitAngle<<endl;
      hDist->Fill(OnePhoton.DistBefore);
      hDistZ->Fill(abs(OnePhoton.BirthZ-OnePhoton.WLSZ));
      hDetSpec->Fill(OnePhoton.WLSLambda);
      hScintSpec->Fill(OnePhoton.BirthLambda);
      hRefls->Fill(OnePhoton.ReflectBefore);
      hWLSCount->Fill(OnePhoton.WLSCount);
    }
  }
  hExitAngle->Write();
  hDist->Write();
  hDistZ->Write();
  hDetSpec->Write();
  hScintSpec->Write();
  hRefls->Write();
  hWLSCount->Write();
  
}
