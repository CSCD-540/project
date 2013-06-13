#include <stdio.h>
#include <stdlib.h>

#define FILESYSTEM_SIZE 1000
#define FS_DEBUG 1

int filesystem[FILESYSTEM_SIZE];
int lastFree;//tracks last free position node
int lastFill;//tracks last filled position node
int lastFid;//tracks last file pointer assigned
int end;//tracks end of meta data
int waste;//tracks wasted space, ie space that is too small for header data, this will be useful for determining when to defragment

/**
 * offset constants
 * All are defined as positive values, so node offsets should be subtracted
 * FS_PREV_NODE_OFFSET or FS_PREV_REC_OFFSET are not used, but they are included in case they are needed in the future
 */
<<<<<<< HEAD
#define FS_PREV_REC_OFFSET 0
#define FS_NEXT_REC_OFFSET 1
#define FS_FID_REC_OFFSET 2
#define FS_SIZE_REC_OFFSET 3
#define FS_DATA_REC_OFFSET 4
#define FS_PREV_NODE_OFFSET 0
#define FS_NEXT_NODE_OFFSET 1
#define FS_NAME_NODE_OFFSET 2
#define FS_NAME_NODE_SIZE 16
#define FS_DATA_NODE_OFFSET (FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)
#define FS_LOCATION_NODE_OFFSET (1+FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)
=======
int fs_findFile(int fid){
  
  int index;
  
  fs_init();
  
  index=lastFill;
  //break out when file is found, or run out of filled nodes
  while(filesystem[index-FS_DATA_NODE_OFFSET]!=fid&&index)
    index=filesystem[index];
  
  if(!index){
    index=-1;
    printf("\nFile not found\n");
  }
  
  return index;
}

/**
 * copies a file to a new one with 'name'
 */
void fs_copy(int fid, char* name){

  //int* buf
  //int* procSizes
  int len, index, procNum, i;
   
  //get file
  index=fs_findFile(fid); 
  
  if(index==-1)
    return;
  //get length
  len=fs_findRecSize(filesystem[index-FS_LOCATION_NODE_OFFSET]);
  
  char buf[len];
  //copy all data to temp array
  fs_getData(fid);
  
  //get proc info
  procNum=fs_getProcessCount(fid);
  
  int procSizes[procNum];
  
  for(i=0;i<procNum;i++)
    procSizes[i]=fs_getProcessSize(fid, i);
    
  //add to filesystem with 'name'
  fs_store(len, buf, name, procNum, procSizes);
}

/**
 * returns the size of a particular process of a file or 0 if process does not exist
 * 
 * process numbers start at 0
 * 
 */
int fs_getProcessSize(int id, int process){
  
  int loc,index;
  
  //get file
  index=fs_findFile(id);
  
    if(index==-1)
    return;
  
  //go to record
  loc=filesystem[index-FS_LOCATION_NODE_OFFSET];

  //retrieve size of process
  if(process<filesystem[loc+FS_PROC_COUNT_REC_OFFSET])
    return filesystem[loc+FS_PROC_VALUES_REC_OFFSET+process];
    
  return 0;
}
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3

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
<<<<<<< HEAD
 * Description of structure of meta node
 * +---------------------------------------------------------------------------------------------------------------------------------------------------+
 * |next node|record location(1)|data(1)|file name(16)|next 0 if none(1)|prev 0 if none(1)|last fill loc(1)|last free loc(1)|lastFid(1)|end(1)|waste(1)|
 * +---------------------------------------------------------------------------------------------------------------------------------------------------+
 * 				 	                               <-stat of meta node
=======
 * Note: currently always returns 1
 */
int* fs_getData(int id){

 
  int index=fs_findFile(id);
  int size=fs_findRecSize(filesystem[index-FS_LOCATION_NODE_OFFSET]);
  
  int* buf=malloc(size*sizeof(int));
  
  fs_get(id, 0, size, buf);
  
  return buf;
  
}

/**
 * Gets a specific page and stores it in buf
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3
 * 
 * Data can either be the fid in the case of a fillNode or length in the case of a FreeNode
 * Fill nodes are files currently stored
 * Free nodes are deleted files
 * file name can be 7 characters long as it must be 0 terminated
 * 
 */
<<<<<<< HEAD

