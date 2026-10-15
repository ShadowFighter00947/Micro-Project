#ifndef FILEIO_H
#define FILEIO_H

#include "topic.h"

// Saves the plan to a text file.
void savePlanToFile(const char *filename, const char *subject, int totalTime, struct Topic *topics, int count);

// Load plan from file.
void loadPlanFromFile(const char *filename, struct Topic *topics, int *count);

#endif