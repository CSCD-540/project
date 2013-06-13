#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

/**
 * Description of structure for record
 * +------------------------------------------------------------------------------------------------------------------------+
 * |prev -1 if none (1)|next -1 if none(1)|file id(1)|size of data(1)|process Count(1)|process sizes (6)|data(?)|next record|
 * *------------------------------------------------------------------------------------------------------------------------+
 *  start of record->
 * 
 * Description of structure of meta node
 * +-------------------------------------------------------------------------------------------------------------------------------------------------------+
 * |next node|record location(1)|data(1)|file name(16)|next 0 if none(1)|prev 0 if none(1)|last fill loc(1)|last free loc(1)|lastFid(1)|end(1)|freespace(1)|
 * +-------------------------------------------------------------------------------------------------------------------------------------------------------+
 * 				 	                               <-stat of meta node
 * 
 * Data can either be the fid in the case of a fillNode or length in the case of a FreeNode
 * Fill nodes are files currently stored
 * Free nodes are deleted files
 * file name can be 15 characters long as it must be 0 terminated
 * End
 */


/**
 * TODO
 * move interface items to different file
 * change inode.filename to char* instead of int* also make 16 characters
 */

/**
 * Returns the index of a file with fid (metadata indexm, not record index)
 * or -1 on fail
 */
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

/**
 * returns the process count of a particular file
 */
int fs_getProcessCount(int id){
   
  int index, loc;
  
  
  //get file
    index=fs_findFile(id);
    
    if(index==-1)
      return;
    
  //get record
    loc=filesystem[index-FS_LOCATION_NODE_OFFSET];    
    //retrieve count
    return filesystem[loc+FS_PROC_COUNT_REC_OFFSET];
    
}

/**
 * Gets all the data from a particular file and stores it in buf
 * 
 * Returns 1 on success
 * else returns 0
 * 
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
 * 
 * returns 1 on sucess
 * else 0
 * 
 * Note: currently always returns 1
 */
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
  int numOfProc;
  char temp[len];
  //char data[size];
  
  int i,j,val;
  
  FILE *fout=fopen(path, "r");
  //int procSize[numOfProc];

  //get number of processes
  fscanf(fout, "%s %d \n", temp, &numOfProc);
  
  int procSize[numOfProc];
  
  //get process sizes
  fscanf(fout, "%s ", temp);
  for(i=0;i<numOfProc;i++){
    fscanf(fout, "%d ", &j);
    procSize[i]=j+1;
    size+=procSize[i];
  }
  fscanf(fout, "\n");
  
  int data[size];
  size=0;
  
  //fill data
  for(i=0;i<numOfProc;i++){
    for(j=0;j<procSize[i];j++){
      fscanf(fout, "%d", &val);
      data[size+j]=val;
    }
    size+=procSize[i];
    fscanf(fout, "\n");
  }

  fclose(fout);
    
    fs_store(size, data, name, numOfProc, procSize);
  


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
 */
void fs_removeAllFiles(){
  int curr;
  
  fs_init();
  
  curr=lastFill;
  
  while(curr){
    fs_removeFile(filesystem[curr-FS_DATA_NODE_OFFSET]);
    curr=filesystem[curr];
  }
}

//run only once, ever. Sets up filesystem parameters
//though a new filesystem will require this to run i suppose
void fs_initialize(){
 lastFid=0;
 lastFree=0;
 lastFill=0;
 end=0;
 freeSpace=FILESYSTEM_SIZE-5;
 filesystem[FILESYSTEM_SIZE-1]=freeSpace;
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
  freeSpace=filesystem[FILESYSTEM_SIZE-1];
  end=filesystem[FILESYSTEM_SIZE-2];
  lastFid=filesystem[FILESYSTEM_SIZE-3];
  lastFree=filesystem[FILESYSTEM_SIZE-4];
  lastFill=filesystem[FILESYSTEM_SIZE-5];
}

