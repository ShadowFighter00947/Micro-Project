#ifndef AGENT_H
#define AGENT_H

#include "topic.h"

#define LOW_SCORE  50   // Below this counts as struggling.
#define HIGH_SCORE 85   // Above this counts as mastered.

// Actions the agent can choose for a topic.
#define ACTION_STUDY  0
#define ACTION_REVISE 1
#define ACTION_REVIEW 2

// Step 1: choose an action for each topic and adjust priorities from history.
void runAgent(struct Topic *topics, int count, const struct Topic *prev, int prevCount, int *actions);

// Step 2: enforce the time rules (review slots, minimum and maximum share).
void applyTimeLimits(struct Topic *topics, int count, int totalTime, const int *actions);

// Prints the suggested action for each topic.
void printAgentActions(const struct Topic *topics, int count, const int *actions);

#endif
