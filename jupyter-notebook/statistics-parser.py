#!/usr/bin/env python
# coding: utf-8

# In[74]:


import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from ast import literal_eval

plt.rcParams['figure.figsize'] = [15, 15]


# In[75]:


# Load data
all_stats_header = ["algorithm_name", "max_iterations", "duration", "problem_size", "error", "all_errors", "virtual_mem", "physical_mem"]
all_stats = pd.read_csv("../statistics_4_prob 0.1 to 1.0.csv", names=all_stats_header, index_col=False)

# Convert all errors when script was running to list
# all_stats[all_stats_header[5]] = all_stats[all_stats_header[5]].str.split(' ')

all_stats


# In[76]:


# exclude the genetic runs from all data
genetic_alg_df = all_stats[all_stats["algorithm_name"].str.contains("GENETIC")]

# retreive genetic scenarios from column name
genetic_alg_params_names = {0: "algorithm_name", 1: "population", 2: "mutation_type", 3: "mutation_prob", 4: "crossover_type", 5: "crossover_prob", 6: "selection_type"}
genetic_alg_params = genetic_alg_df["algorithm_name"].str.split(" ", expand=True)
genetic_alg_params.rename(columns=genetic_alg_params_names,inplace=True)


# Remove the already splitted first column
del genetic_alg_df['algorithm_name']

# Concat two dataframes
genetic_alg_df = pd.concat([genetic_alg_params, genetic_alg_df], axis=1)

# Perform cleanup for column rows - translate ints to algorithm names
genetic_alg_df["mutation_type"] = genetic_alg_df["mutation_type"].replace(['0'],'single point mutation')
genetic_alg_df["mutation_type"] = genetic_alg_df["mutation_type"].replace(['1'],'two point mutation')

genetic_alg_df["crossover_type"] = genetic_alg_df["crossover_type"].replace(['0'],'single cross over')
genetic_alg_df["crossover_type"] = genetic_alg_df["crossover_type"].replace(['1'],'multiple crossover')

genetic_alg_df["selection_type"] = genetic_alg_df["selection_type"].replace(['0'],'tournament selection')
genetic_alg_df["selection_type"] = genetic_alg_df["selection_type"].replace(['1'],'elite selection')

# Round up the decimals points for mutation probs
genetic_alg_df['crossover_prob'] = genetic_alg_df['crossover_prob'].astype(float)
genetic_alg_df['crossover_prob'] = genetic_alg_df['crossover_prob'].round(decimals = 1)

genetic_alg_df['mutation_prob'] = genetic_alg_df['mutation_prob'].astype(float)
genetic_alg_df['mutation_prob'] = genetic_alg_df['mutation_prob'].round(decimals = 1)

genetic_alg_df 


# In[77]:


# Modify genetic dataframe to resemble the basic_alg dataframe with other algorithms
#genetic_alg_df["algorithm_name"] = "Genetic population " + genetic_alg_df["population"].astype(str) + " " + genetic_alg_df["mutation_type"] + " " +  genetic_alg_df["crossover_type"] + " " + genetic_alg_df["selection_type"] + " mutation prob " + genetic_alg_df["mutation_prob"].astype(str) + " crossover prob " + genetic_alg_df["crossover_prob"].astype(str)


#del genetic_alg_df["population"]
#del genetic_alg_df["mutation_type"]
#del genetic_alg_df["mutation_prob"]
#del genetic_alg_df["crossover_type"]
#del genetic_alg_df["crossover_prob"]
#del genetic_alg_df["selection_type"]
#genetic_alg_df


# In[78]:


basic_alg_df = all_stats[~all_stats["algorithm_name"].str.contains("GENETIC")]
basic_alg_df

#all_data_df = pd.concat([basic_alg_df, genetic_alg_df], axis=0)
#all_data_df


# In[79]:


# Obtain general values from the dataframe
algorithm_names = basic_alg_df[all_stats_header[0]].unique()
number_of_iterations = basic_alg_df[all_stats_header[1]].unique()
problem_sizes = basic_alg_df[all_stats_header[3]].unique()

print("Following algorithms were used:", algorithm_names)
print("Number of iterations was set to: ", number_of_iterations)
print("Problem sizes were equal to the values of: ", problem_sizes)


# In[80]:


# Gather statistics from smaller dataframes
mean_durations = {}
mean_errors = {}
mean_virt_mem = {}
mean_phys_mem = {}

for size in problem_sizes:
    problem_df = basic_alg_df.loc[basic_alg_df['problem_size'] == size]
    mean_time = {}
    mean_error = {}
    mean_virt_m = {}
    mean_phys_m = {}
    for algorithm in algorithm_names:
        statistics_entry = {}
        substracted_df = problem_df.loc[problem_df['algorithm_name'] == algorithm]

        mean_time[algorithm] = substracted_df["duration"].mean()
        mean_error[algorithm] = substracted_df["error"].mean()
        
        mean_virt_m[algorithm] = substracted_df["virtual_mem"].mean()
        mean_phys_m[algorithm] = substracted_df["physical_mem"].mean()
    
    mean_durations[str(size)] = mean_time
    mean_errors[str(size)] = mean_error
    mean_virt_mem[str(size)] = mean_virt_m
    mean_phys_mem[str(size)] = mean_phys_m
    
