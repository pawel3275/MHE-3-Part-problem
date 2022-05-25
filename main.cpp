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

vector <int> generate_problem(int size)
{
	random_device rnd_device;
	mt19937 mersenne_engine{ rnd_device() };
	uniform_int_distribution<int> dist{ 0, size * 5 };

	auto gen = [&dist, &mersenne_engine]() {
		return dist(mersenne_engine);
	};

	vector<int> vec(size);
	generate(begin(vec), end(vec), gen);
	return vec;
}

int main(int argc, char** argv)
{
	// Take care of args
	vector <string> arguments = parse_argc_argv(argc, argv);
	unsigned int max_iteration = stoi(arguments[0]);
	string filepath = arguments[1];

	// Load numbers from input file [Legacy]
	//vector <int> numbers = ThreePartProb::load_numbers_from_file(filepath);

	// Define the algorithms
	Anealling annealing_alg = Anealling();
	HillClimb hill_alg = HillClimb();
	Genetic genetic_alg = Genetic();
	Tabu tabu_alg = Tabu();

	// Parameters:
	vector<double> mutation_probabilities{ 1.0, 0.7, 0.4, 0.1 };
	vector<double> selection_probabilities { 1.0, 0.7, 0.4, 0.1 };
	vector<double> populations{ 10, 50, 100, 250};
	vector<int> problem_size{6, 9, 12, 15, 18, 21, 24};
	int total_iterations = 30; 

	for (int h = 0; h < problem_size.size(); h++)
	{
		//Iterate 5 times (to gather more statistics).
		//Comment out later
		for (int i = 0; i < total_iterations; i++)
		{
			vector <int> numbers = generate_problem(problem_size[h]);
			cout << "############### ITERATION: " << i << " ###############" << endl;
			vector<vector<int>> triplets = ThreePartProb::get_triplets(numbers);
			annealing_alg.simulated_annealing(max_iteration, triplets);
			hill_alg.hill_climb_deterministic(max_iteration, triplets);
			hill_alg.hill_climb_stochastic(max_iteration, triplets);
			tabu_alg.tabu_search(max_iteration, triplets);
			cout << "Starting genetic algorithms..." << endl;
			for (unsigned short j = 0; j < mutation_probabilities.size(); j++)
			{
				for (unsigned short k = 0; k < 2; k++) // for mutation methods
				{
					for (unsigned short l = 0; l < 2; l++) // for crossover methods
					{
						for (unsigned short m = 0; m < 2; m++) // for selection methods
						{

							genetic_alg.run_genetic_algorithm(max_iteration,
								populations[j],
								static_cast<Genetic::mutation>(k),
								mutation_probabilities[j], //1.0 for always, 0.0 for never
								static_cast<Genetic::crossover>(l),
								selection_probabilities[j], //1.0 for always, 0.0 for never
								static_cast<Genetic::selection>(m),
								triplets);
							cout << ".";
						}
					}
				}
			}
			cout << endl << "############################################" << endl << endl;
		}
	}

	return 0;
}
