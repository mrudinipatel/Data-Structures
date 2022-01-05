#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hash.h"

int getIndex(char *buildingDesig, char * roomNum);
int entries( char *filebase );
void query( char * code, int index, char * building, int roomindex, char * room);
void intersection(char * buildSetFile, char*roomSetFile, char*alexSetFile);
int set2idx(char * setFile);
char * get_string(char * filename, int num);

int main(int argc, char *argv[]){
    long indexOfBuilding = 0; // holds index of building 
    long indexOfRoom = 0; // holds index of room
    char loop; // for for loop
    int sub = 0; // holds index values for subject 
    int courseNum = 0; // holds index values of courseno
    int days = 0; // holds index values for days
    int start = 0; // holds index values for start time
    int end = 0; // holds index values for end time
    char nameOfFile[BUFFER]; // for the intersection file
    int duplicates = -1; // used to find duplicates
    char buildingSetFile[BUFFER]; // for the set file for the building 
    char roomSetFile[BUFFER]; // for the set file for the room

    if(argc != 3){ // if there arent three command line arguments exit the program
        exit(-1);
    }
  
    //create proper set file from command line inputs 
    strcpy(nameOfFile, argv[1]);
    strcat(nameOfFile, argv[2]);
    strcat(nameOfFile, ".set");

    // for set file for building 
    strcpy(buildingSetFile, "building");
    strcat(buildingSetFile, ".set");

    //for setFile for room
    strcpy(roomSetFile, "room");
    strcat(roomSetFile, ".set");

    indexOfBuilding = getIndex("building", argv[1]); // get index of building 
    indexOfRoom = getIndex("room", argv[2]); // get index of room 

    query("code", -1, "building", indexOfBuilding, buildingSetFile); // create building.set file 
    query("code", -1, "room", indexOfRoom, roomSetFile); // create room.set file 


    intersection(buildingSetFile,roomSetFile, nameOfFile); // instersection between building and room.set file will bee stored in alex200.set 
  
    FILE *fp = fopen(nameOfFile, "rb" ); // open set file
    
    for (int i=0; fread(&loop,1,1,fp)==1; i++){ // display subject, courseno, days, from, and to values for the building and room number 
        if (loop){
            query("code", i, "subject", -1, "subject.set"); // creates set file for subject 
            sub = set2idx("subject.set"); // gets index for subject 
        
            query("code", i, "courseno", -1, "courseno.set"); // create set file for courseno
            courseNum = set2idx("courseno.set"); // gets index for courseno
            
	    if(duplicates == courseNum){ // if there is a duplicate do not print 
                continue; 
            }

            duplicates = courseNum; // will hold duplicate course code
            
            query("code", i, "days", -1, "days.set"); // create set file for days
            days = set2idx("days.set"); // gets index for days
        
            query("code",i, "to", -1, "to.set"); // create set file for the ending time
            end = set2idx("to.set"); // gets index for the ending time
            
            query("code", i, "from", -1, "from.set"); // create set file for the start time
            start = set2idx("from.set");// gets index for the start time

            // prints strings of the following attributes
            printf("%s %s %s %s - %s\n",get_string("subject", sub), get_string("courseno", courseNum), get_string("days", days), get_string("from", start), get_string("to", end));
        }
    }

    fclose(fp);// close file 
}

int getIndex(char *buildingDesig, char *roomNum){
    char idxname[BUFFER];
    char txtname[BUFFER];

    char *basename;
    char *value;
    long hash_table[ HASHSIZE ];

    // identify text file name
    strcpy(idxname, buildingDesig);
    strcat(idxname, ".idx" );
    strcpy(txtname, buildingDesig);
    strcat(txtname, ".txt" );

    // basefile for hashing
    basename = buildingDesig;

    // target value
    value = roomNum;

    // load hashtable from file into memory
    get_hashtable(basename, hash_table );

    // open text file
    FILE *idxfile = fopen(idxname, "r" );
    FILE *txtfile = fopen(txtname, "r" );

    // print result of hash_lookup
    long lookupHash =  hash_lookup(value, hash_table, idxfile, txtfile );
    fclose(idxfile);
    fclose(txtfile);

    return lookupHash;
}

int entries(char *filebase){
    char filename[BUFFER];
    int ent;

    strcpy(filename, filebase );
    strcat(filename, ".idx" );
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    ent = ftell(fp) / sizeof(long);
    fclose(fp);

    return ent;
}

void query(char * code, int index, char * building, int roomindex, char * room){
    char *v1 = code;
    int i1 = index;
    char *v2 = building;
    int i2 = roomindex;

    int n1 = entries(v1);
    int n2 = entries(v2);

    char filename[BUFFER];

    strcpy(filename, v1);
    strcat(filename, "_");
    strcat(filename, v2);
    strcat(filename, ".rel");

    FILE *fp = fopen( filename, "rb");
    char *array = malloc( n1*n2);
    fread(array, 1, n1*n2, fp);
    fclose(fp);
  
    fp = fopen(room, "wb");
    
    if ((i1 == -1) && (i2 >= 0)){
       for (int i = 0; i < n1; i++){
          int index = i * n2 + i2;
          fwrite(array+index, 1, 1, fp);
       }
    }

    if ((i1 > 0) && (i2 ==- 1)){
      for (int j = 0; j < n2; j++){
         int index = i1 * n2 + j;
         fwrite(array+index, 1, 1, fp);
       }
    }
	
    free(array);
    fclose(fp);  
}

void intersection(char * buildSetFile, char*roomSetFile, char*alexSetFile){
   FILE *fp1 = fopen(buildSetFile, "rb");
   FILE *fp2 = fopen(roomSetFile, "rb");
   FILE *fp3 = fopen(alexSetFile, "wb");
   char b1, b2, b3;
   
   while (fread(&b1, 1, 1, fp1) == 1 && fread(&b2, 1, 1, fp2)){
        b3 = b1&&b2;
	fwrite(&b3, 1, 1, fp3);
   }

   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
}

int set2idx(char * setFile){
    char boolean;
    int temp = 0;

    FILE *fp = fopen(setFile, "rb");

    for (int i = 0; fread(&boolean,1,1,fp)==1; i++){
       if (boolean){
           temp = i;
       }
    }
    
    fclose(fp);
    return temp;
}

char * get_string(char * filename, int num){
    char *basename;
    int idx, idx2;
    char * contents = NULL;
    basename = filename;
    idx = num;
    char txtfile[BUFFER];
    char idxfile[BUFFER];
    char buffer[BUFFER];
	
    FILE *fptxt, *fpidx;
	
    strcpy(txtfile, basename);
    strcat(txtfile, ".txt");
    strcpy(idxfile, basename);
    strcat(idxfile, ".idx");
	
    fptxt = fopen(txtfile, "r");
    fpidx = fopen(idxfile, "r");
	
    fseek(fpidx, sizeof(long) * idx, SEEK_SET);
	
    if (fread(&idx2, sizeof(long), 1, fpidx ) != 1){
    	fprintf(stderr, "Error: Invalid index, please try again.\n" );
    	exit(-1);
    }
	
    fclose(fpidx);
    fseek(fptxt, idx2, SEEK_SET);
    fgets(buffer, BUFFER, fptxt);
	
    contents = malloc(strlen(buffer) + 1);
    strcpy(contents, buffer);
	
    fclose(fptxt);
	
    contents[strlen(contents) - 1] = '\0';
    return contents;
}



