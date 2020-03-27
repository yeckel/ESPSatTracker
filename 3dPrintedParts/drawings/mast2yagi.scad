rotatorRod=20.5;
antenaRod=15.5;
wallThickness=4;

difference(){
	sphere(r=30);

	#translate([0,rotatorRod/2,0]) rotate([00,90,00]) cube([rotatorRod,rotatorRod,100],center=true);
	#translate([0,-antenaRod/2-0.2,0]) cube([antenaRod,antenaRod,100],center=true);
	#rotate([90,00,00]) cylinder(d=4,h=150, center=true);
}