/*
 * 2023.4.7 Liu Yapeng
 * 三维向量类
 * 需要三维点类 (class Point)
 * class MyVector
 */

#include "Point.h"

class MyVector //向量
{
    private:
        Point P;
    public:
        // 构造生成
        MyVector() : P(){}
        MyVector(const Point &p1, const Point &p2) {
            P = Point(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
        }
        MyVector(const Point &p1) : P(p1){}
        MyVector(double x, double y, double z) : P(x, y, z){}
        MyVector(double v) : P(v, v, v){}
        MyVector(double *array) : P(array){}
        MyVector(const MyVector& v) : P(v.P){}
        
        double x(void) const {return P.x();}
        double y(void) const {return P.y();}
        double z(void) const {return P.z();}
        double norm() const {return std::sqrt(this->norm2());} //模长
        double norm2() const {return P[0]*P[0] + P[1]*P[1] + P[2]*P[2];} //模长的平方

        void normalize() //归一化
        {
            double len = norm();
            if(len > 0)
            {
                P[0] /= len;
                P[1] /= len;
                P[2] /= len;
            }
        }

        MyVector normalized() const { //返回归一化的向量
            MyVector y(*this);
            y.normalize();
            return y;
        }

        void negate() //取反方向
        {
            P[0] = -P[0];
            P[1] = -P[1];
            P[2] = -P[2];
        }

        double& operator[](int i) { return P[i]; }
        double operator[](int i) const { return P[i]; }
        double& operator()(int i) { return P[i]; }
        double operator()(int i) const { return P[i]; }

        int Direction(double &val) const //返回在哪个方向最大，同时val=P[x]
        {
            //val = max(P[0], max(P[1], P[2]));
            if((P[0] >= P[1]) && (P[0] >= P[2]))
            {
                val = P[0];
                return 0;
            }
            else if((P[1] >= P[0]) && (P[1] >= P[2]))
            {
                val = P[1];
                return 1;
            }
            else
            {
                val = P[2];
                return 2;
            }
        }

        double dot(const MyVector& b) const //点乘
        {
            return x() * b.x() + y() * b.y() + z() * b.z();
        }

        MyVector cross(const MyVector &b) const //叉乘
        {
            return MyVector(y() * b.z() - b.y() * z(), -(x() * b.z() - b.x() * z()), x() * b.y() - b.x() * y());
        }
        
};

MyVector operator+(const MyVector& a, const MyVector& b)
{
    return MyVector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Point operator+(const Point& a, const MyVector& b) //点+线返回点
{
    return Point(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

MyVector operator-(const MyVector& a, const MyVector& b)
{
    return MyVector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

Point operator-(const Point& a, const MyVector& b)
{
    return Point(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

MyVector operator-(const Point& a, const Point& b)
{
    return MyVector(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

double dot(const MyVector &a, const MyVector &b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

double norm(const MyVector &v){
    return sqrt(dot(v, v));
}

double norm2(const MyVector &v){
    return dot(v, v);
}

MyVector cross(const MyVector &a, const MyVector &b) //注意这个地方没有交换律
{
    return MyVector(a.y() * b.z() - b.y() * a.z(), -(a.x() * b.z() - b.x() * a.z()), a.x() * b.y() - b.x() * a.y());
}

double vectorAngle(const MyVector &a, const MyVector &b)
{
    double costheta = dot(a, b);
    double sintheta = norm(cross(a, b));
    return atan2(sintheta, costheta);
}

double signedAngle(const MyVector &a, const MyVector &b, const MyVector &n)
{
    double costheta = dot(a, b);
    double sintheta = dot(cross(a, b), n);
    return atan2(sintheta, costheta);
}

void build_orthobasis_naive(MyVector &dir, MyVector &dir1, MyVector &dir2)
{
    dir.normalize();
    if (dir[1] != 0.0 && dir[2] != 0.0)
    {
        dir1 = MyVector(1.0, 0.0, -dir[0] / dir[2]);
        dir2 = MyVector(dir[0] / dir[2], -(dir[0] * dir[0] + dir[2] * dir[2]) / (dir[1] * dir[2]), 1.0);
    }
    else if (dir[0] != 0.0 && dir[2] != 0.0)
    {
        dir1 = MyVector(-dir[1] / dir[0], 1.0, 0.0);
        dir2 = MyVector(1.0, dir[1] / dir[0], -(dir[1] * dir[1] + dir[0] * dir[0]) / (dir[0] * dir[2]));
    }
    else if (dir[0] != 0.0 && dir[1] != 0.0)
    {
        dir1 = MyVector(0.0, -dir[2] / dir[1], 1.0);
        dir2 = MyVector(-(dir[1] * dir[1] + dir[2] * dir[2]) / (dir[0] * dir[1]), 1.0, dir[2] / dir[1]);
    }
    else if (dir[0] == 0.0 && dir[1] == 0.0)
    {
        dir1 = MyVector(0.0, 1.0, 0.0);
        dir2 = MyVector(1.0, 0.0, 0.0);
    }
    else if (dir[1] == 0.0 && dir[2] == 0.0)
    {
        dir1 = MyVector(0.0, 1.0, 0.0);
        dir2 = MyVector(0.0, 0.0, 1.0);
    }
    else if (dir[0] == 0.0 && dir[2] == 0.0)
    {
        dir1 = MyVector(1.0, 0.0, 0.0);
        dir2 = MyVector(0.0, 0.0, 1.0);
    }
    dir1.normalize();
    dir2.normalize();
}

//from normal build tangent & binormal
void build_orthobasis(MyVector &normal, MyVector &tangent, MyVector &binormal) //构建正交基
{
    normal.normalize();
    if(fabs(normal[0]) > fabs(normal[1]))
        tangent = MyVector(0.0, 1.0, 0.0);
    else
        tangent = MyVector(1.0, 0.0, 0.0);
    //build binormal
    binormal = cross(tangent, normal);
    binormal.normalize();
    //correct the tangent
    tangent = cross(normal, binormal);
    tangent.normalize();

    if(binormal.norm() == 0.0 || tangent.norm() == 0.0)
        build_orthobasis_naive(normal, tangent, binormal);
    return ;
}