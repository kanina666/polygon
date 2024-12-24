#include <vector>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <memory>
#include <stack>
#include <chrono>

struct Node {
    double lon, lat;
    std::vector<std::pair<Node*, double>> edges;
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

struct Graph {
    std::unordered_map<std::pair<double, double>, std::unique_ptr<Node>, pair_hash> nodes;

    Node* find_closest_node(double lat, double lon) {
        double min_distance = std::numeric_limits<double>::max();
        Node* node_founded = nullptr;

        for (const auto& node : nodes) {
            double distance = std::sqrt(std::pow(node.second->lat - lat, 2) + std::pow(node.second->lon - lon, 2));
            if (distance < min_distance) {
                node_founded = node.second.get();
                min_distance = distance;
            }
        }

        return node_founded;
    }

    void add_node(double lon, double lat) {
        if (nodes.find({lat, lon}) == nodes.end()) {
            auto node = std::make_unique<Node>();
            node->lon = lon;
            node->lat = lat;
            nodes[{lat, lon}] = std::move(node);
        }
    }

    void add_edge(double lon1, double lat1, double lon2, double lat2, double distance) {
        auto it1 = nodes.find({lat1, lon1});
        auto it2 = nodes.find({lat2, lon2});

        if (it1 != nodes.end() && it2 != nodes.end()) {
            Node* node1 = it1->second.get();
            Node* node2 = it2->second.get();
            node1->edges.emplace_back(node2, distance);
            node2->edges.emplace_back(node1, distance);
        }
    }

    void load_graph_from_file(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string node_part, neighbor_part;

            std::getline(ss, node_part, ':');
            double lon1, lat1;
            sscanf(node_part.c_str(), "%lf,%lf", &lon1, &lat1);
            add_node(lon1, lat1);

            while (std::getline(ss, neighbor_part, ';')) {
                double lon2, lat2, distance;
                sscanf(neighbor_part.c_str(), "%lf,%lf,%lf", &lon2, &lat2, &distance);
                add_node(lon2, lat2);
                add_edge(lon1, lat1, lon2, lat2, distance);
            }
        }
    }

    bool is_connected() {
        if (nodes.empty()) return true;

        Node* start = nodes.begin()->second.get();

        std::unordered_set<Node*> visited;
        std::queue<Node*> q;
        q.push(start);
        visited.insert(start);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            for (auto [neighbor, _] : current->edges) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }

        return visited.size() == nodes.size();
    }

    std::vector<Node *> dijkstra(Node* start, Node* goal) {
        std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> pq;
        std::unordered_map<Node*, double> distances;
        std::unordered_map<Node*, Node*> parent;

        for (auto& node : nodes) {
            distances[node.second.get()] = std::numeric_limits<double>::infinity();
        }

        distances[start] = 0;
        pq.push({0, start});
        parent[start] = nullptr;

        while (!pq.empty()) {
            auto [current_dist, current] = pq.top();
            pq.pop();

            if (current_dist > distances[current]) continue;

            if (current == goal){
                std::vector<Node*> path;
                while (current) {
                    path.push_back(current);
                    current = parent[current];
                }
                std::reverse(path.begin(), path.end());
                return path;
            };

            for (auto [neighbor, edge_dist] : current->edges) {
                double new_dist = current_dist + edge_dist;

                if (new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    pq.push({new_dist, neighbor});
                    parent[neighbor] = current;
                }
            }
        }

        return {};
    }

    std::vector<Node*> bfs(Node* start, Node* goal) {
        std::queue<Node*> q;
        std::unordered_map<Node*, Node*> parent;
        std::unordered_set<Node*> visited;
        std::vector<Node*> path;

        q.push(start);
        visited.insert(start);
        parent[start] = nullptr;

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current == goal) {
                while (current) {
                    path.push_back(current);
                    current = parent[current];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (auto [neighbor, _] : current->edges) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        return path;
    }

    std::vector<Node*> dfs(Node* start, Node* goal) {
        std::stack<Node*> s;
        std::unordered_map<Node*, Node*> parent;
        std::unordered_set<Node*> visited;
        std::vector<Node*> path;

        s.push(start);
        visited.insert(start);
        parent[start] = nullptr;

        while (!s.empty()) {
            Node* current = s.top();
            s.pop();

            if (current == goal) {
                while (current) {
                    path.push_back(current);
                    current = parent[current];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (auto [neighbor, _] : current->edges) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    s.push(neighbor);
                }
            }
        }
        return path;
    }


    double heuristic(Node* a, Node* b) {
        return std::sqrt(std::pow(a->lat - b->lat, 2) + std::pow(a->lon - b->lon, 2));
    }

    std::vector<Node *> a_star(Node* start, Node* goal) {
        std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> open_set;
        std::unordered_map<Node*, double> g_score;
        std::unordered_map<Node*, double> f_score;
        std::unordered_map<Node*, Node*> came_from;

        for (const auto& node : nodes) {
            g_score[node.second.get()] = std::numeric_limits<double>::infinity();
            f_score[node.second.get()] = std::numeric_limits<double>::infinity();
        }

        g_score[start] = 0;
        f_score[start] = heuristic(start, goal);
        open_set.push({f_score[start], start});
        came_from[start] = nullptr;

        while (!open_set.empty()) {
            Node* current = open_set.top().second;
            open_set.pop();

            if (current == goal) {
                std::vector<Node*> path;
                while (current){
                    path.push_back(current);
                    current = came_from[current];
                }
                std::reverse(path.begin(),path.end());
                return path;
            }

            for (auto [neighbor, weight] : current->edges) {
                double tentative_g_score = g_score[current] + weight;

                if (tentative_g_score < g_score[neighbor]) {
                    came_from[neighbor] = current;
                    g_score[neighbor] = tentative_g_score;
                    f_score[neighbor] = g_score[neighbor] + heuristic(neighbor, goal);
                    open_set.push({f_score[neighbor], neighbor});
                }
            }
        }

        return {};
    }
};

