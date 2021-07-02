#include<iostream>
#include <random>
#include<string>
#include<vector>



#include <algorithm>
#include <sstream>
#include <iterator>

#include"Bitsched.h"

using namespace std;

void random_assignment(PriorityScheduler& schedule, double priority_alloc, int bit_length, long total_entries);

int main()
{
	PriorityScheduler schedule1;
	

	
	/*
	schedule1.add_to_table(schedule1.create_entry("101011", 1));
	schedule1.add_to_table(schedule1.create_entry("111111", 1));
	schedule1.add_to_table(schedule1.create_entry("100001", 1));
	schedule1.add_to_table(schedule1.create_entry("100101", 1));
	

	schedule1.add_to_table(schedule1.create_entry("101011", 0));
	schedule1.add_to_table(schedule1.create_entry("111011", 0));
	schedule1.add_to_table(schedule1.create_entry("101001", 0));
	schedule1.add_to_table(schedule1.create_entry("101101", 0));
	*/

	random_assignment(schedule1, 0.1, 32, 16000);
	
	//schedule1.display_priority_list();
	schedule1.display_non_priority_list();
	//schedule1.test_run();

	return 0;
}


void random_assignment(PriorityScheduler& schedule, double priority_alloc, int bit_length, long total_entries)
{
	std::default_random_engine defaultRandomEngine(std::random_device{}());
	std::uniform_int_distribution<> uniformDistribution(0, 1);
	std::ostringstream* oss;


	vector<char> temp_string;


	for (int i = 0; i < total_entries; i++) {
		oss = new ostringstream;
		
		for (int j = 0; j < bit_length; ++j)
		{
			temp_string.push_back(static_cast<char>(uniformDistribution(defaultRandomEngine)));
		}


		std::copy(temp_string.begin(), temp_string.end() - 1,std::ostream_iterator<int>(*oss, ""));

		// last element (no delimiter(
		*oss << temp_string.back();

		schedule.add_to_table(schedule.create_entry(oss->str(), 1));

		delete oss;
		oss = nullptr;
		temp_string.clear();
	}
}
