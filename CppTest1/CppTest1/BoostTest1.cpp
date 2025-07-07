// CppTest1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<boost/smart_ptr.hpp>
#include <iostream>

struct file {
    file(const char* file_name) {
        std::cout << "Get file"<<file_name << std::endl;
    }

    ~file() {
        std::cout << "Close file"<< std::endl;
    }
};

void test1()
{
    boost::scoped_ptr<int> sp(new int(10));
    sp.reset();
    assert(sp == 0);
    if (sp == NULL) {
        std::cout << "sp==NULL" << std::endl;
    }
    boost::scoped_ptr<file>sp2(new file("/inc/t1.ini"));
}

void test2() {
    int arr1[10] = { 0 };
    int* arr = new int[10];
    boost::scoped_array<int>sa(arr);
    for (int i = 0; i < 10;i++) {
        sa[i] = i;
        std::cout<<sa[i]<<std::endl;
    } 
   
}

class share {
public:
    share(boost::shared_ptr<int>p):_p(p) {}

    void print() {
        std::cout<<_p.use_count()<<std::endl;
        std::cout << *_p << std::endl;
    }

    void print_f() {
        std::cout << "use_count: " << _p.use_count() << std::endl;
        std::cout << *_p << std::endl;
    }

public:
    boost::shared_ptr<int>_p;
};

void TestSharedPtr() {
    boost::shared_ptr<int>sp1(new int(1));
    boost::shared_ptr<int>sp2 = sp1;
    assert(sp2!=NULL&&sp2.use_count() == 2);
    share s1(sp1);
    *sp1 = 2;
    std::cout <<s1._p .use_count() << std::endl;
    s1.print_f();
}

//int main(){
//    ///int* p = new int (10);
//    //test2();
//    TestSharedPtr();
//   // system("pause");
//    
//    return 0;
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
