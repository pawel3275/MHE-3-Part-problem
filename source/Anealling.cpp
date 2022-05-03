#include "../header/Anealling.h"

void Anealling::simulated_annealing(int max_iterations, vector<vector<int>> triplets)
{
	vector <double> errors;
	// Let s = s0
	vector<vector<int>> annealing_point = triplets;
	vector<vector<int>> best_triplet = triplets;

	auto start = chrono::steady_clock::now();
	// For k = 0 through kmax(exclusive) :
	for (int i = 0; i < max_iterations; i++)
	{
		// Pick a random neighbour, snew <- neighbour(s)
		vector<vector<int>> random_point = get_neighbour(annealing_point);
		if (get_error(random_point) < get_error(annealing_point))
		{
			annealing_point = random_point;
		}
		else
		{
			double random_distribution_number = get_rand_double(0.0, 1.0);
			// T <- temperature( 1 - (k+1)/kmax )
			if (random_distribution_number < exp(-abs(get_error(random_point) - (get_error(annealing_point)) / (1.0 / (i)))))
			{
				// assaign new best
				annealing_point = random_point;
			}
		}
		//If P(E(s), E(snew), T) >= random(0, 1):
		if (get_error(annealing_point) < get_error(best_triplet))
		{
			//s <- snew
			best_triplet = annealing_point;
			errors.push_back(get_error(best_triplet));
		}
	}
	auto end = chrono::steady_clock::now();
	chrono::duration<double> time_taken = end - start;

	print_best_triplet(best_triplet, "Simulated annealing:", time_taken, get_error(best_triplet));
	gather_statistics_to_file("SIMULATED_ANNEALING", time_taken, get_error(best_triplet), errors, max_iterations, triplets.size());
}

vector<vector<int>> Anealling::get_neighbour(vector<vector<int>> triplets)
{
	vector<vector<int>> neighbour = triplets;
	swap(neighbour[get_rand_int(0, neighbour.size() - 1)][get_rand_int(0, 2)],
		neighbour[get_rand_int(0, neighbour.size() - 1)][get_rand_int(0, 2)]);

	return neighbour;
}