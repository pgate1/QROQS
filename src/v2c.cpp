/*
	Copyright Takano
*/

#include "stdafx.h"

#include "v2c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nagureo {
  int x;
  char *name;
  struct nagureo *next;
} NOV;

void readtext(char *, char*);
int calcheck(char);
NOV *get_in(char *);
void wakeru(NOV *, const char *);
char *strsave(char *);
void kakko(NOV *);
void narabi(NOV *);
void gambol(NOV*);

enum {
  X   = ' ',
  AND = '&',
  NOT = '~',
  OR  = '|',
  XOR = '^'
};


/*
int main(int argc , char *argv[]) {
    FILE *fp;
    char buf[1000];
    char s[1000];
    
    if (argc == 1) {
        readtext(fgets(buf, 1000, stdin), s);
        printf("%s", s);
    }
    argc--;
    while(argc--) {
        argv++;
        
        if((fp = fopen(*argv, "r")) == NULL) {
            cant(*argv);
            continue;
        }
        while (fgets(buf, 1000, fp) != NULL) {
            readtext(buf, s);
            printf("%s\n", s);
        }
    }
    return 0;
}


void cant(char *s){
  printf("can't read %s", s);
  exit(1);
}
*/


void readtext(const char *name, char *x) {//‚±‚ê‚ðŒÄ‚×III
	if(name[0]=='\0'){
		x[0]='\0';
		return;
	}

  NOV *xp;
  NOV dmy;
  xp = &dmy;
  xp->x = -2;
  xp->next = NULL;
  
  wakeru(xp, name);
  gambol(xp);

  if (xp->next->x == -1) {
    strcpy(x, xp->next->name);
    free(xp->next->name);
    free(xp->next);
  }
}

//×Ø‚ê
void wakeru(NOV *xp, const char *name) {
  int c;
  char buf[50];
  char *bf = buf;
  int start_flag = 0;
  NOV  *yp;

  while(1) {
    if (*name == 'i') {
      if (start_flag) {
	yp = get_in(buf);
	yp->x = 0;
	xp->next = yp;
	xp = yp;
	bf = buf;
	*bf = '\0';
      }
      else 
	start_flag = 1;
    }
    if ((c = calcheck(*name)) > 0) {
      yp = get_in(buf);
      yp->x = c;
      xp->next = yp;
      xp = yp;
      bf = buf;
      *bf = '\0';
    }
    if ((*name == '\0') || (*name == '\n')) {
      yp = get_in(buf);
      yp->x = -1;
      xp->next = yp;
      return;
    }
    *bf++ = *name;
    *bf = '\0';
    name++;
  }
}

NOV *get_in(char *buf) {
  NOV *yp;
  if ((yp = (NOV *)malloc(sizeof(NOV))) == NULL) {
    printf("malloc error");
    exit(1);
  }
  yp->name = strsave(buf);
  yp->next = NULL;
  return yp;
}

//•¶Žš—ñ—Ìˆæ
char* strsave(char *name) {
  char *n;
  if ((n = (char *)malloc(strlen(name) + 1)) == NULL){
    printf("malloc error");
    exit(1);
  }
  strcpy(n, name);
  
  return n;
}

int calcheck(char c) {
  switch(c) {
  case AND:
    return 1;
  case OR:
    return 1;
  case XOR:
    return 1;
  case NOT:
    return 2;
  }
  return 0;
}

// ~ˆ—
void kakko(NOV *ember) {
  NOV *xp, *yp;
  char buf[2000];
  
  for (xp = ember->next; xp != NULL; xp = xp->next) {
    if ((strlen(xp->name) != 1) && (xp->x == 2)) {
      sprintf(buf, "(~%s)", xp->name);
      xp->x = xp->next->x;
      free(xp->name);
      xp->name = strsave(buf);
      yp = xp->next->next;
      free(xp->next->name);
      free(xp->next);
      xp->next = yp;
    }
  }
}

//‚»‚Ì‘¼
void narabi(NOV *nard) {
  NOV *xp, *yp;
  char buf[2000];
  
  for (xp = nard->next; xp != NULL; xp = xp->next) {
    if ((xp->next == NULL) || xp->next->next == NULL) 
      break;
    if ((strlen(xp->name) > 1) && (strlen(xp->next->name) > 1) && (xp->next->x != 0)) {
      sprintf(buf, "(%s%s%s)", xp->name, xp->next->next->name, xp->next->name);
      xp->x = xp->next->next->x;
      free(xp->name);
      xp->name = strsave(buf);
      yp = xp->next->next->next;
      free(xp->next->next->name);
      free(xp->next->next);
      free(xp->next->name);
      free(xp->next);
      xp->next = yp;
    }
  }
}

void gambol(NOV* xp) {
  int i = 0;
  while (xp->next->x != -1) {
    kakko(xp);
    narabi(xp);
    i++;
    if (i == 1000)
      break;
  }
}

/// EOF ///
