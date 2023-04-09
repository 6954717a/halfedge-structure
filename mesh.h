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
        MyVert * v_; //半边对应的七点
        MyEdge * e; //对应的全边
        MyPolyFace * poly_face; //对应的面
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
        std::vector<MyHalfedge *> half_edges;
        std::vector<MyVert *> vertices;
        std::vector<MyEdge *> edges;
        std::vector<MyPolyFace *> polygons;
    public:
        PolyMesh() {};
        ~PolyMesh() {};
    public:
        std::vector<MyVert *> polygonVert(MyPolyFace *face) const;
        std::vector<MyPolyFace *> vertAdjacent(MyVert * vert) const;
        std::vector<MyHalfedge *> vertAdjacentHalfEdge(MyVert * vert) const;
        std::vector<MyVert *> vertAdjacentVert(MyVert * vert) const;
        std::vector<MyPolyFace *> edgeAdjacentPolygon(MyEdge * edge) const;
        MyPolyFace * edgeAdjacentAnotherPolygon(MyEdge * edge, MyPolyFace * face) const;
        bool isBoundary(MyVert * vert) const;
        bool isBoundary(const MyEdge * edge) const;
        bool isBoundary(const MyHalfedge * halfedge) const;
        MyVert * addVert(double x, double y, double z);
        MyPolyFace * addPolyFace(std::vector<size_t> & v_loop_id);
        MyPolyFace * addPolyFace(std::vector<MyVert *> & v_loop);
        MyEdge * edgeBetween(MyVert * v0, MyVert * v1);
        MyEdge * newEdge(MyVert * v1, MyVert * v2);
        MyEdge * addEdge(MyVert * v_begin, MyVert * v_end);
        MyHalfedge * newHalfEdge();
        MyPolyFace * newPolyFace();
        size_t numVertices() const { return vertices.size(); }
        size_t numPolygons() const { return polygons.size(); }
        MyVert * vert(const size_t id) { return (id < numVertices() ? vertices[id] : nullptr); }
        MyPolyFace * polyface(const size_t id) { return (id < numPolygons() ? polygons[id] : nullptr); }
};