#ifndef WLSfiber_h
#define WLSfiber_h

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

class WLSfiber
{
public:
  WLSfiber();
  ~WLSfiber();
  //both of these functions return logical volumes containing core and cladding
  //creates a straight segement of WLS fiber
  G4LogicalVolume *BuildStraight(G4double radius,G4double length, G4double _curve_radius);
  //creates a curved segment of WLS fiber
  G4LogicalVolume *BuildTorus(G4double fibradius,G4double curveradius,G4double deltaphi);
private:

  G4Element* elementH;
  G4Element* elementC;
  G4Element* elementO;

  G4Material *Polystyrene;
  G4Material *PMMA;
  G4Material *Fluorinated_Polymer;
  G4Material *CoreMat;

  G4MaterialPropertiesTable *wlsMPT;
  G4MaterialPropertiesTable *innercladMPT;
  G4MaterialPropertiesTable *outercladMPT;

  G4LogicalVolume *l_fiber;
  G4LogicalVolume *l_core;
  G4LogicalVolume *l_cladding1;
  G4LogicalVolume *l_cladding2;

  G4LogicalVolume *l_cladding;
  G4LogicalVolume *l_torus;



  G4VPhysicalVolume *p_core;
  G4VPhysicalVolume *p_cladding1;
  G4VPhysicalVolume *p_cladding2;

  G4VPhysicalVolume *p_cladding;


  G4VisAttributes *yellow;
  G4VisAttributes *red;
  G4VisAttributes *blue;



};

#endif
