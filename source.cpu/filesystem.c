#include <stdio.h>
#include <stdlib.h>

#define FILESYSTEM_SIZE 1000

int filesystem[FILESYSTEM_SIZE];
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
 * 
 */

/**
 * TODO
 * add support ot append to file
 * add interface
 * get stuff from filesystem
 * take info from importmemory in cpu.c and put in firstRun
 */

//run only once, ever. Sets up filesystem parameters
//though a new filesystem will require this to run i suppose
void fs_firstRun(){
 lastFid=0;
 lastFree=0;
 lastFill=0;
 end=0;
 waste=0;
 filesystem[FILESYSTEM_SIZE-1]=waste;
 filesystem[FILESYSTEM_SIZE-2]=end;
 filesystem[FILESYSTEM_SIZE-3]=lastFid;
 filesystem[FILESYSTEM_SIZE-4]=lastFree;
 filesystem[FILESYSTEM_SIZE-5]=lastFill;
}

/**
 * makes sure values are initialized, should be ran instead of firstRun assuming firstRun has already occurred
 * For now used at the beginning of store and list
 */
void fs_init(){
  waste=filesystem[FILESYSTEM_SIZE-1];
  end=filesystem[FILESYSTEM_SIZE-2];
  lastFid=filesystem[FILESYSTEM_SIZE-3];
  lastFree=filesystem[FILESYSTEM_SIZE-4];
  lastFill=filesystem[FILESYSTEM_SIZE-5];
}

/**
 * Stores data into filesystem
 * First version: Just add to filesystem assuming all previous space used **DONE**
 * Second version: Check to see if contigous space available from previously deleted files, then add to end if not, this requires delete to be implemented
 * Third version: Second version with additional check to see if non-contigous space available before adding to end. this requires delete to be implemented
 * Fourth version: defragmenter. This will need to solve the problem of small fragments smaller then the header size of a record
 */
void fs_store(int len, int* data){
  int prev,loc,i;
 
  fs_init();
  
  //fill in meta info first
  if(!lastFill){
    lastFill=FILESYSTEM_SIZE-6;
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-1]=0;//no next
    filesystem[lastFill-2]=lastFid++;
    filesystem[lastFill-3]=0;//since no records start at beginning
    end=lastFill;//last record of meta data
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
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
 
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
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
void fs_addFree(int index){
 
  int loc;
  
  fs_init();
  
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
    filesystem[FILESYSTEM_SIZE-4]=lastFree; 
}

/**
 * Removes an record with fid from filesystem
 * Version1: Removes it then makes record of it in LastFree **Done**
 */
void fs_delete(int fid){
  //in case not initialized already
  fs_init();
 
  int next=0;
  int prev=0;
  int hasPrev=lastFill;
  
  while(hasPrev){
    //check for match
    if(filesystem[hasPrev-2]==fid){
      
      //if lastFill is being removed, set lastFill to prev
//issue
      if(lastFill==lastFree)
	lastFill=filesystem[lastFill];
//end issue
      
      next=filesystem[hasPrev-1];//get next
      prev=filesystem[hasPrev];//get prev
      
      //if there is a next then give it location of new prev
      if(next){
	filesystem[next]=prev;
      }
      
      //if there is a prev then give it location of new next
      if(prev){
	filesystem[prev-1]=next;
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
void fs_list(){
  
  //in case not initialized already
  fs_init();
 
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
 
  printf("SIZE: %d\n",FILESYSTEM_SIZE);
  
  int a[1]={1};
  int b[2]={1,2};
  int c[3]={1,2,3};
  int d[4]={1,2,3,4};
  
  fs_firstRun();
  fs_store(1,a);
  fs_store(2,b);
  fs_store(3,c);
  fs_store(4,d);
  printf("Input test\n");
  fs_list();
  fs_delete(2);
  printf("middle delete test\n");
  fs_list();
  fs_delete(0);
  printf("start delete test\n");
  fs_list();
  fs_delete(3);
  printf("end delete test\n");
  fs_list();  
}