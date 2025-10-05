#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "MetroGraph.h"

int calculateFare(double distance) {
    if (distance <= 2) return 10;
    if (distance <= 5) return 20;
    if (distance <= 12) return 30;
    if (distance <= 21) return 40;
    if (distance <= 32) return 50;
    return 60;
}

double estimateTravelTime(double distance, int numStations, int numInterchanges) {
    const double AVG_SPEED_KMPH = 33.0;
    const double STATION_DWELL_TIME_MIN = 0.5;
    const double INTERCHANGE_PENALTY_MIN = 5.0;

    double runningTime = (distance / AVG_SPEED_KMPH) * 60.0;
    double stopTime = numStations * STATION_DWELL_TIME_MIN;
    double interchangeTime = numInterchanges * INTERCHANGE_PENALTY_MIN;

    return runningTime + stopTime + interchangeTime;
}

void showMenu() {
    std::cout << "\n===== Delhi Metro Pathfinder =====" << std::endl;
    std::cout << "1. Find shortest route (by distance)" << std::endl;
    std::cout << "2. Find shortest route (by stations)" << std::endl;
    std::cout << "3. Find route with minimum interchanges" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    MetroGraph metro("stations.csv");
    int choice = 0;

    while (choice != 4) {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();

        if (choice >= 1 && choice <= 3) {
            std::cout << "Enter starting station: ";
            std::string startStationName;
            std::getline(std::cin, startStationName);

            std::cout << "Enter destination station: ";
            std::string endStationName;
            std::getline(std::cin, endStationName);

            if (!metro.stationExists(startStationName) || !metro.stationExists(endStationName)) {
                std::cout << "\nError: Invalid station name entered. Please check and try again." << std::endl;
                continue;
            }

            std::string startNode = metro.getStationId(startStationName);
            std::string endNode = metro.getStationId(endStationName);

            std::vector<std::string> path;
            if (choice == 1) {
                path = metro.findShortestPath_Distance(startNode, endNode);
            } else if (choice == 2) {
                path = metro.findShortestPath_Stops(startNode, endNode);
            } else {
                path = metro.findMinInterchangesPath(startNode, endNode);
            }
            
            if (path.size() <= 1 && startStationName != endStationName) {
                std::cout << "\nNo path found between " << startStationName << " and " << endStationName << "." << std::endl;
            } else {
                std::cout << "\n===== Your Route =====" << std::endl;
                metro.displayPath(path);
                
                double distance = metro.getPathDistance(path);
                int interchanges = metro.countInterchanges(path);

                std::cout << "\n--- Summary ---" << std::endl;
                std::cout << "Total Distance: " << std::fixed << std::setprecision(2) << distance << " km" << std::endl;
                std::cout << "Total Stations: " << path.size() << std::endl;
                std::cout << "Interchanges: " << interchanges << std::endl;
                std::cout << "Estimated Fare: " << calculateFare(distance) << " INR" << std::endl;
                std::cout << "Estimated Time: " << static_cast<int>(estimateTravelTime(distance, path.size(), interchanges)) << " minutes" << std::endl;
            }
        } else if (choice == 4) {
            std::cout << "Exiting... Thank you!" << std::endl;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
