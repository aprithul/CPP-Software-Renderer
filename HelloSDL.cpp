//Using SDL and standard IO
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
    
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 720;
float light_pan_speed = 8;
utils::Color RED={0xFF,0x00,0x00,0xFF};

void draw();
/*void draw_point(utils::Point p, utils::Color c);
void draw_line(utils::Point p0, utils::Point p1, utils::Color c);
void draw_model_wireframe_orthographic(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces);
void draw_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color c, utils::Color outline_color);
void draw_rectangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Point p3, utils::Color c,bool do_fill);
int rasterize_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color);
utils::Point objectspace_to_screenspace(const utils::Vector4d& vertex);
void draw_zbuffer();
void clear_zbuffer();
std::vector<utils::Vector4d> points;
std::vector<utils::Vector3i> faces;
float* z_buffer;
*/

bool do_quit = false;
rendering::Renderer* software_renderer; 

//Screen dimension constants
int main( int argc, char* args[] )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }           

    SDL_Event event;
    software_renderer = new rendering::Renderer("Hello Soft-Rend", 1024,768); 
    rendering::Mesh* teapot_mesh = new rendering::Mesh("teapot.obj",RED); 

    software_renderer->add_to_render(teapot_mesh);

       //The window we'll be rendering to
                   
        while(!do_quit)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                    do_quit = true;
                if(event.type == SDL_KEYUP)
                {
                    if(event.key.keysym.sym == SDLK_l)
                        software_renderer->render_mode = ( software_renderer->render_mode == rendering::LIT?rendering::UNLIT:rendering::LIT);
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
                }
            }
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

/*
void draw()
{ 

    software_renderer->clear_zbuffer();   
    software_renderer->draw();
    if( software_renderer->render_mode == rendering::Z_BUFFER)    
        software_renderer->draw_zbuffer();

}*/

