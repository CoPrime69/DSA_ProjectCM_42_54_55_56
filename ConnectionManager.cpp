#include "ConnectionManager.h"
#include <iostream> // Include iostream for cout
#include <fstream>
#include <algorithm> // For sorting
#include <queue>     // For BFS
#include <random>    // For random user generation
#include "User.h" 
#include <string>     // For string operations
#include <vector>     // For using dynamic arrays
#include <map>        // For key-value pairs
#include <set>        // For ordered collections

using namespace std;

// Function to convert UserCategory to string
std::string categoryToString(User::UserCategory category) {
    switch (category) {
        case User::PROFESSOR: return "Professor";
        case User::UNDERGRADUATE: return "UnderGraduate";
        case User::POSTGRADUATE: return "Postgraduate";
        case User::DOCTORATE: return "Doctorate";
        case User::PROFESSIONAL: return "Professional";
        default: return "Unknown";
    }
}

// Function to perform BFS and calculate shortest paths
unordered_map<string, int> ConnectionManager::bfsShortestPaths(const string& startUser) {
    unordered_map<string, int> distances;
    queue<string> q;
    set<string> visited;

    // Initialize distances
    for (const auto& user : users) {
        distances[user.getName()] = -1; // -1 indicates unreachable
    }

    distances[startUser] = 0; // Start user has distance 0
    q.push(startUser);
    visited.insert(startUser);

    while (!q.empty()) {
        string currentUser = q.front();
        q.pop();

        // Find the User object
        auto it = find_if(users.begin(), users.end(), [&currentUser](const User& user) {
            return user.getName() == currentUser;
        });

        if (it != users.end()) {
            // Visit all connections of the current user
            for (const auto& conn : it->getConnections()) {
                if (visited.find(conn) == visited.end()) {
                    visited.insert(conn);
                    distances[conn] = distances[currentUser] + 1;
                    q.push(conn);
                }
            }
        }
    }

    return distances;
}

// Function to calculate Betweenness Centrality
void ConnectionManager::calculateBetweennessCentrality() {
    for (auto& user : users) {
        user.setBetweennessCentrality(0.0); // Use setter

        // For each user, find shortest paths from them to all others
        for (const auto& otherUser : users) {
            if (user.getName() != otherUser.getName()) {
                auto distances = bfsShortestPaths(user.getName());

                // Count the number of shortest paths that pass through the user
                for (const auto& targetUser : users) {
                    if (targetUser.getName() != user.getName() && targetUser.getName() != otherUser.getName()) {
                        if (distances[targetUser.getName()] != -1) {
                            user.setBetweennessCentrality(user.getBetweennessCentrality() + 1.0 / distances[targetUser.getName()]);
                        }
                    }
                }
            }
        }
    }
}

// Update other methods similarly...
void ConnectionManager::calculateClosenessCentrality() {
    for (auto& user : users) {
        auto distances = bfsShortestPaths(user.getName());
        double totalDistance = 0.0;
        int reachableCount = 0;

        // Sum the distances to all other users
        for (const auto& distance : distances) {
            if (distance.second != -1 && distance.first != user.getName()) { // Ignore unreachable and self
                totalDistance += distance.second;
                reachableCount++;
            }
        }

        if (reachableCount > 0) {
            user.setClosenessCentrality(static_cast<double>(reachableCount) / totalDistance); // Use setter
        } else {
            user.setClosenessCentrality(0); // If no users are reachable
        }
    }
}

void ConnectionManager::calculatePageRank() {
    const double d = 0.85; // Damping factor
    const int maxIterations = 100;
    const double tolerance = 1e-6; // Convergence tolerance

    // Initialize PageRank for each user
    for (auto& user : users) {
        user.setPageRank(1.0 / users.size()); // Use setter
    }

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        vector<double> newPageRank(users.size(), 0.0);

        // Calculate PageRank for each user
        for (const auto& user : users) {
            double weightedSum = 0.0;
            for (const auto& conn : user.getConnections()) {
                // Find the connection user
                auto it = find_if(users.begin(), users.end(), [&conn](const User& u) { return u.getName() == conn; });
                if (it != users.end()) {
                    int totalConn = it->getConnections().size();
                    // Add weighted contribution based on connections
                    double weight = (user.getBranch() == it->getBranch() && user.getCategory() == it->getCategory()) ? 1.5 : 1.0;
                    weightedSum += (it->getPageRank() / totalConn) * weight; // Adjust weight for same branch and category
                }
            }
            newPageRank[&user - &users[0]] += (1 - d) / users.size() + d * weightedSum; // Adjust with damping factor
        }

        // Check for convergence
        double maxChange = 0.0;
        for (size_t i = 0; i < users.size(); ++i) {
            maxChange = max(maxChange, abs(newPageRank[i] - users[i].getPageRank())); // Use getter
            users[i].setPageRank(newPageRank[i]); // Use setter
        }

        if (maxChange < tolerance) {
            break; // Stop if converged
        }
    }
}


// Function to get the top N ranked users based on PageRank
vector<User> ConnectionManager::getTopRankedUsers(int topN) {
    // Sort users by PageRank score in descending order
    sort(users.begin(), users.end(), [](const User& a, const User& b) {
        return a.getPageRank() > b.getPageRank(); // Correctly use getter
    });

    // Get the top N users
    vector<User> topUsers;
    for (int i = 0; i < topN && i < users.size(); ++i) {
        topUsers.push_back(users[i]);
    }

    return topUsers;
}

// Function to calculate centrality measures
void ConnectionManager::calculateCentralities() {
    for (auto& user : users) {
        user.setBetweennessCentrality(0.0); // Use setter
        user.setClosenessCentrality(0.0);    // Use setter
        user.setPageRank(0.0);                // Reset before calculation
    }

    calculateBetweennessCentrality();
    calculateClosenessCentrality();
    calculatePageRank();
}

