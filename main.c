#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>

#define KEY "664c69616c7565725061726f"

char *scramble(const char*, const char*, int*); /* Originally sub_804D5E0 */

int sub_804D415(const char*, char*, int);
int createHash(unsigned char *, signed int, unsigned char *); /* Originally sub_804D0C0 */
int sub_804D1A8(char*, int, unsigned char*);

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("usage: %s input output\n", argv[0]);
    return 1;
  }

  int bytesRead;
  char* buffer = scramble(argv[1], KEY, &bytesRead);

  if (!buffer) {
    printf("Unable to read %s!\n", argv[1]);
    return 1;
  }

  FILE *file;
  if (argc == 2)
    file = stdout;
  else
    file = fopen(argv[2], "wb");

  fwrite(buffer, 1, bytesRead, file);
  fclose(file);

  free(buffer);
  return 0;
}


char *scramble(const char *filename, const char *key, int *bytesRead)
{
  char *license;

  FILE *file = fopen(filename , "rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind( file );
    char *buffer = (char*)malloc(size);
    if (buffer) {
      fread((void *)buffer, 1, size, file);
      fflush(file);
      fclose(file);
      sub_804D415(key, buffer, size);
      *bytesRead = size;
      license = buffer;
    }
    else { /* malloc failed! */
      *bytesRead = 0;
      license = 0;
    }
  }
  else { /* fopen64 failed! */
    *bytesRead = 0;
    license = 0;
  }

  /* Return results */
  return license;
}


int sub_804D415(const char *key, char *buffer, int size)
{

  char dest[512];
  strcpy(dest, key);

  unsigned int lengthWithNull;
  unsigned int length;
  lengthWithNull = strlen(dest);
  length = lengthWithNull - 1;

  length = (signed int)length / 2;
  char s[] = "AAAA";

  int i;
  int temp1;
  unsigned char magic[256];
  for (i=0 ; i<=(signed int)length ; ++i) {
    s[2] = dest[2 * i];
    s[3] = dest[2 * i + 1];

    sscanf((const char *)&s, "%x", &temp1);
    magic[i] = temp1;
  }
  unsigned char hashTable[258];
  createHash(magic, length, hashTable);
  sub_804D1A8(buffer, size, hashTable);

  return 0;
}


int createHash(unsigned char *magic, signed int length, unsigned char *hashTable)
{
  int16_t i;
  for (i = 0; i <= 255; ++i)
    hashTable[i] = i;

  hashTable[256] = 0;
  hashTable[257] = 0;

  int temp1 = 0;
  int temp2 = 0;
  for (i = 0 ; i <= 255 ; ++i) {
    temp1 = magic[temp2] + hashTable[i] + temp1;
    temp1 &= 0xff;

    temp2++;
    temp2 %= length + 1;

    //Need to swap bytes hashTable[i] with hashTable[temp1]
    char tmpByte = hashTable[i];
    hashTable[i] = hashTable[temp1];
    hashTable[temp1] = tmpByte;
  }

  return 0;
}


int sub_804D1A8(char *buffer, int size, unsigned char *hashTable)
{
  int result;

  unsigned char v5 = hashTable[256];
  unsigned char v6 = hashTable[257];
  int16_t i;
  for (i = 0; i < size; ++i)
  {
    ++v5;
    v6 += hashTable[v5];

    char tmpByte = hashTable[v5];
    hashTable[v5] = hashTable[v6];
    hashTable[v6] = tmpByte;

    buffer[i] ^= hashTable[(hashTable[v6] + hashTable[v5]) & 0xff];
  }
  hashTable[256] = v5;
  result = v6;
  hashTable[257] = v6;

  return result;
}
