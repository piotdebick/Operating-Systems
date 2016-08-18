#ifndef CSC332_Lab_2_memory_h
#define CSC332_Lab_2_memory_h

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;


struct Job
{
	string name;
	int size;
	bool status;
	int part_num;
};


struct Partition
{
	int size;
	int num;
	bool status;
	int unused;

	int blocks;      //Memory Blocks within a paritition
	Job* job;
	string name;
};


class Memory
{
public:

	//default constructor
	Memory();

	// Two Argument Constructor
	Memory(int memorySize, int numberPartition);

	//Four Argument Constructor
	Memory(int memorySize, int numberPartition, vector<int> partitionSize, int numberJobs, vector<int> jobSize);


	void setPartitionSize(int* size);


	void setJObSize(vector<int> size);


	void print(Partition*, Job*);


	string assign_name(string name, int num);


	int getNumberJob() { return num_jobs; }
	int getNumberPartition() { return num_part; }
	int getMemorySize() { return mememorySize; }

	void setNumberJob(int temp) { num_jobs = temp; }
	void setPartitionNumber(int temp) { num_part = temp; }
	void setMemorySize(int temp) { mememorySize = temp; }

	//Allocaters
	Partition* allocatePartition();
	Job* allocateJobs();

	void fill_block(Partition& P, Job J);

	//Memory Management Functions
	void FirstFit();
	void BestFit();
	void NextFit();
	void WorstFit();


private:
	int mememorySize;
	int num_part;
	int num_jobs;

	vector<int> PartitionSizes;     //Array Of Partition Sizes
	vector<int> JobSizes;           //Array Of Job Sizes
};

#endif