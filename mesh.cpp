/*
 * 2023.4.9-4.10 Liu Yapeng
 * 半边表PolyMesh类功能实现
 * 需要mesh.h头文件
 * class MyVert, class MyHalfedge, class MyEdge, class MyPolyFace, class PolyMesh的部分功能实现
 * 点, 半边, 全边, 三角面, 三维物体网格化 存储
 */


#include "mesh.h"

void MyVert::adjustOutgoingHalfedge()
{
    if(isIsolated()) return ;
    MyHalfedge *begin = halfEdge();
    MyHalfedge *he = begin;
    do
    {
        if(he -> isBoundary()) //边界问题处理
        {
            setHalfedge(he); //到了边界，加半边
            return ;
        }
        he = he->twinn()->next(); //遍历
    } while(he != begin);
}

Point MyPolyFace::getFaceCenter()
{
    assert(he_begin != nullptr);
    MyHalfedge * he = he_begin;
    Point getCenter(0, 0, 0);
    do
    {
        MyVert * fv = he->toVert(); //半边的下一个点
        getCenter = getCenter + fv->position(); //向前迭代
        he = he->next();
    } while (he != he_begin);
    getCenter = Point(getCenter.x() / 3, getCenter.y() / 3, getCenter.z() / 3);
    return getCenter;
}

std::vector<MyVert *> PolyMesh::polygonVert(MyPolyFace * face) const 
{
    std::vector<MyVert *> f_vert;
    MyHalfedge * he_begin = face -> halfEdge();
    MyHalfedge * he = face -> halfEdge();
    do
    {
        f_vert.push_back(he->fromVert());
        he = he->next();
    } while (he != he_begin);
    return f_vert;
}

MyVert * PolyMesh::addVert(double x, double y, double z)
{
    auto w_ptr = new MyVert(x, y, z);
    w_ptr -> Setindex(vertices.size());
    vertices.push_back(w_ptr);
    return w_ptr;
}

MyPolyFace * PolyMesh::addPolyFace(std::vector<size_t> &v_loop_id)
{
    size_t v_size = v_loop_id.size();
    std::vector<MyVert *> v_loop;
    //v_loop.reverse(v_size);
    for(size_t i=0; i<v_size; i++)
        v_loop.push_back(vertices[v_loop_id[i]]);
    return addPolyFace(v_loop);
}

MyPolyFace *PolyMesh::addPolyFace(std::vector<MyVert *> &v_loop)
{
    size_t v_size = v_loop.size();
    MyPolyFace *face = newPolyFace();
    std::vector<bool> is_edge_new(v_size, false);
    std::vector<bool> is_needc(v_size, false);

    MyHalfedge * he1 = nullptr;
    MyHalfedge * he2 = nullptr;
    std::vector<MyHalfedge *> vec_edges;
    for(size_t i=0; i<v_size; i++)
    {
        size_t i1 = (i + 1) % v_size;
        MyEdge * e = edgeBetween(v_loop[i], v_loop[i1]);
        if(e == nullptr)
        {
            e = addEdge(v_loop[i], v_loop[i1]);
            is_edge_new[i] = true;
        }
        if(e -> halfEdge() -> fromVert() != v_loop[i])
        {
            he1 = e->halfEdge() -> twinn();
            he2 = e->halfEdge();
        }
        else
        {
            he1 = e->halfEdge();
            he2 = e->halfEdge() -> twinn();
        }
        if(face->halfEdge() == nullptr)
            face -> setHalfedge(he1); //设置从v_loop[1]出发的半边为face,加入vec_edges
        he1 -> setPolygon(face);
        vec_edges.push_back(he1);
    }

    MyHalfedge *he_ip, *he_in;
    MyHalfedge *he_op, *he_on;
    MyHalfedge *he_bp, *he_bn;

    for(size_t i=0;i<v_size;i++)
    {
        size_t i1 = (i + 1) % v_size;
        MyVert *vtop = v_loop[i1];
        he_ip = vec_edges[i];
        he_in - vec_edges[i1];

        assert(he_ip != nullptr);
        assert(he_in != nullptr); //先判定不是空边

        size_t flag = 0;
        if(is_edge_new[i]) flag |= 1;
        if(is_edge_new[i1]) flag |= 2;

        MyHalfedge * he_op = he_in -> twinn();
        MyHalfedge * he_on = he_ip -> twinn();

        switch (flag)
        {
            case 1:
                he_bp = he_in -> prev();
                he_bp->setNext(he_on);
                he_on->setPrev(he_bp); //建立的是双向边
                vtop->setHalfedge(he_on);
                break;

            case 2:
                he_bn = he_ip->next();
                he_op->setNext(he_bn);
                he_bn->setPrev(he_op);
                vtop->setHalfedge(he_bn);
                break;

            case 3:
                if(vtop->halfEdge() == nullptr)
                {
                    vtop->setHalfedge(he_on);
                    he_op->setNext(he_bn);
                    he_on->setPrev(he_op);
                }
                else
                {
                    he_bn = vtop->halfEdge();
                    he_bp = he_bn->prev();

                    he_bp->setNext(he_on);
                    he_on->setPrev(he_bp);
                    he_op->setNext(he_bn);
                    he_bn->setPrev(he_op);
                }
                break;
            
            case 0:
                is_needc[i1] = true;
        }
        he_ip->setNext(he_in);
        he_in->setPrev(he_ip);
    }
    for(size_t i=0; i<v_size; i++)
        if(is_needc[i])
            v_loop[i]->adjustOutgoingHalfedge();
    return face;
} 

