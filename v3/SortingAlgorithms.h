#ifndef SORTINGALGORITHMS
#define SORTINGALGORITHMS

template <class Comparable>
class SortingAlgorithms
{
	//QUICK SORT
	void swap(Comparable & obj1, Comparable & obj2);
	Comparable & median3(vector<Comparable> & a, int left, int right);
	Comparable & random(vector<Comparable> & a, int left, int right);
	Comparable & first(vector<Comparable> & a, int left, int right);
	void quicksort(vector<Comparable> & a, int left, int right, string method);
	void insertionSort(vector<Comparable> & a, int left, int right);


	//MERGE SORT
	void merge(vector<Comparable> & array, int start, int mid, int end);
	void mergeSort(vector<Comparable> & array, int l, int r);


	//HEAP SORT
	inline int leftChild(int i);
	void heapsort(vector<Comparable> & a);
	void percDown(vector<Comparable> & a, int i, int n);


	//INSERTION SORT
	void insertionSort(vector <Comparable> & a);
};


#endif