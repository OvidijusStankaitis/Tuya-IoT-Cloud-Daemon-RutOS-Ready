#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "getSysInfo.h"

double get_memory_usage()
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL)
    {
        syslog(LOG_ERR, "Error opening /proc/meminfo");
        return -1;
    }

    double total_memory;
    double free_memory;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "MemTotal: %lf kB", &total_memory))
        {
            continue;
        }
        else if (sscanf(line, "MemAvailable: %lf kB", &free_memory))
        {
            continue;
        }
    }

    fclose(file);

    double used_memory = (total_memory - free_memory) / (1024*1024);
    return used_memory;
}