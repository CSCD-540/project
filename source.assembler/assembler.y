%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char  *yytext;
FILE  *yyin;
FILE  *outfile;
int   yy_flex_debug;
int   execute;


typedef struct { 
  char labelname[4];
  int  pid;
  int  memloc;
} ILABEL;

typedef struct { 
  char label[5];
  int address;
} JSYM;

typedef struct { 
  char varname[11];
  int address;
} VTABLE;

VTABLE vtable[50];      // variable/address name table
int vtablex = 0;
int vaddress = 0;

ILABEL ilabel[10];      // store mem loc for label
int ilabelindex = 0;
int initdataindex = 0;

int initdatasection[100];
int index2initdata[20];
int index2index=0;

JSYM locjsym[30];
JSYM refjsym[30];
int refex=0;
int locex=0;

typedef struct { 
  char idop[11];
  int type;
} PARSE;

PARSE parse[100];

int yylex(void);
void yyerror(char *);

int i,j,kkkk;

int  sindex = 0;
int  pindex = 0;
int  memloc = 0;

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

int gmem[MAXGMEM];        // global var sit here 
int mem[MAXPRO][MAXMEM]; 
int endprog[MAXPRO];      // last instruction of proc
int jsym[60]; 
int pid = 0;              // process id

int p0running;

//execute one instruction return 0 if instruction is not lock
int exe(int stack[][STACKSIZE],int sp[],int reg[][REGISTERSIZE], int next_instruct[],int next_inst[], int cur_proc);
int pop(int stack[][STACKSIZE], int proc_id, int sp[], int calledfrom);
void push(int stack[][STACKSIZE], int proc_id, int sp[],int data, int calledfrom);
void print_stack(int stack[][STACKSIZE],int sp[]); //debug
void print_register(int reg[][REGISTERSIZE]); //debug
void print_gmem();

int showparse(int beg) { 
  int i;

  for (i=beg; i<sindex; i++)   
    printf("############## i=%d  %s, (type)%d\n", i, parse[i].idop, parse[i].type);
      
  printf("end show\n");
}

#if 0
  int termcopy(int beg) { 
    int i, j;
    printf("termcopy: storage loc calc here\n");
    j = 0;        
    strcpy(parsecopy[j].idop, parse[beg].idop);
    parsecopy[j].type = parse[beg].type;
 
    printf("%s  ->   %s\n", parse[beg].idop, parsecopy[beg].idop);

    if(parse[beg+1].idop[0] == '[') { 
      i= beg+1; 
      j++;
      do { 
        /* append parse[i].idop to tgt */
        strcpy(parsecopy[j].idop, parse[i].idop);
        printf("%s  ->   %s\n", parse[i].idop, parsecopy[j].idop);
        parsecopy[j].type = parse[beg].type;
        j++;
      } while (parse[i++].idop[0] != ']');
    }

    copysindex = j;
  }
#endif

int showterm( int beg) { 
  int i;
  return;
}

%}

%union {
  int intval;
  char *sptr;
}

%token NUMBER
%token ID
%token LD 
%token LA 
%token LOAD 
%token LOADI 
%token ADD
%token SUB 
%token MUL 
%token DIV 
%token END
%token ENDP
%token PROCESS
%token AND
%token OR
%token NOT
%token LE_OP
%token GE_OP
%token LT_OP
%token GT_OP
%token EQ_OP
%token NE_OP
%token STOP 
%token STOR
%token ST 
%token DATA 
%token IDATA 
%token COMMA 
%token COLON 
%token LOCK 
%token UNLOCK 

%token OPEN 
%token READ 
%token WRITE 
%token CLOSE 
%token SEEK 
%token POPD 
%token POP 

%token DOT 
%token HALT 
%token JFALSE 
%token JMP 

%left  LPREC 
%right RPREC 

