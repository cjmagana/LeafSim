#ifndef B5EmCalorimeterSD_h
#define B5EmCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "PhantomHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PhantomSD : public G4VSensitiveDetector
{

public:
    PhantomSD(G4String name);
    virtual ~PhantomSD();

    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

private:
    PhantomHitsCollection* fHitsCollection;
    G4int fHCID;
};

#endif
