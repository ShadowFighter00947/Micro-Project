#include <stdio.h>
#include <ctype.h>
#include "topic.h"
#include "planner.h"

static int sameName(const char *a, const char *b){
    while(*a && *b){
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static float performanceFactor(int score){
    if (score < 50) return 1.3f;
    if(score <= 75) return 1.0;
    return 0.7f;
}

static const char *reasonFor(int score){
    if (score < 50) return "+ time (low score)";
    if (score <= 75) return "normal time";
    return "- time (strong score)"; 
}

void applyHistory(struct Topic *topics, int count, struct Topic *prev, int prevCount){
    for (int i = 0; i < count; i++){
        for (int j = 0; j < prevCount; j++){
            if (sameName(topics[i].name, prev[j].name)){
                int entered = topics[i].score;
                int enteredDiff = topics[i].difficulty;

                topics[i].score = (int)(entered * 0.7f + prev[j].score * 0.3f + 0.5f);
                
                if(prev[j].difficulty > 0){
                    topics[i].difficulty = (int)(enteredDiff * 0.7f + prev[j].difficulty * 0.3f + 0.5f);
                }
                printf("[HISTORY] %s: entered %d%%, last session %d%%, using %d%%\n", topics[i].name, entered, prev[j].score, topics[i].score);
                printf("[HISTORY] %s: difficulty entered %d, last feedback %d, using %d\n", topics[i].name , enteredDiff, prev[j].difficulty, topics[i].difficulty);
                break;
            }
        }
    }
}

void allocateTime(struct Topic *topics, int count, int totalTime){
    // Step 1:- Calculate weight for each topic.
    float weights[count];
    float totalWeight = 0;
    
    for (int i = 0; i < count; i++){
        float difficulty = topics[i].difficulty * 0.3;
        float priority = topics[i].priority * 0.3;
        float score = (100 - topics[i].score) * 0.4;

        weights[i] = (difficulty + priority + score) * performanceFactor(topics[i].score);
        totalWeight += weights[i];
    }

    // Step 2:- Allocate time based on weight.
    for (int i = 0; i < count; i++){
        if (totalWeight == 0){
            topics[i].allocatedTime = totalTime / count;
        }else {
            topics[i].allocatedTime = (int)((weights[i] / totalWeight) * totalTime + 0.5f);
        }
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
        printf("   Reason     : %s\n", reasonFor(topics[i].score));
        printf("   --------------------\n");
    }
}