/**
 * Stores data into filesystem
 * 
 * if name==0 then file does not have name
 *
 *len is length of data, name is name of file
 * 
 * First version: Just add to filesystem assuming all previous space used **DONE**
 * Second version: Check to see if space at end, if not fragment **POSTPONED**
 * Third version: Check to see if contigous space available from previously deleted files, then add to end if not, this requires delete to be implemented **DONE**
 * Fourth version: Second version with additional check to see if non-contigous space available before adding to end. this requires delete to be implemented **POSTPONED**
 * Fifth version: defragmenter. This will need to solve the problem of small fragments smaller then the header size of a record **DONE**
 * Sixth version: add support for processes
 */
void fs_store(int len, void* data, char* name, int numOfProc, int* procSizes){
  int prev,loc,i,overflow,frontOffset;
 
  fs_init();
  
  //fill in meta info first
  if(!lastFill){
    lastFill=FILESYSTEM_SIZE-6;//first item of metadata if lastFill is not set
    filesystem[lastFill]=0;//no prev
    filesystem[lastFill-FS_NEXT_NODE_OFFSET]=0;//no next
    filesystem[lastFill-FS_DATA_NODE_OFFSET]=lastFid++;
    loc=0;
    filesystem[loc]=-1;
    filesystem[loc+FS_NEXT_REC_OFFSET]=-1;

    //if name not equal to null
    if(name){
      int i;
      //copy name
      for(i=0;i<FS_NAME_NODE_SIZE&&name[i];i++)
	filesystem[lastFill-FS_NAME_NODE_OFFSET-i]=name[i];
     filesystem[lastFill-FS_NAME_NODE_OFFSET-i]=0;
    }else
      filesystem[lastFill-FS_NAME_NODE_OFFSET]=0;//terminate
      
    filesystem[loc+FS_SIZE_REC_OFFSET]=len;
    
    //input proc information
    filesystem[loc+FS_PROC_COUNT_REC_OFFSET]=numOfProc;
    for(i=0;i<numOfProc;i++)
      filesystem[loc+FS_PROC_VALUES_REC_OFFSET+i]=procSizes[i];
      
    //copy data
    for(i=0;i<len;i++)
      filesystem[loc+FS_DATA_REC_OFFSET+i]=*(((int*)data)+i);
      
    filesystem[lastFill-FS_LOCATION_NODE_OFFSET]=0;//since no records, start at beginning
    end=lastFill;//last record of meta data
    freeSpace-=(len+FS_LOCATION_NODE_OFFSET+FS_DATA_REC_OFFSET+1);
    
    filesystem[FILESYSTEM_SIZE-1]=freeSpace;
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
    
  }else{
      //make sure enouch space at end to insert another file
    loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];
    frontOffset=loc+filesystem[loc+FS_SIZE_REC_OFFSET]+FS_DATA_REC_OFFSET;
    overflow=((lastFill-FS_LOCATION_NODE_OFFSET-FS_LOCATION_NODE_OFFSET-1)-(len+FS_DATA_REC_OFFSET+frontOffset));
    if(overflow>=0)
      fs_nonFragment(len, data, name, numOfProc, procSizes);
    //else  if((FS_LOCATION_NODE_OFFSET+len+FS_DATA_REC_OFFSET)<(freeSpace-(lastFill-(FS_LOCATION_NODE_OFFSET-frontOffset))))  //fragment file
      //fs_fragment(len, data, name);
    else 
      printf("\nInsertion failed\n");//abort insertion
    
  }
  //check to see if need to be defraged
  if(fs_defragCheck())
    printf("\nWarning: Filesystem dangerously full. Insertions might fail\n");
}

/**
 * if no fragment, store using this
 */
