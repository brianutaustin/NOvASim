#include "NOVAstrip.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include "fstream"
#include "iostream"
#include "G4Box.hh"
//#include "G4Torus.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "WLSfiber.hh"
#include "G4UnitsTable.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

NOVAstrip::NOVAstrip(G4String buildfile)
{
  //  SetValuestoDefault();
  //Read keys and values from file buildfile:
  std::ifstream config;
  config.open(buildfile);
  if (!config.is_open())
    G4cout << "Error opening file " << buildfile << G4endl;
  else{
    while(1){
      G4String variable,unit;
      G4double value;
      
      config>>variable;
      if(!config.good()) break;
      if(variable == "striplength"){
	config >> value >> unit;
	striplength = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "stripwidth"){
	config >> value >> unit;
	stripwidth = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "stripheight"){
	config >> value >> unit;
	stripheight = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "fiberradius"){
	config >> value >> unit;
	fibradius = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "fiberlength"){
	config>>value>>unit;
	fiblength=value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable=="paintthickness"){
	config >> value >> unit;
	paintthickness = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "curveradius"){
	config >> value >> unit;
	curve_radius = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "off_axis_dist"){
	config >> value >> unit;
	off_axis_dist = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "lightyield"){
	config >> lightyield;
      }
    }
  }

  config.close();
  Construct();

}

NOVAstrip::~NOVAstrip()
{}
void NOVAstrip::SetValuestoDefault(){
  // set default values here
}
void NOVAstrip::Construct()
{

  elementH = new G4Element( "Hydrogen", "H", 1. , 1.00794*g/mole );
  elementC = new G4Element( "Carbon", "C", 6. , 12.011*g/mole );
  elementO = new G4Element( "Oxygen", "O", 8. , 15.9994*g/mole );
  elementTi = new G4Element( "Titanium","Ti",22,47.88*g/mole);

  scintillator = new G4Material("scintillator",  1.032*g/cm3, 2, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  scintillator->AddElement( elementH, 0.666 );
  scintillator->AddElement( elementC, 0.334 );
  
  Polystyrene = new G4Material("Polystyrene",  1.05*g/cm3, 2, kStateSolid, 273.15*kelvin, 1.0*atmosphere );
  Polystyrene->AddElement( elementH, 0.498 );
  Polystyrene->AddElement( elementC, 0.502 );

  Vacuum = new G4Material ("Vacuum", 1., 1.01*g/mole,universe_mean_density, kStateGas, 3.e-18*pascal,2.73*kelvin);


  Ti02 = new G4Material("Ti02",4.23*g/cm3,2,kStateSolid);
  Ti02->AddElement(elementO,2);
  Ti02->AddElement(elementTi,1);

  coating = new G4Material("coating",1.4316*g/cm3,2,kStateSolid);
  coating->AddMaterial(Ti02,.12);
  coating->AddMaterial(Polystyrene,.88);

  G4int scintEntries = 0;
  G4int absorbEntries = 0;
  G4int wlsAbEntries = 0; 
  G4int wlsEmEntries = 0;
  G4double absorblength = 0;
  G4double wlsabsorblength = 0;
  G4double pWavelength;

  G4double scintEnergy[500];
  G4double scintEmit[500];
  G4double scintIndex[500];
  G4double scintSlow[500];
  G4double vacIndex[500];
  G4double vacAbsorb[500];
  G4double absorbEnergy[500];
  G4double Absorb[500];
  G4double wlsEnergy[500];
  G4double wlsEmit[500];
  G4double wlsAbsorb[500];



  //  G4double scintIndexconst = 1.58; original.  changed 09/18/07
  G4double scintIndexconst = 1.47;

  G4double vacAbsorbconst = 100*m;

  std::ifstream ReadScint;

  //  std::string Scint_file = "pTP_emission.dat";

//  ReadScint.open("pl_file_1.dat");

 ReadScint.open("ppo_emission.txt");
 if(ReadScint.is_open()){
  while(!ReadScint.eof()){
    G4String filler;
    ReadScint >> pWavelength >> filler >> scintEmit[scintEntries];
    scintEnergy[scintEntries] = (1240/pWavelength)*eV; //convert wavelength to eV
    scintIndex[scintEntries] = scintIndexconst;
    scintSlow[scintEntries] = 0.0;
    vacIndex[scintEntries] = 1.0;
    vacAbsorb[scintEntries] = vacAbsorbconst;
    scintEntries++;
  }
 }
 else
   G4cout << "Error opening file: "  << G4endl;
 ReadScint.close();

  std::ifstream Readabsorb;


  //  G4String Bulk = "fiberPSTabsorb.dat";
//  G4String Readabsorblength = "PSTBulkAbsorb.cfg";

  Readabsorb.open("PSTBulkAbsorb.cfg");
  if(Readabsorb.is_open()){
    while(!Readabsorb.eof()){
      G4String filler;
      Readabsorb >> pWavelength >> filler >> absorblength;
      absorbEnergy[absorbEntries] = (1240/pWavelength)*eV;
      Absorb[absorbEntries] = 1.8*absorblength*m;
      absorbEntries++;
    }
  }
  else
    G4cout << "Error opening file: " << G4endl; 
  Readabsorb.close();
    
  std::ifstream ReadWLSa;
  //  G4String WLSabsorb = "UPS923.cfg";

  ReadWLSa.open("UPS923.cfg");
  if (ReadWLSa.is_open()){
    while (!ReadWLSa.eof()){
      G4String filler;
      ReadWLSa >> pWavelength >> filler >> wlsabsorblength;
      wlsEnergy[wlsAbEntries] = (1240/pWavelength) *eV;
      wlsAbsorb[wlsAbEntries] = 1.8*wlsabsorblength*m;
      wlsAbEntries++;
    }
  }
  else 
    G4cout << "Error opening file: " << G4endl;
  ReadWLSa.close();



  wlsEmEntries = 0;
  std::ifstream ReadWLSe;
  //  G4String WLSemit = "full_popop_emission.cfg";
  ReadWLSe.open("bisMSB_emission.txt");
  if(ReadWLSe.is_open()){
    while (!ReadWLSe.eof()){
      G4String filler;
      ReadWLSe >> pWavelength >> filler >> wlsEmit[wlsEmEntries];
      wlsEnergy[wlsEmEntries] = (1240/pWavelength) *eV;
      wlsEmEntries++;
    }
  }
  else 
    G4cout << "Error opening file: " << G4endl;
  ReadWLSe.close();



  scintMPT = new G4MaterialPropertiesTable();
  G4double scintYield = lightyield/MeV;
  G4double scintRes = 1;
  G4double scintFastconst = 2.1*ns;
  G4double scintSlowconst = 10*ns;
  //  G4double efficiency = 1.0;

  scintMPT->AddProperty("RINDEX",scintEnergy,scintIndex,scintEntries);
  scintMPT->AddProperty("ABSLENGTH",absorbEnergy,Absorb,absorbEntries);
  scintMPT->AddProperty("WLSCOMPONENT", wlsEnergy, wlsEmit, wlsEmEntries);
  scintMPT->AddProperty("WLSABSLENGTH", wlsEnergy, wlsAbsorb, wlsAbEntries);
  scintMPT->AddConstProperty("WLSTIMECONSTANT", 1*ns);
  scintMPT->AddConstProperty("CONSTANTQUANTUMYIELD", 0.9);

  scintMPT->AddProperty("FASTCOMPONENT",scintEnergy,scintEmit,scintEntries);
  scintMPT->AddProperty("SLOWCOMPONENT",scintEnergy,scintSlow,scintEntries);
  scintMPT->AddConstProperty("SCINTILLATIONYIELD",scintYield);
  scintMPT->AddConstProperty("RESOLUTIONSCALE",scintRes);
  scintMPT->AddConstProperty("FASTTIMECONSTANT",scintFastconst);
  scintMPT->AddConstProperty("SLOWTIMECONSTANT",scintSlowconst);
  scintMPT->AddConstProperty("YIELDRATIO",1.0);
  scintillator->SetMaterialPropertiesTable(scintMPT);


  vacMPT = new G4MaterialPropertiesTable();
  vacMPT->AddProperty("RINDEX",scintEnergy,vacIndex,scintEntries);
  vacMPT->AddProperty("ABSLENGTH",scintEnergy,vacAbsorb,scintEntries);
  Vacuum->SetMaterialPropertiesTable(vacMPT);
  
  //4. Define Geometries
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0*deg);
  //4.0 Holder


  s_holder = new G4Box("s_holder",stripwidth/2+paintthickness+1*cm,stripheight/2+paintthickness+1*cm,fiblength/2 + curve_radius);
  l_holder = new G4LogicalVolume(s_holder,Vacuum,"l_holder",0,0,0);
  G4cout << "The fiber length is " << fiblength << G4endl;

  //4.1 Fiber
  s_fiber = new G4Tubs("s_fiber",0*mm,fibradius,fiblength/2,0,twopi*rad);

  FibMaker = new WLSfiber();

  l_fiber = FibMaker->BuildStraight(fibradius,fiblength,curve_radius);
  
  //4.2 Scintillator Strip
 
  G4double inner_corner_radius = 4*mm;
  G4double outer_corner_radius = inner_corner_radius + paintthickness;

  startstrip = new G4Box("startstrip",
			 stripwidth/2 - inner_corner_radius,
			 stripheight/2,
			 striplength/2);

  startstrip2 = new G4Box("startstrip2",
			 stripwidth/2,
			 stripheight/2 - inner_corner_radius,
			 striplength/2);

  add_box_strip = new G4UnionSolid("add_box_strip",
				   startstrip,
				   startstrip2,
				   G4Transform3D(DontRotate,G4ThreeVector(0.0*cm,0.0*cm,0.0*cm)));

  


  s_inner_corner1 = new G4Tubs("s_inner_corner1",0*mm,inner_corner_radius + 0.0*mm,striplength/2,0,twopi*rad);
  s_inner_corner2 = new G4Tubs("s_inner_corner2",0*mm,inner_corner_radius + 0.0*mm,striplength/2,0,twopi*rad);
  s_inner_corner3 = new G4Tubs("s_inner_corner3",0*mm,inner_corner_radius + 0.0*mm,striplength/2,0,twopi*rad);
  s_inner_corner4 = new G4Tubs("s_inner_corner4",0*mm,inner_corner_radius + 0.0*mm,striplength/2,0,twopi*rad);

  // try new corners
  /*  they're OK
  s_inner_corner1 = new G4Tubs("s_inner_corner1",
			       0.0*mm,
			       inner_corner_radius,
			       striplength/2 - 0*paintthickness,
			       0,
			       0.5*pi*rad);

  s_inner_corner2 = new G4Tubs("s_inner_corner2",
			       0.0*mm,
			       inner_corner_radius,
			       striplength/2 - 0*paintthickness,
			       0.5*pi*rad,
			       1*pi*rad);

  s_inner_corner3 = new G4Tubs("s_inner_corne3",
			       0.0*mm,
			       inner_corner_radius,
			       striplength/2 - 0*paintthickness,
			       1.0*pi*rad,
			       1.5*pi*rad);

  s_inner_corner4 = new G4Tubs("s_inner_corner4",
			       0.0*mm,
			       inner_corner_radius,
			       striplength/2 - 0*paintthickness,
			       1.5*pi*rad,
			       2*pi*rad);
  */
  //  end new inner corners




  add_corner1 = new G4UnionSolid("add_corner1",
				 add_box_strip,
				 s_inner_corner1,
				 G4Transform3D(DontRotate,
					       G4ThreeVector(stripwidth/2  - inner_corner_radius,
							     stripheight/2 - inner_corner_radius,
							     0.0*mm)));
  
  add_corner2 = new G4UnionSolid("add_corner2",
				 add_corner1,
				 s_inner_corner2,
				 G4Transform3D(DontRotate,
					       G4ThreeVector(-(stripwidth/2 - inner_corner_radius),
							     stripheight/2 - inner_corner_radius,
							     0.0*mm)));
  
  add_corner3 = new G4UnionSolid("add_corner3",
				 add_corner2,
				 s_inner_corner3,
				 G4Transform3D(DontRotate,
					       G4ThreeVector((stripwidth/2 - inner_corner_radius),
							     -(stripheight/2 - inner_corner_radius),
							     0.0*mm)));
  
  add_corner4 = new G4UnionSolid("add_corner4",
				 add_corner3,
				 s_inner_corner4,
				 G4Transform3D(DontRotate,
					       G4ThreeVector(-(stripwidth/2 - inner_corner_radius),
							     -(stripheight/2 - inner_corner_radius),
							     0.0*mm)));
  

  startpaint1 = new G4Box("startpaint1",
			 paintthickness/2,
			 stripheight/2 - inner_corner_radius,
			 striplength/2 - paintthickness);

  s_outer_corner1 = new G4Tubs("s_outer_corner1",
			       inner_corner_radius,
			       outer_corner_radius,
			       striplength/2 - paintthickness,
			       0,
			       0.5*pi*rad);

  add_outer_corner1 = new G4UnionSolid("add_outer_corner1",
				 startpaint1,
				 s_outer_corner1,
				 G4Transform3D(DontRotate,
					       G4ThreeVector( -paintthickness/2 - inner_corner_radius, //ok
							     stripheight/2 - inner_corner_radius,
							     0.0*mm)));
  startpaint2 = new G4Box("startpaint2",
			 stripwidth/2 - inner_corner_radius,
			 paintthickness/2,
			 striplength/2 - paintthickness);





  add_second_wall = new G4UnionSolid("add_second_wall",
				   add_outer_corner1,
				   startpaint2,
				   G4Transform3D(DontRotate,G4ThreeVector(-stripwidth/2 - paintthickness/2,
									  stripheight/2 + paintthickness/2,
									  0.0*cm)));

  s_outer_corner2 = new G4Tubs("s_outer_corner2",
			       inner_corner_radius,
			       outer_corner_radius,
			       striplength/2 - paintthickness,
			       0.0*rad,
			       0.5*pi*rad);

  G4RotationMatrix Rotate90;
  Rotate90.rotateZ(90*deg);

  add_outer_corner2 = new G4UnionSolid("add_outer_corner2",
				 add_second_wall,
				 s_outer_corner2,
				 G4Transform3D(Rotate90,
					       G4ThreeVector( - stripwidth - paintthickness/2 + inner_corner_radius, //ok
							     stripheight/2 - inner_corner_radius,
							     0.0*mm)));

  add_third_wall = new G4UnionSolid("add_third_wall",
				   add_outer_corner2,
				   startpaint1,
				   G4Transform3D(DontRotate,G4ThreeVector(-stripwidth - paintthickness,
									  0.0*cm,
									  0.0*cm)));


  s_outer_corner3 = new G4Tubs("s_outer_corner3",
			       inner_corner_radius,
			       outer_corner_radius,
			       striplength/2 - paintthickness,
			       0.0*rad,
			       0.5*pi*rad);

  G4RotationMatrix Rotate180;
  Rotate180.rotateZ(180*deg);

  add_outer_corner3 = new G4UnionSolid("add_outer_corner3",
				 add_third_wall,
				 s_outer_corner3,
				 G4Transform3D(Rotate180,
					       G4ThreeVector( - stripwidth - paintthickness/2 + inner_corner_radius, //ok
							     -stripheight/2 + inner_corner_radius,
							     0.0*mm)));

  add_fourth_wall = new G4UnionSolid("add_fourth_wall",
				   add_outer_corner3,
				   startpaint2,
				   G4Transform3D(DontRotate,G4ThreeVector(-stripwidth/2 - paintthickness/2,
									  -stripheight/2 - paintthickness/2,
									  0.0*cm)));

  s_outer_corner4 = new G4Tubs("s_outer_corner4",
			       inner_corner_radius,
			       outer_corner_radius,
			       striplength/2 - paintthickness,
			       0.0*rad,
			       0.5*pi*rad);

  G4RotationMatrix Rotate270;
  Rotate270.rotateZ(270*deg);

  add_outer_corner4 = new G4UnionSolid("add_outer_corner4",
				 add_fourth_wall,
				 s_outer_corner4,
				 G4Transform3D(Rotate270,
					       G4ThreeVector( - paintthickness/2 - inner_corner_radius, //ok
							     -stripheight/2 + inner_corner_radius,
							     0.0*mm)));




  // SUBTRACT FIBER HOLES FROM SOLID STRIP (3)
  //  putting in the off axis distance

  s_strip = new G4SubtractionSolid("s_strip",
				   add_corner4,
				   s_fiber,
				   G4Transform3D(DontRotate,G4ThreeVector(off_axis_dist,curve_radius,-2*curve_radius)));
  //was -2*curve
  s_strip2 = new G4SubtractionSolid("s_strip2",
				    s_strip,
				    s_fiber,
				    G4Transform3D(DontRotate,G4ThreeVector(off_axis_dist,-curve_radius,-2*curve_radius)));

  s_curve_fiber = new G4Torus("s_curve_fiber",0*mm,fibradius,curve_radius,0,180*deg);

  G4RotationMatrix RotateY;
  RotateY.rotateY(90*deg);
  RotateY.rotateX(90*deg);


  //was originally striplength/2
  s_strip3 = new G4SubtractionSolid("s_strip3",
				    s_strip2,
				    s_curve_fiber,
				    G4Transform3D(RotateY,G4ThreeVector(off_axis_dist,0.0*cm,striplength/2 - 2*curve_radius)));

  
    p_fiber = new G4PVPlacement(G4Transform3D
				(DontRotate,
				 G4ThreeVector(off_axis_dist, -curve_radius, -2*curve_radius - 50*cm)),  //was - 0*cm
				l_fiber,"p_fiber",l_holder,true,0);
  
    // was lholder
    G4RotationMatrix Rotate;  
    G4double angle = 270.0*deg; 
    Rotate.rotateX(angle);
    Rotate.rotateZ(90.0*deg);

    //    l_strip = new G4LogicalVolume(add_outer_corner4,scintillator,"l_strip",0,0,0);
    l_strip = new G4LogicalVolume(s_strip3,scintillator,"l_strip",0,0,0);
    

    
    p_strip = new G4PVPlacement(G4Transform3D
				(DontRotate,
				 G4ThreeVector(0.0*cm,0.0*cm,0.0*cm)),
				l_strip,"p_strip",l_holder,true,0);
    
    l_paint = new G4LogicalVolume(add_outer_corner4,coating,"l_paint",0,0,0);

    
    p_paint = new G4PVPlacement(G4Transform3D
				(DontRotate,
				 G4ThreeVector(stripwidth/2+paintthickness/2, 0.0*cm,0.0*cm)),
				l_paint,"p_paint",l_holder,true,0);
    

    G4int Ti02_entries = 0;
    G4double Ti02_energy[500];
    G4double Ti02_reflect[500];
    G4double zero[500];
    
    std::ifstream Read_Ti02;
    //    G4String Ti02_file = "/home/pahlka/input_files/reflectance_Ti02_flat.dat";
    //    G4String Ti02_file = "/home/pahlka/input_files/reflectance_Ti02.dat";
    // good! G4String Ti02_file = "/home/pahlka/input_files/reflectance_extrusion.txt";
    G4String Ti02_file = "/work/03069/whf346/lonestar/nova_benton/rutile.txt";

    Read_Ti02.open(Ti02_file);
    if (Read_Ti02.is_open()){
      while(!Read_Ti02.eof()){
	G4String filler;
	G4double wavelength;
	G4double Ti02_ref_coeff;
	Read_Ti02 >> wavelength >> filler >> Ti02_ref_coeff;
	Ti02_energy[Ti02_entries] = (1240/wavelength)*eV;
	Ti02_reflect[Ti02_entries] = Ti02_ref_coeff;
	zero[Ti02_entries] = 1e-6;
	Ti02_entries++;
      }
    }
    else
      G4cout << "Error opening file: " << Ti02_file << G4endl; 
    Read_Ti02.close();

  op_paint = new G4OpticalSurface("op_paint");
  op_paint->SetModel(unified);
  op_paint->SetPolish(0);
  op_paint->SetFinish(groundfrontpainted);
  op_paint->SetType(dielectric_metal);

  paintMPT = new G4MaterialPropertiesTable();
  paintMPT->AddProperty("REFLECTIVITY",Ti02_energy,Ti02_reflect,Ti02_entries);
  paintMPT->AddProperty("SPECULARLOBECONSTANT",Ti02_energy,zero,Ti02_entries);
  paintMPT->AddProperty("SPECULARSPIKECONSTANT",Ti02_energy,zero,Ti02_entries);
  paintMPT->AddProperty("BACKSCATTERCONSTANT",Ti02_energy,zero,Ti02_entries);
 
  op_paint->SetMaterialPropertiesTable(paintMPT);

  b_paint = new G4LogicalSkinSurface("b_paint",l_paint,op_paint);
    
  clear = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  clear->SetForceWireframe(true);

  white = new G4VisAttributes(G4Color(1,1,1));

  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);

  green = new G4VisAttributes(G4Colour(0,1,0));
  green->SetForceWireframe(true);
  
  l_holder->SetVisAttributes(invis);
  l_strip->SetVisAttributes(clear);
  l_paint->SetVisAttributes(white);

}

  
