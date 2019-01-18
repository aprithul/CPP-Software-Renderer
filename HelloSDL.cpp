#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "Global.hpp"
#include "Utils.hpp"
#include "ObjLoader.hpp"
#include "Matrix.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "Model.hpp"
 
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 720;
float light_pan_speed = 8;
double translation_speed = 5;
utils::Color RED={0xFF,0x00,0x00,0xFF};
utils::Color GREEN={0x00,0xFF,0x00,0xFF};
SDL_Event event;
bool do_quit = false;
void draw();
rendering::Renderer* software_renderer; 

//Screen dimension constants
int main( int argc, char* args[] )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }           

    software_renderer = new rendering::Renderer("Pri's Renderer", 1024, 768); 
    
    rendering::Mesh* teapot_mesh = new rendering::Mesh("head.obj",RED); 
    rendering::Model* teapot_model = new rendering::Model(teapot_mesh, 300.0);
    
    software_renderer->add_to_render(teapot_model);
    software_renderer->camera.transform.position = utils::Vector4d(0,0,1000);
    while(!do_quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                do_quit = true;
            if(event.type == SDL_KEYUP)
            {
                if(event.key.keysym.sym == SDLK_l)
                    software_renderer->render_mode = (rendering::RenderMode) (( software_renderer->render_mode + 1)%rendering::COUNT);
                std::cout<<"Render Mode : "<< software_renderer->render_mode<<std::endl;
                                   
            }    
            if(event.type == SDL_KEYDOWN)
            { 
                if(event.key.keysym.sym == SDLK_p)
                    software_renderer->perspective = !software_renderer->perspective; 
                if(event.key.keysym.sym == SDLK_RIGHT)
                    software_renderer->light_target_point.x+=light_pan_speed;
                if(event.key.keysym.sym == SDLK_LEFT)
                    software_renderer->light_target_point.x-=light_pan_speed;
                if(event.key.keysym.sym == SDLK_UP)
                    software_renderer->light_target_point.y+=light_pan_speed;
                if(event.key.keysym.sym == SDLK_DOWN)
                    software_renderer->light_target_point.y-=light_pan_speed;

                if(event.key.keysym.sym == SDLK_w)
                    teapot_model->transform.translate(0,translation_speed,0);
                if(event.key.keysym.sym == SDLK_s)
                    teapot_model->transform.translate(0,-translation_speed,0);
                if(event.key.keysym.sym == SDLK_a){
                    teapot_model->transform.translate(-translation_speed,0,0);
                }
                if(event.key.keysym.sym == SDLK_d){
                    teapot_model->transform.translate(translation_speed,0,0);
                }
                if(event.key.keysym.sym == SDLK_1)
                    teapot_model->transform.rotate(5,0,0);
                if(event.key.keysym.sym == SDLK_2)
                    teapot_model->transform.rotate(-5,0,0);  
                if(event.key.keysym.sym == SDLK_3)
                    teapot_model->transform.rotate(0,5,0);
                if(event.key.keysym.sym == SDLK_4)
                    teapot_model->transform.rotate(0,-5,0);  
                if(event.key.keysym.sym == SDLK_5)
                    teapot_model->transform.rotate(0,0,5);
                if(event.key.keysym.sym == SDLK_6)
                    teapot_model->transform.rotate(0,0,-5);
                if(event.key.keysym.sym == SDLK_7)
                    teapot_model->transform.scale(1.2,1.2,1.2);
                if(event.key.keysym.sym == SDLK_8)
                    teapot_model->transform.scale(1/1.2,1/1.2,1/1.2);
                if(event.key.keysym.sym == SDLK_g)  
                    software_renderer->camera.transform.position.z+=25;
                if(event.key.keysym.sym == SDLK_h)  
                    software_renderer->camera.transform.position.z-=25;
            }
        }

        // rendering
        software_renderer->clear_zbuffer(); 
        software_renderer->draw();
        if(software_renderer->render_mode == rendering::Z_BUFFER)
            software_renderer->draw_zbuffer();
        software_renderer->present();
        //SDL_Delay(16);                
    }
    SDL_Quit();        
    return 0;
}
