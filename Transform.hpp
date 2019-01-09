#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include "Matrix.hpp"
#include "Utils.hpp"
#include <cmath>
#define DEG_TO_RAD (3.14159265/180.0)

namespace rendering
{
    class Transform
    {
        public:
            utils::Vector4d position;
            utils::Vector4d rotation;
            utils::Vector4d local_scale = utils::Vector4d(1.0 ,1.0, 1.0, 1.0);
            utils::Matrix4x4d transformation_matrix = utils::Matrix4x4d::get_identity(); 
            rendering::Transform* translate(double x, double y, double z);
            rendering::Transform* rotate(double x, double y, double z);
            rendering::Transform* scale(double x, double y, double z);
        private:
            utils::Matrix4x4d translation_matrix = utils::Matrix4x4d::get_identity();
            utils::Matrix4x4d scaling_matrix = utils::Matrix4x4d::get_identity();  
            utils::Matrix4x4d rotation_matrix_x = utils::Matrix4x4d::get_identity();
            utils::Matrix4x4d rotation_matrix_y = utils::Matrix4x4d::get_identity();
            utils::Matrix4x4d rotation_matrix_z = utils::Matrix4x4d::get_identity();
            void create_transformation_matrix(); 
     };
}



#endif
