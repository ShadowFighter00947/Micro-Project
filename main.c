#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "topic.h"
#include "planner.h"
#include "feedback.h"
#include "fileio.h"
#include "progress.h"
#include "agent.h"
#include "ui.h"

int main()
{
    char sub[50]; // Takes Max 50 characters.
    int time, nos_topic;

    uiSection("AI/ML STUDY PLANNER");
    printf("Plan your study time, track your progress, and improve each session.\n\n");

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

    uiSection("HISTORY");
    printf("Sessions found: %d\n", sessions);
    for (int i = 0; i < prevCount; i++){
        printf("[LOADED] %s | Difficulty: %d | Priority: %d | Score: %d | Time: %d min\n", prevTopics[i].name, prevTopics[i].difficulty, prevTopics[i].priority, prevTopics[i].score, prevTopics[i].allocatedTime);
    }

    printf("Enter Time available (in minutes):- ");

    if (scanf("%d", &time) != 1 || time <= 0){
        printf("ERROR: Time must be at least 1 minute.\n");
        return 1;
    }
    getchar();

    printf("Enter number of Topics:- ");
    if (scanf("%d", &nos_topic) != 1 || nos_topic <= 0){
        printf("ERROR: Number of topics must be at least 1.\n");
        return 1;
    }
    getchar();

    struct Topic topics[nos_topic];


    for (int i = 0; i < nos_topic; i++) 
{
    printf("\nTopic %d:\n", i + 1);
    inputTopic(&topics[i]); // This calls the function in topic.c
}



// Below section of code shows everything users has given in one go.
    uiSection("SETUP");
    printf("Subject: %s\n", sub);
    printf("Time available: %d min\n", time);
    printf("Topics: %d\n", nos_topic);

    uiSection("TOPICS");

   displayTopics(topics, nos_topic); // This calls the function in topic.
   applyHistory(topics, nos_topic, prevTopics, prevCount);

   int actions[nos_topic];
   uiSection("AGENT");
   runAgent(topics, nos_topic, prevTopics, prevCount, actions);

   allocateTime(topics, nos_topic, time);
   applyTimeLimits(topics, nos_topic, time, actions);

   uiSection("STUDY PLAN");
   printStudyTable(topics, nos_topic, actions);

   getUserFeedback(topics, nos_topic, time, actions);
   askDifficultyFeedback(topics, nos_topic);

   uiSection("PROGRESS");
   showProgress(topics, nos_topic); 

     savePlanToFile(filename, sub, time, topics, nos_topic);

    printf("\nPlan saved to %s\n\n", filename);

#ifdef _WIN32
    system("pause");              // Shows "Press any key to continue . . ."
#else
    printf("Press Enter to exit...");
    getchar();
#endif

    return 0;
}