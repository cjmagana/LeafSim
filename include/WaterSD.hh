#ifndef WaterSD_h
#define WaterSD_h

#include "G4VSensitiveDetector.hh"
#include "WaterHit.hh"

class WaterSD : public G4VSensitiveDetector {
public:
  WaterSD(G4String name);
  virtual ~WaterSD();

  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

private:
  WaterHitsCollection* fHitsCollection;
  G4int fHCID;
};

#endif