void fs_nonFragment(int len, void* data, char* name, int numOfProc, int* procSizes){
  
  int toReturn, overflow, loc, prev, i;

  loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];//set loc to data will be currently filling

    //start with metadata
    filesystem[lastFill-1]=end-(FS_LOCATION_NODE_OFFSET+1);//next of prev
    filesystem[end-(FS_LOCATION_NODE_OFFSET+1)]=lastFill;//prev
    lastFill=end-(FS_LOCATION_NODE_OFFSET+1);
    end=lastFill;
    filesystem[lastFill-FS_NEXT_NODE_OFFSET]=0;//no next
    filesystem[lastFill-FS_DATA_NODE_OFFSET]=lastFid++;
    if(name){
      int i;
      //copy name
      for(i=0;i<FS_NAME_NODE_SIZE&&name[i];i++)
	filesystem[lastFill-FS_NAME_NODE_OFFSET-i]=name[i];
      filesystem[lastFill-FS_NAME_NODE_OFFSET-i]=0;
    }else
      filesystem[lastFill-FS_NAME_NODE_OFFSET]=0;//terminate
      
    prev=filesystem[lastFill];
    loc=filesystem[prev-FS_LOCATION_NODE_OFFSET];//location of prev data
    filesystem[lastFill-FS_LOCATION_NODE_OFFSET]=filesystem[loc+FS_SIZE_REC_OFFSET]+FS_DATA_REC_OFFSET+loc ;//size of prev data plus data rec offset (for header info)+location of prev data
    
    loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];//setting loc for next entry
    
    filesystem[FILESYSTEM_SIZE-2]=end;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-5]=lastFill;
    
    //prev and next are -1 at this point. They are only used when fragmenting
    filesystem[loc]=-1;
    filesystem[loc+FS_NEXT_REC_OFFSET]=-1;
    filesystem[loc+FS_FID_REC_OFFSET]=filesystem[lastFill-FS_DATA_NODE_OFFSET];//set fid
    filesystem[loc+FS_SIZE_REC_OFFSET]=len;
    
     //setting proc info
    filesystem[loc+FS_PROC_COUNT_REC_OFFSET]=numOfProc;
    for(i=0;i<numOfProc;i++)
      filesystem[loc+FS_PROC_VALUES_REC_OFFSET+i]=procSizes[i];

    //fill in data
    for(i=0;i<len;i++){
      filesystem[loc+FS_DATA_REC_OFFSET+i]=*(((int*)data)+i);
    }
    //adjust freespace
    freeSpace-=(len+FS_LOCATION_NODE_OFFSET+FS_DATA_REC_OFFSET+1);
    filesystem[FILESYSTEM_SIZE-1]=freeSpace;
    //else file may need to be fragmented    

}

/**
 *Fragments files until done,
 * NOTE: NOT WORKING
 */
void fs_fragment(int len, int* data, int* name){

    int tempFID=lastFid++;
    int pos=0;//position in data
    int prev=-1;
    int next=-1;
    int loc, i;
  
      //will fill free nodes until entire file is read or out of space
    //each free node is a "partition" if there is extra space, it is wasted
    while(len>0&&lastFree){
      //insert data into record
      loc=filesystem[lastFree-FS_LOCATION_NODE_OFFSET];
    
      filesystem[loc]=prev;//set prev rec location for this file
      filesystem[loc+FS_NEXT_REC_OFFSET]=-1;
      //only set next for previous item, if it exists
      if(prev>-1)
	filesystem[prev+FS_NEXT_REC_OFFSET]=loc;//set next for previous item
	
      filesystem[loc+FS_FID_REC_OFFSET]=tempFID;//set fid
      filesystem[lastFill-FS_DATA_NODE_OFFSET]=tempFID;//set fid
      
      //adjust size if necessary
      if(filesystem[lastFree-FS_DATA_NODE_OFFSET]>len)
	filesystem[loc+FS_SIZE_REC_OFFSET]=len;
      
      //fill in data
      for(i=0;i<filesystem[loc+FS_SIZE_REC_OFFSET];i++){
	filesystem[loc+FS_DATA_REC_OFFSET+i]=data[pos+i];
      }
      
      len-=filesystem[lastFree-FS_DATA_NODE_OFFSET];
      pos+=filesystem[lastFree-FS_DATA_NODE_OFFSET];
      
      //only transfer node to fill list first time
      if(prev==-1)
	fs_addFill(filesystem[lastFree]);

      //remove item from freelist
      lastFree=filesystem[lastFree];
      //set prev
      prev=loc;
    }
    
    filesystem[FILESYSTEM_SIZE-4]=lastFree;
    
}

