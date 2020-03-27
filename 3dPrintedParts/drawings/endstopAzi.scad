module halfCylinder(){
		difference(){
			cylinder(d=10,h=10);
			translate([-5,0,5]) cube([10,10,11],center=true);
		}
}

difference(){
	union(){
		translate([29,0,0]) halfCylinder();
		translate([2,0,5]) cube([54,5,10],center=true);
		cylinder(d=40,h=10);
	}

	translate([0,0,-50]) cylinder(d=35,h=100);
	translate([22.5,0,5]) rotate([90,0,0]) cylinder(d=3,h=10,center=true);
	translate([-22.5,0,5]) rotate([90,0,0]) cylinder(d=3,h=10,center=true);
	cube([70,0.5,70],center=true);
}