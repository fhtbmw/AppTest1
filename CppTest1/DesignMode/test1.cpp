#include <iostream>
#include <functional>
#include <assert.h>
#include <vector>
#include <memory>

#define TEST_MEMENTO

//
using namespace std;

#ifdef TEST_VISITOR

#include <iostream>

//观察者模式
class Shape;  // 前向声明
class Circle;
class Square;
class Visitor;  // 修正拼写错误

class Visitor  // 修正拼写错误
{
public:
	virtual void visit(Circle* c) = 0;  // 纯虚函数
	virtual void visit(Square* s) = 0;  // 纯虚函数
	virtual ~Visitor() = default;       // 添加虚析构函数
};

class Shape
{
public:
	virtual void accept(Visitor* v) = 0;  // 抽象类
	virtual ~Shape() = default;
};

class Circle : public Shape
{
public:
	void accept(Visitor* v) override  // 修正拼写错误
	{
		v->visit(this);
	}

	void draw()
	{
		std::cout << "Drawing a circle \n";
	}
};

class Square : public Shape
{
public:
	void accept(Visitor* v) override  // 修正拼写错误
	{
		v->visit(this);
	}

	void draw()
	{
		std::cout << "Drawing a square \n";
	}
};


class AreaVisitor : public Visitor  // 修正拼写错误
{
public:
	void visit(Circle* c) override
	{
		c->draw();
	}

	void visit(Square* s) override
	{
		s->draw();
	}
};

void test1()
{
	Shape* s1 = new Circle();
	Shape* s2 = new Square();

	AreaVisitor area;  // 修正类名拼写

	s1->accept(&area);
	s2->accept(&area);

	delete s1;
	delete s2;
}


#endif

#ifdef TEST_FACTORY

//简单工厂模式
class CashSuper
{
public:
	virtual double acceptCash(double money) = 0;
};

class CashNormal: public CashSuper
{
public:
	double acceptCash(double money) override
	{
		cout << money << endl;
		return money;
	}
};

class CashRebate : public CashSuper
{
public:
	CashRebate(double n): debate(n) {}

	double acceptCash(double money) override
	{
		money = money * debate;
		cout << money << endl;
		return money;
	}

private:
	double debate;
};

class CashFactory
{
public:
	static void createCrashAccept(int type)
	{
		CashSuper* cs = NULL;
		switch (type)
		{
		case 1:
			cs = new CashNormal();
			cs->acceptCash(200);
			break;

		case 2:
			cs = new CashRebate(0.7);
			cs->acceptCash(200);
			break;
		default:
			break;
		}
	}
};

class Fa
{
public:
	virtual double fuc(double din) = 0;

	virtual ~Fa()
	{
	
	}
};

class So1: public Fa
{
public:
	double fuc(double din) override
	{
		return din + 1;
	}

};

class So2 : public Fa
{
public:
	double fuc(double din) override
	{
		return din + 2;
	}

};

class Fac
{
public:
	Fa* fa1 = NULL;

	void CreateFunc(int type)
	{
		switch (type)
		{
		case 1:
			fa1 = new So1();
			break;

		case 2:
			fa1 = new So2();
			break;

		default:
			break;
		}
	}
};

class c1
{
public:
	int a;

protected:
	int ab;

private:
	int b;

};

class c2 : public c1
{
public:
	int c;

	void funcc2()
	{
		c = ab + 1;
	}

private:
	int d;

};

class c3 : public c2
{
public:
	int e;

	void funcc3()
	{
		e = ab + 2;
	}

private:
	int f;
};


void test1()
{
	//CashFactory cf;
	//cf.createCrashAccept(2);
	Fac fa1;
	fa1.CreateFunc(1);

	c1 c11;

	c2 c22;

	c3 c33;


}

#endif

#ifdef TEST_STRATEGY	

//策略模式
class Strategy
{
public:
	virtual void Algorithm()
	{
	
	}
};

class StrategyA : public Strategy
{
public:
	void Algorithm()
	{
		cout << "Algorithm A \n ";
	}
};

class StrategyB : public Strategy
{
public:
	void Algorithm()
	{
		cout << "Algorithm B \n";
	}
};

class Context
{
public:

	void SelectAlgorithm(int type)
	{
		switch (type)
		{
		case 1:
			str = new StrategyA();
			break;

		case 2:
			str = new StrategyB();
			break;

		default:
			break;
		}
	}

	void GetRet()
	{
		str->Algorithm();
	}

	Strategy* str = NULL;
};


