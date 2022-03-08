#include "prim.hpp"
#include "classes.hpp"

Prim::Prim(std::vector<Node*>& allNodes, FastGui::Window& w, bool is_interactive) : allNodes(allNodes), w(w), is_interactive(is_interactive) {}

void Prim::init() {
    std::cout << "Init prim..." << '\n';
    
    canSave = false;

    if (!allNodes.empty()) {
        canSave = true;
        nodes_to_spend = allNodes;
        
        // choose first node
        used_nodes.push_back(allNodes[0]);
        nodes_to_spend.erase(std::remove(nodes_to_spend.begin(), nodes_to_spend.end(), allNodes[0]), nodes_to_spend.end());

    }
}

void Prim::run() {

    if (is_interactive) {
        if (w.isLeftButtonPressed()) {
            int x_pos = w.getMousePos().x;
            int y_pos = w.getMousePos().y;
            allNodes.push_back(new Node(0, x_pos, y_pos));
        }
    }
    else {

        double shortest_dist = static_cast<double>(INTMAX_MAX);
        Node* closest_node = nullptr;
        Node* beginning_node = nullptr;

        if (!nodes_to_spend.empty()) {
            //find node the closest node to current tree
            for (Node* up : used_nodes) {
                for (Node* nts : nodes_to_spend) {

                    // calc distance between current nodes
                    double dist = up->calcDist(nts);

                    // choose the shortest distance
                    if (dist < shortest_dist) {
                        beginning_node = up;
                        closest_node = nts;
                        shortest_dist = dist;
                    }
                }
            }

            // push shortest node to chosen ones
            used_nodes.push_back(closest_node);

            // construct edge from current node and the closest to it node
            correct_edges.push_back(Edge(beginning_node, closest_node));

            // erase closest node from nodes outside tree
            nodes_to_spend.erase(std::remove(nodes_to_spend.begin(), nodes_to_spend.end(), closest_node), nodes_to_spend.end());

            // the purpose of the following loops is to find the closest nodes outside tree to already chosen ones
            // thanks to it I can draw such intresting animation
            for (Node* nts : nodes_to_spend) {
                double shortest_for_chosen_node = static_cast<double>(INTMAX_MAX);
                for (Node* un : used_nodes) {
                    double dist = nts->calcDist(un);
                    if (dist < shortest_for_chosen_node) {
                        beginning_node = nts;
                        closest_node = un;
                        shortest_for_chosen_node = dist;
                    }

                }
                // draw blue lines
                FastGui::drawLine(
                        w.getRenderer(),
                        {0, 0, 255},
                        beginning_node->getX(),
                        beginning_node->getY(),
                        closest_node->getX(),
                        closest_node->getY());
            }

            // -------------------------------------------------

        }
    }

    // draw all nodes (points)
    for (Node* n : allNodes) {
        FastGui::drawCircle(w.getRenderer(), {255, 0, 0}, n->getX(), n->getY(), 5);
    }

    if (nodes_to_spend.empty() && !is_interactive && canSave) {
        SDL_Log("Algorithm has ended");
        writeOutputToFile("../data/outputs/output_" + std::to_string(w.getStartTime()), correct_edges);
        canSave = false;
    }
   
    // draw final edges
    for (Edge e : correct_edges) {
        FastGui::drawLine(
                w.getRenderer(),
                {255, 0, 0},
                e.getA()->getX(),
                e.getA()->getY(),
                e.getB()->getX(),
                e.getB()->getY());
    }
}

void Prim::keyboardInput() {
    if (w.keyPressed(SDLK_SPACE) && is_interactive) {
        init();
        is_interactive = false;
        canSave = true;
    }
    if (w.keyPressed(SDLK_c)) {
        is_interactive = true;
        used_nodes.clear();
        correct_edges.clear();
        canSave = false;
    }
}
