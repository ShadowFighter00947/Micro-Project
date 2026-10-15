#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "topic.h"

void getUserFeedback(struct Topic *topics, int count, int totalTime);
void adjustPlan(struct Topic *topics, int count, int totalTime);

#endif