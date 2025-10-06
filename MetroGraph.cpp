#include "MetroGraph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <queue>
#include <algorithm>
#include <cmath>
#include <tuple>

MetroGraph::MetroGraph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line, currentLineName, prevStationId;
    double prevDistance = 0.0;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == ',') {
            line.pop_back();
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 1) {
            currentLineName = tokens[0];
            prevStationId.clear();
        } else if (tokens.size() >= 2) {
            std::string stationName = tokens[0];
            double currentDistance = std::stod(tokens[1]);
            std::string uniqueId = stationName + "_" + currentLineName;

            stationInfo[uniqueId] = {stationName, currentLineName};

            if (!prevStationId.empty()) {
                double distanceDiff = std::abs(currentDistance - prevDistance);
                adjList[prevStationId].push_back({uniqueId, distanceDiff});
                adjList[uniqueId].push_back({prevStationId, distanceDiff});
            }

            prevStationId = uniqueId;
            prevDistance = currentDistance;

            if (tokens.size() == 3) {
                std::string interchangeLine = tokens[2];
                std::string interchangeId = stationName + "_" + interchangeLine;
                
                adjList[uniqueId].push_back({interchangeId, 0.1});
                adjList[interchangeId].push_back({uniqueId, 0.1});
                
                if (stationInfo.find(interchangeId) == stationInfo.end()) {
                    stationInfo[interchangeId] = {stationName, interchangeLine};
                }
            }
        }
    }
}

bool MetroGraph::stationExists(const std::string& stationName) const {
    for (const auto& pair : stationInfo) {
        if (pair.second.name == stationName) return true;
    }
    return false;
}

std::string MetroGraph::getStationId(const std::string& stationName) const {
    for (const auto& pair : stationInfo) {
        if (pair.second.name == stationName) return pair.first;
    }
    return "";
}

std::vector<std::string> MetroGraph::findShortestPath_Distance(const std::string& startNode, const std::string& endNode) {
    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> predecessors;
    using State = std::pair<double, std::string>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    for (const auto& pair : stationInfo) {
        distances[pair.first] = std::numeric_limits<double>::infinity();
    }

    distances[startNode] = 0;
    pq.push({0.0, startNode});

    while (!pq.empty()) {
        std::string u = pq.top().second;
        pq.pop();

        if (u == endNode) break;

        for (const auto& neighbor : adjList.at(u)) {
            std::string v = neighbor.first;
            double weight = neighbor.second;
            if (distances.at(u) + weight < distances.at(v)) {
                distances[v] = distances.at(u) + weight;
                predecessors[v] = u;
                pq.push({distances.at(v), v});
            }
        }
    }

    std::vector<std::string> path;
    if (predecessors.count(endNode) || startNode == endNode) {
        std::string current = endNode;
        while (!current.empty()) {
            path.push_back(current);
            current = predecessors[current];
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}

std::vector<std::string> MetroGraph::findShortestPath_Stops(const std::string& startNode, const std::string& endNode) {
    std::queue<std::string> q;
    std::unordered_map<std::string, std::string> parent;
    std::unordered_map<std::string, bool> visited;

    q.push(startNode);
    visited[startNode] = true;

    while (!q.empty()) {
        std::string u = q.front();
        q.pop();

        if (u == endNode) break;

        for (const auto& neighbor : adjList.at(u)) {
            std::string v = neighbor.first;
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    
    std::vector<std::string> path;
    if (visited[endNode]) {
        std::string current = endNode;
        while (!current.empty()) {
            path.push_back(current);
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}


std::vector<std::string> MetroGraph::findMinInterchangesPath(const std::string& startNode, const std::string& endNode) {
    // A tuple to store: {interchanges, distance, station_id}
    using State = std::tuple<int, double, std::string>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    
    std::unordered_map<std::string, std::string> predecessors;
    std::unordered_map<std::string, int> interchangeCount;

    for (const auto& pair : stationInfo) {
        interchangeCount[pair.first] = std::numeric_limits<int>::max();
    }

    interchangeCount[startNode] = 0;
    pq.push({0, 0.0, startNode});

    while (!pq.empty()) {
        auto [interchanges, dist, u] = pq.top();
        pq.pop();

        if (interchanges > interchangeCount[u]) {
            continue;
        }
        if (u == endNode) break;
        
        const auto& u_info = stationInfo.at(u);

        for (const auto& neighbor : adjList.at(u)) {
            std::string v = neighbor.first;
            double weight = neighbor.second;
            const auto& v_info = stationInfo.at(v);
            
            // An interchange occurs if the line name changes.
            int newInterchanges = interchanges + (u_info.line != v_info.line);

            if (newInterchanges < interchangeCount[v]) {
                interchangeCount[v] = newInterchanges;
                predecessors[v] = u;
                pq.push({newInterchanges, dist + weight, v});
            }
        }
    }
    
    std::vector<std::string> path;
    if (predecessors.count(endNode) || startNode == endNode) {
        std::string current = endNode;
        while (!current.empty()) {
            path.push_back(current);
            current = predecessors[current];
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}

double MetroGraph::getPathDistance(const std::vector<std::string>& path) const {
    double totalDistance = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        std::string u = path[i];
        std::string v = path[i+1];
        for (const auto& neighbor : adjList.at(u)) {
            if (neighbor.first == v) {
                totalDistance += neighbor.second;
                break;
            }
        }
    }
    return totalDistance;
}

int MetroGraph::countInterchanges(const std::vector<std::string>& path) const {
    if (path.size() < 2) return 0;
    
    int interchanges = 0;
    for(size_t i = 0; i < path.size() - 1; ++i) {
        const auto& line1 = stationInfo.at(path[i]).line;
        const auto& line2 = stationInfo.at(path[i+1]).line;
        if (line1 != line2) {
            interchanges++;
        }
    }
    return interchanges;
}

void MetroGraph::displayPath(const std::vector<std::string>& path) const {
    if (path.empty()) return;

    std::string currentLine = "";
    for (const auto& nodeId : path) {
        const auto& station = stationInfo.at(nodeId);
        if (currentLine != station.line) {
            if (!currentLine.empty()) {
                std::cout << "  --- Change to " << station.line << " Line ---" << std::endl;
            }
            currentLine = station.line;
        }
        std::cout << "    -> " << station.name << " (" << station.line << ")" << std::endl;
    }
}
