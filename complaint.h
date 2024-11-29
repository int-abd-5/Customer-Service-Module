#pragma once
#include<iostream>
#include <queue>


class Complaint {
public:
    int complaintID;
    int userID;
    string complaintText;
    string userType;

    Complaint(int c = 0, int x = 0, string i = "", string typeo = "")
        : complaintID(c), userID(x), complaintText(i), userType(typeo) {}

    bool operator<(const Complaint& other) const {
        return complaintID > other.complaintID; // Lower ID has higher priority
    }

    string getUserType() const {
        return userType;
    }
};

class HoldComplaint {
private:
    queue<Complaint> complaints;
    string userType;

public:
    HoldComplaint(const string& type) : userType(type) {}

    void addComplaint(const Complaint& c) {
        complaints.push(c);
    }

    Complaint serviceComplaint() {
        if (complaints.empty()) {
            throw runtime_error("No complaints to service.");
        }
        Complaint top = complaints.front();
        complaints.pop();
        return top;
    }

    bool isEmpty() const {
        return complaints.empty();
    }

    string getUserType() const {
        return userType;
    }

    int getComplaintCount() const {
        return complaints.size();
    }


    //bool increasePriority(int complaintID) {
    //    queue<Complaint> tempQueue;
    //    bool found = false;
    //    Complaint highPriorityComplaint;

    //    // Transfer elements to a temporary queue
    //    while (!complaints.empty()) {
    //        Complaint current = complaints.front();
    //        complaints.pop();
    //        if (current.complaintID == complaintID) {
    //            highPriorityComplaint = current;
    //            found = true;
    //        }
    //        else {
    //            tempQueue.push(current);
    //        }
    //    }

    //    // If found, push the high priority complaint to the front
    //    if (found) {
    //        complaints.push(highPriorityComplaint);
    //    }

    //    // Restore the rest of the complaints
    //    while (!tempQueue.empty()) {
    //        complaints.push(tempQueue.front());
    //        tempQueue.pop();
    //    }

    //    return found;
    //}


    bool increasePriority(int complaintID) {
        queue<Complaint> tempQueue;
        bool found = false;
        Complaint highPriorityComplaint;

        // Transfer elements to a temporary queue
        while (!complaints.empty()) {
            Complaint current = complaints.front();
            complaints.pop();
            if (current.complaintID == complaintID) {
                highPriorityComplaint = current;
                found = true;
            }
            else {
                tempQueue.push(current);
            }
        }

        // If found, push the high priority complaint to the front
        if (found) {
            // Push the high priority complaint to the front
            complaints.push(highPriorityComplaint);
        }

        // Restore the rest of the complaints
        while (!tempQueue.empty()) {
            complaints.push(tempQueue.front());
            tempQueue.pop();
        }

        return found;
    }
    
    bool operator>(const HoldComplaint& other) const {
        return getTypePriority(userType) > getTypePriority(other.userType);
    }
    bool operator<(const HoldComplaint& other) const {
        return getTypePriority(userType) < getTypePriority(other.userType);
    }
private:
    int getTypePriority(const string& type) const {
        if (type == "platinum") return 1;
        if (type == "gold") return 2;
        if (type == "silver") return 3;
        if (type == "regular") return 4;
        return 5; // "new"
    }
};