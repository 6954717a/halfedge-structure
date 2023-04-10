# Halfedge-structure

数据结构作业1-半边表的数据结构实现(2023/4/7)

实现三维点类、三维向量类，半边表的功能定义部分(2023/4/8)

正在完成半边表的功能函数部分

实现半边表的功能函数编写、三维三角面组合的多边形的功能函数编写

实现三维物体mesh存储及第一版功能设计(2023/4/9-4/10 08:59)

存储类：

### Point
  存储信息：点的x, y, z坐标信息

  实现功能：构造生成函数及定义，修改、获得存储信息，重载下标调用以及(+, * )运算符，点与点之间的距离计算，输出呈现

### MyVector
  存储信息：向量的x, y, z分量

  实现功能：构造生成函数及定义，修改、获得存储信息，重载下标调用以及(+, -)运算符，向量点乘，向量叉乘(无交换律)，求模长，向量夹角，方向夹角，主分量，构建基础正交基，构建正交基

### MyVert
  **存储信息：** 点的编号index, 点的坐标(Point), 对应的第一条半边(MyHalfedge)

  **实现功能：** 构造函数调用，修改存储信息，判断是否为孤立点，调整传出半边(即始遍历半边)

### MyHalfedge
 **存储信息：** 半边的编号(index), 半边对应的起点(MyVert), 半边对应的终点(MyVert), 半边对应的面(MyPolyFace), 半边对应的后继(next)、前驱(pre), 半边对应的对偶边(twin)

 ** 实现功能：** 构造生成函数及定义，修改、获得存储信息，求解半边的起点与终点，求解对偶边的next(用于遍历点的所有出边)，求解pre的对偶边(也是用于遍历)，是否为边界半边

### MyEdge
  **存储信息：** 全边的编号(index)，全边对应的点v1,v2(MyVert)，对应的一条半边(MyHalfedge)

  **实现功能：** 构造函数调用，修改、获得存储信息，上滤更新信息(update())，获得相应的点v1,v2，删点(去除复合类型中const和volatile属性)转换type，获得对应边的长度，求对应中点

### MyPolyFace
  **存储信息：** 三角面的编号(index)，遍历三角面的起始半边(MyHalfedge)
 
  **实现功能：** 构造函数调用，修改、获得存储信息，获取三角形的中心(x/3, y/3, z/3)

### PolyMesh
  **存储信息：** 三维物体存储的半边集合(vector<MyHalfedge * >)，对应点的集合(...MyVert...)，对应全边集合(...MyEdge...)，对应三角面集合(...polygons...)

  **实现功能：** 构造函数调用，修改、获得存储信息，返回多边形的起始遍历点，返回某个点对应的相邻多边形、半边、点，返回某个全边对应的相邻多边形、另一个多边形，判断点/边/半边是否在边界，连接两个点(加全边)，加点(加边+上滤更新信息)，添加新的边界半边、多边形，查询点、多边形的集合大小，查询第id个节点，查询第id个三角面。

```cpp
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
```
