//Piotr Debicki
//CSC 332 
//CSC 305
#include <iostream>
#include <string>
#include "memory.h"
using namespace std;

int main()
{
	int SizeOfMemory = 0;

	vector<int> inputPartition;
	vector<int> inputJobt;

	while (true)
	{
		while (true)
		{
			string input;
			int temp;
			int partitionNumber = 0;       //Parition Size

			cout << "Enter up to 5 parition sizes (Use spaces between each partition) : " << endl;

			getline(cin, input);		//Parition is placed inside of the input
			istringstream box1(input);    //Size of inputs are stored

			while (box1 >> temp)
			{
				inputPartition.push_back(temp);
				partitionNumber++;
			}

			//Calculate the size of the memory
			for (int i = 0; i < inputPartition.size(); i++)
			{
				SizeOfMemory = SizeOfMemory + inputPartition[i];
			}
			cout << endl;
			cout << "Memory size: " << SizeOfMemory << endl;
			cout << "Amount of Partitions: " << inputPartition.size() << endl;
			cout << "Size Of Partitions: ";

			for (int i = 0; i < inputPartition.size(); i++)
				cout << inputPartition[i] << ' ';
			cout << endl << endl;

			//Check the size of the partition
			if ((partitionNumber - 1) >= 5)
			{
				cout << "Sorry! you have either entered 0 or more than 5 partitions. Please try again. \n";
				cout << endl;
				continue;
			}
			break;
		}


		do
		{
			string input2;
			cout << "Enter the sizes of the jobs with a space between them: \n";

			getline(cin, input2);                   // Store Inputs of Job Size
			istringstream box2(input2);              //Jobs are placed into storage 2.


			int j;
			while (box2 >> j)
				inputJobt.push_back(j);

			cout << "Amount of Jobs: " << inputJobt.size() << endl;
			for (int i = 0; i < inputJobt.size(); i++)
				cout << inputJobt[i] << ' ';
			cout << endl;
			break;
		} while (false);

		break;
	}

	Memory MEM(SizeOfMemory, inputPartition.size(), inputPartition, inputJobt.size(), inputJobt);

	while (true)
	{
		string ans;
		cout << "Type FF (Firstfit), BF (Bestfit), NF (Nextfit), or WF (Worstfit). \n";
		cin >> ans;
		transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
		if (ans == "FF") MEM.FirstFit();
		if (ans == "BF") MEM.BestFit();
		if (ans == "NF") MEM.NextFit();
		if (ans == "WF") MEM.WorstFit();
		

	}

	return 1;
}