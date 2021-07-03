#include<iostream>

#include<string>
#include<vector>

#include <algorithm>
#include <random>

#include <sstream>
#include <iterator>

//for reporting to XLS
#include<fstream>

#include"Bitsched.h"

using namespace std;

void random_assignment(PriorityScheduler& schedule, double priority_alloc, int bit_length, long total_entries);
int random_priority_distribution(const double& priority_alloc);

void completion_output(const int& i, const int& runs);

int main()
{
	PriorityScheduler* schedule = new PriorityScheduler;
	
	int runs;
	
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

	
	
	cout << "Waiting to access report file" << endl;
	schedule->create_report("lab.csv");
	cout << "File Accessed and new report object created" << endl;
	
	
	cout << "Running Simulation" << endl;
	cout << "Progress: 0..";

	runs = 100000;

	
	for(int i = 0; i < runs; i++){

	random_assignment(*schedule, 0.3, 32, 100);

	schedule->flip_bits(0.01);

	schedule->write_to_report("lab.csv");

	delete schedule;
	schedule = new PriorityScheduler;



	completion_output(i, runs);

		
	}
	cout << endl;

	/*
	// Tester functions
	cout << "Priority Zero" << endl;
	schedule1.display_priority_list();
	cout << "\n\n";

	cout << "Priority One" << endl;
	schedule1.display_non_priority_list();
	cout << "\n\n";

	//schedule1.test_run();
	*/

	delete schedule;

	cout << "Done!" << endl;
	return 0;
}


void random_assignment(PriorityScheduler& schedule, double priority_alloc, int bit_length, long total_entries)
{
	std::default_random_engine defaultRandomEngine(std::random_device{}());
	std::uniform_int_distribution<> uniformDistribution(0, 1);
	std::ostringstream* oss;


	vector<char> temp_string;
	int priority_bit;


	


	
	

	for (int i = 0; i < total_entries; i++) {
		oss = new ostringstream;
		
		for (int j = 0; j < bit_length; ++j)
		{
			temp_string.push_back(static_cast<char>(uniformDistribution(defaultRandomEngine)));
		}


		std::copy(temp_string.begin(), temp_string.end() - 1,std::ostream_iterator<int>(*oss, ""));

		// last element (no delimiter(
		*oss << temp_string.back();

		priority_bit = random_priority_distribution(priority_alloc);
		schedule.add_to_table(schedule.create_entry(oss->str(), priority_bit));

		delete oss;
		oss = nullptr;
		temp_string.clear();
	}
}

int random_priority_distribution(const double& priority_alloc)
{
	std::default_random_engine defaultRandomEngine(std::random_device{}());
	std::uniform_real_distribution<> uniformPriorityDistribution(0.0, 1.0);
	double value;

	//SIMPLE, slightly inefficient ALGORITHM TO INTRODUCE WAITED VALUES. min:0 , mid = priority_alloc, max = 1.

	value = static_cast<double>(uniformPriorityDistribution(defaultRandomEngine));

	if(value<= priority_alloc)
	{
		return 0;
	} else
	{
		return 1;
	}

	
}


void completion_output(const int& i, const int& runs)
{
	double progress;
	progress = static_cast<double>(static_cast<double>(i+1)/ static_cast<double>(runs) );

	
	if(progress==0.10)
	{
		cout << "...10...";
		
	} else if (progress == 0.20)
	{
		cout << "...20...";

	}
	else if (progress == 0.30)
	{
		cout << "...30...";

	}
	else if (progress == 0.40)
	{
		cout << "...40...";

	}
	else if (progress == 0.50)
	{
		cout << "...50...";

	}
	else if (progress == 0.60)
	{
		cout << "...60...";

	}
	else if (progress == 0.70)
	{
		cout << "...70...";

	}
	else if (progress == 0.80)
	{
		cout << "...80...";

	}
	else if (progress == 0.90)
	{
		cout << "...90...";

	}
	else if (progress == 1.00)
	{
		cout << "...100";

	}
	


	
}