MyEdge * PolyMesh::edgeBetween(MyVert * v0, MyVert * v1)
{
    if(v0 == nullptr || v1 == nullptr) return nullptr;
    if(v0 -> halfEdge() == nullptr) return nullptr;

    MyHalfedge * he_begin = v0->halfEdge();
    MyHalfedge * he = v0->halfEdge();

    do
    {
        if(he->toVert() == v1)
            return he->edge();
        he = he->rotateNext(); //进入下一个节点
    } while (he != he_begin);
    
    return nullptr;   
}

MyEdge * PolyMesh::addEdge(MyVert * v_begin, MyVert * v_end) //加边操作
{
    if(v_begin == nullptr || v_end == nullptr) return nullptr;
    MyEdge * edge = edgeBetween(v_begin, v_end);
    if(edge != nullptr)
        return edge;
    edge = newEdge(v_begin, v_end);
    MyHalfedge * he0 = newHalfEdge();
    MyHalfedge * he1 = newHalfEdge();

    edge->setHalfedge(he0);

    he0->setNext(he1);
    he0->setPrev(he1);
    he0->setTwin(he1);
    he0->setVert(v_begin);
    he0->setEdge(edge);
    he0->setPolygon(nullptr);

    he1->setNext(he0);
    he1->setPrev(he0);
    he1->setTwin(he0);
    he1->setVert(v_end);
    he1->setEdge(edge);
    he1->setPolygon(nullptr);

    return edge;
}

MyEdge * PolyMesh::newEdge(MyVert * v1, MyVert * v2)
{
    MyEdge * w_ptr = new MyEdge(v1, v2);
    w_ptr->set_index(edges.size());
    edges.push_back(w_ptr);
    return w_ptr;
}

MyHalfedge * PolyMesh::newHalfEdge()
{
    MyHalfedge * w_ptr = new MyHalfedge();
    w_ptr->set_index(half_edges.size());
    half_edges.push_back(w_ptr);
    return w_ptr;
}

MyPolyFace * PolyMesh::newPolyFace()
{
    MyPolyFace * w_ptr = new MyPolyFace();
    w_ptr->set_index(polygons.size());
    polygons.push_back(w_ptr);
    return w_ptr;
}

std::vector<MyPolyFace *>PolyMesh::vertAdjacentPolygon(MyVert * vert) const
{
    std::vector<MyPolyFace *> f_list;
    if(vert == nullptr || vert->halfEdge() == nullptr) return f_list;
    auto he_begin = vert->halfEdge();
    auto he = he_begin;
    do
    {
        if(he->polygon() != nullptr)
            f_list.push_back(he->polygon());
            he = he->rotateNext();
    } while (he != he_begin);
    return f_list;
}

std::vector<MyHalfedge *> PolyMesh::vertAdjacentHalfEdge(MyVert * vert) const
{
    std::vector<MyHalfedge *> v_adj;
    if(vert == nullptr || vert->halfEdge() == nullptr) return v_adj;
    auto he_begin = vert->halfEdge();
    auto he = he_begin;
    do
    {
        v_adj.push_back(he);
        he = he->rotateNext();
    } while (he != he_begin);
    return v_adj;
}

std::vector<MyVert *> PolyMesh::vertAdjacentVert(MyVert * vert) const
{
    std::vector<MyVert *> v_adj;
    if(vert == nullptr || vert->halfEdge() == nullptr) return v_adj;
    auto he_begin = vert->halfEdge();
    auto he = he_begin;
    do
    {
        v_adj.push_back(he->toVert());
        he = he->rotateNext();
    } while (he != he_begin);
    return v_adj;
}

std::vector<MyPolyFace *> PolyMesh::edgeAdjacentPolygon(MyEdge * edge) const
{
    std::vector<MyPolyFace *> e_poly;
    e_poly.clear();
    MyPolyFace * poly = edge->halfEdge()->polygon();
    if(poly != nullptr) e_poly.push_back(poly);
    poly = edge->halfEdge()->twinn()->polygon();
    if(poly != nullptr)
        e_poly.push_back(poly);
    return e_poly;
}

MyPolyFace * PolyMesh::edgeAdjacentAnotherPolygon(MyEdge * edge, MyPolyFace * face) const
{
    MyPolyFace * p1;
    MyPolyFace * p2;
    p1 = edge->halfEdge()->polygon();
    p2 = edge->halfEdge()->twinn()->polygon();
    if(p1 == face)
        return p2;
    return p1;
}

bool PolyMesh::isBoundary(MyVert * vert) const
{
    if(vert->halfEdge() == nullptr)
        return true;
    MyHalfedge * he_begin = vert->halfEdge();
    MyHalfedge * he = vert->halfEdge();
    do
    {
        if(he->isBoundary())
            return true;
        he = he->rotateNext();
    } while (he != he_begin);
    return false;
}

bool PolyMesh::isBoundary(const MyEdge * edge) const
{
    return (edge->halfEdge()->isBoundary() || edge->halfEdge()->twinn()->isBoundary());
}

bool PolyMesh::isBoundary(const MyHalfedge * halfedge) const
{
    return halfedge->isBoundary();
}