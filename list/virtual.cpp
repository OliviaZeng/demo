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
