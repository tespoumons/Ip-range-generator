#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MALLOC(type,x) (type)malloc(x)

static long ObtenirTailleFichier(FILE* f)
{
    if(fseek(f, 0, SEEK_END)) // If successful, the function returns zero.
        return 0;

    long fsize = ftell(f); // On failure, -1L is returned,

    if(fsize == -1)
        return 0;

    rewind(f); // void return

    return fsize;
}
typedef struct
{
	char* octets; // pointeur vers les octets
	long taille;

}_fichier_traits;


typedef unsigned int uint;
typedef uint polyvalent;
struct Node
{
    polyvalent value;
    struct Node* suivant;
};

struct Node* currNode = NULL;
_fichier_traits fichier_traits;

void PushIntoLL(polyvalent value)
{
    struct Node* tmpNode = MALLOC(struct Node*, sizeof(struct Node));

    tmpNode->value = value;
    tmpNode->suivant = currNode;

    currNode = tmpNode;
}
#define GETBYTE(x, i) ((x >> 8*(i-1)) & 0xFF)
void PrintLL(char* output)
{
    FILE* f = fopen(output, "wb");
    if(f)
    {
        while(currNode)
        {
           	uint start = currNode->value & (~0 - 0xFF);
           	int i;
            for(i = 0; i <= 0xFF; i++)
            {
                fprintf(f,"%u.%u.%u.%u\n", GETBYTE(start, 4), GETBYTE(start, 3), GETBYTE(start, 2), GETBYTE(start, 1));
                start++;
            }


            currNode = currNode->suivant;
        }
    }
    else return;
}
void DeleteLL()
{
    while(currNode)
    {
        free(currNode);
        currNode = currNode->suivant;
    }
}
int IsDigit(char c)
{
    return ( (c >= '0') && (c <= '9') );
}

uint StrToHost(char * const str)
{
    //192.168.1.1
    char* sp = str;
    uint ip = 0, tmp = 0, shift = 24;

    if(*sp)
    {
        do
        {
            if(IsDigit(*sp))
                tmp = (*sp - '0') + tmp * 10;

            else if(*sp == '.' || *sp == '\0')
            {
                ip |= tmp << shift;
                shift -= 8;
                tmp = 0;
            }
            sp++;
        } while(*(sp-1)) ; // we start checking from the second char
    }

    return ip;
}
char* parseFile(const char* fname)
{
	FILE* f = fopen(fname, "rb");

	if(f)
	{
		long fsize = fichier_traits.taille = ObtenirTailleFichier(f);
		char* fptr = fichier_traits.octets = MALLOC(char*, fsize);

		if(fptr)
		{
			if(fread(fptr, 1, fsize, f) == fsize)
				return fptr;
		}
	}
	return NULL;
}
int main(int argc, char* argv[])
{
	int i;
	if(argc < 3)
		return 0;
	if(parseFile(argv[1])) 
	{
		char* fptr = fichier_traits.octets;
		for(i = 0; i < fichier_traits.taille; i++)
		{
			
			int lastoctet = fichier_traits.taille -1;
			if(fichier_traits.octets[i] == '\n' || i == lastoctet )
			{
				int delta = &fichier_traits.octets[i]-fptr;
				if(i == lastoctet)
					delta++;

				char pch[100];
				memcpy(pch,fptr, delta);
				pch[++delta] = 0;
				PushIntoLL(StrToHost(pch));
				//printf("%s\n", pch);
				//printf("%u\n", StrToHost(pch));
				//printf("%.*s\n", delta, fptr);
				fptr += delta ;

			}
		}
		PrintLL(argv[2]);
		DeleteLL();
	}

}