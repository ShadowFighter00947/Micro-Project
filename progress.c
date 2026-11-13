#include <stdio.h>
#include "topic.h"
#include "progress.h"

#define REVISION_THRESHOLD 50 // Scores below this need revision.

void showProgress(const struct Topic *topics, int count){
    if (count <= 0) return;

    int total = 0;
    int strongest = 0;
    int weakest = 0;

    for (int i = 0; i < count; i++){
        total += topics[i].score;
        if (topics[i].score > topics[strongest].score) strongest = i;
        if (topics[i].score < topics[weakest].score) weakest = i;
    }

    printf("\n=============================\n");
    printf("       PROGRESS SUMMARY      \n");
    printf("=============================\n");
    printf("Average score : %.1f%%\n", (float)total / count);
    printf("Strongest     : %s (%d%%)\n", topics[strongest].name, topics[strongest].score);
    printf("Weakest       : %s (%d%%)\n", topics[weakest].name, topics[weakest].score);

    printf("Needs revision (below %d%%):\n", REVISION_THRESHOLD);
    int any = 0;
    for (int i = 0; i < count; i++){
        if (topics[i].score < REVISION_THRESHOLD){
            printf("  - %s (%d%%)\n", topics[i].name, topics[i].score);
            any = 1;
        }
    }
    if (!any) printf("  None. Keep it up!\n");
}