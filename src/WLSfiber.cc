#include "WLSfiber.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "fstream"
#include "iostream"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4Torus.hh"
#include "G4UnionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

WLSfiber::WLSfiber()
{

  elementH = new G4Element( "Hydrogen", "H", 1. , 1.00794*g/mole );
  elementC = new G4Element( "Carbon", "C", 6. , 12.011*g/mole );
  elementO = new G4Element( "Oxygen", "O", 8. , 16.011*g/mole );


  Polystyrene = new G4Material("Polystyrene",  1.05*g/cm3, 2, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  Polystyrene->AddElement( elementH, 0.498 );
  Polystyrene->AddElement( elementC, 0.502 );

  PMMA = new G4Material("PMMA",  1.19*g/cm3, 3, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  PMMA->AddElement( elementH, 0.532 );
  PMMA->AddElement( elementC, 0.336 );
  PMMA->AddElement( elementO, 0.132 );

  Fluorinated_Polymer = new G4Material("Fluorinated_Polymer",  1.19*g/cm3, 3, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  Fluorinated_Polymer->AddElement( elementH, 0.532 );
  Fluorinated_Polymer->AddElement( elementC, 0.336 );
  Fluorinated_Polymer->AddElement( elementO, 0.132 );

  //2. Optical Parameters
  G4int wlsEntries = 0;
  G4int bulkEntries = 0;
  G4int pmmaEntries = 0;
  G4double wlsEnergy[500];
  G4double wlsEmit[500];
  G4double wlsAbsorb[500];
  G4double coreIndex[500];
  G4double pWavelength;
  G4double absorblength;
  G4double wls_absorblength;

  G4double bulkAbsorb[500];
  G4double pmmabulkAbsorb[500];
  G4double bulkEnergy[500];
  G4double pmmaEnergy[500];
  G4double cladIndex1[500];
  G4double cladIndex2[500];

  G4double coreIndexconst = 1.59;
  G4double cladIndexconst1 = 1.49;
  G4double cladIndexconst2 = 1.42;

  //read WLS absorption spectrum from WLSabsorb.cfg
  std::ifstream ReadWLSa;

  //  G4String WLSabsorb = "y11_abs_length.dat";

  //  ReadWLSa.open(WLSabsorb);
  ReadWLSa.open("y11_abs_length.dat");

  if (ReadWLSa.is_open()){
    while(!ReadWLSa.eof()){
      G4String filler;
      ReadWLSa >> pWavelength >> filler >> wls_absorblength;
      wlsEnergy[wlsEntries] = (1240/pWavelength)*eV;
      wlsAbsorb[wlsEntries] = 1.0*wls_absorblength*m;
      coreIndex[wlsEntries] = coreIndexconst;
      cladIndex1[wlsEntries] = cladIndexconst1;
      cladIndex2[wlsEntries] = cladIndexconst2;

      wlsEntries++;
    }
  }
  else
    //    G4cout<<"Error opening file: "<<WLSabsorb<<G4endl;  
    G4cout<<"Error opening file: "<< "y11_abs_length.dat" <<G4endl;  

  ReadWLSa.close();

  //create MaterialPropertiesTable for core

  wlsMPT = new G4MaterialPropertiesTable();
  wlsMPT->AddProperty("RINDEX",wlsEnergy,coreIndex,wlsEntries);
  wlsMPT->AddProperty("WLSABSLENGTH",wlsEnergy,wlsAbsorb,wlsEntries);
  wlsMPT->AddConstProperty("WLSTIMECONSTANT",12*ns);
 
  innercladMPT = new G4MaterialPropertiesTable();
  innercladMPT->AddProperty("RINDEX",wlsEnergy,cladIndex1,wlsEntries);

  outercladMPT = new G4MaterialPropertiesTable();
  outercladMPT->AddProperty("RINDEX",wlsEnergy,cladIndex2,wlsEntries);


  //read WLS emission spectrum from WLSemit.cfg
  wlsEntries = 0;
  std::ifstream ReadWLSe;
  //  G4String WLSemit = "WLSemit.cfg";
  //  G4String WLSemit="k27_emission.cfg";

  //  ReadWLSe.open(WLSemit);
  ReadWLSe.open("WLSemit.cfg");

  if(ReadWLSe.is_open()){
    while(!ReadWLSe.eof()){
      G4String filler;
      ReadWLSe >> pWavelength >> filler >> wlsEmit[wlsEntries];
      wlsEnergy[wlsEntries] = (1240/pWavelength)*eV;
      wlsEntries++;
    }
  }
  else
    //    G4cout<<"Error opening file: "<<WLSemit<<G4endl; 
    G4cout<<"Error opening file: "<< "WLSemit.cfg" <<G4endl; 

  ReadWLSe.close();

  wlsMPT->AddProperty("WLSCOMPONENT",wlsEnergy,wlsEmit,wlsEntries);
    wlsMPT->AddConstProperty("CONSTANTQUANTUMYIELD",0.7);



  //read bulk absorption spectrum from FiberBulkAbsorb.cfg
  bulkEntries = 0;
  std::ifstream ReadBulk;

  //  G4String Bulk = "fiberPSTabsorb.dat";

  //  ReadBulk.open(Bulk);
  ReadBulk.open("fiberPSTabsorb.dat");

  if(ReadBulk.is_open()){
    while(!ReadBulk.eof()){
      G4String filler;
      ReadBulk >> pWavelength >> filler >> absorblength;
      bulkEnergy[bulkEntries] = (1240/pWavelength)*eV;
      bulkAbsorb[bulkEntries] = 1.0*absorblength*m;
      bulkEntries++;
    }
  }
  else
    //    G4cout<<"Error opening file: "<<Bulk<<G4endl; 
    G4cout<<"Error opening file: "<< "fiberPSTabsorb.dat" <<G4endl; 

  ReadBulk.close();

  //PMMA
  pmmaEntries = 0;
  std::ifstream Read_pmma_Bulk;
  //  G4String pmma_Bulk = "PMMABulkAbsorb.dat";


  //  Read_pmma_Bulk.open(pmma_Bulk);
  Read_pmma_Bulk.open("PMMABulkAbsorb.dat");

  if(Read_pmma_Bulk.is_open()){
    while(!Read_pmma_Bulk.eof()){
      G4String filler;
      Read_pmma_Bulk >> pWavelength >> filler >> absorblength;
      pmmaEnergy[pmmaEntries] = (1240/pWavelength)*eV;
      pmmabulkAbsorb[pmmaEntries] = 1.0*absorblength*m;
      pmmaEntries++;
    }
  }
  else
    //    G4cout << "Error opening file: " << Bulk << G4endl; 
    G4cout << "Error opening file: " << "PMMABulkAbsorb.dat" << G4endl; 

  ReadBulk.close();


  wlsMPT->AddProperty("ABSLENGTH",bulkEnergy,bulkAbsorb,bulkEntries);
  innercladMPT->AddProperty("ABSLENGTH",wlsEnergy,pmmabulkAbsorb,wlsEntries);
  outercladMPT->AddProperty("ABSLENGTH",wlsEnergy,pmmabulkAbsorb,wlsEntries);

  Polystyrene->SetMaterialPropertiesTable(wlsMPT);
  PMMA->SetMaterialPropertiesTable(innercladMPT);
  Fluorinated_Polymer->SetMaterialPropertiesTable(outercladMPT);

}

WLSfiber::~WLSfiber()
{}

G4LogicalVolume *WLSfiber::BuildStraight(G4double radius,G4double length, G4double curve_radius)
{

  //returns a straight section of WLS fiber containing core and cladding
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);

  G4RotationMatrix RotateY;
  RotateY.rotateY(90*deg);
  RotateY.rotateX(90*deg);

  G4double diameter = 2*radius;
  G4double clad_percent = 0.03;

  G4double core_percent = 1 - 2*clad_percent;
  G4double core_radius = (diameter*core_percent)/2;
  G4double inner_radius = core_radius + (diameter*(clad_percent/2));

  G4double deltaphi = 180*deg;
  
  G4cout << "The core radius is: " << core_radius  << G4endl;
  G4cout << "The inner radius is: " << inner_radius  << G4endl;
  G4cout << "The radius is: " << core_radius + 2*(diameter*(clad_percent/2)) << G4endl;
  G4cout << "The curve radius is: " << curve_radius  << G4endl;

  // build the "ficticious" fiber

  G4Tubs *s_straight_fiber;
  s_straight_fiber = new G4Tubs("s_straight_fiber",0*mm,radius,length/2,0,twopi*rad);

  G4Tubs *s_straight_fiber2;
  s_straight_fiber2 = new G4Tubs("s_straight_fiber2",0*mm,radius,length/2,0,twopi*rad);

  G4Torus *s_curve_fiber;
  s_curve_fiber = new G4Torus("s_curve_fiber",0*mm,radius,curve_radius,0,deltaphi);

  G4UnionSolid *straight_plus_curve;

  straight_plus_curve = new G4UnionSolid("straight_plus_curve",
					     s_straight_fiber,
					     s_curve_fiber,
					     G4Transform3D(RotateY,
							   G4ThreeVector(0,curve_radius,length/2 +0.0*mm)));

  G4UnionSolid *straight_plus_curve_plus_straight;

  straight_plus_curve_plus_straight = new G4UnionSolid("straight_plus_curve_plus_straight",
					     straight_plus_curve,
						       //     s_straight_fiber2,
					     s_straight_fiber,
					     G4Transform3D(DontRotate,
							   G4ThreeVector(0,2*curve_radius,0)));


  l_fiber = new G4LogicalVolume(straight_plus_curve_plus_straight,Fluorinated_Polymer,"l_fiber",0,0,0);

  // build the core *************************************************************

  G4Tubs *straight_core;
  straight_core = new G4Tubs("straight_core",0*mm,core_radius,length/2,0,twopi*rad);

  G4Tubs *straight_core2;
  straight_core2 = new G4Tubs("straight_core2",0*mm,core_radius,length/2,0,twopi*rad);

  G4Torus *curved_core;
  curved_core = new G4Torus("curved_core",0*mm,core_radius,curve_radius,0,deltaphi);

  G4UnionSolid *straight_plus_curve_core;

  straight_plus_curve_core = new G4UnionSolid("straight_plus_curve_core",
					      straight_core,
					      curved_core,
					      G4Transform3D(RotateY,
							    G4ThreeVector(0,curve_radius,length/2+0.0*mm)));
  
  G4UnionSolid *straight_plus_curve_p_s_core;

  straight_plus_curve_p_s_core = new G4UnionSolid("straight_plus_curve_p_s_core",
					      straight_plus_curve_core,
						  //     straight_core2,
					      straight_core,
					      G4Transform3D(DontRotate,
							    G4ThreeVector(0,2*curve_radius,0)));

  //  l_core = new G4LogicalVolume(straight_plus_curve_core,Polystyrene,"l_core",0,0,0);
  l_core = new G4LogicalVolume(straight_plus_curve_p_s_core,Polystyrene,"l_core",0,0,0);

  p_core = new G4PVPlacement(G4Transform3D
			     (DontRotate,G4ThreeVector(0,0,0)),
			     l_core,"p_core",l_fiber,true,0);

  // build the first cladding  *****************************************************
  
  G4Tubs *straight_cladding1;
  G4Tubs *straight_cladding12;

  straight_cladding1 = new G4Tubs("straight_cladding1",core_radius,inner_radius,length/2,0,twopi*rad);
  straight_cladding12 = new G4Tubs("straight_cladding12",core_radius,inner_radius,length/2,0,twopi*rad);


  G4Torus *curved_cladding1;
  curved_cladding1 = new G4Torus("curved_cladding1",core_radius,inner_radius,curve_radius,0,deltaphi);

  G4UnionSolid *straight_plus_curve_cladding1;

  straight_plus_curve_cladding1 = new G4UnionSolid("straight_plus_curve_cladding1",
					      straight_cladding1,
					      curved_cladding1,
					      G4Transform3D(RotateY,
							    G4ThreeVector(0,curve_radius,length/2+0.0*mm)));

  G4UnionSolid *straight_plus_curve_p_s_cladding1;

  straight_plus_curve_p_s_cladding1 = new G4UnionSolid("straight_plus_curve_p_s_cladding1",
					      straight_plus_curve_cladding1,
					      straight_cladding12,
					      G4Transform3D(DontRotate,
							    G4ThreeVector(0,2*curve_radius,0)));

  //   l_cladding1 = new G4LogicalVolume(straight_plus_curve_p_s_cladding1,PMMA,"l_cladding1",0,0,0);
   l_cladding1 = new G4LogicalVolume(straight_plus_curve_p_s_cladding1,PMMA,"l_cladding1",0,0,0);



    p_cladding1 = new G4PVPlacement(G4Transform3D
    				  (DontRotate,G4ThreeVector(0,0,0)),
  				  l_cladding1,"p_cladding1",l_fiber,true,0);

  // build the second cladding ***************************************************

  G4Tubs *straight_cladding2;
  G4Tubs *straight_cladding22;

  straight_cladding2 = new G4Tubs("straight_cladding2",inner_radius,radius,length/2,0,twopi*rad);
  straight_cladding22 = new G4Tubs("straight_cladding22",inner_radius,radius,length/2,0,twopi*rad);

  G4Torus *curved_cladding2;
  curved_cladding2 = new G4Torus("curved_cladding2",inner_radius,radius,curve_radius,0,deltaphi);

  G4UnionSolid *straight_plus_curve_cladding2;

  straight_plus_curve_cladding2 = new G4UnionSolid("straight_plus_curve_cladding2",
					      straight_cladding2,
					      curved_cladding2,
					      G4Transform3D(RotateY,
							    G4ThreeVector(0,curve_radius,length/2+0.0*mm)));

  G4UnionSolid *straight_p_c_p_s_cladding2;

  straight_p_c_p_s_cladding2 = new G4UnionSolid("straight_p_c_p_s_cladding2",
					      straight_plus_curve_cladding2,
					      straight_cladding22,
					      G4Transform3D(DontRotate,
							    G4ThreeVector(0,2*curve_radius,0)));

  l_cladding2 = new G4LogicalVolume(straight_p_c_p_s_cladding2,Fluorinated_Polymer,"l_cladding2",0,0,0);
  //l_cladding2 = new G4LogicalVolume(straight_plus_curve_cladding2,PMMA,"l_cladding2",0,0,0);


    p_cladding2 = new G4PVPlacement(G4Transform3D
      				  (DontRotate,G4ThreeVector(0,0,0)),
      				  l_cladding2,"p_cladding2",l_fiber,true,0);




  yellow = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255. ));
  red = new G4VisAttributes( G4Colour(255/255. ,0/255. ,0/255. ));
  blue = new G4VisAttributes( G4Colour(0/255. ,0/255. ,255/255. ));

  //  l_fiber->SetVisAttributes(yellow);
     l_core->SetVisAttributes(yellow);
     l_cladding1->SetVisAttributes(blue);
     l_cladding2->SetVisAttributes(red);

  return l_fiber;

}

