hranol = 15.3;
trubicka = 12.2;
edge = 5;

delka = 70 + trubicka + edge;

module so239()
{
    so239_screw = 3.2;
    so239_screw_distance = 18.2;
    so239_mount_size = 25.4;
    wall_thickness = 5;

    cylinder(h = 8, d = 14);

    translate([0, 0, 10]) rotate([0, 0, 45])
    {
        translate([+so239_screw_distance / 2, +so239_screw_distance / 2, -5]) cylinder(h = wall_thickness * 3, d = 3.2, center = true);
        translate([-so239_screw_distance / 2, -so239_screw_distance / 2, -5]) cylinder(h = wall_thickness * 3, d = 3.2, center = true);
        translate([+so239_screw_distance / 2, -so239_screw_distance / 2, -5]) cylinder(h = wall_thickness * 3, d = 3.2, center = true);
        translate([-so239_screw_distance / 2, +so239_screw_distance / 2, -5]) cylinder(h = wall_thickness * 3, d = 3.2, center = true);
    }

}


module radiator()
{
    difference()
    {
        rotate([90, 0, 0]) cylinder(d=(trubicka + edge), h=80, center=true);


        rotate([90, 0, 0]) cylinder(d= trubicka, h = 1000, center = true);
        rotate([90, 0, 0]) cylinder(d= trubicka + edge + 1, h = 20, center = true);
    }
}

module radiatorBox()
    difference()
{
    union()
    {
        difference()
        {
            translate([delka / 3 + trubicka / 3, 0, 0]) union()
            {
                translate([0,0,-edge/2]) cube([delka, hranol + edge, hranol + edge], center = true);
                translate([0, 0, (35 + trubicka) / 2]) cube([delka, 3 * hranol, 35], center = true);
            }

            translate([0,0,-edge/2]) cube([1400, hranol, hranol], center = true);

            translate([(delka - edge) / 2 - 8, 0, (35 + trubicka) / 2 + 2]) cube([delka - edge, 3 * hranol - edge, 35], center = true);
            translate([70, 0, 25]) rotate([0, 90, 0]) so239();
        }
        translate([-edge / 3, 0, (hranol + trubicka) / 2-2])  radiator();
    }
    translate([-edge / 3, 0, (hranol + trubicka) / 2-2]) rotate([90, 0, 0]) cylinder(d = trubicka, h = 1000, center = true);
}

//intersection()
difference()
{
    radiatorBox();
    cube([delka / 2, delka, delka], center = true);
}
