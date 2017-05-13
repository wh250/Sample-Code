#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//find k'th smallest in array using first number as pivot
//outputs pivot
int kSmall(int k, vector<int> data, int first, int last){
	int p=data[first];
	cout<<p<<endl;
	int i=first+1;
	int j=last;
	int temp=0;
	//partition
	while(i<j){
		if(data[i]<p){
			i++;
		}
		if(data[j]>p){
			j--;
		}
		if(i<=j){
			temp=data[i];
			data[i]=data[j];
			data[j]=temp;
		}
	}
	data[first]=data[j];
	data[j]=p;
	//recursively call for greater, less or returns if equal
	if(k<j+1-first){
		return kSmall(k,data,first,j-1);
	}
	else if (k==j+1-first){
		return data[j];
	}
	else{
		return kSmall(k-j-1+first,data,j+1,last);
	}
}

int main(int argc, char** argv){
	int n=0;
	queue<int> temp;
	string line;
	//read from dataset
	ifstream myfile ("hw2-data.txt");
	if(myfile.is_open()){
		while(getline(myfile,line)){
			n++;
			temp.push(stoi(line));
		}
		myfile.close();
	}
	vector<int> data(n);
	n=0;
	while(!temp.empty()){
		data[n]=temp.front();
		temp.pop();
		n++;
	}
	//output 42th smallest in array
	cout<<kSmall(42,data,0,511)<<endl;
	return 0;
}