%%

  programA: 
    program { 
      int j,k;
      printf("END of ProgramA\n");
      for(j=0; j<ilabelindex; j++) {
        printf("pid = %d\n", ilabel[j].pid );
         printf("memloc = %d\n", ilabel[j].memloc);
         printf("name = %s\n", ilabel[j].labelname);
      }
  };

  program: 
    program0 
      | varlistend  program0  {; }
      | program  program0  {;};

  varlistend: 
    dotdata varlists {;};

  dotdata:
    DOT DATA { 
      sindex=0;
    };

  varlists:
    varlists varlist
      | varlist;

  varlist:   
    term { 
      int i = 0;
      strcpy(vtable[vtablex].varname, parse[i].idop);
      vtable[vtablex].address=vaddress;
      vtablex++;
      vaddress++;

      showsymtable();
                
      gmem[memloc++]=0;
      sindex = 0;
    }
    | term COMMA term { 
      int i,j;

      printf("term comma term\n");
      showparse(0);
      printf("vtablex=%d vaddress:%d\n", vtablex, vaddress);

      strcpy(vtable[vtablex].varname, parse[0].idop);

      vtable[vtablex].address=vaddress;
      sscanf(parse[2].idop, "%d", &i);

      printf("vtable[%d].varname=%s, parse[0].idop=%s\n", 
      vtablex, vtable[vtablex].varname,    parse[0].idop);
      printf(" vtable[vtablex].address=%d\n", vtablex, vtable[vtablex].address);
   
      vaddress += i;
      vtablex++;

      showsymtable();
      printf("vtablex=%d vaddress:%d\n", vtablex, vaddress);

      for(j = 0; j < i; j++)  
        gmem[memloc++]=0;
      
      sindex = 0;
    };

  program0:   
    program1
    | program1 initdata; 

  initdata: 
    dotidata labels {}
    | dotidata {};
  
  dotidata: 
    DOT IDATA { 
      sindex=0;
    };

  labels:   
    labels labl collabeldata
    | labl collabeldata {};

  labl: 
    term {};

  collabeldata: 
    COLON term { 
      int i, j, k;
      int ch; 
      char *chptr;

      printf("collabeldata\n");
      showparse(0);
      printf("==================================\n");
      printf("parse[0].idop=%s\n", parse[0].idop);
      printf("parse[2].idop=%s\n", parse[2].idop);

      for(j=0; j<ilabelindex; j++) {
        printf("pid = %d\n", ilabel[j].pid );
        printf("memloc = %d\n", ilabel[j].memloc);
        printf("name = %s\n", ilabel[j].labelname);

        if( ilabel[j].memloc != -999) { 
          printf("compare ilabel's %s  and label parsed %s\n", ilabel[j].labelname,  parse[0].idop); 
        
          if( strcmp(ilabel[j].labelname ,  parse[0].idop)==0) { 
            printf("label found, endprog is at  %d\n", endprog[ ilabel[j].pid] );
            chptr =  parse[2].idop;
            for( ; *chptr; chptr++) 
              printf("%c", *chptr);

            printf("\n");

            chptr =  parse[2].idop;
            mem[ ilabel[j].pid][ ilabel[j].memloc ] = endprog[ilabel[j].pid];
                 
            for( ; *chptr; chptr++) { 
              printf("at %d: locating '%c'\n", endprog[ilabel[j].pid], *chptr);
              mem[ilabel[j].pid][endprog[ilabel[j].pid]] = *chptr;
              endprog[ilabel[j].pid]++;
            }

           mem[ ilabel[j].pid][endprog[ilabel[j].pid]] = 0;
           endprog[ilabel[j].pid]++;
           ilabel[j].memloc = -999;
         } 
       }

    }

    printf("==================================\n");
    printf("==================================\n");
    
    for(j=0; j<ilabelindex; j++)
      printf("pid = %d  end lo = %d\n", ilabel[j].pid, endprog[ilabel[j].pid]);
    
    printf("==================================\n");

    // allocate parse[2].idop to initdatasection[]
    // and backpatch Li labels. 

    printf("index2initdata[%d] =  %d\n", index2index, initdataindex);       
    index2initdata[index2index]=initdataindex;       

    for (k=0; ch = *((char *)(parse[2].idop)+k); k++)
      initdatasection[initdataindex++]= ch;
    
    index2index++;
 
    for(k=0; k<initdataindex; k++)
      printf("%c", initdatasection[k]);
    
    printf("\n");
    
    for(k = 0; k < index2index; k++)
      printf("%d  ", index2initdata[k]);
    
    printf("\n==================================\n");
    sindex = 0;
  };

  program1:   
    process0 code0 END {
       mem[pid][memloc++]=END;
       sindex=0;
       endprog[pid]=memloc; // last instruction location+1
       printf("at END refex=%d locex=%d\n", refex, locex);
       showjsym(); 
       refex = locex = 0;
       pid++;
     };

  process0:
    PROCESS term {
      showterm(0);
      sindex = 0; 
      memloc = 10;
      
      if(pid > MAXPRO) {
        fprintf(stderr,"max num of proccesses currently 6\n");
        exit(1);
      }
    };

  code0:
    code0 code
    | code;

  code:    
    LOAD term {
      int i, j;
      mem[pid][memloc++] = LOAD;
      i = searchvtable(parse[0].idop, &j, 1);
      if ( i >= 230 ) 
         mem[pid][memloc++]=i;
      else {
         printf("vtable[%d].address=%d\n",i, vtable[i].address);
         mem[pid][memloc++]=vtable[i].address;
       }

       showterm(0);
       sindex = 0;
    }
    | LOADI term {
      int i;
      mem[pid][memloc++] = LOADI;
      sscanf(parse[0].idop, "%d", &i);
      mem[pid][memloc++]=i;
      showterm(0);
      sindex = 0;
    }
    | LA term { 
      int i, j;
      mem[pid][memloc++] = LA;
      printf("LA term parse[0].idop = %s\n",parse[0].idop); 
      i = searchvtable(parse[0].idop, &j, 2);
      printf("LA term searchvtable ret val= %d\n", i); 

      if( i == -999) { 
        ilabel[ilabelindex].pid = pid;
        ilabel[ilabelindex].memloc = memloc;
        sprintf(ilabel[ilabelindex].labelname, "%s",  parse[0].idop);
        ilabelindex++;
        mem[pid][memloc++] = -999;
        printf("wrote mem[%d][%d] =  %d\n", pid, memloc-1, mem[pid][memloc-1]); 
      } else {
        mem[pid][memloc++]=i;
      }
      
      showterm(0);
      sindex = 0;
    }
    | OPEN {
      mem[pid][memloc++]=OPEN;
      showterm(0);
      sindex = 0;
    }
    | READ {
      mem[pid][memloc++]=READ;
      showterm(0);
      sindex = 0;
    }
    | WRITE {
      mem[pid][memloc++]=WRITE;
      showterm(0);
      sindex = 0;
    }
    | CLOSE {
      mem[pid][memloc++]=CLOSE;
      showterm(0);
      sindex = 0;
    }
    | SEEK {
      mem[pid][memloc++]=SEEK;
      showterm(0);
      sindex = 0;
    }
    | POPD term {
      int i, j;
      showterm(0);
      mem[pid][memloc++] = POPD;
      i = searchvtable(parse[0].idop, &j, 31);
      if( i >= 230 ) 
        mem[pid][memloc++]=i;
      else 
        mem[pid][memloc++]=vtable[i].address;
          
      showterm(0);
      sindex = 0;
    }
    | POP {
      mem[pid][memloc++]=POP;
      showterm(0);
      sindex = 0;
    }
    | JFALSE term { 
      mem[pid][memloc++]=JFALSE;
      mem[pid][memloc]=-1;
      refjsym[refex].address = memloc++;
      sprintf( refjsym[refex].label, "%s", parse[0].idop);
      refex++;
      showterm(0);
      sindex = 0;
    }
    | JMP term {
      mem[pid][memloc++] = JMP;
      mem[pid][memloc] = -1;
      refjsym[refex].address = memloc++;
      sprintf( refjsym[refex].label, "%s", parse[0].idop);
      refex++;
      showterm(0);
      sindex = 0;
    }
    | LOCK {
      mem[pid][memloc++]=LOCK;
      showterm(0);
      sindex = 0;
    }
    | UNLOCK {
      mem[pid][memloc++]=UNLOCK;
      showterm(0);
      sindex = 0;
    }
    | LD {
      mem[pid][memloc++]=LD;
      showterm(0);
      sindex = 0;
    }
    | ST {
      mem[pid][memloc++]=ST;
      showterm(0);
      sindex = 0;
    }
    | STOR term {
      int i, j;
      showterm(0);
      mem[pid][memloc++]=STOR;
      i = searchvtable(parse[0].idop, &j, 3);
   
      if( i >= 230 )  
        mem[pid][memloc++] = i;
      else
        mem[pid][memloc++] = i;

      showterm(0);
      sindex = 0;
    }
    | EQ_OP {
      mem[pid][memloc++] = EQ_OP;
      showterm(0);
      sindex = 0;
    }
    | GT_OP { 
      mem[pid][memloc++]=GT_OP;
      showterm(0);
      sindex = 0;
    }
    | GE_OP {
      mem[pid][memloc++]=GE_OP;
      showterm(0);
      sindex = 0;
    }
    | LT_OP {
      mem[pid][memloc++]=LT_OP;
      showterm(0);
      sindex = 0;
    }
    | LE_OP {
      mem[pid][memloc++]=LE_OP;
      showterm(0);
      sindex = 0;
    }
    | ADD {
      mem[pid][memloc++]=ADD;
      showterm(0);
      sindex = 0;
    }
    | SUB {
      mem[pid][memloc++]=SUB;
      showterm(0);
      sindex = 0;
    }
    | AND {
      mem[pid][memloc++]=AND;
      showterm(0);
      sindex = 0;
    }
    | OR  {
      mem[pid][memloc++]=OR;
      showterm(0);
      sindex = 0;
    }
    | HALT {
      mem[pid][memloc++]=HALT;
      showterm(0);
      sindex = 0;
    }
    | term COLON {
      locjsym[locex].address = memloc;
      sprintf( locjsym[locex].label, "%s", parse[0].idop);
      locex++;
      showterm(0);
      sindex = 0;
    };


  term:   
    ID {
      sprintf( parse[sindex].idop, "%s", yylval.sptr);
      parse[sindex].type = ID;
      sindex++;
    }
    | NUMBER {
      sprintf( parse[sindex].idop, "%s", yylval.sptr);
      parse[sindex].type = NUMBER;
      sindex++;
    };

