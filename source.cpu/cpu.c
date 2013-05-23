#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "cpu.tab.h"

#define DEBUG 0

#define MAXPRO 6          // max num of processes
#define MAXMEM 200        // max size of a process
#define STACKSIZE 100     // max size of the stack
#define REGISTERSIZE 10   // size of each process registers
#define MAXGMEM 20        // max size of global memory
#define NORMAL 0          // denotes a normal return from exe()
#define LOCKED 1          // stops process switching until unlock
#define UNLOCKED 2        // remove lock
#define ENDPROCESS 3
#define p0WRITE 4         // tells p0 to run-p0 should only run after a write to gmem

#define FALSE 0
#define TRUE 1

#include "helper.c"
#include "pagingsystem.c"
#include "filesystem2.c"

int  execute;

int  gmem[MAXGMEM];         // global var sit here 
int  mem[MAXPRO][MAXMEM];   // physical memory

int  endprog[MAXPRO];       // last instruction of proc
int  pid = 0;               // current process id

int  p0running;

/* prototypes */
int exe(int stack[][STACKSIZE], int sp[], int reg[][REGISTERSIZE], int next_instruct[], int next_inst[], int cur_proc);
int pop(int stack[][STACKSIZE], int proc_id, int sp[], int calledfrom);
void push(int stack[][STACKSIZE], int proc_id, int sp[],int data, int calledfrom);
void print_stack(int stack[][STACKSIZE],int sp[]); //debug
void print_register(int reg[][REGISTERSIZE]); //debug
void print_gmem();


executeit() {
  int cur_proc;
  int p0 = 0;
  int msg = -1;
  int m;
  int stack[MAXPRO][STACKSIZE];
  int sp[MAXPRO];
  int next_instruct[MAXPRO];
  int proc_complete[MAXPRO];
  int reg[MAXPRO][REGISTERSIZE];
  int locked = UNLOCKED;

  memset(stack, 0, MAXPRO * STACKSIZE * sizeof(int));
  memset(sp , -1, MAXPRO * sizeof(int));
  memset(proc_complete, 0, MAXPRO * sizeof(int));
  memset(reg, 0, 10 * MAXPRO * sizeof(int));
  srand(time(NULL));

  next_instruct[0] = 10;
  next_instruct[1] = 10;
  next_instruct[2] = 10;
  next_instruct[3] = 10;
  next_instruct[4] = 10;
  next_instruct[5] = 10;

  while(1) {
    cont:
      if(locked == UNLOCKED)
        cur_proc = (pid == 1) ? 0:(rand()%(pid - 1)) + 1;

      if(proc_complete[cur_proc] == 1) {
        printf("----------------------------cur_proc: %d\n", cur_proc);
        goto checkdone;
      }

      if (next_instruct[cur_proc] < endprog[cur_proc]) {
        msg = exe(stack, sp, reg, next_instruct, next_instruct, cur_proc);
        
        if(msg == ENDPROCESS)
          proc_complete[cur_proc] = 1;

        //increment next_instruction
        next_instruct[cur_proc]++;
        
        if(msg == UNLOCKED)
          locked = UNLOCKED;
        else if(msg == LOCKED || locked == LOCKED)
          locked = LOCKED;
         
        //run p0 in its entirety after a gmem write
        //cur_proc=0;
        while(msg == p0WRITE || p0running) {
          p0running = 1; 
          cur_proc = 0;
          msg = exe(stack,sp,reg,next_instruct,next_instruct,p0);

          next_instruct[cur_proc]++;
          
          if(p0running == 0) {  
            msg=NORMAL;
            next_instruct[p0]=10;
            break;
          }

          if( next_instruct[p0]>=endprog[p0]) {  
            p0running=0;
            sp[p0]=0;
            next_instruct[p0]=10;
            msg=NORMAL;
            break;
          }
        }
        continue;
      } else {
        proc_complete[cur_proc]=1;
      }

      //check if all processes are done
      checkdone:
        for(cur_proc = 1; cur_proc < pid; cur_proc++)
          if(proc_complete[cur_proc] == 0)
            goto cont;
      break;
  }

  print_stack(stack,sp);        // stack should be all 0 and sp at -1
  print_gmem();
  print_register(reg);
}


