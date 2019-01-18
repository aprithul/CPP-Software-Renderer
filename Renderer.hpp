#ifndef RENDER_HPP
#define RENDER_HPP
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include <SDL2/SDL.h>
#include <math.h>
#include <limits>

namespace rendering
{
    enum RenderMode
    {
        UNLIT    = 0,
        LIT      = 1,
        SMOOTH   = 2,
        Z_BUFFER = 3,
        WIREFRAME= 4,
        COUNT    = 5
    };

    
    class Renderer
    {
        public:
            //double OBJ_TO_SCR_SCALE = 100.0;
            utils::Vector4d light_direction;
            rendering::Camera camera; 
            utils::Vector4d light_target_point;
            utils::Vector4d light_anchor_point;
            rendering::RenderMode render_mode;
            bool perspective;
            
            Renderer(const std::string& window_title, int screen_width, int screen_height);
            void draw();
            void present();
            void draw_zbuffer();
            void clear_zbuffer();
            void add_to_render(Model* model);
            ~Renderer();

        private:
            std::vector<Model*> models_to_draw;
            utils::Color BLACK = {0x00, 0x00, 0x00, 0xFF};
            double* z_buffer;
            int z_buffer_size; 
            void draw_mesh(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces,std::vector<utils::Vector4d>& normals, rendering::Transform& transform);
            utils::Point objectspace_to_screenspace(const utils::Vector4d& vertex, const utils::Vector4d& normal);
            void rasterize_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
            void rasterize_triangle_barycentric(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
            void rasterize_right_angle_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
            void draw_line(utils::Point p0, utils::Point p1, utils::Color c);
            void draw_line_any(utils::Point p0, utils::Point p1, utils::Color c);
            void draw_point(utils::Point p, utils::Color color);
            double get_w1(utils::Point P, utils::Point A, utils::Point B, utils::Point C);
            double get_w2(utils::Point P, double w1, utils::Point A, utils::Point B, utils::Point C);
            SDL_Renderer* renderer = NULL;
            SDL_Window* window = NULL;
            int SCREEN_WIDTH = 1280;
            int SCREEN_HEIGHT = 720;
            int scan_buffer_padding = 16; 
            double* scan_buffer = new double[(SCREEN_HEIGHT+scan_buffer_padding)*2];
            utils::Vector4d get_barycentric(utils::Vector4d _p, utils::Vector4d a, utils::Vector4d b, utils::Vector4d c); 
    };
}

#endif
