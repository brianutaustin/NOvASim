{
ifstream in;
in.open("ExitPosition.out");
Double_t x,y,z;
TH1D *RadDist=new TH1D("RadDist","Radial Distripbution of Exit Position",120,0,0.6);
TH2D *hist=new TH2D("hist","Photon Exit Position from Fiber",120,-.6,.6,120,-.6,.6);
while(1){
  in>>x>>y>>z;
  if(in.eof()) break;
  //  y-=3.6;
  hist->Fill(x,y);
  RadDist->Fill(sqrt(x*x+y*y));
}
TCanvas *c1=new TCanvas(1);
c1->Divide(2,1);
c1->cd(1);
hist->Draw();
c1->cd(2);
RadDist->Draw();
}
