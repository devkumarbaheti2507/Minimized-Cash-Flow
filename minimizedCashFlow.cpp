using namespace std;
#include<bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <string>

int option;
map<string, map<string, int>> transactions, minimizedTransactions;
map<string, int> owes;
vector<string> friends;

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int displayMenu() {
    system("cls");
    gotoxy(0, 0);
    cout << "~~~~~~Minimized Cash Flow Manager~~~~~~" << endl;
    cout << "1. Add a new Friend" << endl;
    cout << "2. Add Transaction" << endl;
    cout << "3. View all Friends" << endl;
    cout << "4. View all Transactions" << endl;
    cout << "5. Calculate Minimal Cash Flow" << endl;
    cout << "6. Reset all Transactions" << endl;
    cout << "7. Reset all Data" << endl;
    cout << "8. Exit" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Enter your choice (option number) : ";
    cin >> option;
    return option;
}

string trim(const string& str) {
    size_t start = 0;
    while (start < str.size() && isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    if (start == str.size())
        return "";
    size_t end = str.size() - 1;
    while (end > start && isspace(static_cast<unsigned char>(str[end]))) {
        end--;
    }
    return str.substr(start, end - start + 1);
}

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return tolower(c); });
    return str;
}

string trimAndToLower(string s) {
    s = trim(s);
    s = toLowerCase(s);
    return s;
}

bool doesExist(const string& f) {
    for (const auto& i : friends)
        if (i == f)
            return true;
    return false;
}

string nameDisplay(const string& f) {
    stringstream ss(f);
    string word, result;
    while (ss >> word) {
        if (!word.empty()) {
            word[0] = toupper(word[0]);
            for (size_t i = 1; i < word.length(); ++i) {
                word[i] = tolower(word[i]);
            }
            result += word + " ";
        }
    }
    if (!result.empty())
        result.pop_back();
    return result;
}

bool addFriend() {
    do {
        string f, name;
        cout << "Enter the name of the friend : ";
        getline(cin >> ws, name);
        f = trimAndToLower(name);
        if (doesExist(f))
            cout << "Friend already exists" << endl;
        else {
            friends.push_back(f);
            cout << nameDisplay(f) << " added to friends list" << endl;
            break;
        }
    } while (true);
    return true;
}

bool displayFriends() {
    if (friends.empty()) {
        cout << "No friends added yet." << endl;
        return false;
    }
    cout << "Friends List:" << endl;
    int count = 1;
    for (const auto& i : friends) {
        cout << count++ << ". " << nameDisplay(i) << endl;
    }
    return true;
}

void displayTransaction(string name1, string name2, int amount) {
    cout << nameDisplay(name1) << " paid " << nameDisplay(name2) << " \u20B9" << amount << endl;
}

bool displayTransactions() {
    if (transactions.empty()) {
        cout << "No transactions recorded yet." << endl;
        return true;
    }
    cout << "~~~~~~ All Transactions ~~~~~~" << endl;
    for (const auto& payer : transactions)
        for (const auto& payee : payer.second)
            if (payee.second > 0)
                displayTransaction(payer.first, payee.first, payee.second);
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    return true;
}

bool transact(string name1, string name2, int amount) {
    transactions[name1][name2] += amount;
    owes[name1] += amount;
    owes[name2] -= amount;
    return true;
}