void test1()
{
	Context con1;
	con1.SelectAlgorithm(2);
	con1.GetRet();
}

#endif

#ifdef TEST_DECORATOR

class Component
{
public:
	virtual void Operation() {};
};

class ConcreteComponent : public Component
{
public:
	void Operation() override	//重写Operation
	{
		cout << "撒大大是 \n";
	}

};

class Decorator : public Component
{
public:
	void SetComponent(Component com)
	{
		this->_com = com;
	}

	void Operation() override	//重写Operation
	{
		_com.Operation();
	}

protected:
	Component _com;
};

class DecoratorA : public Decorator
{
public:
	void Operation()
	{
		__super::Operation();
		cout << "DecoratorA Operation \n";
	}

private:
	string s;
	
};

class DecoratorB : public Decorator
{
public:
	void Operation()
	{
		__super::Operation();
		cout << "DecoratorB Operation \n";
	}

};


class Component2
{
public:
	virtual double calcMoney() {};

};

class CreateComponent2 : public Component2
{
public:

	double calcMoney()
	{
		return 0;
	}
};

class Decorator2 : public Component2
{
public:

	Decorator2(Component2 com2)
	{
		this->com = com2;
	}

	double calcMoney()
	{
		return com.calcMoney();
	}

protected:

	Component2 com;

};

class Decorator2A : public Decorator2
{
public:
	double calcMoney()
	{
		double money = __super::calcMoney();
		return money * 1.5;
	}

};

class Decorator2B : public Decorator2
{
public:
	double calcMoney()
	{
		double money = __super::calcMoney();
		return money * 2;
	}
};


void test1()
{
	ConcreteComponent c = ConcreteComponent();
	DecoratorA a = DecoratorA();
	DecoratorB b = DecoratorB();

	a.SetComponent(c);
	b.SetComponent(c);
	b.Operation();
}

#endif

#ifdef TEST_PROXY

class UserService
{
public:
	virtual void adduser(string name) {};

};

class UserServiceImpl : public UserService
{
public:
	void adduser(string name) override
	{
	}

};

class UserServiceProxy : public UserService
{
public:
	UserServiceProxy(UserService us) {
		this->realservice = us;
	}



private:
	UserService realservice;

};

void test1()
{

}

#endif

#ifdef TEST_FACTORYFUNCTION

//工厂方法类
class Product
{
public:
	virtual void Show() {};

};

class Factory
{
public:
	virtual Product* Manufacture() = 0;

};

class ProductA : public Product
{
public:
	void Show()
	{
		cout << "Product A \n";
	}
};

class ProductB : public Product
{
public:
	void Show()
	{
		cout << "Product B \n";
	}
};

class FactoryA : public Factory
{
public:
	Product* Manufacture()
	{
		return new ProductA;
	}

};

class FactoryB : public Factory
{
public:
	ProductB* Manufacture()
	{
		return new ProductB;
	}

};

void test1()
{
	FactoryA fa;
	fa.Manufacture()->Show();

	FactoryB fb;
	fb.Manufacture()->Show();
}


class TT
{
public:

	void show()
	{
		cout << this << " : "  << this->age << endl;
	}
	int age;
};

void test2()
{
	TT t1;
	t1.age = 2;
	t1.show();
	TT *t2 = &t1;
	t2->show();
}

#endif


#ifdef TEST_PROTYPE 

class Protype
{
public:
	virtual Protype* clone() = 0;
	virtual void show(int i)
	{
		cout << _sname << " : " << i << endl;
	}

private:
	string _sname = "Protype";
};

class Sub : public Protype
{
public:
	Sub()
	{
		cout << "Construct Sub" << endl;
	}

	Protype* clone()
	{
		//return new Sub(*this);		//clone对象,拷贝构造函数	return new Sub() : 创建新对象
		return new Sub();
	}

	void show(int i)
	{
		cout << _name << ":" << i << " " << this << endl;
	}

private:
	string _name = "Sub";
};

void test1()
{
	Protype* p1 = new Sub();
	Protype* p2 = p1->clone();
	Protype* p3 = p1->clone();
	std::unique_ptr<Protype> up1 = std::make_unique<Sub>();

	p1->show(1);
	p2->show(2);
	p3->show(3);
	(*up1).show(4);

	delete p1;
	delete p2;
	delete p3;
}


class Protype2 
{
public:

	Protype2(string sinput) :_sname(sinput)
	{
	}

	virtual std::unique_ptr<Protype2> clone(string _name) = 0;
	virtual void show() {};

private:
	string _sname;
};

