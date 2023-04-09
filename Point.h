/*
 * 2023.4.7 Liu Yapeng
 * 三维点类
 * class Point
 */

#include<iostream>
#include<array>
#include<vector>
#include<cmath>
#include <assert.h>

class Point
{
    private:
        double P[3]; //以私有变量的形式存储
    public:
        Point() {P[0] = P[1] = P[2] = 0;}
        Point(double x, double y, double z) //初始构造
        {
            P[0] = x;
            P[1] = y;
            P[2] = z;
        }
        Point(const double *p) //第二种传入数组定义的方法
        {
            P[0] = p[0];
            P[1] = p[1];
            P[2] = p[2];
        }
        Point(const Point &p) //第三种继承定义的方法
        {
            P[0] = p.P[0];
            P[1] = p.P[1];
            P[2] = p.P[2];
        }
        
        void setPos(double x, double y, double z) //set点的值
        {
            P[0] = x;
            P[1] = y;
            P[2] = z;
        }

        double x(void) const { return P[0]; }
        double y(void) const { return P[1]; }
        double z(void) const { return P[2]; }
        double Point::operator[](int i) const { return P[i]; }
        double &Point::operator[](int i){ return P[i]; }

//        std::ostream &operator<<(std::ostream &out, Point &p) //输出呈现
//        {
//            out << p[0] << " " << p[1] << " " << p[2];
//            return out;
//        }
        double Point::distance(const Point &p) const {
            double x = P[0] - p.P[0];
            double y = P[1] - p.P[1];
            double z = P[2] - p.P[2];
            return std::sqrt(x*x + y*y + z*z);
        }

        double distance(const Point &l, const Point &r) //calc l,r的距离
        {
            double x = (l[0] - r[0]);
            double y = (l[1] - r[1]);
            double z = (l[2] - r[2]);
            return std::sqrt(x*x + y*y + z*z);
        }
        Point Point::operator * (double mult)
        {
            return Point(P[0] * mult, P[1] * mult, P[2] * mult);
        }
};
Point operator * (const Point &a, const double &b)
{
    return Point(a.x() * b, a.y() * b, a.z() * b);
}