/*
void draw_zbuffer()
{
    float z_max = 0;
    float z_min = 99999;
    for(int i=0; i<SCREEN_HEIGHT*SCREEN_WIDTH; i++)
    {
        if(z_buffer[i] > 0.0)
        {
            if(z_buffer[i] > z_max)
                z_max = z_buffer[i];
            if(z_buffer[i] < z_min)
                z_min = z_buffer[i];
        }
    }
   // do_quit = true;
    for(int i=0; i<SCREEN_HEIGHT; i++)
    {
        for(int j=0; j<SCREEN_WIDTH; j++)
        {
            float z_val = (z_buffer[(i*SCREEN_WIDTH)+j]-z_min) / (z_max-z_min);
            if(z_val<0.0)
            {
               // SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00, 0xFF);
               // SDL_RenderDrawPoint(renderer, j,SCREEN_HEIGHT-i);
            }
           else
           {
                float grey_level = utils::lerp(0xFF, 0x00, z_val);
                utils::Color c = {grey_level, grey_level, grey_level, 0xFF};
                utils::Point p(j-(SCREEN_WIDTH/2), i-SCREEN_HEIGHT/2);
                draw_point(p,c);
            }
        }
    }
}
void draw_point(utils::Point p, utils::Color color)
{
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderDrawPoint(renderer, p.x+SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - p.y);
}
float max = -100000;
float min =  100000;
utils::Color BLACK = {0x00,0x00,0x00,0x00};

void draw_model_wireframe_orthographic(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces)
{
    utils::Color outline_color = {0xFF, 0x00, 0x00, 0x00};
    for(int i=0; i<faces.size(); i++)
    {
        utils::Vector4d v1 = vertices[faces[i].x-1];
        utils::Vector4d v2 = vertices[faces[i].y-1];
        utils::Vector4d v3 = vertices[faces[i].z-1];
        
        utils::Point p0 = objectspace_to_screenspace(v1);
        utils::Point p1 = objectspace_to_screenspace(v2);
        utils::Point p2 = objectspace_to_screenspace(v3);
       utils::Vector4d v_a = v2-v1;
        utils::Vector4d v_b = v3-v1;
        utils::Vector4d face_normal = (v_a^v_b).get_normalized();
        utils::Vector4d light = (light_target_point - light_anchor_point).get_normalized();
        double intensity = face_normal*light;
        // if(intensity>=0.0)
        {
            intensity = utils::clamp(0.2,1.0,intensity);
            utils::Color _color = utils::lerp(BLACK,color , render_mode == LIT?intensity:1.0);  
            draw_triangle(p0,p1,p2, _color, outline_color);
           // std::cout<<intensity<<std::endl;
        }
       // draw_line( objectspace_to_screenspace(v1), objectspace_to_screenspace(v2), color);
       // draw_line( objectspace_to_screenspace(v2), objectspace_to_screenspace(v3), color);
       // draw_line( objectspace_to_screenspace(v3), objectspace_to_screenspace(v1), color);
    }
}

utils::Point objectspace_to_screenspace(const utils::Vector4d& vertex)
{
    double sqrd_dist = (vertex.get_sqrd_distance(camera_position));
    utils::Point p((int) (vertex.x*OBJ_TO_SCR_SCALE),(int) (vertex.y*OBJ_TO_SCR_SCALE), sqrd_dist);
    return p;
}

void draw_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color, utils::Color outline_color)
{
   
    // find the low, mid and high points 
    utils::Point low = p0;
    utils::Point mid = p1;
    utils::Point high = p2;
    
    // bubble sort
    if(low.y>mid.y) std::swap(low,mid);
    if(mid.y>high.y) std::swap(mid,high);
    if(low.y>mid.y) std::swap(low,mid);
    int neg_count = 2;
    // do two rasterisation passes
    neg_count += rasterize_triangle(low,mid,high,color);
    neg_count += rasterize_triangle(high,mid,low,color);
//    if(neg_count == 0)
//        std::cout<<"no triangle drawn  "<<p0.y<<"  "<<p1.y<<"  "<<p2.y<<" "<<std::endl; 
    // draw outline

    if(outline_color.a >0) 
    {
        draw_line(p0,p1,outline_color);
        draw_line(p1,p2,outline_color);
        draw_line(p2,p0,outline_color);
    }
    /////////////////////////////////////////// 
}
// p0 anchor
// p1 mid-point
// p2 other end point
int rasterize_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color)
{
   int dir = utils::sign(p1.y - p0.y);
    if(p1.y == p0.y)    
        return -1;  // couldn't rasterise, normal if no distance between anchor and midpoint
    int scan_buffer_height = std::abs(p1.y-p0.y)+1; 
    double* scan_buffer = new double[scan_buffer_height*2];
    double dx_1 = 0.0;
    double dx_2 = 0.0;
       
    //if(p1.y != p0.y)  
    dx_1 = (p1.x - p0.x)/std::abs((double)(p1.y - p0.y));
    if(p2.y != p0.y) // should never be the case
        dx_2 = (p2.x - p0.x)/std::abs((double)(p2.y - p0.y)); 
    
    scan_buffer[0] = p0.x;
    scan_buffer[1] = p0.x;

    for(int i=1; i< scan_buffer_height; i++)
    {
        scan_buffer[i*2] = scan_buffer[(i-1)*2]+dx_1;
        scan_buffer[(i*2)+1] = scan_buffer[((i-1)*2)+1]+dx_2;
   }

    for(int i=p0.y, j=0; std::abs(p0.y-i) < scan_buffer_height; i+=dir,j++)
    {
        double ratio = std::abs(p0.y-i)/(double)scan_buffer_height;
        utils::Point _p1( (int)(scan_buffer[j*2]+0.5),i, utils::lerp(p0.z, p1.z,ratio));
        utils::Point _p2( (int)(scan_buffer[(j*2)+1]+0.5),i, utils::lerp(p0.z, p2.z, ratio));
        draw_line( _p1,_p2,  color); 
    }
    delete[] scan_buffer; 
    return 1;
}

void draw_rectangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Point p3, utils::Color c, bool do_fill)
{
    draw_line(p0,p1,c);
    draw_line(p1,p2,c);
    draw_line(p2,p3,c);
    draw_line(p3,p0,c);
    
    if(do_fill)
    {
        utils::Point p_l(p0.x,p0.y);
        utils::Point p_r(p1.x,p1.y);
        while(p_l.y<p3.y)
        {
            p_l.y++;
            p_r.y++;
            draw_line(p_l,p_r,c);
        }
    }

}

void draw_line(utils::Point p0, utils::Point p1, utils::Color c)
{   
    bool steep = false;
    if(std::abs(p0.x-p1.x) < std::abs(p0.y-p1.y))
    {
        std::swap(p1.x,p1.y);
        std::swap(p0.x,p0.y);
        steep = true;
    }     
    
    if(p0.x>p1.x)
    {
        std::swap(p0,p1);
        //std::swap(p0.y,p1.y);
    }
    
    int sign = p1.y - p0.y > 0?1:-1;
    int y = p0.y;
    float delta = std::abs((p1.y-p0.y)/(float)(p1.x-p0.x));
    float cum_error = 0.f;
    for(int x = p0.x; x<p1.x; x++)
    {
        float _z = utils::lerp(p0.z, p1.z, (x-p0.x)/(double)(p1.x-p0.x));
        utils::Point p(x,y,_z);
        if(steep)
        {
            std::swap(p.x,p.y);
        }
        
        int index = (SCREEN_WIDTH*(p.y+(SCREEN_HEIGHT/2)))+ (p.x+(SCREEN_WIDTH/2));
        if(z_buffer[index]<0.0 || p.z < z_buffer[index])
        {
            z_buffer[index] = p.z;
            draw_point(p,c);
        }

        cum_error += delta;
        if(cum_error>0.5f)
        {
            y += sign;
            cum_error -= 1.0f;
        }
    }
}
*/               
