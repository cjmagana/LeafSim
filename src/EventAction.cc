#include "EventAction.hh"
#include "WaterHit.hh"
#include "PhantomHit.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <map>
#include <assert.h>

EventAction::EventAction()
: G4UserEventAction(),
  fLeaf1ID(-1),
  fLeaf2ID(-1),
  fPhantomHCID(-1),
  fPhantomEdep()
{
  for(int i = 0;i<1600;i++) fWaterHCID[i] = -1;

  // Set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  //intialize the vectors
  fPhantomEdep.resize(100, 0.);
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    if (fPhantomHCID==-1)  {
      G4SDManager* sdManager = G4SDManager::GetSDMpointer();
      fPhantomHCID = sdManager->GetCollectionID("Phantom/PhantomColl");

      for(int i = 0;i<1600;i++) {
        char name[256];
        sprintf(name,"waterDetector%d/waterCollection",i+1);
        fWaterHCID[i] = sdManager->GetCollectionID(name);

      }
    }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
   // Print on screen the hits

   G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce)
    {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl;
        G4Exception("EventAction::EndOfEventAction()",
                    "Code001", JustWarning, msg);
        return;
    }

    //Setup Hits Collections
    
    PhantomHitsCollection* PhantomHC
        = static_cast<PhantomHitsCollection*>(hce->GetHC(fPhantomHCID));

    WaterHitsCollection* hWaterHC[1600];
    bool notFoundWater = false; 
    for(int i = 0; i<1600 ;i++) {
	hWaterHC[i] = static_cast<WaterHitsCollection*>(hce->GetHC(fWaterHCID[i]));
        if(!hWaterHC[i]) notFoundWater = true;
  }

   if ( (!PhantomHC) || notFoundWater) 
   {
	G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found." << G4endl;
        G4Exception("EventAction::EndOfEventAction()",
                    "Code001", JustWarning, msg);
        return;
    }

    /////////////////
    // Fill ntuple // 
    /////////////////

    // Get analysis manager

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Fill ntuple
    
    // PhantomHits
    analysisManager->FillNtupleIColumn(0, PhantomHC->entries());

    // PhantomEnergy
    G4int totalPhantomHit = 0;
    G4double totalPhantomE = 0.;
    for (G4int i=0;i<100;i++) 
    {
        PhantomHit* hit = (*PhantomHC)[i];
        G4double eDep = hit->GetEdep();
        if (eDep>0.)
        {
            totalPhantomHit++;
            totalPhantomE += eDep;
        }
        fPhantomEdep[i] = eDep;
    }
    analysisManager->FillNtupleDColumn(1, totalPhantomE);



    // Water Voxel Info

  G4double waterHitX;
  G4double waterHitY;
  G4double waterHitZ;
  G4int    waterHitDet;
  G4double waterHitE;
  G4double waterHitT;

  G4double totalWaterHit = 0.;
  G4double totalWaterE   = 0.;


  for (int i = 0;i<1600;i++) {
    for (int j = 0;j<hWaterHC[i]->entries();j++) {
      waterHitX = (*hWaterHC[i])[j]->GetPosition().x();
      waterHitY = (*hWaterHC[i])[j]->GetPosition().y();
      waterHitZ = (*hWaterHC[i])[j]->GetPosition().z();
      waterHitE = (*hWaterHC[i])[j]->GetEnergy();

      waterHitT = (*hWaterHC[i])[j]->GetTime();
      waterHitDet = (i+1);

      analysisManager->FillNtupleDColumn(2, waterHitX);
      analysisManager->FillNtupleDColumn(3, waterHitY);
      analysisManager->FillNtupleDColumn(4, waterHitZ);
      analysisManager->FillNtupleDColumn(5, waterHitE);
      analysisManager->FillNtupleDColumn(6, waterHitT);
      analysisManager->FillNtupleIColumn(7, waterHitDet);

      if (waterHitE>0.)
      {
	totalWaterHit++;
	totalWaterE += waterHitE;
      }
    } 
  }

  analysisManager->FillNtupleDColumn(8, totalWaterE);

  analysisManager->AddNtupleRow();

    // Print diagnostics

   G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( printModulo==0 || event->GetEventID() % printModulo != 0) return;

    G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
    G4cout << G4endl
           << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
           << primary->GetG4code()->GetParticleName()
           << " " << primary->GetMomentum() << G4endl;

    // Phantom printout
    G4cout << "Phantom has " << totalPhantomHit << " hits. Total Edep is "
    << totalPhantomE/MeV << " (MeV)" << G4endl;
    G4cout << "WaterDet has " << totalWaterHit << " hits. Total Edep is "
    << totalPhantomE/MeV << " (MeV)" << G4endl;

}
