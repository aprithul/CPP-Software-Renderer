#include "Mesh.hpp"

rendering::Mesh::Mesh(const std::string& file_name, utils::Color color)
{
    this->color = color;
    std::string ext = file_name.substr( file_name.length()-3, 3);
    if(ext == "obj")
        load_obj( file_name, Mesh::vertices, Mesh::faces, Mesh::normals );
}
