#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm> // for transform

using namespace std;

const int EGYPT_POPULATION = 104258327; // Placeholder for Egypt's population statistics

// User info
struct User {
    string fullName;
    string nationalID;
    string password;
    char gender;
    int age;
    string country;
    string governorate;
    bool vaccinated;
    int dosesReceived; // 0 (not vaccinated), 1 (first dose), 2 (both doses)
    User* next; // Linked list pointer
};

// Node structure for double linked list and also queue and stack
struct ListNode {
    User* user;
    ListNode* next;
    ListNode* prev;
};

// Stack for finished users 5lso kol el gor3at eli 3ndi 5lass
class Stack {
private:
    ListNode* top;
public:
    Stack() : top(nullptr) {}

    void push(User* user) {
        ListNode* newNode = new ListNode{user, nullptr, nullptr};
        if (top) {
            newNode->next = top;
            top->prev = newNode;
        }
        top = newNode;
    }

    void pop() {
        if (!top) return;
        ListNode* temp = top;
        top = top->next;
        if (top) top->prev = nullptr;
        delete temp;
    }

    void display() const {
        ListNode* current = top;
        while (current) {
            cout << current->user->fullName << endl;
            current = current->next;
        }
    }
};

// Queue implementation for waiting list w mstnyen gor3a tania
class Queue {
private:
    ListNode* front;
    ListNode* rear;
public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(User* user) {
        ListNode* newNode = new ListNode{user, nullptr, nullptr};
        if (!front) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            newNode->prev = rear;
            rear = newNode;
        }
    }

    void dequeue() {
        if (!front) return;
        ListNode* temp = front;
        front = front->next;
        if (front) front->prev = nullptr;
        delete temp;
    }

    void display() const {
        ListNode* current = front;
        while (current) {
            cout << current->user->fullName << endl;
            current = current->next;
        }
    }
};

// Binary tree node for vaccination centers a4of fully vacinied recived first dose wla unvaccinied
struct TreeNode {
    string centerName;
    TreeNode* left;
    TreeNode* right;
};

class VaccineTrackingSystem {
private:
    User* userListHead;
    ListNode* waitingListHead;
    Stack finishedStack;
    Queue nextDoseQueue;
    TreeNode* vaccinationCenterRoot;
    unordered_set<string> nationalIDs;
    int totalUsers;
    int vaccinatedUsers;
    int unvaccinatedUsers;
    int receivedFirstDose;
    int fullyVaccinated;

    bool isDuplicateID(const string& nationalID) const { //lw el user da5l nfs el id m4 hysglo w hagebha t7t tani fe el add user
        return nationalIDs.count(nationalID) > 0;
    }

    void updateCounts() {
        totalUsers = 0;
        vaccinatedUsers = 0;
        unvaccinatedUsers = 0;
        receivedFirstDose = 0;
        fullyVaccinated = 0;

        User* current = userListHead;
        while (current) {
            totalUsers++;
            if (current->vaccinated) {
                vaccinatedUsers++;
                if (current->dosesReceived == 1) {
                    receivedFirstDose++;
                } else if (current->dosesReceived == 2) {
                    fullyVaccinated++;
                }
            } else {
                unvaccinatedUsers++;
            }
            current = current->next;
        }
    }

public:
    VaccineTrackingSystem() : userListHead(nullptr), waitingListHead(nullptr), vaccinationCenterRoot(nullptr), totalUsers(0), vaccinatedUsers(0), unvaccinatedUsers(0), receivedFirstDose(0), fullyVaccinated(0) {}

    void addUser(const User& user) {
        if (isDuplicateID(user.nationalID)) {
            cout << "National ID already exists. User not added.\n";
            return;
        }

        User* newUser = new User(user);
        newUser->next = userListHead;
        userListHead = newUser;
        nationalIDs.insert(user.nationalID);
        cout << "User added successfully.\n";

        if (user.vaccinated) {
            if (user.dosesReceived == 2) {
                finishedStack.push(newUser); //5lass kda 5ad el dose kamla
            } else if (user.dosesReceived == 1) {
                nextDoseQueue.enqueue(newUser); //nas bad2t ta5od gor3a
            }
        } else { //nas lsa ma5dt4 wla dose 3ndi fa ba3mlha insert fe new dose
            ListNode* newNode = new ListNode{newUser, nullptr, nullptr};
            if (!waitingListHead) {
                waitingListHead = newNode;
            } else {
                ListNode* temp = waitingListHead;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newNode;
                newNode->prev = temp;
            }
        }

        updateCounts();
    }

