#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "topic.h"
#include "planner.h"
#include "feedback.h"
#include "fileio.h"
#include "progress.h"

int main()
{
    char sub[50]; // Takes Max 50 characters.
    int time, nos_topic;

    printf("Enter Subject:- ");
    scanf(" %49[^\n]", sub);

    // Clean the subject: drop arrow-key escape codes, keep only letters,
    // digits and single spaces, and convert to lowercase.
    char clean[50];
    int k = 0;
    for (int i = 0; sub[i]; i++){
        if (sub[i] == 27){                       // ESC character (arrow keys etc.)
            i++;
            if (sub[i] == '[' || sub[i] == 'O'){
                i++;
                while (sub[i] && !isalpha((unsigned char)sub[i])) i++; // skip parameters
            }
            // sub[i] is now the final letter of the sequence; the loop's i++ skips it
            if (sub[i] == '\0') break;
            continue;
        }
        if (isalnum((unsigned char)sub[i])){
            clean[k++] = tolower((unsigned char)sub[i]);
        } else if (sub[i] == ' ' && k > 0 && clean[k - 1] != ' '){
            clean[k++] = ' ';                    // keep single spaces only
        }
    }
    if (k > 0 && clean[k - 1] == ' ') k--;       // remove trailing space
    clean[k] = '\0';

    if (clean[0] == '\0'){
        printf("Error: Invalid subject name.\n");
        return 1;
    }
    strcpy(sub, clean);

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

    if(nos_topic <= 0){
        printf("Error: Number of topics must be at least 1.\n");
        return 1;
    }

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
   applyHistory(topics, nos_topic, prevTopics, prevCount);

   allocateTime(topics, nos_topic, time);
   displayStudyplan(topics, nos_topic);

   getUserFeedback(topics, nos_topic, time);
   askDifficultyFeedback(topics, nos_topic);

   showProgress(topics, nos_topic);

   
   printf("\n[AUTO] Saving plan to: %s\n", filename);

   savePlanToFile(filename, sub, time, topics, nos_topic);

    return 0;
}