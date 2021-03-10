#ifndef HASHTABLE
#define HASHTABLE

#include <vector>

using namespace std;

template <class HashedObj>
class HashTable
{
public:
	explicit HashTable(const HashedObj & notFound, int size = 53);
	
	

	const HashedObj & find(const HashedObj & x);

	
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);

	const HashTable & operator=(const HashTable & rhs);

	enum EntryType { ACTIVE, EMPTY, DELETED };

	int getWordCount()
	{
		return wordCount;
	};

	float getLoadFactor()
	{
		return float(wordCount) / float(array.size());
	};

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj(),
			EntryType i = EMPTY)
			: element(e), info(i) { }
	};

	vector<HashEntry> array;
	int wordCount;
	int currentSize;
	int previousSize;
	const HashedObj ITEM_NOT_FOUND;

	int hash(const HashedObj key, int size);
	bool isActive(int currentPos) const;
	int findPos(const HashedObj & x);
	void rehash();

	bool isPrime(int n)
	{
		if (n == 2 || n == 3)
			return true;

		if (n == 1 || n % 2 == 0)
			return false;

		for (int i = 3; i * i <= n; i += 2)
			if (n % i == 0)
				return false;

		return true;
	}

	/**
	* Internal method to return a prime number
	* at least as large as n.  Assumes n > 0.
	*/
	int nextPrime(int n)
	{
		if (n % 2 == 0)
			n++;

		for (; !isPrime(n); n += 2)
			;

		return n;
	}
};




#endif
