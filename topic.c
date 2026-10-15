#include <stdio.h>
#include "topic.h"

void inputTopic(struct Topic *topic){
    printf("Enter topic name: ");
    scanf(" %[^\n]", topic->name);

    printf("Enter difficulty (1-5): ");
    scanf("%d", &topic->difficulty);
    getchar();

    printf("Enter priority (1-5): ");
    scanf("%d", &topic->priority);
    getchar();

    printf("Enter previous score (0-100): ");
    scanf("%d", &topic->score);
    getchar();

    topic->allocatedTime = 0;
}

void displayTopics(struct Topic *topics, int count){
    printf("\nEntered Topics:\n");
    for(int i = 0; i < count; i++){
        printf("%d. %s - Difficulty: %d, priority: %d, score: %d, Time: %d min\n", i + 1, topics[i].name, topics[i].difficulty, 
            topics[i].priority, topics[i].score, topics[i].allocatedTime);
    }
}