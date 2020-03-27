trubicka = 8.2;
hrana = 1;

difference(){
	union(){
		sphere(d=trubicka+hrana*2);
		cylinder(d=trubicka+hrana*2, h=trubicka);
	}
	union(){
		sphere(d=trubicka);
		cylinder(d=trubicka, h=trubicka+1);
	}
}