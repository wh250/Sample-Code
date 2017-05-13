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

queue<int> rrsh;
queue<int> rrs;
int procid=-1;
bool high;
bool brake;
string inputk="";

void seth(int signum){
	if(inputk=="h"){
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
	else{
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

void setl(int signum){
	if(inputk=="l"){
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
	else{
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

void* readl(void* arg){
	while(1){
		cin>>inputk;
		brake=true;
	}
}

int main(int argc, char** argv){
	string temp;
	time_t start, end;
	int status;
	char* args[]={(char*)"blank"};
	pthread_t tipt;
	int pid;
	int *a=new int;
	int tq=10;
	signal(SIGUSR1,seth);
	signal(SIGUSR2,setl);
	pthread_create(&tipt,NULL,&readl,(void*) a);
	
	for(int i=1;i<argc;i++){
		temp=argv[i];
		temp="./"+temp;
		pid=fork();
		args[0]=(char*)temp.c_str();
		if(pid==0){
			execvp(temp.c_str(),args);
			cout<<"failed"<<endl;
		}
		else{
			rrs.push(pid);
		}
	}
	
	for(int i=0;i<rrs.size();i++){
		kill(rrs.front(),SIGSTOP);
		rrs.push(rrs.front());
		rrs.pop();
	}
	
	while(rrs.size()>0 || rrsh.size()>0){
		if(inputk!=""){
			if(inputk=="h"){
				kill(getpid(),SIGUSR1);
			}
			if(inputk=="l"){
				kill(getpid(),SIGUSR2);
			}
			if(inputk.length()>=3){
				if(inputk[0]=='h' && inputk[1]==' '){
					kill(getpid(),SIGUSR1);
				}
				if(inputk[0]=='l' && inputk[1]==' '){
					kill(getpid(),SIGUSR2);
				}
			}
			inputk="";
		}
		high=!rrsh.empty();
		brake=false;
		
		if(!rrsh.empty()){
			for(int i=0;i<rrsh.size();i++){
				kill(rrsh.front(),SIGSTOP);
				rrsh.push(rrsh.front());
				rrsh.pop();
			}
		}
		
		if(!rrs.empty()){
			for(int i=0;i<rrs.size();i++){
				kill(rrs.front(),SIGSTOP);
				rrs.push(rrs.front());
				rrs.pop();
			}
		}
		
		if(high){
			kill(rrsh.front(),SIGCONT);
			start=time(NULL);
			end=time(NULL);
			while(difftime(end,start)<tq && !brake){
				end=time(NULL);
				if(waitpid(rrsh.front(),&status,WNOHANG)>0){
					rrsh.pop();
					kill(rrsh.front(),SIGCONT);
					brake=true;
				}
			}
			if(!brake){
				kill(rrsh.front(),SIGSTOP);
				rrsh.push(rrsh.front());
				rrsh.pop();
				kill(rrsh.front(),SIGCONT);
			}
		}
		
		else{
			kill(rrs.front(),SIGCONT);
			start=time(NULL);
			end=time(NULL);
			while(difftime(end,start)<tq && !brake){
				end=time(NULL);
				if(waitpid(rrs.front(),&status,WNOHANG)>0){
					rrs.pop();
					kill(rrs.front(),SIGCONT);
					brake=true;
				}
			}
			if(!brake){
				kill(rrs.front(),SIGSTOP);
				rrs.push(rrs.front());
				rrs.pop();
				kill(rrs.front(),SIGCONT);
			}
		}
	}
	pthread_kill(tipt,SIGKILL);
	return 0;
}
