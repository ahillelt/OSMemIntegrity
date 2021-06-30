#include<iostream>
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
	MemAlloc** priority;
	MemAlloc** nonpriority;

	int priority_physical_size;
	int priority_logical_size;
	
	int nonpriority_physical_size;
	int non_priority_logical_size;

	const int DYNAMIC_INCREASE_FACTOR = 2;

public:

	PriorityScheduler(int priority_list_init = 0, int non_priority_list_init = 0, MemAlloc**  priority_ptr = nullptr, MemAlloc** non_priority_ptr = nullptr):nonpriority_physical_size(non_priority_list_init), non_priority_logical_size(non_priority_list_init), priority_physical_size(priority_list_init), priority_logical_size(priority_list_init),priority(priority_ptr), nonpriority(non_priority_ptr){}

	//getters
	int get_priority_size() { return priority_logical_size; }
	int get_non_priority_size() { return non_priority_logical_size; }
	
	//setters
	MemAlloc* create_entry(string word,int priority);
	void add_to_table(MemAlloc* temp_alloc);


	//helpers
	void add_alloc(MemAlloc**& table, MemAlloc* entry, int& logical_size, int& physical_size);


	//test functions
	void display_memalloc(MemAlloc* temp_ptr);
	void display_priority_list() { display_list(0); }
	void display_non_priority_list() { display_list(1); }
	void display_list(int8_t list);


	void test_run()
	{

		display_memalloc(nonpriority[0]);
		
	}
};


MemAlloc* PriorityScheduler::create_entry(string word, int priority)
{
	WordBit* word_array = nullptr;
	int* parity_ptr = nullptr;
	unsigned int* word_size_ptr = nullptr;
	int* priority_ptr = new int(priority);

	word_size_ptr = new unsigned int(word.length());
	priority_ptr = new int(priority);

	int zero_counter, one_counter;

	// create binary WordBit array

	word_array = new WordBit[*word_size_ptr];
	
	zero_counter = 0;
	one_counter = 0;
	for(size_t i = 0; i < *word_size_ptr; i++)
	{
		word_array[i].bit_val = word[i];
		if (word_array[i].bit_val ==0)
		{
			zero_counter++;
		} else
		{
			one_counter++;
		}
	}

	
	if (zero_counter%2 == 0)
	{
		parity_ptr = new int(0);
	} else
	{
		parity_ptr = new int(1);
	}
	
	MemAlloc* new_entry = new MemAlloc{ word_array,parity_ptr,word_size_ptr,priority_ptr };
	
	return new_entry;
  }

void PriorityScheduler::add_to_table(MemAlloc* temp_alloc)
{

	if(*(temp_alloc->priority) == 0){
		//priority

		add_alloc(priority, temp_alloc, priority_logical_size, priority_physical_size);
		
	} else
	{
		//not priority
		add_alloc(nonpriority, temp_alloc, non_priority_logical_size, nonpriority_physical_size);
		
	}


	
	
}

void PriorityScheduler::add_alloc(MemAlloc**& table, MemAlloc* entry, int& logical_size, int& physical_size)
{
	//dynamic memory allocation for table
	if(physical_size==0)
	{
		physical_size++;
		table = new MemAlloc*[physical_size];
		table[logical_size] = entry;
		logical_size++;
	} else if (logical_size < physical_size)
	{

		table[logical_size] = entry;
		logical_size++;
		
		
	} else if (logical_size==physical_size)
	{
		int new_physical_size = physical_size * DYNAMIC_INCREASE_FACTOR;
		MemAlloc* temp = new MemAlloc[new_physical_size];

		for(unsigned int i = 0; i < logical_size; i++)
		{
			temp[i] = *table[i];
		}

		delete[] *table;
		*table = temp;

		physical_size = new_physical_size;

		table[logical_size] = entry;
		logical_size++;
		
	}
	

}




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






void PriorityScheduler::display_memalloc(MemAlloc* temp_ptr)
{
	
	//printf("BIT: %d\n", new_task->word[0].bit_val);
	if(temp_ptr!=nullptr){

		printf("Word: ");
	for (unsigned int i = 0; i < *(temp_ptr->word_size); i++)
	{

		printf("%d ", temp_ptr->word[i].bit_val);
	}
	printf("Parity Bit: %d", *(temp_ptr->parity));
		
	}
	cout << endl;

	
}

void PriorityScheduler::display_list(int8_t list)
{

	MemAlloc** temp_list = nullptr;
	int list_size = 0;


	if(list ==0 )
	{
		temp_list = priority;
		list_size = this->priority_logical_size;
		
	} else if (list == 1)
	{
		temp_list = nonpriority;
		list_size = this->non_priority_logical_size;
	}


	
	for (unsigned int i = 0; i < list_size; i++)
	{

		display_memalloc(temp_list[i]);
				
	}

	
	
}
