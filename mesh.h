/*
 * 2023.4.9 Liu Yapeng
 * 半边表
 * 需要三维点类、三维向量类 (class Point, class MyVector)
 * class MyVert, class MyHalfedge, class MyEdge, class MyPolyFace, class PolyMesh
 * 点, 半边, 全边, 三角面, 三维物体网格
 * 其中PolyMesh中为主要的功能，其中部分功能做了定义，具体的实现在mesh.cpp中
 */
#include "Point.h"
#include "MyVector.h"

class MyVert;
class MyEdge;
class MyHalfedge;
class MyPolyFace;

class MyVert
{
    private:
        int index;
        Point point;    
        MyHalfedge * he;
    public:
        MyVert() : index(-1), point(0, 0, 0), he(nullptr){}
        MyVert(double x, double y, double z) : index(-1), point(x, y, z), he(nullptr){}
        ~MyVert() {index = -1;}
    public:
        //基础调用
        MyHalfedge * const halfEdge() {return he; }
        const MyHalfedge * const halfEdge() const { return he; }
        void setHalfedge (MyHalfedge * _he) { he = _he; }
        void setPosition (Point p) { point = p; }
        void setPosition (double x, double y, double z) { point = Point(x, y, z); }
        Point position() { return point; }
        const Point& position() const { return point; }
        const Point & position() const { return point; }

        double x() const { return point.x(); }
        double y() const { return point.y(); }
        double z() const { return point.z(); }
        
        int index() const { return index; }
        void Setindex(int ind) { index = ind; }
        bool isIsolated() const { return (he == nullptr); } //判断是否是孤立点

        void adjustOutgoingHalfedge();
};

class MyHalfedge
{
    private:
        int index;
        MyVert * v_; //半边对应的起点
        MyEdge * e; //对应的全边
        MyPolyFace * poly_face; //对应的面be
        MyHalfedge * nex, * pre; //后继，前驱
        MyHalfedge * twin; //对应的另一条半边
    public:
        MyHalfedge() : index(-1), nex(nullptr), pre(nullptr), twin(nullptr),
                        v_(nullptr), e(nullptr), poly_face(nullptr){};
        MyHalfedge(MyHalfedge * nex1, MyHalfedge * pre1, MyHalfedge * twin1, MyVert * v, MyEdge * e1, MyPolyFace * p)
                : index(-1), nex(nex1), pre(pre1), twin(twin1), v_(v), e(e1), poly_face(p) {}
        ~MyHalfedge() { index = -1; }
    public:
        //基础调用
        MyHalfedge * const next() { return nex; }
        MyHalfedge * const prev() { return pre; }
        MyHalfedge * const twinn() { return twin; }
        MyVert * const fromVert() { return v_; }
        MyVert * const toVert() { return next() -> fromVert(); }
        MyEdge * const edge() { return e; }
        MyPolyFace * const polygon() { return poly_face; }

        MyHalfedge * const rotateNext() { return twinn() -> next(); } //对偶边的next
        MyHalfedge * const rotatePrev() { return prev() -> twinn(); } //前驱的对偶

        const MyHalfedge * const next() const { return nex; }
        const MyHalfedge * const prev() const { return pre; }
        const MyHalfedge * const twinn() const { return twin; }
        const MyVert * const fromVert() const { return v_; }
        const MyVert * const toVert() const { return next() -> fromVert(); }
        const MyEdge * const edge() const { return e; }
        const MyPolyFace * const polygon() const { return poly_face; }

        void setNext(MyHalfedge * next) { nex = next; }
        void setPrev(MyHalfedge * prev) { pre = prev; }
        void setTwin(MyHalfedge * twinn) { twin = twinn; }
        void setVert(MyVert * vert) { v_ = vert; }
        void setEdge(MyEdge * edge) { e = edge; }
        void setPolygon(MyPolyFace * poly) { poly_face = poly; }

        bool isBoundary() const { return poly_face == nullptr; }
        int index() { return index; }
        void set_index(int ind) { index = ind; }
};

class MyEdge
{
    private:
        int index;
        MyVert * v1;
        MyVert * v2;
        MyHalfedge * he;
    public:
        MyEdge() : index(-1), v1(nullptr), v2(nullptr), he(nullptr) {}
        MyEdge(MyVert * vv1, MyVert * vv2) : index(-1), v1(vv1), v2(vv2), he(nullptr) {}
        MyEdge(MyVert * vv1, MyVert * vv2, MyHalfedge * hhe) : index(-1), v1(vv1), v2(vv2), he(hhe) {}
        ~MyEdge() { index = -1; }
    public:
        MyHalfedge * const halfEdge() { return he; }
        const MyHalfedge * const halfEdge() const {return const_cast<MyEdge *>(this)->halfEdge(); }
        
