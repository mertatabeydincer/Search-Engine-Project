#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>


#include "bst.cpp"
#include "bst.h"
#include "hashTable.cpp"
#include "hashTable.h"
#include "SortingAlgorithms.cpp"
#include "SortingAlgorithms.h"

using namespace std;

struct DocumentItem
{
	string documentName;
	int count;

	DocumentItem()
	{}

	DocumentItem(string DOCNAME, int COUNT)
		:documentName(DOCNAME), count(COUNT){}
};

struct WordItem {
	string word;
	vector<DocumentItem> documentInfo;

	WordItem()
	{}

	WordItem(string WORDNAME)
		:word(WORDNAME){}

	bool operator < (const WordItem & v)
	{
		return word < v.word;
	}

	bool operator >(const WordItem & v)
	{
		return word > v.word;
	}

	bool operator == (const WordItem & v)
	{
		return word == v.word;
	}

	bool operator <= (const WordItem & v)
	{
		return word <= v.word;
	}

	bool operator >= (const WordItem & v)
	{
		return word >= v.word;
	}

	bool operator != (const WordItem & v)
	{
		return word != v.word;
	}
};

WordItem NOT_FOUND; //Created to be used for the case of not finding searched object.

//Create BST
bst<WordItem> searchTree(NOT_FOUND);
vector<WordItem> bstResults; //used to store binary search tree results, this results will be printed

//Create Heap
HashTable<WordItem> hashTable(NOT_FOUND);
vector<WordItem> hashTableResults;  //used to store hash table results, this results will be printed

//Binart Search Results
vector<WordItem> binarySearchResults; //used to store binary search results, this results will be printed

//Create Array to be pre-processed
vector<WordItem> wordArray;
HashTable<WordItem> dummyHashTable(NOT_FOUND); //dummy hash table to be used to find duplicates in wordArray faster.
//prevents for every input, running all over the array if it exists or not.



struct foundWords
{
	string filename;
	vector<WordItem> items;
};

void printResults(vector<WordItem> searchResults, vector<string> searchedWords, int numberOfFiles)
{
	bool exist = false;
	vector<foundWords> results;

	for (int j = 0; j < searchResults.size(); j++)
	{
		for (int k = 0; k < searchResults[j].documentInfo.size(); k++)
		{
			string fileName = searchResults[j].documentInfo[k].documentName;
			foundWords temp;
			temp.filename = fileName;

			if (results.empty())
			{
				temp.items.push_back(searchResults[j]);
				results.push_back(temp);
			}
			else{

				for (int i = 0; i < results.size(); i++)
					if (searchResults[j].documentInfo[k].documentName == results[i].filename)
					{
					results[i].items.push_back(searchResults[j]);
					exist = true;
					break;
					}
				if (!exist)
				{
					temp.items.push_back(searchResults[j]);
					results.push_back(temp);
				}
			}
		}
	}
	bool init = false;
	for (int i = 0; i < results.size(); i++)
	{
		if (results[i].items.size() == searchedWords.size())
		{
			init = true;
			cout << "in Document " << results[i].filename << ", ";
			for (int j = 0; j < results[i].items.size(); j++)
			{
				cout << results[i].items[j].word << " found";
				for (int k = 0; k < results[i].items[j].documentInfo.size(); k++)
				{
					if (results[i].filename == results[i].items[j].documentInfo[k].documentName)
					{
						cout << " " << results[i].items[j].documentInfo[k].count << " times";
						break;
					}
				}

				if (j == results[i].items.size() - 1)
					cout << "." << endl;
				else
					cout << ", ";
			}
		}
	}
	if (!init)
		cout << "No document contains the given query" << endl;
}

//PRE-PROCESSES
void preprocessBST(string word, string fileName)
{
	DocumentItem dctItem;
	dctItem.documentName = fileName;
	dctItem.count = 1;

	WordItem wrdItem;
	wrdItem.word = word;
	wrdItem.documentInfo.push_back(dctItem);
	searchTree.insert(wrdItem);
}

