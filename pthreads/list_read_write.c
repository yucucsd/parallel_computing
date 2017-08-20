#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
struct node{
    int val;
    struct node* next;
};
struct node* head_p;
int num = 10;
long thread_count;
pthread_rwlock_t rwlock;
int Member(int val, struct node* head_p){
    while(head_p != NULL && head_p->val < val){
        head_p = head_p->next;
    }
    if (head_p == NULL || head_p->val > val) return 0;
    else return 1;
}

int Insert(int val, struct node* head_p){
    while (head_p->next != NULL && head_p->next->val < val){
        head_p = head_p->next;
    }
    if (head_p->next == NULL || head_p->next->val >= val){
        struct node* latter = head_p->next;
        struct node* nt = malloc(sizeof(struct node));
        nt->val = val;
        head_p->next = nt;
        nt->next = latter;
        return 1;
    }else{
        return 0;
    }
}

int Delete(int val, struct node* head_p){
    while(head_p->next != NULL && head_p->next->val != val){
        head_p = head_p->next;
    }
    if(head_p->next == NULL || head_p->next->val != val){
        return 0;
    }else{
        struct node* nt = head_p->next;
        head_p->next = head_p->next->next;
        free(nt);
        return 1;
    }
}
void* thread_work(void* rank){
    long my_rank = (long) rank;
    pthread_rwlock_rdlock(&rwlock);
    Member(my_rank, head_p);
    pthread_rwlock_unlock(&rwlock);
    pthread_rwlock_wrlock(&rwlock);
    Delete(my_rank, head_p);
    pthread_rwlock_unlock(&rwlock);
    pthread_rwlock_wrlock(&rwlock);
    Insert(my_rank, head_p);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}
int main(int argc, char* argv[]){
    long thread;
    head_p = malloc(sizeof(struct node));
    head_p->next = malloc(sizeof(struct node));
    struct node* start = head_p->next;
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for (int i = 0; i < num; i++){
        start->next = malloc(sizeof(struct node));
        start->val = i;
        start = start -> next;
    }
    start = head_p->next;
    while(start -> next != NULL){
        printf("%d ", start->val);
        start = start-> next;
    }
    printf("\n");
    pthread_rwlock_init(&rwlock, NULL);
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, thread_work, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    printf("%d\n", Member(0, head_p));
    start = head_p -> next;
    while(start -> next != NULL){
        printf("%d ", start->val);
        start = start-> next;
    }
    printf("\n");
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
