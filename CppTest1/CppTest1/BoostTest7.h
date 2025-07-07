#pragma once
#include<boost/lexical_cast.hpp>
#include<boost/format.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/array.hpp>
#include<boost/assign//list_of.hpp>
#include <boost/typeof/typeof.hpp>
#include<boost/dynamic_bitset.hpp>
#include<boost/unordered_set.hpp>
#include<boost/unordered_map.hpp>
#include<boost/random.hpp>
#include<boost/timer.hpp>
#include<boost/typeof/typeof.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include<boost/functional/hash.hpp>
#include<boost/bimap.hpp>
#include<boost/bimap/multiset_of.hpp>
#include<boost/bimap/vector_of.hpp>
#include<unordered_map>
#include<boost/circular_buffer.hpp>
#include<boost/tuple/tuple.hpp>
#include<boost/tuple/tuple_comparison.hpp>
#include<boost/tuple/tuple_io.hpp>
#include<boost/any.hpp>
#include<boost/exception/diagnostic_information.hpp>
#include<boost/variant.hpp>
#include<boost/multi_array.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>

#define TEMP1 1

using namespace std;
using namespace boost;
using namespace assign;

typedef struct  _Struct
{
	int _data1;
}Struct;

template<typename _Date,int a=1>
class my 
{
	int a;

};

namespace test7 {
	void TestFormat();
	void TestString();
	void TestSet();
	void TestBimap();
	void TestCircularBuf();
	void TestTuple();
	void TestAny();
	void TestVariant();
	void TestMultiArray();
	void TestProperty();
}