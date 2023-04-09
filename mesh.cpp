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