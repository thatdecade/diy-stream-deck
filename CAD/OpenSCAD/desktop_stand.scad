
use <triangles.scad>;

keyboard_width = 130;
keyboard_depth = 52;

stand_depth = 40;
stand_angle = 50;

stand_thickness = 15;

difference()
{
    Triangle(a=keyboard_depth, b=stand_depth, angle=stand_angle, height=keyboard_width, heights=undef, center=undef, centerXYZ=[false, false, false]);
    
    translate([0,0,stand_thickness-0.1]) cube([stand_depth-stand_thickness,keyboard_depth,keyboard_width-stand_thickness*2]);
}