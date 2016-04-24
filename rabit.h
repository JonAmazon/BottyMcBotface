#ifndef _H_RABIT
#define _H_RABIT

#include <cstdlib>
#include <cstdio>

//structure for getting high fidelity uniform deviates
//taken from 'Numerical Recipes 3rd Edition' page 342.
struct randomGen
{
	unsigned long long u,v,w;
	randomGen(unsigned long long j) : v(4101842887655102017LL), w(1)
	{
		u = j^v; int64();
		v = u; int64();
		w = v; int64();
	}
	inline unsigned long long int64()
	{
		u = u*2862933555777941757LL + 7046029254386353087LL;
		v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
		w = 4294957665U*(w & 0xffffffff) + (w >> 32);
		unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
		return (x+v) ^ x;
	}
	inline double doub() {return 5.42101086242752217e-20 * int64();}
	inline unsigned int int32() {return (unsigned int)int64();}
};

/* 
Rabit or "Random Access BInary Tree" is a data structure designed
specifically to have the flexible insert/delete properties of a linked
list, but also the ability to quickly choose a random element from the
list. The run times are O(log(N)) for insert/delete/random choice.
*/


template<typename T>
struct node
{
	//size: The total number of nodes in the tree starting from this node (inclusive)
	//weight: weight of this node
	//totalWeight: cummulative weight of this node and its children.
	int size;
	float weight;
	T data;

	node * parent,* left,* right;

	void print()
	{
		printf("(%d)\n",size);
	}
};

template<typename T>
struct rabit
{
	randomGen * random;
	node<T> * root;

	inline int size(node<T> * n)
	{
		return (n)?n->size:0;
	}

	inline float weight(node<T> * n)
	{
		return (n)?n->weight:0.0;
	}

	int size()
	{
		return size(root);
	}

	void initialize(unsigned long long seed)
	{
		random = new randomGen(seed);
		root = NULL;
	}

	void insertNode(node<T> * par, node<T> * n)
	{
		//Check that the parent is valid, if not
		//then we hecked up somewhere.
		if(par == NULL)
		{
			printf("Rabit error! insertNode called on NULL node\n");
		}
		//Otherwise keep jumping down the rabit hole
		//until we find a free leaf.
		else
		{
			//should we insert left?
			if(size(par->left) < size(par->right))
			{
				//if there is a leaf then follow it
				if(par->left)
				{
					insertNode(par->left,n);
				}
				//otherwise its free! Lets put the new
				//node here.
				else
				{
					n->parent = par;
					par->left = n;
				}
			}
			//or should we insert right?
			else
			{
				//if there is a leaf then follow it
				if(par->right)
				{
					insertNode(par->right,n);
				}
				//otherwise its free! Lets put the new
				//node here.
				else
				{
					n->parent = par;
					par->right = n;
				}
			}
			par->size++;
			par->weight += n->weight;
		}
	}

	node<T> * addNode(float w)
	{
		//create new node in memory
		node<T> * n = new node<T>;
		n->size = 1;
		n->weight = w;
		n->right = NULL;
		n->left = NULL;

		//If this is the first node to be
		//added to the rabit, then set it 
		//to the root.
		if(root == NULL)
		{
			n->parent = NULL;
			root = n;
		}
		//Otherwise, buckle up because we are taking
		//a trip down the rabit hole!
		else
		{
			insertNode(root,n);
		}
		return n;
	}

	node<T> * getRandomNodeFromSubTree(node<T> * par)
	{
		int R;

		//dun goofed!
		if(par == NULL)
		{
			printf("Rabit error! getRandomNode called on NULL node\n");
			return NULL;
		}

		else
		{
			//Generate random number between 0 and sizeTot-1
			R = (random->doub())*weight(par);

			//BOOM! Do this recursively based on the value of R.
			//It can be proven inductively that the probability
			//of choosing any given node of the subtree is exactly
			// prob = (1/sizeTot).
			if(R < weight(par->left)){return getRandomNodeFromSubTree(par->left);}
			else if(R > weight(par) - weight(par->right)){return getRandomNodeFromSubTree(par->right);}
			else{return par;}
		}
	}