/**
 * TODO
 * change inode.filename to char* instead of int* also make 16 characters
 * take info from importmemory in cpu.c and put in firstRun
 * add support ot append to file (need to add to freeList)
 * move interface items to different file
=======
int* fs_getPage(int id, int process, int start, int size){
 
  int procOffset=0, index, loc, i;
  int* buf=malloc(size*sizeof(int));
  
  //get file
  index=fs_findFile(id);

    if(index==-1)
      return;

  //get record
  loc=filesystem[index-FS_LOCATION_NODE_OFFSET];
  
  //get process offset by parsing process section of meta data
  for(i=0;i<FS_PROC_COUNT_REC_OFFSET&&i<process;i++)
    procOffset+=filesystem[loc+FS_PROC_VALUES_REC_OFFSET+i];
  
  fs_get(id, procOffset+start, size, buf);

return buf;
  
}


/**
 * Imports file from host filesystem to our virtual file system.
 * 
 * returns id of created file
 */
int fs_import(char* path, char* name){

  fs_init();
  
  int len=1000;
  int size=0;
  char temp[len];
  //char data[size];
  int i;
  FILE *fout=fopen(path, "r");
  int procSize[1];

  //start by getting size
  while(!feof(fout)){
    fgets(temp, len, fout);
    size+=strlen(temp);
  }
  
  char data[size];
  size=0;
  
  //go to beginning of file
  rewind(fout);
  
  //fill data
  while(!feof(fout)){
    
    fgets(temp, len, fout);
    for(i=0;i<strlen(temp);i++)
      data[size+i]=temp[i];
    size+=strlen(temp);
  }
  
  procSize[0]=size;
  
  fs_store(size, data, name, 1, procSize);
  
  fclose(fout);

  //since was just created, it will be lastFill
  return filesystem[lastFill-FS_DATA_NODE_OFFSET];
}

/**
 * Creates a nameless file then returns start of record data
 * 
 * Note: I was not told that this would be needed. I only added it for completeness sake.
 */
int fs_addData(int size, int* data){
  int procSize[1]={size};
  int loc;
    fs_store(size, data, 0, 1, procSize);

  loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];    
  
  return loc+FS_DATA_REC_OFFSET;
    
}

