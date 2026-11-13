#include <stdio.h>
#include <ctype.h>
#include "topic.h"
#include "agent.h"

#define MIN_SHARE 10 // Minimum % of total time per study topic.
#define MAX_SHARE 40 // Maximium % of total time per study topic.
#define REVIEW_SHARE 5 // % of total time for a review-only topic.
#define LOW_SCORE 50 // Below this count as struggling.
#define HIGH_SCORE 85 // Above this count as mastered.

static int sameTopic(const char *a, const char *b){
    while (*a && *b){
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static int findPrev(const char *name, const struct Topic *prev, int prevCount){
    for (int j = 0; j < prevCount; j++){
        if (sameTopic(name, prev[j].name)) return j;
    }
    return -1;
}