class Sub2 : public Protype2
{
public:
	Sub2(string sinput) : _sSname(sinput), Protype2(sinput)
	{
		
	}

	std::unique_ptr<Protype2> clone(string _name) override
	{
		return std::make_unique<Sub2>(_name);
	}

	void show() override
	{
		cout << "Sub2" << _sSname << endl;
	}

private:
	string _sSname;
};

void test3()
{
	Protype2* pt1 = new Sub2("1");
	std::unique_ptr<Protype2> pt2 = pt1->clone("2");

	pt1->show();
	(*pt2).show();
}

#endif

#ifdef TEST_TEMPLATEFUNC

class TEMPLATE
{
public:

	void callfunc()
	{
		int a = 2;
		func1(a);
	}

	virtual void func1(int a) {};

};

class SubTemp : public TEMPLATE
{
public:

	void func1(int n)
	{
		cout << "SubTemp" << n + 1 << endl;
	}

};

void test3()
{
	TEMPLATE* tp = new SubTemp();
	tp->callfunc();
}

#endif

#ifdef TEST_FACADE

class SubA
{
public:
	void FuncA() {};
};

class SubB
{
public:
	void FuncB() {};
};

class FACADE
{
public:
	SubA suba;
	SubB subb;

	void aa()
	{
		suba.FuncA();
	}

	void bb()
	{
		subb.FuncB();
	}

};

void test3()
{
	FACADE fa;
	fa.aa();
}

#endif

#ifdef TEST_CONSTRUCTOR

class Builder
{
public:
	virtual void BuildHead(){}
	virtual void BuildLeg() {}
};

class BuilderA : public Builder
{
public:
	void BuildHead()
	{
		cout << "BuilderA buildhead " << endl;
	}

	void BuildLeg()
	{
		cout << "BuilderB buildLeg " << endl;
	}
};

class BuilderB : public Builder 
{
public:
	void BuildHead()
	{
		cout << "BuilderB buildhead " << endl;
	}

	void BuildLeg()
	{
		cout << "BuilderB buildLeg " << endl;
	}
};

class BuilderPerson
{
public:
	BuilderPerson(Builder* b): bp(b) {}

	void CreateBuilder()
	{
		bp->BuildHead();
		bp->BuildLeg();
	}

private:
	Builder* bp;
};

void test3()
{
	BuilderA* ba = new BuilderA();
	BuilderPerson bp1(ba);
	bp1.CreateBuilder();

	delete ba;
}

#endif

#ifdef TEST_STATE

#include <iostream>
#include <memory>  // for std::unique_ptr

// State 接口
class State
{
public:
	virtual void StateFunc() {};
};

class Context;
class State1;
class State2;

class State1 : public State
{
public:
	void StateFunc()
	{
		cout << "State1 \n";
	}
};

class State2 : public State
{
public:
	void StateFunc()
	{
		cout << "State2 \n";
	}

};

class Context
{
public:

	Context(State* sa) :sta(sa)
	{
	}

	void SetState(State* s)
	{
		delete sta;
		sta = s;
	}

	void req()
	{
		sta->StateFunc();
	}

	~Context()
	{
		delete sta;
	}

private:
	State* sta;
};

void test4()
{
	Context c1(new State1());
	Context c2(new State2());

	c1.req();
	c2.req();
}

#endif

#ifdef TEST_ADAPTER

class Target 
{
public:
	virtual void Func() {};

};

class Adaptee
{
public:
	void funcAda() {};
};

class Adapter : public Target
{
public:
	void Func()
	{
		adaee.funcAda();
	}

private:
	Adaptee adaee;
};

void test5()
{
	Target* t = new Adapter();
	t->Func();
}

#endif

#ifdef TEST_MEMENTO

class MEMENTO
{
public:
	MEMENTO(const string& s) :sText(s) {}

	string getText() const
	{
		return sText;
	}

private:
	string sText;
};

class ORIGINATOR 
{
public:

	MEMENTO createMemento()
	{
		return MEMENTO(_text);
	}

	void restoreMemento(const MEMENTO& memento)
	{
		_text = memento.getText();
	}


private:
	string _text;
};

class CARETAKER 
{
public:
	void addMemento(const MEMENTO& memento)
	{
		vtmemen.push_back(memento);
	}

	MEMENTO getMemento(int index)
	{
		if (0 < index < vtmemen.size()) 
		{
			return vtmemen[index];
		}
		throw std::out_of_range("Invalid index");
	}

private:
	std::vector<MEMENTO> vtmemen;
};

void test6()
{
		
}

#endif

void main()
{
	test6();
}











