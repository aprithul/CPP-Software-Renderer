#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>

namespace utils
{
struct Point
{
    int x;
    int y;
    double z;
        
    Point()
    {
        x=0;
        y=0;
        z=0.0;
    }
    
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->z = 0.0;
    } 
    
    Point(int x, int y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }    
};

struct Vector3i
{
    int x;
    int y;
    int z;
};

struct Color
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
};

struct Vector4d
{
    double x;
    double y;
    double z;
    double w;
 
    Vector4d()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    Vector4d(Point& p)
    {
        x = p.x;
        y = p.y;
        z = 0;
        w = 1; 
     }
    
    Vector4d(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 0;
    }
    
    Vector4d(double x, double y, double z, double w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    
    Vector4d operator^(const Vector4d& v)
    {
        Vector4d _v;
        _v.x = y*v.z - z*v.y;
        _v.y = z*v.x - x*v.z;
        _v.z = x*v.y - y*v.x;
        _v.w = 0;
        return _v;
     }

    double operator*(const Vector4d& v)
    {
        double scaler = v.x*x + v.y*y + v.z*z + v.w*w;
        return scaler;
    }

    double get_length() const
    {
       return sqrt( x*x + y*y + z*z +w*w); 
    }

    double get_sqrd_length() const
    {
        return (x*x + y*y + z*z + w*w);
    }

    double get_sqrd_distance(const Vector4d& v) const
    {
        return (v.x-x)*(v.x-x) + (v.y-y)*(v.y-y) + (v.z-z)*(v.z-z) + (v.w-w)*(v.w-w);
    }
    
    double get_distance(const Vector4d& v) const
    {
        return sqrt((v.x-x)*(v.x-x) + (v.y-y)*(v.y-y) + (v.z-z)*(v.z-z) + (v.w-w)*(v.w-w));
    }
    
    Vector4d get_normalized() const
    {
        Vector4d _v;
        double len = get_length();      
        _v.x = x/len;
        _v.y = y/len;
        _v.z = z/len;
        _v.w = w;
        return _v;
    }
    
    Vector4d operator+(const Vector4d& v)
    {
        Vector4d _v;
        _v.x = x+v.x;
        _v.y = y+v.y;
        _v.z = z+v.z;
        _v.w = w;
        return _v;
    }

    Vector4d operator-(const Vector4d& v)
    {
        Vector4d _v;
        _v.x = x - v.x;
        _v.y = y - v.y;
        _v.z = z - v.z;
        _v.w = w;
        return _v;
    } 
};
    
    
int static sign(int val)
{
    return val<0?-1:1;
}   

int static sign(float val)
{
    return val<0.f?-1:1;
}

int static sign(double val)
{
    return val<0?-1:1;
}

float static lerp(float start, float end, float t)
{
    return start+((end-start)*t); 
}

double static lerp(double start, double end, double t)
{
    return start+((end-start)*t); 
}

double static clamp(double min, double max, double value)
{
    if(value<min)
        value = min;
    if(value>max)
        value = max;
    return value;
}

utils::Color static lerp(utils::Color start, utils::Color end, float t)
{
    utils::Color _color;
    _color.r = (int)lerp(start.r, end.r, t);
    //std::cout<<_color.r<<"  "<<start.r<<"  "<<end.r<<std::endl;
    _color.g = (int)lerp(start.g, end.g, t);
    _color.b = (int)lerp(start.b, end.b, t);
    _color.a = 0xFF;
    return _color;
}



}
#endif
