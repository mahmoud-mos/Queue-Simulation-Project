// Name: Nour eldin Majd Salem , ID:320250277
// contributors: Mahmoud Mostafa, Abdulrahman Magdy, Amr Hany

#include "Queue.h"
#include "Models.h"
#include "FileHelpers.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

int main()
{
    srand(time(0));

    // Load parameters from settings.txt (written by flask)
    int numServers, arrivalProbability, maxServiceTime, maxSimulationTime;
    readSettings(numServers, arrivalProbability, maxServiceTime, maxSimulationTime);

    // Stats
    int totalCustomersServed = 0;
    int totalVipServed = 0;     // Vip
    int totalRegularServed = 0; // regular
    double totalWaitTime = 0;
    int customerNumber = 1;
    int currentTime = 0;
    int maxQueueLength = 0;

    // Per minute snapshots for the CSV file
    vector<MinuteSnapshot> snapshots;
    Queue<Customer> customerQueue;
    Server *servers = new Server[numServers];
    for (int i = 0; i < numServers; i++)
        servers[i] = Server(i + 1);

    cout << "--simulation started | time=" << maxSimulationTime
         << " servers=" << numServers
         << " arrivalProb=" << arrivalProbability << "%"
         << " maxSvc=" << maxServiceTime << "--\n";

    while (currentTime < maxSimulationTime)
    {
        if ((rand() % 100 + 1) <= arrivalProbability)
        {
            int svcTime = (rand() % maxServiceTime) + 1;

            // new customers is 10% chance to be a VIP
            bool isVip = (rand() % 100 + 1) <= 10;

            Customer newCustomer(customerNumber, currentTime, svcTime, isVip);
            customerQueue.enqueue(newCustomer);

            cout << (isVip ? "[VIP] " : "[Regular] ") << "Customer #" << customerNumber << " arrived.\n";
            customerNumber++;
        }

        if (customerQueue.size() > maxQueueLength)
            maxQueueLength = customerQueue.size();

        for (int i = 0; i < numServers; i++)
            servers[i].tick();

        for (int i = 0; i < numServers; i++)
        {
            if (!servers[i].isBusy() && !customerQueue.empty())
            {
                Customer next = customerQueue.front();
                customerQueue.dequeue();

                int waitTime = currentTime - next.getArrivalTime();
                next.setWaitTime(waitTime);
                totalWaitTime += waitTime;
                totalCustomersServed++;

                // track VIP vs regular
                if (next.getIsVip())
                    totalVipServed++;
                else
                    totalRegularServed++;

                cout << "Server " << servers[i].getId() << " took "
                     << (next.getIsVip() ? "[VIP] " : "[Regular] ") << "Customer #" << next.getId() << "\n";

                servers[i].assignCustomer(next);
            }
        }

        // recording snapshots for this minute for vip/regular
        double avgWait = (totalCustomersServed > 0)
                             ? totalWaitTime / totalCustomersServed
                             : 0.0;
        snapshots.push_back({currentTime + 1,
                             customerQueue.size(),
                             totalCustomersServed,
                             totalVipServed,     // vip
                             totalRegularServed, // regular
                             avgWait});

        currentTime++;
    }

    cout << "--simulation ended--\n";
    cout << "total served      = " << totalCustomersServed << "\n";
    cout << "vip served        = " << totalVipServed << "\n";     // vip
    cout << "regular served    = " << totalRegularServed << "\n"; // regular
    cout << "max queue length  = " << maxQueueLength << "\n";
    double finalAvg = (totalCustomersServed > 0) ? totalWaitTime / totalCustomersServed : 0.0;
    cout << "avg wait time     = " << finalAvg << " minutes\n";
    cout << "still in queue    = " << customerQueue.size() << " customers\n";

    writeResults(snapshots);

    delete[] servers;
    return 0;
}
