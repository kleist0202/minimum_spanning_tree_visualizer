#include "kruskal.hpp"

Kruskal::Kruskal(std::vector<Node*>& allNodes, FastGui::Window& w, bool is_interactive) : 
    allNodes(allNodes), w(w), is_interactive(is_interactive) {}

void Kruskal::init() {
    std::cout << "Init kruskal...." << '\n';

    canSave = false;

    if (!allNodes.empty()) {

        canSave = true;
        // find all possible connections between nodes
        combs = getCombinations(allNodes);

        // construct edges
        for (int i = 0; i < combs.size(); ++i) {
            Edge e(combs[i][0], combs[i][1]);
            edges.push_back(e);
        } 

        // sort all edges by weights
        std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) -> bool
        {
            return a.getWeight() < b.getWeight();
        });
    }
}

void Kruskal::run() {

    if (is_interactive) {
        if (w.isLeftButtonPressed()) {
            int x_pos = w.getMousePos().x;
            int y_pos = w.getMousePos().y;
            allNodes.push_back(new Node(0, x_pos, y_pos));
        }
    }
    else {
        if (!edges.empty()) {
            Node* currA = edges[0].getA();
            Node* currB = edges[0].getB();

            // draw blue line : it is visible when cycle occured
            FastGui::drawLine(w.getRenderer(), {0, 0, 255}, currA->getX(), currA->getY(), currB->getX(), currB->getY());

            // check if cycle occured  
            if (!currA->checkCycle(currB, nullptr)) {
                currA->addNieghbour(currB);
                currB->addNieghbour(currA);
                correct_edges.push_back(edges[0]);
            }
            edges.erase(edges.begin());
        }
        
        if (edges.empty() && !is_interactive && canSave) {
            SDL_Log("Algorithm has ended");
            writeOutputToFile("../data/outputs/output_" + std::to_string(time(NULL)), correct_edges);
            canSave = false;
        }
    }

    // draw all nodes (points)
    for (Node* t : allNodes) {
        FastGui::drawCircle(w.getRenderer(), {255, 0, 0}, t->getX(), t->getY(), 5);
    }

    // draw red line : correct connection
    for (Edge e : correct_edges) {
        FastGui::drawLine(w.getRenderer(), {255, 0, 0}, e.getA()->getX(), e.getA()->getY(), e.getB()->getX(), e.getB()->getY());
    }
}

void Kruskal::keyboardInput() {
    if (w.keyPressed(SDLK_SPACE) && is_interactive) {
        init();
        is_interactive = false;
        canSave = true;
    }
    if (w.keyPressed(SDLK_c)) {
        is_interactive = true;
        canSave = false;
        correct_edges.clear();
        combs.clear();
        edges.clear();
        for (Node* n : allNodes)
            n->clearNeighbours();
    }
}
