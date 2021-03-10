#ifndef BST
#define BST

using namespace std;


template <class Comparable>
class bst;

template <class Comparable>
class bstNode
{
	Comparable 	element;
	bstNode   		*left;
	bstNode   		*right;

	bstNode(const Comparable & theElement,
		bstNode *lt, bstNode *rt, int h = 0)
		: element(theElement), left(lt), right(rt){ }

	friend class bst<Comparable>;
};

template <class Comparable>
class bst
{
public:
	explicit bst(const Comparable & notFound);
	bst(const bst & rhs);

	const Comparable & find(const Comparable & x) const;
	bool isEmpty() const;
	void printTree();

	void makeEmpty();
	void insert(Comparable & x);
	void remove(const Comparable & x);

private:
	bstNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt(bstNode<Comparable> *t) const;

	void insert(Comparable & x, bstNode<Comparable> * & t);
	void remove(const Comparable & x, bstNode<Comparable> * & t) const;

	bstNode<Comparable> * findMin(bstNode<Comparable> *t) const;
	bstNode<Comparable> * findMax(bstNode<Comparable> *t) const;
	bstNode<Comparable> * find(const Comparable & x, bstNode<Comparable> *t) const;

	void makeEmpty(bstNode<Comparable> * & t) const;
	void printTree(bstNode<Comparable> *t);
};


#endif 
