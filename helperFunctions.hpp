#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <iostream>
#include <vector>
#include "classes.hpp"
#include <fstream>

void print_array2d(const std::vector<std::vector<Node*>>& p);
void print_array(const std::vector<Edge>& v);
void print_array(const std::vector<Node*>& v);

void readPointsFromFile(const std::string& fileName, std::vector<Node*>& container);
void writePointsToFile(const std::string& fileName, const std::vector<Node*>& container);
void writeOutputToFile(const std::string& fileName, const std::vector<Edge>& correct_edges);

#endif //  HELPER_FUNCTIONS_HPP