void preprocessHASHTABLE(string word, string fileName)
{
	DocumentItem dctItem;
	dctItem.documentName = fileName;
	dctItem.count = 1;

	WordItem wrdItem;
	wrdItem.word = word;
	wrdItem.documentInfo.push_back(dctItem);
	hashTable.insert(wrdItem);
}

void preprocessArray(string word, string fileName)
{
	DocumentItem dctItem;
	dctItem.documentName = fileName;
	dctItem.count = 1;

	WordItem wrdItem;
	wrdItem.word = word;
	wrdItem.documentInfo.push_back(dctItem);


	if (wordArray.size() == 0)
		wordArray.push_back(NOT_FOUND);
	bool check = true;
	if (dummyHashTable.find(wrdItem).word == "")
	{
		wordArray.push_back(wrdItem);
		dummyHashTable.insert(wrdItem);
	}
	else
	{
		for (int i = 0; i < wordArray.size(); i++)
		{
			if (wrdItem.word == wordArray[i].word)
			{
				for (int j = 0; j < wordArray[i].documentInfo.size(); j++)
				{
					if (wrdItem.documentInfo[0].documentName == wordArray[i].documentInfo[j].documentName)
					{
						wordArray[i].documentInfo[j].count++;
						check = false;
						break;
					}
				}
				if (check)
					wordArray[i].documentInfo.push_back(wrdItem.documentInfo[0]);
			}
		}
	}
}

//FIND FUNCTIONS
void findQueriedBST(vector<string> toBeFound, int k)
{
	for (int i = 0; i < toBeFound.size(); i++)
	{
		WordItem queried(""), found("");
		queried.word = toBeFound[i];
		found = searchTree.find(queried);
		if (found.word != "" && k == 0)
			bstResults.push_back(found);
		else if (found.word == "")
			bstResults.empty();
	}
}

void findQueriedHASHTABLE(vector<string> toBeFound, int k)
{
	for (int i = 0; i < toBeFound.size(); i++)
	{
		WordItem queried, found;
		queried.word = toBeFound[i];
		found = hashTable.find(queried);
		if (found.word != "" && k == 0)
			hashTableResults.push_back(found);
		else
			hashTableResults.empty();
	}
}

void findQueriedBINARYSEARCH(vector<WordItem> &array, int LOW, int HIGH, vector<string> &toBeFound, int k)
{

	for (int i = 0; i < toBeFound.size(); i++)
	{
		int low = LOW, high = HIGH;
		WordItem found("");
		while (low <= high)
		{
			int mid = (low + high) / 2;
			if (toBeFound[i] == array[mid].word)
			{
				found = array[mid];
				if (found.word != "")
				{
					if (k == 0)
						binarySearchResults.push_back(array[mid]);
					break;
				}
				else
					binarySearchResults.empty();
			}
			else if (toBeFound[i] > array[mid].word) // x is on the right side
				low = mid + 1;
			else                       // x is on the left side
				high = mid - 1;
		}
	}
}