//G4LogicalVolume *WLSfiber::BuildTorus(G4double fibradius, G4double _curve_radius, G4double deltaphi)
//{

  /*


  //returns a curved segment of WLS fiber containing core and cladding
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  //Volume is created with axis of symmetry parallel to z-axis, startphi equal to x-axis. 

  G4double corepercent = 0.985;
  G4Torus *s_torus;
  s_torus = new G4Torus("s_torus",0*mm,fibradius,_curve_radius,0,deltaphi);

  l_torus = new G4LogicalVolume(s_torus,Polystyrene,"l_torus",0,0,0);

  
  G4Torus *s_core;
  s_core = new G4Torus("s_core",0*mm,corepercent*fibradius,_curve_radius,0,deltaphi);
  G4LogicalVolume *l_core;
  l_core = new G4LogicalVolume(s_core,Polystyrene,"l_core",0,0,0);
  G4VPhysicalVolume *p_core;
  p_core = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),l_core,"p_core",l_torus,false,0);


  G4Torus *s_cladding;
  s_cladding = new G4Torus("s_cladding",corepercent*fibradius,fibradius,_curve_radius,0,deltaphi);

  l_cladding = new G4LogicalVolume(s_cladding,Polystyrene,"l_cladding",0,0,0);

  p_cladding = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),l_cladding,"p_cladding",l_torus,false,0);
  
  
  yellow = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255. ));
   l_torus->SetVisAttributes(yellow);
   l_core->SetVisAttributes(yellow);
   l_cladding->SetVisAttributes(yellow);
  return l_torus;

  */

//}