/**
 * This will add a node to the free list. 
 * 
 * Since a free node is taken from a fill node, 'end' should not change
 * Note, does not take into consideration multiple partitions
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
    filesystem[index-FS_NEXT_NODE_OFFSET]=0;//no next
  }
   
    lastFree=index;
    loc=filesystem[lastFree-FS_LOCATION_NODE_OFFSET];
    filesystem[lastFree-FS_DATA_NODE_OFFSET]=filesystem[loc+FS_SIZE_REC_OFFSET];//size of available free space (minus header info) 
    //update freespace
    freeSpace+=filesystem[lastFree-FS_DATA_NODE_OFFSET];
    
    filesystem[FILESYSTEM_SIZE-4]=lastFree;
    filesystem[FILESYSTEM_SIZE-1]=freeSpace;
}

/**
 * This will add a node to the fill list. can probably be combined with fs_addFree(int)
 * 
 * Since a free node is taken from a fill node, 'end' should not change
 * Note, does not work yet
 */
void fs_addFill(int index){
 
  int loc;
  
  fs_init();
  
  if(!lastFill){
    filesystem[index]=0;//no prev
    filesystem[index-FS_NEXT_NODE_OFFSET]=0;//no next
  }else{
    filesystem[lastFill-FS_NEXT_NODE_OFFSET]=index;//next of prev
    filesystem[index]=lastFill;//prev
    filesystem[index-FS_NEXT_NODE_OFFSET]=0;//no next
  }
   
    lastFill=index;
    loc=filesystem[lastFill-FS_LOCATION_NODE_OFFSET];

    //adjust free space available
    freeSpace-=filesystem[loc+FS_SIZE_REC_OFFSET];
    
    filesystem[lastFill-FS_DATA_NODE_OFFSET]=lastFid++; 
    filesystem[FILESYSTEM_SIZE-4]=lastFill;
    filesystem[FILESYSTEM_SIZE-3]=lastFid;
    filesystem[FILESYSTEM_SIZE-1]=freeSpace;
}

/**
 * Checks to see if filesystem needs to be defragged, and does so if needed.
 * 
 * If filesystem is near compacity, it will return 1, else 0
 */
int fs_defragCheck(){
  
  fs_init();
  
  int defragLimit=FILESYSTEM_SIZE-(DEFRAG*FILESYSTEM_SIZE);//defrag < 1 
  int dangerZone=FILESYSTEM_SIZE-((defragLimit+FILESYSTEM_SIZE)/2); //halfway between defrag and filesystem size is considered "near full"
  int toReturn=0;
  
  if(freeSpace<defragLimit){
    fs_defrag();
    if(freeSpace<dangerZone)
      toReturn=1;
  }
  
  return toReturn;
  
}

/**
 * "defrags" by grouping items in memory, then compacting.
 */
