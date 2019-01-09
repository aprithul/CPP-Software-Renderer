#ifndef RENDER_HPP
#define RENDER_HPP
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include <SDL2/SDL.h>

namespace rendering
{
    enum RenderMode
    {
        UNLIT    = 0,
        LIT      = 1,
        Z_BUFFER = 2
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
            float* z_buffer; 
            void draw_mesh(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces, rendering::Transform& transform);
            utils::Point objectspace_to_screenspace(const utils::Vector4d& vertex);
            void rasterize_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
            void rasterize_right_angle_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
            void draw_line(utils::Point p0, utils::Point p1, utils::Color c);
            void draw_point(utils::Point p, utils::Color color);

            SDL_Renderer* renderer = NULL;
            SDL_Window* window = NULL;
            unsigned int SCREEN_WIDTH = 1280;
            unsigned int SCREEN_HEIGHT = 720; 
     };
}

#endif
