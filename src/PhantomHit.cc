#include "PhantomHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<PhantomHit>* PhantomHitAllocator;

PhantomHit::PhantomHit()
: G4VHit(), fCellID(-1), fEdep(0.), fPos(0), fPLogV(0)
{}

PhantomHit::PhantomHit(G4int z)
:  G4VHit(), fCellID(z), fEdep(0.), fPos(0), fPLogV(0)
{}

PhantomHit::~PhantomHit()
{}

PhantomHit::PhantomHit(const PhantomHit &right)
: G4VHit() {
    fCellID = right.fCellID;
    fEdep = right.fEdep;
    fPos = right.fPos;
    fRot = right.fRot;
    fPLogV = right.fPLogV;
}

const PhantomHit& PhantomHit::operator=(const PhantomHit &right)
{
    fCellID = right.fCellID;
    fEdep = right.fEdep;
    fPos = right.fPos;
    fRot = right.fRot;
    fPLogV = right.fPLogV;
    return *this;
}

int PhantomHit::operator==(const PhantomHit &right) const
{
    return (fCellID==right.fCellID);
}

void PhantomHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager&&(fEdep>0.))
    {
        // Draw phantom cell with a color corresponding to its energy deposit

	G4Transform3D trans(fRot.inverse(),fPos);
        G4VisAttributes attribs;
        const G4VisAttributes* pVA = fPLogV->GetVisAttributes();
        if (pVA) attribs = *pVA;
        G4double rcol = fEdep/(5.*MeV);
        if (rcol>1.) rcol = 1.;
        if (rcol<0.4) rcol = 0.4;
        G4Colour colour(rcol,0.,0.);
        attribs.SetColour(colour);
        attribs.SetForceSolid(true);
        pVVisManager->Draw(*fPLogV,attribs,trans);
    }
}

const std::map<G4String,G4AttDef>* PhantomHit::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("PhantomHit",isNew);

    if (isNew) {
        (*store)["HitType"]
          = G4AttDef("HitType","Hit Type","Physics","","G4String");

        (*store)["ID"]
          = G4AttDef("ID","ID","Physics","","G4int");

        (*store)["Energy"]
          = G4AttDef("Energy", "Energy Deposited", "Physics", "G4BestUnit",
                     "G4double");
	(*store)["Pos"]
          = G4AttDef("Pos", "Position", "Physics","G4BestUnit",
                     "G4ThreeVector");

        (*store)["LVol"]
          = G4AttDef("LVol","Logical Volume","Physics","","G4String");
    }
    return store;
}

std::vector<G4AttValue>* PhantomHit::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values
      ->push_back(G4AttValue("HitType","PhantomHit",""));
    values
      ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fCellID),""));
    values
      ->push_back(G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));
    values
      ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

    if (fPLogV)
        values->push_back(G4AttValue("LVol",fPLogV->GetName(),""));
    else
        values->push_back(G4AttValue("LVol"," ",""));

    return values;
}

void PhantomHit::Print()
{
    G4cout << "  Cell[" << fCellID << "] " << fEdep/MeV << " (MeV)" << G4endl;
}

