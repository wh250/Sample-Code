#include<vector>
#include<stack>
#include<iostream>
#include<string>
using namespace std;

//solve tower of hanoi iteratively
void stohi(int count, stack<int> &source, stack<int> &destiny, stack<int> &spare, int& mvcnt){
	bool t=true;
	int case1 =0;
	int case2 =0;
	int a;
	while(!(destiny.size()==count or spare.size()==count)){
		//every other move different check must be done
		if(t){
			//check what case to use
			if(!source.empty()){
				if(source.top()==1){
					case1=1;
				}
			}
			if(!destiny.empty()){
				if(destiny.top()==1){
					case1=2;
				}
			}
			if(!spare.empty()){
				if(spare.top()==1){
					case1=3;
				}
			}
			//do corresponding case
			if(case1==1){
				destiny.push(source.top());
				source.pop();
				mvcnt++;
			}
			if(case1==2){
				spare.push(destiny.top());
				destiny.pop();
				mvcnt++;
			}
			if(case1==3){
				source.push(spare.top());
				spare.pop();
				mvcnt++;
			}
			t=false;
		}
		else{
			//check what kind of move must be done
			t=true;
			//increment count
			a=count+1;
			if(!source.empty()){
				if(source.top()!=1){
					a=source.top();
				}
			}
			else {
				a=count+1;
			}
			if(!destiny.empty()){
				if(destiny.top()!=1 and destiny.top()<a){
					a=destiny.top();
				}
			}
			if(!spare.empty()){
				if(spare.top()!=1 and spare.top()<a){
					a=spare.top();
				}
			}

			if(!source.empty()){
				if(source.top()==a){
					case2=1;
				}
			}
			if(!destiny.empty()){
				if(destiny.top()==a){
					case2=2;
				}
			}
			if(!spare.empty()){
				if(spare.top()==a){
					case2=3;
				}
			}
			//do corresponding move
			if(case2==1){
				if(destiny.empty()){
					destiny.push(source.top());					
				}
				else{
					if (destiny.top()>a){
						destiny.push(source.top());
					}
					else {
						spare.push(source.top());
					}
				}
				source.pop();
				mvcnt++;
			}
			if(case2==2){
				if(spare.empty()){
					spare.push(destiny.top());					
				}
				else{
					if (spare.top()>a){
						spare.push(destiny.top());
					}
					else {
						source.push(destiny.top());
					}
				}
				destiny.pop();
				mvcnt++;
			}
			if(case2==3){
				if(source.empty()){
					source.push(spare.top());					
				}
				else{
					if (source.top()>a){
						source.push(spare.top());
					}
					else {
						destiny.push(spare.top());
					}
				}
				spare.pop();
				mvcnt++;
			}
		}
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
	//3 sticks
	stack<int> source;
	stack<int> destination;
	stack<int> spare;
	int movecount=0;
	// move disks onto source
	while (n>0){
		source.push(n);
		n--;
	}
	//do tower of hanoi algorithm
	if(stoi(s)%2==1){
		stohi(stoi(s),source,destination,spare,movecount);
	}
	else {
		stohi(stoi(s),source,spare,destination,movecount);
	}
	//output how many moves it took
	cout<<movecount<<endl;
	movecount=0;
	return 0;
}
