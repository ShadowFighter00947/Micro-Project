#include <stdio.h>
#include <string.h>
#include "topic.h"

// Keeps asking until the user enters a whole number between min and max.
static int readInt(const char *prompt, int min, int max){
    char line[100];
    int value;

    while (1){
        printf("%s", prompt);
        if (fgets(line, sizeof line, stdin) == NULL) return min; // input closed
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max)
            return value;
        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

void inputTopic(struct Topic *topic){
    printf("Enter topic name: ");
    if (scanf(" %49[^\n]", topic->name) != 1)
        strcpy(topic->name, "Topic");

    // Discard anything left on the line (e.g. a name longer than 49 characters).
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    topic->difficulty = readInt("Enter difficulty (1-5): ", 1, 5);
    topic->priority   = readInt("Enter priority (1-5): ", 1, 5);
    topic->score      = readInt("Enter previous score (0-100): ", 0, 100);
    topic->allocatedTime = 0;
}

void displayTopics(struct Topic *topics, int count){
    printf("\nEntered Topics:\n");
    for(int i = 0; i < count; i++){
        printf("%d. %s - Difficulty: %d, priority: %d, score: %d, Time: %d min\n",
               i + 1, topics[i].name, topics[i].difficulty,
               topics[i].priority, topics[i].score, topics[i].allocatedTime);
    }
}
