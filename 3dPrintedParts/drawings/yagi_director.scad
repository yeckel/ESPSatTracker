hranol = 15.3;
trubicka = 8.2;
edge=5;

difference(){
	union(){
		cube([hranol+edge, hranol+edge, hranol+edge], center = true);
		translate([0,0,(hranol+trubicka)/2]) cube([hranol+edge, 3*hranol, trubicka+edge], center = true);
	}
union(){
	cube([1400, hranol, hranol], center = true);
	translate([0,0,(hranol+trubicka)/2]) rotate([90,0,0]) cylinder(d=trubicka, h=1000,center=true);
}
}