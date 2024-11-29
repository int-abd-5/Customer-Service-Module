#pragma once
class user {
private:
    string name;
    string country;
    int userid;
    string email;
    string type;

public:
    //int userid;
    user(string na = " ", string cr = " ", int id = 0, string em = " ", string ty = " ")
        : name(na), country(cr), userid(id), email(em), type(ty) {}

    // Define comparison operators for AVL tree usage
    bool operator<(const user& other) const {
        return userid < other.userid; // Example comparison based on userid
    }

    bool operator>(const user& other) const {
        return userid > other.userid;
    }
    friend ostream& operator<<(ostream& os, const user& us);
    bool operator==(const user& other) const {
        return userid == other.userid;
    }
    bool isequal(int num) const {
        return userid == num;
    }
    bool greater(int num) {
        return userid < num;
    }
    bool less(int num) {
        return userid > num;
    }
    void update(int& id) {
        id = userid;
    }
    void updatename(string& str) {
        str = name;
    }
    void updateemail(string& str) {
        str = email;
    }
    void updatetype(string& str) {
        str = type;
    }
    void updatecountry(string& str) {
        str = country;
    }
};
ostream& operator<<(ostream& os, const user& us) {
    os << "UserID: " << us.userid << "\n"
        << "Name: " << us.name << "\n"
        << "Email: " << us.email << "\n"
        << "Country: " << us.country << "\n"
        << "Type: " << us.type << "\n\n";
    return os;
}