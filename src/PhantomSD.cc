#include "PhantomSD.hh"
#include "PhantomHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

PhantomSD::PhantomSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
    collectionName.insert("PhantomColl");
}

PhantomSD::~PhantomSD()
{}

void PhantomSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection
      = new PhantomHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fHCID,fHitsCollection);

    // fill cell detector hits with zero energy deposition
    for (G4int i=0;i<100;i++)
    {
        PhantomHit* hit = new PhantomHit(i);
        fHitsCollection->insert(hit);
    }
}

G4bool PhantomSD::ProcessHits(G4Step*step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep==0.) return true;

    G4TouchableHistory* touchable
      = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume* physical = touchable->GetVolume();
    G4int copyNo = physical->GetCopyNo();

    PhantomHit* hit = (*fHitsCollection)[copyNo];
    // check if it is first touch
    if (!(hit->GetLogV()))
    {
        // fill volume information
        hit->SetLogV(physical->GetLogicalVolume());
        G4AffineTransform transform
          = touchable->GetHistory()->GetTopTransform();
        transform.Invert();
        hit->SetRot(transform.NetRotation());
        hit->SetPos(transform.NetTranslation());
    }
    // add energy deposition
    hit->AddEdep(edep);

    return true;
}

