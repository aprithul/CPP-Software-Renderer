#include "Camera.hpp"

utils::Matrix4x4d rendering::Camera::get_perspective_matrix()
{
    perspective_matrix.set_value_at(3,2,1.0/transform.position.z);
    return perspective_matrix;
}