// Function to generate users
void ConnectionManager::generateUsers(int userCount) {
    random_device rd;  
    mt19937 eng(rd());

    // Distributions for generating random connections
    uniform_int_distribution<> connectionsDist(5, 50); // Random total connections, professors and professionals will have more
    uniform_int_distribution<> influenceDist(1, 3); // Influence levels: 1 (low), 2 (moderate), 3 (high)

    // First names and surnames for generating unique names
    vector<string> firstNames = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Heidi", "Ivan", "Judy"};
    vector<string> lastNames = {"Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor"};

    // User category distributions
    int numProfessors = userCount / 10;      // 10% professors
    int numProfessionals = userCount / 10;   // 10% professionals
    int numDoctorates = userCount / 10;       // 10% doctorates
    int numPostgraduates = userCount * 3 / 10; // 30% postgraduates
    int numUndergraduates = userCount * 4 / 10; // 40% undergraduates

    // Generate synthetic users
    for (int i = 0; i < numProfessors; ++i) {
        string name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
        string id = "P" + to_string(rand() % 10000); // Unique ID with a P prefix
        string password = to_string(rand() % 10000); // Unique 4-digit password
        users.emplace_back(name, id, password, User::PROFESSOR, "N/A");
        users.back().setInfluence(influenceDist(eng) == 3 ? "High" : "Moderate"); // Set influence
    }

    for (int i = 0; i < numProfessionals; ++i) {
        string name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
        string id = "R" + to_string(rand() % 10000); // Unique ID with an R prefix
        string password = to_string(rand() % 10000); // Unique 4-digit password
        users.emplace_back(name, id, password, User::PROFESSIONAL, "N/A");
        users.back().setInfluence(influenceDist(eng) == 3 ? "High" : "Moderate"); // Set influence
    }

    for (int i = 0; i < numDoctorates; ++i) {
        string name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
        string id = "D" + to_string(rand() % 10000); // Unique ID with a D prefix
        string password = to_string(rand() % 10000); // Unique 4-digit password
        users.emplace_back(name, id, password, User::DOCTORATE, "N/A");
        users.back().setInfluence("Moderate"); // Set influence
    }

    for (int i = 0; i < numPostgraduates; ++i) {
        string name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
        string id = "M" + to_string(rand() % 10000); // Unique ID with an M prefix
        string password = to_string(rand() % 10000); // Unique 4-digit password
        users.emplace_back(name, id, password, User::POSTGRADUATE, "N/A");
        users.back().setInfluence("Low"); // Set influence
    }

    for (int i = 0; i < numUndergraduates; ++i) {
        string name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
        string id = "U" + to_string(rand() % 10000); // Unique ID with a U prefix
        string password = to_string(rand() % 10000); // Unique 4-digit password
        users.emplace_back(name, id, password, User::UNDERGRADUATE, "N/A");
        users.back().setInfluence("Low"); // Set influence
    }

    // Randomly create connections with bias towards professors and professionals
    // Randomly create connections with bias towards professors and professionals
    for (auto& user : users) {
        int totalConnections = connectionsDist(eng);
        for (int j = 0; j < totalConnections; ++j) {
            // Ensure connections are more probable for professors and professionals
            int connIndex;
            if (user.getCategory() == User::PROFESSOR || user.getCategory() == User::PROFESSIONAL) {
                connIndex = rand() % users.size(); // Avoid self-connection
            } else {
                connIndex = rand() % (users.size() / 2); // More likely to connect with professors or professionals
            }
            
            // Ensure that connIndex does not point to the same user
            if (connIndex != (&user - &users[0])) { // Compare with the index of user in the vector
                user.addConnection(users[connIndex].getName());
            }
        }
    }
}




// Function to add a new user
void ConnectionManager::addUser(const string& userName, const string& password, User::UserCategory category, const string& branch) {
    // You might want to check if user already exists before adding
    users.emplace_back(userName, to_string(users.size()), password, category, branch);
}

// Function to connect two users
void ConnectionManager::connectWithUser(const string& userToConnect, const string& currentUser) {
    auto it1 = find_if(users.begin(), users.end(), [&userToConnect](const User& user) { return user.getName() == userToConnect; });
    auto it2 = find_if(users.begin(), users.end(), [&currentUser](const User& user) { return user.getName() == currentUser; });

    if (it1 != users.end() && it2 != users.end()) {
        it1->addConnection(currentUser); // Use the addConnection method
        it2->addConnection(userToConnect); // Use the addConnection method
    }
}

// Function to generate a DOT file for visualization
void ConnectionManager::generateDotFile(const string& filename) {
    ofstream outFile(filename);
    outFile << "graph G {\n";

    for (const auto& user : users) {
        for (const auto& conn : user.getConnections()) {
            outFile << "    \"" << user.getName() << "\" -- \"" << conn << "\";\n"; // Connections between users
        }
    }

    outFile << "}\n";
    outFile.close();
}

// Function to generate a CSV file
void ConnectionManager::generateTable(const std::string& filename) {
    std::ofstream file(filename);
    file << "Name,ID,Password,Category,Influence,Branch,TotalConnections,PageRank\n";

    for (const auto& user : users) {
        file << user.getName() << ", "
             << user.getId() << ", "
             << user.getPassword() << ", "
             << user.categoryToString() << ", " // Get category as string
             << user.getInfluence() << ", " // Include influence level
             << user.getBranch() << ", "
             << user.getTotalConnections() << ", "
             << user.getPageRank() << "\n"; // Ensure PageRank is accessible
    }

    file.close();
}



