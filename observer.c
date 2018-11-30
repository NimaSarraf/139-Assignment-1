//Nima Sarrafzadeh
//CSC 139

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

int BUFFER_SIZE = 500;

void stdDisplay();
void shortDisplay();
void longDisplay(int interval, int duration);
float sLoadAvg();

int main(int argc, char *argv[]){
    char repTypeName[16];
    char c1, c2;
    int interval, duration;

    time_t t = time(NULL);
    struct tm time = *localtime(&t);

    if (argc == 1){
        strcpy(repTypeName, "Standard");
        printf("\n>> Status report type %s at: %02d:%02d:%02d <<\n\n", repTypeName, time.tm_hour, time.tm_min, time.tm_sec);
        stdDisplay();
    }
    else if (argc > 1){
        sscanf(argv[1], "%c%c", &c1, &c2);

        if (c1 != '-') {
            fprintf(stderr, "usage: observer [-s][-l int dur]\n");
            exit(1);
        }

        if (c2 == 's'){
            strcpy(repTypeName, "Short");
            printf("\n>> Status report type %s at: %02d:%02d:%02d <<\n\n", repTypeName, time.tm_hour, time.tm_min, time.tm_sec);
            stdDisplay();
            shortDisplay();
        }
        if (c2 == 'l'){
            strcpy(repTypeName, "Long");
            printf("\n>> Status report type %s at: %02d:%02d:%02d <<\n\n", repTypeName, time.tm_hour, time.tm_min, time.tm_sec);
            interval = atoi(argv[2]);
            duration = atoi(argv[3]);
            shortDisplay();
            longDisplay(interval, duration);
        }
    }
}

void stdDisplay(){
    FILE *procFile;
    char str[BUFFER_SIZE];
    float sec;
    int days, hours, min, seconds;

    //Part B ------
    procFile = fopen("/proc/cpuinfo", "r");

    for (int i=0; i<5 ; i++){
        fgets(str, BUFFER_SIZE, procFile);
    }
    printf("%s", str);

    procFile = fopen("/proc/version", "r");
    fgets(str, BUFFER_SIZE, procFile);
    printf("%s", str);

    procFile = fopen("/proc/uptime", "r");
    fscanf(procFile, "%f", &sec);
    seconds = (int)sec;
    days = seconds/86400;
    seconds = seconds - (days*86400);
    hours = seconds/3600;
    seconds = seconds - (hours*3600);
    min = seconds/60;
    seconds = seconds - (min*60);

    printf("Uptime: %d:%d:%d:%d\n", days, hours, min, seconds);

    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    printf("System time: %02d:%02d:%02d\n",time.tm_hour, time.tm_min, time.tm_sec);

    procFile = fopen("/proc/sys/kernel/hostname", "r");
    fgets(str, BUFFER_SIZE, procFile);
    printf("Machine hostname: %s", str);
}

void shortDisplay(){
    FILE *procFile;
    char str[BUFFER_SIZE];
    char str2[BUFFER_SIZE];

    //Part C ------
    procFile = fopen("/proc/stat", "r");
    fscanf(procFile, "%s", str);

    fscanf(procFile, "%s", str);
    printf("Seconds spent in user mode: %s\n", str);

    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);

    printf("Seconds spent in system mode: %s\n", str);
    fscanf(procFile, "%s", str);

    printf("Seconds spent idling: %s\n", str);

    procFile = fopen("/proc/diskstats", "r");
    for (int i=0; i < 23; i++){
        fgets(str, BUFFER_SIZE, procFile);
    }
    fgets(str, BUFFER_SIZE, procFile);
    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);

    fscanf(procFile, "%s", str);
    printf("Disk read requests: %s\n", str);

    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);
    printf("Disk write requests: %s\n", str);

    procFile = fopen("/proc/1/status", "r");
    for (int i=0; i < 39 ; i++){
        fgets(str, BUFFER_SIZE, procFile);
        if (i == 37){
            printf("%s", str);
            fgets(str, BUFFER_SIZE, procFile);
            printf("%s", str);
        }
    }

    char *rebootCmd = "who -b";
    FILE *filePtr;

    filePtr = popen(rebootCmd, "r");

    fscanf(filePtr, "%s", str);
    fscanf(filePtr, "%s", str);
    fscanf(filePtr, "%s", str);
    fscanf(filePtr, "%s", str2);
    printf("Last system boot: %s %s\n",str, str2);


    procFile = fopen("/proc/stat", "r");

    for (int i=0; i<9 ; i++){
        fgets(str, BUFFER_SIZE, procFile);
    }
    fscanf(procFile, "%s", str);
    fscanf(procFile, "%s", str);
    printf("Processes create since system was booted: %s\n", str);
}

void longDisplay(int interval, int duration){
    FILE* procFile;
    char str[BUFFER_SIZE];
    float loadAvg;
    float loadAvgInterval;
    int iteration = 0;


    //Part D -----
    procFile = fopen("/proc/meminfo", "r");
    fgets(str, BUFFER_SIZE, procFile);
    printf("%s", str);

    procFile = fopen("/proc/meminfo", "r");
    for (int i=0; i<2 ; i++){
        fgets(str, BUFFER_SIZE, procFile);
    }
    printf("%s", str);

    loadAvg = sLoadAvg();
    printf("Load avg over the last minute: %.2f\n", loadAvg);

    loadAvg = 0;
    while (iteration < duration){
        sleep(interval);
        loadAvg += sLoadAvg();
        iteration += interval;
    }
    loadAvgInterval = loadAvg/(duration/interval);
    printf("Load avg over %d seconds sampled every %d second(s): %f\n",\
            duration, interval, loadAvgInterval);
}

float sLoadAvg(){
    FILE* procFile;
    float loadAvg;
    procFile = fopen("/proc/loadavg", "r");
    fscanf(procFile, "%f", &loadAvg);
    return loadAvg;
}
