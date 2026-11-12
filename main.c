#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "topic.h"
#include "planner.h"
#include "feedback.h"
#include "fileio.h"

int main()
{
    char sub[50]; // Takes Max 50 characters.
    int time, nos_topic;

    printf("Enter Subject:- ");
    scanf(" %[^\n]", sub);

    for (int i = 0; sub[i]; i++){
        sub[i] = tolower(sub[i]);
    }

    char filename[100];
    sprintf(filename, "%s.plan", sub);

    struct Topic prevTopics[20];
    int sessions = 0;
    int prevCount = loadLastSession(filename, prevTopics, 20, &sessions);

    printf("Sessions found: %d\n", sessions);
    for (int i = 0; i < prevCount; i++){
        printf("[LOADED] %s | Difficulty: %d | Priority: %d | Score: %d | Time: %d min\n", prevTopics[i].name, prevTopics[i].difficulty, prevTopics[i].priority, prevTopics[i].score, prevTopics[i].allocatedTime);
    }

    printf("Enter Time available (in minutes):- ");
    scanf("%d", &time);
    getchar();

    printf("Enter number of Topics:- ");
    scanf("%d", &nos_topic);
    getchar();

    struct Topic topics[nos_topic];


    for (int i = 0; i < nos_topic; i++) 
{
    printf("\nTopic %d:\n", i + 1);
    inputTopic(&topics[i]); // This calls the function in topic.c
}

for (int i = 0; i < nos_topic; i++){
    for (int j = 0; j < prevCount; j++){
        if (strcmp(topics[i].name, prevTopics[j].name) == 0){
            printf("Last session score for %s: %d%%\n", topics[i].name, prevTopics[j].score);
        }
    }
}

// Below section of code shows everything users has given in one go.
    printf("\nSubject: %s\n", sub);
    printf("Time available (in minutes): %d\n", time);
    printf("Number of Topics: %d\n", nos_topic);

    printf("\nTopics:\n");

   displayTopics(topics, nos_topic); // This calls the function in topic.

   allocateTime(topics, nos_topic, time);
   displayStudyplan(topics, nos_topic);

   getUserFeedback(topics, nos_topic, time);

   
   printf("\n[AUTO] Saving plan to: %s\n", filename);

   savePlanToFile(filename, sub, time, topics, nos_topic);

    return 0;
}