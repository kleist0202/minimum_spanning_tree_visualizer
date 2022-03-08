#include "window.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <algorithm>
#include <boost/program_options.hpp>
#include <mutex>

#include "classes.hpp"

std::once_flag algorithmEnd;

namespace po = boost::program_options;

struct MinimumSpanningTreeConfig{
    std::string algorithm{""};
	int nodes{0};
    int delay{0};
    std::string input_path{""};
    bool interactive{false};
};

struct MinimumSpanningTreeOptions 
{
	po::options_description	options;
	po::variables_map vm;

	MinimumSpanningTreeConfig ca;

	MinimumSpanningTreeOptions(int argc, char** argv) 
        : options() 
    {
		options.add_options()
		("help,h", "display this help.")
		("algorithm,a", po::value<std::string>(&ca.algorithm)->default_value("kruskal"), "chosen algorithm name")
		("nodes,n", po::value<int>(&ca.nodes)->default_value(50), "number of nodes")
		("delay,d", po::value<int>(&ca.delay)->default_value(100), "animation delay")
		("path,p", po::value<std::string>(&ca.input_path)->default_value(""), "input path")
		("interactive,i", "enable interactive mode")
		;

		po::variables_map tmp_vm;
		po::store(po::parse_command_line(argc, argv, options), tmp_vm);
		po::notify(tmp_vm);
		if (tmp_vm.count("help")) {
			std::cout << options << '\n';
			exit(0);
		}

		if (tmp_vm.count("interactive")) {
            ca.interactive = true;
		}
		vm = tmp_vm;
	}
};

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

void kruskal(
    FastGui::Window& w,
    std::vector<Node*>& allNodes,
    std::vector<Edge>& edges,
    std::vector<Edge>& correct_edges
) {
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
    else {
        std::call_once( algorithmEnd, [ correct_edges ]
        {
                SDL_Log("Algorithm has ended");
                writeOutputToFile("../data/outputs/output_" + std::to_string(time(NULL)), correct_edges);
        });
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

void prim(
    FastGui::Window& w,
    std::vector<Node*>& allNodes,
    std::vector<Edge>& edges,
    std::vector<Edge>& correct_edges,
    std::vector<Node*>& nodes_to_spend,
    std::vector<Node*>& used_nodes 
) {
    double shortest_dist = static_cast<double>(INTMAX_MAX);
    Node* closest_node = nullptr;
    Node* beginning_node = nullptr;

    // draw all nodes (points)
    for (Node* n : allNodes) {
        FastGui::drawCircle(w.getRenderer(), {255, 0, 0}, n->getX(), n->getY(), 5);
    }

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
    else {
        std::call_once( algorithmEnd, [ w, correct_edges ]
        {
                SDL_Log("Algorithm has ended");
                writeOutputToFile("../data/outputs/output_" + std::to_string(w.getStartTime()), correct_edges);
        });
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

int main (int argc, char *argv[])
{
    MinimumSpanningTreeOptions opt(argc,argv);

    std::string algorithm = opt.ca.algorithm;
    int nodes_value  = opt.ca.nodes;
    int delay = opt.ca.delay;
    std::string input_path = opt.ca.input_path;
    bool is_interactive = opt.ca.interactive;
    std::cout << is_interactive << '\n';

    FastGui::Window w(800, 800);
    w.setScreenColor(0,0,0);
    w.setDelay(delay);

    std::cout << "Input algorithm option value=" << algorithm << '\n';
    std::cout << "Input nodes number option value=" << nodes_value << '\n';
    // ------------------------------------

    std::vector<Node*> allNodes;                
    if (!input_path.empty())
        readPointsFromFile(input_path, allNodes);
    else {
        // set random number generator
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(100, 700);

        // create new nodes at random postions
        for (int i = 1; i <= nodes_value; ++i) {
            int x = dist(rng);
            int y = dist(rng);
            Node* t = new Node(i, x,y);
            allNodes.push_back(t);
        }
        writePointsToFile("../data/inputs/data_" + std::to_string(w.getStartTime()), allNodes);
    }

    std::vector<Edge> correct_edges;

    // ----- kruskal initialization -----
    std::vector<std::vector<Node*>> combs;   

    // find all possible connections between nodes
    combs = getCombinations(allNodes);

    std::vector<Edge> edges;

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

    // ----- prim initialization -----
    std::vector<Node*> used_nodes;
    std::vector<Node*> nodes_to_spend = allNodes;

    // choose first node
    used_nodes.push_back(allNodes[0]);
    nodes_to_spend.erase(std::remove(nodes_to_spend.begin(), nodes_to_spend.end(), allNodes[0]), nodes_to_spend.end());

    if (algorithm == "kruskal")
        w.main_loop(kruskal, w, allNodes, edges, correct_edges);
    else if(algorithm == "prim")
        w.main_loop(prim, w, allNodes, edges, correct_edges, nodes_to_spend, used_nodes);
    else {
        std::cout << "There is no such algorithm!" << '\n';
        w.windowShouldClose();
    }

    return 0;
}
