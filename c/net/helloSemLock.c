#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SEMKEY 2L
#define PERMS 0666

static struct sembuf op_lock[2]={
0,0,0,
0,1,SEM_UNDO
};

static struct sembuf op_unlock[1]={
0,-1,(IPC_NOWAIT|SEM_UNDO)
};
int sem_id=-1;
void lock(){
if(sem_id<0){
   sem_id=semget(SEMKEY,1,IPC_CREAT|PERMS);
}
   semop(sem_id,&op_lock[0],2);

}
void unlock(){
   semop(sem_id,&op_unlock[0],1);
}




int fork_hello(int s){
   int node_id=0;
   node_id = fork();
   int i;
   if (node_id != 0) {//root
      return node_id;
   }else{ 
      printf("pid(%d) start\n",getpid());
      int semval=0;
      semval=semctl(sem_id,0,GETVAL);
      printf("pid=%d before lock semval%d\n",getpid(),semval);
      lock();

      semval=semctl(sem_id,0,GETVAL);
      printf("pid=%d after lock semval=%d\n",getpid(),semval);
      for (i=0;i<10;i++){//node
         printf("%d pid=%d node_id=%d\n",i,getpid(),node_id);
         usleep(s);
         if(i>4){
            int sempid=semctl(sem_id,0,GETPID);
            if(sempid==getpid()){
               unlock();
               semval=semctl(sem_id,0,GETVAL);
               printf("pid=%d unlock semval=%d\n",getpid(),semval);
            }
         }
      }
   }

   return 0;
}


void waitOver(int pid){
   int status;
   do{
      if( waitpid(pid, &status, WUNTRACED | WCONTINUED)==-1)
         break;
   }while(!WIFEXITED(status) && !WIFSIGNALED(status));
}
int main(int argc,char *argv[],char *envp[])
{
   int id1,id2=0;
   id1=fork_hello(100000);
   if(id1==0){
      printf("pid(%d) end\n",getpid());
      return 0;
   }
   id2=fork_hello(200000);
   if(id2==0){
      printf("pid(%d) end\n",getpid());
      return 0;
   }
   waitOver(id1);
   waitOver(id2);
  printf("main end\n");
   return 0;
}
