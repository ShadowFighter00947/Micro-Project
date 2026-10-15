#ifndef TOPIC_H
#define TOPIC_H

struct Topic{
    char name[50];
    int difficulty;
    int priority;
    int score;
    int allocatedTime;
};

void inputTopic(struct Topic *topic);
void displayTopics(struct Topic *topics, int count);

#endif