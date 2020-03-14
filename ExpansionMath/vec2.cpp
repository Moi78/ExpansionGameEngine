#include "pch.h"

#include "vec2.h"

vec2f::vec2f(float x, float y) : m_x(x), m_y(y)
{
}

vec2f::~vec2f()
{
}

float vec2f::getX()
{
    return m_x;
}

float vec2f::getY()
{
    return m_y;
}

void vec2f::setAll(float x, float y)
{
    m_x = x;
    m_y = y;
}

void vec2f::setX(float x)
{
    m_x = x;
}

void vec2f::setY(float y)
{
    m_y = y;
}

bool vec2f::operator==(const vec2f& a)
{
    return (m_x == a.m_x && m_y == a.m_y);
}

bool vec2f::operator!=(const vec2f& a)
{
    return (m_x != a.m_x && m_y != a.m_y);
}

vec2f vec2f::operator+(const vec2f& a)
{
    return vec2f(m_x + a.m_x, m_y + a.m_y);
}

vec2f vec2f::operator-(const vec2f& a)
{
    return vec2f(m_x - a.m_x, m_y - a.m_y);
}

float vec2f::dotProduct(const vec2f& a)
{
    std::valarray<float> fa = {m_x, m_y};
    std::valarray<float> fb = {a.m_x, a.m_y};
    
    float result = (fa*fb).sum();
    
    return result;
}

//Vector 2 Double

vec2d::vec2d(double x, double y) {
    m_x = x;
    m_y = y;
}

vec2d::~vec2d() {

}

double vec2d::getX() {
    return m_x;
}

double vec2d::getY() {
    return m_y;
}

void vec2d::setX(double nX) {
    m_x = nX;
}

void vec2d::setY(double nY) {
    m_y = nY;
}

void vec2d::setAll(double nX, double nY) {
    m_x = nX;
    m_y = nY;
}

float vec2d::dotProduct(vec2d const& a) {
    std::valarray<double> fa = { m_x, m_y };
    std::valarray<double> fb = { a.m_x, a.m_y };

    float result = (fa * fb).sum();

    return result;
}

bool vec2d::operator==(const vec2d& a)
{
    return (m_x == a.m_x && m_y == a.m_y);
}

bool vec2d::operator!=(const vec2d& a)
{
    return (m_x != a.m_x && m_y != a.m_y);
}

vec2d vec2d::operator+(const vec2d& a)
{
    return vec2d(m_x + a.m_x, m_y + a.m_y);
}

vec2d vec2d::operator-(const vec2d& a)
{
    return vec2d(m_x - a.m_x, m_y - a.m_y);
}