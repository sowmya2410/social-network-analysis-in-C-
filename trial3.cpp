#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;

// Class to represent a post
class Post {
private:
    string content;
    int views;
    int likes;
public:
    Post(const string& content) : content(content), views(0), likes(0) {}

    // Getter methods
    string getContent() const { return content; }
    int getViews() const { return views; }
    int getLikes() const { return likes; }

    // Method to increment views
    void incrementViews() { views++; }

    // Method to like the post
    void like() { likes++; }

    // Method to unlike the post
    void unlike() { if (likes > 0) likes--; }

    // Method to display the post
    void display() const {
        cout << "Content: " << content << endl;
        cout << "Views: " << views << endl;
        cout << "Likes: " << likes << endl;
    }
};

// Class to represent a user
class User {
private:
    string userId;
    string password;
    string name;
    string bio;
    vector<Post> posts;
     unordered_set<string> friends;
public:
    User(const string& userId, const string& password, const string& name, const string& bio)
        : userId(userId), password(password), name(name), bio(bio) {}

    // Getter methods
    string getUserId() const { return userId; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    string getBio() const { return bio; }

    // Method to check if provided credentials match
    bool checkCredentials(const string& userId, const string& password) const {
        return (this->userId == userId && this->password == password);
    }

    // Method to save user credentials to file
    void saveToFile(ofstream& outFile) const {
        outFile << userId << " " << password << " " << name << " " << bio << endl;
    }

    // Methods to manage posts
    void addPost(const string& content) {
        posts.emplace_back(content);
        cout << "Post added successfully!" << endl;
    }

    void removePost(int index) {
        if (index >= 0 && index < posts.size()) {
            posts.erase(posts.begin() + index);
            cout << "Post removed successfully!" << endl;
        } else {
            cout << "Invalid post index." << endl;
        }
    }

    void viewPost(int index) {
        if (index >= 0 && index < posts.size()) {
            posts[index].incrementViews();
            posts[index].display();
        } else {
            cout << "Invalid post index." << endl;
        }
    }

    void likePost(int index) {
        if (index >= 0 && index < posts.size()) {
            posts[index].like();
            cout << "Post liked successfully!" << endl;
        } else {
            cout << "Invalid post index." << endl;
        }
    }

    void unlikePost(int index) {
        if (index >= 0 && index < posts.size()) {
            posts[index].unlike();
            cout << "Post unliked successfully!" << endl;
        } else {
            cout << "Invalid post index." << endl;
        }
    }

    void displayPosts() const {
        for (int i = 0; i < posts.size(); ++i) {
            cout << "Post #" << i + 1 << ":" << endl;
            posts[i].display();
            cout << endl;
        }
    }
     // Methods to manage friends
    void addFriend(const string& friendId) {
        friends.insert(friendId);
        cout << friendId << " added as a friend." << endl;
    }

    void removeFriend(const string& friendId) {
        friends.erase(friendId);
        cout << friendId << " removed from friends." << endl;
    }

    const unordered_set<string>& getFriends() const {
        return friends;
    }
};

// Class to manage users
class UserManager {
private:
    vector<User> users;
    User* loggedInUser;
    unordered_map<string, vector<string>> friendshipGraph;
public:
    UserManager() : loggedInUser(nullptr) {}

    // Method to load users from file
    void loadUsersFromFile(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            cerr << "Error: Cannot open file " << fileName << endl;
            return;
        }

        string userId, password, name, bio;
        while (inFile >> userId >> password >> name >> bio) {
            users.push_back(User(userId, password, name, bio));
        }
        inFile.close();
    }

    // Method to create a new user
    void createUser(const string& userId, const string& password, const string& name, const string& bio, const string& fileName) {
        users.push_back(User(userId, password, name, bio));
        saveToFile(fileName);
        cout << "User created successfully!" << endl;
    }

    // Method to save users to file
    void saveToFile(const string& fileName) {
        ofstream outFile(fileName);
        if (!outFile) {
            cerr << "Error: Cannot open file " << fileName << " for writing" << endl;
            return;
        }

        for (const auto& user : users) {
            user.saveToFile(outFile);
        }
        outFile.close();
    }

    // Method to authenticate a user
    bool authenticateUser(const string& userId, const string& password) {
        for (auto& user : users) {
            if (user.checkCredentials(userId, password)) {
                loggedInUser = &user;
                cout << "Login successful!" << endl;
                return true;
            }
        }
        cout << "Invalid credentials. Login failed." << endl;
        return false;
    }

    // Method to display a user's profile
    void displayProfile(const string& userId) {
        for (const auto& user : users) {
            if (user.getUserId() == userId) {
                cout << "User ID: " << user.getUserId() << endl;
                cout << "Name: " << user.getName() << endl;
                cout << "Bio: " << user.getBio() << endl;
                return;
            }
        }
        cout << "User not found." << endl;
    }

