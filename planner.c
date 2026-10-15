#include <stdio.h>
#include "topic.h"
#include "planner.h"

void allocateTime(struct Topic *topics, int count, int totalTime){
    // Step 1:- Calculate weight for each topic.
    float weights[count];
    float totalWeight = 0;
    
    for (int i = 0; i < count; i++){
        float difficulty = topics[i].difficulty * 0.3;
        float priority = topics[i].priority * 0.3;
        float score = (100 - topics[i].score) * 0.4;

        weights[i] = difficulty + priority + score;
        totalWeight += weights[i];
    }

    // Step 2:- Allocate time based on weight.
    for (int i = 0; i < count; i++){
        topics[i].allocatedTime = (weights[i] / totalWeight) * totalTime;
    } 
}

void displayStudyplan(struct Topic *topics, int count){
    printf("\n=============================\n");
    printf("    YOUR ADAPTIVE STUDY PLAN  \n");
    printf("=============================\n");

    for (int i = 0; i < count; i++){
        printf("\n%d. %s\n", i + 1, topics[i].name);
        printf("   Difficulty : %d/5\n", topics[i].difficulty);
        printf("   Priority   : %d/5\n", topics[i].priority);
        printf("   Score      : %d%%\n", topics[i].score);
        printf("   Time Given : %d min\n", topics[i].allocatedTime);
        printf("   --------------------\n");
    }
}