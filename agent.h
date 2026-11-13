#ifndef AGENT_H
#define AGENT_H

#include "topic.h"

#define ACTION_STUDY 0
#define ACTION_REVISE 1
#define ACTION_REVIEW 2

void runAgent (struct Topic *topics, int count, const struct Topic *prev, int prevCount, int *actions);
void applyTimeLimits(struct Topic *topics, int count, int totalTime, const int *actions);
void printAgentActions(const struct Topic *topics, int count, const int *actions);

#endif