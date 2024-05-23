#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;
int numLikes = 0;
struct User {
    string username;
    string password;
    string profile;
    vector<string> posts;
    vector<string> friends;
    int viewCount; // New member variable to track profile views
};

class SocialNetwork {
private:
    map<string, User> users;
    map<string, vector<string>> graph; // Graph representation: user -> list of friends
    string lastLoggedInUser; // Remember the last logged-in user
public:
    void signup();
    void login();
    void viewProfile(const string& username);
    void viewOtherProfile(const string& loggedInUsername); // New function to view other people's profiles
    void addPost(const string& username);
    void likePost(const string& username);
    void unlikePost(const string& username);
    void addFriend(const string& username);
    void removeFriend(const string& username);
    void suggestFriend(const string& username);
    void degreeOfSeparation(const string& username, const string& targetUsername);
    bool areConnected(const string& src, const string& dest, map<string, bool>& visited);
    string getLastLoggedInUser(); // Return the last logged-in user
    
    // Add method to add an edge between two users
    void addEdge(const string& user1, const string& user2);
    
    // Method to print the graph in matrix format
    void printGraphMatrix();
    
    // Method to print the graph in list format
    void printGraphList();
};

void SocialNetwork::signup() {
    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;
    cout << "Enter password: ";
    cin >> newUser.password;
    cout << "Enter profile: ";
    cin.ignore();
    getline(cin, newUser.profile);
    newUser.viewCount = 0; // Initialize view count to 0

    users[newUser.username] = newUser;
    graph[newUser.username] = vector<string>();
    cout << "Signup successful!\n";
}

void SocialNetwork::login() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end() && users[username].password == password) {
        cout << "Login successful!\n";
        lastLoggedInUser = username; // Update the last logged-in user
    } else {
        cout << "Invalid username or password!\n";
    }
}

