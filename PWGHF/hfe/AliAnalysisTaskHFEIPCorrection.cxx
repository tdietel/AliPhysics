#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom3.h"
#include "TFile.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliESDEvent.h"
#include "AliESDInputHandler.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliMCEventHandler.h"
#include "AliAODv0KineCuts.h"
#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliCentrality.h"
#include "AliESDVZERO.h"
#include "AliESDpid.h"
#include "AliESDtrack.h"
#include "AliAnalysisTaskHFEIPCorrection.h"
#include "AliHFEpidTOF.h"
#include "AliHFEpid.h"
#include "AliHFEtools.h"
//#include "AliESDTrdTracklet.h"
//#include "AliTRDgeometry.h"
//#include "AliHFEcuts.h"
#include "AliHFEpid.h"
#include "AliHFEpidQAmanager.h"
#include "AliHFEtools.h"
#include "AliHFEVZEROEventPlane.h"
#include "AliEventplane.h"
#include <AliOADBContainer.h>
#include "AliMultSelection.h"
#include "AliVertexerTracks.h"

// This analysis builds histograms to enable and check the impact parameter
// correction in phi, z, and pT for the 15o PbPb data set
// Author: Martin Voelkl


ClassImp(AliAnalysisTaskHFEIPCorrection)

//________________________________________________________________________
AliAnalysisTaskHFEIPCorrection::AliAnalysisTaskHFEIPCorrection()
  : AliAnalysisTaskSE(), fAOD(0), fOutputContainer(0), fRd(0), EP2040(0), EP2040Corrected(0), EP2040V0A(0), EP2040V0C(0), TPCnSigma(0), EPCent(0), EPCentCorrected(0), EPCentV0A(0), EPCentV0C(0), DeltaPhi(0), fExtraCuts(0), fIPData(0), fpTIP2040IP(0), fpTIP2040OOP(0), fpTIP3050IP(0), fpTIP3050OOP(0), EventSelectionSteps(0), fPionV0pTRNoCuts(0), fPionV0pTRWithCuts(0), fPionV0pTRNoCutsIP(0), fPionV0pTRWithCutsIP(0), fPionV0pTRNoCutsOOP(0), fPionV0pTRWithCutsOOP(0), fAODV0Cuts(0), fPionV0pTTPC(0), fPionV0pTTPCWithCuts(0), fPionV0pTTPCIP(0), fPionV0pTTPCOOP(0), fPionV0pTTPCIPWTOF(0), fPionV0pTTPCOOPWTOF(0), fPionV0pTTPCIPnoFirst(0), fPionV0pTTPCOOPnoFirst(0), fPionV0pTTPCIPWTOFnoFirst(0), fPionV0pTTPCOOPWTOFnoFirst(0), fDCARegionRun(0), fDCAPhiZHadrons(0), fDCAPhiZHadronsEarlyRuns(0), fDCAPhiZHadronsLateRuns(0), fDCAPhiZHadronsC(0), fDCAPhipTHadrons(0), fDCAPhipTHadronsEarlyRuns(0), fDCAPhipTHadronsLateRuns(0), fDCAPhipTHadronsC(0), fDCAPhiZKaons(0), fDCAPhiZKaonsC(0), fDCAPhipTKaons(0), fDCAPhipTKaonsC(0), fpTPhiZHadrons(0), fDCAWErrHadrons(0), fDCAHadrons(0), fDCAHadronsFineBins(0), fDCAKaons(0), fDCAWErrKaons(0), fDCAKaonsFineBins(0)
{
  // default Constructor
  // Define input and output slots here

  // HFE cuts
    /*hfetrackCuts = new AliHFEcuts("V0trackCuts", "Track Cuts for tagged track Analysis");
    hfetrackCuts->CreateStandardCuts();

    hfetrackCuts->SetMinNClustersTPC(110);
    hfetrackCuts->SetMinNClustersTPCPID(80);
    hfetrackCuts->SetFractionOfSharedTPCClusters(1.1);
    hfetrackCuts->SetMinRatioTPCclusters(0.6);
    hfetrackCuts->SetTPCmodes(3,4);
    hfetrackCuts->SetMinNClustersITS(4);
    hfetrackCuts->SetCutITSpixel(AliHFEextraCuts::kBoth);
    hfetrackCuts->SetCheckITSLayerStatus(kFALSE);
    //hfetrackCuts->UnsetVertexRequirement();
    hfetrackCuts->SetTOFPIDStep(kTRUE);
    hfetrackCuts->Initialize();*/
    
    //fExtraCuts = new AliHFEextraCuts("hfeExtraCuts","HFE Extra Cuts");

    // end HFE cuts
  
}

//________________________________________________________________________
AliAnalysisTaskHFEIPCorrection::AliAnalysisTaskHFEIPCorrection(const char *name)
  : AliAnalysisTaskSE(name), fAOD(0), fOutputContainer(0), fRd(0), EP2040(0), EP2040Corrected(0), EP2040V0A(0), EP2040V0C(0), TPCnSigma(0), EPCent(0), EPCentCorrected(0), EPCentV0A(0), EPCentV0C(0), DeltaPhi(0), fExtraCuts(0), fIPData(0), fpTIP2040IP(0), fpTIP2040OOP(0), fpTIP3050IP(0), fpTIP3050OOP(0), EventSelectionSteps(0), fPionV0pTRNoCuts(0), fPionV0pTRWithCuts(0), fPionV0pTRNoCutsIP(0), fPionV0pTRWithCutsIP(0), fPionV0pTRNoCutsOOP(0), fPionV0pTRWithCutsOOP(0), fAODV0Cuts(0), fPionV0pTTPC(0), fPionV0pTTPCWithCuts(0), fPionV0pTTPCIP(0), fPionV0pTTPCOOP(0), fPionV0pTTPCIPWTOF(0), fPionV0pTTPCOOPWTOF(0), fPionV0pTTPCIPnoFirst(0), fPionV0pTTPCOOPnoFirst(0), fPionV0pTTPCIPWTOFnoFirst(0), fPionV0pTTPCOOPWTOFnoFirst(0), fDCARegionRun(0), fDCAPhiZHadrons(0), fDCAPhiZHadronsEarlyRuns(0), fDCAPhiZHadronsLateRuns(0), fDCAPhiZHadronsC(0), fDCAPhipTHadrons(0), fDCAPhipTHadronsEarlyRuns(0), fDCAPhipTHadronsLateRuns(0), fDCAPhipTHadronsC(0), fDCAPhiZKaons(0), fDCAPhiZKaonsC(0), fDCAPhipTKaons(0), fDCAPhipTKaonsC(0), fpTPhiZHadrons(0), fDCAWErrHadrons(0), fDCAHadrons(0), fDCAHadronsFineBins(0), fDCAKaons(0), fDCAWErrKaons(0), fDCAKaonsFineBins(0)
{
  // HFE cuts
    /*hfetrackCuts = new AliHFEcuts("V0trackCuts", "Track Cuts for tagged track Analysis");
    hfetrackCuts->CreateStandardCuts();

    hfetrackCuts->SetMinNClustersTPC(110);
    hfetrackCuts->SetMinNClustersTPCPID(80);
    hfetrackCuts->SetFractionOfSharedTPCClusters(1.1);
    hfetrackCuts->SetMinRatioTPCclusters(0.6);
    hfetrackCuts->SetTPCmodes(3,4);
    hfetrackCuts->SetMinNClustersITS(4);
    hfetrackCuts->SetCutITSpixel(AliHFEextraCuts::kBoth);
    hfetrackCuts->SetCheckITSLayerStatus(kFALSE);
    //hfetrackCuts->UnsetVertexRequirement();
    hfetrackCuts->SetTOFPIDStep(kTRUE);
    hfetrackCuts->Initialize();
    
    //fExtraCuts = new AliHFEextraCuts("hfeExtraCuts","HFE Extra Cuts");
*/
    // end HFE cuts
  // Constructor
  // Define input and output slots here
  // Input slot #0 works with a TChain
   DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TH1 container
  //DefineOutput(1, TObjArray::Class());
    DefineOutput(1, TList::Class());
    //    DefineOutput(2, TNtuple::Class());
    

}

AliAnalysisTaskHFEIPCorrection::~AliAnalysisTaskHFEIPCorrection()
{
}


