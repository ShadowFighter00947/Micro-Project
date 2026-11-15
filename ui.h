#ifndef UI_H
#define UI_H

#include "topic.h"

void uiSection(const char *title);
void uiMessage(const char *tag, const char *text);
void printStudyTable(const struct Topic *topics, int count, const int *actions);
void printProgressBar(int percent);

#endif