#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int filesystem[SIZE];
int lastFree;//tracks last free position node
int lastFill;//tracks last filled position node
int lastFid;//tracks last file pointer assigned
int end;//tracks end of meta data
int waste;//tracks wasted space, ie space that is too small for header data, this will be useful for determining when to defragment

/**
 * Description of structure for record
 * +-----------------------------------------------------------------------------------+
 * |prev 0 if none (1)|next 0 if none(1)|file id(1)|size of data(1)|data(?)|next record|
 * *-----------------------------------------------------------------------------------+
 *  start of record->
 * 
 * Description of structure of meta node
 * +-------------------------------------------------------------------------------------------------------------------------------------+
 * |next node|record location(1)|data(1)|next 0 if none(1)|prev 0 if none(1)|last fill loc(1)|last free loc(1)|lastFid(1)|end(1)|waste(1)|
 * +-------------------------------------------------------------------------------------------------------------------------------------+
 * 				 	                   <-stat of meta node
 * 
 * Data can either be the fid in the case of a fillNode or length in the case of a FreeNode
 * Fill nodes are files currently stored
 * Free nodes are deleted files
 */

/**
 * TODO
 * add support ot append to file
 */

//run only once, ever. Sets up filesystem parameters
//though a new filesystem will require this to run i suppose
void firstRun(){
 lastFid=0;
 lastFree=0;
 lastFill=0;
 end=0;
 waste=0;
 filesystem[SIZE-1]=waste;
 filesystem[SIZE-2]=end;
 filesystem[SIZE-3]=lastFid;
 filesystem[SIZE-4]=lastFree;
 filesystem[SIZE-5]=lastFill;
}

/**
 * makes sure values are initialized, should be ran instead of firstRun assuming firstRun has already occurred
 * For now used at the beginning of store and list
 */
void init(){
  waste=filesystem[SIZE-1];
  end=filesystem[SIZE-2];
  lastFid=filesystem[SIZE-3];
  lastFree=filesystem[SIZE-4];
  lastFill=filesystem[SIZE-5];
}

/**
 * Stores data into filesystem
 * First version: Just add to filesystem assuming all previous space used **DONE**
 * Second version: Check to see if contigous space available from previously deleted files, then add to end if not, this requires delete to be implemented
 * Third version: Second version with additional check to see if non-contigous space available before adding to end. this requires delete to be implemented
 * Fourth version: defragmenter. This will need to solve the problem of small fragments smaller then the header size of a record
 */
void store(int len, int* data){ 
  int prev,loc,i;
 
  init();
  
  //fill in meta info first
  if(!lastFill){
    lastFill=SIZE-6;
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-1]=0;//no next
    filesystem[lastFill-2]=lastFid++;
    filesystem[lastFill-3]=0;//since no records start at beginning
    end=lastFill;//last record of meta data
    filesystem[SIZE-2]=end;
    filesystem[SIZE-3]=lastFid;
    filesystem[SIZE-5]=lastFill;
  }else{
    filesystem[lastFill-1]=end-4;//next of prev
    filesystem[end-4]=lastFill;//prev
    lastFill=end-4;//meta record is 4 bytes long  
    end=lastFill;
    filesystem[lastFill-1]=0;//no next
    filesystem[lastFill-2]=lastFid++;
    
    prev=filesystem[lastFill];
    loc=filesystem[prev-3];//location of prev data
    filesystem[lastFill-3]=filesystem[loc+3]+4;//size of data plus 4 (for header info)
 
    filesystem[SIZE-2]=end;
    filesystem[SIZE-3]=lastFid;
    filesystem[SIZE-5]=lastFill;
  }
    
  //add to beginning of filesystem
  loc=filesystem[lastFill-3];//location according to meta data
  //prev and next are 0 at this point. They are only used when fragmenting
  filesystem[loc]=0;
  filesystem[loc+1]=0;
  filesystem[loc+2]=filesystem[lastFill-2];//set fid
  filesystem[loc+3]=len;
  //file in data
  for(i=0;i<len;i++){
    filesystem[loc+4+i]=data[i];
  }
  
}

/**
 * This will add a node to the free list. 
 * 
 * Since a free node is taken from a fill node, 'end' should not change
 */
void addFree(int index){
 
  int loc;
  
  init();
  
  if(!lastFree){
    filesystem[index]=0;//no prev
    filesystem[index-1]=0;//no next
  }else{
    filesystem[lastFree-1]=index;//next of prev
    filesystem[index]=lastFree;//prev
    filesystem[index-1]=0;//next
  }
   
    lastFree=index;
    loc=filesystem[lastFree-3];
    filesystem[lastFree-2]=filesystem[loc+3];//size of available free space (minus header info) 
    filesystem[SIZE-4]=lastFree; 
}

/**
 * Removes an record with fid from filesystem
 * Version1: Removes it then makes record of it in LastFree **Done**
 */
void delete(int fid){
  //in case not initialized already
  init();
 
  int next=0;
  int prev=0;
  int hasPrev=lastFill;
  
  while(hasPrev){
    //check for match
    if(filesystem[hasPrev-2]==fid){

      //make new lastFree
      addFree(hasPrev);
      
      //if lastFill is being removed, set lastFill to prev
      if(lastFill==lastFree)
	lastFill=filesystem[lastFill];
      
      next=filesystem[hasPrev-1];//get next
      prev=filesystem[hasPrev];//get prev
      //if there is a next then give it location of new prev
      if(next){
	filesystem[next]=filesystem[prev];
      }
      //if there is a prev then give it location of new next
      if(prev){
	filesystem[prev]=filesystem[next];
      }
      break;
    }
    hasPrev=filesystem[hasPrev];
  }
  
}


/**
 * Version 1:List fids of files
 * Version 2:???
 */
void list(){
  
  //in case not initialized already
  init();
 
  int hasPrev=lastFill;
  
  while(hasPrev){
    printf("FID: %d\n",filesystem[hasPrev-2]);
    hasPrev=filesystem[hasPrev];
  }
}

/**
 * Tester for the filesystem
 */
void main(){
 
  printf("SIZE: %d\n", SIZE);
  
  int a[1]={1};
  int b[2]={1,2};
  int c[3]={1,2,3};
  int d[4]={1,2,3,4};
  
  firstRun();
  store(1,a);
  store(2,b);
  store(3,c);
  store(4,d);
  printf("Input test\n");
  list();
  delete(2);
  printf("middle delete test\n");
  list();
  delete(1);
  printf("start delete test\n");
  list();
  delete(4);
  printf("end delete test\n");
  list();  
}