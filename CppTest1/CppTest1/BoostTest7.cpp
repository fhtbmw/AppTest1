#include"BoostTest7.h"
#include "pch.h"

namespace test7 {

	/*

	字符串数值快速转换

	*/

	void TestLexcical_cast() {
		int x = lexical_cast<int>("12121");
		long y = lexical_cast<long>("23123");
		float z = lexical_cast<float>("2.322312313213213213");

	}

	//转换合法性验证
	template<typename T>
	bool num_valid(const char* str) {
		try {
			lexical_cast<T>(str);
			return true;
		}
		catch (bad_lexical_cast&) {
			return false;
		}
	}

	/*

	格式输出

	*/
	void TestFormat() {
		cout << format("%s: %d + %d = %d\n") % "sum" % 1 % 2 % (1 + 2) << endl;
		format fmt("(%1% + %2% ) * %4% = %3% \n");
		fmt % "dsad" % "s" % "dsads" % 1;
		//	fmt % ((2+5)*5);

			//cout<<fmt.str()<<endl;
		format fmt2("%05d \n %04x \n %10s\n");
		//fmt % 23 % 2.232  %"dad";
		cout << fmt2 % 23 % 2.32 % "dad" << endl;
	}

	/*

	字符串函数

	*/

	void TestString() {
		string s = "dsaada.txt";
		//assert(is_lower(s)&&ends_with(s,"txt"));
	}

	/*

	boost中array使用

	*/
	void TestArray() {
		boost::array<int, 5>a{ 0 };
		a.back() = 5;
		for (BOOST_AUTO(it, a.begin()); it != a.end(); it++) {
			cout << *it << endl;
		}
		int* p = a.c_array();
		*(p + 3) = 4;
		assert(a.at(3) = 4);
		sort(a.begin(), a.end());
	}

	template<typename T, size_t N>
	class MyArray {
	public:
		int GetCapacity() {
			return m_arr.size();
		}

		T& operator[](size_t idx) {
			try {
				if (m_arr.empty()) {
					//throw std::out_of_range("array is empty");
				}
				else if (idx >= N) {
					//	throw std::out_of_range("Index invalid");
				}
				else {
					return m_arr[idx];
				}
			}
			catch (std::out_of_range&) {

			}
		}
	private:
		boost::array<T, N> m_arr;
	};

	void TestArray2() {
		boost::array<int, 34>a(list_of(432)(44)(6));
		MyArray<int, 3>a1;
		a1[5] = 4;
		cout << a1.GetCapacity();

	}

	/*

		处理二进制数字工具

	*/

	void TestBinary() {
		dynamic_bitset<>db1(string("0101"));
		dynamic_bitset<>db2(8, BOOST_BINARY(0101));
		cout << db2;

	}

	/*

		unoredred_set	unordered_map

	*/
	template<typename T>
	void hash_fun() {
		using namespace boost::assign;

		/*T s = list_of(1)(2)(3)(4)(5);
		for (typename T::iterator it = s.begin();it!=s.end();it++) {
			cout << *it<<" " << endl;
		}
		s.insert(6);*/
		//cout << s.size() << endl;

		std::unordered_map<T, string>um1;
		um1 = map_list_of(0, "0")(1, "1")(2, "2");
		um1[32] = "dsa";

		BOOST_AUTO(it, um1.begin());
	}

	template<typename T>
	void Test_fill(T& n) {
		variate_generator<mt19937, uniform_int<>>
			gen((mt19937), uniform_int<>(0, 100));

		for (int i = 0; i < 10000; i++) {
			n.insert(gen());
		}
	}

	void TestBucket() {
		unordered_set<int>us1 = list_of(2)(3)(4);
		size_t bucket_count = us1.bucket_count();
		for (int i = 0; i < bucket_count; ++i) {
			cout << us1.bucket_size(i) << endl;
		}
	}

	/*
	
	bimap:元素双向映射关系集合,左右视图操作同一map

	*/
	template<typename T>
	void PrintMap(T& m)
	{
		for (BOOST_AUTO(it, m.begin()); it != m.end(); it++)
		{
			cout << it->first << endl;
		}
	}

	template<typename K,typename V>
	void PrintMap(const boost::bimap<K,V>& bm) {
		for (auto it:bm.left) {
			cout << it.first << endl;
		}
	}

	template<typename T>
	void PrintMap(const boost::circular_buffer<T>cb) {
		for (BOOST_AUTO(it, cb.begin()); it != cb.end();it++) {
			cout << *it << endl;
		}
	}


