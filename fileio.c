#include <stdio.h>
#include "topic.h"
#include "fileio.h"

void savePlanToFile(const char *filename, const char *subject, int totalTime, struct Topic *topics, int count){
    FILE *fptr;
    fptr = fopen(filename, "w"); // User chooses filename

    if (fptr == NULL){
        printf("Error: Could not save to '%s'!\n", filename);
        return;
    }

    fprintf(fptr, "=============================\n");
    fprintf(fptr, "    SAVED STUDY PLAN: %s\n", subject);
    fprintf(fptr, "=============================\n");
    fprintf(fptr, "Total Time available: %d min\n", totalTime);
    fprintf(fptr, "Number of Topics: %d\n\n", count);

    for (int i = 0; i < count; i++){
        fprintf(fptr, "Topic %d: %s\n", i + 1, topics[i].name);
        fprintf(fptr, " - Difficulty: %d/5\n", topics[i].difficulty);
        fprintf(fptr, " - Priority: %d/5\n", topics[i].priority);
        fprintf(fptr, " - Score: %d\n", topics[i].score);
        fprintf(fptr, " - TIME ALLOCATED: %d min\n", topics[i].allocatedTime);
        fprintf(fptr, "--------------------\n");
    }

    fclose(fptr);
    printf("\n[SYSTEM] Plan saved successfully to '%s'!\n", filename);
}