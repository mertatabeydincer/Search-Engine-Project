#include <vector>
#include <iostream>
using namespace std;

//////////////
//QUICK SORT//
//////////////

template <class Comparable>
void swap(vector<Comparable> & obj1, Comparable & obj2)
{
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

template <class Comparable>
Comparable & median3(vector<Comparable> & a, int left, int right)
{
	int center = (left + right) / 2;
	if (a[center] < a[left])
		swap(a[left], a[center]);
	if (a[right] < a[left])
		swap(a[left], a[right]);
	if (a[right] < a[center])
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

template <class Comparable>
Comparable & random(vector<Comparable> & a, int left, int right)
{
	srand(time(NULL));
	int random = left + rand() % (right - left);

	if (random > right)
		random %= right;

	return a[random];

}

template <class Comparable>
Comparable & first(vector<Comparable> & a, int left, int right)
{
	Comparable pivot = a[left];
	while (left <= right) {
		while (a[left] < pivot)
			left++;
		while (a[right] > pivot)
			right--;
		if (left <= right) {
			Comparable tmp = a[left];
			a[left] = a[right];
			a[right] = tmp;
			left++;
			right--;
		}
	}
	return a[left];
}

template <class Comparable>
void quicksort(vector<Comparable> & a, int left, int right, string method)
{
	if (left + 10 <= right)
	{
		Comparable pivot;
		if (method == "Median")
			pivot = median3(a, left, right);
		else if (method == "Random")
			pivot = random(a, left, right);
		else if (method == "First")
			pivot = first(a, left, right);


		// Begin partitioning
		int i = left, j = right - 1;
		for (;;)
		{
			while (a[++i] < pivot) {
				cout << "";
			}

			while (pivot < a[--j]) {
				cout << "";
			}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}
		swap(a[i], a[right - 1]);   // Restore pivot

		quicksort(a, left, i - 1, method);       // Sort small elements
		quicksort(a, i + 1, right, method);    // Sort large elements
	}
	else  // Do an insertion sort on the subarray
		insertionSort(a, left, right);
}

template <class Comparable>
void insertionSort(vector<Comparable> & a, int left, int right)
{
	for (int p = left + 1; p <= right; p++)
	{
		Comparable tmp = a[p];
		int j;

		for (j = p; j > left && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}

}

//////////////
//MERGE SORT//
//////////////

template <class Comparable>
void merge(vector<Comparable> & array, int low, int mid, int high)
{
	int start = mid + 1;
	if (array[mid] <= array[start]) {
		return;
	}

	while (low <= mid && start <= high) {

		if (array[low] <= array[start]) {
			low++;
		}
		else {
			Comparable value = array[start];
			int index = start;


			while (index != low) {
				array[index] = array[index - 1];
				index--;
			}
			array[low] = value;

			low++;
			mid++;
			start++;
		}
	}
}

template <class Comparable>
void mergeSort(vector<Comparable> & array, int low, int high)
{

	if (low < high) {

		int middle = low + (high - low) / 2;

		// Sort first and second halves 
		mergeSort(array, low, middle);
		mergeSort(array, middle + 1, high);

		merge(array, low, middle, high);
	}
}


/////////////
//HEAP SORT//
/////////////


inline int leftChild(int i)
{
	return 2 * i + 1;
}




template <class Comparable>
void heapsort(vector<Comparable> & a)
{
	
	// buildHeap
	for (int i = a.size() / 2; i >= 0; i--)
		percDown(a, i, a.size());

	
		
	// sort
	for (int j = a.size() - 1; j > 0; j--)
	{
		swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}

template <class Comparable>
void percDown(vector<Comparable> & a, int i, int n)
{
	int child;
	Comparable tmp;

	for (tmp = a[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			child++;
		if (a[child] > tmp)
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}


//////////////////
//INSERTION SORT//
//////////////////

template <class Comparable>
void insertionSort(vector <Comparable> & a)
{
	int j;
	for (int p = 1; p < a.size(); p++)
	{
		Comparable tmp = a[p];
		for (j = p; j > 0 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}