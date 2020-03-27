profil=20;
stena=1;
tloustka=2;

vnitek=profil-stena;

difference(){
	union(){
		rotate([0,90,0]) translate([0,0,profil])cube([vnitek,vnitek,2*profil],center = true);
		cube([profil+tloustka,profil+tloustka,profil+tloustka], center = true);
	}
	rotate([90,0,0]) cube([vnitek,vnitek,2*profil],center = true);
}