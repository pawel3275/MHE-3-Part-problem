#pragma once
#include "ThreePartProb.h"
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>
#include <map>
#include <array>
#include <sstream>
#include <fstream>
#include <chrono>
#include <omp.h>

using namespace std;

class Genetic :
	public ThreePartProb
{
public:
	// enums
	static enum mutation {
		one_point = 0,
		two_point = 1
	};

	static enum selection {
		tournament = 0,
		elite = 1
	};

	static enum crossover {
		single = 0,
		multiple = 1
	};

	void run_genetic_algorithm(unsigned int max_iterations,
		int population,
		mutation mutation,
		double mutation_prob,
		crossover crossover,
		double crossover_prob,
		selection selection,
		const vector<vector<int>> triplets);

	// mutation
	void mutation_one_point(vector<vector<int>>& triplets, double mutation_prob);
	void mutation_two_point(vector<vector<int>>& triplets, double mutation_prob);

	// selection
	vector<vector<vector<int>>> selection_tournament(vector<vector<vector<int>>> population);
	vector<vector<vector<int>>> selection_elite(vector<vector<vector<int>>> population, int average);

	// crossover
	vector<vector<int>> crossover_single(vector<vector<int>> first_parent, vector<vector<int>> second_parent, double crossover_prob);
	vector<vector<int>> crossover_multiple(vector<vector<int>> first_parent, vector<vector<int>> second_parent, double crossover_prob);

	// utilities
	int getIndex(vector<double> v, int K);
	vector<vector<vector<int>>> generate_population(int population_size, vector<vector<int>> triplets);
	vector <int> unpack_triplet(vector < vector<int>> triplets);
	vector < vector<int>> convert_to_triplet(vector<int> unpacked_vec);
	template<typename T>
	double getAverage(std::vector<T> const& v);

};