#include "Transform.hpp"
using namespace rendering;

Transform* Transform::translate(double x, double y, double z)
{
    translation_matrix.set_value_at(0,3,x);
    translation_matrix.set_value_at(1,3,y);
    translation_matrix.set_value_at(2,3,z);

    transformation_matrix = transformation_matrix * translation_matrix;
    return this; 
}

Transform* Transform::rotate(double x, double y, double z)
{
    double _x = transformation_matrix.get_value_at(0,3);
    double _y = transformation_matrix.get_value_at(1,3);
    double _z = transformation_matrix.get_value_at(2,3);
    
    Transform::translate(-_x, -_y, -_z);
    //x axis
    rotation_matrix_x.set_value_at(1,1, cos(x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(1,2, -sin(x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(2,1, sin(x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(2,2, cos(x*DEG_TO_RAD));
    //y axis
    rotation_matrix_y.set_value_at(0,0, cos(x*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(0,2, sin(x*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(2,0, -sin(x*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(2,2, cos(x*DEG_TO_RAD));
    //z axis 
    rotation_matrix_z.set_value_at(0,0, cos(x*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(0,1, -sin(x*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(1,0, sin(x*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(1,1, cos(x*DEG_TO_RAD));

    transformation_matrix = transformation_matrix * rotation_matrix_y;// * rotation_matrix_y * rotation_matrix_z; 
    Transform::translate(_x,_y,_z); 
    return this;
}

Transform* Transform::scale(double x, double y, double z)
{
    return this;
}




