SteerAODAnalysisTaskPIDFluctuation(const Char_t *inputfilename, Int_t maxFiles = kMaxInt, Int_t maxEv = kMaxInt)
{

  /* include path for ACLic */
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_ROOT/TOF");
  /* load libraries */
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  /* build analysis task class */
  gROOT->LoadMacro("AliAnalysisTaskPIDFluctuation.cxx+g");

  /* setup input chain */
  TString str = inputfilename;
  const Char_t *filename;
  TChain *chain = new TChain("aodTree");
  if (str.EndsWith(".xml")) {
    TGrid::Connect("alien://");
    Info("", "reading data list from collection:");
    TGridCollection *coll gGrid->OpenCollection(inputfilename, maxFiles);
    coll->Reset();
    while (coll->Next()) {
      filename = coll->GetTURL();
      Info("", Form("%s", filename));
      chain->Add(filename);
    }
  }
  else if (str.EndsWith(".txt")) {
    Info("", "reading data list from text file:");
    ifstream is(inputfilename);
    Char_t buf[4096];
    while(!is.eof()) {
      is.getline(buf, 4096);
      if (is.eof()) break;
      chain->Add(buf);
      Info("", Form("%s", buf));
    }
    is.close();
  }
  else {
    Info("", "single file:");
    filename = inputfilename;
    Info("", Form("%s", filename));
    chain->Add(filename);
  }
  Info("", Form("chain is ready: %d events", chain->GetEntries()));

  /* create analysis manager */
  AliAnalysisManager *mgr = new AliAnalysisManager("PIDFluctuation");

  /* define input event handler */
  AliAODInputHandler *aodh = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodh);

  gROOT->LoadMacro("AddAnalysisTaskPIDFluctuation.C");
  AddAnalysisTaskPIDFluctuation(AliAODTrack::kTrkGlobal, 0., 10., -0.8, 0.8);
  AddAnalysisTaskPIDFluctuation(AliAODTrack::kTrkTPCOnly, 0., 10., -0.8, 0.8);
  AddAnalysisTaskPIDFluctuation(AliAODTrack::kTrkTPCOnly, 0.3, 1.5, -0.8, 0.8);
  AddAnalysisTaskPIDFluctuation(AliAODTrack::kTrkTPCOnly, 0.3, 1.5, -0.4, 0.4);

  /* start analysis */
  mgr->SetDebugLevel(0);
  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("local", chain, maxEv);

  /* create dummy file to tell we are done */
  gSystem->Exec("touch done");

}