void fs_defrag(){
  //int ** items;
  int** metaData;
  int** data;
  int count;
  int hasPrev;
  int firstPos=FILESYSTEM_SIZE, lastPos=0;
  int prevFirstPos, prevLastPos;
  int i,j,k, loc;
  
  fs_init();
  
  hasPrev=lastFill;
    
  //count number of items, and get greatest, least nodes
  for(count=0;hasPrev;count++){
    if(filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]<=filesystem[firstPos-FS_LOCATION_NODE_OFFSET])
      firstPos=hasPrev;
    if(filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]>=filesystem[lastPos-FS_LOCATION_NODE_OFFSET])
      lastPos=hasPrev;    
    hasPrev=filesystem[hasPrev];
  }
  
  int items[count][2];
  
  //insert greatest and least items, and their sizes
  items[0][0]=firstPos;
  items[0][1]=fs_findRecSize(filesystem[firstPos-FS_LOCATION_NODE_OFFSET]);
  items[count-1][0]=lastPos;
  items[count-1][1]=fs_findRecSize(filesystem[lastPos-FS_LOCATION_NODE_OFFSET]);

  prevFirstPos=firstPos;
  prevLastPos=lastPos;

  //copy record metadata from filesystem
  metaData=malloc(count*sizeof(int*));

  //can do this upfront as the size of all metadata is the same
  for(i=0;i<count;i++)
    metaData[i]=malloc(FS_DATA_REC_OFFSET*sizeof(int));
  
  fs_getRecMetaData(filesystem[firstPos-FS_LOCATION_NODE_OFFSET], metaData[0], FS_DATA_REC_OFFSET);
  fs_getRecMetaData(filesystem[lastPos-FS_LOCATION_NODE_OFFSET],metaData[count-1], FS_DATA_REC_OFFSET);
  
  //copy data from filesystem
  data=malloc(count*sizeof(int*)); 
  data[0]=malloc(sizeof(int)*items[0][1]);
  data[count-1]=malloc(sizeof(int)*items[count-1][1]);

  fs_getRecData(filesystem[firstPos-FS_LOCATION_NODE_OFFSET], data[0], items[0][1]);
  fs_getRecData(filesystem[lastPos-FS_LOCATION_NODE_OFFSET], data[count-1], items[count-1][1]);
  
  //next bit is a little redundant, but it's quicker and easier so I'm doing it
  //basically selection sort
  for(i=1;i<(count+1)/2;i++){
    hasPrev=lastFill;
   
    //reset values;
    firstPos=items[count-1][0];
    lastPos=items[0][0];
    
    for(count=0;hasPrev;count++){
     if(filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]<=filesystem[firstPos-FS_LOCATION_NODE_OFFSET]&&filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]>filesystem[prevFirstPos-FS_LOCATION_NODE_OFFSET])
       firstPos=hasPrev;
     if(filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]>=filesystem[lastPos-FS_LOCATION_NODE_OFFSET]&&filesystem[hasPrev-FS_LOCATION_NODE_OFFSET]<filesystem[prevLastPos-FS_LOCATION_NODE_OFFSET])
       lastPos=hasPrev;
    
     hasPrev=filesystem[hasPrev];
    }
        
    items[i][0]=firstPos;
    items[i][1]=fs_findRecSize(filesystem[firstPos-FS_LOCATION_NODE_OFFSET]);
    items[count-i-1][0]=lastPos;
    items[count-i-1][1]=fs_findRecSize(filesystem[lastPos-FS_LOCATION_NODE_OFFSET]);
    prevFirstPos=firstPos;
    prevLastPos=lastPos;
    
    fs_getRecMetaData(filesystem[firstPos-FS_LOCATION_NODE_OFFSET],metaData[i], FS_DATA_REC_OFFSET);
    fs_getRecMetaData(filesystem[lastPos-FS_LOCATION_NODE_OFFSET],metaData[count-i-1],FS_DATA_REC_OFFSET);

    data[i]=malloc(sizeof(int)*items[i][1]);
    data[count-i-1]=malloc(sizeof(int)*items[count-i-1][1]);
    
    fs_getRecData(filesystem[firstPos-FS_LOCATION_NODE_OFFSET], data[i], items[i][1]);
    fs_getRecData(filesystem[firstPos-FS_LOCATION_NODE_OFFSET], data[count-i-1], items[count-i-1][1]);
  }
  
  //fill in filesystem from beginning to end while reconstructing record meta data
  k=0;
  for(i=0;i<count;i++){
    //new offset
    filesystem[items[i][0]-FS_LOCATION_NODE_OFFSET]=k;
    //copy metadata into new position
    for(j=0;j<(FS_DATA_REC_OFFSET);j++){
      filesystem[k]=metaData[i][j];
      k++;
    }
    //reset next and prev since data is contigous
    loc=filesystem[items[i][0]-FS_LOCATION_NODE_OFFSET];
    filesystem[loc]=-1;
    loc=filesystem[loc+FS_NEXT_REC_OFFSET]=-1;
    
    for(j=0;j<items[i][1];j++){
      filesystem[k]=data[i][j];
      k++;
    }
  }
 
 for(i=0;i<count;i++){
   free(metaData[i]);
   free(data[i]);
 }
 
 free(metaData);
 free(data);

 //reset freeNodes
 lastFree=0;
 
 //insertion sort fillnodes from greatest index to least
 k=0;
 for(i=1;i<count;i++){
   k=items[i][0];   
   for(j=i-1;j>-1&&k<items[j][0];j--){
       items[j+1][0]=items[j][0];
   } 
   items[j+1][0]=k;
 }
 
 //assumes order is prev, next, name will break if that changes unfortunately
 //now compress Fillnodes
 //k is offset from back
 k=0;
 for(i=0;i<count;i++){
   //hasNext
   if(i!=count-1)
    filesystem[FILESYSTEM_SIZE-6-k-FS_NEXT_NODE_OFFSET]=FILESYSTEM_SIZE-7-k-FS_LOCATION_NODE_OFFSET;
   //hasPrev
   if(k==0){
      filesystem[FILESYSTEM_SIZE-6-k]=0;
   }else{
      filesystem[FILESYSTEM_SIZE-6-k]=FILESYSTEM_SIZE-5-k+FS_LOCATION_NODE_OFFSET;
     }
     lastFill=FILESYSTEM_SIZE-6-k;
   
   //syncing with j I suppose
   k+=FS_NAME_NODE_OFFSET;
   for(j=FS_NAME_NODE_OFFSET;j<FS_LOCATION_NODE_OFFSET+1;j++){
    filesystem[FILESYSTEM_SIZE-6-k]=filesystem[items[count-1-i][0]-j];
    k++;
   }

 }

 end=lastFill;
 //freeSpace is now equal to size-files-nodes-filesystem values
 freeSpace=FILESYSTEM_SIZE-k-end-5;
 
 filesystem[FILESYSTEM_SIZE-1]=freeSpace;
 filesystem[FILESYSTEM_SIZE-2]=end;
 filesystem[FILESYSTEM_SIZE-4]=lastFree;
 filesystem[FILESYSTEM_SIZE-5]=lastFill;
 
}

