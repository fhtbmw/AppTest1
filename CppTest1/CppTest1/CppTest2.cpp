#include <iostream>
#include <vector>
#include <tuple>
#include <complex>
#include <string>
using namespace std;

void show() {

}

template<typename T,typename ... Types>
void show(const T&f,const Types&... args)
{
	std::cout << "First: " << f << endl;
	//std::cout << "args: " << args ... << endl;
	show(args ...);
}

/*

lambda 

*/
void Test1() {
	//string s{"dsadasd"};
	//double x{3.4};
	//show(4334,"dsassdsadsadsa",std::vector<char>('dsa',4));
	auto e=[] {
		cout << "lambda" << endl;
	};
	e();

	[] {
		cout<<"lambda2" << endl;
	}();

	auto l = [](const string& s)
		{
			string s1 =s;
			cout << s1;
		};
	l("dead");

	int m_x = 0;
	auto l2 = [&m_x]
		{
			m_x++;
		};
	
}


/*

pair、tuple

*/
typedef std::pair<int, float> m_pair;
m_pair p(1,3.3);

void TestTuple()
{
	tuple<char, int, string>t1('c',344,"dsd");

	tuple<int, double>t2{ make_tuple(22,33.4) };
	int i = 3;
	double j = 3.4;
	make_tuple(ref(i), ref(j))=t2;	//t2中的值赋给i j
	tie(i,j)=t2;

	cout<<get<1>(t1)<<endl;

}

/*

线程 

*/

//生产消费模型

#include <iostream>
#include <stack>
#include <thread>
#include <condition_variable>
#include <mutex>
#include<boost/bind.hpp>

using namespace std;

mutex io_mu;
mutex mu;

class Buffer {
public:
    Buffer(size_t n) : current_size(0), capacity(n) {}

    void put(int x);
    void get(int* x);

private:
    stack<int> b_s;
    int current_size, capacity;
    condition_variable con_put;
    condition_variable con_get;

    bool isEmpty() {
        return current_size == 0;
    }

    bool isFull() {
        return current_size == capacity;
    }
};

void Buffer::put(int x) {
    unique_lock<mutex> lock(mu);
    while (isFull()) {
        {
            lock_guard<mutex> io_lock(io_mu);
            cout << "Produce Thread: " << this_thread::get_id() << " waiting" << endl;
        }
        con_put.wait(lock);
    }
    b_s.push(x);
    ++current_size;
    lock.unlock();
    con_get.notify_one(); // Notify a consumer
}

void Buffer::get(int* x) {
    unique_lock<mutex> lock(mu);
    while (isEmpty()) {
        {
            lock_guard<mutex> io_lock(io_mu);
            cout << "Consume Thread: " << this_thread::get_id() << " waiting" << endl;
        }
        con_get.wait(lock);
    }
    *x = b_s.top();
    b_s.pop();
    --current_size;
    lock.unlock();
    con_put.notify_one(); // Notify a producer
}

Buffer Bu(5);

void producer(int n) {
    for (int i = 0; i < n; i++) {
        {
            lock_guard<mutex> io_lock(io_mu);
            cout << "put " << i << endl;
        }
        Bu.put(i);
    }
}

void consumer(int n) {
    int x;
    for (int i = 0; i < n; i++) {
        Bu.get(&x);
        {
            lock_guard<mutex> io_lock(io_mu);
            cout << "get: " << x << endl;
        }
    }
}

void TestCondition() {
    thread t1(bind(producer, 20));
    thread t2(bind(consumer, 10));
    thread t3(bind(consumer, 10));
    t1.join();
    t2.join();
    t3.join();
}

int main(){
	TestCondition();
	return 0;
}
