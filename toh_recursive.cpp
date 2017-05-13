#include<vector>
#include<stack>
#include<iostream>
#include<string>
using namespace std;

//solve tower of hanoi recursively
//3 stacks representing each stick
void stohr(int count, stack<int> &source, stack<int> &destiny, stack<int> &spare, int& mvcnt){
	if(count==1){
		//increment moves
		mvcnt++;
		destiny.push(source.top());
		source.pop();
	}
	else{
		stohr(count-1,source,spare,destiny,mvcnt);
		stohr(1,source,destiny,spare,mvcnt);
		stohr(count-1,spare,destiny,source,mvcnt);
	}
}

int main(int argc, char** argv){
	if(argc != 2){
		cout<<"error: not correct # of arguments"<<endl;
		return -1;
	}
	string s=argv[1];
	int n = stoi(s);
	if(n<1){
		cout<<"error: argument must be 1 or greater"<<endl;
		return -2;
	}
	stack<int> source;
	stack<int> destination;
	stack<int> spare;
	int movecount=0;
	//push disks onto first stick
	while (n>0){
		source.push(n);
		n--;
	}
	//move them to destination stick
	stohr(stoi(s),source,destination,spare,movecount);
	//output how many moves it took
	cout<<movecount<<endl;
	movecount=0;
	return 0;
}
