#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
class Base
{
    public:
        Base(){ std::cout<<"Base::Base()"<<std::endl; }
        //~Base(){ std::cout<<"Base::~Base()"<<std::endl; }
        virtual ~Base(){ std::cout<<"Base::~Base()"<<std::endl; }

        virtual void Print(void)
        {
            std::cout<<"Base::Printf"<<std::endl;
        }
		virtual void f(int i) {cout << "Base::f" << i << endl;}
		void g(void) {cout << "Base::g"<< endl;}
		void h(void) {cout << "Base::h" << endl;}

};

class Children:public Base
{
    public:
        Children(){ std::cout<<"Children::Children()"<<std::endl; }
        ~Children(){ std::cout<<"Children::~Children()"<<std::endl; }
        virtual void Print(void)
        {
            std::cout<<"Children::Printf"<<std::endl;
        }
		void f(int i) {cout << "child::f" << i << endl;}
		void g(void) {cout << "child::g"<< endl;}
		void h(int i) {cout << "child::h" << i << endl;}
};



int main(int argc, char *argv[])
{
    Base *base1 = new Base;
    std::cout<<"-------------------------------"<<std::endl;
    Base *base2 = new Children;
    std::cout<<"-------------------------------"<<std::endl;
    Children *child = new Children;
    std::cout<<"-------------------------------"<<std::endl;
    Base *child1 = child;
    std::cout<<"-------------------------------"<<std::endl;

    base1->Print();
    child->Print();
    child1->Print();

    std::cout<<"-------------------------------"<<std::endl;
    delete base1;
    std::cout<<"-------------------------------"<<std::endl;
    delete base2;
    std::cout<<"-------------------------------"<<std::endl;
    delete child;
////////////////////////////////////////////////////////////
    std::cout<<"//////////////////////////////////////////////"<<std::endl;

//如果没有virtual 关键字，跟b/d指向的类型有关，否则就跟c分配的对象类型有关
	Base *base3 = new Base;
    std::cout<<"-------------------------------"<<std::endl;
    Children *child3 = new Children;
    std::cout<<"-------------------------------"<<std::endl;

    Children *c = new Children;
	Base *b = c;
	Children *d = c;

	base3->f(1);
	child3->f(2);
	base3->g();
	child3->g();
	base3->h();
	child3->h(2);
    std::cout<<"-------------------------------"<<std::endl;

	b->f(1);
	d->f(2);
	b->g();
	d->g();
	b->h();
	d->h(2);

    delete base3;
    delete child3;
    delete c;
    std::cout<<"//////////////////////////////////////////////"<<std::endl;
////////////////////////////////////////////////////////////
    map<int, int> m ;
    m.insert(pair<int, int>(1, 1)) ;
    m.insert(pair<int, int>(2, 2)) ;
    m.insert(pair<int, int>(3, 3)) ;
    m.insert(pair<int, int>(4, 3)) ;
    m.insert(pair<int, int>(5, 5)) ;
	for (map<int,int>::iterator iter=m.begin();iter!=m.end();) {
		if (3 == iter->second)
			m.erase(iter++);
		else
			iter++;
	}

	vector<int> it;
	it.push_back(1);
	it.push_back(2);
	it.push_back(3);
	it.push_back(3);
	it.push_back(5);
	it.push_back(6);
	it.erase(remove(it.begin(), it.end(),3), it.end());

	for (vector<int>::iterator iter=it.begin();iter!=it.end();iter++) {
		printf("%d ", *iter);
	}
	printf("\n");
	for (map<int,int>::iterator iter=m.begin();iter!=m.end();iter++) {
		printf("%d-%d ", iter->first,iter->second);
	}
    return 0;
}
/*
Base::Base()
-------------------------------
Base::Base()
Children::Children()
-------------------------------
Base::Base()
Children::Children()
-------------------------------
-------------------------------
Base::Printf
Children::Printf
Children::Printf
-------------------------------
Base::~Base()
-------------------------------
Children::~Children()
Base::~Base()
-------------------------------
Children::~Children()
Base::~Base()
//////////////////////////////////////////////
Base::Base()
-------------------------------
Base::Base()
Children::Children()
-------------------------------
Base::Base()
Children::Children()
Base::f1
child::f2
Base::g
child::g
Base::h
child::h2
-------------------------------
child::f1
child::f2
Base::g
child::g
Base::h
child::h2
Base::~Base()
Children::~Children()
Base::~Base()
Children::~Children()
Base::~Base()
//////////////////////////////////////////////
1 2 5 6 
1-1 2-2 5-5
*/