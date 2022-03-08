#include "classes.hpp"

// Node

double Node::calcDist(const Node& other) const {
    int xDist = x - other.getX();
    int yDist = y - other.getY();

    return std::sqrt(xDist*xDist + yDist*yDist);
}
        
double Node::calcDist(const Node* other) const {
    int xDist = x - other->getX();
    int yDist = y - other->getY();

    return std::sqrt(xDist*xDist + yDist*yDist);
}

void Node::addNieghbour(Node* neighbour) {
    neighbours.push_back(neighbour);
}

Node* Node::checkCycle(Node* other, Node* old) {
    if (this == other)
        return this;

    for (Node* t : neighbours) {
        if (t == old)
            continue;
        Node* result = t->checkCycle(other, this);
        if (result != nullptr)
            return result;
    }
    return nullptr;
}

bool Node::operator<(const Node& other) {
    return id < other.id;
}

void Node::clearNeighbours() {
    neighbours.clear();
}

std::ostream& operator<<(std::ostream &output, const Node& obj ) {
    output << "id: " << obj.id << " neighbours: ";
    if (obj.neighbours.empty())
        output << "empty";
    for (Node* n : obj.neighbours) {
        output << n->id << ", ";
    }
    return output;
}

// Edge

Edge::Edge(Node* a, Node* b) : a(a), b(b) {
    weight = a->calcDist(*b);
}

std::ostream &operator<<(std::ostream &output, const Edge& obj ) {
    output << obj.a->getId() << "--------" << obj.b->getId() << " weight:" << obj.weight;
    return output;
}
