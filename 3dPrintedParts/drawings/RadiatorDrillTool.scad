trubicka = 12.4;
dira=3;

difference(){
	cube([20,20,40],center=true);
	translate([0,0,10]) cylinder(d=trubicka,h=40,center=true);
	translate([0,0,-5]) rotate([0,90,0]) cylinder(d=dira,h=40,center=true);
}