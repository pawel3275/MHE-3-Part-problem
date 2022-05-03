#pragma once
#include "ThreePartProb.h"

using namespace std;

class Tabu :
	public ThreePartProb
{
public:
	void tabu_search(unsigned int max_iterations, vector<vector<int>> triplets);
};

