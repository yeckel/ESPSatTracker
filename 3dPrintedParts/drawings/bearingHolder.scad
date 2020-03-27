difference(){
union(){
cylinder(d=76,h=19);//obal loziska stena 4mm
translate([0,0,2]) cube([80,80,4],center=true); //podstavec
}
cylinder(d=69,h=15); //lozisko
cylinder(d=65,h=30); //vnitrek loziska

translate([33,33,0]) cylinder(d=3,h=20);
translate([-33,-33,0]) cylinder(d=3,h=20);
translate([33,-33,0]) cylinder(d=3,h=20);
translate([-33,33,0]) cylinder(d=3,h=20);
}