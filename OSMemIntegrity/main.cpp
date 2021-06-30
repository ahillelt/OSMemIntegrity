#include<iostream>

#include"Bitsched.h"


using namespace std;


int main()
{
	PriorityScheduler schedule1;
	

	
	MemAlloc* new_task = schedule1.create_entry("101011", 1);
	schedule1.add_to_table(new_task);


	schedule1.add_to_table(schedule1.create_entry("111111", 1));
	
	schedule1.add_to_table(schedule1.create_entry("100001", 1));

	schedule1.add_to_table(schedule1.create_entry("100101", 1));

	
	//schedule1.display_priority_list();
	schedule1.display_non_priority_list();
	//schedule1.test_run();

	return 0;
}


