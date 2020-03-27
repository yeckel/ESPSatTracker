thickness = 4;
difference(){
union(){
translate([0,0,50]) cube([40,12,thickness]);
translate([0,0,0]) cube([thickness,12,50]);
translate([0,0,0]) cube([20,12,thickness]);
}
#translate([12,6,-10]) cylinder(d=3,h=20);
}