profil=20/1.013;
stena=3*1.4;

vnitek=profil-stena;

rotate([0,0,0]) translate([0,0,profil])cube([vnitek,vnitek,2*profil],center = true);
rotate([0,90,0]) translate([0,0,profil])cube([vnitek,vnitek,2*profil],center = true);
rotate([90,0,0]) translate([0,0,profil])cube([vnitek,vnitek,2*profil],center = true);
cube([profil,profil,profil-stena], center = true);