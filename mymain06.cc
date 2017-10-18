//Program designed to look at having multiple strings on top of each other
//Cody Duncan

#include <iostream>
#include "Pythia8/Pythia.h"

#include "TH1.h"
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TSystem.h"

using namespace Pythia8;
using namespace std;

int main(int argc, char* argv[]){
  TApplication theApp("hist", &argc, argv);
  Pythia pythia;
  Event& event = pythia.event;
  pythia.readFile("mymain06.cmnd");
  ParticleData& pdt = pythia.particleData;
  int nEvents = 100000;


  pythia.init();

  TH1D* pTNLam = new TH1D("ptNLam", "ratio of yields", 100, 1., 100.);
  TH1D* pTNPi = new TH1D("pTNPi","ratio of yields", 100, 1., 100.);
  int nCh, nchlessthan1,nFail;
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    event.reset();
    nCh = 0;

    if ( !pythia.next() ) {
      cout << " error\n";
      break;
    }

    for (int i = 0; i < event.size(); ++i){
      if( abs(event[i].eta())<0.5 && (event[i].isFinal()  && event[i].isCharged()) ){
        nCh++;
      }
    }

    if (nCh < 1){ nchlessthan1++; pythia.next(); }

    for (int i = 0; i < event.size(); ++i){

      if( abs(event[i].eta()) < 0.5 ){

        int m1index = event[i].mother1();
        int m2index = event[i].mother2();
        int m1id = event[m1index].id();
        int m2id = event[m2index].id();

        if ( (abs(event[i].id()) == 211) && !((abs(m1id) == 211) || (abs(m2id) == 211))){
          pTNPi->Fill(nCh);
        }
        else if ( (abs( event[i].id()) == 310) && !((abs(m1id) == 310) || (abs(m2id) == 3312))){
          pTNLam->Fill(nCh);
        }
      }
    }


  }

  pTNLam->Divide(pTNPi);
  pTNLam->Draw();
  gPad->SetLogy();
  gPad->SetLogx();
  std::cout << "\nDouble click on the histogram window to quit.\n";
  gPad->WaitPrimitive();
  cout<<"nCh   "<<nCh<<endl;
  cout<<"nFail    "<<nFail<<endl;
  cout<<"Mean charged multiplicity    "<<nCh*1./nEvents<<endl;
  return 0;
}
