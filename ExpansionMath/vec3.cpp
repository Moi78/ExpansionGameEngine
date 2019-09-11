#include "pch.h"

#include "vec3.h"

//Constructors

//Float
vec3f::vec3f(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
}

//Int
vec3i::vec3i(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
}

//Double
vec3d::vec3d(double x, double y, double z) : m_x(x), m_y(y), m_z(z)
{
}

//Getters

//Double
double vec3d::getX()
{
    return m_x;
}

double vec3d::getY()
{
    return m_y;
}

double vec3d::getZ()
{
    return m_z;
}

vec3d::~vec3d()
{
}

//Float
float vec3f::getX()
{
    return m_x;
}

float vec3f::getY()
{
    return m_y;
}

 float vec3f::getZ()
{
    return m_z;
}

 vec3f::~vec3f()
{
}

//Int
int vec3i::getX()
{
    return m_x;
}

int vec3i::getY()
{
    return m_y;
}

int vec3i::getZ()
{
    return m_z;
}

vec3i::~vec3i()
{
}

//Setters

//float
void vec3f::setAll(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void vec3f::setX(float x)
{
    m_x = x;
}

void vec3f::setY(float y)
{
    m_y = y;
}

void vec3f::setZ(float z)
{
    m_z = z;
}


//Int
void vec3i::setAll(int x, int y, int z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void vec3i::setX(int x)
{
    m_x = x;
}

void vec3i::setY(int y)
{
    m_y = y;
}

void vec3i::setZ(int z)
{
    m_z = z;
}

//Double
void vec3d::setAll(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void vec3d::setX(double x)
{
    m_x = x;
}

void vec3d::setY(double y)
{
    m_y = y;
}

void vec3d::setZ(double z)
{
    m_z = z;
}

//Operators

//Float
bool vec3f::operator==(const vec3f& a)
{
    return (m_x == a.m_x && m_y == a.m_y && m_z == a.m_z);
}

bool vec3f::operator!=(const vec3f& a)
{
    return (m_x != a.m_x && m_y != a.m_y && m_z != a.m_z);
}

vec3f vec3f::operator+(const vec3f& a)
{
    return vec3f(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
}

vec3f vec3f::operator-(const vec3f& a)
{
    return vec3f(m_x - a.m_x, m_y - a.m_y, m_z + a.m_z);
}

//Int
bool vec3i::operator==(const vec3i& a)
{
    return (m_x == a.m_x && m_y == a.m_y && m_z == a.m_z);
}

bool vec3i::operator!=(const vec3i& a)
{
    return (m_x != a.m_x && m_y != a.m_y && m_z != a.m_z);
}

vec3i vec3i::operator+(const vec3i& a)
{
    return vec3i(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
}

vec3i vec3i::operator-(const vec3i& a)
{
    return vec3i(m_x - a.m_x, m_y - a.m_y, m_z + a.m_z);
}

//Double
bool vec3d::operator==(const vec3d& a)
{
    return (m_x == a.m_x && m_y == a.m_y && m_z == a.m_z);
}

bool vec3d::operator!=(const vec3d& a)
{
    return (m_x != a.m_x && m_y != a.m_y && m_z != a.m_z);
}

vec3d vec3d::operator+(const vec3d& a)
{
    return vec3d(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
}

vec3d vec3d::operator-(const vec3d& a)
{
    return vec3d(m_x - a.m_x, m_y - a.m_y, m_z + a.m_z);
}

float vec3f::dotProduct(vec3f a)
{
    std::valarray<float> fa = {m_x, m_y, m_z};
    std::valarray<float> fb = {a.getX(), a.getY(), a.getZ()};
    
    float result = (fa*fb).sum();
    
    return result;
}

float vec3i::dotProduct(vec3i a)
{
    std::valarray<int> fa = {m_x, m_y, m_z};
    std::valarray<int> fb = {a.getX(), a.getY(), a.getZ()};
    
    float result = (fa*fb).sum();
    
    return result;
}

float vec3d::dotProduct(vec3d a)
{
    std::valarray<double> fa = {m_x, m_y, m_z};
    std::valarray<double> fb = {a.getX(), a.getY(), a.getZ()};
    
    float result = (fa*fb).sum();
    
    return result;
}