%%

main(int argc, char **argv) {
  
  if(argc != 2) { 
    fprintf(stderr, "usage: assembler <input> \n");
    fprintf(stderr, "output: <input>.cpu \n");
    exit(0);
  }

  // global file pointer
  yyin = fopen(argv[1], "r" );

  // bison function
  yyparse();
  
  exportMemory(argv[1]);

  fclose(yyin);
  
  return 0;
}

void yyerror(char *s) {
  extern unsigned int linenumber;
  fprintf(stderr, "%s\n", s);
  fprintf(stderr, "line %d:  %s\n", linenumber+1, yytext);
  return;
}

showsymtable() {
  int i;
  for (i=0; i<vtablex; i++) {  
    printf("%d:  %s\n",
    vtable[i].address,  vtable[i].varname);
  }
}

int searchvtable(char *str, int *j, int from) {
  int i, found;

  printf("--------search vtable for %s  from=%d\n", str, from);
  printf("%c %c\n", *str, *(str+1));

  if( *str =='r' && ( '0' <= *(str+1) && *(str+1) <= '9' )) { 
    printf("this is local var\n");
    
    // 230 or above is locar var, r?
    return( (*(str+1) - '0')+230 ); 
  }

  for(i=found=0; i<vtablex; i++) {
    if(strcmp(vtable[i].varname, str)==0) { 
      found = 1;
      *j = i; 
      break;
    }
  }
  
  if(found) {
    ;
  } else {
    return (-999);
  }

  printf("---------search vtable for %s ended\n", str);
  printf("----------returning %d   and  j= %d\n", vtable[i].address, *j);
  return( vtable[i].address);
}

