#ifndef USER_H
#define USER_H

#include <string>
#include <set>

class User {
public:
    // Enum for user categories
    enum UserCategory {
        UNDERGRADUATE,
        POSTGRADUATE,
        DOCTORATE,
        PROFESSIONAL,
        PROFESSOR
    };

private:
    std::string name;
    std::string id;
    std::string password;
    UserCategory category;
    std::string branch;
    std::set<std::string> connections; // Set to store connections (unique)
    int totalConnections = 0; // Total number of connections
    double degreeCentrality = 0.0;
    double betweennessCentrality = 0.0;
    double closenessCentrality = 0.0;
    double pageRank = 0.0;
    std::string influence; // Influence level as a string

public:
    // Constructor
    User(const std::string& name, const std::string& id, const std::string& password,
         UserCategory category, const std::string& branch)
        : name(name), id(id), password(password), category(category), branch(branch) {}

    // Public getter functions
    std::string getName() const { return name; }
    std::string getId() const { return id; }
    std::string getPassword() const { return password; }
    UserCategory getCategory() const { return category; }
    std::string getBranch() const { return branch; }
    std::set<std::string> getConnections() const { return connections; }
    
    int getTotalConnections() const { return totalConnections; } // Getter for totalConnections
    std::string getInfluence() const { return influence; } // Getter for influence

    // Function to set influence
    void setInfluence(const std::string& level) { influence = level; }

    // Public setter functions for centrality measures
    void setBetweennessCentrality(double value) { betweennessCentrality = value; }
    void setClosenessCentrality(double value) { closenessCentrality = value; }
    void setPageRank(double value) { pageRank = value; }
    void setPassword(const std::string& newPassword) { password = newPassword; }
    
    // Getter functions for centrality measures
    double getBetweennessCentrality() const { return betweennessCentrality; }
    double getClosenessCentrality() const { return closenessCentrality; }
    double getPageRank() const { return pageRank; }

    // Convert UserCategory enum to string
    std::string categoryToString() const {
        switch (category) {
            case UNDERGRADUATE: return "Undergraduate";
            case POSTGRADUATE: return "Postgraduate";
            case DOCTORATE: return "Doctorate";
            case PROFESSIONAL: return "Professional";
            case PROFESSOR: return "Professor";
            default: return "Unknown";
        }
    }

    // Function to add a connection
    void addConnection(const std::string& connectionName) {
        connections.insert(connectionName);
        totalConnections++; // Increment totalConnections when a new connection is added
    }
};

#endif // USER_H
