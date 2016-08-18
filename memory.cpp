#include "memory.h"
#include <iomanip>

using namespace std;


Memory::Memory() //Default Constructor 
{
}


Memory::Memory(int memorySize, int numberPartition)  //Constructor with 2 Arguments
{
	memorySize = mememorySize;
	numberPartition = num_part;

}


Memory::Memory(int memorySize, int numberPartition, vector<int> partitionSize, int numberJobs, vector<int> jobSize) //Constructor with 4 Arguments
{
	mememorySize = memorySize;
	num_part = numberPartition;
	num_jobs = numberJobs;
	PartitionSizes = partitionSize;
	JobSizes = jobSize;

}


string Memory::assign_name(string name, int num) //T# names for partitions
{
	std::stringstream ss;
	ss << name << num + 1;
	return ss.str();
}


void Memory::print(Partition* P, Job* J) //Couts Partition and the job associated with it
{
	unsigned long total_waste = 0;

	for (int i = 0; i < num_part; i++)
	{
		cout << P[i].name << "(" << P[i].size << "K) : ";
		for (int j = 0; j < P[i].blocks; j++)
		{
			if (P[i].job[j].name == "")
				cout << "EMPTY" << endl;
			else
			{
				if (j > 0)
					cout << setw(13);

				cout << P[i].job[j].name << "(" << P[i].job[j].size << "K)" << endl;
			}
		}

		cout << "Status: ";
		if (P[i].status)
			cout << "Free";
		else
			cout << "Busy";
		cout << endl;
		cout << "Wasted: " << P[i].unused << "K" << endl;
		cout << endl;
		total_waste += P[i].unused;
	}
	cout << "Total Memory Size: " << mememorySize << "K" << endl;
	cout << "Total Memory Used: " << mememorySize - total_waste << "K" << endl;
	cout << "Total Memory Waste: " << total_waste << "K" << endl << endl;
}

//allocate partition
Partition* Memory::allocatePartition()
{
	Partition* P = new Partition[num_part];

	for (int i = 0; i < num_part; i++)
	{
		P[i].size = PartitionSizes[i];
		P[i].status = true;
		P[i].unused = P[i].size;
		P[i].name = assign_name("T", i);
		P[i].blocks = 1;
		P[i].job = new Job[1];
	}

	return P;
}

//Places job in correct place.
Job* Memory::allocateJobs()
{
	Job* J = new Job[num_jobs];

	for (int i = 0; i < num_jobs; i++)
	{
		J[i].size = JobSizes[i];
		J[i].status = true;
		J[i].name = assign_name("J", i);
	}

	return J;
}

//FirstFit
void Memory::FirstFit()
{
	Partition* P = allocatePartition();
	Job* J = allocateJobs();

	for (unsigned long i = 0; i < num_jobs; i++)
	{
		//Checks Partition where to place the job
		for (int k = 0; k < num_part; k++)
		{
			if (J[i].size <= P[k].size && P[k].status)
			{
				P[k].job[0] = J[i];
				P[k].unused = P[k].size - J[i].size;
				P[k].status = false;    //show parition is busy
				break;
			}
		}

	}
	print(P, J);
}

//BestFit 
void Memory::BestFit()
{
	Partition* P = allocatePartition();
	Job* J = allocateJobs();

	unsigned long least_memory_waste = 9999999;
	unsigned long memory_waste;
	unsigned long min_index;

	for (int i = 0; i < num_jobs; i++)
	{
		//Comparing BestFit for Partition
		for (int k = 0; k < num_part; k++)
		{
			if (J[i].size <= P[k].size && P[k].status)
			{
				memory_waste = P[k].size - J[i].size;
				if (least_memory_waste > memory_waste)
				{
					least_memory_waste = memory_waste;
					min_index = k;
				}
			}
		}

		least_memory_waste = 9999999;   //Reset Variable
		P[min_index].job[0] = J[i];
		P[min_index].unused = P[min_index].size - J[i].size;
		P[min_index].status = false;    //Status of Partition
	}
	print(P, J);
}

//Next Fit
void Memory::NextFit()
{
	Partition* P = allocatePartition();
	Job* J = allocateJobs();


	queue<unsigned long> free_block;

	//Add Index
	for (int i = 0; i < num_part; i++)
		free_block.push(i);

	for (unsigned long i = 0; i < num_jobs; i++)
	{
		for (unsigned long k = free_block.front(); !free_block.empty(); k = free_block.front())
		{
			if (J[i].size <= P[k].unused)
			{

				if (P[k].job[P[k].blocks - 1].name == "")
					P[k].job[P[k].blocks - 1] = J[i];
				else
					fill_block(P[k], J[i]);

				P[k].unused = P[k].unused - J[i].size;
				P[k].status = false; //State of Partition
				free_block.push(k);
				free_block.pop();
				break;
			}
			else
			{
				free_block.push(k);
				free_block.pop();
			}
		}
	}
	print(P, J);
}

//Worst Fit
void Memory::WorstFit()
{
	Partition* P = allocatePartition();
	Job* J = allocateJobs();

	for (int i = 0; i < num_jobs; i++)
	{
		long most_waste = -1;
		long memory_waste = -1;
		long max_index = -1;

		//Checking partitions for worse fit
		for (int k = 0; k < num_part; k++)
		{

			if ((J[i].size <= P[k].unused))
			{
				memory_waste = P[k].unused - J[i].size;
				if (most_waste < memory_waste)
				{
					most_waste = memory_waste;
					max_index = k;
				}
			}
		}
		if (max_index != -1)
		{
			if (P[max_index].job[P[max_index].blocks - 1].name == "")
				P[max_index].job[P[max_index].blocks - 1] = J[i];
			else
				fill_block(P[max_index], J[i]);

			P[max_index].unused = P[max_index].size - J[i].size;
			P[max_index].status = false; //show partition is busy
		}
	}
	print(P, J);
}

void Memory::fill_block(Partition& P, Job J)
{
	Partition temp;
	temp.job = new Job[P.blocks];
	int k = 0;

	//Copies all the jobs to a temporary variable
	for (int i = 0; i < P.blocks; i++)
		temp.job[i] = P.job[i];

	//Deallocates the job array
	delete[] P.job;

	//Re-initialize job array
	P.job = new Job[++P.blocks];

	for (k = 0; k < P.blocks - 1; k++)
		P.job[k] = temp.job[k];

	P.job[k] = J;
}


