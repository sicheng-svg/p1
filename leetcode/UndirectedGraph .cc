#include <iostream>
#include <vector>
#include <iomanip>

/*基于邻接矩阵的无向图*/
class GraphAdjMat{
public:
    /*vertices就是顶点， 遍历vertices创建顶尖，随后创建边*/
    GraphAdjMat(const std::vector<int>& vertices, const std::vector<std::vector<int>>& edges){
        /*添加顶点*/
        for(int val: vertices){
            addVertex(val);
        }
        /*添加边*/
        /*这里传入的edges表示的是边列表，edge就是一条边的两个顶点*/
        for(const std::vector<int>& edge: edges){
            addEdge(edge[0], edge[1]);
        }
    }

    int size() const { return vertices.size();}

    /*添加顶点*/
    /*新增节点后，原来是一个n*n的矩阵，就要变为n+1*n+1的矩阵*/
    void addVertex(int val){
        int n = size(); /*n个顶点，表示时一个n*n的矩阵*/

        /*添加顶点时，需要添加完整的一行和一列*/
        vertices.push_back(val);

        /*现在矩阵中添加新的一行，随后push列*/
        adjMat.emplace_back(std::vector<int>(n, 0));
        for(std::vector<int>& row: adjMat){
            row.push_back(0);
        }
    }

    /*删除顶点，则需要先删除对应行，随后对每一行删除对应列*/
    void removeVertex(int index){
        if(index < 0 || index >= size()) {
            throw std::out_of_range("顶点不存在");
        }
        vertices.erase(vertices.begin() + index);
        adjMat.erase(adjMat.begin() + index);
        for(std::vector<int>& row: adjMat){
            row.erase(row.begin() + index);
        }
    }

    /*添加边，因为无向图，所以要对称添加*/
    void addEdge(int i, int j){
        if(i<0 || i>=size() || j<0 || j>=size() || i==j){
            throw std::out_of_range("顶点不存在");
        }
        adjMat[i][j] = 1;
        adjMat[j][i] = 1;
    }
    void removeEdge(int i, int j){
        if(i<0 || i>=size() || j<0 || j>=size() /* 删除边来说，因为我们本身就不允许自环出现，况且删除自环是正常的请求|| i==j*/){
            throw std::out_of_range("顶点不存在");
        }
        adjMat[i][j] = 0;
        adjMat[j][i] = 0;
    }

    bool hasEdge(int i, int j) const{
        if(i<0 || i>=size() || j<0 || j>=size() /* 同上 || i==j*/){
            throw std::out_of_range("顶点不存在");
        }
        return adjMat[i][j] == 1;
    }
    std::vector<int> getNeighbors(int i) const{
        if(i<0 || i>=size()) {
            throw std::out_of_range("顶点不存在");
        }
        std::vector<int> ans;
        for(int j=0; j<size(); ++j){
            if(adjMat[i][j] == 1){
                ans.push_back(vertices[j]);
            }
        }
        return ans;
    }
    /*对于无向图来说，不存分入度和出度，所以它的度就是边的个数*/
    int degree(int i) const{
        if(i<0 || i>=size()) {
            throw std::out_of_range("顶点不存在");
        }
        int ans = 0;
        for(int j=0; j<size(); ++j){
            ans += adjMat[i][j];
        }
        return ans;
    }
    // 打印整个矩阵（调试用）
    void print() const {
        std::cout << "    ";
        for (int i = 0; i < size(); ++i) {
            std::cout << std::setw(3) << i;
        }
        std::cout << "\n";
        
        for (int i = 0; i < size(); ++i) {
            std::cout << std::setw(3) << i << " ";
            for (int j = 0; j < size(); ++j) {
                std::cout << std::setw(3) << adjMat[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

private:
    /*顶点列表，元素代表顶点值，索引代表顶点索引*/
    std::vector<int> vertices;
    std::vector<std::vector<int>> adjMat;
};

int main(){

    GraphAdjMat graph({0, 1, 2, 3}, {{0, 1}});  
    graph.print();

    graph.addEdge(2, 3);
    graph.addEdge(1, 3);
    graph.print();

    graph.addVertex(4);
    graph.print();

    graph.addEdge(4, 3);
    graph.print();

    return 0;
}