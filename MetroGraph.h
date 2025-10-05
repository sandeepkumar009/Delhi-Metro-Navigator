#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Station.h"

class MetroGraph {
private:
    // The graph is stored as an adjacency list.
    // The key is a unique station ID.
    // The value is a list of its neighbors and the distance to them.
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> adjList;

    // A map to get a station's details (name, line) from its unique ID.
    std::unordered_map<std::string, Station> stationInfo;

public:
    MetroGraph(const std::string& filename);
    bool stationExists(const std::string& stationName) const;
    std::string getStationId(const std::string& stationName) const;

    std::vector<std::string> findShortestPath_Distance(const std::string& startNode, const std::string& endNode);
    std::vector<std::string> findShortestPath_Stops(const std::string& startNode, const std::string& endNode);
    std::vector<std::string> findMinInterchangesPath(const std::string& startNode, const std::string& endNode);

    void displayPath(const std::vector<std::string>& path) const;
    double getPathDistance(const std::vector<std::string>& path) const;
    int countInterchanges(const std::vector<std::string>& path) const;
};
