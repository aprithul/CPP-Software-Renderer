#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Matrix.hpp"
#include "Transform.hpp"

namespace rendering
{
    class Camera
    {
        public:
            rendering::Transform transform;
            utils::Matrix4x4d get_perspective_matrix();
        private:
            utils::Matrix4x4d perspective_matrix = utils::Matrix4x4d::get_identity();
    };
}
#endif
