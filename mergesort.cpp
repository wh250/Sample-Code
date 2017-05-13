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

//mergesort algorithm
vector<int> Mergesort(vector<int> data, int first, int last, double& count) {
	//if the size of the array is 1 return itself
	if (first == last) {
		vector<int> a(1);
		a[0] = data[first];
		return a;
	}
	//cut itself in half and mergesort both
	vector<int> a=Mergesort(data,first, (last + first) / 2, count);
	vector<int> b=Mergesort(data, 1 + (last + first) / 2, last, count);
	int i = 0;
	int j = 0;
	int z = 0;
	vector<int> c(last - first + 1);
	//takes sorted half arrays and sorts them into self
	while (z<c.size()) {
		//if 1st half is empty or leftmost value of 2nd side is less than that of the 1st
		if (i == a.size() or (b[j]<a[i] and j!=b.size())) {
			//put value from 2nd side in
			c[z] = b[j];
			j++;
		}
		else {
			//put value from 1st side in
			c[z] = a[i];
			i++;
		}
		z++;
		//increment counter each time you put a value into the array
		count++;
	}
	//returns sorted array
	return c;
}

//shell of a function just to call mergesort with less trouble
vector<int> mergesort(vector<int> data, double& count) {
	return Mergesort(data,0,data.size()-1, count);
}

//in place mergesort
double ipmerge(vector<int>& data){
	double count=0;
	int a;
	int b;
	int c;
	int temp;
	//doubling size of group to sort each time
	for(int i=2; i<=data.size();i*=2){
		//for every 2 groups
		for(int j=0;j<data.size();j+=i){
			a=j;
			b=j+i/2;
			c=j;
			//combine 2 sorted groups into 1 group
			while(c<j+i){
				//if 2nd half is less or 1st half is empty
				if(a==b or (data[b]<data[a] and b!=j+i)){
					//shift things over and put 2nd half in front
					temp=data[b];
					for(int k=b-1; k>=a;k--){
						data[k+1]=data[k];
						//increment values when you need to shift stuff
						count++;
					}
					data[c]=temp;
					a++;
					b++;
					c++;
				}
				//if 1st half is less or 2nd half is empty
				else{
					//1st half is already in front so just move counters over
					a++;
					c++;
				}
				//and each time you put it in
				count++;
			}
		}
	}
	return count;
}

//takes 1 argument to choose text file
int main(int argc, char** argv) {
	if (argc != 2) {
		return -1;
	}
	//reads in data from "dus-n.txt"
	//n=2^input and is number of elements in file
	int n = stoi(argv[1]);
	n = bexp(n);
	int a = 0;
	string file;
	file = "dus-";
	file += argv[1];
	file += ".txt";
	//copy data in 2 times, one for each sort
	vector<int> data(n);
	vector<int> data1(n);
	string line;
	ifstream myfile(file);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			data[a] = stoi(line);
			data1[a]=stoi(line);
			a++;
		}
		myfile.close();
	}
	double count=0;
	//cout steps taken for recursive mergecount and in place mergecount
	mergesort(data,count);
	cout<<"Mergesort count"<<endl;
	cout<<count<<endl;
	cout<<"In place mergesort count"<<endl;
	cout<<ipmerge(data1)<<endl;
	return 0;
}
