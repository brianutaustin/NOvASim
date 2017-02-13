{

gROOT->Reset();
gStyle->SetOptFit();
#include <fstream.h>
float x[600], y[600];

   ifstream in;

Int_t n;

TString file;
TString label1="Detector: ";
Int_t nlines = 0;

TFile *f = new TFile("fromfile.root","RECREATE");
TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x");
TString title;
TString top;
TString start="Histogram ";
TString filler;

  cout<<endl<<"Filename: "<<endl;
  cin>>file;
  in.open(file);
  top=start;
  title=label1+file;

int index=0;

  total  = new TH1F("total","This is the total distribution",100,-4,4);

c1 = new TCanvas("c1","Title", 200, 10, 600, 600);

while(!in.eof()){
  in >> x[index] >> filler >> y[index]; 
  //  cout << x[index] << endl;
  index++;
}
in.close();
graph=new TGraph(index, x, y);

  for (int i=0; i<index; i++){
     total->Fill(y[i]);
     //    cout << y[i] << ", ";
  }

   graph->Draw("AP");
   c1->Update();
   graph->GetHistogram()->SetXTitle("Wavelength (nm)");
   graph->GetHistogram()->SetYTitle("Counts");

   /* fill options */
   graph->SetFillColor(0);
   graph->SetFillStyle(1);

   /* line options */
   graph->SetLineColor(4);
   graph->SetLineWidth(1);
   graph->SetLineStyle(1);

   /* marker options */
   graph->SetMarkerColor(2);
   graph->SetMarkerStyle(6);
   graph->SetMarkerSize(1);



//   graph->Fit("landau","q");

   c1->Update();
//   TPaveStats *stats1 = (TPaveStats*)gr1->GetListOfFunctions()->FindObject("stats");
//   stats1->SetTextColor(kBlue); 
//   stats1->SetX1NDC(0.0); stats1->SetX2NDC(0.5); stats1->SetY1NDC(0.75);
   c1->Modified();

}
