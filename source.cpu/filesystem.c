#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int filesystem[SIZE];
int lastFree;//tracks last free position node
int lastFill;//tracks last filled position node
int lastFid;//tracks last file pointer assigned
int end;//tracks end of meta data

/**
 * Description of structure for record
 * +-----------------------------------------------------------------------------------+
 * |prev 0 if none (1)|next 0 if none(1)|file id(1)|size of data(1)|data(?)|next record|
 * *-----------------------------------------------------------------------------------+
 *  start of record->
 * 
 * Description of structure of meta node
 * +---------------------------------------------------------------------------------------------------------------------------+
 * |next node|record location(1)|fid(1)|next 0 if none(1)|prev 0 if none(1)|last fill loc(1)|last free loc(1)|lastFid(1)|end(1)|
 * +---------------------------------------------------------------------------------------------------------------------------+
 * 					                 <-stat of meta node
 * 
 */

/**
 * TODO
 * Store files
 * list files
 */

//run only once, ever. Sets up filesystem parameters
//though a new filesystem will require this to run i suppose
void firstRun(){
 lastFid=0;
 lastFree=0;
 lastFill=0;
 end=0;
 filesystem[SIZE-1]=end;
 filesystem[SIZE-2]=lastFid;
 filesystem[SIZE-3]=lastFree;
 filesystem[SIZE-4]=lastFill;
}

/**
 * makes sure values are initialized, should be ran instead of firstRun assuming firstRun has already occurred
 * For now used at the beginning of store and list
 */
void init(){
  end=filesystem[SIZE-1];
  lastFid=filesystem[SIZE-2];
  lastFree=filesystem[SIZE-3];
  lastFill=filesystem[SIZE-4];
}

/**
 * Stores data into filesystem
 * First version: Just add to filesystem assuming all previous space used **DONE**
 * Second version: Check to see if contigous space available from previously deleted files, then add to end if not, this requires delete to be implemented
 * Third version: Second version with additional check to see if non-contigous space available before adding to end. this requires delete to be implemented
 * Fourth version: defragmenter. This will need to solve the problem of small fragments smaller then the header size of a record
 * **note**:  
 */
void store(int len, int* data){ 
  int prev,loc,i;
 
  init();
  
  //fill in meta info first
  if(!lastFill){
    lastFill=SIZE-5;
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-1]=0;//no next
    filesystem[lastFill-2]=lastFid++;
    filesystem[lastFill-3]=0;//since no records start at beginning
    end=lastFill;//last record of meta data
    filesystem[SIZE-1]=end;
    filesystem[SIZE-2]=lastFid;
    filesystem[SIZE-3]=lastFill;
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
 
    filesystem[SIZE-1]=end;
    filesystem[SIZE-2]=lastFid;
    filesystem[SIZE-3]=lastFill;
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
 * Removes an record with fid from filesystem
 * Version1: Removes it. That's it
 * Version2: Removes it then makes record of it in LastFree
 */
void delete(int fid){
  
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
  list();
  
}