	node<T> * getRandomNode()
	{
		if(root){return getRandomNodeFromSubTree(root);}
		else{return NULL;}
	}

	node<T> * findReplacement(node<T> * p)
	{
		node<T> * s = p;

		if(s == NULL)
		{printf("Rabit error! findReplacement called on NULL node\n");}

		//Find a bottom node on the largest sub-tree
		while((s->left) || (s->right))
		{
			if(size(s->left) > size(s->right))
			{
				s = s->left;
			}
			else
			{
				s = s->right;
			}
		}

		return s;
	}

	node<T> * deleteNode(node<T> * M)
	{
		if(M == NULL){printf("Rabit error! deleteNode called on NULL node\n");}

		float dW;
		node<T> * P = M->parent;
		node<T> * L = M->left;
		node<T> * R = M->right;
		node<T> * S,* SP; //replacement if needed

		//If the node has no children
		if((R == NULL)&&(L==NULL))
		{
			dW = weight(NULL)-weight(M);
			if(P)
			{
				//Just remove from parent and get on with your day.
				if(P->left == M){P->left = NULL;}
				else{P->right = NULL;}
				while(P)
				{
					P->size--;
					P->weight += dW;
					P = P->parent;
				}
			}
			else
			{
				//Has no children AND no parent... you just deleted the root dawg.
				root = NULL;
			}
		}
		//Only one child, and its a lefty.
		else if((R == NULL)&&(L))
		{
			dW = weight(L)-weight(M);
			if(P)
			{
				if(P->left == M){P->left = L;}
				else{P->right = L;}
				L->parent = P;
				while(P)
				{
					P->size--;
					P->weight += dW;
					P = P->parent;
				}
			}
			else
			{
				//This is the new root.
				L->parent = NULL;
				root = L;
			}
		}
		//Only one child and its a righty.
		else if((R)&&(L == NULL))
		{
			dW = weight(R)-weight(M);
			if(P)
			{
				if(P->left == M){P->left = R;}
				else{P->right = R;}
				R->parent = P;
				while(P)
				{
					P->size--;
					P->weight += dW;
					P = P->parent;
				}
			}
			else
			{
				//This is the new root.
				R->parent = NULL;
				root = R;
			}
		}
		//Oh, god damn it! We have to look at both sides
		else
		{
			S = deleteNode(findReplacement(root));
			//S is now an orphaned node that we will put in place of M
			//note that we can't use R or L anymore as they may not exist anymore
			//after the deletion of S
			S->size = M->size;
			S->left = M->left;
			S->right = M->right;
			if(S->left){S->left->parent = S;}
			if(S->right){S->right->parent = S;}
			S->parent = P;
			
			dW = weight(S) + weight(M->left) + weight(M->right) - weight(M);
			S->weight = weight(M) + dW;
			if(P)
			{
				if(P->left == M){P->left = S;}
				else{P->right = S;}
				while(P)
				{
					P->weight += dW;
					P = P->parent;
				}
			}
			else
			{
				S->parent = NULL;
				root = S;
			}
			
		}

		//I'm returning M so that you can handle any specific memory freeing
		//operations on 'data' before the node is gone forever. Try not to cause
		//a memory leak plx.
		M->left = NULL;
		M->right = NULL;
		M->parent = NULL;
		M->size = 0;
		return M;
	}

	void alterWeight(node<T> * n, float dw)
	{
		float dwRect;
		node<T> * p;

		if(weight(n) + dw < weight(n->left) + weight(n->right))
		{
			dwRect = weight(n)-(weight(n->left) + weight(n->right));
		}
		p = n;
		while(p)
		{
			p->weight += dwRect;
			p = p->parent;
		}
	}

	void scaleAllInOrder(node<T> * n, float a)
	{
		if(n)
		{
			n->weight *= a;
			scaleAllInOrder(n->left,a);
			scaleAllInOrder(n->right,a);
		}
	}

	void scaleAllBy(float a)
	{
		scaleAllInOrder(root,a);
	}

	void printNode(node<T> * n)
	{
		if(n)
		{
			printNode(n->left);
			printf("(%f,%p)",n->weight,(void *)n->parent);
			printNode(n->right);
		}
	}

	void print()
	{
		printNode(root);
		printf("\n");
	}
};

#endif