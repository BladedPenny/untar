#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <errno.h>

long get_filesize(FILE *fp);
int const RECORD_SIZE = 512;


int populatePathList(char *listofpath[], char *filepath);
int get_contentSize(long int filesize);
int main(int argc,char **argv){
 char *filePath = argv[1];
 FILE *tarFile = fopen(filePath, "r"); //tar file instance
	    
 while(true)
 {
//    printf("Stream position %lu  \n",ftell(tarFile));
    char *archiveptr =  (char*)malloc(1024);
    fread(archiveptr, 1, 1024, tarFile);
    fseek(tarFile,-1024,SEEK_CUR);
    int k = 0;
    bool isEmpty = true;
    for(k; k < 1024; k++){
      if(*archiveptr != NULL){
       isEmpty = false;
       }
	archiveptr++;
    }
//    free(archiveptr);
       
    if(isEmpty){
       break;
    }
   
    char *headerRecord = (char *) malloc(512);
    fread(headerRecord,1,512,tarFile);
	
  
    char fileName[100]; //NAME OF THE FILE to be extracted or written to	
     k = 0;
    for(k; k<100; k++){
	fileName[k] = headerRecord[k];
    }
   // printf("THis is the HEADEREcoRD: %s \n", headerRecord);
    char fileSize[12]; //file size in terms of a string
    int fileSizeCounter = 0;
    int fileSizeStarts = 124;
    while(fileSizeCounter<12){
   	fileSize[fileSizeCounter] = headerRecord[fileSizeStarts];
       	fileSizeStarts++;
        fileSizeCounter++;
	}		
   char *fileSizeptr;
   long int FILESIZE; //FILE SIZE OF THE CONTENT TO BE READ
   FILESIZE = strtol(fileSize, &fileSizeptr, 8);
   //printf("This is the fileSize: %lu \n",FILESIZE);

   if(FILESIZE == 0){
	mkdir(fileName, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);



}  
   if(FILESIZE!=0)
   {
  //  printf("this is the filename wjen SIZE > 0: %s \n", fileName);
    char *FileToOpen = strdup(fileName);
    char *temppointer = (char *) malloc(FILESIZE);
    fread(temppointer, 1, FILESIZE, tarFile);
   // printf("THIs is the temppointer: %s \n", temppointer);
    char *ListPath[100];
    int num = populatePathList(ListPath, fileName);
    while(num > 0){
         mkdir(ListPath[num -1], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH); 
	 num--;

		}
//    printf("This is the filename: %s \n", fileName);	   
    FILE *newFile = fopen(FileToOpen, "w+b");
  //  printf("THe filename is OPENED: %s \n", fileName);
    fwrite(temppointer, 1, FILESIZE, newFile);
    fclose(newFile);
   }
long int sizetoadd = get_contentSize(FILESIZE) - FILESIZE;
fseek(tarFile, sizetoadd, SEEK_CUR);
	
}
 fclose(tarFile);
	
return 0;
}


int populatePathList(char *listofpath[], char *filename){
char *dirNameReturn;
dirNameReturn = dirname(filename);
int num = 0;

while(strcmp(dirNameReturn, ".") != 0){
listofpath[num] = strdup(dirNameReturn);
dirNameReturn = dirname(dirNameReturn);
num++;
//printf("From Populate Path List %s \n",listofpath[num-1]);
}
return num;

}



int get_contentSize(long int filesize){


long int multiple = filesize/ 512;
long int modulo = filesize % 512;
long int thingtoadd;
if(modulo > 0){
thingtoadd = (multiple * 512 )+ 512;

                }else{thingtoadd = multiple*512;
                       }

return thingtoadd;
}
