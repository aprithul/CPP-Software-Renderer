#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Utils.hpp"
#include <fstream>
//void load_obj(const std::string& path, std::vector<utils::Vector4d>& obj_points, std::vector<utils::Vector3i>& obj_faces);
void load_obj(const std::string& path, std::vector<utils::Vector4d>& obj_points, std::vector<utils::Vector3i>& obj_faces, std::vector<utils::Vector4d>& obj_normals);

#endif
