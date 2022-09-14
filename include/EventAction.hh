#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Types.hh"
#include <string>
#include <map>

#include <vector>

class EventAction : public G4UserEventAction
{
public:
   EventAction();
   virtual ~EventAction();

   virtual void BeginOfEventAction(const G4Event*);
   virtual void EndOfEventAction(const G4Event*);
   
   std::vector<G4double>& GetPhantomEdep() { return fPhantomEdep; }   

private:
    G4int fLeaf1ID;
    G4int fLeaf2ID;
    G4int fPhantomHCID;
    G4int fWaterHCID[1600];

    std::vector<G4double> fPhantomEdep;
};

#endif
