{
TString fname;
cout<<"Filename?"<<endl;
cin>>fname;
ifstream in;
in.open(fname);
Double_t x[1000],y[1000];
Int_t count;
while(1){
  in>>x[count]>>",">>y[count];
  cout<<y[count]<<endl;
  //x[count]=count;
  if(in.eof()) break;
  count++;
}
TGraph *graph=new TGraph(count+1,x,y);
graph->Draw("alp");
}
