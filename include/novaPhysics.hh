
// Physics List Header File 
//   novaPhysics.hh generated by Geant4 Physics Editor at Fri Oct 28 17:12:28 CDT 2005  


 #ifndef  novaPhysics_h
#define  novaPhysics_h  1

#include "G4VUserPhysicsList.hh"
class  novaPhysics:  public G4VUserPhysicsList
   {      public:
      novaPhysics();
      virtual ~novaPhysics();

  protected:
     virtual void ConstructParticle();
     virtual void ConstructProcess();
     virtual void ConstructOp();
     virtual void SetCuts();
   protected:
     virtual void ConstructBosons();
     virtual void ConstructLeptons();
     virtual void ConstructMesons();
     virtual void ConstructBaryons();
   protected:
     virtual void ConstructGeneral();
     virtual void ConstructEM();
     virtual void ConstructAllShortLiveds();
};
#endif

