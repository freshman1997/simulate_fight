#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

struct Node {
    int x, y;  // 节点坐标
    int g, h;  // g代表起点到当前节点的实际代价，h代表当前节点到目标节点的估计代价
    Node* parent;  // 指向父节点的指针

    Node(int x, int y) {
        this->x = x;
        this->y = y;
        g = 0;
        h = 0;
        parent = nullptr;
    }

    int f() const {  // f = g + h
        return g + h;
    }
};

// 计算节点间的曼哈顿距离
int calculateDistance(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// 检查节点是否在开放列表中
bool isInOpenList(const vector<Node*>& openList, const Node* node) {
    for (const auto& n : openList) {
        if (n->x == node->x && n->y == node->y) {
            return true;
        }
    }
    return false;
}

// 检查节点是否在关闭列表中
bool isInClosedList(const vector<Node*>& closedList, const Node* node) {
    for (const auto& n : closedList) {
        if (n->x == node->x && n->y == node->y) {
            return true;
        }
    }
    return false;
}

// 从起点到目标节点的路径
vector<Node*> getPath(Node* node) {
    vector<Node*> path;
    while (node != nullptr) {
        path.push_back(node);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// A*算法实现
vector<Node*> aStarSearch(Node* start, Node* target) {
    vector<Node*> openList;  // 开放列表，存放待探索的节点
    vector<Node*> closedList;  // 关闭列表，存放已探索的节点

    openList.push_back(start);

    while (!openList.empty()) {
        // 在开放列表中查找f值最小的节点
        Node* currentNode = openList[0];
        int currentIndex = 0;
        for (int i = 1; i < openList.size(); ++i) {
            if (openList[i]->f() < currentNode->f()) {
                currentNode = openList[i];
                currentIndex = i;
            }
        }

        // 从开放列表中移除当前节点，加入关闭列表
        openList.erase(openList.begin() + currentIndex);
        closedList.push_back(currentNode);

        // 找到目标节点，返回路径
        if (currentNode->x == target->x && currentNode->y == target->y) {
            return getPath(currentNode);
        }

        // 遍历当前节点的邻居节点
        vector<Node*> neighbors;
        
        // 在实际应用中，此处根据地图数据获取或计算邻居节点
        // 这里简化为上下左右四个方向
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {-1, 1, 0, 0};
        for (int i = 0; i < 4; ++i) {
            int newX = currentNode->x + dx[i];
            int newY = currentNode->y + dy[i];

            // 跳过越界或在关闭列表中的节点
            if (newX < 0 || newX >= 10 || newY < 0 || newY >= 10 ||
                isInClosedList(closedList, new Node(newX, newY))) {
                continue;
            }

            Node* neighbor = new Node(newX, newY);
            neighbor->parent = currentNode;
            neighbor->g = currentNode->g + 1;
            neighbor->h = calculateDistance(*neighbor, *target);

            // 如果邻居节点已经在开放列表中，检查是否有更优的路径
            if (isInOpenList(openList, neighbor)) {
                for (const auto& n : openList) {
                    if (n->x == neighbor->x && n->y == neighbor->y) {
                        if (neighbor->g < n->g) {
                            n->g = neighbor->g;
                            n->parent = neighbor->parent;
                        }
                        break;
                    }
                }
            } else {
                openList.push_back(neighbor);
            }
        }
    }

    return {};  // 无法找到路径，返回空路径
}

int Test_AStar() {
    Node* start = new Node(0, 0);
    Node* target = new Node(9, 9);

    vector<Node*> path = aStarSearch(start, target);

    if (path.empty()) {
        cout << "No path found!" << endl;
    } else {
        cout << "Path found:" << endl;
        for (const auto& node : path) {
            cout << "(" << node->x << ", " << node->y << ")" << endl;
        }
    }

    return 0;
}