showjsym() { 
  int i, j;
  
  printf("SHOWSYM %d\n", execute);
  
  for (i = 0; i < locex; i++) {
    if (execute == 0) 
      printf("%s  %d\n", locjsym[i].label, locjsym[i].address );
  }

  printf("SHOWSYM refex %d\n", refex);
  
  for (i = 0; i < refex; i++) {
    if (execute == 0) 
      printf("%s  %d", refjsym[i].label, refjsym[i].address );
   
    if (execute == 0) 
      printf(" --  %d\n", mem[pid][ refjsym[i].address] );
  }
  
  /* search refjsym in locjsym, fill with locjsym.address */  
  for (i = 0; i < refex; i++) {
    printf("%s  %d", refjsym[i].label, refjsym[i].address );
    printf(" --  %d\n", mem[pid][ refjsym[i].address] );
    
    /* search refjsym in locjsym, fill with locjsym.address */  
    if(mem[pid][ refjsym[i].address] != -1)
      if(execute==0)   
        printf("jump lable wrong %s\n", refjsym[i].label);
    
    for(j = 0; j < locex; j++) {

      if(strcmp(locjsym[j].label, refjsym[i].label) == 0) {
        mem[pid][ refjsym[i].address] = locjsym[j].address; 
        break;
      }
    }
    
    if(j >= locex)
     if(execute==0) 
       fprintf(stderr,"lable not found: %s\n", refjsym[i].label);
  }
  
  printf("DONE showsym\n");  
}


int searchsym( char *str, int j) {
  int i;

  for(i = 0; i < vtablex; i++) {
    if(vtable[i].address == j ) { 
      sscanf(vtable[i].varname, "%s", str);
      return; 
    }
  }
  
  fprintf(stderr, " symbol %s not found, error\n", str);
  fprintf(stderr, " this may be initdata label.\n");

  printf("---------search vtable for %s ended\n", str);
}


