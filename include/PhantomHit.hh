#ifndef PhantomHit_h
#define PhantomHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class PhantomHit : public G4VHit
{
public:
    PhantomHit();
    PhantomHit(G4int z);
    PhantomHit(const PhantomHit &right);
    virtual ~PhantomHit();

    const PhantomHit& operator=(const PhantomHit &right);
    int operator==(const PhantomHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void SetCellID(G4int z) { fCellID = z; }
    G4int GetCellID() const { return fCellID; }

    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }
    
    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }

    void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
    const G4LogicalVolume* GetLogV() const { return fPLogV; }

private:
    G4int fCellID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fPLogV;
};

typedef G4THitsCollection<PhantomHit> PhantomHitsCollection;

extern G4ThreadLocal G4Allocator<PhantomHit>* PhantomHitAllocator;

inline void* PhantomHit::operator new(size_t)
{
    if (!PhantomHitAllocator)
        PhantomHitAllocator = new G4Allocator<PhantomHit>;
    return (void*)PhantomHitAllocator->MallocSingle();
}

inline void PhantomHit::operator delete(void* aHit)
{
    PhantomHitAllocator->FreeSingle((PhantomHit*) aHit);
}

#endif