	void TestBimap() {
		bimap<int, string>bm;
		bm.left.insert(make_pair(1,"first"));
		bm.left.insert(make_pair(2,"second"));
		bm.left.insert(make_pair(3,"third"));
		
		bm.right.insert(make_pair("four", 4));
		bm.right.insert(make_pair("5",5));
		bm.right.insert(make_pair("six",6));
	
		/*for (BOOST_AUTO(it, bm.begin()); it != bm.end();it++) {
			cout<<it->get_left_pair().get_right()<<endl;
				
		}	
		for (BOOST_AUTO(it, bm.left.begin());it!=bm.left.end();it++) {
			cout<<it->second<<endl;
		}*/
		bimap<boost::bimaps::set_of<string>, boost::bimaps::vector_of<int>> bm2;
		bm2.left.insert(make_pair("aaa",1));
		bm2.left.insert(make_pair("bbb",2));
		bm2.left.insert(make_pair("ccc",3));
		bm2.left.insert(make_pair("aaa",4));
		
		cout << bm2.size()<<endl;
		
		//PrintMap(bm2.left);
	}
	
	/*
	
	环形缓冲区
	
	*/

	void TestCircularBuf() {
		/*circular_buffer<float>cb1;
		BOOST_ASSERT(cb1.capacity()==cb1.size());
		cb1=list_of(1)(3)(0);
		
		cout<<cb1.size()<<endl;
		PrintMap(cb1);

		cb1.push_back(2);
		PrintMap(cb1);*/
		circular_buffer<char>cb2(3);
		cb2.push_back('1');
		cb2.push_back('a');
		cout << cb2.size()<<endl;
		cout << *(cb2.end()-1)<<endl;
		cb2.resize(20,'c');
		//PrintMap(cb2);

	}

	/*
	
	tuple元素集合类,最多支持10种类型的元素
	
	*/

	void TestTuple() {
		typedef boost::tuple <int, string, char, long> m_tuple;
		m_tuple mt1(1, "dsdasdssaasda", 'c', 0x994234);
		m_tuple mt2(mt1);
		//cout<<mt1.get_head()<<endl;
		
		m_tuple mt3 = boost::tuples::make_tuple(1,"dsdas",'c',230);
		assert(mt1>mt3);		//typle基于 字典序 比较 
	
		std::tuple<int, char>m_tuple2(1,'d');
		//m_tuple2(1,'d');

		cout<<mt2<<endl;	//包含tuple.io hpp文件，从而正常输出tuple对象
	}

	/*
	
	any 容器，只能装载一个元素

	*/
	void TestAny() {
		try
		{
			any a(232);
			any_cast<char>(a);	//通过any_cast访问any的内部对象,若类型错误会导致异常
			any_cast<int>(a);
		}
		catch (boost::exception&)
		{
			cout << current_exception_diagnostic_information();
		}
	
	}

	/*
	
	variant，存储不容类型的植 

	*/
	void TestVariant() {
		variant<string, int, double>va;
		va = 2.2;
		try
		{
			get<double>(va);
		}
		catch (const boost::bad_get&e)
		{
			cout << e.what();
		}

		typedef variant<int, double, string>  var_v;
		var_v v=23;
		assert(v.type()==typeid(int));

	}

	//static_visitor 为不同类型提供访问器
	struct var_print :public static_visitor<>
	{
		template<typename T>
		void operator()(T& t)const
		{
			cout << t<< endl;
		}
	};

	void TestVariant2() 
	{
		variant<int, string, double>v;
		
		v = 233;
		apply_visitor(var_print(), v);
	}

	/*
	
	multi-array类型 多维数组

	*/

	void TestMultiArray()
	{
		multi_array<int, 3>ma(extents[2][3][4]);
		int v = 1;
		for (int i = 0; i < 2;i++) 
		{
			for (int j=0;j<3;j++)
			{	
				for (int k=0;k<4;k++)
				{
					ma[i][j][k] =v++ ;
				}
			}		
		}

	}

	/*
	
	property_tree 树形数据结构，处理xml、ini、json、info文件

	*/

	void TestProperty() {
		boost::property_tree::ptree pt;
		try {
			read_xml("test.xml", pt);
			cout << pt.get<string>("conf.urls.url") << endl;
		}
		catch (boost::property_tree::xml_parser_error &e) {
			cerr<<"read error"<<e.what() << endl;
		}
		catch (out_of_range &e) {
			cerr<<"Key error"<<e.what() << endl;
		}
	}

	void TestProperty2() {
		boost::property_tree::ptree pt;
		pt.put("conf.urls.url","https://www.ddd.com");
		pt.add("conf.urls.url","https://www.dad.com");
		

		write_xml("test.xml", pt);
	}


}

int main() {
	test7::TestProperty2();
}



