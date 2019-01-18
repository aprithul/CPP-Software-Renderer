#include "Renderer.hpp"
rendering::Renderer::Renderer(const std::string& window_title, int screen_width, int screen_height):light_direction(0.577, 0.577, -0.577), light_target_point(screen_width/2, screen_height/2,0), light_anchor_point(screen_width/2, screen_height/2,-100.0)
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
    z_buffer_size = SCREEN_WIDTH*SCREEN_HEIGHT; 
    z_buffer = new double[z_buffer_size];
    render_mode = UNLIT;
    int max = std::numeric_limits<int>::max(); 
}

rendering::Renderer::~Renderer()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer); 
    delete[] scan_buffer;
}

void rendering::Renderer::add_to_render(Model* model)
{
    models_to_draw.push_back(model);
}

void rendering::Renderer::clear_zbuffer()
{
    for(int i=0; i<z_buffer_size; i++)
    {
        z_buffer[i] = -std::numeric_limits<int>::max();
    } 
}

void rendering::Renderer::draw_zbuffer()
{
    float z_max = 0;
    float z_min = std::numeric_limits<int>::max();
    for(int i=0; i<z_buffer_size; i++)
    {
        if(z_buffer[i] != -std::numeric_limits<int>::max())
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
            double z_val = (z_buffer[(i*SCREEN_WIDTH)+j]-z_min) / (z_max-z_min);
            if(z_val>-1)
            {
                unsigned int grey_level = utils::lerp(0x00, 0xFF, z_val);
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
    for(int i=0; i<models_to_draw.size(); i++)
    {
       draw_mesh(models_to_draw[i]->mesh->color, models_to_draw[i]->mesh->vertices, models_to_draw[i]->mesh->faces,models_to_draw[i]->mesh->normals, models_to_draw[i]->transform);
    }
}

void rendering::Renderer::present()
{
    SDL_RenderPresent(renderer);
}

void rendering::Renderer::draw_mesh(utils::Color color, std::vector<utils::Vector4d>& vertices, std::vector<utils::Vector3i>& faces, std::vector<utils::Vector4d>& normals, rendering::Transform& transform)
{
    utils::Matrix4x4d ptm = camera.get_perspective_matrix()*transform.transformation_matrix;
    utils::Matrix4x4d normal_transform_matrix =  (camera.get_perspective_matrix() * transform.rotation_matrix_xyz);
//    normal_transform_matrix = utils::Matrix4x4d::get_identity();
    for(int i=0; i<faces.size(); i++)
    {
        utils::Vector4d v1 = ptm * vertices[faces[i].x-1];
        v1 = v1*(1/v1.w);
        utils::Vector4d v2 = ptm * vertices[faces[i].y-1];
        v2 = v2*(1/v2.w);
        utils::Vector4d v3 = ptm * vertices[faces[i].z-1];
        v3 = v3*(1/v3.w);
       /* v1 = vertices[faces[i].x-1];
        v2 = vertices[faces[i].y-1];
        v3 = vertices[faces[i].z-1];
        */ 
        utils::Vector4d n1 =  (normal_transform_matrix * normals[faces[i].x-1]).get_normalized(); 
        utils::Vector4d n2 =  (normal_transform_matrix * normals[faces[i].y-1]).get_normalized(); 
        utils::Vector4d n3 =  (normal_transform_matrix * normals[faces[i].z-1]).get_normalized(); 
        
        utils::Vector4d _n = (n1+n2+n3).get_normalized();
        
        utils::Point p0 = objectspace_to_screenspace(v1, n1);
        //p0.normal = n1;
        utils::Point p1 = objectspace_to_screenspace(v2, n2);
        //p1.normal = n2;
        utils::Point p2 = objectspace_to_screenspace(v3, n3);
        //p2.normal = n3;
        if(render_mode == WIREFRAME)
        {
            if(std::abs(vertices[faces[i].x-1].x)>100)
            {
                std::cout<<"before: "<<vertices[faces[i].x-1].x<<std::endl;
                std::cout<<"after: "<<p0.x<<std::endl;
            }
            rendering::Renderer::draw_line_any(p1,p2,color);
            rendering::Renderer::draw_line_any(p2,p0,color);
            rendering::Renderer::draw_line_any(p0,p1,color);
        }    
        else if(render_mode == SMOOTH)
        {
            
            rasterize_triangle(p0,p1,p2, color);
            //rasterize_triangle(p0,p1,p2, color);
        }
        else
        {
            utils::Vector4d v_a = v2-v1;
            utils::Vector4d v_b = v3-v1;
            utils::Vector4d face_normal = (v_a^v_b).get_normalized();
            utils::Vector4d light = (light_target_point - light_anchor_point).get_normalized();
            double intensity =face_normal *light;
            if(intensity>=0.0)
            {
                intensity = utils::clamp(0.25,1.0,intensity);
                utils::Color _color = utils::lerp(BLACK,color , render_mode == LIT?intensity:1.0);
                //rasterize_triangle(p0,p1,p2, _color);
                //_color = color; 
                rasterize_triangle(p0,p1,p2, _color);
            } 
         }  
    }
}

utils::Point rendering::Renderer::objectspace_to_screenspace(const utils::Vector4d& vertex, const utils::Vector4d& normal)
{
   /* double sqrd_dist = (vertex.get_sqrd_distance(camera.transform.position));
    if(sqrd_dist<0)
        std::cout<<"imp"<<std::endl;
    */
    utils::Point p((int) (vertex.x),(int) (vertex.y), (int)(vertex.z), normal);
    return p;
}

utils::Vector4d rendering::Renderer::get_barycentric(utils::Vector4d _p, utils::Vector4d a, utils::Vector4d b, utils::Vector4d c)
{
    utils::Vector4d v0 = b - a, v1 = c - a, v2 = _p - a;
    double d00 = v0*v0;
    double d01 = v0*v1;
    double d11 = v1*v1;
    double d20 = v2*v0;
    double d21 = v2*v1;
    double denom = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    if(denom < 1)
    {
        u = -1.0;
        v = 1.0;
        w = 1.0;
    } 
    
    return utils::Vector4d(u,v,w);
}

void rendering::Renderer::rasterize_triangle_barycentric(utils::Point p1, utils::Point p2, utils::Point p3, utils::Color color)
{
    int dir = utils::sign(p2.y - p1.y);
    if(p2.y != p1.y)    
    {  // couldn't rasterise, normal if no distance between anchor and midpoint
        int scan_buffer_height = std::abs(p2.y-p1.y)+1; 
        double dx_1 = 0.0;
        double dx_2 = 0.0;
        dx_1 = (p2.x - p1.x)/std::abs((double)(p2.y - p1.y));
        if(p3.y != p1.y) // should never be the case
            dx_2 = (p3.x - p1.x)/std::abs((double)(p3.y - p1.y)); 
        
       // scan_buffer[0] = p0.x;
       // scan_buffer[1] = p0.x;
       
        double dx_1_acum = p1.x;
        double dx_2_acum = p1.x;
         
       // for(int i= p0.y, j=0; std::abs(i-p1.y) != 0; i+=dir)
        int i=p1.y-dir;
        int j=0; 
        do{
            i+=dir;
            if(i<=(SCREEN_HEIGHT/2)+(scan_buffer_padding/2) && i>=-(SCREEN_HEIGHT/2)-(scan_buffer_padding/2))
            {
                scan_buffer[j*2] = dx_1_acum;
                scan_buffer[(j*2)+1] = dx_2_acum;
                j++;
            }
            
            dx_1_acum += dx_1;
            dx_2_acum += dx_2;
      
         }while(std::abs(i-p2.y) != 0);

        for(int i=p1.y, j=0; std::abs(p1.y-i) < scan_buffer_height; i+=dir,j++)
        {
            //utils::Point _p1( (int)(scan_buffer[j*2]+0.5),i, utils::lerp(p0.z, p1.z,ratio),utils::lerp(p0.normal, p1.normal, ratio));
           // utils::Point _p2( (int)(scan_buffer[(j*2)+1]+0.5),i, utils::lerp(p0.z, p2.z, ratio), utils::lerp(p0.normal, p2.normal, ratio));
            //utils::Point _p1( (int)(scan_buffer[j*2]+0.5),i, _z,utils::lerp(p0.normal, p1.normal, ratio));
            //utils::Point _p2( (int)(scan_buffer[(j*2)+1]+0.5),i, _z, utils::lerp(p0.normal, p2.normal, ratio));
 //draw_line( _p1,_p2,  color);
            int _low = (int)(scan_buffer[j*2]);
            int _high = (int)(scan_buffer[(j*2)+1]);
            if(_low>_high) std::swap(_low, _high);
           // _low -= 5;
           // _high+= 5;
            for( int k = _low ; k<=_high; k++)
            {
                utils::Color _color = color; 
                utils::Point p(k,i);
                
                utils::Vector4d _p(p.x,p.y);
                utils::Vector4d a(p1.x,p1.y);
                utils::Vector4d b(p2.x,p2.y);
                utils::Vector4d c(p3.x,p3.y);
                utils::Vector4d bc = rendering::Renderer::get_barycentric(_p, a, b, c); 
                       
               // double w1 = rendering::Renderer::get_w1(p, p1,p2,p3);
               // double w2 = rendering::Renderer::get_w2(p, w1, p1,p2,p3);
               // if( w1 >= 0.0 && w2 >= 0.0 && (w1+w2) <= 1.0)
                if(bc.x<-0.0000001 || bc.y<-0.0000001 || bc.z<-0.0000001) //floating point error?
                {
                    continue;
                }
                else 
                {
                    p.z = p1.z*bc.x + p2.z*bc.y + p3.z*bc.z;
                    if(p.x>= -SCREEN_WIDTH/2 && p.x<=SCREEN_WIDTH/2 && p.y>=-SCREEN_HEIGHT/2 && p.y<=SCREEN_HEIGHT/2)
                    {
                        int index = (SCREEN_WIDTH*(p.y+(SCREEN_HEIGHT/2)))+ (p.x+(SCREEN_WIDTH/2));
                        if(index<z_buffer_size && (z_buffer[index]==(-std::numeric_limits<int>::max()) || p.z > z_buffer[index]))
                        {
                            z_buffer[index] = p.z;
                            if(render_mode == SMOOTH)
                            {
                                utils::Vector4d _normal = (p1.normal*bc.x + p2.normal*bc.y + p3.normal*bc.z).get_normalized();
                                utils::Vector4d light = (light_target_point - light_anchor_point).get_normalized();
                                double intensity = _normal *light;
                                if(intensity<0.0)
                                    continue;
                                intensity = utils::clamp(0.25,1.0,intensity);
                                _color = utils::lerp(BLACK,color, intensity);  
                            }
                             
                            rendering::Renderer::draw_point(p, _color);
                        }
                    }
                }
            }  

          }
    }
    return; 
    // find the bounding rectangle
    // x bounds
/*
    int x_min = std::numeric_limits<int>::max();
    int x_max = -std::numeric_limits<int>::max();
    if(p1.x>p2.x)
    {
        x_min = p2.x;
        x_max = p1.x;
    }
    else
    {
        x_min = p1.x;
        x_max = p2.x;
    }
    if(p3.x < x_min)
        x_min = p3.x-10;
    if(p3.x > x_max)
        x_max = p3.x+10;    
    
    // y bounds
    int y_min = std::numeric_limits<int>::max();
    int y_max = -std::numeric_limits<int>::max();
    
    if(p1.y>p2.y)
    {
        y_min = p2.y;
        y_max = p1.y;
    }
    else
    {
        y_min = p1.y;
        y_max = p2.y;
    }
    if(p3.y < y_min)
        y_min = p3.y-10;
    if(p3.y > y_max)
        y_max = p3.y+10;


    // do rasterization
    
    // find w1 and w2
    bool no_point_rendered = true; 
    
    for(int i = x_min; i<=x_max; i++)
    {
        for(int j = y_min; j<=y_max; j++)
        {
            utils::Color _color = color; 
            utils::Point p(i, j);
            
            utils::Vector4d _p(p.x,p.y);
            utils::Vector4d a(p1.x,p1.y);
            utils::Vector4d b(p2.x,p2.y);
            utils::Vector4d c(p3.x,p3.y);
            utils::Vector4d bc = rendering::Renderer::get_barycentric(_p, a, b, c); 
                   
           // double w1 = rendering::Renderer::get_w1(p, p1,p2,p3);
           // double w2 = rendering::Renderer::get_w2(p, w1, p1,p2,p3);
           // if( w1 >= 0.0 && w2 >= 0.0 && (w1+w2) <= 1.0)
            if(bc.x<-0.0000001 || bc.y<-0.0000001 || bc.z<-0.0000001) //floating point error?
            {
                continue;
            }
            else 
            {
                p.z = p1.z*bc.x + p2.z*bc.y + p3.z*bc.z;
                if(p.x>= -SCREEN_WIDTH/2 && p.x<=SCREEN_WIDTH/2 && p.y>=-SCREEN_HEIGHT/2 && p.y<=SCREEN_HEIGHT/2)
                {
                    int index = (SCREEN_WIDTH*(p.y+(SCREEN_HEIGHT/2)))+ (p.x+(SCREEN_WIDTH/2));
                    if(index<z_buffer_size && (z_buffer[index]==(-std::numeric_limits<int>::max()) || p.z > z_buffer[index]))
                    {
                        z_buffer[index] = p.z;
                        if(render_mode == SMOOTH)
                        {
                            utils::Vector4d _normal = (p1.normal*bc.x + p2.normal*bc.y + p3.normal*bc.z).get_normalized();
                            utils::Vector4d light = (light_target_point - light_anchor_point).get_normalized();
                            double intensity = _normal *light;
                            if(intensity<0.0)
                                continue;
                            intensity = utils::clamp(0.25,1.0,intensity);
                            _color = utils::lerp(BLACK,color, intensity);  
                        }
                         
                        rendering::Renderer::draw_point(p, _color);
                        no_point_rendered = false;
                    }
                }
            } 
         }
    }*/
    /*if(no_point_rendered)
    {
        double area = (p1.x*(p2.y-p3.y)) + (p2.x*(p3.y-p1.y)) +  (p3.x*(p1.y-p2.y)); 
        area = area/2.0;
        std::cout<<"area: "<<area<<std::endl; 
     }*/
}

double rendering::Renderer::get_w1(utils::Point P, utils::Point A, utils::Point B, utils::Point C)
{
    double numerator = A.x*(C.y-A.y) + (P.y-A.y)*(C.x-A.x) - P.x*(C.y-A.y);
    double denominator = (B.y-A.y)*(C.x-A.x)-(B.x-A.x)*(C.y-A.y);
    
    return (numerator/denominator);

}

double rendering::Renderer::get_w2(utils::Point P, double w1, utils::Point A, utils::Point B, utils::Point C)
{
    double numerator = P.y-A.y-w1*(B.y-A.y);
    double denominator = C.y - A.y;
    
    return (numerator/denominator);
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
    rasterize_triangle_barycentric(low,mid,high,color);
    rasterize_triangle_barycentric(high,mid,low,color);
 
}

void rendering::Renderer::rasterize_right_angle_triangle(utils::Point p0, utils::Point p1, utils::Point p2, utils::Color color)
{
    int dir = utils::sign(p1.y - p0.y);
    if(p1.y != p0.y)    
    {  // couldn't rasterise, normal if no distance between anchor and midpoint
        int scan_buffer_height = std::abs(p1.y-p0.y)+1; 
        
        double dx_1 = 0.0;
        double dx_2 = 0.0;
        double _z = (p0.z+p1.z+p2.z)/3.0; 
        //if(p1.y != p0.y)  
        dx_1 = (p1.x - p0.x)/std::abs((double)(p1.y - p0.y));
        if(p2.y != p0.y) // should never be the case
            dx_2 = (p2.x - p0.x)/std::abs((double)(p2.y - p0.y)); 
        
       // scan_buffer[0] = p0.x;
       // scan_buffer[1] = p0.x;
       
        double dx_1_acum = p0.x;
        double dx_2_acum = p0.x;
         
       // for(int i= p0.y, j=0; std::abs(i-p1.y) != 0; i+=dir)
        int i=p0.y-dir;
        int j=0; 
        do{
            i+=dir;
            if(i<=(SCREEN_HEIGHT/2)+(scan_buffer_padding/2) && i>=-(SCREEN_HEIGHT/2)-(scan_buffer_padding/2))
            {
                scan_buffer[j*2] = dx_1_acum;
                scan_buffer[(j*2)+1] = dx_2_acum;
                j++;
            }
            
            dx_1_acum += dx_1;
            dx_2_acum += dx_2;
      
         }while(std::abs(i-p1.y) != 0);

        /*for(int i=1; i< scan_buffer_height; i++)
        {
            scan_buffer[i*2] = scan_buffer[(i-1)*2]+dx_1;
            scan_buffer[(i*2)+1] = scan_buffer[((i-1)*2)+1]+dx_2;
        }*/

        for(int i=p0.y, j=0; std::abs(p0.y-i) < scan_buffer_height; i+=dir,j++)
        {
            double ratio = std::abs(p0.y-i)/(double)scan_buffer_height;
            //utils::Point _p1( (int)(scan_buffer[j*2]+0.5),i, utils::lerp(p0.z, p1.z,ratio),utils::lerp(p0.normal, p1.normal, ratio));
           // utils::Point _p2( (int)(scan_buffer[(j*2)+1]+0.5),i, utils::lerp(p0.z, p2.z, ratio), utils::lerp(p0.normal, p2.normal, ratio));
            utils::Point _p1( (int)(scan_buffer[j*2]+0.5),i, _z,utils::lerp(p0.normal, p1.normal, ratio));
            utils::Point _p2( (int)(scan_buffer[(j*2)+1]+0.5),i, _z, utils::lerp(p0.normal, p2.normal, ratio));
 draw_line( _p1,_p2,  color); 
        }
    } 
}

void rendering::Renderer::draw_line_any(utils::Point p0, utils::Point p1, utils::Color c)
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
            
    for(int x = p0.x; x<=p1.x; x++)
    {
        utils::Point p(x,y);
        if(steep)
        {
            std::swap(p.x,p.y);
        }
        int index = (SCREEN_WIDTH*(p.y+(SCREEN_HEIGHT/2)))+ (p.x+(SCREEN_WIDTH/2));
        
        if(p.x>= -SCREEN_WIDTH/2 && p.x<=SCREEN_WIDTH/2 && p.y>=-SCREEN_HEIGHT/2 && p.y<=SCREEN_HEIGHT/2)
        {
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
            
    for(int x = p0.x; x<=p1.x; x++)
    {
        double _z = 0.0; //utils::lerp(p0.z, p1.z, (x-p0.x)/(double)(p1.x-p0.x));
        utils::Vector4d _n;
        if(p0.x==p1.x)
        {
            _z = p0.z;
            _n = p0.normal;
        }
        else{
            double ratio =  (x-p0.x)/(double)(p1.x-p0.x);
            _z = utils::lerp(p0.z, p1.z, ratio);
            _z = p0.z;
            _n = utils::lerp(p0.normal, p1.normal, ratio);
        }
        utils::Point p(x,y,_z,_n);
        if(steep)
        {
            std::swap(p.x,p.y);
        }
        int index = (SCREEN_WIDTH*(p.y+(SCREEN_HEIGHT/2)))+ (p.x+(SCREEN_WIDTH/2));
        
       // if(p.x>= -screen_width/2 && p.x<=screen_width/2 && p.y>=-screen_height/2 && p.y<=screen_height/2)
        if(p.x>= -SCREEN_WIDTH/2 && p.x<=SCREEN_WIDTH/2 && p.y>=-SCREEN_HEIGHT/2 && p.y<=SCREEN_HEIGHT/2)
        {
            if(index<z_buffer_size && (z_buffer[index]<0.0 || p.z < z_buffer[index]))
            {
                z_buffer[index] = p.z;
              
                if(render_mode == SMOOTH) 
                {
                    utils::Vector4d light = (light_target_point - light_anchor_point).get_normalized();
                    double intensity = p.normal *light;
                    intensity = utils::clamp(0.25,1.0,intensity);
                    utils::Color _color = utils::lerp(BLACK,c, intensity);  
                    draw_point(p,_color);
                }
                else
                    draw_point(p,c);
            }
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