/**
 * Finds the size of a record based upon it's index.
 * 
 * returns record's size
 */
int fs_findRecSize(int index){
  
  fs_init();
  
  int size=0;
  
  do{
    size+=filesystem[index+FS_SIZE_REC_OFFSET];
    index=filesystem[index];
  }while(index!=-1);
   
  return size;
  
}

/**
 * Get's the metadata from a record based upon it's index
 * buf is a supplied buffer
 */
void fs_getRecMetaData(int index, int* buf, int size){
  int i;
  
  fs_init();
  
    for(i=0;i<size;i++){
      buf[i]=filesystem[index+i];
    }  
}

/**
 * Get's the data from a record based upon it's index and size
 * buf is a supplied buffer
 * size is size of buffer
 */
void fs_getRecData(int index, int* buf, int size){
  int i,j,len;
  
  fs_init();
  
  j=0;

  len=size;
//  while(len>0&&j<size){
  do{
    for(i=0;i<len&&i<filesystem[index+FS_SIZE_REC_OFFSET];i++){
      buf[ j]=filesystem[index+FS_DATA_REC_OFFSET+i];
      j++;
    }
    len-=(i+1);
    index=filesystem[index+1];//index gets next
  }while(index!=-1);
    
}

/**
 * Removes an record with fid from filesystem
 * Version1: Removes it **Done**
 * Version2: makes record of it in LastFree **Done**
 */
void fs_removeFile(int fid){
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
      //add index to be removed to lastFree list
      fs_addFree(hasPrev);
      break;
    }
    hasPrev=filesystem[hasPrev];
  }
  
}

/**
 * Version 1:List fids of files **done**
 * Version 2:List all size of deleted partitions **done**
 * 
 * Note: Can be shortened, but not critical to do so.
 */
