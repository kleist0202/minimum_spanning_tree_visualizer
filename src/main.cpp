#include "window.hpp"
#include <iostream>
#include <random>
#include <boost/program_options.hpp>

#include "classes.hpp"
#include "kruskal.hpp"
#include "prim.hpp"
#include "helperFunctions.hpp"

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

int main (int argc, char *argv[])
{
    MinimumSpanningTreeOptions opt(argc,argv);

    std::string algorithm = opt.ca.algorithm;
    int nodes_value  = opt.ca.nodes;
    int delay = opt.ca.delay;
    std::string input_path = opt.ca.input_path;
    bool is_interactive = opt.ca.interactive;

    FastGui::Window w(800, 800);
    w.setScreenColor(0,0,0);
    w.setDelay(delay);

    std::cout << "Input algorithm option value=" << algorithm << '\n';
    std::cout << "Input nodes number option value=" << nodes_value << '\n';
    std::cout << "Delay value=" << delay << '\n';
    // ------------------------------------

    std::vector<Node*> allNodes;                

    if (!is_interactive) {
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
    }
    else {
        std::cout << "Interactive mode enabled" << '\n';
    }

    Kruskal k(allNodes, w, is_interactive);
    Prim p(allNodes, w, is_interactive);

    if (algorithm == "kruskal")
        w.main_loop(k);
    else if(algorithm == "prim")
        w.main_loop(p);
    else {
        std::cout << "There is no such algorithm!" << '\n';
        w.windowShouldClose();
    }

    return 0;
}
