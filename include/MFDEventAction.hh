#ifndef MFDEventAction_h
#define MFDEventAction_h 1

#include "G4Run.hh"
#include "G4Event.hh"

#include "G4THitsMap.hh"
#include <vector>

class MFDEventAction : public G4Run {

public:

  MFDEventAction(const std::vector<G4String> mfdName);
  virtual ~MFDEventAction();

  virtual void RecordEvent(const G4Event*);

  G4int GetNumberOfHitsMap() const {return theRunMap.size();}

  G4THitsMap<G4double>* GetHitsMap(G4int i){return theRunMap[i];}
  G4THitsMap<G4double>* GetHitsMap(const G4String& detName,const G4String& colName);
  G4THitsMap<G4double>* GetHitsMap(const G4String& fullName);

  void DumpAllScorer();

private:
  std::vector<G4String>              theCollName;  
  std::vector<G4int>                 theCollID;    
  std::vector<G4THitsMap<G4double>*> theRunMap;    
};

#endif
