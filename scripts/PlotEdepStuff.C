{
ifstream fin;
fin.open("edepstuff");
Double_t Ein, Edep,Depth;
Int_t Gen;
Double_t Incident[40],Deposited[40],Lost[40],Penetration[40];
Double_t SigmaD[40],SigmaL[40],SigmaP[40];
Double_t SumD=0,SumL=0,SumP=0,Sum2D=0,Sum2L=0,Sum2P=0;
Int_t numpoints=0;
fin>>Ein>>Edep>>Gen>>Depth;

Int_t N=0;
do{
  
  N++;
  Incident[numpoints]=Ein;
  SumD+=Edep;
  SumL+=Ein-Edep;
  SumP+=Depth;
  Sum2D+=Edep*Edep;
  Sum2L+=(Ein-Edep)*(Ein-Edep);
  Sum2P+=Depth*Depth;
  fin>>Ein>>Edep>>Gen>>Depth;
  if(Incident[numpoints]!=Ein||fin.eof()){
    Deposited[numpoints]=SumD/N;
    Lost[numpoints]=SumL/N;
    Penetration[numpoints]=SumP/N;
    SigmaD[numpoints]=Deposited[numpoints]/sqrt(N);//sqrt(Sum2D/N-(SumD/N*SumD/N));
    SigmaL[numpoints]=Lost[numpoints]/sqrt(N);//sqrt(Sum2L/N-(SumL/N*SumL/N));
    SigmaP[numpoints]=Penetration[numpoints]/sqrt(N);//sqrt(Sum2P/N-(SumP/N*SumP/N));
    cout<<Incident[numpoints]<<" "<<Deposited[numpoints]<<" "<<SigmaD[numpoints]<<" "<<Lost[numpoints]<<" "<<SigmaL[numpoints]<<" "<<Penetration[numpoints]<<" "<<SigmaP[numpoints]<<endl;
    numpoints++;
    N=0;
    SumD=0;
    SumL=0;
    SumP=0;
    Sum2D=0;
    Sum2L=0;
    Sum2P=0;
  }
  //cout<<numpoints<<" "<<N<<endl;
}while(!fin.eof());
fin.close();
Double_t ex[40];
for (int i=0;i<40;i++)
  ex[i]=0;

TCanvas *c1=new TCanvas(1);
c1->Divide(2,2);
c1->cd(1);
TGraphErrors *gd=new TGraphErrors(numpoints,Incident,Deposited,ex,SigmaD);
gd->SetTitle("Energy Deposited (MeV)");
gd->Draw("alp");
c1->cd(2);
TGraphErrors *gl=new TGraphErrors(numpoints,Incident,Lost,ex,SigmaL);
gl->SetTitle("Energy Lost (MeV)");
gl->Draw("alp");
c1->cd(3);
TGraphErrors *gp=new TGraphErrors(numpoints,Incident,Penetration,ex,SigmaP);
gp->SetTitle("Penetration Depth (mm)");
gp->Draw("alp");
c1->cd(0);
}


    
