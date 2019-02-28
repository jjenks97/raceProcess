#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>

void raceTortoise(int racerId, double tMin, double tMax, int tInterval, int raceLength) {

    double totalDistance= 0;
    double rate;
    double distance;
    
    struct timespec ts;
    ts.tv_nsec = 1000000 * tInterval;

    while( totalDistance < raceLength ) {
        // sleep tInterval
        
        nanosleep( &ts, NULL );
        // update distance traveled
        // compute rate (random number bewteen tMin and tMax)
        // compute distance = rate * time 
        // add distance to totalDistance

        //print status
        printf("Tortoise %d\t\t%f\t%f\t%f\n", racerId, speed, distance, totalDistance);

    }

}

int main(int argc, char * argv[]) {
    // check argument number
    if(argc != 11) {
        printf("%s takes 10 paramters\n", argv[0]);
        return 1;
    }

    // parse the parameters 
    int tNum = atoi(argv[1]);
    double tMin = atof(argv[2]);
    double tMax = atof(argv[3]);
    int tInterval = atoi(argv[4]);
    int hNum = atoi(argv[5]);
    double hProb = atof(argv[6]);
    double hInterval = atof(argv[7]);
    double hMin = atof(argv[8]);
    double hMax = atof(argv[9]);
    double raceLength = atof(argv[10]);

    int racerCount = tNum + hNum;
    
    // check the min/max


    // Fork the tortioses and hares
    pid_t forkId;
    pid_t childIds[racerCount];
    pid_t racerId = 0;
    int isTortoise = 0;
    int isHare = 0;

    for(int i=0; i < racerCount; i++) {
        forkId = fork();
        if(forkId == 0) {
            // in the child
            if( i < tNum ) {
                isTortoise = 1;
                racerId = i + 1;
            }
            else {
                isHare = 1;
                racerId = i + 1 - tNum;
            }
            break;
        }

        if( forkId  < 0) {
            perror("fork");
            return -1;
        }
        childIds[i] = forkId;
    }

    // Child
    if (forkId == 0) {
        if(isTortoise) {
            //run the tortoise function
            raceTortoise(racerId, tMin, tMax, tInterval, raceLength);
        }
        else if (isHare) {
            raceHare(racerId, hMin, hMax, hInterval, hProb, raceLength);
        }
        else {
            printf("ERROR: This shouldn't happen.")
        }
        exit(0);
    }
    // Parent
    else {
        printf("Racer\t\tSpeed\tDist.\tTotal Dist.\n");
        int childReturnStatus;
        // wait for the first child process to end
        pid_t winnerId = wait( &childReturnStatus );
        
        // print out the info about the winner

        // kill all the other processes 
        for(int i=0; i < racerCount; i++) {
            kill(childIds[i], SIGTERM);    
        }
    }
    
    return 0;

}