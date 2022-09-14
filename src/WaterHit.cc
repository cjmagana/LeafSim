#include "WaterHit.hh"

G4ThreadLocal G4Allocator<WaterHit>* WaterHitAllocator;

WaterHit::WaterHit(G4int ID, G4ThreeVector pos, G4double energy, G4double time) :
  G4VHit(), fID(ID), fPos(pos), fEnergy(energy), fTime(time) {

}

WaterHit::~WaterHit() {

}