/**
 * Deletes all files
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3
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
    lastFill=FILESYSTEM_SIZE-6;//first item of metadata if lastFill is not set
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-FS_NEXT_NODE_OFFSET]=0;//no next
    filesystem[lastFill-FS_DATA_NODE_OFFSET]=lastFid++;
    loc=0;
    //if name not equal
    if(name){
      int i;
      //copy name
      for(i=FS_NAME_NODE_OFFSET;i<FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE&&name[i];i++)
	filesystem[lastFill-i]=name[i];
     filesystem[lastFill-i]=0;
    }else
      filesystem[lastFill-FS_NAME_NODE_OFFSET]=0;//terminate
    filesystem[lastFill-FS_LOCATION_NODE_OFFSET]=0;//since no records, start at beginning
    end=lastFill;//last record of meta data
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
  }else{
    filesystem[lastFill-1]=end-(FS_LOCATION_NODE_OFFSET+1);//next of prev
    filesystem[end-(FS_LOCATION_NODE_OFFSET+1)]=lastFill;//prev
    lastFill=end-(FS_LOCATION_NODE_OFFSET+1);//meta record is 12 bytes long  
    end=lastFill;
    filesystem[lastFill-FS_NEXT_NODE_OFFSET]=0;//no next
    filesystem[lastFill-FS_DATA_NODE_OFFSET]=lastFid++;
    
    prev=filesystem[lastFill];
    loc=filesystem[prev-FS_LOCATION_NODE_OFFSET];//location of prev data
    filesystem[lastFill-FS_LOCATION_NODE_OFFSET]=filesystem[loc+3]+4+loc;//size of prev data plus 4 (for header info)+location of prev data
    
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
  }
  
  loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];//set loc to data will be currently filling
  //prev and next are 0 at this point. They are only used when fragmenting
  filesystem[loc]=0;
  filesystem[loc+FS_NEXT_REC_OFFSET]=0;
  filesystem[loc+FS_FID_REC_OFFSET]=filesystem[lastFill-FS_DATA_NODE_OFFSET];//set fid
  filesystem[loc+FS_SIZE_REC_OFFSET]=len;
  //file in data
  for(i=0;i<len;i++){
    filesystem[loc+FS_DATA_REC_OFFSET+i]=data[i];
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
    filesystem[index-FS_NEXT_NODE_OFFSET]=0;//no next
  }else{
    filesystem[lastFree-FS_NEXT_NODE_OFFSET]=index;//next of prev
    filesystem[index]=lastFree;//prev
    filesystem[index-FS_NEXT_NODE_OFFSET]=0;//next
  }
   
    lastFree=index;
    loc=filesystem[lastFree-FS_LOCATION_NODE_OFFSET];
    filesystem[lastFree-FS_DATA_NODE_OFFSET]=filesystem[loc+FS_SIZE_REC_OFFSET];//size of available free space (minus header info) 
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
    if(filesystem[hasPrev-FS_DATA_NODE_OFFSET]==fid){
      
      //if lastFill is being removed, set lastFill to prev

      if(lastFill==hasPrev){
	lastFill=filesystem[lastFill];
        filesystem[FILESYSTEM_SIZE-5]=lastFill;
      }else{

	next=filesystem[hasPrev-FS_NEXT_NODE_OFFSET];//get next
	prev=filesystem[hasPrev];//get prev
	
	//if there is a next then give it location of new prev
	if(next){
	  filesystem[next]=prev;
	}
	
	//if there is a prev then give it location of new next
	if(prev){
	  filesystem[prev-FS_NEXT_NODE_OFFSET]=next;
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
    printf("FID: %d\n",filesystem[hasPrev-FS_DATA_NODE_OFFSET]);
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
<<<<<<< HEAD
typedef struct {
  int fid;
  int filename[FS_NAME_NODE_SIZE];
  int length;
}inode;

=======
void fs_ls(){
  
  //in case not initialized already
  fs_init();
 
  int hasPrev=lastFill;
  int i;
  
  printf("\nFilled list:\n");
  while(hasPrev){
    printf("Name: ");
    for(i=0;filesystem[hasPrev-FS_NAME_NODE_OFFSET-i]&&i<FS_NAME_NODE_SIZE;i++)
      printf("%c", filesystem[hasPrev-FS_NAME_NODE_OFFSET-i]);
    
    printf(" FID: %d\n",filesystem[hasPrev-FS_DATA_NODE_OFFSET]);
    hasPrev=filesystem[hasPrev];
  }
  
  hasPrev=lastFree;
  printf("Free list:\n");
  while(hasPrev){
    printf("Size: %d\n",filesystem[hasPrev-FS_DATA_NODE_OFFSET]);
    hasPrev=filesystem[hasPrev];
  }
}
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3

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
void fs_list_intf(inode* buf, int len){
  //in case not initialized already
  fs_init();
 
  int hasPrev=lastFill;
<<<<<<< HEAD
  int dataLoc=0;
  int i,j;
    
  j=0;
=======
  int j,id;
  int len=fs_getINodeCount();
  INode temp;
  
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3
  hasPrev=lastFill;
  while(hasPrev&&j<len){
    len--;
    if(len<0)
      break;
    buf[len].fid=filesystem[hasPrev-FS_DATA_NODE_OFFSET];
    //copy filename
      for(i=FS_NAME_NODE_OFFSET;i<(FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)&&filesystem[hasPrev-i];i++)
	buf[len].filename[i]=filesystem[hasPrev-i];
     buf[len].filename[i-FS_NAME_NODE_OFFSET]=0;
    
<<<<<<< HEAD
    dataLoc=filesystem[hasPrev-FS_LOCATION_NODE_OFFSET];//location of data
    buf[len].length=filesystem[dataLoc+FS_SIZE_REC_OFFSET];//length of data
=======
    buf[j]=*fs_getNode(id);
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3

    hasPrev=filesystem[hasPrev];
    j++;
  }
}

/**
<<<<<<< HEAD
 * IFACE
=======
 * Returns an INode with a fid of id
 */
INode* fs_getNode(int id){
  
  INode *toReturn=malloc(sizeof(INode));
  int index, loc, i;
  
  index=fs_findFile(id);
  
  if(index==-1)
    return 0;
  
  loc=filesystem[index-FS_LOCATION_NODE_OFFSET];
  
  toReturn->id=filesystem[index-FS_DATA_NODE_OFFSET];
  
  toReturn->name=malloc(FS_NAME_NODE_SIZE*sizeof(char *));
  
  //copy filename
  for(i=0;i<FS_NAME_NODE_SIZE&&filesystem[index-i];i++)
    toReturn->name[i]=filesystem[index-i-FS_NAME_NODE_OFFSET];
  toReturn->name[i]=0;
    
  toReturn->fileSize=filesystem[loc+FS_SIZE_REC_OFFSET];//length of data  
  toReturn->fileStart=loc;
  
  toReturn->processes=fs_getProcessCount(id);
  
  //first one is special
  toReturn->processStart[0]=loc+FS_DATA_REC_OFFSET;
  
  for(i=0;i<toReturn->processes;i++){
    if(i>0)
      toReturn->processStart[i]=loc+FS_DATA_REC_OFFSET+toReturn->processSize[i-1];
    toReturn->processSize[i]=filesystem[loc+FS_PROC_VALUES_REC_OFFSET+i];
  }
 
 return toReturn;
 
}

