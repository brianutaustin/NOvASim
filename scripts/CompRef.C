{
ifstream in[4];
TH1F *hist[4];
TCanvas *c1=new TCanvas(1);
c1->Divide(2,2);
for(Int_t i=0;i<4;i++){
  char fname[20];
  sprintf(fname,"Ref%d",i+1);
  in[i].open(fname);
  hist[i]=new TH1F("hist","Reflection Profile",100,0,200);
  Float_t dist;
  while(!in[i].eof()){
    in[i]>>dist;
    hist[i]->Fill(dist);
  }
  c1->cd(i+1);
  hist[i]->Draw();
  in[i].close();
}
c1->cd(0);

}
