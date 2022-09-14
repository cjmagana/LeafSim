#include "RunAction.hh"
//#include "MFDEventAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{
  // Create analysis manager
  // Choice of tech is done in Analysis.hh

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("out");

  // Create ntuple

  if ( fEventAction ) {
    analysisManager->CreateNtuple("out", "Hits");
    analysisManager->CreateNtupleIColumn("PhantomHits");
    analysisManager->CreateNtupleDColumn("PhantomEnergy"); 
    analysisManager->CreateNtupleDColumn("WaterHitX");
    analysisManager->CreateNtupleDColumn("WaterHitY");
    analysisManager->CreateNtupleDColumn("WaterHitZ");
    analysisManager->CreateNtupleDColumn("WaterHitE");
    analysisManager->CreateNtupleDColumn("WaterHitT");
    analysisManager->CreateNtupleIColumn("WaterHitDet");
    analysisManager->CreateNtupleDColumn("TotalWaterEnergy");

  analysisManager->FinishNtuple();
  }

}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open output file
  // Default file name is set in RunAction::RunAction()
  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // Save ntuple
 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

}
