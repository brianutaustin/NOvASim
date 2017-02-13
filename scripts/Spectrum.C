{
TString fname;
cout<<"Filename?"<<endl;
cin>>fname;
ifstream in;
in.open(fname);
Double_t x[1000],y[1000];
Double_t length;
Int_t count;
while(1){
  in>>x[count]>>",">>length;
  y[count]=1-exp(-1/length);
  cout<<y[count]<<endl;
  //x[count]=count;
  if(in.eof()) break;
  count++;
}
TGraph *graph=new TGraph(count+1,x,y);
graph->Draw("alp");
}
