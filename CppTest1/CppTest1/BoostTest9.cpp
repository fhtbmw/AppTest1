#include<boost/random.hpp>
#include<iostream>
#include<ctime>
#include<boost/io/ios_state.hpp>
#include<boost/filesystem.hpp>

using namespace std;
using namespace boost;


void TestRandom() {
	mt19937 rng(time(0));
	for (int i = 0; i < 100;++i) {
		cout<<rng()<<endl;
	}
}

void TestFileSystem() {
	boost::filesystem::path p = "dsadada";
	if (exists(p)) {
		remove_all(p);
	}


}


int main() {
	TestFileSystem();
	return 0;
}