#include "Model.hpp"

rendering::Model::Model(rendering::Mesh* mesh, double scaling_factor)
{
    this->mesh = mesh;
    this->scaling_factor = scaling_factor;
    
    for(int i=0; i<mesh->vertices.size(); i++)
        mesh->vertices[i] = mesh->vertices[i]*scaling_factor; 

}
