{
ifstream in;
in.open("BulkAbs");
TH1F *hist=new TH1F("hist","Bulk Absorption Profile",30,0,2250*5);
Float_t dist;
while(!in.eof()){
  in>>dist;
  hist->Fill(dist);
}
in.close();
hist->Fit("expo");
gStyle->SetOptFit(1111);
TF1 *fitfunc=hist->GetFunction("expo");
Double_t lambda=fitfunc->GetParameter(1);
Double_t delta=fitfunc->GetParError(1);
Double_t mu=1/lambda;
Double_t muerror=delta/(lambda*lambda);
char label[50];
sprintf(label,"Absorption Length= %d +/- %d",-mu,muerror);
cout<<label<<endl;
TPaveLabel *pave=new TPaveLabel(6000,2000,11000,2400,label);
pave->Draw("same");
}
