int main() {
  TString rootfile = "radialAlignment3d.root";
  if( gSystem->AccessPathName(rootfile) ) { // returns false if true
   std::cout << std::endl << "File: " << rootfile << " not there!!!" << std::endl;
   return 0;
  }

  TFile *file = new TFile(rootfile,"UPDATE"); 
  if (!file->IsOpen()) { 
    std::cout << "File " << rootfile << " not open " << std::endl;
    return 0;
  }
  if (file->IsZombie()) { std::cout << "error opening file " << std::endl;}

  TH3D *histo = new TH3D("phiChi2;5","", 100,-2200,-1800, 60, -60,60 ,1000,0,0.002);
  TKey *key = file->FindKey(histoName[i]);
  if (key ==0){
    cout << "!!Histogram does not exist!!" << endl;
    return 0;
  }
  file->GetObject("phiChi2;5", histo);
  cout << " min bin " << histo->GetMinimumBin() << endl;

  return 1; 
}
