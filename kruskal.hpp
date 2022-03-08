#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <algorithm>
#include <mutex>

#include "window.hpp"
#include "program.hpp"
#include "classes.hpp"
#include "helperFunctions.hpp"

class Kruskal : public Program {
private:
    std::vector<std::vector<Node*>> combs;   
    std::vector<Node*> allNodes;
    std::vector<Edge> edges;
    std::vector<Edge> correct_edges;
    std::once_flag algorithmEnd;
    FastGui::Window& w;
    bool is_interactive;
    bool canSave;

public:
    Kruskal(std::vector<Node*>& allNodes, FastGui::Window& w, bool is_interactive);

private:
    void init() override;
    void run() override;
    void keyboardInput() override;

    // returns vector of combinations,
    // each combination is also stored in vector of two elements (r = 2, because edge has two nodes)
    template<typename T>
    std::vector<std::vector<T>> getCombinations(std::vector<T>& vec) {
        std::vector<std::vector<T>> outVal;

        int n = vec.size();
        int r = 2;
        int j = 0;

        std::vector<bool> v(n);
        std::fill(v.end() - r, v.end(), true);

        do {
            std::vector<T> temp(r);
            for (int i = 0; i < n; ++i) {
                if (v[i]) {
                    temp[j] = vec[i];
                    j++;
                }
            }
            j = 0;
            outVal.emplace_back(temp);
        } while (std::next_permutation(v.begin(), v.end()));

        return outVal;
    }

};

#endif // KRUSKAL_HPP
