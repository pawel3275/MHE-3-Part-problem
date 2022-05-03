#pragma once
#include "ThreePartProb.h"

using namespace std;

class Anealling :
	public ThreePartProb
{
public:
	vector<vector<int>> get_neighbour(vector<vector<int>> triplets);
	void simulated_annealing(int max_iterations, vector<vector<int>> triplets);
};

