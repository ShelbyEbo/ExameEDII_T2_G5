#ifndef REPORT_H
#define REPORT_H

#include "chat.h"
#include "graph.h"
#include "file.h"

typedef struct
{
    File *file;
    int count;
} FileCount;

void report_shares_per_user(const ShareGraph *g);
void report_top_sender(const ShareGraph *g);
void report_top_receiver(const ShareGraph *g);
void report_most_shared_files(const ShareGraph *g);
void report_total_members(const ShareGraph *g);
void report_all(const ShareGraph *g);

#endif