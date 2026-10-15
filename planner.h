#ifndef PLANNER_H
#define PLANNER_H

// Function to calculate time for all topics.
void allocateTime(struct Topic *topics, int count, int totalTime);

// Function to display the final study plan.
void displayStudyplan(struct Topic *topics, int count);

#endif