//________________________________________________________________________
void AliAnalysisTaskHFEIPCorrection::UserCreateOutputObjects()
{
  // Create histograms
  // Called once
    Double_t ptbinningX[19] = {0., 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 2., 2.5, 3., 4., 6., 8., 10., 12., 16., 20.}; // Apr 2018 binning
    //Double_t IPBinning[401];
    
    EPCent = new TH2D(Form("EPCent"),Form("EPCent"), 20, 0., TMath::Pi(), 10, 0., 100.);
    EPCentV0A = new TH2D(Form("EPCentV0A"),Form("EPCentV0A"), 20, 0., TMath::Pi(), 10, 0., 100.);
    EPCentV0C = new TH2D(Form("EPCentV0C"),Form("EPCentV0C"), 20, 0., TMath::Pi(), 10, 0., 100.);
    EPCentCorrected = new TH2D(Form("EPCentCorrected"),Form("EPCentCorrected"), 20, 0., TMath::Pi(), 10, 0., 100.);
    
    EP2040 = new TH1D(Form("EP2040"),Form("EP2040"), 100, 0., TMath::Pi());
    EP2040Corrected = new TH1D(Form("EP2040Corrected"),Form("EP2040Corrected"), 100, 0., TMath::Pi());
    EP2040V0A = new TH1D(Form("EP2040V0A"),Form("EP2040V0A"), 100, 0., TMath::Pi());
    EP2040V0C = new TH1D(Form("EP2040V0C"),Form("EP2040V0C"), 100, 0., TMath::Pi());
    DeltaPhi = new TH1D(Form("DeltaPhi"),Form("DeltaPhi"), 40, 0., TMath::Pi());
    
    TPCnSigma = new TH2D("TPCnSigma", "", 18, ptbinningX, 100, -10., 5.);
    fIPData =  new TH2D("fIPData", "", 18, ptbinningX, 400, -0.2, 0.2);

    fpTIP2040IP =  new TH2D("pTIP2040IP", "", 18, ptbinningX, 400, -0.2, 0.2);
    fpTIP2040OOP =  new TH2D("pTIP2040OOP", "", 18, ptbinningX, 400, -0.2, 0.2);
    fpTIP3050IP =  new TH2D("pTIP3050IP", "", 18, ptbinningX, 400, -0.2, 0.2);
    fpTIP3050OOP =  new TH2D("pTIP3050OOP", "", 18, ptbinningX, 400, -0.2, 0.2);

    fPionV0pTRNoCuts = new TH3D(Form("fPionV0pTRNoCuts"),Form("fPionV0pTRNoCuts"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTRWithCuts = new TH3D(Form("fPionV0pTRWithCuts"),Form("fPionV0pTRWithCuts"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTRNoCutsIP = new TH3D(Form("fPionV0pTRNoCutsIP"),Form("fPionV0pTRNoCutsIP"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTRWithCutsIP = new TH3D(Form("fPionV0pTRWithCutsIP"),Form("fPionV0pTRWithCutsIP"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTRNoCutsOOP = new TH3D(Form("fPionV0pTRNoCutsOOP"),Form("fPionV0pTRNoCutsOOP"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTRWithCutsOOP = new TH3D(Form("fPionV0pTRWithCutsOOP"),Form("fPionV0pTRWithCutsOOP"), 40, 0., 10., 80, 0., 20., 10, 0., 100.);
    fPionV0pTTPC = new TH2D(Form("fPionV0pTTPC"),Form("fPionV0pTTPC"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCWithCuts = new TH2D(Form("fPionV0pTTPCWithCuts"),Form("fPionV0pTTPCWithCuts"), 18, ptbinningX, 200, -10., 10.);

    fPionV0pTTPCIP = new TH2D(Form("fPionV0pTTPCIP"),Form("fPionV0pTTPCIP"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCOOP = new TH2D(Form("fPionV0pTTPCOOP"),Form("fPionV0pTTPCOOP"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCIPWTOF = new TH2D(Form("fPionV0pTTPCIPWTOF"),Form("fPionV0pTTPCIPWTOF"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCOOPWTOF = new TH2D(Form("fPionV0pTTPCOOPWTOF"),Form("fPionV0pTTPCOOPWTOF"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCIPnoFirst = new TH2D(Form("fPionV0pTTPCIPnoFirst"),Form("fPionV0pTTPCIPnoFirst"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCOOPnoFirst = new TH2D(Form("fPionV0pTTPCOOPnoFirst"),Form("fPionV0pTTPCOOPnoFirst"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCIPWTOFnoFirst = new TH2D(Form("fPionV0pTTPCIPWTOFnoFirst"),Form("fPionV0pTTPCIPWTOFnoFirst"), 18, ptbinningX, 200, -10., 10.);
    fPionV0pTTPCOOPWTOFnoFirst = new TH2D(Form("fPionV0pTTPCOOPWTOFnoFirst"),Form("fPionV0pTTPCOOPWTOFnoFirst"), 18, ptbinningX, 200, -10., 10.);


    EventSelectionSteps = new TH1D(Form("EventSelectionSteps"),Form("EventSelectionSteps"), 10, -0.5, 9.5);

    fDCARegionRun = new TH3D(Form("fDCARegionRun"),Form("fDCARegionRun"), 400, -0.05, 0.05, 5, 0.5, 5.5, 183, 0.5, 183.5);
    fpTPhiZHadrons = new TH3D(Form("fpTPhiZHadrons"),Form("fpTPhiZHadrons"), 40, 0., 2.*3.14159, 40, 0., 10., 12, -12., 12.);
    fDCAPhiZHadrons = new TH3D(Form("fDCAPhiZHadrons"),Form("fDCAPhiZHadrons"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhiZHadronsEarlyRuns = new TH3D(Form("fDCAPhiZHadronsEarlyRuns"),Form("fDCAPhiZHadronsEarlyRuns"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhiZHadronsLateRuns = new TH3D(Form("fDCAPhiZHadronsLateRuns"),Form("fDCAPhiZHadronsLateRuns"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhiZHadronsC = new TH3D(Form("fDCAPhiZHadronsC"),Form("fDCAPhiZHadronsC"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhipTHadrons = new TH3D(Form("fDCAPhipTHadrons"),Form("fDCAPhipTHadrons"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAPhipTHadronsEarlyRuns = new TH3D(Form("fDCAPhipTHadronsEarlyRuns"),Form("fDCAPhipTHadronsEarlyRuns"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAPhipTHadronsLateRuns = new TH3D(Form("fDCAPhipTHadronsLateRuns"),Form("fDCAPhipTHadronsLateRuns"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAPhipTHadronsC = new TH3D(Form("fDCAPhipTHadronsC"),Form("fDCAPhipTHadronsC"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAPhiZKaons = new TH3D(Form("fDCAPhiZKaons"),Form("fDCAPhiZKaons"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhiZKaonsC = new TH3D(Form("fDCAPhiZKaonsC"),Form("fDCAPhiZKaonsC"), 40, 0., 2.*3.14159, 400, -0.05, 0.05, 12, -12., 12.);
    fDCAPhipTKaons = new TH3D(Form("fDCAPhipTKaons"),Form("fDCAPhipTKaons"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAPhipTKaonsC = new TH3D(Form("fDCAPhipTKaonsC"),Form("fDCAPhipTKaonsC"), 400, -0.05, 0.05, 40, 0., 2.*3.14159, 40, 0., 10.);
    fDCAWErrHadrons = new TH3D(Form("fDCAWErrHadrons"),Form("fDCAWErrHadrons"), 80, 0., 10., 400, -0.2, 0.2, 100, 0.0, 0.01);
    fDCAHadrons = new TH2D(Form("fDCAHadrons"),Form("fDCAHadrons"), 18, ptbinningX, 400, -0.2, 0.2);
    fDCAHadronsFineBins = new TH3D(Form("fDCAHadronsFineBins"),Form("fDCAHadronsFineBins"), 80, 0., 10., 400, -0.2, 0.2, 10, 0., 100.);
    fDCAKaons = new TH2D(Form("fDCAKaons"),Form("fDCAKaons"), 18, ptbinningX, 400, -0.2, 0.2);
    fDCAWErrKaons = new TH3D(Form("fDCAWErrKaons"),Form("fDCAWErrKaons"), 80, 0., 10., 400, -0.2, 0.2, 100, 0.0, 0.01);
    fDCAKaonsFineBins = new TH3D(Form("fDCAKaonsFineBins"),Form("fDCAKaonsFineBins"), 80, 0., 10., 400, -0.2, 0.2, 10, 0., 100.);
    
    fExtraCuts = new AliHFEextraCuts("hfeExtraCuts","HFE Extra Cuts");
    fAODV0Cuts = new AliAODv0KineCuts();
    
    fRd = new TRandom3(0);

    fOutputContainer = new TObjArray(1);
    fOutputContainer->SetName(GetName());
    fOutputContainer->SetOwner();
    fOutputContainer->Add(EPCent);
    fOutputContainer->Add(EPCentV0A);
    fOutputContainer->Add(EPCentV0C);
    fOutputContainer->Add(EPCentCorrected);
    fOutputContainer->Add(EP2040);
    fOutputContainer->Add(EP2040Corrected);
    fOutputContainer->Add(EP2040V0A);
    fOutputContainer->Add(EP2040V0C);
    fOutputContainer->Add(DeltaPhi);
    fOutputContainer->Add(TPCnSigma);
    fOutputContainer->Add(fIPData);
    fOutputContainer->Add(fpTIP2040IP);
    fOutputContainer->Add(fpTIP2040OOP);
    fOutputContainer->Add(fpTIP3050IP);
    fOutputContainer->Add(fpTIP3050OOP);
    fOutputContainer->Add(fPionV0pTRNoCuts);
    fOutputContainer->Add(fPionV0pTRWithCuts);
    fOutputContainer->Add(fPionV0pTRNoCutsIP);
    fOutputContainer->Add(fPionV0pTRWithCutsIP);
    fOutputContainer->Add(fPionV0pTRNoCutsOOP);
    fOutputContainer->Add(fPionV0pTRWithCutsOOP);
    fOutputContainer->Add(fPionV0pTTPC);
    fOutputContainer->Add(fPionV0pTTPCWithCuts);
    fOutputContainer->Add(fPionV0pTTPCIP);
    fOutputContainer->Add(fPionV0pTTPCOOP);
    fOutputContainer->Add(fPionV0pTTPCIPWTOF);
    fOutputContainer->Add(fPionV0pTTPCOOPWTOF);
    fOutputContainer->Add(fPionV0pTTPCIPnoFirst);
    fOutputContainer->Add(fPionV0pTTPCOOPnoFirst);
    fOutputContainer->Add(fPionV0pTTPCIPWTOFnoFirst);
    fOutputContainer->Add(fPionV0pTTPCOOPWTOFnoFirst);
    fOutputContainer->Add(EventSelectionSteps);
    fOutputContainer->Add(fDCARegionRun);
    fOutputContainer->Add(fDCAPhiZHadrons);
    fOutputContainer->Add(fDCAPhiZHadronsEarlyRuns);
    fOutputContainer->Add(fDCAPhiZHadronsLateRuns);
    fOutputContainer->Add(fDCAPhiZHadronsC);
    fOutputContainer->Add(fDCAPhipTHadrons);
    fOutputContainer->Add(fDCAPhipTHadronsEarlyRuns);
    fOutputContainer->Add(fDCAPhipTHadronsLateRuns);
    fOutputContainer->Add(fDCAPhipTHadronsC);
    fOutputContainer->Add(fDCAPhiZKaons);
    fOutputContainer->Add(fDCAPhiZKaonsC);
    fOutputContainer->Add(fDCAPhipTKaons);
    fOutputContainer->Add(fDCAPhipTKaonsC);
    fOutputContainer->Add(fpTPhiZHadrons);
    fOutputContainer->Add(fDCAWErrHadrons);
    fOutputContainer->Add(fDCAHadrons);
    fOutputContainer->Add(fDCAHadronsFineBins);
    fOutputContainer->Add(fDCAKaons);
    fOutputContainer->Add(fDCAWErrKaons);
    fOutputContainer->Add(fDCAKaonsFineBins);

    PostData(1, fOutputContainer);
    
}

//_____________________________________________________________________________
void AliAnalysisTaskHFEIPCorrection::UserExec(Option_t *)
{
  //
  // Called for each event
    //
    

    //AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
    //AliAODInputHandler *aodH = dynamic_cast<AliAODInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());

    if (!fAOD)
    {
	printf("ERROR: Could not get AODInputHandler \n");
    }
    else fAOD = dynamic_cast<AliAODEvent*>(InputEvent());


    Process(fAOD);


    // Post output data.
    //PostData(1, fOutput);
    //PostData(1, fOutputContainer);
    //    PostData(2, rec);
    //    PostData(2, treeoutput);
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesTrackCuts(AliAODTrack *track)
{
    if(TMath::Abs(track->Eta())>0.8) return kFALSE;
    ULong_t status = track->GetStatus();
    // Basic tracking
    if(!((status & AliVTrack::kITSrefit) && (status & AliVTrack::kTPCrefit))) return kFALSE;
    // ITS tracking
    if(!(track->HasPointOnITSLayer(0) && track->HasPointOnITSLayer(1) && track->GetITSNcls()>=4)) return kFALSE;
    Int_t nclustersITS(track->GetITSclusters(NULL)),
          nclustersTPC(track->GetTPCNcls()),
          nclustersTPCall(track->GetTPCClusterMap().CountBits()),
          nclustersTPCshared(0);
    UChar_t nfindableTPC = track->GetTPCNclsF();
    Double_t FoundOverFindable = (nfindableTPC ? static_cast<Float_t>(nclustersTPC)/static_cast<Float_t>(nfindableTPC) : 0);
    // TPC Quality
    if(!(track->GetTPCNcls()>=110 && track->GetTPCsignalN()>=80 && FoundOverFindable>0.6 && track->GetTPCchi2()/Double_t(nclustersTPC)<4.)) return kFALSE;
    // ITS Quality
    if(track->GetITSchi2()/double(track->GetITSNcls()) > 10.) return kFALSE; 
      UChar_t ITSShared = track->GetITSSharedClusterMap();
      Int_t nSharedITS = 0;
      for(int i=0;i<6;i++)
          if((ITSShared >> i) & 1)
              nSharedITS++;
    if(nSharedITS > 3) return kFALSE; 
    // TOF quality
    //if(track->GetTOFsignal()>=99999) return kFALSE;
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesTrackCutsNoFirst(AliAODTrack *track)
{
    if(TMath::Abs(track->Eta())>0.8) return kFALSE;
    ULong_t status = track->GetStatus();
    // Basic tracking
    if(!((status & AliVTrack::kITSrefit) && (status & AliVTrack::kTPCrefit))) return kFALSE;
    // ITS tracking
    if(!(!(track->HasPointOnITSLayer(0)) && track->HasPointOnITSLayer(1) && track->GetITSNcls()>=4)) return kFALSE;
    Int_t nclustersITS(track->GetITSclusters(NULL)),
          nclustersTPC(track->GetTPCNcls()),
          nclustersTPCall(track->GetTPCClusterMap().CountBits()),
          nclustersTPCshared(0);
    UChar_t nfindableTPC = track->GetTPCNclsF();
    Double_t FoundOverFindable = (nfindableTPC ? static_cast<Float_t>(nclustersTPC)/static_cast<Float_t>(nfindableTPC) : 0);
    // TPC Quality
    if(!(track->GetTPCNcls()>=110 && track->GetTPCsignalN()>=80 && FoundOverFindable>0.6 && track->GetTPCchi2()/Double_t(nclustersTPC)<4.)) return kFALSE;
    // ITS Quality
    if(track->GetITSchi2()/double(track->GetITSNcls()) > 10.) return kFALSE; 
      UChar_t ITSShared = track->GetITSSharedClusterMap();
      Int_t nSharedITS = 0;
      for(int i=0;i<6;i++)
          if((ITSShared >> i) & 1)
              nSharedITS++;
    if(nSharedITS > 3) return kFALSE; 
    // TOF quality
    //if(track->GetTOFsignal()>=99999) return kFALSE;
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesElectronPID(AliAODTrack *track, AliPIDResponse *pid)
{
    if(pid->NumberOfSigmasTPC(track, AliPID::kElectron) >3. || pid->NumberOfSigmasTPC(track, AliPID::kElectron) <-0.5) return kFALSE;
    if(TMath::Abs(pid->NumberOfSigmasTOF(track, AliPID::kElectron))>3.) return kFALSE;
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesPionPID(AliAODTrack *track, AliPIDResponse *pid)
{
    if(pid->NumberOfSigmasTPC(track, AliPID::kPion) > 3. || pid->NumberOfSigmasTPC(track, AliPID::kPion) < -1.) return kFALSE;
    if(TMath::Abs(pid->NumberOfSigmasTOF(track, AliPID::kPion))>3.) return kFALSE; // Should be basically same as electron
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesKaonPID(AliAODTrack *track, AliPIDResponse *pid)
{
    if(pid->NumberOfSigmasTPC(track, AliPID::kKaon) >3. || pid->NumberOfSigmasTPC(track, AliPID::kKaon) <-3.) return kFALSE;
    if(TMath::Abs(pid->NumberOfSigmasTOF(track, AliPID::kKaon))>2.) return kFALSE;
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesMinimalTrackCuts(AliAODTrack *track)
{
    if(TMath::Abs(track->Eta())>0.8) return kFALSE;
    ULong_t status = track->GetStatus();
    // Basic tracking
    if(!((status & AliVTrack::kITSrefit) && (status & AliVTrack::kTPCrefit))) return kFALSE;
    return kTRUE;
}

Bool_t AliAnalysisTaskHFEIPCorrection::PassesITSTrackCuts(AliAODTrack *track)
{
    if(TMath::Abs(track->Eta())>0.8) return kFALSE;
    ULong_t status = track->GetStatus();
    // Basic tracking
    if(!((status & AliVTrack::kITSrefit) && (status & AliVTrack::kTPCrefit))) return kFALSE;
    // ITS tracking
    if(!(track->HasPointOnITSLayer(0) && track->HasPointOnITSLayer(1) && track->GetITSNcls()>=4)) return kFALSE;
    Int_t nclustersITS(track->GetITSclusters(NULL)),
          nclustersTPC(track->GetTPCNcls()),
          nclustersTPCall(track->GetTPCClusterMap().CountBits()),
          nclustersTPCshared(0);
    // ITS Quality
    if(track->GetITSchi2()/double(track->GetITSNcls()) > 10.) return kFALSE; 
      UChar_t ITSShared = track->GetITSSharedClusterMap();
      Int_t nSharedITS = 0;
      for(int i=0;i<6;i++)
          if((ITSShared >> i) & 1)
              nSharedITS++;
    if(nSharedITS > 3) return kFALSE; 
    return kTRUE;
}

Int_t AliAnalysisTaskHFEIPCorrection::IsInMisalignedRegion(AliAODTrack *track, double vtxz)
{
  double halfstavephi = 2. * TMath::Pi() / 40.;
  double phi = track->Phi();
  double zSPD = vtxz+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))); // z position of track at first layer
  if(phi>6.*halfstavephi && phi<=9.*halfstavephi && zSPD>0.) return 1;
  if(phi>17.*halfstavephi && phi<=19.*halfstavephi && zSPD>0.) return 2;
  if(phi>21.*halfstavephi && phi<=23.*halfstavephi) return 3;
  if(phi>25.*halfstavephi && phi<=27.*halfstavephi && zSPD<0.) return 4;
  return 0;
}

AliAODVertex * AliAnalysisTaskHFEIPCorrection::CorrectVertex(AliAODEvent *aodEvent, double vtxz) // Vertex without using excluded regions
{
  AliAODVertex * vtxcorr;
  AliAODTrack *track = 0x0;
  Int_t * skippedTracks = new Int_t[fInputEvent->GetNumberOfTracks()];
  Int_t nskipped = 0;
  // First fill array of tracks to be disregarded in primary vertex estimation
  for(Int_t itrack = 0; itrack < fInputEvent->GetNumberOfTracks(); itrack++)
      {
        // Run track loop
        track = dynamic_cast<AliAODTrack *>(fInputEvent->GetTrack(itrack));
        if(track->Pt()>0.5 && TMath::Abs(track->Eta())<1.)
        {
          if(IsInMisalignedRegion(track, vtxz)>0)
          {
            if(((Int_t) track->GetID())>0)
            {
              skippedTracks[nskipped] = (Int_t) track->GetID();
              nskipped++;
            }
          }
        }
      }
  // Now reconstruct the primary vertex without these tracks
  AliVertexerTracks vertexer(aodEvent->GetMagneticField());
  vertexer.SetITSMode();
  vertexer.SetMinClusters(3);
  vertexer.SetConstraintOff();
  vertexer.SetSkipTracks(nskipped,skippedTracks);
  AliESDVertex *vtxESDNew = vertexer.FindPrimaryVertex(aodEvent);

  // convert to AliAODVertex -- copied from hfe task
  Double_t pos[3],cov[6],chi2perNDF;
  vtxESDNew->GetXYZ(pos); // position
  vtxESDNew->GetCovMatrix(cov); //covariance matrix
  chi2perNDF = vtxESDNew->GetChi2toNDF();
  delete vtxESDNew; vtxESDNew=NULL;

  vtxcorr = new AliAODVertex(pos,cov,chi2perNDF); // Makes AOD vertex

  delete[] skippedTracks;
  return vtxcorr;
}

void AliAnalysisTaskHFEIPCorrection::GetTrackImpactParameter(AliAODEvent *aodEvent, AliAODTrack *track, AliAODVertex * pvtx, Double_t &dcaxy)
{
  Double_t dcaD[3]; // 2 should be sufficient, 3 to check if reason for crash
  Double_t covD[3];
  Double_t fMagField;
  const Double_t kBeampiperadius=3.;
  AliAODTrack aodtrack(*track);
  AliExternalTrackParam etp; etp.CopyFromVTrack(&aodtrack);
  fMagField = aodEvent->GetMagneticField();
  etp.PropagateToDCA(pvtx, fMagField, kBeampiperadius, dcaD, covD);
  dcaxy = dcaD[0];
}

void AliAnalysisTaskHFEIPCorrection::GetCorrectedImpactParameter(AliAODEvent *aodEvent, AliAODTrack *track, Double_t primVertexZ, Double_t &dcaxy)
{
  // primVertexZ is the z Position of the primary vertex
  // Using 40 phi bins, 12 z bins (-12 to +12 cm) and smooth pT correction
  int PhiBin = int(track->Phi()/(2.*TMath::Pi())*40); // phi bin
  double z_SPD1 = primVertexZ+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))); // z position of track in inner SPD layer
  int zBin = int((z_SPD1+12.)/24.*12.); // z bin
  int pt = track->Pt();
  double oldIP, dcaErr;
  fExtraCuts->GetHFEImpactParameters((AliVTrack *)track,oldIP,dcaErr);
    
  double correctionMatrixEarlyRuns[40][12] = {
{ -0.000683 , -0.00104 , -0.000436 , -0.000785 , -0.001 , -0.00117 , -0.00103 , -0.000972 , -0.00149 , -0.0014 , -0.00147 , -0.00158 },
{ -0.000429 , -0.000331 , -0.000925 , -0.00097 , -0.000927 , -0.00109 , -0.00088 , -0.000629 , -0.000606 , -0.000768 , -0.000875 , -0.000799 },
{ 9.07e-05 , -0.000232 , -0.000481 , -0.000671 , -0.000605 , -0.000969 , -0.000581 , -0.000624 , -0.000443 , 0.000108 , -0.000173 , -0.00068 },
{ -0.000188 , -0.000327 , -0.000773 , -0.00087 , -0.00092 , -0.000998 , -0.000481 , 9.87e-05 , 1.33e-05 , -0.000173 , -5.06e-05 , -0.000686 },
{ -0.00151 , -0.00167 , -0.00194 , -0.00157 , -0.00123 , -0.00122 , -0.000314 , -0.00038 , -0.000235 , 0.000169 , -0.000415 , -0.000513 },
{ -0.00181 , -0.00122 , -0.00145 , -0.00142 , -0.00114 , -0.00115 , -0.000569 , 0.000239 , 0.000207 , -5e-05 , -0.000104 , -0.000634 },
{ -0.000947 , -0.000512 , -0.000538 , -0.000851 , -0.000932 , -0.00113 , -0.00405 , -0.00372 , -0.00334 , -0.00351 , -0.00297 , -0.00292 },
{ -0.000949 , -0.000806 , -0.00121 , -0.000865 , -0.000237 , -0.001 , -0.00561 , -0.00621 , -0.00594 , -0.00551 , -0.00464 , -0.004 },
{ -0.00142 , -0.00111 , -0.00216 , -0.00132 , -0.000642 , -0.00153 , -0.00284 , -0.00295 , -0.00314 , -0.00349 , -0.00267 , -0.00226 },
{ 0 , 0 , 0 , 0 , 0 , 0.00153 , 0.000849 , 0.000952 , 0.000799 , 0.000527 , 0.000224 , 0.000269 },
{ 0 , 0 , 0 , 0 , 0 , 0.000431 , -0.000211 , 0.000241 , 0.00019 , -9.1e-05 , 0.000373 , -0.000161 },
{ 0 , 0 , 0 , 0 , 0 , -0.000518 , -0.000786 , -0.000138 , -0.000261 , -0.000291 , 0.000237 , -9.44e-05 },
{ 0 , 0 , 0 , 0 , 0 , -0.000486 , -0.000984 , 1.7e-05 , 0.000154 , -0.0004 , 0.000312 , 0.000181 },
{ 0 , 0 , 0 , 0 , 0 , -0.000825 , -0.00171 , -0.0012 , -0.00113 , -0.000976 , -0.000555 , -0.000574 },
{ 0.00528 , 0.00546 , 0.00561 , 0.0053 , 0.00782 , 0.00596 , -0.00188 , -0.00094 , -0.000935 , -0.00146 , -0.000619 , -0.00122 },
{ -0.000379 , -0.000128 , -0.000382 , -0.000331 , 0.000108 , 7.47e-06 , -0.000598 , -9.11e-06 , -0.000398 , -0.000601 , 5.74e-05 , -0.000298 },
{ 4.06e-05 , 0.000116 , -0.000144 , -0.00012 , -9.49e-06 , -0.000392 , -0.000871 , -0.000249 , -0.00042 , -0.000808 , -0.000448 , -0.000573 },
{ -0.000487 , -0.000342 , -0.000591 , -0.000453 , -0.000259 , -0.000576 , 5.04e-05 , 0.00179 , 0.00162 , 0.00171 , 0.00157 , 0.000738 },
{ -0.000837 , -0.000198 , -0.00133 , -0.000705 , -0.000506 , -0.000963 , -0.00114 , 0.00114 , 0.0012 , 0.000515 , 0.000815 , 0.000431 },
{ -0.000696 , -8.76e-05 , -0.000227 , -0.000296 , -0.000356 , -0.000756 , -0.000786 , -0.000473 , -0.000626 , -0.00091 , -0.000828 , -0.000739 },
{ -0.000317 , -0.000368 , -0.000625 , -0.00042 , -0.000556 , -0.000786 , -0.000867 , -0.001 , -0.000915 , -0.00108 , -0.000911 , -0.000861 },
{ -0.00264 , -0.00284 , -0.00345 , -0.00373 , -0.00395 , -0.00415 , -0.00475 , -0.00518 , -0.00455 , -0.00389 , -0.00374 , -0.00383 },
{ -0.003 , -0.00298 , -0.00348 , -0.00363 , -0.00436 , -0.00443 , -0.00512 , -0.00489 , -0.00516 , -0.00505 , -0.00461 , -0.00453 },
{ -0.00201 , -0.00196 , -0.00182 , -0.00186 , -0.00201 , -0.00184 , -0.00139 , 0 , 0 , 0 , 0 , 0 },
{ -0.00226 , -0.0022 , -0.00268 , -0.00234 , -0.00166 , -0.00256 , -0.00262 , -0.00311 , -0.00336 , -0.00292 , -0.00268 , -0.00203 },
{ -0.00359 , -0.00288 , -0.00304 , -0.0031 , -0.00305 , -0.00314 , -0.0031 , -0.0027 , -0.0028 , -0.00293 , -0.00291 , -0.00201 },
{ -0.00257 , -0.0031 , -0.00341 , -0.00355 , -0.00353 , -0.00368 , -0.00232 , -0.00279 , -0.00294 , -0.00252 , -0.00274 , -0.00221 },
{ 0 , 0 , 0 , 0 , 0 , 0 , -0.00229 , -0.0025 , -0.00262 , -0.00274 , -0.00266 , -0.0024 },
{ 0.000774 , -0.000295 , -5.94e-05 , 0.000142 , -0.000405 , -0.000619 , -0.0011 , -0.00163 , -0.0019 , -0.0014 , -0.00187 , -0.00209 },
{ -0.00156 , -0.000174 , -0.000509 , -0.000816 , -0.000144 , -0.000683 , -0.00159 , -0.00121 , -0.00134 , -0.00222 , -0.00196 , -0.00176 },
{ -0.00022 , 0.00105 , 0.00124 , 0.000443 , 0.000713 , 0.00162 , -0.00128 , -0.00204 , -0.00173 , -0.00154 , -0.00213 , -0.0013 },
{ 0.000455 , 0.000776 , 0.000464 , 0.000417 , 0.000714 , 0.00041 , -0.00131 , -0.00149 , -0.0019 , -0.00193 , -0.000986 , -0.00151 },
{ -0.000248 , -0.000424 , -0.000149 , -0.000592 , -0.00101 , -0.00134 , -0.0018 , -0.00161 , -0.00156 , -0.00167 , -0.00185 , -0.0019 },
{ -4.59e-05 , -0.000561 , -0.000717 , -0.000926 , -0.00118 , -0.00157 , -0.00227 , -0.0022 , -0.00251 , -0.00231 , -0.00231 , -0.00205 },
{ 0.000267 , 5.08e-05 , 0.000296 , -0.000128 , -1.75e-05 , -0.000261 , 0.000354 , 0 , 0 , 0 , 0 , 0 },
{ -0.000259 , -0.000225 , -0.000134 , -0.000399 , -0.00022 , 5.61e-05 , 1.45e-05 , -0.000399 , -0.00145 , -0.00201 , -0.000793 , -0.00092 },
{ -0.00117 , -0.00129 , -0.000751 , -0.00122 , -0.00143 , -0.00104 , -0.00138 , -0.00108 , -0.00121 , -0.00102 , -0.000913 , -0.00132 },
{ -0.00186 , -0.00166 , -0.00158 , -0.00174 , -0.00167 , -0.0019 , -0.00102 , -0.000805 , -0.000779 , -0.000263 , -0.000164 , -0.000461 },
{ -0.00151 , -0.00135 , -0.0013 , -0.00134 , -0.00132 , -0.00143 , -0.000603 , -0.000576 , -0.000571 , -7.53e-05 , -4.39e-05 , -0.000317 },
{ -0.00166 , -0.00161 , -0.00148 , -0.00148 , -0.00129 , -0.00169 , -0.00119 , -0.00086 , -0.00172 , -0.0018 , -0.00125 , -0.00151 }
}; // Hardcoded, 15o correction matrix for the amplitude parameter of the correction function, bins are [phibin][zbin]

double correctionMatrixLateRuns[40][12] = {
{ 0.000114 , -8.83e-05 , 0.000528 , -8.9e-05 , -0.00022 , -0.000357 , -0.000213 , -0.000163 , -0.000792 , -0.000666 , -0.000543 , -0.000764 },
{ 0.000446 , 0.000558 , -7.86e-05 , -0.000105 , -9.73e-05 , -0.000263 , -0.000173 , 0.000129 , -4.93e-05 , -6.08e-05 , -0.000176 , -0.00029 },
{ 0.000953 , 0.000763 , 0.000427 , 0.000145 , 0.000363 , -2.55e-05 , 0.000359 , 0.000305 , 0.000352 , 0.000879 , 0.000528 , 6.47e-05 },
{ 0.00088 , 0.000752 , 0.000357 , 0.000185 , 0.000142 , 1.42e-06 , 0.000368 , 0.000811 , 0.000901 , 0.000543 , 0.000828 , 0.000172 },
{ -0.000738 , -0.000591 , -0.000974 , -0.000571 , -0.000404 , -0.000397 , 0.000505 , 0.000376 , 0.00053 , 0.000898 , 0.00044 , 6.72e-05 },
{ -0.000963 , -0.000277 , -0.000583 , -0.000642 , -0.000338 , -0.000361 , 0.000167 , 0.00103 , 0.000815 , 0.00071 , 0.000591 , 0.000152 },
{ 0.000426 , 0.000499 , 0.000576 , 0.000281 , 0.00011 , -5.69e-05 , -0.00305 , -0.0028 , -0.00235 , -0.00273 , -0.00219 , -0.00196 },
{ 0.000178 , 0.000189 , -0.000147 , 0.000163 , 0.00077 , 1.37e-05 , -0.00465 , -0.00543 , -0.00505 , -0.0047 , -0.00394 , -0.00305 },
{ -0.000225 , -0.000128 , -0.000944 , -0.000217 , 0.000501 , -0.000481 , -0.00216 , -0.00246 , -0.00255 , -0.00293 , -0.00197 , -0.00162 },
{ 0 , 0 , 0 , 0 , 0 , 0.00143 , 0.00135 , 0.00139 , 0.00132 , 0.000918 , 0.000703 , 0.000998 },
{ 0 , 0 , 0 , 0 , 0 , 0.000747 , 0.000278 , 0.000759 , 0.000592 , 0.000288 , 0.000902 , 0.000388 },
{ 0 , 0 , 0 , 0 , 0 , 0.000283 , -0.000195 , 0.000452 , 0.000454 , 0.000362 , 0.000776 , 0.000565 },
{ 0 , 0 , 0 , 0 , 0 , -9.02e-05 , -0.000432 , 0.000709 , 0.000771 , 3.08e-06 , 0.000861 , 0.000676 },
{ 0 , 0 , 0 , 0 , 0 , -0.00115 , -0.00114 , -0.000507 , -0.000299 , -0.000238 , 0.00016 , 0.000344 },
{ 0 , 0 , 0.0104 , 0.0109 , 0.0154 , 0 , -0.00107 , -0.000138 , -0.000275 , -0.000841 , 0.0001 , -0.000255 },
{ 0.000763 , 0.001 , 0.000771 , 0.000781 , 0.00131 , 0.00108 , 0.000179 , 0.000676 , 0.00026 , -8.63e-05 , 0.000926 , 0.000429 },
{ 0.000838 , 0.00111 , 0.000807 , 0.000881 , 0.000947 , 0.00062 , -9.65e-05 , 0.000507 , 0.000344 , -0.000205 , 0.000252 , 0.000238 },
{ 0.000909 , 0.000858 , 0.000463 , 0.000592 , 0.000875 , 0.000575 , 0.00113 , 0.00277 , 0.00287 , 0.00287 , 0.00259 , 0.00146 },
{ 5.73e-05 , 0.000894 , -0.000269 , 0.000742 , 0.00078 , 9.85e-05 , 5.43e-05 , 0.00232 , 0.00233 , 0.00171 , 0.00188 , 0.00161 },
{ 0.000354 , 0.00126 , 0.000932 , 0.000969 , 0.000791 , 0.000384 , 0.000298 , 0.000732 , 0.000442 , 0.000268 , 0.000245 , 0.000317 },
{ 0.000907 , 0.00114 , 0.000664 , 0.000916 , 0.000624 , 0.000659 , 0.000404 , 0.000142 , 0.000299 , -1.87e-05 , 0.000312 , 0.000397 },
{ -0.0021 , -0.00186 , -0.00232 , -0.00254 , -0.00282 , -0.00313 , -0.00368 , -0.00414 , -0.0035 , -0.00277 , -0.00258 , -0.00275 },
{ -0.00185 , -0.00184 , -0.00238 , -0.00253 , -0.00328 , -0.00332 , -0.00405 , -0.00389 , -0.00411 , -0.00388 , -0.00347 , -0.00355 },
{ -0.000455 , -0.000429 , -0.00048 , -0.000618 , -0.000634 , -0.000348 , 0.00017 , 0 , 0 , 0 , 0 , 0 },
{ -0.000758 , -0.000642 , -0.00118 , -0.00106 , -0.000257 , -0.00116 , -0.00131 , -0.00208 , -0.00211 , -0.00167 , -0.00139 , -0.000702 },
{ -0.0021 , -0.00156 , -0.00168 , -0.00185 , -0.00194 , -0.002 , -0.00184 , -0.0015 , -0.0015 , -0.00154 , -0.00159 , -0.000732 },
{ -0.00126 , -0.00168 , -0.00245 , -0.00245 , -0.00266 , -0.00241 , -0.00103 , -0.00156 , -0.00185 , -0.00119 , -0.00135 , -0.000824 },
{ 0 , 0 , 0 , 0 , 0 , 0 , -0.00114 , -0.0013 , -0.00144 , -0.00147 , -0.00133 , -0.00124 },
{ 0.00153 , 0.000797 , 0.00115 , 0.00136 , 0.000833 , 0.000597 , 0.000257 , -0.000188 , -0.000506 , -0.000139 , -0.000359 , -0.00043 },
{ -0.000104 , 0.000889 , 0.000451 , 0.000561 , 0.0011 , 0.000489 , -0.000186 , 0.000132 , -9.97e-05 , -0.000682 , -0.000475 , -3.75e-07 },
{ 0.000518 , 0.00193 , 0.00214 , 0.00138 , 0.00169 , 0.00273 , -7.75e-05 , -0.000903 , -0.000568 , -0.00031 , -0.000771 , 7.98e-05 },
{ 0.00112 , 0.0013 , 0.00135 , 0.0014 , 0.00151 , 0.00137 , -0.00021 , -0.000277 , -0.000672 , -0.000424 , 0.000329 , -4.97e-05 },
{ 0.000295 , 8.55e-05 , 0.000501 , 5.88e-06 , -0.000251 , -0.000605 , -0.000998 , -0.00077 , -0.000605 , -0.00068 , -0.000894 , -0.000468 },
{ -4.39e-05 , 9.05e-05 , -0.000151 , -0.000384 , -0.000385 , -0.000908 , -0.00145 , -0.00138 , -0.00154 , -0.00148 , -0.00141 , -0.00108 },
{ 0.000924 , 0.000762 , 0.00087 , 0.000478 , 0.000699 , 0.000349 , 0.00135 , 0 , 0 , 0 , 0 , 0 },
{ 0.000534 , 0.000273 , 0.000512 , 0.000367 , 0.000547 , 0.000747 , 0.000785 , 0.00025 , -0.00042 , -0.000993 , -0.000249 , -0.000247 },
{ -0.000631 , -0.000784 , -0.000278 , -0.000822 , -0.00087 , -0.000478 , -0.00085 , -0.000419 , -0.000445 , -0.000433 , -0.000376 , -0.000552 },
{ -0.00109 , -0.000822 , -0.000965 , -0.00104 , -0.000902 , -0.00119 , -0.000429 , -9.94e-05 , -1.34e-05 , 0.000677 , 0.000541 , 0.000417 },
{ -0.00102 , -0.000594 , -0.000451 , -0.000547 , -0.000505 , -0.000739 , 0.000298 , 0.000266 , 0.000262 , 0.000761 , 0.000835 , 0.000457 },
{ -0.000854 , -0.000851 , -0.000773 , -0.000646 , -0.000455 , -0.00103 , -0.000426 , -0.0001 , -0.00087 , -0.000856 , -0.000199 , -0.000902 }
};

  Double_t correctedDCA = 1.;
  if(aodEvent->GetRunNumber() <= 246276 && pt > 0.) correctedDCA = oldIP - correctionMatrixEarlyRuns[PhiBin][zBin]*1.22/(1.+1./(0.57+9.8/(pt*pt)));
  if(aodEvent->GetRunNumber() > 246276 && pt > 0.) correctedDCA = oldIP - correctionMatrixLateRuns[PhiBin][zBin]*1.22/(1.+1./(0.57+9.8/(pt*pt)));
  dcaxy = correctedDCA;
}



Int_t AliAnalysisTaskHFEIPCorrection::ReturnRunBin(Int_t RunNr)
{
  Int_t runList[183]={
  246994, 246991, 246989, 246984, 246982, 246980, 246949, 246948, 246945, 246942, 246937, 246930, 246928, 246871, 246870, 246867, 246865, 246864, 246859, 246858, 246855, 246851, 246847, 246846, 246845, 246844, 246810, 246809, 246808, 246807, 246806, 246805, 246804, 246766, 246765, 246763, 246760, 246759, 246758, 246757, 246755, 246751, 246750, 246676, 246675, 246671, 246648, 246639, 246583, 246575, 246568, 246567, 246553, 246543, 246540, 246495, 246493, 246488, 246487, 246434, 246433, 246431, 246428, 246424, 246392, 246391, 246390, 246276, 246275, 246272, 246271, 246225, 246222, 246220, 246217, 246187, 246185, 246182, 246181, 246180, 246178, 246153, 246152, 246151, 246148, 246115, 246113, 246089, 246087, 246053, 246052, 246049, 246048, 246042, 246037, 246036, 246012, 246003, 246001, 245996, 245963, 245954, 245952, 245949, 245923, 245833, 245831, 245829, 245793, 245785, 245775, 245766, 245759, 245752, 245738, 245731, 245729, 245705, 245702, 245700, 245692, 245683, 245554, 245545, 245544, 245543, 245542, 245540, 245535, 245507, 245505, 245504, 245501, 245497, 245496, 245454, 245453, 245452, 245450, 245446, 245441, 245439, 245411, 245410, 245409, 245407, 245401, 245397, 245396, 245353, 245349, 245347, 245346, 245345, 245343, 245341, 245259, 245256, 245253, 245233, 245232, 245231, 245230, 245152, 245151, 245148, 245146, 245145, 245068, 245066, 245064, 245061, 244983, 244982, 244980, 244975, 244972, 244918, 244917, 244911, 244889, 244827, 244824};
  for(int i=0;i<183;i++)
    if(runList[i]==RunNr) return i+1;
  return 0;
}


//________________________________________________________________________
void AliAnalysisTaskHFEIPCorrection::Process(AliAODEvent *const aodEvent)
{
  // Main loop
  // Called for each event
  EventSelectionSteps->Fill(4);
  //if(!(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & (AliVEvent::kCentral | AliVEvent::kSemiCentral | AliVEvent::kMB))) //return;
  EventSelectionSteps->Fill(5);

  if (!aodEvent) {
    Printf("ERROR: aodEvent not available");
    //return;
  }
  AliInputEventHandler* handler = dynamic_cast<AliInputEventHandler*>( AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  if(!(handler)) printf("AOD inputhandler not available \n");


  AliPIDResponse *pid = NULL;
  if(handler){
    pid = handler->GetPIDResponse();
  } else {
    AliError("No Handler");
  }
  if(!pid){
    AliError("No PID response");
    return;
  }
  
    if(!fExtraCuts){
    fExtraCuts = new AliHFEextraCuts("hfeExtraCuts","HFE Extra Cuts");
  }
  fExtraCuts->SetRecEventInfo(aodEvent);

  /*AliMCEventHandler* MCEventHandler = (AliMCEventHandler*)AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler();
  //AliMCEvent* fMCEvent=0;
  if(MCEventHandler)
  fMCEvent = MCEventHandler->MCEvent();*/

  //Float_t centrality = -1.;  // In Run2, centrality is estimated differently
  //AliCentrality *hicent = aodEvent->GetCentrality();
  //centrality = hicent->GetCentralityPercentile("V0M");

Float_t centrality = -1;
Float_t fV0Cent = -999.;
Float_t fV0CentCalib = -999.;
 
AliMultSelection *MultSelection = 0x0;
MultSelection = (AliMultSelection*)aodEvent->FindListObject("MultSelection");
if(!MultSelection){
  AliWarning("AliMultSelection object not found!");
}else{
  fV0Cent = MultSelection->GetMultiplicityPercentile("V0M", false);
  fV0CentCalib = MultSelection->GetMultiplicityPercentile("V0M", true);
  centrality = fV0CentCalib;
}

  const AliAODVertex *vertex = aodEvent->GetPrimaryVertex();
  const AliAODVertex *vertexSPD = aodEvent->GetPrimaryVertexSPD();
  Double_t vcov[6];
  vertex->GetCovMatrix(vcov);
  Double_t vtx[3];
  Double_t vtxSPD[3];
  vertex->GetXYZ(vtx);
  vertexSPD->GetXYZ(vtxSPD);

  AliAODVertex *correctedVertex;
  Bool_t madecorvtx = kFALSE;

  bool analyzeEvent=(TMath::Sqrt(vcov[5]) < 0.25 && TMath::Abs(vtx[2])<10. && TMath::Abs(vtx[2] - vtxSPD[2]) < 0.5 && centrality <= 100.);
  //hfetrackCuts->SetRecEvent(aodEvent);

   


   if(centrality>=20.0 && centrality<=40.0)
   {
      EventSelectionSteps->Fill(0);
      if(TMath::Abs(vtx[2])<10.)
      {
        EventSelectionSteps->Fill(1);
        if(TMath::Abs(vtx[2] - vtxSPD[2]) < 0.5)
        {
          EventSelectionSteps->Fill(2);
          if(TMath::Sqrt(vcov[5])) EventSelectionSteps->Fill(3);
        }
      }
   }

  ULong_t status;
  AliEventplane* vEPa;
  Double_t qVx, qVy;
  double epCorrArray[2];
  double epCorr;
  Double_t IP=0.;

  AliAODv0 * v0;
  AliAODTrack* V0Daughter[2];
  Int_t V0MotherPdg, V0Daughter1Pdg, V0Daughter2Pdg;
  double recoRadius=0.;
  Double_t IPCorrected = 0.;
  Double_t IPCorrectedChField = 0.;
  Int_t RunBin = ReturnRunBin(aodEvent->GetRunNumber());

  bool EPFlatteningReject = false;
  Float_t V0PlanePhi;
  Float_t V0APlanePhi;
  Float_t V0CPlanePhi;

  if(analyzeEvent)
  {
    vEPa = fInputEvent->GetEventplane();
    V0PlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,10,2,qVx,qVy));
    if(V0PlanePhi>TMath::Pi()) V0PlanePhi -=TMath::Pi();
    V0APlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,8,2,qVx,qVy));
    if(V0APlanePhi > TMath::Pi()) V0APlanePhi = V0APlanePhi - TMath::Pi();
    V0CPlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,9,2,qVx,qVy));
    if(V0CPlanePhi > TMath::Pi()) V0CPlanePhi = V0CPlanePhi - TMath::Pi();
    double rndm = fRd->Rndm();
    if(centrality>=20.0 && centrality<=50.0)
    {
        if(V0PlanePhi<0) V0PlanePhi+= 3.14159;
        if(V0PlanePhi>3.14159) V0PlanePhi-= 3.14159; // Just to be safe
        if(rndm>0.888359/(0.884154+0.171683*TMath::Gaus(V0PlanePhi,0.609013, 0.705609)+0.0634041*TMath::Erf((V0PlanePhi-1.99791)/0.492068))) EPFlatteningReject = true;
    }
  }

  if(analyzeEvent && !EPFlatteningReject)
  {
      //correctedVertex = CorrectVertex(aodEvent, vtx[2]); // created without problematic ITS regions
      //madecorvtx = true;
      //Double_t vtxcorr[3];
      //correctedVertex->GetXYZ(vtxcorr);
      /*vEPa = fInputEvent->GetEventplane();
      Float_t V0PlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,10,2,qVx,qVy));
      if(V0PlanePhi>TMath::Pi()) V0PlanePhi -=TMath::Pi();
      Float_t V0APlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,8,2,qVx,qVy));
      if(V0APlanePhi > TMath::Pi()) V0APlanePhi = V0APlanePhi - TMath::Pi();
      Float_t V0CPlanePhi = TVector2::Phi_0_2pi(vEPa->CalculateVZEROEventPlane(fInputEvent,9,2,qVx,qVy));
      if(V0CPlanePhi > TMath::Pi()) V0CPlanePhi = V0CPlanePhi - TMath::Pi();*/
    

      EPCent->Fill(V0PlanePhi, centrality);
      EPCentV0A->Fill(V0APlanePhi, centrality);
      EPCentV0C->Fill(V0CPlanePhi, centrality);
      //EPCentCorrected->Fill(epCorr, centrality);
        
    if(centrality>=20.0 && centrality<=40.0)
    {
      EP2040->Fill(V0PlanePhi);
      //EP2040Corrected->Fill(epCorr);
      EP2040V0A->Fill(V0APlanePhi);
      EP2040V0C->Fill(V0CPlanePhi);
    }
      double DPhi =0.;
      Float_t dcaxy = -999., dcaz = -999.;
      Double_t dcaErr, dcaxyD;
      Double_t dcaxyC;
      AliAODTrack *track = 0x0;
      for(Int_t itrack = 0; itrack < fInputEvent->GetNumberOfTracks(); itrack++)
      {
        // Run track loop
        track = dynamic_cast<AliAODTrack *>(fInputEvent->GetTrack(itrack));
        if(!track) continue;
        if(!PassesTrackCuts(track)) continue;
        if(PassesElectronPID(track, pid))
        {
          if(centrality>=20.0 && centrality<=50.0) TPCnSigma->Fill(track->Pt(),pid->NumberOfSigmasTPC(track, AliPID::kElectron));
          DPhi = track->Phi() - V0PlanePhi - TMath::Pi();
          while(DPhi<0.) DPhi += TMath::Pi();
          
          fExtraCuts->GetImpactParameters((AliVTrack *)track,dcaxy,dcaz);
          fExtraCuts->GetHFEImpactParameters((AliVTrack *)track,dcaxyD,dcaErr);
          IP = dcaxyD*track->Charge()*TMath::Sign(1.,aodEvent->GetMagneticField());
          GetCorrectedImpactParameter(aodEvent, track, vtx[2], IPCorrected);
          IPCorrectedChField = IPCorrected *track->Charge()*TMath::Sign(1.,aodEvent->GetMagneticField());
          if(centrality>=20.0 && centrality<=50.0) fIPData->Fill(track->Pt(), IPCorrectedChField);
          if(track->Pt() > 0.5)
          {
            DeltaPhi->Fill(DPhi);
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
            {
              if(centrality>=20.0 && centrality<=40.0) fpTIP2040IP->Fill(track->Pt(), IPCorrectedChField);
              if(centrality>=30.0 && centrality<=50.0) fpTIP3050IP->Fill(track->Pt(), IPCorrectedChField);
            }
            else
            {
              if(centrality>=20.0 && centrality<=40.0) fpTIP2040OOP->Fill(track->Pt(), IPCorrectedChField);
              if(centrality>=30.0 && centrality<=50.0) fpTIP3050OOP->Fill(track->Pt(), IPCorrectedChField);
            }
          }
        }
        if(PassesPionPID(track, pid) && centrality>=0.0 && centrality<=60.0) // To avoid calling the IP calculation for all tracks
        {
          GetCorrectedImpactParameter(aodEvent, track, vtx[2], IPCorrected);
          IPCorrectedChField = IPCorrected *track->Charge()*TMath::Sign(1.,aodEvent->GetMagneticField());
          fDCAHadrons->Fill(track->Pt(), IPCorrectedChField);
        }
        if((PassesPionPID(track, pid) || PassesKaonPID(track, pid)) && track->Pt() > 0.5) // To avoid calling the IP calculation for all tracks
        {
          fExtraCuts->GetHFEImpactParameters((AliVTrack *)track,dcaxyD,dcaErr);
          //GetTrackImpactParameter(aodEvent, track, correctedVertex, dcaxyC);
          IP = dcaxyD*track->Charge()*TMath::Sign(1.,aodEvent->GetMagneticField());
          GetCorrectedImpactParameter(aodEvent, track, vtx[2], IPCorrected);
          if(PassesPionPID(track, pid))
          {
            if(centrality>=0.0 && centrality<=80.0 && track->Pt()>1.)
            {
              fDCAPhiZHadrons->Fill(track->Phi(), dcaxyD, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              if(aodEvent->GetRunNumber() <= 246276) fDCAPhiZHadronsEarlyRuns->Fill(track->Phi(), dcaxyD, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              else  fDCAPhiZHadronsLateRuns->Fill(track->Phi(), dcaxyD, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              fDCAPhiZHadronsC->Fill(track->Phi(), IPCorrected, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              if(IsInMisalignedRegion(track, vtx[2])>0)fDCARegionRun->Fill(dcaxyD, IsInMisalignedRegion(track, vtx[2]) ,RunBin);
              else fDCARegionRun->Fill(dcaxyD, 5 ,RunBin);
            }
            if(centrality>=0.0 && centrality<=80.0 && track->Pt()>0.5)
            {
              fpTPhiZHadrons->Fill(track->Phi(), track->Pt(), vtx[2]+7.*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              if(aodEvent->GetRunNumber() <= 246276) fDCAPhipTHadronsEarlyRuns->Fill(dcaxyD, track->Phi(), track->Pt());
              else  fDCAPhipTHadronsLateRuns->Fill(dcaxyD, track->Phi(), track->Pt());
              fDCAPhipTHadrons->Fill(dcaxyD, track->Phi(), track->Pt());
              fDCAPhipTHadronsC->Fill(IPCorrected, track->Phi(), track->Pt());
            }
            if(centrality>=20.0 && centrality<=50.0) fDCAWErrHadrons->Fill(track->Pt(), IP, dcaxyD/dcaErr);
            fDCAHadronsFineBins->Fill(track->Pt(), IP, centrality);
          }
          if(PassesKaonPID(track, pid))
          {
            if(centrality>=0.0 && centrality<=80.0 && track->Pt()>1.)
            {
              fDCAPhiZKaons->Fill(track->Phi(), dcaxyD, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
              fDCAPhiZKaonsC->Fill(track->Phi(), IPCorrected, vtx[2]+4.5*TMath::Tan(TMath::Pi()/2.-2.*TMath::ATan(TMath::Exp(-track->Eta()))));
            }
            if(centrality>=0.0 && centrality<=80.0 && track->Pt()>0.5)
            {
              fDCAPhipTKaons->Fill(dcaxyD, track->Phi(), track->Pt());
              fDCAPhipTKaonsC->Fill(IPCorrected, track->Phi(), track->Pt());
            }
            if(centrality>=20.0 && centrality<=50.0) fDCAKaons->Fill(track->Pt(), IP);
            if(centrality>=20.0 && centrality<=50.0) fDCAWErrKaons->Fill(track->Pt(), IP, dcaxyD/dcaErr);
            fDCAKaonsFineBins->Fill(track->Pt(), IP, centrality);
          }
        }
      }
    fAODV0Cuts->SetEvent(aodEvent);
    for(int i= 0;i<aodEvent->GetNumberOfV0s();i++)
    {
      //

      v0 = aodEvent->GetV0(i);
      //if(!PassesTrackCuts(track));
      if(fAODV0Cuts->ProcessV0(v0, V0MotherPdg, V0Daughter1Pdg, V0Daughter2Pdg))
      {
        recoRadius = v0->RadiusSecVtx();
        if(TMath::Abs(V0MotherPdg)==310 && recoRadius>0.5)
        {
          V0Daughter[0] = dynamic_cast<AliAODTrack *> (v0->GetSecondaryVtx()->GetDaughter(0)); // This is how to get the daughter particles in AODs, apparently
          V0Daughter[1] = dynamic_cast<AliAODTrack *> (v0->GetSecondaryVtx()->GetDaughter(1));

          

          if(PassesMinimalTrackCuts(V0Daughter[0]))
          {
            fPionV0pTRNoCuts->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);
            DPhi = V0Daughter[0]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
              fPionV0pTRNoCutsIP->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);
            else
              fPionV0pTRNoCutsOOP->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);

          }
          if(PassesMinimalTrackCuts(V0Daughter[1]))
          {
            fPionV0pTRNoCuts->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
            DPhi = V0Daughter[1]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
              fPionV0pTRNoCutsIP->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
            else
              fPionV0pTRNoCutsOOP->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
          }
          if(PassesITSTrackCuts(V0Daughter[0]))
          {
            fPionV0pTRWithCuts->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);
            DPhi = V0Daughter[0]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
              fPionV0pTRWithCutsIP->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);
            else
              fPionV0pTRWithCutsOOP->Fill(V0Daughter[0]->Pt(), recoRadius, centrality);
          }
          if(PassesITSTrackCuts(V0Daughter[1]))
          {
            fPionV0pTRWithCuts->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
            DPhi = V0Daughter[1]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
              fPionV0pTRWithCutsIP->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
            else
              fPionV0pTRWithCutsOOP->Fill(V0Daughter[1]->Pt(), recoRadius, centrality);
          }
          if(centrality>=20.0 && centrality<=50.0){
          if(PassesMinimalTrackCuts(V0Daughter[0])) fPionV0pTTPC->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kPion));
          if(PassesMinimalTrackCuts(V0Daughter[1])) fPionV0pTTPC->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kPion));
          if(PassesMinimalTrackCuts(V0Daughter[0]) && V0Daughter[0]->GetTPCNcls()>=110 && V0Daughter[0]->GetTPCsignalN()>=80) fPionV0pTTPCWithCuts->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kPion));
          if(PassesMinimalTrackCuts(V0Daughter[1]) && V0Daughter[1]->GetTPCNcls()>=110 && V0Daughter[1]->GetTPCsignalN()>=80) fPionV0pTTPCWithCuts->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kPion));}
        }

        // TOF eff occupancy dependence
        if(TMath::Abs(V0MotherPdg)==22 && centrality>=20.0 && centrality<=40.0)
        {
          V0Daughter[0] = dynamic_cast<AliAODTrack *> (v0->GetSecondaryVtx()->GetDaughter(0)); // This is how to get the daughter particles in AODs, apparently
          V0Daughter[1] = dynamic_cast<AliAODTrack *> (v0->GetSecondaryVtx()->GetDaughter(1));

          if(PassesTrackCuts(V0Daughter[0]))
          {
            DPhi = V0Daughter[0]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
            {
              fPionV0pTTPCIP->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[0], AliPID::kElectron))<3.)
                fPionV0pTTPCIPWTOF->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
            }
            else
            {
              fPionV0pTTPCOOP->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[0], AliPID::kElectron))<3.)
                fPionV0pTTPCOOPWTOF->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
            }
          }

          if(PassesTrackCuts(V0Daughter[1]))
          {
            DPhi = V0Daughter[1]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
            {
              fPionV0pTTPCIP->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[1], AliPID::kElectron))<3.)
                fPionV0pTTPCIPWTOF->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
            }
            else
            {
              fPionV0pTTPCOOP->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[1], AliPID::kElectron))<3.)
                fPionV0pTTPCOOPWTOF->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
            }
          }

          if(PassesTrackCutsNoFirst(V0Daughter[0]))
          {
            DPhi = V0Daughter[0]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
            {
              fPionV0pTTPCIPnoFirst->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[0], AliPID::kElectron))<3.)
                fPionV0pTTPCIPWTOFnoFirst->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
            }
            else
            {
              fPionV0pTTPCOOPnoFirst->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[0], AliPID::kElectron))<3.)
                fPionV0pTTPCOOPWTOFnoFirst->Fill(V0Daughter[0]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[0], AliPID::kElectron));
            }
          }

          if(PassesTrackCutsNoFirst(V0Daughter[1]))
          {
            DPhi = V0Daughter[1]->Phi() - V0PlanePhi - TMath::Pi();
            while(DPhi<0.) DPhi += TMath::Pi();
            if(DPhi < TMath::Pi()/4. || DPhi > TMath::Pi()*3./4.) // IP
            {
              fPionV0pTTPCIPnoFirst->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[1], AliPID::kElectron))<3.)
                fPionV0pTTPCIPWTOFnoFirst->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
            }
            else
            {
              fPionV0pTTPCOOPnoFirst->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
              if(TMath::Abs(pid->NumberOfSigmasTOF(V0Daughter[1], AliPID::kElectron))<3.)
                fPionV0pTTPCOOPWTOFnoFirst->Fill(V0Daughter[1]->Pt(), pid->NumberOfSigmasTPC(V0Daughter[1], AliPID::kElectron));
            }
          }

        }

      }
    }
  }
  
  //if(madecorvtx) delete correctedVertex;
  PostData(1, fOutputContainer);
}

//________________________________________________________________________
void AliAnalysisTaskHFEIPCorrection::Terminate(const Option_t *)
{
//   treeoutput->Write();

}
