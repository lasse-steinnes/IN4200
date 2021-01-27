#include "set2-ex1.hpp"


void Exercise_1::read_binary(string filename, int n){
   // read from binary file
   FILE *binfile; // pointer to a file stream
   double *fromfile; // pointer to load data from file
   fromfile = (double *)malloc(n * sizeof(double)); // allocate dynamic memory

   string dir;
   dir.append("./data/binary/" + filename + ".bin"); // use string
   const char * dirchar = dir.c_str();   // string to const char * for input
   binfile = fopen(dirchar,"rb"); // open the right folder and name, read rb

   fread(fromfile, sizeof(double), n, binfile); // read from binfile onto fromfile

   fclose(binfile);

   delete [] fromfile;
}

void Exercise_1::write_binary(double *data, string filename, int n){
  // writes array of double to a binary file: .bin

  FILE *binfile; // declare pointer to a file stream
  string dir;
  dir.append("./data/binary/" + filename + ".bin"); // use string
  const char * dirchar = dir.c_str();   // string to const char * for input

  binfile = fopen(dirchar,"wb"); // open the right folder and name, write wb
  fwrite(data, sizeof(double), n, binfile); // write bin file sizeof(double)
                                            //gives storage info
  fclose(binfile);                          // close file

}

void Exercise_1::read_ascii(string filename, int n){
  // read from ascii

  FILE *asciifile; // pointer to a file stream
  double *fromfile; // pointer to load data from file
  fromfile = (double *)malloc(n * sizeof(double)); // allocate dynamic memory

  string dir;
  dir.append("./data/ascii/" + filename + ".txt"); // use string
  const char * dirchar = dir.c_str();   // string to const char * for input
  asciifile = fopen(dirchar,"r"); // open the right folder and name, read rb

  for (int i = 0; i < n; i ++)
    fscanf(asciifile,"%lf", &fromfile[i]);

  fclose(asciifile);

  delete [] fromfile;

}

void Exercise_1::write_ascii(double *data, string filename, int n){
  // writes array of double to ascii file:  .txt

  FILE *asciifile; // file stream pointer

  // applying filename to stream
  string dir;
  dir.append("./data/ascii/" + filename + ".txt"); // use string
  const char * dirchar = dir.c_str();   // string to const char * for input

  asciifile = fopen(dirchar, "w"); // apply stream pointer
  for (int i = 0; i < n; i ++)
    fprintf(asciifile, "%lf\n", data[i]);

  fclose(asciifile);
}

/* Note: binary - only numbers 0-255 - efficient and compact
- binary data is almost exactly how it is stored in memory
- less readable, but can include a txt (ascii file) with some readable info
- PS: How you store the varibles in programme (type declaration) matters when
 reading off the file --> can leave the data useless, i.e.
 if stored as double and read as float etc.
- used for environment with minimal memory storage and/or minimal communication
bandwidth

ASCII text for example - predefined set */