    void displayAllUsers() const { //bygeb 3ndi kol el user 7ata lw wa5ed dose aw la
        User* current = userListHead;
        while (current) {
            cout << "Full Name: " << current->fullName << ", National ID: " << current->nationalID << ", Vaccinated: " << (current->vaccinated ? "Yes" : "No") << ", Doses Received: " << current->dosesReceived << endl;
            current = current->next;
        }
    }

    void displayWaitingList() const { //nas ma5dt4 wla dose
        ListNode* current = waitingListHead;
        while (current) {
            cout << "Full Name: " << current->user->fullName << endl;
            current = current->next;
        }
    }

    void viewUserRecordByNationalID(const string& nationalID) const { // 3n tare2 el admin hoa bd5l el password w mn 5lalo hna ana bd5l el id w a4of elrecord lay 7ad
        User* current = userListHead;
        while (current) {
            if (current->nationalID == nationalID) {
                cout << "Full Name: " << current->fullName << ", National ID: " << current->nationalID << ", Vaccinated: " << (current->vaccinated ? "Yes" : "No") << ", Doses Received: " << current->dosesReceived << endl;
                return;
            }
            current = current->next;
        }
        cout << "User not found.\n";
    }

    void deleteUserByNationalID(const string& nationalID) { // 3n tare2 el admin hoa bd5l el password w mn 5lalo hna ana bd5l el id w delete elrecord lay 7ad
        if (userListHead == nullptr) {
            cout << "User not found.\n";
            return;
        }

        if (userListHead->nationalID == nationalID) {
            User* temp = userListHead;
            userListHead = userListHead->next; //delete begin
            delete temp;
            nationalIDs.erase(nationalID);
            cout << "User deleted successfully.\n";
            updateCounts();
            return;
        }

        User* current = userListHead;
        while (current->next) {
            if (current->next->nationalID == nationalID) { //delete from any where
                User* temp = current->next;
                current->next = current->next->next;
                delete temp;
                nationalIDs.erase(nationalID);
                cout << "User deleted successfully.\n";
                updateCounts();
                return;
            }
            current = current->next;
        }

        cout << "User not found.\n";
    }

    void displayStatistics() const {
        cout << "Total Users: " << totalUsers << endl;
        cout << "Vaccinated Users: " << vaccinatedUsers << endl;
        cout << "Unvaccinated Users: " << unvaccinatedUsers << endl;
        cout << "Users who received at least one dose: " << receivedFirstDose + fullyVaccinated << endl;
        cout << "Fully Vaccinated Users: " << fullyVaccinated << endl;
        cout << "Percentage of those who applied for vaccination to the total population: "
             << percentageAppliedForVaccinationToTotal() << "%" << endl;
        cout << "Percentage of unvaccinated users: "
             << percentageUnvaccinated() << "%" << endl;
        cout << "Percentage of users who received at least one dose: "
             << percentageReceivedAtLeastOneDose() << "%" << endl; //el functions deh heya 3ndi tht ana 3amlha
        cout << "Percentage of fully vaccinated users: "
             << percentageFullyVaccinated() << "%" << endl;
    }

    double percentageAppliedForVaccinationToTotal() const {
        return (totalUsers * 100.0) / EGYPT_POPULATION;
    }

    double percentageUnvaccinated() const {
        return (unvaccinatedUsers * 100.0) / totalUsers;
    }

    double percentageReceivedAtLeastOneDose() const {
        return (receivedFirstDose * 100.0) / totalUsers;
    }

    double percentageFullyVaccinated() const {
        return (fullyVaccinated * 100.0) / totalUsers;
    }

