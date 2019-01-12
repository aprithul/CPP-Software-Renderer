#include "Transform.hpp"
using namespace rendering;

Transform* Transform::translate(double x, double y, double z)
{
    position.x += x;
    position.y += y;
    position.z += z;
    translation_matrix.set_value_at(0,3,position.x);
    translation_matrix.set_value_at(1,3,position.y);
    translation_matrix.set_value_at(2,3,position.z);
    create_transformation_matrix(); 
    //transformation_matrix = transformation_matrix * translation_matrix;
    return this; 
}

Transform* Transform::rotate(double x, double y, double z)
{
   // double _x = transformation_matrix.get_value_at(0,3);
   // double _y = transformation_matrix.get_value_at(1,3);
   // double _z = transformation_matrix.get_value_at(2,3);
    
    //Transform::translate(-_x, -_y, -_z);
    //std::cout<<_x<<", "<<_y<<", "<<_z<<std::endl; 
    //std::cout<<transformation_matrix.get_value_at(0,3)<<", "<<transformation_matrix.get_value_at(1,3)<<", "<<transformation_matrix.get_value_at(2,3)<<std::endl; 
    rotation.x += x;
    rotation.y += y;
    rotation.z += z;

     //x axis
    rotation_matrix_x.set_value_at(1,1, cos(rotation.x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(1,2, -sin(rotation.x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(2,1, sin(rotation.x*DEG_TO_RAD));
    rotation_matrix_x.set_value_at(2,2, cos(rotation.x*DEG_TO_RAD));
    //y axis
    rotation_matrix_y.set_value_at(0,0, cos(rotation.y*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(0,2, sin(rotation.y*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(2,0, -sin(rotation.y*DEG_TO_RAD));
    rotation_matrix_y.set_value_at(2,2, cos(rotation.y*DEG_TO_RAD));
    //z axis 
    rotation_matrix_z.set_value_at(0,0, cos(rotation.z*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(0,1, -sin(rotation.z*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(1,0, sin(rotation.z*DEG_TO_RAD));
    rotation_matrix_z.set_value_at(1,1, cos(rotation.z*DEG_TO_RAD));
   // std::cout<<"......................rm..............."<<std::endl;
   // rotation_matrix_z.print_matrix();
   // transformation_matrix =transformation_matrix*rotation_matrix_x *  rotation_matrix_y * rotation_matrix_z; 
    //Transform::translate(_x,_y,_z); 
    create_transformation_matrix(); 
     return this;
}

Transform* Transform::scale(double x, double y, double z)
{
    local_scale.x *= x;
    local_scale.y *= y;
    local_scale.z *= z;
    scaling_matrix.set_value_at(0,0,local_scale.x); 
    scaling_matrix.set_value_at(1,1,local_scale.y); 
    scaling_matrix.set_value_at(2,2,local_scale.z);
    create_transformation_matrix(); 
    return this;
}

void Transform::create_transformation_matrix()
{
    rotation_matrix_xyz = (rotation_matrix_x*rotation_matrix_y*rotation_matrix_z);
    transformation_matrix = translation_matrix * (rotation_matrix_xyz*scaling_matrix);
}
