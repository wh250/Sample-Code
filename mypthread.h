//mypthread.c
//header for user level pthread
#ifndef H_MYPTHREAD
#define H_MYPTHREAD

#include <ucontext.h>
#include <stdio.h>      
#include <stdlib.h>
#include <errno.h>

//threadnode
struct threadNode; //defined in .c class

//structure for thread information, contains node and id
typedef struct{
    //Thread ID
    short tid;
    //Pointer to thread node 
    struct threadNode* mynode;
} mypthread_t;

typedef struct {
    //Not specified to implement in project instructions but decided to do it anyway.
} mypthread_attr_t;

// Functions
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

void mypthread_exit(void *retval);

int mypthread_yield(void);

int mypthread_join(mypthread_t thread, void **retval);

typedef int mypthread_mutex_t;
typedef int mypthread_mutexattr_t;

static inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
        	const mypthread_mutexattr_t *attr) { return 0; }

static inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_lock(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex) { return 0; }

#endif