    // Function to edit user information 3n tare2 el id w el password lw sa7 hy2dr y4of bynato aw y3mlha edit w yhslha update aw y3ml exist
    void editUser(const string& nationalID, const string& password) {
        User* currentUser = userListHead;
        while (currentUser != nullptr) {
            if (currentUser->nationalID == nationalID && currentUser->password == password) {
                // Edit user details
                cout << "Enter new details:\n";
                cout << "Full Name: ";
                getline(cin, currentUser->fullName);
                cout << "Gender: ";
                cin >> currentUser->gender;
                cout << "Age: ";
                cin >> currentUser->age;
                cin.ignore(); // Ignore newline character
                cout << "Country: ";
                getline(cin, currentUser->country);
                cout << "Governorate: ";
                getline(cin, currentUser->governorate);
                cout << "Password: ";
                getline(cin, currentUser->password);
                char vaccinatedChoice;
                cout << "Are you vaccinated? (Y/N): ";
                cin >> vaccinatedChoice;
                currentUser->vaccinated = (vaccinatedChoice == 'Y' || vaccinatedChoice == 'y');
                if (currentUser->vaccinated) {
                    cout << "Enter doses received (1 or 2): ";
                    cin >> currentUser->dosesReceived;
                } else {
                    currentUser->dosesReceived = 0;
                }
                cout << "Updated successfully.\n";
                return;
            }
            currentUser = currentUser->next;
        }
        cout << "User not found or incorrect credentials.\n";
    }

    User* findUserByNationalID(const string& nationalID) const { //bdwr 3al id bs
        User* current = userListHead;
        while (current) {
            if (current->nationalID == nationalID) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
};
const string adminPassword = "admin123";

bool authenticateAdmin() { //authen 34an el admin yview aw ydelete
    string enteredPassword;
    cout << "Enter Admin Password: ";
    getline(cin, enteredPassword);
    return enteredPassword == adminPassword;
}

void userMenu(VaccineTrackingSystem& system, const string& nationalID, const string& password) { //34an goz2 el user y3ml edit aw update w kda b2sm 3 3mlyat mn gwa
    // Implement the user menu similar to the main menu
    User* current = system.findUserByNationalID(nationalID);
    if (current && current->password == password) {
        while (true) {
            cout << "\n1. View My Record\n2. Edit My Record\n3. Exit\n";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                system.viewUserRecordByNationalID(nationalID);
            } else if (choice == 2) {
                system.editUser(nationalID, password);
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } else {
        cout << "Invalid national ID or password.\n";
    }
}

int main() {
    VaccineTrackingSystem system;

    while (true) {
        cout << "\n1. Add User\n2. Display All Users\n3. Display Waiting List\n";
        cout << "4. Admin View User Record By National ID\n5. Admin Delete User By National ID\n";
        cout << "6. Display Statistics\n7. Exit\n8. User Login\n";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            User user;
            cout << "Enter Full Name: ";
            getline(cin, user.fullName);
            cout << "Enter National ID: ";
            getline(cin, user.nationalID);
            cout << "Enter Password: ";
            getline(cin, user.password);
            cout << "Enter Gender (M/F): ";
            cin >> user.gender;
            cout << "Enter Age: ";
            cin >> user.age;
            cin.ignore();
            cout << "Enter Country: ";
            getline(cin, user.country);
            cout << "Enter Governorate: ";
            getline(cin, user.governorate);
            char vaccinatedChoice;
            cout << "Are you vaccinated? (Y/N): ";
            cin >> vaccinatedChoice;
            user.vaccinated = (vaccinatedChoice == 'Y' || vaccinatedChoice == 'y');
            if (user.vaccinated) {
                cout << "Enter doses received (1 or 2): ";
                cin >> user.dosesReceived;
            } else {
                user.dosesReceived = 0;
            }

            system.addUser(user);
        } else if (choice == 2) {
            system.displayAllUsers();
        } else if (choice == 3) {
            system.displayWaitingList();
        } else if (choice == 4) {
            if (!authenticateAdmin()) {
                cout << "Incorrect admin password.\n";
                continue;
            }
            string nationalID;
            cout << "Enter National ID: ";
            getline(cin, nationalID);
            system.viewUserRecordByNationalID(nationalID);
        } else if (choice == 5) {
            if (!authenticateAdmin()) {
                cout << "Incorrect admin password.\n";
                continue;
            }
            string nationalID;
            cout << "Enter National ID: ";
            getline(cin, nationalID);
            system.deleteUserByNationalID(nationalID);
        } else if (choice == 6) {
            system.displayStatistics();
        } else if (choice == 7) {
            break;
        } else if (choice == 8) {
            string nationalID, password;
            cout << "Enter National ID: ";
            getline(cin, nationalID);
            cout << "Enter Password: ";
            getline(cin, password);
            userMenu(system, nationalID, password);
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
