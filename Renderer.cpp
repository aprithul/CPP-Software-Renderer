#include "Renderer.hpp"

rendering::Renderer::Renderer(const std::string& window_title, int screen_width, int screen_height):light_direction(0.577, 0.577, -0.577),camera_position(0,0,10.0), light_target_point(screen_width/2, screen_height/2,0), light_anchor_point(screen_width/2, screen_height/2,-100.0)
{
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    std::cout<<"SW, SH: "<<SCREEN_WIDTH<<","<<SCREEN_HEIGHT<<std::endl;
    window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);  
    if(window == NULL)
    {
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window,0,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    z_buffer = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
    render_mode = Z_BUFFER;
}

rendering::Renderer::~Renderer()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer); 
}

void rendering::Renderer::add_to_render(Mesh* mesh)
{
    meshes_to_draw.push_back(mesh);
}

void rendering::Renderer::clear_zbuffer()
{
    for(int i=0; i<SCREEN_HEIGHT; i++)
    {
        for(int j=0; j<SCREEN_WIDTH; j++)
        {
            z_buffer[(i*SCREEN_WIDTH)+j] = -1;
        }
    } 
}

void rendering::Renderer::draw_zbuffer()
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
            if(z_val>=0.0)
            {
                unsigned int grey_level = utils::lerp(0xFF, 0x00, z_val);
                utils::Color c = {grey_level, grey_level, grey_level, 0xFF};
                utils::Point p(j-(SCREEN_WIDTH/2), i-SCREEN_HEIGHT/2);
                draw_point(p,c);
            }
        }
    }
}

void rendering::Renderer::draw()
{
   //std::cout<<"drawn"<<std::endl; 
    SDL_SetRenderDrawColor(renderer,0x00,0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    for(int i=0; i<meshes_to_draw.size(); i++)
    {
       draw_mesh(meshes_to_draw[i]->color, meshes_to_draw[i]->vertices, meshes_to_draw[i]->faces);
     }
    
}

void rendering::Renderer::present()
{
    SDL_RenderPresent(renderer);
}

void rendering::Renderer::draw_mesh(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces)
{
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
        intensity = utils::clamp(0.2,1.0,intensity);
        utils::Color _color = utils::lerp(BLACK,color , render_mode == LIT?intensity:1.0);  
        rasterize_triangle(p0,p1,p2, _color);
    }
}

utils::Point rendering::Renderer::objectspace_to_screenspace(const utils::Vector4d& vertex)
{
    double sqrd_dist = (vertex.get_sqrd_distance(camera_position));
    utils::Point p((int) (vertex.x*OBJ_TO_SCR_SCALE),(int) (vertex.y*OBJ_TO_SCR_SCALE), sqrd_dist);
    return p;
}

void rendering::Renderer::rasterize_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color)
{
    // find the low, mid and high points 
    utils::Point low = p0;
    utils::Point mid = p1;
    utils::Point high = p2;
    
    // bubble sort
    if(low.y>mid.y) std::swap(low,mid);
    if(mid.y>high.y) std::swap(mid,high);
    if(low.y>mid.y) std::swap(low,mid);
    // do two rasterisation passes
    rasterize_right_angle_triangle(low,mid,high,color);
    rasterize_right_angle_triangle(high,mid,low,color);
 
}

void rendering::Renderer::rasterize_right_angle_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color)
{
    int dir = utils::sign(p1.y - p0.y);
    if(p1.y != p0.y)    
    {  // couldn't rasterise, normal if no distance between anchor and midpoint
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
    } 
}

void rendering::Renderer::draw_line(utils::Point p0, utils::Point p1, utils::Color c)
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

void rendering::Renderer::draw_point(utils::Point p, utils::Color color)
{
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderDrawPoint(renderer, p.x+SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - p.y);
}
