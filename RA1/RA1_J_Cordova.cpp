#include <iostream>
using namespace std;

int main() {

    //First Name Prompt + Getter
    cout << "Please enter your first name >>" << endl;
    string firstName;
    getline(cin, firstName);

    //Last Name Prompt + Getter
    cout << "Please enter your last name >>" << endl;;
    string lastName;
    getline(cin, lastName);

    //Section Number Prompt + Getter
    cout << "Please enter your CS 280 section number >>" << endl;;
    int sectionNumber;
    cin >> sectionNumber;

    //Final Message
    cout << "Welcome " << firstName << " to CS 280 section " << sectionNumber << "." <<endl;
    return 0;
}