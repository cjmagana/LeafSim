#include "MFDEventAction.hh"
#include "G4SDManager.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

MFDEventAction::MFDEventAction(const std::vector<G4String> mfdName): G4Run()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4int Nmfd = mfdName.size();
  for ( G4int idet = 0; idet < Nmfd ; idet++){
    G4String detName = mfdName[idet];
    G4MultiFunctionalDetector* mfd =
      (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));

    if ( mfd ){
	for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); icol++){
	  G4VPrimitiveScorer* waterDetMFD=mfd->GetPrimitive(icol);

          G4String collectionName = waterDetMFD->GetName();
          G4String fullCollectionName = detName+"/"+collectionName;
          G4int    collectionID = SDman->GetCollectionID(fullCollectionName);
	  if ( collectionID >= 0 ){
              G4cout << "++ "<<fullCollectionName<< " id " << collectionID << G4endl;
	    theCollName.push_back(fullCollectionName);
            theCollID.push_back(collectionID);
            theRunMap.push_back(new G4THitsMap<G4double>(detName,collectionName));
           }
	  else{
                G4cout << "** collection " << fullCollectionName << " not found. "<<G4endl;
           }
        }
    }
  }
}

MFDEventAction::~MFDEventAction() 
{
  G4int Nmap = theRunMap.size();
  for ( G4int i = 0; i < Nmap; i++){
    if(theRunMap[i] ) theRunMap[i]->clear();
  }
  theCollName.clear();
  theCollID.clear();
  theRunMap.clear();
}

void MFDEventAction::RecordEvent(const G4Event* aEvent)
{
  numberOfEvent++;

  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  G4int Ncol = theCollID.size();
  for ( G4int i = 0; i < Ncol ; i++ ){ 
    G4THitsMap<G4double>* EvtMap=0;
    if ( theCollID[i] >= 0 ){          
      EvtMap = (G4THitsMap<G4double>*)(HCE->GetHC(theCollID[i]));
    }else{
      G4cout <<" Error EvtMap Not Found "<< i << G4endl;
    }
    if ( EvtMap )  {
      *theRunMap[i] += *EvtMap;
    }
  }
}

G4THitsMap<G4double>* MFDEventAction::GetHitsMap(const G4String& detName,
                                         const G4String& colName){
    G4String fullName = detName+"/"+colName;
    return GetHitsMap(fullName);
}

G4THitsMap<G4double>* MFDEventAction::GetHitsMap(const G4String& fullName){
    G4int Ncol = theCollName.size();
    for ( G4int i = 0; i < Ncol; i++){
        if ( theCollName[i] == fullName ){
            return theRunMap[i];
        }
    }
    return NULL;
}

void MFDEventAction::DumpAllScorer(){
  
  G4int n = GetNumberOfHitsMap();
  
  for ( G4int i = 0; i < n ; i++ ){
    G4THitsMap<G4double>* RunMap =GetHitsMap(i);
    if ( RunMap ) {
      G4cout << " PrimitiveScorer RUN "
             << RunMap->GetSDname() <<","<< RunMap->GetName() << G4endl;
      G4cout << " Number of entries " << RunMap->entries() << G4endl;
      std::map<G4int,G4double*>::iterator itr = RunMap->GetMap()->begin();
      for(; itr != RunMap->GetMap()->end(); itr++) {
        G4cout << "  copy no.: " << itr->first
               << "  Run Value : " << *(itr->second)
               << G4endl;
      }
    }
  }
}
