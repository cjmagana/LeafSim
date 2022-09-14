#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "StepMax.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

// Physic lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4Decay.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"

#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 1.*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  helIsRegisted  = false;
  bicIsRegisted  = false;
  biciIsRegisted = false;
  locIonIonInelasticIsRegistered = false;

  stepMaxProcess  = 0;

  pMessenger = new PhysicsListMessenger(this);

  SetVerboseLevel(1);

  // EM physics
  emPhysicsList = new G4EmStandardPhysics_option3(1);
  emName = G4String("emstandard_opt3");

//  emPhysicsList = new G4EmLivermorePhysics();
//  emName = G4String("LowE_Livermore");

  // Deacy physics and all particles
  decPhysicsList = new G4DecayPhysics();
}

PhysicsList::~PhysicsList()
{
  delete pMessenger;
  delete emPhysicsList;
  delete decPhysicsList;
  for(size_t i=0; i<hadronPhys.size(); i++) {delete hadronPhys[i];}
}

void PhysicsList::AddPackage(const G4String& name)
{
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys =factory.GetReferencePhysList(name);
  G4int i=0;
  const G4VPhysicsConstructor* elem= phys->GetPhysics(i);
  G4VPhysicsConstructor* tmp = const_cast<G4VPhysicsConstructor*> (elem);
  while (elem !=0)
        {
          RegisterPhysics(tmp);
          elem= phys->GetPhysics(++i) ;
          tmp = const_cast<G4VPhysicsConstructor*> (elem);
        }
    G4cout << "THE FOLLOWING PHYSICS PACKEGE LIST HAS BEEN ACTIVATED: "<<name<< G4endl;
}

void PhysicsList::ConstructParticle()
{
  decPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  // transportation
  //
  AddTransportation();

  // electromagnetic physics list
  //
  emPhysicsList->ConstructProcess();
  em_config.AddModels();

  // decay physics list
  //
  decPhysicsList->ConstructProcess();

  // hadronic physics lists
  for(size_t i=0; i<hadronPhys.size(); i++) {
    hadronPhys[i]->ConstructProcess();
  }

  // step limitation (as a full process)
  //
  AddStepMax();
}

void PhysicsList::AddPhysicsList(const G4String& name)
{

  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }
  if (name == emName) return;

  //////////////////////////////
  //   ELECTROMAGNETIC MODELS //
  //////////////////////////////

  if (name == "standard_opt3") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option3();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << G4endl;

 } else if (name == "LowE_Livermore") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePhysics();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;

 } else if (name == "LowE_Penelope") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmPenelopePhysics();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;

  ///////////////////////
  //   HADRONIC MODELS //
  ///////////////////////

  } else if (name == "elastic" && !helIsRegisted) {
    G4cout << "THE FOLLOWING HADRONIC ELASTIC PHYSICS LIST HAS BEEN ACTIVATED: G4HadronElasticPhysics()" << G4endl;
    hadronPhys.push_back( new G4HadronElasticPhysics());
    helIsRegisted = true;

  } else if (name == "binary" && !bicIsRegisted) {
    hadronPhys.push_back(new G4HadronInelasticQBBC());
    bicIsRegisted = true;
    G4cout << "THE FOLLOWING HADRONIC INELASTIC PHYSICS LIST HAS BEEN ACTIVATED: G4HadronInelasticQBBC()" << G4endl;

  } else if (name == "binary_ion" && !biciIsRegisted) {
    hadronPhys.push_back(new G4IonBinaryCascadePhysics());
    biciIsRegisted = true;
  
  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  stepMaxProcess = new StepMax();

  GetParticleIterator()->reset();
  while ((*GetParticleIterator())()){
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (stepMaxProcess->IsApplicable(*particle) && pmanager)
      {
        pmanager ->AddDiscreteProcess(stepMaxProcess);
      }
  }
}

void PhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}

void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

