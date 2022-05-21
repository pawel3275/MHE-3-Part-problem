#include "../header/Genetic.h"



vector<vector<vector<int>>> Genetic::generate_population(int population_size, vector<vector<int>> triplets)
{
    random_device device;
    mt19937 rng(device());
    
    vector <int> unpacked_values = unpack_triplet(triplets);

    vector<vector<vector<int>>> population;
    for (int i = 0; i < population_size; i++)
    {
        // pack them again into triplets
        population.push_back(get_triplets(unpacked_values));
    }
    return population;
}

vector <int> Genetic::unpack_triplet(vector < vector<int>> triplets)
{
    vector <int> unpacked_values;
    for (unsigned int i = 0; i < triplets.size(); i++)
        for (unsigned j = 0; j < triplets[i].size(); j++)
            unpacked_values.push_back(triplets[i][j]);
    return unpacked_values;
}

vector < vector<int>> Genetic::convert_to_triplet(vector<int> unpacked_vec)
{
    int index = 0;
    vector<vector<int>> triplets;
    for (unsigned int i = 0; i < unpacked_vec.size() / 3; i++)
    {
        vector<int> triplet;
        for (unsigned int j = 0; j < 3; j++)
        {
            triplet.push_back(unpacked_vec[index]);
            index++;
        }
        triplets.push_back(triplet);
    }
    return triplets;
}

vector<vector<vector<int>>> Genetic::selection_tournament(vector<vector<vector<int>>> population)
{
    // take two random people, check for fitness and push back better one.
    vector<vector<vector<int>>> ret;
    for (unsigned int i = 0; i < population.size(); i++)
    {
        int rand_int = get_rand_int(0, population.size() - 1);
        vector<vector<int>> random_point_a = population[rand_int];

        rand_int = get_rand_int(0, population.size() - 1);
        vector<vector<int>> random_point_b = population[rand_int];

        if (get_error(random_point_a) < get_error(random_point_b))
        {
            ret.push_back(random_point_a);
        }
        else
        {
            ret.push_back(random_point_b);
        }
    }
    return ret;
}

vector<vector<vector<int>>> Genetic::selection_elite(vector<vector<vector<int>>> population, int average)
{
    // Obtain only the best speciment. If the fitness equals less then average
    // push them to vector and return only those whose fitness is below average.
    vector<vector<vector<int>>> ret;
    for (unsigned int i = 0; i < population.size(); i++)
    {
        if (get_error(population[i]) < average)
        {
            ret.push_back(population[i]);
        }
    }
    return ret;
}

vector<vector<int>> Genetic::crossover_single(vector<vector<int>> first_parent, vector<vector<int>> second_parent, double crossover_prob)
{
    double rand_num = get_rand_double(0.0, 1.0);
    if (rand_num > crossover_prob)
    {
        return first_parent;
    }

    vector <int> unpacked_parent_a = unpack_triplet(first_parent);
    vector <int> unpacked_parent_b = unpack_triplet(second_parent);

    // from parent a take random subset
    int rand_int = get_rand_int(0, first_parent.size() - 1);
    vector <int> child_gen_a = first_parent[rand_int];
        
    // construct the rest of gene from parent b
    for (unsigned int j = 0; j < child_gen_a.size(); j++)
    {
        auto it = std::find(unpacked_parent_b.begin(), unpacked_parent_b.end(), child_gen_a[j]);
        // check that there actually is a 3 in our vector
        if (it != unpacked_parent_b.end())
        {
            unpacked_parent_b.erase(it);
        }

    }
    //dorobic mirror dla drugiego genu wzgledem pierwszego
    child_gen_a.insert(child_gen_a.end(), unpacked_parent_b.begin(), unpacked_parent_b.end());
    vector<vector<int>> child = convert_to_triplet(child_gen_a);

    return child;
}

vector<vector<int>> Genetic::crossover_multiple(vector<vector<int>> first_parent, vector<vector<int>> second_parent, double crossover_prob)
{
    double rand_num = get_rand_double(0.0, 1.0);
    if (rand_num > crossover_prob)
    {
        return first_parent;
    }

    vector <int> unpacked_parent_a = unpack_triplet(first_parent);
    vector <int> unpacked_parent_b = unpack_triplet(second_parent);
    vector <int> child;
    // take every second number from each unpacked parent
    for (unsigned int i = 0; i < unpacked_parent_a.size(); i+=2)
    {
        child.push_back(unpacked_parent_a[i]);
    }

    // construct the rest of gene from parent b
    for (unsigned int j = 0; j < child.size(); j++)
    {
        auto it = std::find(unpacked_parent_b.begin(), unpacked_parent_b.end(), child[j]);
        if (it != unpacked_parent_b.end())
        {
            unpacked_parent_b.erase(it);
        }

    }
    child.insert(child.end(), unpacked_parent_b.begin(), unpacked_parent_b.end());
    return convert_to_triplet(child);
}