mean_durations = pd.DataFrame(mean_durations)
mean_errors = pd.DataFrame(mean_errors)
mean_virt_mem = pd.DataFrame(mean_virt_mem)
mean_phys_mem = pd.DataFrame(mean_phys_mem)


# In[81]:


mean_durations = mean_durations.T
plt.figure();
mean_durations.plot(figsize=(10,10))
mean_durations


# In[82]:


mean_errors = mean_errors.T
plt.figure();
mean_errors.plot(figsize=(10,10))
mean_errors


# In[83]:


mean_virt_mem = mean_virt_mem.T
plt.figure();
mean_virt_mem.plot(figsize=(10,10))
mean_virt_mem


# In[84]:


mean_phys_mem
mean_phys_mem = mean_phys_mem.T
plt.figure();
mean_phys_mem.plot(figsize=(10,10))
mean_phys_mem


# In[ ]:





# In[85]:


# Create error rates dataframe and convert the error rates values from string to list
error_rates_df = basic_alg_df[["algorithm_name", "problem_size", "all_errors"]].copy()
longest_error_rates = {}

for size in problem_sizes:
    df_1 = error_rates_df.loc[error_rates_df['problem_size'] == size]
    rates = {}
    for algorithm in algorithm_names:
        df_2 = df_1.loc[df_1['algorithm_name'] == algorithm]
        lengths = df_2["all_errors"].str.len()
        argmax = np.where(lengths == lengths.max())[0]
        rates[algorithm] = df_2["all_errors"].iloc[argmax[0]]
    longest_error_rates[size] = rates
        
longest_error_rates_df = pd.DataFrame(longest_error_rates)
longest_error_rates_df = longest_error_rates_df.T

# Convert cell values to lists
dict_1 = longest_error_rates_df.to_dict()
longest = 0
for algorithm in algorithm_names:
    for size in problem_sizes:
        a_string = dict_1[algorithm][size]
        a_list = a_string.split()
        map_object = map(int, a_list)
        list_of_integers = list(map_object)
        if len(list_of_integers) > longest:
            longest = len(list_of_integers)
        dict_1[algorithm][size] = list_of_integers

# Lists in dataframes do not match each other in length
# Hence we need to expand them by repateting the same last value n times
# Where n is the missing length to the longest one
for algorithm in algorithm_names:
    for size in problem_sizes:
        missing_len = longest - len(dict_1[algorithm][size])
        for x in range(missing_len):
            last_int = dict_1[algorithm][size][-1]
            dict_1[algorithm][size].append(last_int)
        
        
longest_error_rates_df = pd.DataFrame(dict_1)
longest_error_rates_df


# In[86]:


population_error_rates = []

longest_error_rates_dict = longest_error_rates_df.T.to_dict()
for key, value in longest_error_rates_dict.items():
    df = pd.DataFrame(longest_error_rates_dict[key])
    plot_title = f"Population {key}"
    df.plot(subplots=True, figsize=(15,15), ylim=-10, use_index=True, title=plot_title) # one line in 4 plots
    plt.show()


# In[87]:


genetic_alg_df
populations = genetic_alg_df["population"].unique()
mutations = genetic_alg_df["mutation_type"].unique()
mutations_prob = genetic_alg_df["mutation_prob"].unique()
crossovers_prob = genetic_alg_df["crossover_prob"].unique()
crossovers = genetic_alg_df["crossover_type"].unique()
selection_types = genetic_alg_df["selection_type"].unique()
genetic_problem_sizes = genetic_alg_df["problem_size"].unique()

print(f"Genetic algorithm was ran with populations of: {populations}")
print(f"Possible mutations were {mutations} with probabilities of {mutations_prob}")
print(f"Possible crossovers were {crossovers} with probabilities of {crossovers_prob}")
print(f"Possible selections were {selection_types}")


# In[88]:


df_per_population_list = []
genetic_alg_df
for pop in populations:
    df_per_population_list.append(genetic_alg_df.loc[genetic_alg_df['population'] == pop])
df_per_population_list[1]


# In[ ]:





# In[89]:


# Calculate average error
avg_dg = genetic_alg_df
average_errors = {}
avg_dg["algorithm_name"] = "Genetic population " + genetic_alg_df["population"].astype(str) + " " + genetic_alg_df["mutation_type"] + " " +  genetic_alg_df["crossover_type"] + " " + genetic_alg_df["selection_type"] + " mutation prob " + genetic_alg_df["mutation_prob"].astype(str) + " crossover prob " + genetic_alg_df["crossover_prob"].astype(str)


