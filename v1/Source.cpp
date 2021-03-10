#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "AVLSearchTree.cpp"
#include "AVLSearchTree.h"



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
};

WordItem NOT_FOUND;
AvlTree<WordItem> searchTree(NOT_FOUND);
vector<WordItem> searchResults;

template<class compare>
bool alreadyHas(vector<compare> v, compare key)
{
	if (count(v.begin(), v.end(), key))
		return true;
	else
		return false;
}

struct foundWords
{
	string filename;
	vector<WordItem> items;
};

void printResults(int numberOfFiles, int wordCount)
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
	bool init;
	for (int i = 0; i < results.size(); i++)
	{
		init = false;
		if (results[i].items.size() == wordCount)
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

void preprocess(string word, string fileName)
{
	DocumentItem dctItem;
	dctItem.documentName = fileName;
	dctItem.count = 1;

	WordItem wrdItem;
	wrdItem.word = word;
	wrdItem.documentInfo.push_back(dctItem);
	searchTree.insert(wrdItem);
}

void findQueried(string word)
{
	WordItem queried, found;
	queried.word = word;

	found = searchTree.find(queried);
	if (found.word != "")
		searchResults.push_back(found);
}

int main()
{
	int numberOfFiles = 0;
	cout << "Enter number of input files: ";
	cin >> numberOfFiles;
	bool check;
	if (cin.fail())
	{
		cout << "Your input is not valid. It must be an integer. Program has been stopped working." << endl;
		system("pause");
		return 0;
	}


	for (int i = 0; i < numberOfFiles; i++)
	{
		string fileName;
		bool fail = false;
		do
		{
			cout << "Enter " << i + 1 << ". file name:";
			cin >> fileName;
			fstream inFile(fileName);
			if (inFile.fail())
			{
				cout << "Error! File " << fileName << " can not be found." << endl;
				fail = true;
			}
			else
			{
				string line = "", temp = "";

				while (getline(inFile, line))
				{
					int a = line.size();
					for (int i = 0; i < line.size(); i++)
					{
						if (line[i] >= 65 && line[i] <= 90)
							line[i] += 32; //to make it lowercase.
						if (line[i] >= 97 && line[i] <= 122)
							temp += line[i];
						else
						{
							if (temp != "")
								preprocess(temp, fileName);
							temp = "";
						}
					}
				}
				fail = false;
			}
		} while (fail);
	}

	string queriedWords = "";
	cout << "Enter queried words in one line: ";
	cin.get();	//consume the '\n' of previous input.
	getline(cin, queriedWords);

	string temp = "";
	int wordCount = 0;
	for (int i = 0; i < queriedWords.size(); i++)
	{
		if (queriedWords[i] >= 65 && queriedWords[i] <= 90)
			queriedWords[i] += 32; //to make it lowercase.
		if (queriedWords[i] >= 97 && queriedWords[i] <= 122)
			temp += queriedWords[i];
		else
		{
			if (temp != "")
			{
				findQueried(temp);
				wordCount++;
			}
			temp = "";
		}

		if (i == queriedWords.size() - 1)
		{
			if (temp != "")
			{
				findQueried(temp);
				wordCount++;
			}
			temp = "";
		}
	}


	printResults(numberOfFiles, wordCount);

	searchTree.makeEmpty(); //Deletes tree

	system("pause");
	return 0;
}