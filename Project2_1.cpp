#include <vector>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <iostream>
#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

//signal handler for SIGINT
//just output how it was interrupted and kill self
void c(int signum) {
	cout << getpid() << " is dead by control c" << endl;
	kill(getpid(), SIGKILL);
}

//signal handler for SIGQUIT
//output how it was interrupted
//kill self and all children
void d(int signum) {
	cout << getpid() << " kills all children and dies by control \" << endl;
	kill(0, SIGKILL);
}

//signal for children to say their pid and ppid
void f(int signum) {
	cout << "Hi I'm process " << getpid() << " and my parent is " << getppid() << endl;
}

//function to find min, max, and sum using process forking, with a different thread for each
//children spawn children for this purpose
//not allowed to use pipes or shared memory
// all children are to say their pid and parent id by means of signal
//if process takes longer than 3 seconds parent determines it should be killed and other children kill it
int main(int argc, char** argv) {
	int ppid = getpid();
	signal(SIGINT, c);
	signal(SIGQUIT, d);
	//initialize min, max, sum
	//kill is pid of process to be killed
	//-1 is no one
	int kil=-1;
	int max;
	int min;
	int sum=0;
	int n = 0;
	int pstatus;
	//read data from data.txt into array
	vector<int> array;
	string line;
	string file = "data.txt";
	ifstream myfile(file);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			array.push_back(stoi(line));
		}
		myfile.close();
	}
	max = array[0];
	min = array[0];
	signal(SIGUSR1, f);
	//first fork, to find max
	int pid = fork();
	//child process which spawns its own
	if (pid == 0) {
		//children do this for the length of the array
		while (pid == 0 && n < array.size()-1) {
			//make their own child
			pid = fork();
			n++;
			if (pid != 0) {
				//if a process is defined as slow kill it
				if (kil > 0) {
					kill(kil, SIGKILL);
					pid=-1;
				}
				//make child say number
				kill(pid, SIGUSR1);
				//wait for child's exit
				wait(&pstatus);
				if (WIFEXITED(pstatus)) {
					max = WEXITSTATUS(pstatus);
				}
			}
			else {
				//check max vs corresponding value in array
				if (max < array[n]) {
					max = array[n];
				}
			}
		}
		//return max
		exit(max);
	}
	else {
		//parent process
		//make child say its number
		kill(pid, SIGUSR1);
		//wait 3 seconds
		sleep(3);
		//if child is not done set it to be killed
		if (waitpid(pid,&pstatus, WNOHANG) < 0) {
			kil = pid;
		}
		else {
			//otherwise set max = to return value
			if (WIFEXITED(pstatus)) {
				max = WEXITSTATUS(pstatus);
			}
		}
		//reset values, repeat for minimum
		int pid = fork();
		n=0;
		kil=-1;
		//same thing
		if (pid == 0) {
			while (pid == 0 && n < array.size()-1) {
				pid = fork();
				n++;
				if (pid != 0) {
					if (kil > 0) {
						kill(kil, SIGKILL);
						pid=-1;
					}
					kill(pid, SIGUSR1);
					wait(&pstatus);
					if (WIFEXITED(pstatus)) {
						min = WEXITSTATUS(pstatus);
					}
				}
				else {
					if (min > array[n]) {
						min = array[n];
					}
				}
			}
			exit(min);
		}	
		else {
			//same thing, wait 3 seconds, etc
			kill(pid, SIGUSR1);
			sleep(3);
			if (waitpid(pid,&pstatus, WNOHANG) < 0) {
				kil = pid;
			}
			else {
				if (WIFEXITED(pstatus)) {
					min = WEXITSTATUS(pstatus);
				}
			}
			//reset values and repeat for sum
			n=0;
			kil=-1;
			int pid = fork();
			if (pid == 0) {
			while (pid == 0 && n < array.size()) {
				pid = fork();
				if (pid != 0) {
					if (kil > 0) {
						kill(kil, SIGKILL);
						pid=-1;
					}
					kill(pid, SIGUSR1);
					wait(&pstatus);
					if (WIFEXITED(pstatus)) {
						sum += WEXITSTATUS(pstatus);
					}
					n++;
				}
				else {
					sum=array[n];
					n++;
				}
			}
			exit(sum);
		}
		else {
			kill(pid, SIGUSR1);
			sleep(3);
			if (waitpid(pid,&pstatus, WNOHANG) < 0) {
				kil = pid;
			}
			else {
				if (WIFEXITED(pstatus)) {
					sum += WEXITSTATUS(pstatus);
				}
			}
			//finally, output max, min, and sum
			cout << "max: " << max << endl;
			cout << "min: " << min << endl;
			cout << "sum: " << sum << endl;
			return 0;
			}
		}
	}
}
