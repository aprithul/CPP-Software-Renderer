#ifndef MESH_HPP
#define MESH_HPP
#include <vector>
#include <string>
#include "Utils.hpp"
#include "ObjLoader.hpp"

namespace rendering
{
    class Mesh
    {
        public:
            std::vector<utils::Vector4d> vertices;
            std::vector<utils::Vector3i> faces;
            utils::Color color;
         
            Mesh(const std::string& file_name, utils::Color color);
            Mesh();   
    };
}

#endif
