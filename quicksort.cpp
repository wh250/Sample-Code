#include<vector>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//returns 2^n
int bexp(int n) {
	int a = 1;
	for (int i = 0; i<n; i++) {
		a *= 2;
	}
	return a;
}

//recursive quicksort
double Quicksort(vector<int>& data, int first, int last) {
	double count = 0;
	int temp = 0;
	//if the size of array is 1 stop
	if(last<=first){
		return count;
	}
	//takes median of first, middle, last value and uses it as pivot
	if (data[last] > data[(first + last) / 2] && data[last]<data[first] || data[last]<data[(first + last) / 2] && data[last] > data[first]) {
		temp = data[first];
		data[first] = data[last];
		data[last] = temp;
	}
	else if (data[(last + first) / 2]>data[first] && data[(last + first) / 2]<data[last] || data[(last + first) / 2]<data[first] && data[(last + first) / 2]) {
		temp = data[first];
		data[first] = data[(last+first)/2];
		data[(last + first) / 2] = temp;
	}
	int p = data[first];
	int i = first+1;
	int j = last;
	//quicksort algorithm
	while (i<j) {
		if (data[i]<=p) {
			i++;
		}
		if (data[j]>p) {
			j--;
		}
		if (i <= j) {
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
			//count goes up each time values are swapped
			count++;
		}
	}
	j=first;
		while(data[j+1]<=data[first] and j!=last){
			j++;
		}
	data[first] = data[j];
	data[j] = p;
	//pivot is in data[j], recursively quicksort the remaining halves
	count+=Quicksort(data, first, j-1);
	count+=Quicksort(data, j+1, last);
	//return number of steps taken
	return count
}

//function to call quicksort without needing to put 0, data.size()-1
double qsort(vector<int>& data) {
	return Quicksort(data, 0, data.size()-1);
}

//takes 1 argument to choose text file
int main(int argc, char** argv) {
	if (argc != 2) {
		return -1;
	}
	int n = stoi(argv[1]);
	//reads from file with name "dus-n.txt"
	//n=2^input and is number of elements in file
	n = bexp(n);
	int a = 0;
	string file;
	file = "dus-";
	file += argv[1];
	file += ".txt";
	//read in data
	vector<int> data(n);
	string line;
	ifstream myfile(file);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			data[a] = stoi(line);
			a++;
		}
		myfile.close();
	}
	//outputs number of steps taken to sort it once and again
	cout << "Quicksort count:"<<endl;
	cout << qsort(data)<<endl;
	//2nd time takes nlogn because it chooses median rather than n^2 from a sorted array
	cout << "Quicksort of sorted count:"<<endl;
	cout << qsort(data)<<endl;
	return 0;
}
