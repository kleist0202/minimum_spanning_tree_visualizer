#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Node {
private:
    int id;
    int x, y;
    std::vector<Node*> neighbours;
public:
    Node() : id(), x(0), y(0) {};
    Node(int id, int x, int y) : id(id), x(x), y(y) {}
    
    double calcDist(const Node& other) const;
    double calcDist(const Node* other) const;

    int getX() const { return x; }
    int getY() const { return y; }
    int getId() const { return id; }
    std::vector<Node*> getNeighbours() const { return neighbours; }

    void addNieghbour(Node* neighbour); 

    Node* checkCycle(Node* other, Node* old);

    bool operator <(const Node& other);

    friend std::ostream &operator<<(std::ostream &output, const Node& obj);
};

class Edge {
    Node* a;
    Node* b;
    double weight;
public:
    Edge(Node* a, Node* b);

    double getWeight() const { return weight; }
    Node* getA() const { return a; }
    Node* getB() const { return b; }

    friend std::ostream &operator<<(std::ostream &output, const Edge& obj);
};

#endif // CLASSES_HPP
