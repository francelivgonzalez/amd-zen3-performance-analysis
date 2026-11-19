#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

using namespace std;

int main(){
	
	const size_t N = 1000000;

	vector<int> data(N);

	iota(data.begin(), data.end(), 1);
	
	long long sum = 0;

	auto start = chrono::steady_clock::now();

	for (size_t i=0; i < N; i++){
		sum += data[i];
}
	auto end = chrono::steady_clock::now();
		
	auto elapsed = chrono::duration_cast<>(end - start).count();

	cout << "Sequential access\n";
	cout << "Time (ns):" << elapsed << "/n";
	cout << "Sum: " << sum \n";

	
	return 0; 
}