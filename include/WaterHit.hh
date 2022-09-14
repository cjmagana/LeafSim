#ifndef WaterHit_h
#define WaterHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class WaterHit : public G4VHit {
public:
  WaterHit(G4int,G4ThreeVector,G4double,G4double);
  virtual ~WaterHit();

  G4int GetID() const {return fID;};
  G4double GetTime() const {return fTime;};
  G4double GetEnergy() const {return fEnergy;};
  G4ThreeVector GetPosition() const {return fPos;};

  inline void* operator new(size_t);
  inline void operator delete(void*);

private:
  G4int fID;
  G4ThreeVector fPos;
  G4double fEnergy;
  G4double fTime;

};

typedef G4THitsCollection<WaterHit> WaterHitsCollection;

extern G4ThreadLocal G4Allocator<WaterHit>* WaterHitAllocator;

inline void* WaterHit::operator new(size_t)
{
    if (!WaterHitAllocator) WaterHitAllocator = new G4Allocator<WaterHit>;
    return (void*)WaterHitAllocator->MallocSingle();
}

inline void WaterHit::operator delete(void* aHit)
{
    WaterHitAllocator->FreeSingle((WaterHit*) aHit);
}

#endif