        void setHalfedge(MyHalfedge * hhe) { he = hhe; }
        void setVert(MyVert * vv1, MyVert * vv2) { v1 = vv1; v2 = vv2; }
        void updateVert() { v1 = he->fromVert(); v2 = he->toVert(); }
        int index() const { return index; }
        void set_index(int ind) { index = ind; }

        MyVert * getVert(int edge_v)
        {
            updateVert();
            if (edge_v == 0) return v1;
            else if(edge_v == 1) return v2;
            else return nullptr;
        }
        const MyVert * getVert(int edge_v) const 
        {
            return const_cast<MyEdge*> (this)->getVert(edge_v);
        }
        double length()
        {
            updateVert();
            MyVector t = v1->position() - v2->position();
            return t.norm();
        }
        Point getCenter()
        {
            updateVert();
            return v1->position() * 0.5 + v2->position() * 0.5;
        }
};

class MyPolyFace
{
    private:
        int index;
        MyHalfedge * he_begin;
    public:
        MyPolyFace(): index(-1), he_begin(nullptr){}
        MyPolyFace(MyHalfedge * he) : index(-1), he_begin(he){}
        ~MyPolyFace() { index = -1; }
    public:
        MyHalfedge * const halfEdge() { return he_begin; }
        const MyHalfedge * const halfEdge() const { return const_cast<MyPolyFace*>(this)->halfEdge(); }
        void setHalfedge(MyHalfedge * he) { he_begin = he; }
        int index() const { return index; }
        void set_index(int ind) { index = ind; }
        Point getFaceCenter();
};

class PolyMesh
{
    private:
        std::vector<MyHalfedge *> half_edges; //半边集合
        std::vector<MyVert *> vertices; //点集合
        std::vector<MyEdge *> edges; //全边集合
        std::vector<MyPolyFace *> polygons; //多边形集合
    public:
        PolyMesh() {};
        ~PolyMesh() {};
    public:
        std::vector<MyVert *> polygonVert(MyPolyFace *face) const; //返回多边形的起始遍历点
        std::vector<MyPolyFace *> vertAdjacentPolygon(MyVert * vert) const; //返回某个点对应的相邻多边形
        std::vector<MyHalfedge *> vertAdjacentHalfEdge(MyVert * vert) const; //...相邻半边
        std::vector<MyVert *> vertAdjacentVert(MyVert * vert) const; //..相邻点
        std::vector<MyPolyFace *> edgeAdjacentPolygon(MyEdge * edge) const; //某个边对应的相邻三角形
        MyPolyFace * edgeAdjacentAnotherPolygon(MyEdge * edge, MyPolyFace * face) const; //半边的另一个相邻多边形
        bool isBoundary(MyVert * vert) const; //判断点是否在边界
        bool isBoundary(const MyEdge * edge) const; //判断边是否在边界
        bool isBoundary(const MyHalfedge * halfedge) const; //判断半边是否在边界
        MyVert * addVert(double x, double y, double z); //加点
        MyPolyFace * addPolyFace(std::vector<size_t> & v_loop_id); //以id的形式加边
        MyPolyFace * addPolyFace(std::vector<MyVert *> & v_loop); //以点的形式加边
        MyEdge * edgeBetween(MyVert * v0, MyVert * v1); //求两个节点之间的边
        MyEdge * newEdge(MyVert * v1, MyVert * v2); //将v1, v2连接
        MyEdge * addEdge(MyVert * v_begin, MyVert * v_end); //在v1, v2加边，上滤更新信息
        MyHalfedge * newHalfEdge(); //新的边界半边
        MyPolyFace * newPolyFace(); //新的边界多边形
        size_t numVertices() const { return vertices.size(); } //查询集合大小
        size_t numPolygons() const { return polygons.size(); } //查询多边形的集合大小
        MyVert * vert(const size_t id) { return (id < numVertices() ? vertices[id] : nullptr); } //查询第id个节点
        MyPolyFace * polyface(const size_t id) { return (id < numPolygons() ? polygons[id] : nullptr); } //查询第id个三角面
};