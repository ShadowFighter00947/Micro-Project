#include <stdio.h>
#include <ctype.h>
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

// Below section of code shows everything users has given in one go.
    printf("\nSubject: %s\n", sub);
    printf("Time available (in minutes): %d\n", time);
    printf("Number of Topics: %d\n", nos_topic);

    printf("\nTopics:\n");

   displayTopics(topics, nos_topic); // This calls the function in topic.

   allocateTime(topics, nos_topic, time);
   displayStudyplan(topics, nos_topic);

   getUserFeedback(topics, nos_topic, time);

   char filename[100];
   sprintf(filename,  "%s.plan", sub);
   printf("\n[AUTO] Saving plan to: %s\n", filename);

   savePlanToFile(filename, sub, time, topics, nos_topic);

    return 0;
}