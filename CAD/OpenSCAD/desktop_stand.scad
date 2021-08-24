
use <triangles.scad>;

keyboard_width = 130;
keyboard_depth = 51.76;


stand_angle = 50;

stand_y_thickness = 10;
stand_x_thickness = 6;

stand_h_length = keyboard_depth - stand_y_thickness;

stand_cutout_depth = 60;
stand_cutout_height = 65;

//shell
difference()
{
    echo(stand_h_length);
    Isosceles_Triangle( angle=stand_angle, H=stand_h_length, height=keyboard_width );
    
    rotate([0,0,-stand_angle]) 
    translate([-stand_cutout_depth/2,-0.1-stand_cutout_height/2-stand_y_thickness/2,stand_y_thickness]) cube([stand_cutout_depth,stand_cutout_height,keyboard_width-stand_y_thickness*2]);
}
draw_arms();

module draw_arms()
{
    x_position = stand_h_length - stand_x_thickness;
    
    arm(x_position);
    translate([0,0,keyboard_width-stand_y_thickness]) arm(x_position);

    //crossbeam
    translate([x_position,-stand_y_thickness,0]) cube([stand_x_thickness,stand_y_thickness,keyboard_width]);
}

//arms
module arm(x_position)
{
    tab_length = 2;
    arm_length = keyboard_depth + 0.2; //friction fit
    
    //arm
    translate([x_position,-arm_length,0]) cube([stand_x_thickness,arm_length,stand_y_thickness]);
    
    //tab
    translate([x_position,-arm_length+tab_length,0]) 
    rotate([0,0,180]) Right_Angled_Triangle( a=tab_length, b=tab_length, height=stand_y_thickness );
    
}