{
ifstream in;
in.open("Reflections");
TH1F *hist=new TH1F("hist","Reflection Profile",100,0,300);
Float_t dist;
while(!in.eof()){
  in>>dist;
  hist->Fill(dist);
}
in.close();
//hist->Fit("expo");
hist->Draw();
/*gStyle->SetOptFit(1111);
TF1 *fitfunc=hist->GetFunction("expo");
Double_t lambda=fitfunc->GetParameter(1);
Double_t delta=fitfunc->GetParError(1);
Double_t mu=1/lambda;
Double_t muerror=delta/(lambda*lambda);
char label[50];
sprintf(label,"Absorption Length= %d +/- %d",-mu,muerror);
cout<<label<<endl;
TPaveLabel *pave=new TPaveLabel(5000,2000,10000,4000,label);
pave->Draw("same");
*/
}
