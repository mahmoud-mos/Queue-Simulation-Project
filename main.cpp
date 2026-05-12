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

    //Load parameters from settings.txt (written by flask)
    int numServers, arrivalProbability, maxServiceTime, maxSimulationTime;
    readSettings(numServers, arrivalProbability, maxServiceTime, maxSimulationTime);

    // ── Stats ──
    int totalCustomersServed = 0;
    double totalWaitTime = 0;
    int customerNumber = 1;
    int currentTime = 0;
    int maxQueueLength = 0;

    //Per minute snapshots for the CSV filr 
    vector<MinuteSnapshot> snapshots;
    Queue<Customer> customerQueue;
    Server* servers = new Server[numServers];
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
            Customer newCustomer(customerNumber, currentTime, svcTime);
            customerQueue.enqueue(newCustomer);
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
                servers[i].assignCustomer(next);
            }
        }

        //recording snapshots for this minute
        double avgWait = (totalCustomersServed > 0)
            ? totalWaitTime / totalCustomersServed
            : 0.0;
        snapshots.push_back({ currentTime + 1,
                              customerQueue.size(),
                              totalCustomersServed,
                              avgWait });

        currentTime++;
    }

    cout << "--simulation ended--\n";
    cout << "total served      = " << totalCustomersServed << "\n";
    cout << "max queue length  = " << maxQueueLength << "\n";
    double finalAvg = (totalCustomersServed > 0) ? totalWaitTime / totalCustomersServed : 0.0;
    cout << "avg wait time     = " << finalAvg << " minutes\n";
    cout << "still in queue    = " << customerQueue.size() << " customers\n";

    writeResults(snapshots);

    delete[] servers;
    return 0;
}