    // Methods to manage posts for the logged-in user
    void addPost(const string& content) {
        if (loggedInUser) {
            loggedInUser->addPost(content);
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void removePost(int index) {
        if (loggedInUser) {
            loggedInUser->removePost(index);
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void viewPost(int index) {
        if (loggedInUser) {
            loggedInUser->viewPost(index);
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void likePost(int index) {
        if (loggedInUser) {
            loggedInUser->likePost(index);
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void unlikePost(int index) {
        if (loggedInUser) {
            loggedInUser->unlikePost(index);
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void displayPosts() const {
        if (loggedInUser) {
            loggedInUser->displayPosts();
        } else {
            cout << "No user is logged in." << endl;
        }
    }
    // Methods to manage friendships
    void addFriend(const string& friendId) {
        if (loggedInUser) {
            loggedInUser->addFriend(friendId);
            friendshipGraph[loggedInUser->getUserId()].push_back(friendId);
            cout << "Friend added successfully!" << endl;
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    void removeFriend(const string& friendId) {
        if (loggedInUser) {
            loggedInUser->removeFriend(friendId);
            friendshipGraph[loggedInUser->getUserId()].erase(
                remove(friendshipGraph[loggedInUser->getUserId()].begin(), friendshipGraph[loggedInUser->getUserId()].end(), friendId),
                friendshipGraph[loggedInUser->getUserId()].end()
            );
            cout << "Friend removed successfully!" << endl;
        } else {
            cout << "No user is logged in." << endl;
        }
    }

    // Method to display followers of a user
    void displayFollowers() const {
        if(loggedInUser){
        cout << "Followers of " << loggedInUser->getUserId() << ":" << endl;
        if (friendshipGraph.find(loggedInUser->getUserId()) != friendshipGraph.end()) {
            for (const auto& pair : friendshipGraph) {
                if (pair.second.end() != find(pair.second.begin(), pair.second.end(), loggedInUser->getUserId())) {
                    cout << pair.first << endl;
                }
            }
        }} else {
            cout << "No followers found." << endl;
        }
    }

    // Method to display followings of a user
    void displayFollowings() const {
        if(loggedInUser){
        cout << "Followings of " << loggedInUser->getUserId() << ":" << endl;
        if (friendshipGraph.find(loggedInUser->getUserId()) != friendshipGraph.end()) {
            for (const auto& friendId : friendshipGraph.at(loggedInUser->getUserId())) {
                cout << friendId << endl;
            }
        }} else {
            cout << "No followings found." << endl;
        }
    }
};

int main() {
    UserManager userManager;

    // Load existing users from file
    userManager.loadUsersFromFile("users.txt");

    string username, password, name, bio, content, friendId;
    int n, postIndex;

    do {
        cout << "1. Signup" << endl;
        cout << "2. Login" << endl;
        cout << "3. Display Profile" << endl;
        cout << "4. Add Post" << endl;
        cout << "5. Remove Post" << endl;
        cout << "6. View Post" << endl;
        cout << "7. Like Post" << endl;
        cout << "8. Unlike Post" << endl;
        cout << "9. Display All Posts" << endl;
        cout << "10. Add Friend" << endl;
        cout << "11. Remove Friend" << endl;
        cout << "12. Display Followers" << endl;
        cout << "13. Display Followings" << endl;
        cout << "14. Exit" << endl;
        cout << "Enter your choice: " << endl;
        cin >> n;
        switch (n) {
        case 1:
            cout << "Enter the username: " << endl;
            cin >> username;
            cout << "Enter the password: " << endl;
            cin >> password;
            cout << "Enter your name: " << endl;
            cin >> name;
            cout << "Enter your bio: " << endl;
            cin.ignore(); // to clear the newline character left by cin
            getline(cin, bio);
            userManager.createUser(username, password, name, bio, "users.txt");
            break;
        case 2:
            cout << "Enter the username: " << endl;
            cin >> username;
            cout << "Enter the password: " << endl;
            cin >> password;
            userManager.authenticateUser(username, password);
            break;
        case 3:
            cout << "Enter the username to display profile: " << endl;
            cin >> username;
            userManager.displayProfile(username);
            break;
        case 4:
            cout << "Enter the post content: " << endl;
            cin.ignore(); // to clear the newline character left by cin
            getline(cin, content);
            userManager.addPost(content);
            break;
        case 5:
            userManager.displayPosts();
            cout << "Enter the post index to remove: " << endl;
            cin >> postIndex;
            userManager.removePost(postIndex - 1); // Adjust for 0-based index
            break;
        case 6:
            userManager.displayPosts();
            cout << "Enter the post index to view: " << endl;
            cin >> postIndex;
            userManager.viewPost(postIndex - 1); // Adjust for 0-based index
            break;
        case 7:
            userManager.displayPosts();
            cout << "Enter the post index to like: " << endl;
            cin >> postIndex;
            userManager.likePost(postIndex - 1); // Adjust for 0-based index
            break;
        case 8:
             userManager.displayPosts();
            cout << "Enter the post index to unlike: " << endl;
            cin >> postIndex;
            userManager.unlikePost(postIndex - 1); // Adjust for 0-based index
            break;
        case 9:
            userManager.displayPosts();
            break;
        case 10:
            cout << "Enter the friend's username: " << endl;
            cin >> friendId;
            userManager.addFriend(friendId);
            break;
        case 11:
            cout << "Enter the friend's username to remove: " << endl;
            cin >> friendId;
            userManager.removeFriend(friendId);
            break;
        case 12:
         
            userManager.displayFollowers();
            break;
        case 13:
             userManager.displayFollowings();
            break;
        case 14:
            cout << "Exiting" << endl;
            exit(0);
            break;
        default:
            cout << "Enter a valid choice" << endl;
            break;
        }
    } while (n != 14);

    return 0;
}

  
