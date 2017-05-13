#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;


//function to, by menas of sieve, find and output primes
void *sieve(void *arg) {
	int* p = (int*)arg;
	int n = *p;
	bool A[n + 1];
	for (int i = 0; i <= n; i++) {
		A[i] = true;
	}
	for (int i = 2; i <= n; i++) {
		if (A[i]) {
			for (int j = i*i; j <= n; j += i) {
				A[j] = false;
			}
		}
	}
	for (int i = 2; i <= n; i++) {
		if (A[i]) {
			cout << i << endl;
		}
	}
}

//function that finds primes but only outputs those whose reverse are also primes
void *reverse(void *arg) {
	int* p = (int*)arg;
	int n = *p;
	int b = 0;
	int c = 0;
	bool A[n + 1];
	for (int i = 0; i <= n; i++) {
		A[i] = true;
	}
	for (int i = 2; i <= n; i++) {
		if (A[i]) {
			for (int j = i*i; j <= n; j += i) {
				A[j] = false;
			}
		}
	}
	for (int i = 2; i <= n; i++) {
		b = i;
		c = 0;
		if (A[i]) {
			while (b != 0) {
				c = c * 10 + (b % 10);
				b /= 10;
			}
			if (A[c]) {
				cout << i << endl;
			}
		}
	}
}

//makes 2 threads, 1 to find primes, other to find primes whose reverse are also primes
int main(int argc, char** argv) {
	pthread_t thread1, thread2;
	int* a = new int;
	*a = stoi(argv[1]);
	cout << "Primes" << endl;
	pthread_create(&thread1, NULL, &sieve, (void*)a);
	pthread_join(thread1, NULL);
	cout << "Reverse are Primes" << endl;
	pthread_create(&thread2, NULL, &reverse, (void*)a);
	pthread_join(thread2, NULL);
	return 0;
}