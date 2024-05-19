#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Class to represent a user
class User {
private:
    string userId;
    string password;
public:
    User(const string& userId, const string& password) : userId(userId), password(password) {}

    // Getter methods
    string getUserId() const { return userId; }
    string getPassword() const { return password; }

    // Method to check if provided credentials match
    bool checkCredentials(const string& userId, const string& password) const {
        return (this->userId == userId && this->password == password);
    }

    // Method to save user credentials to file
    void saveToFile(ofstream& outFile) const{
        outFile << userId << " " << password << endl;
    }
};

// Class to manage users
class UserManager {
private:
    vector<User> users;
public:
    // Method to load users from file
    void loadUsersFromFile(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            cerr << "Error: Cannot open file " << fileName << endl;
            return;
        }

        string userId, password;
        while (inFile >> userId >> password) {
            users.push_back(User(userId, password));
        }
        inFile.close();
    }

    // Method to create a new user
    void createUser(const string& userId, const string& password, const string& fileName) {
        users.push_back(User(userId, password));
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
        for (const auto& user : users) {
            if (user.checkCredentials(userId, password)) {
                cout << "Login successful!" << endl;
                return true;
            }
        }
        cout << "Invalid credentials. Login failed." << endl;
        return false;
    }
};

int main() {
    UserManager userManager;

    // Load existing users from file
    userManager.loadUsersFromFile("users.txt");
//checking
string username;
string password;
    int n;
    
    do{
        cout<<"1.Signup"<<endl;
        cout<<"2.Login"<<endl;
        cout<<"3.exit"<<endl;
       cout<<"enter your choice: "<<endl;
       cin>>n;
       switch (n)
       {
       case 1: 
        cout<<"enter the username: "<<endl;
        cin>>username;
        cout<<"enter the password: "<<endl;
        cin>>password;
        userManager.createUser(username,password, "users.txt");
        break;
    case 2:
        cout<<"enter the username: "<<endl;
        cin>>username;
        cout<<"enter the password: "<<endl;
        cin>>password;
         userManager.authenticateUser(username,password);

    break;
    case 3:
    cout<<"exiting"
    <<endl;
    exit(0);
    break;
       
       default:
       cout<<"enter valid choice"<<endl;;
        break;
       }
    }while(n!=3);

    // Creating new users


    // Attempting login with existing credentials
   

    return 0;
}
