#include<iostream>
#include<queue>
#include<string>
#include<stdio.h>
#include<sys/types.h>
#include<csignal>
#include<sys/wait.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctime>
using namespace std;

//high priority queue
queue<int> rrsh;
//low priority queue
queue<int> rrs;
//if there are high priority processes
bool high;
//interrupts time quantum
bool brake;
//input string
string inputk="";

//set priority to high and interrupt time quantum
void seth(int signum){
	//if input is only h
	if(inputk=="h"){
		//if there are low priority processes, move currently running one from low priority queue to high priority queue
		if(!rrs.empty()){
			cout<<rrs.front()<<" changed to high priority"<<endl;
			rrsh.push(rrs.front());
			rrs.pop();
			for(int i=0;i<rrsh.size();i++){
				kill(rrsh.front(),SIGSTOP);
				rrsh.push(rrsh.front());
				rrsh.pop();
			}
			brake=true;
		}
		else{
			cout<<"No more low priority processes"<<endl;
		}
	}
	//if input is h + process id
	else{
		//find process with id, move it from low to high
		bool found=false;
		string y="";
		for(int i=0;inputk[i]!='\0';i++){
			y+=inputk[i];
		}
		int f=stoi(y);
		for(int i=0;i<rrs.size();i++){
			if(rrs.front()==f){
				cout<<f<<" changed to high priority"<<endl;
				rrsh.push(rrs.front());
				rrs.pop();
				found=true;
			}
			rrs.push(rrs.front());
			rrs.pop();
		}
		if(!found){
			cout<<"Process not found"<<endl;
		}
	}
}

//set process priority to low and interrupt time quantum
void setl(int signum){
	//if input is just l
	if(inputk=="l"){
		//if there are high priority processes, set the currently running one to low
		if(!rrsh.empty()){
			cout<<rrsh.front()<<" changed to low priority"<<endl;
			rrs.push(rrsh.front());
			rrsh.pop();
			for(int i=0;i<rrs.size();i++){
				kill(rrs.front(),SIGSTOP);
				rrs.push(rrs.front());
				rrs.pop();
			}
			brake=true;
		}
		else{
			cout<<"No more high priority processes"<<endl;
		}
	}
	//if input is l + process id
	else{
		//find process id, then set it to low
		bool found=false;
		string y="";
		for(int i=0;inputk[i]!='\0';i++){
			y+=inputk[i];
		}
		int f=stoi(y);
		for(int i=0;i<rrsh.size();i++){
			if(rrsh.front()==f){
				cout<<f<<" changed to low priority"<<endl;
				rrs.push(rrsh.front());
				rrsh.pop();
				found=true;
			}
			rrsh.push(rrsh.front());
			rrsh.pop();
		}
		if(!found){
			cout<<"Process not found"<<endl;
		}
	}
}

//thread to read input
//if input is received, interrupt time quantum
void* readl(void* arg){
	while(1){
		cin>>inputk;
		brake=true;
	}
}

//create processes
void create_process(int argc, char** argv) {
	string temp;
	char* args[] = { (char*)"blank" };
	int pid;
	//create all child processes
	for (int i = 1; i<argc; i++) {
		temp = argv[i];
		//add ./ to run it
		temp = "./" + temp;
		//fork and exec
		pid = fork();
		args[0] = (char*)temp.c_str();
		if (pid == 0) {
			execvp(temp.c_str(), args);
			cout << "failed" << endl;
		}
		else {
			//add process id to queue, default low
			rrs.push(pid);
		}
	}
}

void stopall(queue<int>& qq) {
	for (int i = 0; i < qq.size(); i++) {
		kill(qq.front(), SIGSTOP);
		qq.push(qq.front());
		qq.pop();
	}
}

void checkinput() {
	//if there is input
	if (inputk != "") {
		//check what input is and call corespondind function
		//input must be "h", "l", "h pid", or "l pid"
		if (inputk == "h") {
			kill(getpid(), SIGUSR1);
		}
		if (inputk == "l") {
			kill(getpid(), SIGUSR2);
		}
		if (inputk.length() >= 3) {
			if (inputk[0] == 'h' && inputk[1] == ' ') {
				kill(getpid(), SIGUSR1);
			}
			if (inputk[0] == 'l' && inputk[1] == ' ') {
				kill(getpid(), SIGUSR2);
			}
		}
		inputk = "";
	}
}

//run for 1 time quantum or until interrupted
void runtq(queue<int> qq) {
	//time measurement
	time_t start, end;
	//start first process in queue
	kill(qq.front(), SIGCONT);
	//set start time of current time quantum
	start = time(NULL);
	end = time(NULL);
	//run for 1 time quantum or until interrupted
	while (difftime(end, start)<tq && !brake) {
		end = time(NULL);
		//if process finished in time quantum
		if (waitpid(qq.front(), &status, WNOHANG)>0) {
			//remove process and set next process to run
			qq.pop();
			kill(qq.front(), SIGCONT);
			//interrupt
			brake = true;
		}
	}
	//if process didn't finish and time quantum ended
	//stop current process, move to back of queue, start next one
	if (!brake) {
		kill(qq.front(), SIGSTOP);
		qq.push(qq.front());
		qq.pop();
		kill(qq.front(), SIGCONT);
	}
}

//Round robin process scheduler
//Takes input in form of ./scheduler procname1 procname2 etc
//Has high and low priorities, settable from shell commands
//all processes are run as children for 1 time quantum. 
//High priority processes go first
int main(int argc, char** argv){
	int status;
	//thread to check input
	pthread_t tipt;
	int *a=new int;
	//time quantum is 10 seconds
	float tq=10;
	//signal to set low or high
	signal(SIGUSR1,seth);
	signal(SIGUSR2,setl);
	//make input thread
	pthread_create(&tipt,NULL,&readl,(void*) a);
	//create processes
	create_process(argc, argv);
	//Stop all child processes
	stopall(rrs);
	
	//if there are processes
	while(rrs.size()>0 || rrsh.size()>0){
		checkinput();
		//interrupt is false
		brake=false;
		//stop all processes in queue
		if(!rrsh.empty()){
			stopall(rrsh);
		}
		if(!rrs.empty()){
			stopall(rrs);
		}
		//if there are high priority processes
		if(!rrsh.empty()){
			runtq(rrsh);
		}
		//otherwise run low priority process queue
		else{
			runtq(rrs);
		}
	}
	//if all processes are finished
	//kill input thread
	pthread_kill(tipt,SIGKILL);
	//finish
	return 0;
}
