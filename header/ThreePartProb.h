#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

class ThreePartProb
{
public:
	ThreePartProb() {};
	void print_best_triplet(
		vector<vector<int>> best_triplet,
		string algorithm,
		chrono::duration<double> time,
		double error
	);

	void gather_statistics_to_file(
		string algorithm,
		chrono::duration<double> time,
		double error,
		vector <double> error_values,
		int max_iterations,
		int problem_size
	);

	void init_cpu_usage_stats();
	double getcput_usage_Value();

	vector<vector<vector<int>>> generate_neighbours(vector<vector<int>> triplets);
	static vector<vector<int>> get_triplets(vector<int> input);
	static vector<int> load_numbers_from_file(string path);

	int get_rand_int(int start, int end);

	static bool is_valid(vector<int> data);

	double get_rand_double(double start, double end);
	double get_error(vector<vector<int>> triplets);
};