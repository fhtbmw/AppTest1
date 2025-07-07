#include <iostream>
#include <memory>

class A
{
public:
	A()
	{
		std::cout << "A constructor" << std::endl;
	}

	~A()
	{
		std::cout << "A destructor" << std::endl;
	}
};

int main()
{
	{
		//��ʼ����ʽ1
		std::shared_ptr<A> sp1(new A());

		std::cout << "use count: " << sp1.use_count() << std::endl;

		//��ʼ����ʽ2
		std::shared_ptr<A> sp2(sp1);
		std::cout << "use count: " << sp1.use_count() << std::endl;

		sp2.reset();
		std::cout << "use count: " << sp1.use_count() << std::endl;

		{
			std::shared_ptr<A> sp3 = sp1;
			std::cout << "use count: " << sp1.use_count() << std::endl;
		}

		std::shared_ptr<A> sp4 = std::make_shared<A>();

		std::cout << "use count: " << sp1.use_count() << std::endl;
	}

	return 0;
}