#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eggnog.h"

#define TAGSIZE 800

static int col = 1;

static void handle_tag(char *tag)
{
	if (strcmp(tag, "<h2>") == 0) {
		printf("\033[1;31m"); // bold red
	} else if (strcmp(tag, "<h2 id=\"part2\">") == 0) {
		printf("\n\n");
		printf("\033[1;31m"); // bold red
		col = 1;
	} else if (strcmp(tag, "<em class=\"star\">") == 0) {
		printf("\033[1;33m"); // bold yellow
	} else if (strcmp(tag, "<em>") == 0) {
		printf("\033[1;32m"); // bold green
	} else if (strcmp(tag, "</h2>") == 0) {
		printf("\n");
		col = 1;
	} else if (strcmp(tag, "</p>") == 0) {
		printf("\n");
	} else {
		printf("\033[0m");  // reset colors
	}
}

void pretty_print(char *html)
{
	char *tmp;
    tmp = html;
    // where we want to start
    const char *START = "class=\"day-desc\">";
	size_t startlen = strlen(START);
	tmp = strstr(html, START);
    // move pointer ahead to starting point
	tmp += startlen;
    // where we want to end
	char *tmp2;
    // one of these will cut us off where we want to
    tmp2 = strstr(html, "<p class=\"day-success\">Both parts of this puzzle are complete!");
    if (tmp2 == NULL) {
        tmp2 = strstr(html, "<form method=\"post\"");
    }

	// if tmp is still null...
	if (tmp2 == NULL) {
		printf("session id no longer viable\n");
		printf("run\n\t`rm $HOME/.eggnog/sessionid.txt\n");
		printf("and rerun the program with a new session id\n");
		exit(EXIT_FAILURE);
	}
    // get byte distance from start to end
	long end = (tmp2 - tmp)*sizeof(*tmp);
	printf("%ld\n", end);

	char tag[TAGSIZE] = {0};
	int j = 0;
	int PRINT = 0;
	//printf("OUTSIDE LOOP\n");
	for (int i = 0; i < end && *tmp; i++, tmp++) {
		//printf("IN THE LOOP\n");
		if (*tmp == '<') {
			PRINT = 0;
			tag[j++] = *tmp;
		} else if (*tmp == '>') {
			// end of tag
			tag[j++] = *tmp;
			handle_tag(tag);
			PRINT = 1;
			j = 0;
			memset(tag, 0, TAGSIZE);
		} else if (PRINT) {
            // try to keep it under 80 columns wide
			if (col > 70 && *tmp == ' ') {
				printf("\n");
				col = 1;
			} else {
			printf("%c", *tmp);
			col++;
		    } 
        } else if (!PRINT) {
			tag[j++] = *tmp;
		}
	}
}