bool addTransaction() {
    string name1, name2;
    int amount;
    do {
        cout << "Enter the name of friend who payed : ";
        getline(cin >> ws, name1);
        name1 = trimAndToLower(name1);
        if (!doesExist(name1)) {
            string add;
            cout << "The friend does not exist" << endl;
            cout << "Do you wish to add the friend (Y \\ N) : ";
            cin >> add;
            if (add == "Y" || add == "y") {
                friends.push_back(name1);
                cout << nameDisplay(name1) << " added to friends list" << endl;
                if (!doesExist(name1))
                    continue;
            } else {
                break;
            }
        }
        cout << "Enter the name of friend who got paid : ";
        getline(cin >> ws, name2);
        name2 = trimAndToLower(name2);
        if (!doesExist(name2)) {
            string add;
            cout << "The friend does not exist" << endl;
            cout << "Do you wish to add the friend (Y \\ N) : ";
            cin >> add;
            if (add == "Y" || add == "y") {
                friends.push_back(name2);
                cout << nameDisplay(name2) << " added to friends list" << endl;
                if (!doesExist(name2))
                    continue;
            } else {
                break;
            }
        }
        if (name1 == name2) {
            cout << "Payer and payee cannot be the same person!" << endl;
            continue;
        }
        do {
            cout << "Enter the amount paid : ";
            cin >> amount;
            if (amount <= 0)
                cout << "Enter the positive amount" << endl;
        } while (amount <= 0);
        if (transact(name1, name2, amount)) {
            displayTransaction(name1, name2, amount);
            break;
        }
    } while (true);
    return true;
}

bool clearTransactions() {
    if (transactions.empty()) {
        cout << "No transactions to clear." << endl;
        return true;
    }
    transactions.clear();
    owes.clear();
    minimizedTransactions.clear();
    cout << "All transactions have been cleared." << endl;
    return true;
}

bool clearData() {
    if (transactions.empty() && friends.empty()) {
        cout << "No data to clear." << endl;
        return true;
    }
    transactions.clear();
    friends.clear();
    owes.clear();
    minimizedTransactions.clear();
    cout << "All data (friends and transactions) has been cleared." << endl;
    return true;
}

string debtor() {
    string maxDebtor = friends[0];
    for (int i = 1; i < friends.size(); i++)
        if (owes[friends[i]] < owes[maxDebtor])
            maxDebtor = friends[i];
    return maxDebtor;
}

string creditor() {
    string maxCreditor = friends[0];
    for (int i = 1; i < friends.size(); i++)
        if (owes[friends[i]] > owes[maxCreditor])
            maxCreditor = friends[i];
    return maxCreditor;
}

void displayMinimizedTransaction(string name1, string name2, int amount) {
    cout << nameDisplay(name1) << " has to pay " << nameDisplay(name2) << " \u20B9" << amount << endl;
}

bool displayMinimized() {
    if (transactions.empty()) {
        cout << "No transactions recorded yet." << endl;
        return true;
    }
    cout << "~~~ Minimized Transactions ~~~" << endl;
    for (const auto& payer : minimizedTransactions)
        for (const auto& payee : payer.second)
            if (payee.second > 0)
                displayMinimizedTransaction(payer.first, payee.first, payee.second);
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    return true;
}

bool minimize() {
    if (transactions.size() == 0) {
        cout << "No transactions to minimize the cash flow" << endl;
        return true;
    }
    string mD = debtor(), mC = creditor();
    while (!(owes[mD] == 0 && owes[mC] == 0)) {
        int amount = min(owes[mC], -1 * owes[mD]);
        minimizedTransactions[mD][mC] += amount;
        owes[mD] += amount;
        owes[mC] -= amount;
        mD = debtor();
        mC = creditor();
    }
    displayMinimized();
    return true;
}

bool selectOption() {
    system("cls");
    gotoxy(0, 0);
    bool operation = false;
    switch (option) {
        case 1:
            operation = addFriend();
            break;
        case 2:
            operation = addTransaction();
            break;
        case 3:
            operation = displayFriends();
            break;
        case 4:
            operation = displayTransactions();
            break;
        case 5:
            operation = minimize();
            break;
        case 6:
            operation = clearTransactions();
            break;
        case 7:
            operation = clearData();
            break;
        case 8:
            return false;
        default:
            cout << "Select a valid option" << endl;
            break;
    }
    return true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    bool running = true;
    while (running) {
        displayMenu();
        running = selectOption();
        if (running) {
            cout << "Press any key to continue..." << endl;
            _getch();
        }
    }
    cout << "Exiting program. Goodbye!" << endl;
    return 0;
}