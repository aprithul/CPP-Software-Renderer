#include "ObjLoader.hpp"

void load_obj(const std::string& path, std::vector<utils::Vector3d>& obj_vertices, std::vector<utils::Vector3i>& obj_faces)
{
    std::string vertex_tag = "v";
    std::string face_tag = "f";
    std::ifstream file(path);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file,line))
        {
            std::string token;
            utils::Vector3d vertex;
            utils::Vector3i face;
            bool is_vertex = false;
            bool is_face = false;
            int count = 0;
            std::stringstream line_stream(line);
            while(std::getline(line_stream,token,' '))
            {
                if(is_vertex)
                {
                    if(count == 0)
                        vertex.x = atof(token.c_str());
                    else if(count == 1)
                        vertex.y = atof(token.c_str());
                    else if(count == 2)
                        vertex.z = atof(token.c_str());
                    
                    count++;
                }
                else if(is_face)
                {
                    if(count == 0)
                        face.x = atoi(token.c_str());
                    else if(count == 1)
                        face.y = atoi(token.c_str());
                    else if(count == 2)
                        face.z = atoi(token.c_str());
                    
                    count++;   
                }
                else if(token == vertex_tag)
                {
                    is_vertex = true;
                }
                else if(token == face_tag)
                {
                    is_face = true;
                }
                
            }
            
            if(is_vertex)
                obj_vertices.push_back(vertex);
            if(is_face)
                obj_faces.push_back(face);
        }
        
    }
    else
    {
        std::cout<<"File not open"<<std::endl;
    }
}
