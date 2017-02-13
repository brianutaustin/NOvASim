{
ifstream in;
in.open("escapes");
Double_t x,y,z;
//TH3F *hist=new TH3F("hist","Escape Positions",42,-21,21,11,-5.5,5.5,500,-500,500);
TH3F *hist=new TH3F("hist","Escape Positions",100,-100,100,100,-100,100,100,-100,100);
//TH2F *hist=new TH2F("hist","Escape Positions",42,-21,-21,11,-5.5,5.5);
while(!in.eof()){
  in>>x>>y>>z;
  hist->Fill(x,y,z);
}
hist->Draw();
}
  
