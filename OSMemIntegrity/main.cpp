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
	MemAlloc* priority;
	MemAlloc* nonpriority;

	int priority_list_size;
	int nonpriority_list_size;
	

public:

	PriorityScheduler(int priority_list_init = 0, int non_priority__list_init = 0, MemAlloc*  priority_ptr = nullptr, MemAlloc* non_priority_ptr = nullptr):priority_list_size(priority_list_init), nonpriority_list_size(non_priority__list_init),priority(priority_ptr), nonpriority(non_priority_ptr){}

	//getters
	int get_priority_size() { return priority_list_size; }
	int get_non_priority_size() { return nonpriority_list_size; }
	
	//setters
	MemAlloc* create_entry(string word,int priority);
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

	if(zero_counter<one_counter)
	{
		parity_ptr = new int(-1);
	} else if (zero_counter==one_counter)
	{
		parity_ptr = new int(0);
	} else
	{
		parity_ptr = new int(1);
	}
	
	MemAlloc* new_entry = new MemAlloc{ word_array,parity_ptr,word_size_ptr,priority_ptr };
	
	return new_entry;
  }


int main()
{
	PriorityScheduler schedule1;
	

	
	MemAlloc* new_task = schedule1.create_entry("101011", 1);

	


	return 0;
}

void display_memalloc(MemAlloc* temp_ptr)
{
	
	//printf("BIT: %d\n", new_task->word[0].bit_val);


	for (int i = 0; i < *(temp_ptr->word_size); i++)
	{

		printf("%d ", temp_ptr->word[i].bit_val);
	}

	cout << endl;

	
}