int main()
{
	int numberOfFiles = 0;
	cout << "Enter number of input files: ";
	cin >> numberOfFiles;

	//Checks if integer input valid or not
	if (cin.fail())
	{
		cout << "Your input is not valid. It must be an integer. Program has been stopped working." << endl;
		system("pause");
		return 0;
	}

	//Gets file names and checks it exists or not
	vector<string> fileNames;
	for (int i = 0; i < numberOfFiles; i++)
	{
		string fileName;
		bool fail = false;
		do{
			cout << "Enter " << i + 1 << ". file name:";
			cin >> fileName;
			fstream test(fileName);
			fail = false;
			if (test.fail())
			{
				cout << "The file " << fileName << " does not exist." << endl;
				fail = true;
			}
			else
				fileNames.push_back(fileName);
		} while (fail);
	}


	//Extract each word from files to be sent to preprocess
	for (int fileNumber = 0; fileNumber < numberOfFiles; fileNumber++)
	{
		fstream inFile(fileNames[fileNumber]);
		string line = "", temp = "";

		while (getline(inFile, line))
		{
			//This for loop separates the alphabetical characters from others and creates words to send to preprocess
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] >= 65 && line[i] <= 90)
					line[i] += 32; //to make it lowercase.
				if (line[i] >= 97 && line[i] <= 122)
					temp += line[i];
				else
				{
					if (temp != "")
					{
						preprocessBST(temp, fileNames[fileNumber]);
						preprocessHASHTABLE(temp, fileNames[fileNumber]);
						preprocessArray(temp, fileNames[fileNumber]);
					}
					temp = "";
				}

				if (line[i + 1] == '\0')
				{
					if (temp != "")
					{
						preprocessBST(temp, fileNames[fileNumber]);
						preprocessHASHTABLE(temp, fileNames[fileNumber]);
						preprocessArray(temp, fileNames[fileNumber]);
					}
					temp = "";
				}
			}
		}
	}

	cout << "\nAfter preprocessing, the unique word count is " << hashTable.getWordCount() << ". Current load ratio is " << hashTable.getLoadFactor() << endl;


	string queried = "";
	cout << "Enter queried words in one line: ";
	cin.get();	//consume the '\n' of previous input.
	getline(cin, queried);

	string temp = "";
	vector<string> quariedWords; //Keeps the quaried words

	//This for loop separates the alphabetical characters from others and stores them in vector as quaried strings
	for (int i = 0; i < queried.size(); i++)
	{
		if (queried[i] >= 65 && queried[i] <= 90)
			queried[i] += 32; //to make it lowercase.
		if (queried[i] >= 97 && queried[i] <= 122)
			temp += queried[i];
		else
		{
			if (temp != "")
				quariedWords.push_back(temp);
			temp = "";
		}

		if (queried[i + 1] == '\0')
		{
			if (temp != "")
				quariedWords.push_back(temp);
			temp = "";
		}
	}



	//Time calculation for Binary Search Tree
	cout << "\nTime calculation for Binary Search Tree";
	int k = 10000;
	clock_t start = clock();
	for (int i = 0; i < k; i++)
	{
		findQueriedBST(quariedWords, i);
	}
	double end = float(clock() - start);
	double bstTime = end / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Hash Table 
	cout << "Time calculation for Hash Table";
	start = clock();
	for (int i = 0; i < k; i++)
	{
		findQueriedHASHTABLE(quariedWords, i);
	}
	end = float(clock() - start);
	double hTableTime = end / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Binary Search
	vector<WordItem> BinarySearchArray = wordArray;
	//A quick sort for array to be used in binary search
	quicksort(BinarySearchArray, 0, BinarySearchArray.size() - 1, "Median");
	cout << "Time calculation for Binary Search";
	start = clock();
	for (int i = 0; i < k; i++)
	{
		findQueriedBINARYSEARCH(BinarySearchArray, 0, BinarySearchArray.size() - 1, quariedWords, i);
	}
	end = float(clock() - start);
	double binarySearchTime = end / k;
	cout << " - COMPLETED" << endl;

	//These if else statements are only for determining k value in terms of the size of the input.
	//Here the point is being time friendly. For instance, there is no point of running insertion sort for 20.000+ words 100 times.
	if (wordArray.size() <= 1000)
		k = 100;
	else if (wordArray.size() > 1000 && wordArray.size() <= 10000)
		k = 10;
	else if (wordArray.size() > 20000)
		k = 1;
	else if (wordArray.size() > 10000)
		k = 3;

	//Time calculation for Quick Sort (Median)
	cout << "Time calculation for Quick Sort (Median)";
	double sum = 0;
	vector<WordItem> QuickSortArray = wordArray; //Copy original array to be sorted
	for (int i = 0; i <= k; i++)
	{
		QuickSortArray = wordArray;
		clock_t start = clock();
		quicksort(QuickSortArray, 0, QuickSortArray.size() - 1, "Median");
		double end = float(clock());
		sum += end - start;
	}
	double QuickSortTimeMedian = sum / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Quick Sort (Random)
	cout << "Time calculation for Quick Sort (Random)";
	k = 10;
	sum = 0;
	for (int i = 0; i <= k; i++)
	{
		QuickSortArray = wordArray;
		clock_t start = clock();
		quicksort(QuickSortArray, 0, QuickSortArray.size() - 1, "Random");
		double end = float(clock());
		sum += end - start;
	}
	double QuickSortTimeRandom = sum / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Quick Sort (First)
	cout << "Time calculation for Quick Sort (First)";
	sum = 0;
	for (int i = 0; i <= k; i++)
	{
		QuickSortArray = wordArray;
		clock_t start = clock();
		quicksort(QuickSortArray, 0, QuickSortArray.size() - 1, "First");
		double end = float(clock());
		sum += end - start;
	}
	double QuickSortTimeFirst = sum / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Merge Sort
	cout << "Time calculation for Merge Sort";
	sum = 0;
	vector<WordItem> MergeSortArray = wordArray; //Copy original array to be sorted
	for (int i = 0; i <= k; i++)
	{
		MergeSortArray = wordArray;
		clock_t start = clock();
		mergeSort(MergeSortArray, 0, MergeSortArray.size()-1);
		double end = float(clock());
		sum += end - start;
	}
	double MergeSortTime = sum / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Heap Sort
	cout << "Time calculation for Heap Sort";
	sum = 0;
	vector<WordItem> HeapSortArray = wordArray; //Copy original array to be sorted
	for (int i = 0; i <= k; i++)
	{
		HeapSortArray = wordArray;
		clock_t start = clock();
		heapsort(HeapSortArray);
		double end = float(clock());
		sum += end - start;
	}
	double HeapSortTime = sum / k;
	cout << " - COMPLETED" << endl;


	//Time calculation for Insertion Sort 
	cout << "Time calculation for Insertion Sort";
	sum = 0;
	vector<WordItem> InsertionSortArray = wordArray; //Copy original array to be sorted
	for (int i = 0; i <= k; i++)
	{
		InsertionSortArray = wordArray;
		clock_t start = clock();
		insertionSort(InsertionSortArray);
		double end = float(clock());
		sum += end - start;
	}
	double InsertionSortTime = sum / k;
	cout << " - COMPLETED\n\n";



	//Print results
	printResults(bstResults, quariedWords, numberOfFiles);
	printResults(hashTableResults, quariedWords, numberOfFiles);
	printResults(binarySearchResults, quariedWords, numberOfFiles);


	cout << "\nBinary Search Tree Time: " << bstTime << "\n";
	cout << "Hash Table Time: " << hTableTime << "\n";
	cout << "Binary Search Time: " << binarySearchTime << "\n";


	cout << "\nQuick Sort(median) Time: " << QuickSortTimeMedian << "\n";
	cout << "Quick Sort(random) Time: " << QuickSortTimeRandom << "\n";
	cout << "Quick Sort(first) Time: " << QuickSortTimeFirst << "\n";
	cout << "Merge Sort Time: " << MergeSortTime << "\n";
	cout << "Heap Sort Time: " << HeapSortTime << "\n";
	cout << "Insertion Sort Time: " << InsertionSortTime << "\n";


	cout << "\nSpeed Up BST/HST: " << bstTime / hTableTime << endl;
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << MergeSortTime / QuickSortTimeMedian << endl;
	cout << "Speed Up Heap Sort/Quick Sort(Median): " << HeapSortTime / QuickSortTimeMedian << endl;
	cout << "Speed Up Insertion Sort/Quick Sort(Median): " << InsertionSortTime / QuickSortTimeMedian << endl;


	cout << "\nSpeed Up Binary Search Tree Time/Binary Search: " << bstTime / binarySearchTime << endl;
	cout << "Speed Up Hash Table Time/Binary Search  : " << hTableTime / binarySearchTime << endl;

	searchTree.makeEmpty(); //Deletes tree
	hashTable.makeEmpty(); //Empties the hash table
	wordArray.clear(); //Clears the array

	system("pause");
	return 0;
}
