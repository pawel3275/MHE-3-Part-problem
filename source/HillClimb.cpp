#include "../header/HillClimb.h"

void HillClimb::hill_climb_stochastic(unsigned int max_iterations, const vector<vector<int>> triplets)
{
	vector <double> errors;
	// initial solution
	vector<vector<int>> best_triplet = triplets;
	auto start = chrono::steady_clock::now();
	// Main loop
	for (unsigned int i = 0; i < max_iterations; i++)
	{
		// N <- neigbours (s)
		vector<vector<vector<int>>> my_neighbours_points = generate_neighbours(best_triplet);

		// end if empty
		if (my_neighbours_points.empty())
		{
			break;
		}
		// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
		// Stochastic -> get random neighbour
		for (unsigned int i = 0; i < my_neighbours_points.size(); i++)
		{
			int random_number = get_rand_int(0, my_neighbours_points.size() - 1);
			// Compare random to the current best
			if (get_error(my_neighbours_points[random_number]) < get_error(best_triplet))
			{
				best_triplet = my_neighbours_points[random_number];
				errors.push_back(get_error(best_triplet));
			}
		}
	}
	auto end = chrono::steady_clock::now();
	chrono::duration<double> time_taken = end - start;

	print_best_triplet(best_triplet, "Hill climb stochastic:", time_taken, get_error(best_triplet));
	gather_statistics_to_file("HILL_CLIMB_STOCHASTIC", time_taken, get_error(best_triplet), errors, max_iterations, triplets.size());
}

void HillClimb::hill_climb_deterministic(unsigned int max_iterations, const vector<vector<int>> triplets)
{
	vector <double> errors;

	// initial solution
	vector<vector<int>> best_triplet = triplets;
	auto start = chrono::steady_clock::now();
	// Main loop
	for (unsigned int i = 0; i < max_iterations; i++)
	{
		// N <- neigbours (s)
		vector<vector<vector<int>>> my_neighbours_points = generate_neighbours(best_triplet);

		// end if empty
		if (my_neighbours_points.empty())
		{
			break;
		}

		// for n in all neighbours
		for (auto neighbour_to_check : my_neighbours_points)
		{
			//check all neigbours, if better -> get new best
			if (get_error(neighbour_to_check) < get_error(best_triplet))
			{
				best_triplet = neighbour_to_check;
				errors.push_back(get_error(best_triplet));
			}
		}
	}
	auto end = chrono::steady_clock::now();
	chrono::duration<double> time_taken = end - start;

	print_best_triplet(best_triplet, "Hill climb deterministic:", time_taken, get_error(best_triplet));
	gather_statistics_to_file("HILL_CLIMB_DETERMINISTIC", time_taken, get_error(best_triplet), errors, max_iterations, triplets.size());
}