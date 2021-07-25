#pragma once

#include<iostream>

//for random bit-flips func
#include <algorithm>
#include <random>

//for reporting
#include<fstream>
#include<vector>
#include<string>

using namespace std;

struct WordBit;
struct MemAlloc;
void display_memalloc(MemAlloc* temp_ptr);

struct MemAlloc
{
	WordBit* word;
	int* parity;
	unsigned int* word_size;
	int* priority;
};

struct WordBit
{
	unsigned int bit_val : 1;
};

class PriorityScheduler
{
private:
	//Tables are indexed and can follow any array based data structure
	MemAlloc** priority;
	MemAlloc** nonpriority;

	unsigned int priority_physical_size;
	unsigned int priority_logical_size;

	unsigned int nonpriority_physical_size;
	unsigned int non_priority_logical_size;

	const int DYNAMIC_INCREASE_FACTOR = 2;

	unsigned int bit_flips_introduced_non_priority;
	unsigned int bit_flips_introduced_priority;

public:

	PriorityScheduler(int priority_list_init = 0, int non_priority_list_init = 0, MemAlloc** priority_ptr = nullptr, MemAlloc** non_priority_ptr = nullptr) :nonpriority_physical_size(non_priority_list_init), non_priority_logical_size(non_priority_list_init), priority_physical_size(priority_list_init), priority_logical_size(priority_list_init), priority(priority_ptr), nonpriority(non_priority_ptr), bit_flips_introduced_non_priority(0), bit_flips_introduced_priority(0) {}
	~PriorityScheduler();
	//getters
	int get_priority_size() { return priority_logical_size; }
	int get_non_priority_size() { return non_priority_logical_size; }

	//setters
	MemAlloc* create_entry(std::string word, int priority);
	void add_to_table(MemAlloc* temp_alloc);

	//helpers
	void add_alloc(MemAlloc**& table, MemAlloc* entry, unsigned int& logical_size, unsigned int& physical_size);

	//test functions
	void display_memalloc(MemAlloc* temp_ptr);
	void display_priority_list() { display_list(0); }
	void display_non_priority_list() { display_list(1); }
	void display_list(int8_t list);

	//bit error introducer
	void flip_bits(const double& randomizer);

	//checker (stores data in CSV)
	void create_report(std::string filename);
	void write_to_report(std::string filename);

	void test_run()
	{
		display_memalloc(nonpriority[0]);
	}
};

inline PriorityScheduler::~PriorityScheduler()
{
	//release structs
	if (priority != nullptr) {
		delete[] * priority;
	}

	if (nonpriority != nullptr) {
		delete[] * nonpriority;
	}

	priority = nullptr;
	nonpriority = nullptr;
}

MemAlloc* PriorityScheduler::create_entry(std::string word, int priority)
{
	WordBit* word_array = nullptr;
	int* parity_ptr = nullptr;
	unsigned int* word_size_ptr = nullptr;
	int* priority_ptr = new int(priority);

	word_size_ptr = new unsigned int(word.length());
	priority_ptr = new int(priority);

	unsigned int zero_counter, one_counter;

	// create binary WordBit array

	// computationally similar to summing all bits and performing %2

	word_array = new WordBit[*word_size_ptr];

	zero_counter = 0;
	one_counter = 0;
	for (size_t i = 0; i < *word_size_ptr; i++)
	{
		word_array[i].bit_val = word[i];
		if (word_array[i].bit_val == 0)
		{
			zero_counter++;
		}
		else
		{
			one_counter++;
		}
	}

	if (zero_counter % 2 == 0)
	{
		parity_ptr = new int(0);
	}
	else
	{
		parity_ptr = new int(1);
	}

	MemAlloc* new_entry = new MemAlloc{ word_array,parity_ptr,word_size_ptr,priority_ptr };

	return new_entry;
}

void PriorityScheduler::add_to_table(MemAlloc* temp_alloc)
{
	if (*(temp_alloc->priority) == 0) {
		//priority

		add_alloc(priority, temp_alloc, priority_logical_size, priority_physical_size);
	}
	else
	{
		//not priority
		add_alloc(nonpriority, temp_alloc, non_priority_logical_size, nonpriority_physical_size);
	}
}

void PriorityScheduler::add_alloc(MemAlloc**& table, MemAlloc* entry, unsigned int& logical_size, unsigned int& physical_size)
{
	//dynamic memory allocation for table
	if (physical_size == 0)
	{
		physical_size++;
		table = new MemAlloc * [physical_size];
		table[logical_size] = entry;
		logical_size++;
	}
	else if (logical_size < physical_size)
	{
		table[logical_size] = entry;
		logical_size++;
	}
	else if (logical_size == physical_size)
	{
		int new_physical_size = physical_size * DYNAMIC_INCREASE_FACTOR;
		MemAlloc** temp = new MemAlloc * [new_physical_size];

		for (size_t i = 0; i < logical_size; i++)
		{
			temp[i] = table[i];
		}

		delete[] table;
		table = temp;

		physical_size = new_physical_size;

		table[logical_size] = entry;
		logical_size++;
	}
}

void PriorityScheduler::display_memalloc(MemAlloc* temp_ptr)
{
	//printf("BIT: %d\n", new_task->word[0].bit_val);
	if (temp_ptr != nullptr) {
		printf("Word: ");
		for (unsigned int i = 0; i < *(temp_ptr->word_size); i++)
		{
			printf("%d ", temp_ptr->word[i].bit_val);
		}
		printf("Parity Bit: %d", *(temp_ptr->parity));
	}
	printf("\n");
}