void SocialNetwork::viewProfile(const string& username) {
	
    if (users.find(username) != users.end()) {
        User& user = users[username];
        cout << "Username: " << user.username << endl;
        cout << "Profile: " << user.profile << endl;
        cout << "Posts:" << endl;
        for (size_t i = 0; i < user.posts.size(); ++i) {
            
            cout << i + 1 << ". " << user.posts[i]; // Adjust post index to start from 1
            
            cout << " (Likes: " << numLikes << ")" << endl;
        }
        cout << "Friends:" << endl;
        for (const string& friendName : user.friends) {
            cout << friendName << endl;
        }
        cout << "View Count: " << user.viewCount << endl; // Display view count
        user.viewCount++; // Increment view count
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::viewOtherProfile(const string& loggedInUsername) {
    string username;
    cout << "Enter the username of the profile you want to view: ";
    cin >> username;

    if (users.find(username) != users.end()) {
        viewProfile(username); // Call viewProfile function to display the profile
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::addPost(const string& username) {
    if (users.find(username) != users.end()) {
        User& user = users[username];
        string post;
        cout << "Enter post: ";
        cin.ignore();
        getline(cin, post);
        user.posts.push_back(post);
        cout << "Post added successfully!\n";
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::likePost(const string& username) {
	
    string postOwner;
    cout << "Enter the username of the post owner: ";
    cin >> postOwner;

    if (users.find(postOwner) != users.end()) {
        User& user = users[postOwner];
        
        // Display all the posts of the post owner along with indices
        cout << "Posts of user " << postOwner << ":" << endl;
        for (size_t i = 0; i < user.posts.size(); ++i) {
            cout << i+1 << ". " << user.posts[i] << endl;
        }
        
        int postIndex;
        cout << "Enter the index of the post you want to like: ";
        cin >> postIndex;

        if (postIndex >= 0 && postIndex < user.posts.size()+1) {
            // Check if the post is already liked by the user
            size_t likedByIndex = user.posts[postIndex-1].find("Liked by: " + username);
            if (likedByIndex == string::npos) {
				numLikes++;
                // Post not liked by the user, add the like
                user.posts[postIndex-1] += " - Liked by: " + username;
                cout << "Post liked successfully!\n";
            } else {
                cout << "You have already liked this post!\n";
            }
        } else {
            cout << "Invalid post index!\n";
        }
    } else {
        cout << "Post owner not found!\n";
    }
	
}


void SocialNetwork::unlikePost(const string& username) {
    string postOwner;
    cout << "Enter the username of the post owner: ";
    cin >> postOwner;

    if (users.find(postOwner) != users.end()) {
        User& user = users[postOwner];
        // Display all the posts of the post owner along with indices
        cout << "Posts of user " << postOwner << ":" << endl;
        for (size_t i = 0; i < user.posts.size(); ++i) {
            cout << i << ". " << user.posts[i] << endl;
        }
        int postIndex;
        cout << "Enter the index of the post you want to unlike: ";
        cin >> postIndex;

        if (postIndex >= 0 && postIndex < user.posts.size()) {
            // Check if the post is already liked by the user
            size_t likedByIndex = user.posts[postIndex].find("Liked by: " + username);
            if (likedByIndex != string::npos) {
                // Erase the liked by information
                user.posts[postIndex].erase(likedByIndex);
                // Decrement numLikes
                numLikes--;
                cout << "Post unliked successfully!\n";
            } else {
                cout << "You haven't liked this post!\n";
            }
        } else {
            cout << "Invalid post index!\n";
        }
    } else {
        cout << "Post owner not found!\n";
    }
}


void SocialNetwork::addFriend(const string& username) {
    if (users.find(username) != users.end()) {
        string friendUsername;
        cout << "Enter friend's username: ";
        cin >> friendUsername;

        if (users.find(friendUsername) != users.end()) {
            User& user = users[username];
            User& friendUser = users[friendUsername];

            user.friends.push_back(friendUsername);
            friendUser.friends.push_back(username);

            graph[username].push_back(friendUsername);
            graph[friendUsername].push_back(username);

            cout << "Friend added successfully!\n";
        } else {
            cout << "Friend not found!\n";
        }
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::removeFriend(const string& username) {
    if (users.find(username) != users.end()) {
        string friendUsername;
        cout << "Enter friend's username to remove: ";
        cin >> friendUsername;

        User& user = users[username];
        auto it = find(user.friends.begin(), user.friends.end(), friendUsername);
        if (it != user.friends.end()) {
            user.friends.erase(it);

            // Remove friend from the other user's friend list
            User& friendUser = users[friendUsername];
            it = find(friendUser.friends.begin(), friendUser.friends.end(), username);
            if (it != friendUser.friends.end()) {
                friendUser.friends.erase(it);
            }

            // Remove friend from the graph
            auto it_graph = find(graph[username].begin(), graph[username].end(), friendUsername);
            if (it_graph != graph[username].end()) {
                graph[username].erase(it_graph);
            }

            it_graph = find(graph[friendUsername].begin(), graph[friendUsername].end(), username);
            if (it_graph != graph[friendUsername].end()) {
                graph[friendUsername].erase(it_graph);
            }

            cout << "Friend removed successfully!\n";
        } else {
            cout << "Friend not found!\n";
        }
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::suggestFriend(const string& username) {
    if (users.find(username) != users.end()) {
        User& user = users[username];
        map<string, int> suggestedFriends;

        // Count friends of friends
        for (const string& friendName : user.friends) {
            for (const string& friendOfFriend : users[friendName].friends) {
                if (friendOfFriend != username && find(user.friends.begin(), user.friends.end(), friendOfFriend) == user.friends.end()) {
                    // Increment count for friend of friend if not already a friend and not the user itself
                    suggestedFriends[friendOfFriend]++;
                }
            }
        }

        // Find friend with maximum mutual friends
        string suggestedFriend;
        int maxMutualFriends = 0;
        for (const auto& pair : suggestedFriends) {
            if (pair.second > maxMutualFriends) {
                suggestedFriend = pair.first;
                maxMutualFriends = pair.second;
            }
        }

        if (!suggestedFriend.empty()) {
            cout << "Suggested friend for " << username << ": " << suggestedFriend << endl;
        } else {
            cout << "No suggested friends for " << username << " at the moment.\n";
        }
    } else {
        cout << "User not found!\n";
    }
}

void SocialNetwork::degreeOfSeparation(const string& loggedInUsername, const string& targetUsername) {
    if (users.find(targetUsername) != users.end()) {
        map<string, bool> visited;
        int degree = 0;

        queue<string> q;
        q.push(targetUsername);
        visited[targetUsername] = true;

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string current = q.front();
                q.pop();

                for (const string& friendName : graph[current]) {
                    if (!visited[friendName]) {
                        visited[friendName] = true;
                        q.push(friendName);
                    }
                }
            }
            degree++;
        }

        cout << "Degree of separation between " << loggedInUsername << " and " << targetUsername << " is " << degree - 1 << endl;
    } else {
        cout << "Target user not found!\n";
    }
}

string SocialNetwork::getLastLoggedInUser() {
    return lastLoggedInUser; // Return the last logged-in user
}

// Method to add an edge between two users
void SocialNetwork::addEdge(const string& user1, const string& user2) {
    if (users.find(user1) != users.end() && users.find(user2) != users.end()) {
        graph[user1].push_back(user2);
        graph[user2].push_back(user1);
        cout << "Edge added successfully between " << user1 << " and " << user2 << endl;
    } else {
        cout << "One or both users not found!\n";
    }
}

// Method to print the graph in matrix format
void SocialNetwork::printGraphMatrix() {
    cout << "Adjacency Matrix:" << endl;
    for (const auto& entry : graph) {
        const string& user = entry.first;
        const vector<string>& friends = entry.second;

        cout << user << ": ";
        for (const auto& user2 : users) {
            if (find(friends.begin(), friends.end(), user2.first) != friends.end()) {
                cout << "1 ";
            } else {
                cout << "0 ";
            }
        }
        cout << endl;
    }
}

// Method to print the graph in list format
void SocialNetwork::printGraphList() {
    cout << "Adjacency List:" << endl;
    for (const auto& entry : graph) {
        const string& user = entry.first;
        const vector<string>& friends = entry.second;

        cout << user << ": ";
        for (const auto& friendName : friends) {
            cout << friendName << " ";
        }
        cout << endl;
    }
}

int main() {
    SocialNetwork network;
    int choice;

    do {
        cout << "1. Signup\n2. Login\n3. View Profile\n4. Add Post\n5. Like Post\n6. Unlike Post\n7. Add Friend\n8. Remove Friend\n9. Suggest Friend\n10. Degree of Separation\n11. Add Edge\n12. Print Graph (Matrix)\n13. Print Graph (List)\n14. View Other Profile\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                network.signup();
                break;
            case 2:
                network.login();
                break;
            case 3:
                network.viewProfile(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 4:
                network.addPost(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 5:
                network.likePost(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 6:
                network.unlikePost(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 7:
                network.addFriend(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 8:
                network.removeFriend(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 9:
                network.suggestFriend(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 10: {
                string targetUsername;
                cout << "Enter the username for degree of separation calculation: ";
                cin >> targetUsername;
                network.degreeOfSeparation(network.getLastLoggedInUser(), targetUsername); // Use the last logged-in user
                break;
            }
            case 11: {
                string user1, user2;
                cout << "Enter usernames to add an edge: ";
                cin >> user1 >> user2;
                network.addEdge(user1, user2);
                break;
            }
            case 12:
                network.printGraphMatrix();
                break;
            case 13:
                network.printGraphList();
                break;
            case 14:
                network.viewOtherProfile(network.getLastLoggedInUser()); // Use the last logged-in user
                break;
            case 0:
                cout << "Exiting...";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
