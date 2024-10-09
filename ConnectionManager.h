#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "User.h"
#include <vector>
#include <unordered_map>
#include <random>
#include <queue>
#include <fstream>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

class ConnectionManager {
private:
    vector<User> users;

    // Function declarations
    unordered_map<string, int> bfsShortestPaths(const string& startUser);
    void calculateBetweennessCentrality();
    void calculateClosenessCentrality();
    void calculatePageRank();

public:
    void generateUsers(int userCount);
    void calculateCentralities();
    void generateDotFile(const string& filename);
    void generateTable(const string& filename);
    void addUser(const string& userName, const string& password, User::UserCategory category, const string& branch);
    void connectWithUser(const std::string& userToConnect, const std::string& currentUser);
    vector<User> getTopRankedUsers(int topN = 5);
    const vector<User>& getUsers() const { return users; }
};

#endif // CONNECTIONMANAGER_H