del avg_dg["population"]
del avg_dg["mutation_type"]
del avg_dg["mutation_prob"]
del avg_dg["crossover_type"]
del avg_dg["crossover_prob"]
del avg_dg["selection_type"]

genetic_algorithms = avg_dg["algorithm_name"].unique()


# In[90]:


genetic_mean_durations = {}
genetic_mean_errors = {}
genetic_mean_virt_mem = {}
genetic_mean_phys_mem = {}

for size in genetic_problem_sizes:
    problem_df = avg_dg.loc[avg_dg['problem_size'] == size]
    mean_time = {}
    mean_error = {}
    mean_virt_m = {}
    mean_phys_m = {}
    for algorithm in genetic_algorithms:
        statistics_entry = {}
        substracted_df = problem_df.loc[problem_df['algorithm_name'] == algorithm]

        mean_time[algorithm] = substracted_df["duration"].mean()
        mean_error[algorithm] = substracted_df["error"].mean()
        
        mean_virt_m[algorithm] = substracted_df["virtual_mem"].mean()
        mean_phys_m[algorithm] = substracted_df["physical_mem"].mean()
    
    genetic_mean_durations[str(size)] = mean_time
    genetic_mean_errors[str(size)] = mean_error
    genetic_mean_virt_mem[str(size)] = mean_virt_m
    genetic_mean_phys_mem[str(size)] = mean_phys_m
    
genetic_mean_durations = pd.DataFrame(genetic_mean_durations)
genetic_mean_errors = pd.DataFrame(genetic_mean_errors)
genetic_mean_virt_mem = pd.DataFrame(genetic_mean_virt_mem)
genetic_mean_phys_mem = pd.DataFrame(genetic_mean_phys_mem)


# In[91]:


genetic_mean_durations = genetic_mean_durations.T
plt.figure();
genetic_mean_durations.plot(figsize=(50,50), subplots=True)
genetic_mean_durations


# In[92]:


genetic_mean_errors = genetic_mean_errors.T
plt.figure();
genetic_mean_errors.plot(figsize=(50,50), subplots=True)
genetic_mean_errors


# In[93]:


genetic_mean_phys_mem = genetic_mean_phys_mem.T
plt.figure();
genetic_mean_phys_mem.plot(figsize=(50,50), subplots=True)
genetic_mean_phys_mem


# In[94]:


genetic_mean_virt_mem = genetic_mean_virt_mem.T
plt.figure();
genetic_mean_virt_mem.plot(figsize=(50,50), subplots=True)
genetic_mean_virt_mem


# In[95]:


# Create error rates dataframe and convert the error rates values from string to list
genetic_error_rates_df = avg_dg[["algorithm_name", "problem_size", "all_errors"]].copy()
longest_error_rates = {}

for size in genetic_problem_sizes:
    df_1 = genetic_error_rates_df.loc[genetic_error_rates_df['problem_size'] == size]
    rates = {}
    for algorithm in genetic_algorithms:
        df_2 = df_1.loc[df_1['algorithm_name'] == algorithm]
        lengths = df_2["all_errors"].str.len()
        argmax = np.where(lengths == lengths.max())[0]
        rates[algorithm] = df_2["all_errors"].iloc[argmax[0]]
    longest_error_rates[size] = rates
        
genetic_longest_error_rates_df = pd.DataFrame(longest_error_rates)
genetic_longest_error_rates_df = genetic_longest_error_rates_df.T

# Convert cell values to lists
dict_1 = genetic_longest_error_rates_df.to_dict()
longest = 0
for algorithm in genetic_algorithms:
    for size in genetic_problem_sizes:
        a_string = dict_1[algorithm][size]
        a_list = a_string.split()
        map_object = map(int, a_list)
        list_of_integers = list(map_object)
        if len(list_of_integers) > longest:
            longest = len(list_of_integers)
        dict_1[algorithm][size] = list_of_integers

# Lists in dataframes do not match each other in length
# Hence we need to expand them by repateting the same last value n times
# Where n is the missing length to the longest one
for algorithm in genetic_algorithms:
    for size in genetic_problem_sizes:
        missing_len = longest - len(dict_1[algorithm][size])
        for x in range(missing_len):
            last_int = dict_1[algorithm][size][-1]
            dict_1[algorithm][size].append(last_int)
        
        
genetic_longest_error_rates_df = pd.DataFrame(dict_1)
genetic_longest_error_rates_df


# In[96]:


population_error_rates = []

longest_error_rates_dict = genetic_longest_error_rates_df.T.to_dict()
for key, value in longest_error_rates_dict.items():
    df = pd.DataFrame(longest_error_rates_dict[key])
    plot_title = f"Population {key}"
    df.plot(subplots=True, figsize=(50,50), ylim=-10, use_index=True, title=plot_title) # one line in 4 plots
    plt.show()


# In[97]:





# In[ ]:




