include <Getriebe.scad>
include <MCAD/stepper.scad>
include <MCAD/bearing.scad>
include <MyModules.scad>


boxHeight = 200;
rodWidth=20;


//box(boxHeight,rodWidth);
//
////svisla
//translate([(rodWidth+boxHeight)/2,(rodWidth+boxHeight)/2,0]){
//	translate([0,0,0]) bearing(model=6008);
//	translate([0,0,boxHeight]) bearing(model=6008);
//	translate([0,0,-boxHeight]) cylinder(h=boxHeight*2,d=40);
//	translate([0,0,rodWidth]) pfeilrad (modul=1, zahnzahl=170, breite=10, bohrung=40, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
//}
////motor
//translate([50,35,35]) rotate([0,0,0]) {
//	motor(Nema23, dualAxis=false);
//	translate([0,0,-15])pfeilrad (modul=1, zahnzahl=20, breite=10, bohrung=8, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
//}
// 
//
////vodorovna
//translate([boxHeight/2,5*boxHeight/7,(2*rodWidth+boxHeight)/2]) rotate([0,90,0]){
//	translate([0,0,-boxHeight/2]) bearing(model=6008);
//	translate([0,0,boxHeight/2]) bearing(model=6008);
//	translate([0,0,-1.5*boxHeight]) cylinder(h=boxHeight*3,d=40);
//   translate([0,0,4*rodWidth]) pfeilrad (modul=1, zahnzahl=170, breite=10, bohrung=40, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
//}
////motor
//translate([175,50,100]) rotate([0,-90,0]){
// motor(Nema23, dualAxis=false);
// translate([0,0,-15])pfeilrad (modul=1, zahnzahl=20, breite=10, bohrung=8, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
//}


//~~~~~~~~~~~kolecko k motoru~~~~~~~~~~~~~~
//pfeilrad (modul=1, zahnzahl=20, breite=10, bohrung=8.2, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
//difference(){
//	cylinder(d=14,h=20);
//	translate([0,0,-1])cylinder(d=8,h=22);
//}

difference(){
difference(){
	union(){
		cylinder(h=40,d=47);
		pfeilrad (modul=1, zahnzahl=160, breite=10, bohrung=38, eingriffswinkel=20, schraegungswinkel=30, optimiert=true);
	}
	cylinder(d=145,h=150,center=true);
}
	cylinder(d=50,h=150,center=true);
}

//difference(){
//	union(){
//		translate([0,0,15+11]) cylinder(d=40,h=25);
//		translate([0,0,15]) cylinder(d=47,h=11);
//		translate([0,0,0]) cylinder(d=50,h=15);
//	}
//	cube([20,20,150],center=true);
//}