exportMemory(char* filename) {
  int i;
  int j;
  
  strcat(filename, ".cpu");
  
  FILE *fd;
  fd = fopen(filename, "w");
  
  
  printf("************************************\n");
  printf("  Exporting Memory: %s\n", filename);
  printf("************************************\n");
      
  fprintf(fd, "processes: %d \n", pid);
  for (i = 0; i < pid; i++) {
    fprintf(fd, "  pid: %d \n", i);
    fprintf(fd, "  end: %d \n    ", endprog[i]);
    for (j = 0; j <= endprog[i]; j++)
      fprintf(fd, "%d ", mem[i][j]);
    fprintf(fd, "\n");
  }
  
}


// change printf to fprintf to print to file
showmem() { 
  int i;
  int k; 
  char name[11];
  int progid;
  
  for (k = 0; k < pid; k++) {
    printf("############### pid %d ############\n", k);
    printf("############### endprog %d ############\n", endprog[k]);
  
    // endprog[] contains last instruction addrs of proc
    for (i = 0; i < endprog[k]; i++) { 
      switch (mem[k][i] ) {

        case POPD:
          if (mem[k][i+1]<230) { 
            searchsym(name, mem[k][i + 1]);
            printf("%04d:  POPD\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  POPD\t %d\n", i, mem[k][i+1]); 
          
          i++;
          break;

        case POP :
          printf("%04d:  POP\n", i); 
          break;

        case OPEN :
          printf("%04d:  OPEN\n", i); 
          break;

        case READ :
          printf("%04d:  READ\n", i); 
          break;

        case WRITE :
          printf("%04d:  WRITE\n", i); 
          break;

        case SEEK :
          printf("%04d:  SEEK\n", i); 
          break;
          
        case CLOSE :
          printf("%04d:  CLOSE\n", i); 
          break;

        case LD :
          printf("%04d:  LD\n", i); 
          break;

        case LA :
          searchsym(name, mem[k][i+1]);
          printf("%04d:  LA\t %d (%s)\n", i, mem[k][i+1], name); i++;
          break;

        case LOAD :
          if(mem[k][i+1]<230) { 
            searchsym(name, mem[k][i+1]);
            printf("%04d:  LOAD\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  LOAD\t %d\n", i, mem[k][i+1]); 
          i++;
          break;

        case LOADI :
          printf("%04d:  LOADI\t %d\n", i, mem[k][i+1]); i++;
          break;

        case ADD:
          printf("%04d:  ADD\n", i); 
          break;

        case SUB :
          printf("%04d:  SUB\n", i); 
          break;

        case MUL :
          printf("%04d:  MUL\n", i); 
          break;

        case DIV :
          printf("%04d:  DIV\n", i); 
          break;
          case END:
          printf("%04d:  END\n", i); 
          break;

        case ENDP:
          printf("%04d:  ENDP\n", i); 
          break;

        case AND:
          printf("%04d:  AND\n", i); 
          break;

        case OR:
          printf("%04d:  OR\n", i); 
          break;

        case NOT:
          printf("%04d:  NOT\n", i); 
          break;

        case LE_OP:
          printf("%04d:  LE_OP\n", i); 
          break;
          case GE_OP:
          printf("%04d:  GE_OP\n", i); 
          break;

        case LT_OP:
          printf("%04d:  LT_OP\n", i); 
          break;
          case GT_OP:
          printf("%04d:  GT_OP\n", i); 
          break;

        case EQ_OP:
          printf("%04d:  EQ_OP\n", i); 
          break;

        case NE_OP:
          printf("%04d:  NE_OP\n", i); 
          break;

        case STOP :
          printf("%04d:  STOP\n", i); 
          break;

        case STOR:
          if(mem[k][i+1]<230) { 
            searchsym(name, mem[k][i+1]);
            printf("%04d:  STOR\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  STOR\t %d\n", i, mem[k][i+1]); 
          
          i++;
          break;

        case ST :
          printf("%04d:  ST\n", i); 
          break;

        case LOCK :
          printf("%04d:  LOCK\n", i); 
          break;

        case UNLOCK :
          printf("%04d:  UNLOCK\n", i); 
          break;

        case HALT :
          printf("%04d:  HALT\n", i); 
          break;

        case JFALSE :
          printf("%04d:  JFALSE\t %d\n",  i, mem[k][i+1]); i++; 
          break;

        case JMP: 
          printf("%04d:  JMP\t %d\n", i, mem[k][i+1]); i++; 
          break;

        default:
          printf("(%04d:   %d)\n", i, mem[k][i]);  
          break;
      }
    }
  } // end of printing each proc memory

}