{
TString filename;
cout<<"Filename?"<<endl;
cin>>filename;
Double_t x[100],y[100];
Int_t entries=0;
ifstream in;
in.open(filename);
Double_t pathlength;
while (!in.eof()){
  in>>x[entries]>>",">>pathlength;
  y[entries]=1-exp(-1/pathlength);
  //cout<<x[entries]<<" "<<y[entries]<<endl;
  entries++;
}
in.close();
TGraph *gr=new TGraph(entries,x,y);
gr->SetTitle(filename);
cout<<"Second file?"<<endl;
cin>>filename;
ifstream in2;
in2.open(filename);
entries=0;
while (!in2.eof()){
  in2>>x[entries]>>",">>pathlength;
  y[entries]=1-exp(-1/pathlength);
  //cout<<x[entries]<<" "<<y[entries]<<endl;
  entries++;
}
in2.close();
TGraph *gr2=new TGraph(entries,x,y);
gr2->SetTitle(filename);

gr->Draw("alp");
gr2->Draw("lp");

}