int Genetic::getIndex(vector<double> v, int K)
{
    auto it = find(v.begin(), v.end(), K);
    int index = -1;
    // If element was found
    if (it != v.end())
    {
        index = it - v.begin();
    }
    return index;
}

void Genetic::mutation_one_point(vector<vector<int>> &triplets, double mutation_prob)
{
    double rand_num = get_rand_double(0.0, 1.0);
    if (rand_num > mutation_prob || triplets.size() < 2)
    {
        return;
    }

    // mutate subsets by swapping values
    for (unsigned int i = 1; i < triplets.size(); i++)
    {
        int swap_index = get_rand_int(0, 2); // because we have 3 values in each subset
        swap(triplets[i - 1][swap_index], triplets[i][swap_index]);
    }
}

void Genetic::mutation_two_point(vector<vector<int>>& triplets, double mutation_prob)
{
    double rand_num = get_rand_double(0.0, 1.0);
    if (rand_num > mutation_prob || triplets.size() < 2)
    {
        return;
    }

    // mutate subsets by swapping values
    for (unsigned int i = 1; i < triplets.size(); i++)
    {
        int swap_index = get_rand_int(0, 2); // because we have 3 values in each subset
        swap(triplets[i - 1][swap_index], triplets[i][swap_index]);

        swap_index = get_rand_int(0, 2);
        swap(triplets[i - 1][swap_index], triplets[i][swap_index]);
    }
}

template<typename T>
double Genetic::getAverage(std::vector<T> const& v) {
    if (v.empty()) 
    {
        return 0;
    }
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

void Genetic::run_genetic_algorithm(unsigned int max_iterations,
    int population,
    mutation mutation,
    double mutation_prob,
    crossover crossover,
    double crossover_prob,
    selection selection,
    const vector<vector<int>> triplets)
{
    // vector containing error values
    vector<double> fitnesses;
    
    // Already shuffled population
    vector<vector<vector<int>>> init_population = generate_population(population, triplets);

    // Iterate over each triplet and see what error do we get and push it back to fitness
    for (auto& specimen : init_population) fitnesses.push_back(get_error(specimen));
    cout << "Lowest error = " << *min_element(fitnesses.begin(), fitnesses.end()) << endl;
    int lowest_error = *min_element(fitnesses.begin(), fitnesses.end());
    int lowest_error_index = getIndex(fitnesses, lowest_error);
    for (unsigned int i = 0; i < max_iterations; i++)
    {
        // no need for loop for parents, function has loop inside
        vector<vector<vector<int>>> parents;
        switch (selection)
        {
        case elite:
            parents = selection_elite(init_population, getAverage(fitnesses));
        default:
            parents = selection_tournament(init_population);
            break;
        }
        
        vector<vector<vector<int>>> offspring;
        for (unsigned int i = 0; i < parents.size(); i += 2) 
        {
            // crossover will cross the triplets according to the first parent
            // second parent is subsetted
            switch (crossover)
            {
            case single:
                offspring.push_back(crossover_single(parents[i], parents[i + 1], crossover_prob));
                offspring.push_back(crossover_single(parents[i + 1], parents[i], crossover_prob));
                break;
            case multiple:
                offspring.push_back(crossover_multiple(parents[i], parents[i + 1], crossover_prob));
                offspring.push_back(crossover_multiple(parents[i + 1], parents[i], crossover_prob));
                break;
            default:
                break;
            }

        }
        
        for (unsigned int i = 0; i < parents.size(); i++)
        {
            switch (mutation)
            {
            case one_point:
                mutation_one_point(offspring[i], mutation_prob);
                break;
            case two_point:
                mutation_two_point(offspring[i], mutation_prob);
                break;
            default:
                break;
            }
        }
        
        fitnesses.clear();
        for (auto& specimen : offspring) fitnesses.push_back(get_error(specimen));
        int error = *min_element(fitnesses.begin(), fitnesses.end());
        if (error < lowest_error)
        {
            init_population = offspring;
            lowest_error = error;
            lowest_error_index = getIndex(fitnesses, lowest_error);
            cout << "Lowest error = " << *min_element(fitnesses.begin(), fitnesses.end()) << endl;
            if (lowest_error == 0)
            {
                cout << "Praise the lord!";
                break;
            }
        }
    }
    vector<vector<int>> best_triplet = init_population[lowest_error_index];
    
    for (unsigned int i = 0; i < init_population.size(); i++)
    {
        vector<vector<int>> random_point_a = init_population[i];
        cout << get_error(random_point_a) << endl;
    }

    //return population;
}