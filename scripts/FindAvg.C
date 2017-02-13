{
ifstream in;
in.open("pigtails.txt");
TString dummy[20];
Double_t total,avg=0,avg2=0;
Int_t count=0;
while(!in.eof()){
  in>>dummy;
  if(dummy=="total") {
    in>>total;
    if(total>=8){
      total=total/2-4;
      count++;
      avg+=total;
      avg2+=total*total;
    }
    cout<<"Total="<<total<<endl;
  }
}
avg=avg/count;
avg2=avg2/count;
cout<<endl<<"Average=  "<<avg<<endl;
cout      <<"Variance= "<<avg2-(avg*avg)<<endl;
cout      <<"Std Dev=  "<<sqrt(avg2-(avg*avg))<<endl;
}
