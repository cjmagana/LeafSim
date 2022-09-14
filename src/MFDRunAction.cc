#include "MFDRunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "MFDEventAction.hh"

#include <fstream>
#include "g4root.hh"


MFDRunAction::MFDRunAction()
{;}

MFDRunAction::~MFDRunAction()
{;}

void MFDRunAction::BeginOfRunAction(const G4Run*)
{;}

G4Run*  MFDRunAction::GenerateRun(){

  std::vector<G4String> theSDNames(1,"MFDet");
  return new MFDEventAction(theSDNames);

}

void MFDRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  G4cout << " Number of Events Processed:"
         << NbOfEvents << " events. " <<G4endl;

  MFDEventAction* theRun = (MFDEventAction*)aRun;
  theRun->DumpAllScorer();

  G4int NhitsMap = theRun->GetNumberOfHitsMap();
  for ( G4int im = 0; im < NhitsMap ; im++ ){
    G4THitsMap<G4double>* RunMap =theRun->GetHitsMap(im);
    if ( RunMap ) {
      G4String filename = RunMap->GetName()+".root";
      std::ofstream  out(filename.data());
        for  ( G4int i = 0; i < 100; i++){
          G4double* unitAreaFlux   = (*RunMap)[i];
          if( !unitAreaFlux ) unitAreaFlux = new G4double(0.0);
          out << std::setw(5) << i << ",\t"
              << std::setw(5)<< *unitAreaFlux*mm*mm
              << ",\t"<< " /mm2 "<<G4endl;
        }
        out.close();
      }
    }
}
