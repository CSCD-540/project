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
 * Search for IFACE to find interface items
 */

/**
 * Description of structure for record
 * +-----------------------------------------------------------------------------------+
 * |prev 0 if none (1)|next 0 if none(1)|file id(1)|size of data(1)|data(?)|next record|
 * *-----------------------------------------------------------------------------------+
 *  start of record->
 * 
 * Description of structure of meta node
 * +-------------------------------------------------------------------------------------------------------------------------------------+
 * |next node|record location(1)|data(1)|file name(8)|next 0 if none(1)|prev 0 if none(1)|last fill loc(1)|last free loc(1)|lastFid(1)|end(1)|waste(1)|
 * +-------------------------------------------------------------------------------------------------------------------------------------+
 * 				 	                              <-stat of meta node
 * 
 * Data can either be the fid in the case of a fillNode or length in the case of a FreeNode
 * Fill nodes are files currently stored
 * Free nodes are deleted files
 * file name can be 7 characters long as it must be 0 terminated
 * 
 */

/**
 * TODO
 * take info from importmemory in cpu.c and put in firstRun
 * add support ot append to file (need to add to freeList)
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
 * if name==0 then file does not have name
 * First version: Just add to filesystem assuming all previous space used **DONE**
 * Second version: Check to see if contigous space available from previously deleted files, then add to end if not, this requires delete to be implemented
 * Third version: Second version with additional check to see if non-contigous space available before adding to end. this requires delete to be implemented
 * Fourth version: defragmenter. This will need to solve the problem of small fragments smaller then the header size of a record
 */
void fs_store(int len, int* data, int* name){
  int prev,loc,i;
 
  fs_init();
  
  //fill in meta info first
  if(!lastFill){
    lastFill=FILESYSTEM_SIZE-6;
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-1]=0;//no next
    filesystem[lastFill-2]=lastFid++;
    //if name not equal
    if(name){
      int i;
      //copy name
      for(i=3;i<10&&name[i];i++)
	filesystem[lastFill-i]=name[i];
     filesystem[lastFill-i]=0;
    }else
      filesystem[lastFill-3]=0;//terminate
    filesystem[lastFill-11]=0;//since no records start at beginning
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
    loc=filesystem[prev-11];//location of prev data
    filesystem[lastFill-11]=filesystem[loc+3]+4;//size of data plus 4 (for header info)
 
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
 * Note, does not work yet
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
 * Version1: Removes it **Done**
 * Version2: makes record of it in LastFree
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
      if(lastFill==hasPrev){
	lastFill=filesystem[lastFill];
        filesystem[FILESYSTEM_SIZE-5]=lastFill;
      }else{
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
 * Begin interface items
 * This struct should be moved to a different file. Possibly filesystem_intf.h.
 * 
 * fid: The unique identifier for an inode as used by the filesystem
 * filename: The name of the file as a string
 * length: The number of 'words' in the inode, a word is an int in our filesystem
 * 
 */
typedef struct {
  int fid;
  int filename[8];
  int length;
}inode;


/**
 * Returns number of filled inodes
 */
int fs_inodeCount(){
  
  int count=0;
  int hasPrev=lastFill;
  
  while(hasPrev){
    count++;
    hasPrev=filesystem[hasPrev];
  }
  
  return count;
}

/**
 * Returns a list of filled inodes in the filesystem
 * a buffer needs to be supplied
 */
void fs_list_intf(inode* toReturn, int len){
  //in case not initialized already
  fs_init();
 
  int hasPrev=lastFill;
  int dataLoc=0;
  int i;
    
  hasPrev=lastFill;
  while(hasPrev){
    len--;
    if(len<0)
      break;
    toReturn[len].fid=filesystem[hasPrev-2];
    //copy filename
      for(i=3;i<10&&filesystem[hasPrev-i];i++)
	toReturn[len].filename[i]=filesystem[hasPrev-i];
     toReturn[len].filename[i-3]=0;
    
    dataLoc=filesystem[hasPrev-11];//location of data
    toReturn[len].length=filesystem[dataLoc+3];//length of data

    hasPrev=filesystem[hasPrev];
  }
}

/**
 * IFACE
 * Returns data based upon a file id. It takes data from the offset to the length, assuming offset+length < the total file length
 * If offset is greater than file length, it will return 0
 * If offset+length is greater than file length, but offset is less than file length, it will read to the end of the file
 * toReturn is a supplied buffer of maximum length 'length'
 * 
 * Warning, has not been fully tested
 */
void fs_get_intf(int fid, int offset, int length, int* toReturn){

  int hasPrev=lastFill;
  int dataLoc=0;
  int dataLen=0;
  int i;
  
  while(hasPrev){
    //check for match
    if(filesystem[hasPrev-2]==fid){
      dataLoc=filesystem[hasPrev-11];      
      dataLen=filesystem[dataLoc+3];

      //ensure offset is less than file length      
      if(offset>=dataLen)
	toReturn=0;
      
      //ensure sticks within file
      if(offset+length>dataLen)
	length=dataLen-offset;

      //dataLoc location of record
      //4 is the offset where data portion of record is located
      //offset is offset from beginning of record to interested data
      for(i=0;i<length;i++){
	toReturn[i]=filesystem[dataLoc+4+offset+i];
      }
      
      break;
    }    
    hasPrev=filesystem[hasPrev];
  }

  if(hasPrev)
    toReturn=0;
  
}

/**
 * Stores new data to the filesystem, then returns the corresponding inode
 * 
 */
inode fs_store_intf(int* data){

  int len=sizeof(data)/sizeof(data[0]);  //determine length of data
  int* name=0;//no idea how to get this currently, might not use it
  int dataLoc=0;
  int i;
  inode toReturn;
  
  //will always be stored as lastFilled.
  fs_store(len, data, 0);
  
  toReturn.fid=filesystem[lastFill-2];
    //copy filename

    if(name){
    //while i is less than ten, and name[i] is not zero
      for(i=3;i<10&&name[i-3];i++)
	toReturn.filename[i-3]=filesystem[lastFill-i];
      toReturn.filename[i-3]=0;
    }else
      toReturn.filename[0]=0;
    
    dataLoc=filesystem[lastFill-11];//location of data
    toReturn.length=len;//length of data  
    
    return toReturn;
 
}
/**
 * End interface items
 */


/**
 * Tester for the filesystem
 */
/*
void main(){
 
  printf("SIZE: %d\n",FILESYSTEM_SIZE);
  
  int a[1]={1};
  int b[2]={1,2};
  int c[3]={1,2,3};
  int d[4]={1,2,3,4};
  
  fs_firstRun();
  fs_store(1,a,0);
  fs_store(2,b,0);
  fs_store(3,c,0);
  fs_store(4,d,0);
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
  
  int e[5]={1,2,3,4,5};
  int length;
  inode testNode;
  int buf[100];
  
  fs_store_intf(e);
  length=fs_inodeCount();
  inode testArr[length];
  
  fs_list_intf(testArr, length);
  fs_get_intf(1,0,2,buf);
  fs_get_intf(5,1,2,buf); 
}
*/