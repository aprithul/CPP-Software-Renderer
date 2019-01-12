#include "ObjLoader.hpp"

void load_obj(const std::string& path, std::vector<utils::Vector4d>& obj_vertices, std::vector<utils::Vector3i>& obj_faces, std::vector<utils::Vector4d>& obj_normals)
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
            utils::Vector4d vertex;
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
            {
                vertex.w = 1.0;
                obj_vertices.push_back(vertex);
                obj_normals.push_back(utils::Vector4d());
            }
            if(is_face)
                obj_faces.push_back(face);
        }
        
        // get normals
        for( int i=0; i<obj_faces.size(); i++)
        {
            utils::Vector3i face = obj_faces[i];
            utils::Vector4d v1 = obj_vertices[face.x-1];
            utils::Vector4d v2 = obj_vertices[face.y-1];
            utils::Vector4d v3 = obj_vertices[face.z-1];

            utils::Vector4d v_a = v2-v1;
            utils::Vector4d v_b = v3-v1;
            utils::Vector4d face_normal = (v_a^v_b).get_normalized();
            
            obj_normals[face.x-1] = obj_normals[face.x-1] + face_normal;
            obj_normals[face.y-1] = obj_normals[face.y-1] + face_normal;
            obj_normals[face.z-1] = obj_normals[face.z-1] + face_normal;
        }

        for(int i=0; i<obj_normals.size(); i++)
            obj_normals[i] = obj_normals[i].get_normalized();
    }
    else
    {
        std::cout<<"File not open"<<std::endl;
    }
}
