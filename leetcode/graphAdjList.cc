#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

/* 基于邻接表的无向图 */
class GraphAdjList{
public:
    /* 默认构造，生成空图 */
    GraphAdjList() = default;

    /* 根据顶点数构造 */
    GraphAdjList(int n): adj(n){
        if(n < 0) throw std::invalid_argument("顶点数不能为负");
    }

    /* 根据顶点数 + 边列表构造 */
    GraphAdjList(int n, const std::vector<std::vector<int>>& edges): adj(n){
        if(n < 0) throw std::invalid_argument("顶点数不能为负");
        for(const std::vector<int>& edge: edges){
            if(edge.size() != 2){
                throw std::invalid_argument("每条边必须是 {u, v} 形式");
            }
            addEdge(edge[0], edge[1]);
        }
    }

    /* 返回顶点个数 */
    int size() const { return static_cast<int>(adj.size()); }

    /* 添加新顶点，返回其索引 */
    int addVertex(){
        adj.emplace_back();
        return size() - 1;
    }
    void removeVertex(int i){
        isVaild(i);

        /* 1. 将顶点i的邻居列表清空 */
        // adj[i].clear(); 这一步多余，后面会直接删除该数组

        /* 2. 遍历其他顶点，删除指向i的边 */
        for(auto& neighbors : adj){
            /* erase-remove idion remove将指定元素移动到末尾，其余元素紧凑向前移动，返回一个指向新结束位置的迭代器(无效值) */
            /* 最后直接区间删除后面垃圾数据即可 */
            neighbors.erase(
                std::remove(neighbors.begin(), neighbors.end(), i),
                neighbors.end()
            );
        }

        /* 3. 将顶底i从邻接表中删除 */
        adj.erase(adj.begin() + i);

        /* 4. 因为删除了一行，所以之前在i之后的行他们的行下标都提前了 所以要将所有的邻居列表中原本大于i的值全都减-1*/
        for(auto& neighbors : adj){
            for(int& v : neighbors){
                if (v > i) --v;
            }
        }
    }

    /* 添加边 */
    void addEdge(int u, int v){
        isVaild(u);
        isVaild(v);
        if (u == v) throw std::invalid_argument("不允许自环");
        if (hasEdge(u, v)) return;  // 防重边

        /* 找到对应的节点，添加另一个顶点到邻居列表 */
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    /* 移除边 */
    void removeEdge(int u, int v){
        isVaild(u);
        isVaild(v);

        /* 遍历对应顶点的邻居列表，找到待删除顶点的位置，随后删除*/
        for(int i=0; i<adj[u].size(); ++i){
            if(adj[u][i] == v){
                adj[u].erase(adj[u].begin() + i);
            }
        }
        for(int i=0; i<adj[v].size(); ++i){
            if(adj[v][i] == u){
                adj[v].erase(adj[v].begin() + i);
            }
        }

    }

    /* 判断指点边是否存在 */
    bool hasEdge(int u, int v) const{
        isVaild(u);
        isVaild(v);

        for(int i=0; i<adj[u].size(); ++i){
            if(adj[u][i] == v){
                return true;
            }
        }
        return false;
    }

    /* 获取邻居列表 */
    const std::vector<int>& getNeighbors(int u) const{
        isVaild(u);
        return adj[u];
    }

    /* 获取指定节点的度*/
    int degree(int u) const{
        isVaild(u);
        return static_cast<int>(adj[u].size());
    }

    /* 获取边的数量*/
    int edgeCount() const{
        int total = 0;
        for(auto& neighbor: adj){
            total += neighbor.size();
        }
        return total / 2;
    }

    /* 打印邻接表（调试用）*/
    void print() const {
        std::cout << "邻接表 (共 " << size() << " 个顶点, " 
                  << edgeCount() << " 条边):\n";
        for (int i = 0; i < size(); ++i) {
            std::cout << "  " << i << " -> [";
            for (size_t j = 0; j < adj[i].size(); ++j) {
                std::cout << adj[i][j];
                if (j + 1 < adj[i].size()) std::cout << ", ";
            }
            std::cout << "]\n";
        }
        std::cout << "\n";
    }

private:
    std::vector<std::vector<int>> adj;

    void isVaild(int i) const {
        if(i < 0 || i >= size()){
            throw std::out_of_range("顶点索引超出范围");
        }
    }
};

/* ====== 测试 ====== */
int main() {
    // 构造一张 5 个顶点的图
    //     0 --- 1
    //     |   / |
    //     |  /  |
    //     2 --- 3 --- 4
    GraphAdjList g(5, {
        {0, 1},
        {0, 2},
        {1, 2},
        {1, 3},
        {2, 3},
        {3, 4}
    });
    
    g.print();
    
    std::cout << "顶点 1 的度: " << g.degree(1) << "\n";
    std::cout << "顶点 1 的邻居: ";
    for (int v : g.getNeighbors(1)) std::cout << v << " ";
    std::cout << "\n\n";
    
    std::cout << "边 (0,1) 存在吗? " << std::boolalpha << g.hasEdge(0, 1) << "\n";
    std::cout << "边 (0,4) 存在吗? " << g.hasEdge(0, 4) << "\n\n";
    
    // 删除边 (1, 2)
    g.removeEdge(1, 2);
    std::cout << "删除边 (1, 2) 后:\n";
    g.print();
    
    // 添加一个新顶点
    int newV = g.addVertex();
    g.addEdge(newV, 0);
    std::cout << "添加顶点 " << newV << " 并连接到 0 后:\n";
    g.print();
    
    // 测试异常
    try {
        g.addEdge(0, 0);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    
    try {
        g.hasEdge(0, 100);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    
    return 0;
}