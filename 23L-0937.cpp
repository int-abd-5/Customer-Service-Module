#include <iostream>
#include "avl.h"
#include "list.h"
#include "user.h"
#include "avlgroup.h"
#include "Heap.h"
#include "complaint.h"
#include <stack>
#include "avlindex.h"
using namespace std;

class userlist {
private:
    MinHeap<HoldComplaint> allComplaints;
    list<user> allusers;
    avl<int> idtree;
    avl<string> nametree;
    avl<string> emailtree;
    avlgroup<string> countrygroup;
    avlgroup<string> typegroup;
    stack<Complaint> priorityStack; 
    AVLGroup<int> userIdIndex;

public:
    userlist() {
        HoldComplaint compArray[] = {
            HoldComplaint("platinum"),
            HoldComplaint("gold"),
            HoldComplaint("silver"),
            HoldComplaint("regular"),
            HoldComplaint("new")
        };
        allComplaints = MinHeap<HoldComplaint>(compArray, 5);
    }


    bool insert(user& user1) {
        // Insert the user into the linked list and get the pointer to the new node
        auto* listNode = allusers.addatend(user1);

        // Create a new AVL tree node with the user data and the list node pointer
        user* tmp = allusers.ends();
        int userid;
        string name;
        string email,country,type;
        tmp->update(userid);
        tmp->updatename(name);
        tmp->updateemail(email);
        tmp->updatetype(type);
        tmp->updatecountry(country);


        countrygroup.insert(country, tmp);
        typegroup.insert(type, tmp);
        // Insert the new node into the AVL tree
        nametree.insert(name, listNode);
        emailtree.insert(email, listNode);
        return idtree.insert(userid,listNode);
    }

    void registerComplaint(const Complaint& comp) {
        bool found = false;
        vector<HoldComplaint> tempStorage;

        while (!allComplaints.isEmpty()) {
            HoldComplaint minHold = allComplaints.getMin();
            allComplaints.deleteMin();

            if (minHold.getUserType() == comp.getUserType()) {
                minHold.addComplaint(comp);
                found = true;
            }

            tempStorage.push_back(minHold);
        }

        for (const auto& hold : tempStorage) {
            allComplaints.insert(hold);
        }

        if (!found) {
            cout << "Error: User type not found!" << endl;
        }
        else {
            userIdIndex.insert(comp.userID, comp);
        }
    }

    void serviceComplaints() {
        if (!priorityStack.empty()) {
            // Service the complaint from the stack
            Complaint serviced = priorityStack.top();
            priorityStack.pop();
            cout << "Serviced High-Priority Complaint ID: " << serviced.complaintID
                << ", User ID: " << serviced.userID
                << ", Type: " << serviced.getUserType() << endl;
            return;
        }

        if (!allComplaints.isEmpty()) {
            try {
                HoldComplaint minHold = allComplaints.getMin();
                allComplaints.deleteMin();

                if (!minHold.isEmpty()) {
                    Complaint serviced = minHold.serviceComplaint();
                    cout << "Serviced Complaint ID: " << serviced.complaintID
                        << ", User ID: " << serviced.userID
                        << ", Type: " << serviced.getUserType() << endl;
                }

                if (!minHold.isEmpty()) {
                    allComplaints.insert(minHold);
                }
            }
            catch (const runtime_error& e) {
                cout << "Error servicing complaint: " << e.what() << endl;
            }
        }
    }

    bool search(int userid) {
        return idtree.search(userid); // Use the AVL tree's public search method
    }
    bool searchn(string str ) {
        return nametree.search(str); // Use the AVL tree's public search method
    }
    bool searche(string str) {
        return emailtree.search(str); // Use the AVL tree's public search method
    }
    bool deleteuser(int user) {
        string name, email,type,country;


        idtree.remove(user,name,email,type,country);
        countrygroup.remove(country);
        typegroup.remove(type);
        nametree.remove(name);
        emailtree.remove(email);
        return 1;
    }
    bool deleteusern(string user) {
        nametree.remove(user);
        return 1;
    }
    bool deleteusere(string user) {
        emailtree.remove(user);
        return 1;
    }

