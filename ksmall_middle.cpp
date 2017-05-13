#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//find k'th smallest element in array
//this one chooses pivot from middle of array, outputs pivot
//k is k'th smallest, data is the datset, first is the beginning, last is the end
int kSmall(int k, vector<int> data, int first, int last){
	int p=first+last;
	p++;
	p/=2;
	int i=first+1;
	int j=last;
	int temp=0;
	temp=data[first];
	data[first]=data[p];
	data[p]=temp;
	p=data[first];
	cout<<p<<endl;
	//partition according to pivot
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
	//check for left, right or if it is the kth smallest return it
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

//find kth smallest, output it
int main(int argc, char** argv){
	int n=0;
	queue<int> temp;
	string line;
	//read from "hw2-data.txt" into array
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
	//find the 42th smallest element
	//512 elements in array, so 511 is end
	cout<<kSmall(42,data,0,511)<<endl;
	return 0;
}
