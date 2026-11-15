#include <stdio.h>
#include <string.h>
#include "topic.h"
#include "planner.h"
#include "agent.h"
#include "feedback.h"
#include "ui.h"


// Reads one line and returns the number, asking again until it is between min and max.
// If input is closed (Ctrl+D), it returns min so the program cannot loop forever.
static int readInt(const char *prompt, int min, int max){
    char line[100];
    int value;

    while (1){
        printf("%s", prompt);
        if (fgets(line, sizeof line, stdin) == NULL) return min;
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max)
            return value;
        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

void getUserFeedback(struct Topic *topics, int count, int totalTime, int *actions){
    char line[100];
    int satisfied = 0;

    uiSection("FEEDBACK");

    while (!satisfied){
        printf("Are you satisfied with this plan? (y/n): ");
        if (fgets(line, sizeof line, stdin) == NULL) return; // input closed

        char response = '\0';
        for (int i = 0; line[i]; i++){
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'){
                response = line[i];
                break;
            }
        }

        if (response == 'y' || response == 'Y'){
            uiMessage("OK", "Great! Your plan is ready.");
            satisfied = 1;
        }
        else if (response == 'n' || response == 'N'){
            adjustPlan(topics, count, totalTime, actions);
        }
        else{
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
}

void adjustPlan(struct Topic *topics, int count, int totalTime, int *actions){
    char prompt[60];
    snprintf(prompt, sizeof prompt, "Which topic do you want to improve? (1-%d): ", count);
    int choice = readInt(prompt, 1, count) - 1;   // Convert to 0-based index.

    printf("\nCurrent values for %s:\n", topics[choice].name);
    printf("  Difficulty: %d/5\n", topics[choice].difficulty);
    printf("  Priority  : %d/5\n", topics[choice].priority);
    printf("  Score     : %d%%\n\n", topics[choice].score);

    topics[choice].difficulty = readInt("Enter new difficulty (1-5): ", 1, 5);
    topics[choice].priority   = readInt("Enter new priority (1-5): ", 1, 5);
    topics[choice].score      = readInt("Enter new previous score (0-100): ", 0, 100);

    // Refresh this topic's action from its new score.
    if (topics[choice].score < LOW_SCORE)
        actions[choice] = ACTION_STUDY;
    else if (topics[choice].score > HIGH_SCORE && actions[choice] == ACTION_REVIEW)
        actions[choice] = ACTION_REVIEW;
    else
        actions[choice] = ACTION_REVISE;

    // Same pipeline as the first plan, so all the time rules still apply.
    allocateTime(topics, count, totalTime);
    applyTimeLimits(topics, count, totalTime, actions);

    uiSection("UPDATED STUDY PLAN");
    printStudyTable(topics, count, actions);
}

void askDifficultyFeedback(struct Topic *topics, int count){
    uiSection("DIFFICULTY RATING");
    printf("How hard was each topic?\n");
    printf("1 = Very Easy, 2 = Easy, 3 = Medium, 4 = Hard, 5 = Very Hard\n");

    for (int i = 0; i < count; i++){
        char prompt[80];
        snprintf(prompt, sizeof prompt, "  %s (1-5): ", topics[i].name);
        topics[i].difficulty = readInt(prompt, 1, 5);
    }
}
