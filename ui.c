#include <stdio.h>
#include "ui.h"
#include "agent.h"
// Prints a section header.
void uiSection(const char *title){
    printf("\n==== %s ====\n", title);
}

// Prints a tagged message, e.g. "[AGENT] Maths: priority raised to 3".
void uiMessage(const char *tag, const char *text){
    printf("[%s] %s\n", tag, text);
}

// Maps the numeric priority (1 to 5) to a label.
static const char *priorityLabel(int priority){
    if (priority >= 4) return "High";
    if (priority >= 2) return "Medium";
    return "Low";
}

// Prints one horizontal border line for the table.
static void printBorder(void){
   printf("+----------------+------------------+----------+------------+\n"); 
}

// Prints the studty plan table with a Total row at the bottom.
void printStudyTable(const struct Topic *topics, int count, const int *actions){
    int total = 0;

    printBorder();
    printf("| %-14s | %-16s | %-8s | %-10s |\n", "Topic", "Action", "Time", "Priority");
    printBorder();

    for (int i = 0; i < count; i++){
        const char *label = "Study";
        if (actions[i] == ACTION_REVISE) label = "Revise";
        if (actions[i] == ACTION_REVIEW) label = "Review only";

        printf("| %-14.14s | %-16s | %4d min | %-10s |\n", topics[i].name, label, topics[i].allocatedTime, priorityLabel(topics[i].priority));
        total += topics[i].allocatedTime;
    }

    printBorder();
    printf("| %-14s | %-16s | %4d min | %-10s |\n", "Total", "", total, "");
    printBorder();
}

// Prints the overall progress bar.
void printProgressBar(int percent){
    int filled = percent / 5; // 20 blocks total, each block = 5%
    if (filled > 20) filled = 20;
    if (filled < 0) filled = 0;

    printf("Overall Progress: [");
    for (int i = 0; i < 20; i++){
        printf("%c", i < filled ? '#' : '-');
    }
    printf("] %d%%\n", percent);
}