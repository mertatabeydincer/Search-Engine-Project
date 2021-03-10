#include <string>
#include "hashTable.h"

using namespace std;

//PUBLIC:
/**
* Construct the hash table.
*/
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size)
	: ITEM_NOT_FOUND(notFound), array(nextPrime(size))
{
	makeEmpty();
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	currentSize = 0;
	for (int i = 0; i < array.size(); i++)
		array[i].info = EMPTY;
}

/**
* Remove item x from the hash table.
*  x has to be in the table
*/
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x)
{
	int currentPos = findPos(x);
	if (isActive(currentPos))
		array[currentPos].info = DELETED;
}

/**
* Find item x in the hash table.
* Return the matching item, or ITEM_NOT_FOUND, if not found.
*/
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x)
{
	int currentPos = findPos(x);
	if (isActive(currentPos))
		return array[currentPos].element;

	return   ITEM_NOT_FOUND;
}

/**
* Insert item x into the hash table. If the item is
* already present, then do nothing.
*/
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
	// Insert x as active
	int currentPos = findPos(x);
	if (isActive(currentPos))
	{

		if (array[currentPos].element.word == x.word)
		{
			bool check = true;
			for (int i = 0; i < array[currentPos].element.documentInfo.size(); i++)
			{
				if (x.documentInfo[0].documentName == array[currentPos].element.documentInfo[i].documentName)
				{
					array[currentPos].element.documentInfo[i].count++;
					check = false;
					break;
				}
			}
			if (check)
				array[currentPos].element.documentInfo.push_back(x.documentInfo[0]);
		}
		else
			return;
	}
	else
	{
		array[currentPos] = HashEntry(x, ACTIVE);
		int pre_size = array.size();
		// enlarge the hash table if necessary 
		float load_factor = float(++wordCount) / float(array.size());
		if (load_factor >= (9.0 / 10.0))
		{
			rehash();
			//cout << "previous table size: " << pre_size << ", new table size: " << array.size() << ", current unique word count: " << wordCount + 1 << ", current load factor: " << float(wordCount) / float(array.size()) << endl;
		}
	}
}




//PRIVATE:
template <class HashedObj>
int HashTable<HashedObj>::hash(const HashedObj key, int size)
{
	int position = 0;
	
	for (int i = 0; i < key.word.length(); i++)
		position = 37 * position + pow(3, i)*key.word[i];

	position = position % size;

	if (position < 0)
		position = position + size;

	return position;
}

/**
* Method that performs quadratic probing resolution.
* Return the position where the search for x terminates.
*/
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) 
{
	int collisionNum = 0;
	int currentPos = hash(x, array.size());

	while (array[currentPos].info != EMPTY && array[currentPos].element.word != x.word)
	{
		currentPos = currentPos + 2 * ++collisionNum-1;  // Compute ith probe
		while(currentPos >=  array.size())
			currentPos -= array.size();
	}

	return currentPos;
}


/**
* Return true if currentPos exists and is active.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
	return array[currentPos].info == ACTIVE;
}


/**
* Expand the hash table.
*/
template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
	vector<HashEntry> oldArray = array;

	// Create new double-sized, empty table
	array.resize(nextPrime(2 * oldArray.size()));
	for (int j = 0; j < array.size(); j++)
		array[j].info = EMPTY;

	// Copy table over
	wordCount = 0;
	for (int i = 0; i < oldArray.size(); i++)
		if (oldArray[i].info == ACTIVE)
			insert(oldArray[i].element);

	//cout << "rehashed..." << endl;
}




