# Delhi-Metro-Navigator

A console-based C++ application to find optimal routes within the Delhi Metro network. This project models the entire metro system as a weighted graph and uses various pathfinding algorithms to provide the best routes based on distance, number of stations, or number of interchanges.

## Features

- **Multiple Routing Options**: Find the best route based on three different criteria:
  1.  **Shortest Distance**: Calculates the path with the minimum total kilometers using **Dijkstra's Algorithm**.
  2.  **Fewest Stations**: Finds the path with the minimum number of stops using **Breadth-First Search (BFS)**.
  3.  **Minimum Interchanges**: A custom algorithm to find a route that prioritizes fewer line changes.
- **Route Details**: Displays a clear, step-by-step travel plan for the calculated route.
- **Travel Summary**: Provides a summary for each route, including:
  -   Total distance.
  -   Total number of stations.
  -   Number of line changes.
  -   Estimated fare.
  -   Estimated travel time.

## Data Source

The metro network is built from the `stations.csv` file, which contains a sequential list of stations for each metro line, their cumulative distance from the start of the line, and any interchanges. The application is designed to parse this specific format to build the graph in memory.

The station, line, and distance data was collected from [delhimetrorail.info](https://delhimetrorail.info/delhi-metro-stations) and is used for educational and non-commercial purposes only.
## Getting Started

Follow these instructions to compile and run the project on your local machine.

### Prerequisites

You will need a C++ compiler installed on your system. The GNU C++ Compiler (`g++`) is recommended.

### Compilation & Execution

1.  **Clone the repository:**
    ```sh
    git clone https://github.com/sandeepkumar009/Delhi-Metro-Navigator.git
    cd Delhi-Metro-Navigator
    ```

2.  **Compile the source files:**
    Open a terminal in the project directory and run the following command to compile all the `.cpp` files and create an executable named `main`.
    ```sh
    g++ main.cpp MetroGraph.cpp -o main
    ```

3.  **Run the application:**
    Execute the compiled program.
    ```sh
    ./main
    ```

## How to Use

Once the application is running, you will be presented with a menu.

1.  Choose one of the routing options (1, 2, or 3).
2.  Enter the full name of your starting station when prompted.
3.  Enter the full name of your destination station.
4.  The application will display the calculated route and a summary of your journey.

**Example Output:**

    ===== Your Route =====
    -> Rajiv Chowk (Blue)
    -> Barakhamba Road (Blue)
    -> Mandi House (Blue)
    --- Change to Violet Line ---
    -> Mandi House (Violet)
    -> Janpath (Violet)

    --- Summary ---
    Total Distance: 2.80 km
    Total Stations: 4
    Interchanges: 1
    Estimated Fare: 20 INR
    Estimated Time: 12 minutes

## Code Structure

The project is structured using Object-Oriented principles for clarity and maintainability:

-   `main.cpp`: Handles the user interface, menu, and user input/output.
-   `MetroGraph.h`: The header file declaring the `MetroGraph` class, which encapsulates all the graph data and pathfinding logic.
-   `MetroGraph.cpp`: The implementation of the `MetroGraph` class, including the CSV parser and all algorithms.
-   `Station.h`: A simple `struct` that defines the properties of a station.

## Algorithms & Data Structures

-   **Core Data Structure**: The metro network is represented as a **Graph** using an **Adjacency List**.
-   **Algorithms**:
    -   **Dijkstra's Algorithm**: For finding the shortest path by distance.
    -   **Breadth-First Search (BFS)**: For finding the path with the fewest stops.
    -   **Modified Dijkstra's**: A custom search algorithm that prioritizes minimizing interchanges.