/**
 * Takes a file and adds it to the database then returns the file id.
 */
int fs_addFile(char* name, int fileSize, int processes, int* processStart, int* processSize, int* data){
  
  fs_init();
  //not sure what to do with processStart I don't think I need it  
  fs_store(fileSize, data, name, processes, processSize);
  
  //id will be the lastFill-FS_DATA_NODE_OFFSET since it was the last filled
  return filesystem[lastFill-FS_DATA_NODE_OFFSET];
  
}


/**
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3
 * Returns data based upon a file id. It takes data from the offset to the length, assuming offset+length < the total file length
 * If offset is greater than file length, it will return 0
 * If offset+length is greater than file length, it will return 0
 * If fid not found, it will return 0
 * toReturn is a supplied buffer of maximum length 'length'
 * 
 * Warning, has not been fully tested
 */
<<<<<<< HEAD
void fs_get_intf(int fid, int offset, int length, int* toReturn){
=======
void fs_get(int fid, int offset, int length, int* toReturn){
>>>>>>> 5872d04b096ff50662781d9780a64084dfbf62f3

  int hasPrev=lastFill;
  int dataLoc=0;
  int dataLen=0;
  int i;
  
  while(hasPrev){
    //check for match
    if(filesystem[hasPrev-FS_DATA_NODE_OFFSET]==fid){
      dataLoc=filesystem[hasPrev-FS_LOCATION_NODE_OFFSET];      
      dataLen=filesystem[dataLoc+FS_SIZE_REC_OFFSET];

      //ensure offset is less than file length      
      if(offset>=dataLen){
	toReturn=0;
	length=0;
      }
      
      //ensure sticks within file
      if(offset+length>dataLen){
	toReturn=0;
	length=0;
      }
      //dataLoc location of record
      //4 is the offset where data portion of record is located
      //offset is offset from beginning of record to interested data
      for(i=0;i<length;i++){
	toReturn[i]=filesystem[dataLoc+FS_DATA_REC_OFFSET+offset+i];
	printf("\n%d\n", toReturn[i]);
      }
      
      break;
    }    
    hasPrev=filesystem[hasPrev];
  }

  if(!hasPrev)
    toReturn=0;
  
}

/**
 * Stores new data to the filesystem, then returns the corresponding inode
 * 
 */
inode fs_store_intf(int* data, int len){

  int* name=0;//no idea how to get this currently, might not use it
  int dataLoc=0;
  int i;
  inode toReturn;
  
  //will always be stored as lastFilled.
  fs_store(len, data, 0);
  
  toReturn.fid=filesystem[lastFill-FS_DATA_NODE_OFFSET];
    //copy filename

    if(name){
    //while i is less than ten, and name[i] is not zero
      for(i=FS_NAME_NODE_OFFSET;i<(FS_NAME_NODE_OFFSET+FS_NAME_NODE_SIZE)&&name[i-3];i++)
	toReturn.filename[i-FS_NAME_NODE_OFFSET]=filesystem[lastFill-i];
      toReturn.filename[i-FS_NAME_NODE_OFFSET]=0;
    }else
      toReturn.filename[0]=0;
    
    dataLoc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];//location of data
    toReturn.length=len;//length of data  
    
    return toReturn;
 
}

void fs_dumpAllData(){
  int i;
  for(i=0;i<FILESYSTEM_SIZE;i++)
    printf("%4d ", filesystem[i]);
}

/**
 * End interface items
 */



/**
 * Tester for the filesystem
 */
void fs_test(){
//void main(){
  
  printf("SIZE: %d\n",FILESYSTEM_SIZE);
  
  int a[1]={1};
  int b[2]={1,2};
  int c[3]={1,2,3};
  int d[4]={1,2,3,4};
  
  fs_firstRun();
  fs_store(1,a,0);
  fs_store(2,b,0);
  printf("\nfs_store(2,b,0)\n");
  fs_dump();
  fs_store(3,c,0);
    printf("\nfs_store(3,c,0);\n");
  fs_dump();
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
  
  fs_store_intf(e, 5);

  fs_dump();

  length=fs_inodeCount();
  inode testArr[length];
  
  fs_list_intf(testArr, length);
  printf("testArr.fid=%d\ntestArr.filename=%p\ntestArr.length=%d\n", testArr[0].fid, testArr[0].filename, testArr[0].length);
  
  
  fs_get_intf(1,0,1,buf);
  int i;
  for(i=0;i<1;i++)
    printf("%d ", buf[i]);
  printf("\n");
  fs_get_intf(4,1,3,buf);
  for(i=0;i<3;i++)
    printf("%d ", buf[i]);
  printf("\n");
 
}

