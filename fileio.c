#include <stdio.h>
#include <time.h>
#include <string.h>
#include "topic.h"
#include "fileio.h"

void savePlanToFile(const char *filename, const char *subject, int totalTime, struct Topic *topics, int count){
    FILE *fptr;
    fptr = fopen(filename, "a"); // User chooses filename

    if (fptr == NULL){
        printf("Error: Could not save to '%s'!\n", filename);
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(fptr, "=============================\n");
    fprintf(fptr, "    SAVED STUDY PLAN: %s\n", subject);
    fprintf(fptr, "    Date: %04d-%02d-%02d Time: %02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
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

    fprintf(fptr, "\n\n");

    fclose(fptr);
    printf("\n[SYSTEM] Plan saved successfully to '%s'!\n", filename);
}

int loadLastSession(const char *filename, struct Topic *topics, int maxTopics, int *sessions)
{
    FILE *fptr = fopen(filename, "r");
    char line[200];
    char name[50];
    int count = 0;
    int sessionNo = 0;
    int n, value;

    if (sessions != NULL) *sessions = 0;
    if (fptr == NULL) return 0;

    while (fgets(line, sizeof(line), fptr) != NULL){
        if (strstr(line, "SAVED STUDY PLAN") != NULL){
            sessionNo++;
            count = 0;
            continue;
        }

        if (sscanf(line, "Topic %d: %49[^\n]", &n, name) == 2){
            if (count < maxTopics){
                strcpy(topics[count].name, name);
                topics[count].difficulty = 0;
                topics[count].priority = 0;
                topics[count].score = 0;
                topics[count].allocatedTime = 0;
            }
            count++;
            continue;
        }

        if(count < 1 || count > maxTopics) continue;
        struct Topic *t = &topics[count - 1];

        if (sscanf(line, " - Difficulty: %d", &value) == 1)
            t->difficulty = value;
        else if (sscanf(line, " - Priority: %d", &value) == 1)
            t->priority = value;
        else if (sscanf(line, " - Score: %d", &value) == 1)
            t->score = value;
        else if (sscanf(line, " - TIME ALLOCATED: %d", &value) == 1)
            t->allocatedTime = value;
    }

    fclose(fptr);

    if(sessions != NULL) *sessions = sessionNo;
    return (count > maxTopics)? maxTopics : count;
}