void fs_list(){
  
  //in case not initialized already
  fs_init();
  
  int hasPrev=lastFill;
  
  printf("Filled list:\n");
  while(hasPrev){
    printf("FID: %d\n",filesystem[hasPrev-FS_DATA_NODE_OFFSET]);
    hasPrev=filesystem[hasPrev];
  }
  
  hasPrev=lastFree;
  printf("Free list:\n");
  while(hasPrev){
    printf("Size: %d\n",filesystem[hasPrev-FS_DATA_NODE_OFFSET]);
    hasPrev=filesystem[hasPrev];
  }
}

/**
 * Lists available filled nodes with their name and fids
 */
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

/**
 * Returns number of filled inodes
 */
int fs_getINodeCount(){
  
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
void fs_getAllNodes(INode *buf[]){
  //in case not initialized already
  fs_init();
 
  int hasPrev=lastFill;
  int j,id;
  int len=fs_getINodeCount();
  INode temp;
  
  hasPrev=lastFill;
  for(j=0;hasPrev&&j<len;j++){

    if(len<0)
      break;
    id=filesystem[hasPrev-FS_DATA_NODE_OFFSET];
    
    buf[j]=fs_getNode(id);

    hasPrev=filesystem[hasPrev];
  }
}

/**
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
 * Returns data based upon a file id. It takes data from the offset to the length, assuming offset+length < the total file length
 * If offset is greater than file length, it will return 0
 * If offset+length is greater than file length, it will return 0
 * If fid not found, it will return 0
 * toReturn is a supplied buffer of maximum length 'length'
 * 
 * Warning, has not been fully tested
 */
void fs_get(int fid, int offset, int length, int* toReturn){

  int dataLoc=0;
  int dataLen=0;
  int i;
  int index;
  
  index=fs_findFile(fid);
  
  if(index==-1){
    toReturn=0;
    length=0;
  }
  
  dataLoc=filesystem[index-FS_LOCATION_NODE_OFFSET];      
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
  }
    
}

void fs_dumpAllData(){
  int i;
  for(i=0;i<FILESYSTEM_SIZE;i++)
    printf("%8d", filesystem[i]);
}


/**
 * Tester for the filesystem
 */
//void main(){
void fs_test(){

 printf("SIZE: %d\n",FILESYSTEM_SIZE);
  
fs_initialize();
fs_import("./source.cpu/temp.txt", "prog1");
fs_copy(0,"prog2");
fs_copy(1, "prog3");
printf("COUNT: %d\n", fs_getINodeCount());
fs_removeFile(0);
fs_removeAllFiles();
/*  int a[1]={1};
  int b[2]={1,2};
  int c[3]={1,2,3};
  int d[4]={1,2,3,4};
  
    
  fs_store(1,(char*)a,0);
  fs_store(2,(char*)b,0);
  printf("\nfs_store(2,b,0)\n");
  fs_store(3,(char*)c,0);
    printf("\nfs_store(3,c,0);\n");
  fs_store(4,(char*)d,0);
  printf("Input test\n");
  fs_list();
  fs_removeFile(2);
  printf("middle delete test\n");
  fs_list();
  fs_removeFile(0);
  printf("start delete test\n");
  fs_list();
  fs_removeFile(3);
  printf("end delete test\n");
  fs_list();  
  fs_removeFile(1);
  printf("delete last item\n");
  fs_list();
  
  int e[5]={1,2,3,4,5};
  fs_store(5,(char*)e,0);
    fs_list();
  int f[6]={1,2,3,4,5,6};
  fs_store(6,(char*)f,0);
  fs_list();
  int g[7]={1,2,3,4,5,6,7};
  fs_store(7,(char*)g,0);
  fs_list();
  int h[8]={1,2,3,4,5,6,7,8};
  fs_store(8,(char*)h,0);
  fs_list();
  
  printf("freeSpace: %d\n", freeSpace);
  
  int length;
  inode testNode;
  int buf[100];
  
  fs_store_intf((char*)e, 5);

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
  
  printf("finished");
//*/ 
}