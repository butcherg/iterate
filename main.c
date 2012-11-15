#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ggetopt.h"

#ifndef MSVC
#include "Objects/MingW/iterate_private.h"
#define COMPILER "MinGW"
#else
#define COMPILER "MS Visual C++"
#include "Objects/Visual C++/iterate_private.h"
#endif

#define STRINGLEN 1024

/* from http://beej.us/guide/bgc/output/html/multipage/gets.html, renamed to chomp() */
char * chomp(char *s)
{
    int len = strlen(s);

    if (len > 0 && s[len-1] == '\n')  // if there's a newline
        s[len-1] = '\0';          // truncate the string

    return s;
}

int main(int argc, char *argv[])
{
    int i, c;
    char * list, * item;
    char * before, * insertpoint, * after;
    char cmd[STRINGLEN], filename[STRINGLEN], fileline[STRINGLEN];
    FILE * listfile;
    strcpy(filename,"");
    list = NULL;

    while ((c = pgetopt (argc, argv, (char *) "f:l:v")) != -1)
        switch (c)
            {
                case 'f':
                    if (strlen(poptarg) > STRINGLEN) {
                        fprintf(stderr, "iterate: Filename too long.\n");
                        exit(1);
                    }
                    else {
                        strncpy(filename,poptarg,STRINGLEN);
                    }
                    break;
                case 'l':
                    list = (char*) malloc (strlen(poptarg)+1);
                    if (list==NULL) exit (1);
                    strncpy(list,poptarg,strlen(poptarg));
                    break;
                case 'v':
                    fprintf(stderr, "iterate %d.%d build %d (%s)\n%s\n", VER_MAJOR, VER_MINOR, VER_BUILD, COMPILER, LEGAL_COPYRIGHT);
                    return(0);
                    break;
                default:
                    fprintf(stderr, "Usage: iterate [-f listfile-one-per-line] [-l comma-separated-list] [-v] \"command\"\n");
                    return(1);
            }
            
    if (!list & strcmp(filename,"") == 0) strcpy(filename,"iterate.txt");
    
    
    if (!system(NULL)) {
        fprintf(stderr, "iterate: Command processor not found.\n");
        exit (EXIT_FAILURE);
    }
    
    if (poptind >= argc) {
        fprintf(stderr, "iterate: No command\n");
        exit(1);
    }
    before = argv[poptind];
    insertpoint = strpbrk(before,"$");
    if (insertpoint !=  NULL) {
        after = insertpoint+1;
        insertpoint[0] = '\0';
    }
    else {
        after = NULL;
    }
    
    
    if (strcmp(filename,"") != 0) {
        listfile = fopen (filename , "r");
        if (listfile == NULL) {
            fprintf(stderr, "iterate: File not found.\n");
            exit(1);
        }
        
        item = fgets(fileline, STRINGLEN, listfile);
        while(!feof(listfile)) {
            item = chomp(fileline);
            strcpy(cmd,before);
            if (after) {
                strcat(cmd,item);
                strcat(cmd,after);
            }
            printf("\n%s:\n", cmd);
            i=system (cmd);
            
            item = fgets(fileline, STRINGLEN, listfile);            
        }
        fclose(listfile);
    }
    else {
        item = strtok(list,",");
        while (item!=NULL) {
            strcpy(cmd,before);
            if (after) {
                strcat(cmd,item);
                strcat(cmd,after);
            }
            printf("\n%s:\n", cmd);
            i=system (cmd);
            item = strtok(NULL, ",");
        }
    }
    return 0;
}
