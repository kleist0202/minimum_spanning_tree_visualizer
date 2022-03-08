#include "helperFunctions.hpp"

void print_array2d(const std::vector<std::vector<Node*>>& p) {
    for (int i = 0; i < p.size(); ++i) {
        for (int j = 0; j < p[i].size(); ++j) {
            std::cout << *p[i][j] << " ";
        }
        std::cout << '\n';
    }
}

void print_array(const std::vector<Edge>& v) {
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << '\n';
    } 
}

void print_array(const std::vector<Node*>& v) {
    for (int i = 0; i < v.size(); ++i) {
        std::cout << *v[i] << '\n';
    } 
}

void readPointsFromFile(const std::string& fileName, std::vector<Node*>& container) {
    std::ifstream ist(fileName, std::ios_base::in);
    if (!ist) throw std::runtime_error("can't open input file");

    int xPos, yPos;
    int treesCount = 0;

    while(ist >> xPos >> yPos) {
        treesCount++;
        Node* t = new Node(treesCount, xPos, yPos);
        container.push_back(t);
    };
}

void writePointsToFile(const std::string& fileName, const std::vector<Node*>& container) {
    std::ofstream ost(fileName, std::ios_base::out);

    for (Node* n : container) {
        ost << n->getX() << ' ' << n->getY() << '\n';
    }
    ost.close();
}

void writeOutputToFile(const std::string& fileName, const std::vector<Edge>& correct_edges) {
    std::ofstream ost(fileName, std::ios_base::out);

    for (Edge e : correct_edges) {
        ost << e.getA()->getX() << ' ' << e.getA()->getY()
            << " - "
            << e.getB()->getX() << ' ' << e.getB()->getY()
            << '\n';
    }
    ost.close();
}

