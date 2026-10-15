#include <stdio.h>
#include "topic.h"
#include "planner.h"
#include "feedback.h"

void getUserFeedback(struct Topic *topics, int count, int totalTime){
    char response;
    int satisfied = 0;

    while (!satisfied){
        printf("\nAre you satisfied with this plan? (y/n): ");
        scanf(" %c", &response);

        if (response == 'y' || response == 'Y'){
            printf("\nGreat.! Your plan is ready.\n");
            satisfied = 1; // Exit loop.
        }
        else if (response == 'n' || response == 'N'){
            adjustPlan(topics, count, totalTime);
        }
        else{
            printf("\nInvalid input. Please enter 'y' or 'n'.\n");
        }
    }
}

void adjustPlan(struct Topic *topics, int count, int totalTime){
    int choice;

    printf("\nWhich topic do you want to improve? (enter topic number 1-%d): ", count);
    scanf("%d", &choice);

    if (choice < 1 || choice > count){
        printf("Invalid topic number.\n");
        return;
    }

    choice--; // Convert to array index (0-based).

    printf("\nCurrent value for %s:\n", topics[choice].name);
    printf(" Difficulty: %d/5\n", topics[choice].difficulty);
    printf(" Priority: %d/5\n", topics[choice].priority);
    printf(" Score: %d%%\n\n", topics[choice].score);

    printf("Enter new difficulty (1-5): ");
    scanf("%d", &topics[choice].difficulty);

    printf("Enter new priority (1-5): ");
    scanf("%d", &topics[choice].priority);

    printf("Enter new previous score (0-100): ");
    scanf("%d", &topics[choice].score);

    allocateTime(topics, count, totalTime);
    displayStudyplan(topics, count);
}
