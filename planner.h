#ifndef PLANNER_H
#define PLANNER_H

// Function to calculate time for all topics.
void allocateTime(struct Topic *topics, int count, int totalTime);

// Blend current scores with the last saved session.
void applyHistory(struct Topic *topics, int count, struct Topic*prev, int prevCount);

// Function to display the final study plan.
void displayStudyplan(struct Topic *topics, int count);

#endif