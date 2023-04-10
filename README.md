# Halfedge-structure

数据结构作业1-半边表的数据结构实现(2023/4/7)

实现三维点类、三维向量类，半边表的功能定义部分(2023/4/8)

正在完成半边表的功能函数部分

实现半边表的功能函数编写、三维三角面的功能函数编写

实现三维物体mesh存储及第一版功能设计(2023/4/9-4/10 08:59)

存储类：

### Point
存储信息：点的x, y, z坐标信息
实现功能：构造生成函数及定义，修改存储信息，重载下标调用以及(+, * )运算符，点与点之间的距离计算，输出呈现

### MyVector
存储信息：向量的x, y, z分量
实现功能：构造生成函数及定义，修改存储信息，重载下标调用以及(+, -)运算符，向量点乘，向量叉乘(无交换律)，求模长，向量夹角，方向夹角，主分量，构建基础正交基，构建正交基

### MyVert
存储信息：点的编号index, 点的坐标(Point), 对应的第一条半边(MyHalfedge)
实现功能：

### MyHalfedge
存储信息：半边的编号(index), 半边对应的起点(MyVert), 半边对应的终点(MyVert), 半边对应的面(MyPolyFace), 半边对应的后继(next)、前驱(pre), 半边对应的对偶边(twin)
实现功能：

### MyEdge
存储信息：全边的编号(index)，全边对应的点v1,v2(MyVert)，对应的一条半边(MyHalfedge)
实现功能：

### MyPolyFace
存储信息：三角面的编号(index)，遍历三角面的起始半边(MyHalfedge)
实现功能：

### PolyMesh
存储信息：三维物体存储的半边集合(vector<MyHalfedge * >)，对应点的集合(...MyVert...)，对应全边集合(...MyEdge...)，对应三角面集合(...polygons...)
实现功能：