int main() {
    Graph graph;
    graph.load_graph_from_file("spb_graph.txt");

    double start_lon = 30.329460;
    double start_lat = 59.848231;
    double goal_lon = 30.308726;
    double goal_lat = 59.956435;

    Node* start_node = graph.find_closest_node(start_lat, start_lon);
    Node* goal_node = graph.find_closest_node(goal_lat, goal_lon);
    std::cout << "Graph connected: " << (graph.is_connected() ? "Yes" : "No") << std::endl;

    if (start_node && goal_node) {
        auto start_time_d = std::chrono::high_resolution_clock::now();
        std::vector<Node*> dijkstra_path = graph.dijkstra(start_node, goal_node);
        auto end_time_d = std::chrono::high_resolution_clock::now();
//        std::cout << "Dijkstra Path: ";
//        for (Node* n : dijkstra_path) {
//            std::cout << "(" << n->lon << "," << n->lat << ") ";
//        }
        std::cout << std::endl;
        std::cout << " Time Dijkstra: " << std::chrono::duration<double>(end_time_d - start_time_d).count() << " seconds" << std::endl;

        auto start_time_b = std::chrono::high_resolution_clock::now();
        std::vector<Node*> bfs_path = graph.bfs(start_node, goal_node);
        auto end_time_b = std::chrono::high_resolution_clock::now();
//        std::cout << "BFS: ";
//        for (Node* n : bfs_path) {
//            std::cout << "(" << n->lon << "," << n->lat << ") ";
//        }
        std::cout << std::endl;
        std::cout << " Time BFS: " << std::chrono::duration<double>(end_time_b - start_time_b).count() << " seconds" << std::endl;

        auto start_time_dfs = std::chrono::high_resolution_clock::now();
        std::vector<Node*> dfs_path = graph.dfs(start_node, goal_node);
        auto end_time_dfs = std::chrono::high_resolution_clock::now();
//        std::cout << "DFS: ";
//        for (Node* n : dfs_path) {
//            std::cout << "(" << n->lon << "," << n->lat << ") ";
//        }
        std::cout << std::endl;
        std::cout << " Time DFS: " << std::chrono::duration<double>(end_time_dfs - start_time_dfs).count() << " seconds" << std::endl;

        auto start_time_a = std::chrono::high_resolution_clock::now();
        std::vector<Node*> a_star_path = graph.a_star(start_node, goal_node);
        auto end_time_a = std::chrono::high_resolution_clock::now();
//        std::cout << "A* Path: ";
//        for (Node* n : a_star_path) {
//            std::cout << "(" << n->lon << "," << n->lat << ") ";
//        }
        std::cout << std::endl;
        std::cout << " Time A*: " << std::chrono::duration<double>(end_time_a - start_time_a).count() << " seconds" << std::endl;

    } else {
        std::cout << "Start or goal node not found in graph." << std::endl;
    }

    return 0;
}