int exe(int stack[][STACKSIZE], int sp[], int reg[][REGISTERSIZE], int next_instruct[], int next_inst[], int cur_proc) {
  int i,k, m; 
  int tmp,tmp1, tmp2;
  char name[11];

  i = next_inst[cur_proc];

  switch (mem[cur_proc][i]) {
    /** OPEN, READ, CLOSE, WRITE, SEEK ::  OS services **/
    case OPEN :
      tmp = peek(stack, cur_proc, sp, 0) ;
      printf("OPEN offset=  0,  data=%d\n", tmp); 
      tmp1 = peek(stack, cur_proc, sp, -1) ;
      printf("OPEN offset= -1,  data=%d\n", tmp1); 
      i = 0;
      while ( name[i] =  mem[cur_proc][tmp1+i++] );
      printf("filename passed = %s\n", name);
      printf("OS service call  --- <OPEN>  return file descriptor!(987 is fake)\n");
      push(stack, cur_proc, sp, 987, 11); // dummy fd =987 
      break;
      
    case READ :
      tmp = peek(stack,cur_proc,sp, 0);
      printf("READ,  file descriptor=%d\n", tmp); 
      printf("OS service call  --- <READ> return int read (777 is fake)\n");
      push(stack,cur_proc,sp, 777, 13); // dummy fd =777 
      break;

    case CLOSE :
      tmp = peek(stack, cur_proc, sp, 0);
      printf("CLOSE,  file descriptor=%d\n", tmp); 
      printf("OS service call  --- <CLOSE> close file\n");
      break;
    
    case WRITE :
      tmp = peek(stack, cur_proc, sp, 0);
      printf("WRITE offset=  0,  data=%d\n", tmp); 
      tmp1 = peek(stack, cur_proc, sp, -1) ;
      printf("WRITE offset= -1,  fd =%d\n", tmp1); 
      printf("OS service call  --- <WRITE> \n");
      break;

    case SEEK :
      tmp = peek(stack, cur_proc, sp, 0) ;
      printf("SEEK offset=  0,  data=%d\n", tmp); 
      tmp1 = peek(stack, cur_proc, sp, -1) ;
      printf("SEEK offset= -1,  fd =%d\n", tmp1); 
      printf("OS service call  --- <SEEK> \n");
      break;

    case POPD : 
      tmp = mem[cur_proc][i+1];
      tmp1 = pop(stack, cur_proc, sp, 10) ;
      if(tmp < 230) {   
        gmem[tmp]=tmp1;
      } else {  
        tmp = tmp - 230;
        reg[cur_proc][tmp]= tmp1;
      }
      next_inst[cur_proc]++;
      break;

    case POP : 
      tmp1 = pop(stack, cur_proc, sp, 12) ;
      break;

    case LD : 
      tmp = pop(stack, cur_proc, sp, 14) ;
      tmp1 = gmem[tmp];
      push(stack, cur_proc, sp, tmp1, 15);
      break;

    case LA : 
      tmp = mem[cur_proc][i + 1];
      //load address of start of array
      push(stack,cur_proc,sp,tmp, 17);
      next_inst[cur_proc]++;
      break; 
    
    case LOAD : 
      tmp = mem[cur_proc][i + 1];
      if (tmp < 230)
        tmp1 = gmem[tmp];
      else {
        tmp = tmp - 230;
        tmp1 = reg[cur_proc][tmp];
      }
      push(stack, cur_proc, sp, tmp1, 19);
      next_inst[cur_proc]++;
      break;

    case LOADI : 
      push(stack, cur_proc, sp, mem[cur_proc][i + 1], 21); 
      next_inst[cur_proc]++;
      break;
    
    case ADD: 
      tmp1 = pop(stack, cur_proc, sp, 16);
      tmp2 = pop(stack, cur_proc, sp, 18);
      tmp1 += tmp2;
      push(stack, cur_proc, sp, tmp1, 23);
      break;

    case SUB : 
      tmp1 = pop(stack, cur_proc, sp, 20);
      tmp2 = pop(stack, cur_proc, sp, 22);
      tmp1 = tmp2 - tmp1;
      push(stack, cur_proc, sp, tmp1, 25);
      break;

    case MUL: 
      tmp1 = pop(stack, cur_proc, sp, 24);
      tmp2 = pop(stack, cur_proc, sp, 26);
      tmp1 *= tmp2;
      push(stack, cur_proc, sp, tmp1, 27);
      break;

    case DIV : 
      tmp1 = pop(stack, cur_proc, sp, 28);
      tmp2 = pop(stack, cur_proc, sp, 30);
      tmp1 /= tmp2;
      push(stack, cur_proc, sp, tmp1, 29);
      break;
    
    case END: 
      printf("Process %d completed normally\n", cur_proc);
      p0running = 0;
      return ENDPROCESS;
     
    case ENDP: 
      printf("ENDP\n");
      break;

    case AND: 
      tmp1 = pop(stack, cur_proc, sp, 32);
      tmp2 = pop(stack, cur_proc, sp, 34);
      tmp1 = tmp1 && tmp2;
      push(stack, cur_proc, sp, tmp1, 31);
      break;

    case OR: 
      tmp1 = pop(stack, cur_proc, sp, 36);
      tmp2 = pop(stack, cur_proc, sp, 38);
      tmp1 = tmp1 || tmp2;
      push(stack, cur_proc, sp, tmp1, 33);
      break;
 
    case NOT: 
      tmp1 = pop(stack, cur_proc, sp, 40);
      tmp1 = !tmp1;
      push(stack, cur_proc, sp, tmp1, 35);
      break; 

    case LE_OP: 
      tmp1 = pop(stack, cur_proc, sp, 42);
      tmp2 = pop(stack, cur_proc, sp, 44);
      tmp = tmp1 <= tmp2;
      push(stack, cur_proc, sp, tmp, 37);
      break; 

    case GE_OP: 
      tmp1 = pop(stack, cur_proc, sp, 46);
      tmp2 = pop(stack, cur_proc, sp, 48);
      tmp = tmp1 >= tmp2;
      push(stack, cur_proc, sp, tmp, 39);
      break; 

    case LT_OP:
      tmp1 = pop(stack, cur_proc, sp, 50);
      tmp2 = pop(stack, cur_proc, sp, 52);
      tmp = tmp2 < tmp1;
      push(stack, cur_proc, sp, tmp, 41);
      break;
 
    case GT_OP: 
      tmp1 = pop(stack, cur_proc, sp, 54);
      tmp2 = pop(stack, cur_proc, sp, 56);
      tmp = tmp1 > tmp2;
      push(stack, cur_proc, sp, tmp, 43);
      break;

    case EQ_OP: 
      tmp1 = pop(stack, cur_proc, sp, 58);
      tmp2 = pop(stack, cur_proc, sp, 60);
      tmp = tmp1 == tmp2;
      push(stack, cur_proc, sp, tmp, 45);
      break;

    case NE_OP: 
      tmp1 = pop(stack, cur_proc, sp, 62);
      tmp2 = pop(stack, cur_proc, sp, 64);
      tmp = tmp1 != tmp2;
      push(stack, cur_proc, sp, tmp, 47);
      break; 

    case STOP: 
      printf("STOP called by proccess %d, hit any key to continue\n", cur_proc);
      scanf("%d",&tmp2);
      break;
    
    case STOR: 
      tmp = pop(stack, cur_proc, sp, 68);
      tmp1 = mem[cur_proc][i + 1];
      if(tmp1 < 230) {
        gmem[tmp1]=tmp;
        printf("Process %d wrote to global mem in index %d, %d\n",cur_proc,tmp1,gmem[tmp1]);
        next_inst[cur_proc]++;
        return p0WRITE;
      } else {    
        reg[cur_proc][tmp1 - 230] = tmp;
        next_inst[cur_proc]++;
      }
      break;
      
    case ST: 
      tmp = pop(stack, cur_proc, sp, 70); //does ST ever need to store in a register?
      tmp1 = pop(stack, cur_proc, sp, 72);
      gmem[tmp] = tmp1;
      printf("process %d wrote to global mem in index %d, %d\n",cur_proc,tmp,gmem[tmp]);
      return p0WRITE;
    
    case LOCK: 
      printf("LOCK called by process %d\n", cur_proc); 
      return LOCKED;
    
    case UNLOCK: 
      printf("UNLOCK called\n");
      return UNLOCKED;
      
    case HALT: 
      printf("HALT called by process %d\n\n", cur_proc);
      print_gmem();
      print_register(reg);
      exit(0); 
      
    case JFALSE: 
      tmp = pop(stack, cur_proc, sp, 74);
      tmp2 = mem[cur_proc][i + 1];
      if (tmp == 0)
        next_instruct[cur_proc] = tmp2 - 1; //sub one for PC in executeit()
      else 
        next_inst[cur_proc]++;
      break;
      
    case JMP: 
      tmp = mem[cur_proc][i + 1];
      next_instruct[cur_proc] = tmp - 1; //sub one for PC in executeit() 
      break;

    default:
      printf("illegal instruction mem[%d][%d]\n", cur_proc, i);
      printf("(%04d:   %d)\n", i, mem[cur_proc][i]);  
      break;

  }

  return NORMAL;
}

