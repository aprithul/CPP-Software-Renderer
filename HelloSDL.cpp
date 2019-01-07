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
double translation_speed = 4;
utils::Color RED={0xFF,0x00,0x00,0xFF};
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

    software_renderer = new rendering::Renderer("Hello Soft-Rend", 1024,768); 
    rendering::Mesh* teapot_mesh = new rendering::Mesh("teapot.obj",RED); 
    rendering::Model* teapot_model = new rendering::Model(teapot_mesh, 100.0);
    software_renderer->add_to_render(teapot_model);
    
    while(!do_quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                do_quit = true;
            if(event.type == SDL_KEYUP)
            {
                if(event.key.keysym.sym == SDLK_l)
                    software_renderer->render_mode =  ( software_renderer->render_mode == rendering::LIT?rendering::UNLIT:rendering::LIT);
                if(event.key.keysym.sym == SDLK_z)
                    software_renderer->render_mode = rendering::Z_BUFFER;
                std::cout<<"Render Mode : "<< software_renderer->render_mode<<std::endl;
                                   
            }    
            if(event.type == SDL_KEYDOWN)
            {  
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
                if(event.key.keysym.sym == SDLK_a)
                    teapot_model->transform.translate(-translation_speed,0,0);
                if(event.key.keysym.sym == SDLK_d)
                    teapot_model->transform.translate(translation_speed,0,0);
                if(event.key.keysym.sym == SDLK_q)
                    teapot_model->transform.rotate(-5,5,0);
                if(event.key.keysym.sym == SDLK_e)
                    teapot_model->transform.rotate(5,5,0);  
             }
        }

        // rendering
        software_renderer->clear_zbuffer(); 
        software_renderer->draw();
        if(software_renderer->render_mode == rendering::Z_BUFFER)
            software_renderer->draw_zbuffer();
        software_renderer->present();
        SDL_Delay(16);                
    }
    SDL_Quit();        
    return 0;
}
