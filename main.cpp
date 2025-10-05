#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <queue>
#include <algorithm>

using namespace std;

struct Node
{
    string stationName;
    string lineName;
    double distance;
};

unordered_map<string, vector<Node>> metroMap;

void build_graph(string filename)
{
    ifstream file(filename);
    string line;
    string currentLine;
    string previousStation;
    double previousDistance = 0.0;

    while(getline(file, line))
    {
        vector<string> tokens;
        string token;
        for(int i=0; i<line.size(); i++)
        {
            if(line[i] == ',')
            {
                tokens.push_back(token);
                token.clear();
                continue;
            }
            token.push_back(line[i]);
        }
        
        if(tokens.size() == 1)
        {
            currentLine = tokens[0];
            previousStation.clear();
            previousDistance = 0.0;
        }
        else
        {
            if(previousStation.empty())
            {
                previousStation = tokens[0];
                previousDistance = stod(tokens[1]);
            }
            else
            {
                double currentDistance = stod(tokens[1]);
                double distanceDiff = abs(currentDistance - previousDistance);

                metroMap[previousStation].push_back({tokens[0], currentLine, distanceDiff});
                metroMap[tokens[0]].push_back({previousStation, currentLine, distanceDiff});

                previousStation = tokens[0];
                previousDistance = currentDistance;

                if(tokens.size() == 3)
                {
                    metroMap[tokens[0]].push_back({tokens[0], currentLine, 0.0});
                    metroMap[tokens[0]].push_back({tokens[0], tokens[2], 0.0});
                }
            }
        }
    }
}

int calculateFare(double distance) {
    if (distance <= 0) {
        cout << "Invalid distance!" << endl;
        return 0;
    }
    else if (distance <= 2) return 10;
    else if (distance <= 5) return 20;
    else if (distance <= 12) return 30;
    else if (distance <= 21) return 40;
    else if (distance <= 32) return 50;
    else return 60;
}

double estimateTravelTime(double distance, int numStations, int numInterchanges=0)
{
    double avgSpeed = 32.0;
    double stationStopTime = 0.5;

    double runningTime = (distance / avgSpeed) * 60.0;
    double stopTime = numStations * stationStopTime;
    double interchangeTime = numInterchanges * 5.0; // average 5 min per interchange

    double totalTime = runningTime + stopTime + interchangeTime;

    return totalTime;
}

void dijkstra(string start_node, string end_node)
{
    unordered_map<string, double> distances;
    unordered_map<string, string> predecessors;
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
    vector<pair<string, string>> path;
    for (const auto& pair : metroMap) {
        distances[pair.first] = numeric_limits<double>::infinity();
    }

    distances[start_node] = 0;
    pq.push({0, start_node});
    while(1)
    {
        double dist = pq.top().first;
        string node = pq.top().second;
        pq.pop();
        if(dist > distances[node]) continue;
        path.push_back({node, ""});
        if(node == end_node)
        {
            break;
        }
        for(auto neighbor : metroMap[node])
        {
            if(dist + neighbor.distance < distances[neighbor.stationName])
            {
                distances[neighbor.stationName] = dist + neighbor.distance;
                pq.push({distances[neighbor.stationName], neighbor.stationName});
                predecessors[neighbor.stationName] = node;
            }
        }
    }
    if(distances[end_node] != numeric_limits<double>::infinity())
    {
        cout << "\nShortest distance from " << start_node << " to " << end_node << " is: " << distances[end_node] << " km" << endl;
        cout << "Path: ";
        vector<string> revPath;
        string current = end_node;
        while(current != start_node)
        {
            revPath.push_back(current);
            current = predecessors[current];
        }
        revPath.push_back(start_node);
        reverse(revPath.begin(), revPath.end());
        for(int i=0; i<revPath.size(); i++)
        {
            cout << revPath[i] << endl;
            if(i != revPath.size() - 1) cout << " -> ";
        }
        cout << endl;

        cout << "Total stations: " << revPath.size() << endl;
        cout << "Total fare: " << calculateFare(distances[end_node]) << " INR" << endl;
        cout << "Estimated travel time: " << estimateTravelTime(distances[end_node], revPath.size() - 1) << " minutes" << endl;
    }
    else
    {
        cout << "No path found from " << start_node << " to " << end_node << endl;
    }
}

void findShortestPathStops(string start, string end)
{
    unordered_map<string, int> visited;
    unordered_map<string, string> parent;
    int stops = 0;
    double distance = 0.0;
    queue<pair<string, double>> q;
    q.push({start, 0.0});
    visited[start] = 1;
    parent[start] = "";
    while(!q.empty())
    {
        stops++;
        string node = q.front().first;
        double dist = q.front().second;
        q.pop();
        if(node == end)
        {
            distance = dist;
            break;
        }

        for(auto neighbor : metroMap[node])
        {
            if(!visited[neighbor.stationName])
            {
                visited[neighbor.stationName] = 1;
                parent[neighbor.stationName] = node;
                q.push({neighbor.stationName, dist + neighbor.distance});
            }
        }
    }

    if(visited[end])
    {
        cout << "Shortest path from " << start << " to " << end << " in terms of stops:" << endl;
        vector<string> path;
        string current = end;
        while(current != "")
        {
            path.push_back(current);
            current = parent[current];
        }
        reverse(path.begin(), path.end());
        for(int i=0; i<path.size(); i++)
        {
            cout << path[i] << endl;
            if(i != path.size() - 1) cout << " -> ";
        }
        cout << endl;

        cout << "Total stations: " << path.size() << endl;
        cout << "Total fare: " << calculateFare(path.size()) << " INR" << endl;
        cout << "Estimated travel time: " << estimateTravelTime(distance, path.size() - 1) << " minutes" << endl;
    }
    else
    {
        cout << "No path found from " << start << " to " << end << endl;
    }
}

void showMenu() {
    cout << "\n===== Delhi Metro Navigator =====" << endl;
    cout << "1. Find shortest path by distance" << endl;
    cout << "2. Find shortest path by number of stations" << endl;
    cout << "3. Exit" << endl;
    cout << "=================================" << endl;
    cout << "Enter your choice: ";
}

int main()
{
    build_graph("stations.csv");

    int choice = 0;
    string startStation, endStation;
    while(choice != 3)
    {
        showMenu();
        cin >> choice;

        cin.ignore();
        if (choice == 1 || choice == 2) {
            cout << "Enter starting station: ";
            getline(cin, startStation);
            cout << "Enter destination station: ";
            getline(cin, endStation);

            if (choice == 1) {
                dijkstra(startStation, endStation);
            } else {
                findShortestPathStops(startStation, endStation);
            }
        } else if (choice == 3) {
            cout << "Exiting... Thank you for using Delhi Metro Navigator!" << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
