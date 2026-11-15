#include <stdio.h>
#include <ctype.h>
#include "topic.h"
#include "agent.h"
#include "ui.h"

#define MIN_SHARE    10  // Minimum % of total time per study topic.
#define MAX_SHARE    40  // Maximum % of total time per study topic.
#define REVIEW_SHARE 5   // % of total time for a review-only topic.


static int sameTopic(const char *a, const char *b){
    while (*a && *b){
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static int findPrev(const char *name, const struct Topic *prev, int prevCount){
    for (int j = 0; j < prevCount; j++){
        if (sameTopic(name, prev[j].name)) return j;
    }
    return -1;
}

// Step 1: decide an action for each topic and make automatic priority changes.
void runAgent(struct Topic *topics, int count, const struct Topic *prev, int prevCount, int *actions){
    int logged = 0;

    char msg[200];

    for (int i = 0; i < count; i++){
        int now = topics[i].score;
        int j = findPrev(topics[i].name, prev, prevCount);

        actions[i] = (now < LOW_SCORE) ? ACTION_STUDY : ACTION_REVISE;
        if (j < 0) continue; // No history for this topic.

        int last = prev[j].score;

        // Repeat struggle: low in both the last session and this one.
        if (last < LOW_SCORE && now < LOW_SCORE){
            if (topics[i].priority < 5){
                topics[i].priority++;
            }
            snprintf(msg, sizeof msg, "%s: below %d%% twice (%d%%, %d%%), priority raised to %d", topics[i].name, LOW_SCORE, last, now, topics[i].priority);
            uiMessage("AGENT", msg);
            logged = 1;
        }

        // Mastery: high in both the last session and this one.
        if (last > HIGH_SCORE && now > HIGH_SCORE){
            actions[i] = ACTION_REVIEW;
            snprintf(msg, sizeof msg, "%s: above %d%% twice (%d%%, %d%%), moved to review slot", topics[i].name, HIGH_SCORE, last, now);
            uiMessage("AGENT", msg);
            logged = 1;
        }
    }

    if (!logged) uiMessage("AGENT", "No automatic changes.");
}

// Step 2: enforce the time rules. Review topics get a fixed small slot,
// and study topics are kept between MIN_SHARE and MAX_SHARE of the total.
void applyTimeLimits(struct Topic *topics, int count, int totalTime, const int *actions){
    int reviewTotal = 0;
    int studyCount = 0;
    int studySum = 0;
    char msg[200];

    for (int i = 0; i < count; i++){
        if (actions[i] == ACTION_REVIEW){
            topics[i].allocatedTime = totalTime * REVIEW_SHARE / 100;
            reviewTotal += topics[i].allocatedTime;
        } else {
            studyCount++;
            studySum += topics[i].allocatedTime;
        }
    }

    if (studyCount == 0){
        // Everything is mastered: share the time evenly.
        for (int i = 0; i < count; i++) topics[i].allocatedTime = totalTime / count;
        topics[0].allocatedTime += totalTime % count;
        return; 
    }


    int budget = totalTime - reviewTotal;

    // Scale the study topics so they fill the remaining budget.
    for (int i = 0; i < count; i++){
        if (actions[i] == ACTION_REVIEW) continue;
        if (studySum > 0)
            topics[i].allocatedTime = topics[i].allocatedTime * budget / studySum;
        else
            topics[i].allocatedTime = budget / studyCount;
    }

       // Enforce min and max only when the limits can actually be met.
    int minT = totalTime * MIN_SHARE / 100;
    int maxT = totalTime * MAX_SHARE / 100;
    int enforce = (studyCount >= 3 &&
                   minT * studyCount <= budget &&
                   maxT * studyCount >= budget);

    if (enforce){
        for (int i = 0; i < count; i++){
            if (actions[i] == ACTION_REVIEW) continue;
            if (topics[i].allocatedTime < minT){
                snprintf(msg, sizeof msg, "%s: raised to minimum %d min", topics[i].name, minT);
                uiMessage("AGENT", msg);
                topics[i].allocatedTime = minT;
            }
            if (topics[i].allocatedTime > maxT){
                snprintf(msg, sizeof msg, "%s: capped at maximum %d min", topics[i].name, maxT);
                uiMessage("AGENT", msg);
                topics[i].allocatedTime = maxT;
            }
        }
    }

    // Make the total exact by moving the leftover one minute at a time,
    // never pushing a study topic outside its allowed range.
    int used = 0;
    for (int i = 0; i < count; i++) used += topics[i].allocatedTime;

    int diff = totalTime - used;
    int step = (diff > 0) ? 1 : -1;
    int lo = enforce ? minT : 0;
    int hi = enforce ? maxT : totalTime;

    while (diff != 0){
        int moved = 0;
        for (int i = 0; i < count && diff != 0; i++){
            if (actions[i] == ACTION_REVIEW) continue;
            int next = topics[i].allocatedTime + step;
            if (next >= lo && next <= hi){
                topics[i].allocatedTime = next;
                diff -= step;
                moved = 1;
            }
        }
        if (!moved) break; // No topic can take more.
    }

    if (diff != 0){ // Fallback: first study topic takes the rest.
        for (int i = 0; i < count; i++){
            if (actions[i] != ACTION_REVIEW){
                topics[i].allocatedTime += diff;
                break;
            }
        }
    }
}



void printAgentActions(const struct Topic *topics, int count, const int *actions){
    printf("\nSuggested actions:\n");
    for (int i = 0; i < count; i++){
        const char *label = "Study";
        if (actions[i] == ACTION_REVISE) label = "Revise";
        if (actions[i] == ACTION_REVIEW) label = "Review only";
        printf("  %d. %s -> %s\n", i + 1, topics[i].name, label);
    }
}
