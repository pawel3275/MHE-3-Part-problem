#pragma once
#include "ThreePartProb.h"

using namespace std;

class HillClimb :
	public ThreePartProb
{
public:
	void hill_climb_stochastic(unsigned int iterations, const vector<vector<int>> triplets);
	void hill_climb_deterministic(unsigned int iterations, const vector<vector<int>> triplets);
};