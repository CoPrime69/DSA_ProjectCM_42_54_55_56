#include <iostream>
#include "ConnectionManager.h"

int main() {
    ConnectionManager connectionManager;

    // Generate synthetic users
    connectionManager.generateUsers(20); // You can specify the number of users to generate

    // Connect some users
    connectionManager.connectWithUser("Alice", "Bob");
    connectionManager.connectWithUser("Alice", "Charlie");
    connectionManager.connectWithUser("Bob", "David");
    
    // Calculate centrality measures
    connectionManager.calculateCentralities();

    // Get top ranked users based on PageRank
    vector<User> topUsers = connectionManager.getTopRankedUsers(5);
    std::cout << "Top Ranked Users:\n";
    for (const auto& user : topUsers) {
        std::cout << "Name: " << user.getName() << ", PageRank: " << user.getPageRank() << "\n";
    }

    // Generate a DOT file for visualization
    connectionManager.generateDotFile("users_graph.dot");

    // Generate a CSV file with user details
    connectionManager.generateTable("users_data.csv");

    std::cout << "User data and connections generated.\n";

    return 0;
}
