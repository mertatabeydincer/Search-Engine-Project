#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include "bst.cpp"
#include "bst.h"
#include "hashTable.cpp"
#include "hashTable.h"



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
};

WordItem NOT_FOUND; //Created to be used for the case of not finding searched object.

//Create BST
bst<WordItem> searchTree(NOT_FOUND);
vector<WordItem> bstResults;

//Create Heap
HashTable<WordItem> hashTable(NOT_FOUND);
vector<WordItem> hashTableResults;

struct foundWords
{
	string filename;
	vector<WordItem> items;
};

void printResults(vector<WordItem> searchResults, int numberOfFiles, int numberOfQuaried)
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
		if (results[i].items.size() == numberOfQuaried)
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

void findQueriedBST(vector<string> toBeFound, int k)
{
	for (int i = 0; i < toBeFound.size(); i++)
	{
		WordItem queried, found;
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
					}
					temp = "";
				}

				if (line[i + 1] == '\0')
				{
					if (temp != "")
					{
						preprocessBST(temp, fileNames[fileNumber]);
						preprocessHASHTABLE(temp, fileNames[fileNumber]);
					}
					temp = "";
				}
			}
		}
	}


	cout << "\n\nAfter preprocessing, the unique word count is " << hashTable.getWordCount() << ". Current load ratio is " << hashTable.getLoadFactor() << endl;

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
	int k = 2500;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		findQueriedBST(quariedWords, i);
	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);
	int bstTime = BSTTime.count() / k;


	//Time calculation for Hash Table 
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		findQueriedHASHTABLE(quariedWords, i);
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);
	int hTableTime = HTTime.count() / k;

	printResults(bstResults, numberOfFiles, quariedWords.size());
	printResults(hashTableResults, numberOfFiles, quariedWords.size());

	cout << "\nTime: " << bstTime << "\n";
	cout << "\nTime: " << hTableTime << "\n";

	cout << "Speed Up:" << float(bstTime) / float(hTableTime) << endl;

	searchTree.makeEmpty(); //Deletes tree
	hashTable.makeEmpty(); //Empties the hash table

	system("pause");
	return 0;
}