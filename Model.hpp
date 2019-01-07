#ifndef MODEL_HPP
#define MODEL_HPP

#include "Utils.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"

namespace rendering
{
    class Model
    {
        public:
            Model(rendering::Mesh* mesh, double scaling_factor);
            rendering::Mesh* mesh;
            rendering::Transform transform;
            double scaling_factor;
    };

}


#endif