int peek(int stack[][STACKSIZE], int proc_id, int sp[], int offset) {
  int val = stack[proc_id][sp[proc_id] + offset];
  printf("peek : %d\n", val);
  return val;
}

int pop(int stack[][STACKSIZE], int proc_id, int sp[], int calledfrom) {
  int val = stack[proc_id][sp[proc_id]];
  sp[proc_id]--;
  if(sp[proc_id] < -1) {
    printf("Stack Underflow: process %d %d\n",proc_id,sp[proc_id]);
    printf("Called from  %d\n", calledfrom);
    exit(-1);
  }
  return val;
}

void push(int stack[][STACKSIZE], int proc_id, int sp[], int data, int calledfrom) {
  sp[proc_id]++;

  if(sp[proc_id]>STACKSIZE) {
    printf("Stack Overflow: process %d %d %d\n",proc_id,sp[proc_id],data);
    printf("PUSH Called from  %d\n", calledfrom);
    exit(-1);
  }
  
  stack[proc_id][sp[proc_id]]=data;
}


//debug routines
void print_stack(int stack[][STACKSIZE], int sp[]) {
  int i,j;
  for (i = 0; i < pid; i++) {
    printf("Stack contents for process %d\n", i);
    for (j = 0; j < STACKSIZE; j++)
      printf("%d\n",stack[i][j]);
    
    printf("SP at %d\n\n",sp[i]);
  }
}

