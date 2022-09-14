#include "CellParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

CellParameterisation::CellParameterisation()
: G4VPVParameterisation()
{
    for (G4int copyNo=0;copyNo<100;copyNo++)
    {
        G4int column = copyNo / 10;
        G4int row = copyNo % 10;
        fXCell[copyNo] = (column-4)*40.*cm - 20.*cm;
        fYCell[copyNo] = (row-4)*40.*cm - 20.*cm;
    }
}

CellParameterisation::~CellParameterisation()
{}

void CellParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
    physVol->SetTranslation(G4ThreeVector(fXCell[copyNo],fYCell[copyNo],0.));
}

