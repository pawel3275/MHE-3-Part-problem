#include "../header/Tabu.h"

// https://www.researchgate.net/figure/Pseudo-code-of-Tabu-Search-algorithm_fig1_220233010
void Tabu::tabu_search(unsigned int max_iterations, vector<vector<int>> triplets)
{
	unsigned int tabu_size = triplets.size();

	vector<double> errors;
	// initial solution
	vector<vector<int>> best_triplet = triplets;
	vector<vector<int>> temp_solution = best_triplet;

	set<vector<vector<int>>> taboo_set;
	list<vector<vector<int>>> taboo_list;

	auto compare_function = [&](auto a, auto b)
	{
		return get_error(a) > get_error(b);
	};

	auto is_in_taboo = [&](vector<vector<int>> value)
	{
		return taboo_set.count(value);
	};

	auto removal_condition = [&](auto value)
	{
		return is_in_taboo(value);
	};

	auto start = chrono::system_clock::now();
	for (unsigned int i = 0; i < max_iterations; i++)
	{
		// Neighbours but not in tabu
		vector<vector<vector<int>>> neighbours = generate_neighbours(best_triplet);
		neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(), removal_condition), neighbours.end());

		if (neighbours.size() == 0)
		{
			break;
		}

		// TODO: Remove later
		// double max = *max_element(vector.begin(), vector.end());
		// Hill hill = std::max(hills.begin(), hills.end(), byHeight);
		//auto byHeight = [&](const Hill& a, const Hill& b) {
		//    return a.height < b.height;
		//};
		temp_solution = *max_element(neighbours.begin(), neighbours.end(), compare_function);

		taboo_set.insert(temp_solution);
		taboo_list.push_back(temp_solution);

		if (get_error(temp_solution) < get_error(best_triplet))
		{
			best_triplet = temp_solution;
			errors.push_back(get_error(best_triplet));
		}

		if (taboo_set.size() > tabu_size)
		{
			taboo_set.erase(taboo_list.front());
			taboo_list.pop_front();
		}
	}
	auto finish = chrono::system_clock::now();
	chrono::duration<double> time_taken = finish - start;

	print_best_triplet(best_triplet, "Tabu:", time_taken, get_error(best_triplet));
	gather_statistics_to_file("TABU", time_taken, get_error(best_triplet), errors, max_iterations, triplets.size());
};