void print_gmem() {
  int i;
  printf("Global memory: size %d\n",MAXGMEM);
  
  for (i = 0; i < MAXGMEM; i++)
    printf("%d  ", gmem[i]);
  
  printf("\n");
}

void print_register(int reg[][REGISTERSIZE]) {
  int i,j;
  printf("Register data\n");
  for (i = 0; i < pid; i++) {
    printf("Process %d: ",i);
    
    for (j = 0; j < REGISTERSIZE; j++)
      printf("%d  ",reg[i][j]);
    
    printf("\n");
  }
}

// TODO: convert to paging
void importMemory(char* filename) {
  int i;
  int j;
  int process;
  
  FILE* fp;
  
  fp = fopen(filename, "r");  
  
  getString(fp);
  pid = getInt(fp);

  for (i = 0; i < pid; i++) {
    getString(fp);
    process = getInt(fp);
    
    getString(fp);
    endprog[i] = getInt(fp);
    
    int data[endprog[i]];
    
    for (j = 0; j <= endprog[i]; j++)
      mem[i][j] = getInt(fp);
  }
  
  fclose(fp);
}


main(int argc, char **argv) {
  int   sharedMemoryId;
  char  c;
  char* s;
  char* sharedMemory;
  
  
  execute = TRUE;

  if(argc != 2) { 
    fprintf(stderr, "usage: cpu <input> \n");
    exit(0);
  }
  
  if (FS_DEBUG)
    fs_test();
  
  /*
  printf("\n\n\n\n");
  starLine();
  printf("cpu starting...\n");
  starLine();
  printf("\n");
  
  fs_initialize();
  pageTable_Initialize();
  
  if (DEBUG)
    fs_test();
  
  heavyLine();
  fs_import(argv[1]);
  printf("\n");

  heavyLine(); 
  printf("displaying inodes... \n");
  lightLine();
  fs_dump();
  printf("\n");
  
  heavyLine();
  printf("displaying filesystem... \n");
  lightLine();
  fs_dumpAllData();
  lightLine();
  
  int i;
  int j;
  
  int tempPageSize = 5;
  int tempPage[tempPageSize];
  
    
  printf("getFile()\n");
  printf("displaying tempPage: \n");
  
  for (i = 0; i < 4; i++) {
    fs_getFile(1, i * tempPageSize, tempPageSize, tempPage);
  
    for (j = 0; j < tempPageSize; j++)
      printf("%5d", tempPage[j]);
    printf("\n");
  }
  lightLine();
  */

  // read file into memory
  importMemory(argv[1]);
  
  while(TRUE) {
    // scheduler_nextProcess();
    executeit();
      
  }
  
  
  return 0;
}