void PriorityScheduler::display_list(int8_t list)
{
	MemAlloc** temp_list = nullptr;
	int list_size = 0;

	if (list == 0 && priority != nullptr)
	{
		temp_list = priority;
		list_size = this->priority_logical_size;

		for (unsigned int i = 0; i < list_size; i++)
		{
			display_memalloc(temp_list[i]);
		}
		printf("\n");
	}
	else if (list == 1 && nonpriority != nullptr)
	{
		temp_list = nonpriority;
		list_size = this->non_priority_logical_size;

		for (unsigned int i = 0; i < list_size; i++)
		{
			display_memalloc(temp_list[i]);
		}
		printf("\n");
	}
}

void PriorityScheduler::flip_bits(const double& randomizer)
{
	double value;
	std::default_random_engine defaultRandomEngine(std::random_device{}());
	std::uniform_real_distribution<> uniformPriorityDistribution(0.0, 1.0);

	unsigned int array_position;

	//NON PRIORITY
	for (size_t i = 0; i < non_priority_logical_size; i++)
	{
		value = static_cast<double>(uniformPriorityDistribution(defaultRandomEngine));

		if (value <= randomizer)
		{
			//introduce error

			bit_flips_introduced_non_priority++;

			std::uniform_int_distribution<> array_select(0, *(nonpriority[i]->word_size));

			array_position = static_cast<unsigned int>(array_select(defaultRandomEngine));

			if (nonpriority[i]->word[array_position].bit_val == 0)
			{
				nonpriority[i]->word[array_position].bit_val = 1;
			}
			else
			{
				nonpriority[i]->word[array_position].bit_val = 0;
			}
		}
	}

	//NON PRIORITY
	for (size_t i = 0; i < priority_logical_size; i++)
	{
		value = static_cast<double>(uniformPriorityDistribution(defaultRandomEngine));

		if (value <= randomizer)
		{
			//introduce error

			bit_flips_introduced_priority++;

			std::uniform_int_distribution<> array_select(0, *(priority[i]->word_size));

			array_position = static_cast<int>(array_select(defaultRandomEngine));

			if (priority[i]->word[array_position].bit_val == 0)
			{
				priority[i]->word[array_position].bit_val = 1;
			}
			else
			{
				priority[i]->word[array_position].bit_val = 0;
			}
		}
	}
}

void PriorityScheduler::create_report(std::string filename)
{
	std::ofstream csv_file;
	std::vector<std::string> header_names;

	header_names.push_back("Type");
	header_names.push_back("Entries #");
	header_names.push_back("True Correct");
	header_names.push_back("True False");
	header_names.push_back("Detected False");
	header_names.push_back("Computational Steps");

	csv_file.open(filename);

	while (!csv_file)
	{
		csv_file.clear();
		csv_file.open(filename);
	}

	for (int i = 0; i < header_names.size(); i++)
	{
		csv_file << header_names.at(i);
		csv_file << ",";
	}
	csv_file << "\n";

	csv_file.close();
}

void PriorityScheduler::write_to_report(std::string filename)
{
	std::ofstream csv_file;
	std::vector<std::string> header_names;

	//counter variables

	unsigned int zero_counter;
	unsigned int one_counter;

	std::vector<int> error_loc_non_priority;
	std::vector<int> error_loc_priority;

	unsigned int non_priority_errors = 0;
	unsigned int priority_errors = 0;

	csv_file.open(filename, std::ios_base::app);

	while (!csv_file)
	{
		csv_file.clear();
		csv_file.open(filename);
	}

	//data collection
	zero_counter = 0;
	one_counter = 0;
	//non priority
	for (size_t i = 0; i < non_priority_logical_size; i++)
	{
		zero_counter = 0;
		one_counter = 0;

		for (size_t j = 0; j < *nonpriority[i]->word_size; j++)
		{
			if (nonpriority[i]->word[j].bit_val == 0)
			{
				zero_counter++;
			}
			else
			{
				one_counter++;
			}
		}

		if (zero_counter % 2 != *nonpriority[i]->parity)
		{
			non_priority_errors++;
			error_loc_non_priority.push_back(i);
		}
	}

	//priority
	for (size_t i = 0; i < priority_logical_size; i++)
	{
		zero_counter = 0;
		one_counter = 0;

		for (size_t j = 0; j < *priority[i]->word_size; j++)
		{
			if (priority[i]->word[j].bit_val == 0)
			{
				zero_counter++;
			}
			else
			{
				one_counter++;
			}
		}

		if (zero_counter % 2 != *priority[i]->parity)
		{
			priority_errors++;
			error_loc_priority.push_back(i);
		}
	}

	csv_file << "non_priority" << "," << get_non_priority_size() << "," << (non_priority_logical_size - bit_flips_introduced_non_priority) << "," << bit_flips_introduced_non_priority << "," << non_priority_errors << "," << "\n";

	csv_file << "priority" << "," << get_priority_size() << "," << (priority_logical_size - bit_flips_introduced_priority) << "," << bit_flips_introduced_priority << "," << priority_errors << "," << "\n";;

	csv_file.close();
}