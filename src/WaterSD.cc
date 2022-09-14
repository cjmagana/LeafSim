#include "WaterSD.hh"
#include "G4SDManager.hh"

WaterSD::WaterSD(G4String name) :
  G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
  G4String HCname = "waterCollection";
  collectionName.insert(HCname);
}

WaterSD::~WaterSD() {
}

void WaterSD::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection = new WaterHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0) {
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
    hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool WaterSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();
  G4ThreeVector position = preStepPoint->GetPosition();

  WaterHit* hit = new WaterHit(copyNo,position,edep,hitTime);
  fHitsCollection->insert(hit);

  return true;
}