    void listUsersByCountry(const string& country) {
        cout << "Users in country " << country << ":" << endl;
        countrygroup.listUsers(country);
    }

    void listUsersByType(const string& type) {
        cout << "Users of type " << type << ":" << endl;
        typegroup.listUsers(type);
    }

    void displayCountryIndex() {
        cout << "Country Index:" << endl;
        countrygroup.display();
    }

    void displayTypeIndex() {
        cout << "Type Index:" << endl;
        typegroup.display();
    }
    
    void deleteCountryIndex(const string& country) {
        countrygroup.removeGroup(country);
        cout << "Country index for " << country << " deleted." << endl;
    }

    void deleteTypeIndex(const string& type) {
        typegroup.removeGroup(type);
        cout << "Type index for " << type << " deleted." << endl;
    }


    void printallusers() {
        //allusers.printforward();
        idtree.printInorder();
        cout << "\n Name indexes: \n";
        nametree.printInorder();
        cout << "\n Email indexes: \n";
        emailtree.printInorder();
    }

    void printAllComplaintsPriorityWise() {
        // Create a copy of the heap to avoid modifying the original
        MinHeap<HoldComplaint> tempHeap = allComplaints;
        //cout << "in function";
        while (!tempHeap.isEmpty()) {
            //cout << "*";
            try {
                HoldComplaint minHold = tempHeap.getMin();
                tempHeap.deleteMin();

                cout << "User Type: " << minHold.getUserType() << endl;
                while (!minHold.isEmpty()) {
                    Complaint comp = minHold.serviceComplaint();
                    cout << "  Complaint ID: " << comp.complaintID
                        << ", User ID: " << comp.userID
                        << ", Text: " << comp.complaintText << endl;
                }
            }
            catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
                break;
            }
        }
    }

    void increasePriority(int complaintID) {
        vector<HoldComplaint> tempStorage;
        bool found = false;

        while (!allComplaints.isEmpty()) {
            HoldComplaint minHold = allComplaints.getMin();
            allComplaints.deleteMin();

            if (minHold.increasePriority(complaintID)) {
                // Remove the complaint from the HoldComplaint and push it onto the stack
                Complaint highPriorityComplaint = minHold.serviceComplaint();
                priorityStack.push(highPriorityComplaint);
                found = true;
            }

            tempStorage.push_back(minHold);
        }

        for (const auto& hold : tempStorage) {
            allComplaints.insert(hold);
        }

        if (!found) {
            cout << "Error: Complaint ID not found!" << endl;
        }
    }
    void displayComplaintsByUserID(int userID) {
        cout << "Complaints for User ID " << userID << ":" << endl;
        userIdIndex.listComplaints(userID);
    }

    void printComplaintsByCountry(const string& country) {
        // Get the list of users for the given country
        list<user*>* usersList = countrygroup.getUsers(country);
        if (!usersList) {
            cout << "No users found in country: " << country << endl;
            return;
        }

        cout << "Complaints for users in " << country << ":" << endl;

        // Iterate over the users in the country using an iterator
        for (auto it = usersList->begin(); it != usersList->end(); ++it) {
            user* userPtr = *it;
            int userID;
            userPtr->update(userID);  // Assuming user class has a method to get user ID
            vector<Complaint>* complaints = userIdIndex.getComplaints(userID);
            if (complaints) {
                cout << "User ID: " << userID << endl;
                for (const auto& comp : *complaints) {
                    cout << "  Complaint ID: " << comp.complaintID
                        << ", Text: " << comp.complaintText
                        << ", Type: " << comp.userType << endl;
                }
            }
            else {
                cout << "No complaints found for User ID: " << userID << endl;
            }
        }
    }
};

