#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void usage() {
  printf("You need to provide a file name\n");
}

struct line {
  char *string;
  struct line *next;
  struct line *prev;
};

void renderLines(short height, struct line* lines) {
  printf("renderLines\n");
  printf("\x1b[2J"); // clear the screen
  printf("\x1b[0;0f"); // move to top left

  if(lines) {

    // find line to show at top
    struct line* top = lines;
    short above = floor(height / 2);
    signed short numbering = 0;
    for(int i = 0; i < above; i++) {
      if(top->prev == NULL) break;
      top = top->prev;
      numbering--;
    }

    // print lines from top to bottom
    struct line* line = top;
    for(int i = 0; i < height; i++) {
      printf("%3d | %s", numbering, line->string);
      line = line->next;
      if(line == NULL) break;
      numbering++;
    }

  } else {
    printf("<EMPTY>");
  }

  printf("\x1b[%d;1f", height + 1); // move to bottom
}

signed int saveFile(char* filename, struct line* lines) {
  FILE *fp;
  fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening file");
    return(1);
  }

  struct line* ptr = lines;
  while(ptr) {
    fprintf(fp, "%s", ptr->string);
    ptr = ptr->next;
  }

  fclose(fp);
  return(0);
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    usage();
    return -1;
  }

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Error opening file");
    return(-1);
  }

  struct line *lines = (struct line*) malloc(sizeof(struct line)); // dummy head
  struct line *current = lines;
  int appendToPrevious = 0;
  while(1) {
    char input[100];
    if (fgets(input, 100, fp) == NULL)
      break;
    if(!appendToPrevious) {
      struct line *link = (struct line*) malloc(sizeof(struct line));
      link->string = (char *) malloc(100);
      strncpy(link->string, &input[0], 100);
      current->next = link;
      link->prev = current;
      current = link;
    } else {
      char * newStr;
      if((newStr = malloc(strlen(current->string)+101)) != NULL) {
        newStr[0] = '\0';
        strcat(newStr, current->string);
        strncat(newStr, &input[0], 100);
        free(current->string);
        current->string = newStr;
      } else {
        fprintf(stderr, "malloc failed\n");
        return(-1);
      }
    }
    appendToPrevious = input[strlen(input) - 1] != '\n';
  }
  fclose(fp);

  // clean up dummy head; lines is be null in both directions if file empty
  struct line* tmpHead = lines;
  lines = lines->next;
  free(tmpHead);
  if(lines)
    lines->prev = NULL;

  renderLines(w.ws_row - 1, lines);

  struct line *top = lines;
  while(1) {
    char in = getchar();
    getchar();
    switch(in) {
      case 'q': return 0;
      case 'j': if(top->next) top = top->next; break;
      case 'k': if(top->prev) top = top->prev; break;
      case 'w': {
        if(saveFile(argv[1], lines))
          return -1;
        break;
      }
      case 'd': {
        if(top->prev == NULL) { // first line i.e. top == lines
          if(top->next == NULL) { // file will now be empty
            lines = NULL;
            top = NULL;
          } else {
            lines = lines->next;
            free(lines->prev);
            lines->prev = NULL;
            top = lines;
          }
        } else if(top->next == NULL) { // last line
          struct line* newLastLine = top->prev;
          newLastLine->next = NULL;
          free(top);
          top = newLastLine;
        } else {
          top->prev->next = top->next;
          top->next->prev = top->prev;
          struct line* next = top->next;
          free(top);
          top = next;
        }
        break;
      }
    }
    renderLines(w.ws_row - 1, top);
  }
}
