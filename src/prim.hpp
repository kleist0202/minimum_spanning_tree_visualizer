#ifndef PRIM_HPP
#define PRIM_HPP

#include <mutex>
#include <algorithm>

#include "window.hpp"
#include "program.hpp"
#include "classes.hpp"
#include "helperFunctions.hpp"

class Prim : public Program {
private:
    std::vector<Node*> allNodes;
    std::vector<Edge> correct_edges;
    std::vector<Node*> used_nodes;
    std::vector<Node*> nodes_to_spend;
    std::once_flag algorithmEnd;
    FastGui::Window& w;
    bool is_interactive;
    bool canSave;

public:
    Prim(std::vector<Node*>& allNodes, FastGui::Window& w, bool is_interactive);

private:
    void init() override;
    void run() override;
    void keyboardInput() override;
};

#endif // PRIM_HPP
