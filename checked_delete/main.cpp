#include <iostream>

class ClassA
{
public:
	int v;

public:
	ClassA(){}
	~ClassA(){}
};

class ClassB
{
};

class ClassC;

class Empty { };
struct X : Empty 
{
	int a;
	// ...
};

template<class T> inline void checked_delete(T * x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete x;
}

void f()
{
	Empty a, b;
	if (&a == &b) std::cout << "impossible: report error to compiler supplier";

	Empty* p1 = new Empty;
	Empty* p2 = new Empty;
	if (p1 == p2) std::cout << "impossible: report error to compiler supplier";
}

void f(X* p)
{
	void* p1 = p;
	void* p2 = &(p->a);
	if (p1 == p2) std::cout << "nice: good optimizer";
}

int main()
{
	//ClassA a;
	//std::cout << sizeof(a) << std::endl;
	//std::cout << sizeof(ClassA) << std::endl;

	//ClassB b;
	//std::cout << sizeof(b) << std::endl;
	//std::cout << sizeof(ClassB) << std::endl;

	f();
	f(new X());

	int t;
	std::cin >> t;
	return 0;
}