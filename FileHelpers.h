// Mahmoud Mostafa, 320250263
// Contributors: Amr Hany

#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// per minute snapshot written during the simulation loop
struct MinuteSnapshot
{
    int minute;
    int queue_length;
    int customers_served;
    double avg_wait_time;
};

inline void readSettings(int &numServers, int &arrivalProbability, int &maxServiceTime, int &maxSimulationTime)
{
    ifstream inFile("settings.txt");
    if (inFile.is_open())
    {
        inFile >> numServers >> arrivalProbability >> maxServiceTime >> maxSimulationTime;
        inFile.close();
        cout << "Settings loaded from settings.txt\n";
    }
    else
    {
        cout << "settings.txt not found — using defaults\n";
        numServers        = 3;
        arrivalProbability = 30;
        maxServiceTime    = 8;
        maxSimulationTime = 60;
    }
}

inline void writeResults(const vector<MinuteSnapshot> &snapshots)
{
    ofstream outFile("results.csv");
    if (!outFile.is_open())
    {
        cout << "ERROR: couldn't create results.csv\n";
        return;
    }

    outFile << "minute,queue_length,customers_served,avg_wait_time\n"; // header for the file
    for (const auto &s : snapshots)
        outFile << s.minute << "," << s.queue_length << ","    
                << s.customers_served << "," << s.avg_wait_time << "\n";

    outFile.close();
    cout << "Results written to results.csv\n";
}

#endif
