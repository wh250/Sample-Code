//mypthread.c
//user space pthread library
//uses context to implement
#include "mypthread.h"
#include <list>
#define MEM 64000

//information about a thread
typedef struct threadNode{
    //thread id
	short tid;
	//done or not
	bool done=false;
	//context that called it
	ucontext_t Mainfunc;
	//context to run
	ucontext_t thrdfunc;
	//return value
	void* retal;
	//value to make sure it only runs create once
	int a=0;
}Node;

//context for thread
ucontext_t thrd;
//threadnode for main
threadNode* Main = new threadNode;
//list of all threads, Main is automatically put in
std::list<threadNode*> threadlist{Main};
int swit;

//function pointer
void* (*fn)(void*);
//argument pointer
void* argm;
//shell to change function with argument to one without
void fnshell(){
	fn(argm);
} 

//pthread_create implementation with context switching
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void* (*start_routine) (void *), void *arg){
	//new threadnode for new thread
	threadNode* t = new threadNode;
	if (0 == t){
		printf ("get mem faile\n");
	}
	//store main function in main and thread
	thread->mynode=t;
	getcontext(&(Main->Mainfunc));
	getcontext(&(t->Mainfunc));
	getcontext(&(Main->thrdfunc));
	getcontext(&(t->thrdfunc));
	//set new information for new thread context
	t->thrdfunc.uc_link=0;
	t->thrdfunc.uc_stack.ss_sp=malloc(MEM);
	t->thrdfunc.uc_stack.ss_size=MEM;
	t->thrdfunc.uc_stack.ss_flags=0;
	//make new context for new thread with function and argument
	makecontext(&(t->thrdfunc),(void(*)())start_routine,1,arg);
	//only call create once, since main is stored here and resumes here
	// and i don't want to run it again
	if(t->a>0){
		return 0;
	}
	t->a++;
	//put thread in front of list
	threadlist.push_front(t);
	//change context from main caller to new thread
	swapcontext(&(t->Mainfunc),&(t->thrdfunc));
	return 0;
}

//exit thread
void mypthread_exit(void *retval){
	//is done, store return value
	threadlist.front()->done=true;
	threadlist.front()->retal=retval;
	//remove from list
	threadNode* temp=threadlist.front();
	threadlist.pop_front();
	//set context back to caller
	setcontext(&(temp->Mainfunc));
}

//yield so others can run
int mypthread_yield(){
	//move from front of list to back
	threadlist.push_back(threadlist.front());
	threadlist.pop_front();
	//swap to next context of next thread
	swapcontext(&(threadlist.back()->thrdfunc),&(threadlist.front()->thrdfunc));
	return 0;
}

//join done thread
int mypthread_join(mypthread_t thread, void **retval){
	//allows to call exit function, and checks return value
	while(!thread.mynode->done){
		setcontext(&(thread.mynode->thrdfunc));
	}
	*retval=thread.mynode->retal;
	return 0;
}