int main() {
    
    userlist ul;

    // Test data
    user u1("Alice", "USA", 1, "alice@example.com", "gold");
    user u2("Bob", "Canada", 2, "bob@example.com", "gold");
    user u3("Charlie", "USA", 3, "charlie@example.com", "platinum");
    user u4("David", "Germany", 4, "david@example.com", "regular");
    user u5("Eve", "Germany", 5, "eve@example.com", "new");

    // Insert users
    cout << "Inserting users..." << endl;
    ul.insert(u1);
    ul.insert(u2);
    ul.insert(u3);
    ul.insert(u4);
    ul.insert(u5);

    // Print all users
    cout << "\nAll users in the list:" << endl;
    ul.printallusers();

    // Search for a user
    int searchID = 3;
    cout << "\nSearching for user with UserID " << searchID << ":" << endl;
    if (ul.search(searchID)) {
        cout << "User found." << endl;
    }
    else {
        cout << "User not found." << endl;
    }

    string str="Bob";
    cout << "\nSearching for user with name " << str << ":" << endl;
    if (ul.searchn(str)) {
        cout << "User found." << endl;
    }
    else {
        cout << "User not found." << endl;
    }

    string str2 = "alice@example.com";
    cout << "\nSearching for user with email " << str2 << ":" << endl;
    if (ul.searche(str2)) {
        cout << "User found." << endl;
    }
    else {
        cout << "User not found." << endl;
    }

    // Delete a user
    /*int deleteID = 2;
    cout << "\nDeleting user with UserID " << deleteID << ":" << endl;
    if (ul.deleteuser(deleteID)) {
        cout << "User deleted successfully." << endl;
    }
    else {
        cout << "User not found." << endl;
    }*/

    string str3 = "Charlie";
    cout << "\nDeleting for user with name " << str3 << ":" << endl;
    if (ul.deleteusern(str3)) {
        cout << "User found." << endl;
    }
    else {
        cout << "User not found." << endl;
    }

    string str4 = "alice@example.com";
    cout << "\nDeleting  for user with email " << str4 << ":" << endl;
    if (ul.deleteusere(str4)) {
        cout << "User found." << endl;
    }
    else {
        cout << "User not found." << endl;
    }
    // Print all users after deletion
    cout << "\nAll users in the list after deletion:" << endl;
    ul.printallusers();

    ul.listUsersByCountry("Canada");
    cout << "\n\n***************************************************************************\n\n";
    ul.listUsersByType("regular");
    cout << "\n\n***************************************************************************\n\n";
    ul.displayCountryIndex();
    cout << "\n\n***************************************************************************\n\n";
    ul.deleteCountryIndex("Canada");
    ul.displayCountryIndex();
    ul.deleteTypeIndex("platinum");
    ul.displayTypeIndex();

    cout << "\n\n***************************************************************************\n\n";
    // Register some complaints
    ul.registerComplaint(Complaint(1, 1, "Issue with service.", "gold"));
    ul.registerComplaint(Complaint(2, 3, "Billing issue.", "platinum"));
    ul.registerComplaint(Complaint(3, 2, "Technical support needed.", "gold"));
    ul.registerComplaint(Complaint(4, 4, "Account access problem.", "regular"));
    ul.registerComplaint(Complaint(5, 5, "New user query.", "new"));
    ul.registerComplaint(Complaint(6, 5, "not me", "new"));
    ul.registerComplaint(Complaint(7, 3, "New gaaga.", "platinum"));

    // Service complaints
    ul.serviceComplaints();
    cout << "\n\n***************************************************************************\n\n";
    ul.printAllComplaintsPriorityWise();
    cout << "\n\n***************************************************************************\n\n";
    ul.increasePriority(5);
    ul.serviceComplaints();
    ul.printAllComplaintsPriorityWise();
    cout << "\n\n***************************************************************************\n\n";
    ul.displayComplaintsByUserID(5);

    cout << "\n\n***************************************************************************\n\n";
    ul.printComplaintsByCountry("USA");
    return 0;
}
