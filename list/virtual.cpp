#include <stdio.h>
#include <stdlib.h>
#include <iostream>


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

    return 0;
}
#if 0
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
#endif



