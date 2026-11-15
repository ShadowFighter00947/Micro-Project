#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "topic.h"

void getUserFeedback(struct Topic *topics, int count, int totalTime, int *actions);
void adjustPlan(struct Topic *topics, int count, int totalTime, int *actions);
void askDifficultyFeedback(struct Topic *topics, int count);

#endif