#include "../header/ThreePartProb.h"

// for CPU usage statistics
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

random_device device;
mt19937 random_num(device());

vector<vector<vector<int>>> ThreePartProb::generate_neighbours(vector<vector<int>> triplets)
{
	vector<vector<vector<int>>> neighbours;
	for (unsigned int i = 1; i < triplets.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				vector<vector<int>> current_neighbour = triplets;
				swap(current_neighbour[0][j], current_neighbour[i][k]);
				neighbours.push_back(current_neighbour);
			}
		}
	}
	return neighbours;
}

bool ThreePartProb::is_valid(vector<int> input_numbers)
{
	bool is_valid = (input_numbers.size() % 3 == 0 && input_numbers.size() >= 3) ? true : false;
	return is_valid;
}

vector<vector<int>> ThreePartProb::get_triplets(vector<int> input)
{
	random_device device;
	mt19937 rng(device());
	shuffle(begin(input), end(input), rng);

	int index = 0;
	vector<vector<int>> triplets;
	for (unsigned int i = 0; i < input.size() / 3; i++)
	{
		vector<int> triplet;
		for (unsigned int j = 0; j < 3; j++)
		{
			triplet.push_back(input[index]);
			index++;
		}
		triplets.push_back(triplet);
	}
	return triplets;
}

double ThreePartProb::get_error(vector<vector<int>> triplets)
{
	double sum = 0;
	double triplet_sum = 0;
	vector<double> triplets_sums;
	for (auto triplet : triplets)
	{
		triplet_sum = accumulate(triplet.begin(), triplet.end(), 0.0);
		triplets_sums.push_back(triplet_sum);
	}

	double first_triplet_sum = triplets_sums[0];
	for (double triplet_sum_value : triplets_sums)
	{
		sum += abs(first_triplet_sum - triplet_sum_value);
	}
	return sum;
}

void ThreePartProb::print_best_triplet(
	vector<vector<int>> best_triplet,
	string algorithm,
	chrono::duration<double> time,
	double error
)
{
	cout << algorithm << endl;
	for (unsigned int i = 0; i < best_triplet.size(); i++)
	{
		std::cout << "{";
		std::cout << best_triplet[i][0] << ", " << best_triplet[i][1] << ", " << best_triplet[i][2];
		std::cout << "}" << endl;
	}
	cout << "Acheived time: " << time.count() << " with error: " << error << endl << endl;
}

void ThreePartProb::gather_statistics_to_file(
	string algorithm,
	chrono::duration<double> time,
	double error,
	vector <double> error_values,
	int max_iterations,
	int problem_size
)
{
	// Gather statistics.
	init_cpu_usage_stats();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	//string cpu_usage_percent = to_string(getcput_usage_Value());

	ofstream csv_file;
	string gathered_error_values;
	string delimeter = ";";
	for (int i = 0; i < error_values.size(); i++)
	{
		string error = to_string(int(error_values[i]));
		gathered_error_values += error + " ";
	}
	csv_file.open("statistics.csv", ios::app);
	csv_file << algorithm << delimeter << max_iterations << delimeter << time.count() << delimeter << problem_size * 3 \
		<< delimeter << error << delimeter << gathered_error_values << delimeter << virtualMemUsedByMe \
		<< delimeter << physMemUsedByMe << delimeter << endl;
	//<< cpu_usage_percent << endl;
	csv_file.close();
}

vector<int> ThreePartProb::load_numbers_from_file(string path)
{
	vector<int> numbers;
	struct stat buffer;
	bool file_exists = (stat(path.c_str(), &buffer) == 0);

	if (!file_exists)
	{
		cout << "File does not exist. Exiting..." << endl;
		return numbers;
	}

	int element;
	ifstream in_file(path);
	while (in_file >> element)
	{
		numbers.push_back(element);
	}
	return numbers;
}

int ThreePartProb::get_rand_int(int start, int end)
{
	uniform_int_distribution<int> rand_int = uniform_int_distribution<int>(start, end);
	return rand_int(random_num);
}

double ThreePartProb::get_rand_double(double start, double end)
{
	uniform_real_distribution<> random_distribution(start, end);

	return random_distribution(device);
}

void ThreePartProb::init_cpu_usage_stats()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double ThreePartProb::getcput_usage_Value()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return percent * 100;
}