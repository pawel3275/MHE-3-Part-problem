#include <string>
#include <iostream>

#include "header/ThreePartProb.h"
#include "header/Tabu.h"
#include "header/Anealling.h"
#include "header/HillClimb.h"
#include "header/Genetic.h"

using namespace std;
void print_help()
{
	cout << "Invalid command line - required: [max_iterations] [input_file_path]" << endl;
	cout << "Usage:" << endl;
	cout << "outfile.out [max_iterations] [input_file_path]" << endl;
	cout << "Example:" << endl;
	cout << "outfile.out 10000 testing_data.txt" << endl;
}

vector <string> parse_argc_argv(int argc, char** argv)
{
	if (argc != 3)
	{
		print_help();
		exit(0);
	}

	vector<string> arguments(argv + 1, argv + argc);
	return arguments;
}

int main(int argc, char** argv)
{
	// Take care of args
	vector <string> arguments = parse_argc_argv(argc, argv);
	unsigned int max_iteration = stoi(arguments[0]);
	string filepath = arguments[1];

	// Load numbers from input file
	vector <int> numbers = ThreePartProb::load_numbers_from_file(filepath);

	// Define the algorithms
	Anealling annealing_alg = Anealling();
	HillClimb hill_alg = HillClimb();
	Genetic genetic_alg = Genetic();
	Tabu tabu_alg = Tabu();

	// Check if the provided input was valid
	if (ThreePartProb::is_valid(numbers))
	{
		// Iterate 5 times (to gather more statistics).
		// Comment out later
		//for (int i = 0; i < 5; i++)
		//{
		//	cout << "############### ITERATION: " << i << " ###############" << endl;
		//	vector<vector<int>> triplets = ThreePartProb::get_triplets(numbers);
		//	annealing_alg.simulated_annealing(max_iteration, triplets);
		//	hill_alg.hill_climb_deterministic(max_iteration, triplets);
		//	hill_alg.hill_climb_stochastic(max_iteration, triplets);
		//	tabu_alg.tabu_search(max_iteration, triplets);
		//	cout << "############################################" << endl << endl;
		//}
		vector<vector<int>> triplets = ThreePartProb::get_triplets(numbers);
		genetic_alg.run_genetic_algorithm(max_iteration, 
			100, 
			Genetic::mutation::one_point, 
			1.1, 
			Genetic::crossover::single,
			1.1,
			Genetic::selection::tournament,
			triplets);
	}
	else
	{
		cout << "Invalid input numbers format. They cannot be divided into 3 subsets." << endl;
	}

	return 0;
}
