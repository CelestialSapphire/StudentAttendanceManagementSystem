#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

const int max_tutor = 100;
const int max_students = 100;
const int month = 12;
const int date = 31;
const int numSubjects = 5;
string subject;
int subject_code, tutor_count, student_count;
string student_username, student_password, student_usn;
string tutor_username, tutor_password;
struct tutor
{
    string username;
    string password;
};
struct student
{
    string username;
    string password;
    string studentName;
    string usn;
    char status;
    unordered_map<string, int> attendanceCount;
};

struct attendance_record
{
    string usn;
    char status;
};

string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", ltm);
    return string(buffer);
}
tutor tutorlist[max_tutor];
student studentlist[max_students];
bool isUserExists(const string &userType, const string &usernameOrUSN)
{
    ifstream userFile(userType + "Details.csv");
    if (userFile.is_open())
    {
        string line;
        while (getline(userFile, line))
        {
            stringstream ss(line);
            string usnOrUsername;
            getline(ss, usnOrUsername, ','); // Read USN or username
            if (usnOrUsername == usernameOrUSN)
            {
                userFile.close();
                return true; // User already exists
            }
        }
        userFile.close();
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    // }
    return false; // User does not exist
}

void TutorSignup(string userType)
{
    tutor newuser;
    cout << "Enter " << userType << " username: ";
    cin >> newuser.username;

    // Check if the username already exists
    if (isUserExists("Tutor", newuser.username))
    {
        cout << "Username already exists. Please choose a different one.\n";
        return;
    }

    cout << "Enter " << userType << " password: ";
    cin >> newuser.password;

    ofstream userFile("TutorDetails.csv", ios::app);
    if (userFile.is_open())
    {
        userFile << newuser.username << "," << newuser.password << endl;
        userFile.close();
        cout << "\n"
             << userType << " signed up successfully.\n";
        tutorlist[tutor_count++] = newuser;
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    // }
}

void StudentSignup(string userType)
{
    student newuser;
    cout << "Enter " << userType << " USN: ";
    cin >> newuser.usn;

    // Check if the USN already exists
    if (isUserExists("Student", newuser.usn))
    {
        cout << "USN already exists. Please choose a different one.\n";
        return;
    }

    cout << "Enter " << userType << " username: ";
    cin >> newuser.username;

    // Check if the username already exists
    if (isUserExists("Student", newuser.username))
    {
        cout << "Username already exists. Please choose a different one.\n";
        return;
    }

    cout << "Enter " << userType << " password: ";
    cin >> newuser.password;

    ofstream userFile("StudentDetails.csv", ios::app);
    if (userFile.is_open())
    {
        userFile << newuser.usn << "," << newuser.username << ","
                 << newuser.password << endl;
        userFile.close();
        cout << "\n"
             << userType << " signed up successfully.\n";
        studentlist[student_count++] = newuser;
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    // }
}

bool StudentLogin(string userType)
{

    cout << "Enter " << userType << " USN: ";
    cin >> student_usn;
    cout << "Enter " << userType << " username: ";
    cin >> student_username;
    cout << "Enter " << userType << " password: ";
    cin >> student_password;

    ifstream userFile("StudentDetails.csv");
    if (userFile.is_open())
    {
        string line;
        while (getline(userFile, line))
        {
            stringstream ss(line);
            string usn, storedUsername, storedPassword;
            getline(ss, usn, ',');            // Read USN
            getline(ss, storedUsername, ','); // Read username
            getline(ss, storedPassword);      // Read password
            if (usn == student_usn && storedUsername == student_username &&
                storedPassword == student_password)
            {
                cout << "\n"
                     << userType << " logged in successfully.\n\n";
                userFile.close();
                return true;
            }
        }
        userFile.close();
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    // }
    cout << "Invalid " << userType << " credentials. Login failed.\n";
    return false;
}

bool TutorLogin(string userType)
{

    cout << "Enter " << userType << " username: ";
    cin >> tutor_username;
    cout << "Enter " << userType << " password: ";
    cin >> tutor_password;

    ifstream userFile("TutorDetails.csv");
    string line;
    while (getline(userFile, line))
    {
        size_t pos = line.find(",");
        string storedUsername = line.substr(0, pos);
        string storedPassword = line.substr(pos + 1);
        if (storedUsername == tutor_username && storedPassword == tutor_password)
        {
            cout << "\n\U0001F680" << userType << " logged in successfully.\U0001F680\n";
            userFile.close();
            return true;
        }
    }
    userFile.close();
    cout << "Invalid " << userType << " credentials. Login failed.\n";
    return false;
}

void viewStudents(const string &userType)
{
    ifstream userFile(userType + "Details.csv");
    if (userFile.is_open())
    {
        cout << "\nStudents registered are: \n\n";
        string usn, username, password;
        int sno = 1;
        cout << "SL.No\tUSN\tName\n";
        while (getline(userFile, usn, ',') && getline(userFile, username, ',') &&
               getline(userFile, password))
        {
            cout << sno++ << ".\t" << usn << "\t" << username << "\n";
        }
        userFile.close();
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    // }
}
void viewTimetable()
{
    cout << "----------------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << setw(15) << "Time" << setw(25) << "Monday" << setw(25) << "Tuesday"
         << setw(25) << "Wednesday" << setw(25) << "Thursday" << setw(25) << "Friday"
         << "\n";
    cout << "----------------------------------------------------------------------------------------------------------------------------------------------\n";

    cout << setw(15) << "9:00-10:00" << setw(25) << "OS" << setw(25) << "MATHS"
         << setw(25) << "TEA BREAK" << setw(25) << "DDCO" << setw(25) << "DSA"
         << "\n";
    cout << setw(15) << "10:00-11:00" << setw(25) << "JAVA" << setw(25) << "OS"
         << setw(25) << "TEA BREAK" << setw(25) << "DSA" << setw(25) << "MATHS"
         << "\n";
    cout << setw(15) << "11:00-12:00" << setw(25) << "OSL / OOPSL" << setw(25) << "OSL / OOPSL"
         << setw(25) << "TEA BREAK" << setw(25) << "DSAL / DDCOL" << setw(25) << "DSAL / DDCOL"
         << "\n";
    cout << setw(15) << "12:00-1:00" << setw(25) << "OSL / OOPSL" << setw(25) << "OSL / OOPSL"
         << setw(25) << "TEA BREAK" << setw(25) << "DSAL / DDCOL" << setw(25) << "DSAL / DDCOL"
         << "\n";
    cout << setw(15) << "1:00-2:00" << setw(25) << "MATHS" << setw(25) << "OS"
         << setw(25) << "TEA BREAK" << setw(25) << "DAE" << setw(25) << "DAE"
         << "\n";
    cout << setw(15) << "2:00-3:00" << setw(25) << "JAVA" << setw(25) << "MATHS"
         << setw(25) << "TEA BREAK" << setw(25) << "JAVA" << setw(25) << "MATHS"
         << "\n";
    cout << "----------------------------------------------------------------------------------------------------------------------------------------------\n";
}
void takeAttendance(string userType)
{
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    vector<vector<attendance_record>> attendanceRecords(numSubjects);

    int subject_code;
    string subject;

    cout << "Maths - 1\nDSA - 2\nJava - 3\nOS - 4\nDDCO - 5\n";
    cout << "Enter subject code: ";
    cin >> subject_code;

    if (subject_code >= 1 && subject_code <= numSubjects)
    {
        subject = subjects[subject_code - 1];
    }
    else
    {
        cerr << "Invalid subject code\n";
        return;
    }

    string currentDate = getCurrentDate();
    cout << "\nTaking attendance on " << currentDate << "\n"
         << "Subject: " << subject << endl;

    // Check if attendance has already been taken for the current date and subject
    string attendanceFileName = subject + "_Attendance.csv";
    ifstream attendanceFile(attendanceFileName);
    if (attendanceFile.is_open())
    {
        string line;
        while (getline(attendanceFile, line))
        {
            stringstream ss(line);
            string date;
            getline(ss, date, ',');
            if (date == currentDate)
            {
                cout << "Attendance for " << subject << " has already been taken today.\n";
                attendanceFile.close();
                return;
            }
        }
        attendanceFile.close();
    }

    ofstream attendanceFileOut(attendanceFileName, ios::app);

    ifstream userFile(userType + "Details.csv");
    if (userFile.is_open())
    {
        string usn, username, password;
        int sno = 1;
        cout << "\nSL.No\tUSN\tName\tAttendance\n";
        while (getline(userFile, usn, ',') && getline(userFile, username, ',') &&
               getline(userFile, password))
        {
            cout << sno++ << ".\t" << usn << "\t" << username << "\t";
            char status;
            cin >> status;
            attendanceFileOut << currentDate << "," << username << "," << usn << ","
                              << status << endl;
            if (subject_code >= 1 && subject_code <= numSubjects)
            {
                attendanceRecords[subject_code - 1].push_back({usn, status});
            }
            else
            {
                cerr << "Invalid subject code\n";
                return;
            }
        }
        userFile.close();
    }
    // else
    // {
    //     cerr << "Error opening the file for " << userType << " details." << endl;
    //     attendanceFileOut.close();
    //     return;
    // }

    attendanceFileOut.close();

    cout << "\n Attendance taken successfully.🏳\n";
}

void calculateStudentAttendancePercentage(string userType)
{
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    if (userType == "Tutor")
    {
        cout << "Enter student name: ";
        cin >> student_username;
        cout << "Enter student USN: ";
        cin >> student_usn;
    }
    cout << "Attendance percentage for " << student_username << " (USN: " << student_usn << "):\n\n";
    cout << "Subject\t\tAttendance Percentage\n";

    for (int i = 0; i < numSubjects; ++i)
    {
        string filename = subjects[i] + "_Attendance.csv";
        ifstream file(filename);

        if (!file.is_open())
        {
            cerr << "Error opening the file " << filename << endl;
            continue;
        }

        int presentCount = 0;
        int totalAttendance = 0;

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string date, usn, status, name;
            getline(ss, date, ',');
            getline(ss, name, ',');
            getline(ss, usn, ',');
            getline(ss, status, ',');

            if (usn == student_usn)
            {
                if (status == "P")
                {
                    presentCount++;
                }
                totalAttendance++;
            }
        }
        file.close();

        double attendancePercentage = 0.0;
        if (totalAttendance > 0)
        {
            attendancePercentage =
                static_cast<double>(presentCount) / totalAttendance * 100.0;
        }

        cout << subjects[i] << "\t\t" << fixed << setprecision(2)
             << attendancePercentage << "%\n";
    }
}

void viewAttendanceForCurrentDate()
{
    string currentDate = getCurrentDate(); // Get current date
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    for (int i = 0; i < numSubjects; ++i)
    {
        cout << "-----------------------------------------------------------\n";
        cout << "Viewing attendance for " << subjects[i] << " on " << currentDate
             << "\n";

        ifstream attendanceFile(subjects[i] + "_Attendance.csv");
        if (!attendanceFile.is_open())
        {
            cerr << "Attendance file not found for " << subjects[i] << " on "
                 << currentDate << "\n";
            continue;
        }

        cout << setw(30) << "Name" << setw(15) << "USN" << setw(15)
             << "Attendance\n";
        string line;
        string usn, username, date, status;
        while (getline(attendanceFile, date, ',') &&
               getline(attendanceFile, usn, ',') &&
               getline(attendanceFile, username, ',') &&
               getline(attendanceFile, status))
        {
            if (date == currentDate)
            {
                cout << setw(30) << username << setw(15) << usn << setw(12) << status
                     << endl;
            }
        }
        attendanceFile.close();
        cout << "-----------------------------------------------------------\n";
    }
}
void displayAbsentees()
{
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    string currentDate = getCurrentDate();

    for (int i = 0; i < numSubjects; ++i)
    {
        cout << "-----------------------------------------------------------\n";
        cout << "Displaying absentees for " << subjects[i] << " on " << currentDate
             << "\n";

        ifstream attendanceFile(subjects[i] + "_Attendance.csv");
        cout << setw(30) << "USN" << setw(15) << "Name" << endl;

        string line, usn, username, status, date;
        while (getline(attendanceFile, line))
        {
            istringstream ss(line);
            getline(ss, date, ',');
            getline(ss, usn, ',');
            getline(ss, username, ',');
            getline(ss, status);

            // Check if the student is absent (status == "A")
            if (status == "A")
            {
                if (date == currentDate)
                    cout << setw(30) << username << setw(15) << usn << endl;
            }
        }

        attendanceFile.close();
        cout << "-----------------------------------------------------------\n";
    }
}
void viewAttendanceForOtherDate()
{
    string other_date;
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    cout << "Input date(dd-mm-yyyy) to view Attendance: ";
    cin >> other_date;
    for (int i = 0; i < numSubjects; ++i)
    {
        cout << "-----------------------------------------------------------\n";
        cout << "Viewing attendance for " << subjects[i] << " on " << other_date
             << "\n";

        ifstream attendanceFile(subjects[i] + "_Attendance.csv");
        if (!attendanceFile.is_open())
        {
            cerr << "Attendance file not found for " << subjects[i] << " on "
                 << other_date << "\n";
            continue;
        }

        cout << setw(30) << "Name" << setw(15) << "USN" << setw(15)
             << "Attendance\n";
        string line;
        string date, usn, username, status;
        // bool attendanceFound = false; // Flag to check if attendance is found for the specified date
        while (getline(attendanceFile, date, ',') &&
               getline(attendanceFile, usn, ',') &&
               getline(attendanceFile, username, ',') &&
               getline(attendanceFile, status))
        {
            if (date == other_date)
            {
                cout << setw(30) << username << setw(15) << usn << setw(12) << status
                     << endl;
                // attendanceFound = true;
            }
        }
        attendanceFile.close();
        cout << "-----------------------------------------------------------\n";
    }
}
void displayAbsenteesForOtherDate()
{
    string other_date;
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};
    cout << "Input date(dd-mm-yyyy) to view Attendance: ";
    cin >> other_date;
    for (int i = 0; i < numSubjects; ++i)
    {
        ifstream attendanceFile(subjects[i] + "_Attendance.csv");
        if (!attendanceFile.is_open())
        {
            cerr << "Attendance file not found for " << subjects[i] << " on "
                 << other_date << "\n";
            continue;
        }
        cout << "-----------------------------------------------------------\n";
        cout << "Displaying absentees for " << subjects[i] << " on " << other_date
             << "\n";
        cout << setw(30) << "Name" << setw(15) << "USN" << setw(15)
             << "Attendance\n";
        string line;
        string date, usn, username, status;
        while (getline(attendanceFile, line))
        {
            istringstream ss(line);
            getline(ss, date, ',');
            getline(ss, usn, ',');
            getline(ss, username, ',');
            getline(ss, status);

            // Check if the student is absent (status == "A")
            if (status == "A")
            {
                if (date == other_date)
                    cout << setw(30) << username << setw(15) << usn << endl;
            }
        }

        attendanceFile.close();
        cout << "-----------------------------------------------------------\n";
    }
}
void giveRemarks()
{
    string tutorName, studentName, studentUSN, subject;

    cout << "Enter student name: ";

    cin >> studentName;
    cout << "Enter student USN: ";

    cin >> studentUSN;
    cout << "Enter subject: ";

    cin >> subject;
    cout << "Enter remarks for " << studentName << " (USN: " << studentUSN << ") in "
         << subject << ": ";
    string remarks;
    cin.ignore();
    getline(cin, remarks);

    ofstream remarksFile(subject + "_Remarks.csv", ios::app);
    // if (!remarksFile.is_open())
    // {
    //     cerr << "Error opening the file for remarks" << endl;
    //     return;
    // }
    remarksFile << tutor_username << "," << studentUSN << "," << studentName
                << "," << remarks << endl;
    remarksFile.close();
    cout << "Remarks added successfully.\n";
}

void message()
{
    string currentDate = getCurrentDate();
    string role, name, subject;
    cout << "Enter role(Tutor/Student): ";
    cin >> role;
    if (role == "tutor" || role == "Tutor")
    {
        name = tutor_username;
    }
    else
    {
        name = student_username;
    }
    cout << "Enter message \n";
    string message;
    cin.ignore(); // Ignore any remaining newline characters in the input buffer
    getline(cin, message);
    ofstream messagesFile("Message.csv", ios::app);
    // if (!messagesFile.is_open())
    // {
    //     cerr << "Error opening the file for messages" << endl;
    //     return;
    // }
    messagesFile << currentDate << "," << role << "," << name << "," << message << endl;
    messagesFile.close();
    cout << "Message added successfully.\n";
}
void displaymessage()
{
    string currentDate = getCurrentDate();
    ifstream messagesFile("Message.csv");
    string line;
    // bool found = false;
    cout << "-------------------------------------------------------------------------------------------------------------------------\n";
    while (getline(messagesFile, line))
    {
        stringstream ss(line);
        string role, name, message, currentDate;
        getline(ss, currentDate, ',');
        getline(ss, role, ',');
        getline(ss, name, ',');
        getline(ss, message);
        cout << currentDate << " - " << role << " - " << name << " - " << message << "\n";
        // found = true;
        cout << "-------------------------------------------------------------------------------------------------------------------------\n";
    }

    messagesFile.close();
}
void searchUser() {
    string usernameOrUSN,userType;
    cout << "Enter the userType(Student/Tutor): ";
    cin >> userType;
    cout << "\nEnter the username (for tutor) or USN (for student) to search: ";
    cin >> usernameOrUSN;

    ifstream inputFile;
    if (userType == "tutor" || userType == "Tutor") {
        inputFile.open("tutorDetails.csv"); // Assuming "tutorDetails.csv" is the file containing tutor details
    } else if (userType == "student" || userType == "Student") {
        inputFile.open("studentDetails.csv"); // Assuming "studentDetails.csv" is the file containing student details
    } else {
        cout << "Invalid user type." << endl;
        return;
    }

    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line;
    bool found = false;

    // Read each line from the file
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string usnOrUsername, name;

        // Extract USN/username and name from the line
        getline(ss, usnOrUsername, ',');
        getline(ss, name, ',');

        // Check if the entered username/USN matches with the current record
        if (usnOrUsername == usernameOrUSN) {
            found = true;
            if (userType == "student") {
                cout << "Student record found\n";
                cout << "USN: " << usnOrUsername << endl; // Display the USN
                cout << "Username: " << name << endl; // Display the name
            } else {
                cout << "Tutor record found\n";
                cout << "Username: " << usnOrUsername << endl; // Display the username
            }
            
            cout << "-------------------------------------------------------------------------------------------------------------------------\n";
            break; // Exit the loop after finding the record
        }
    }

    if (!found) {
        cout << "Record not found." << endl;
    }

    inputFile.close(); // Close the input file
}

void displayRemarks()
{
    string studentName, studentUSN;
    string subjects[] = {"Maths", "DSA", "Java", "OS", "DDCO"};

    for (int i = 0; i < 5; i++)
    {
        ifstream remarksFile(subjects[i] + "_Remarks.csv");
        string line;
        // bool found = false;
        while (getline(remarksFile, line))
        {
            stringstream ss(line);
            string tutorName, usn, name, remarks;
            getline(ss, tutorName, ',');
            getline(ss, usn, ',');
            getline(ss, name, ',');
            getline(ss, remarks);

            if (usn == student_usn && name == student_username)
            {
                cout << "Remarks given by " << tutorName << " for " << studentName
                     << "(USN: " << student_usn << ") in " << subjects[i] << ": " << remarks
                     << endl;
                // found = true;
                break;
            }
        }
        remarksFile.close();
    }
}

void deleteStudentRecord(const string &userType)
{
    string usernameOrUSN;
    cout << "Enter the username or USN of the student you want to delete: ";
    cin >> usernameOrUSN;

    ifstream inputFile(userType + "Details.csv");
    ofstream tempFile("temp.csv");

    if (inputFile.is_open() && tempFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string usnOrUsername;
            getline(ss, usnOrUsername, ','); // Read USN or username

            // Check if the current line contains the specified username or USN
            if (usnOrUsername != usernameOrUSN)
            {
                tempFile << line << endl; // Write the line to the temporary file
            }
        }
        inputFile.close();
        tempFile.close();

        // Remove the original file and rename the temporary file
        remove((userType + "Details.csv").c_str());
        rename("temp.csv", (userType + "Details.csv").c_str());

        cout << "Student record deleted successfully.\n";
    }
    else
    {
        cerr << "Error: Unable to open files for deleting student record.\n";
    }
}
void editUserDetails(const string &userType)
{
    string usernameOrUSN;
    cout << "Enter the username (for tutor) or USN (for student) to edit: ";
    cin >> usernameOrUSN;

    ifstream inputFile(userType + "Details.csv");
    ofstream tempFile("temp.csv");

    if (inputFile.is_open() && tempFile.is_open())
    {
        string line;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string usn, username, password;
            getline(ss, usn, ',');
            getline(ss, username, ',');
            getline(ss, password);

            // Check if the current line contains the specified username or USN
            if (usernameOrUSN == username || usernameOrUSN == usn)
            {
                string newUsername, newPassword, newusn;
                if (userType == "Student" || userType == "student")
                {
                    cout << "Enter the new USN: ";
                    cin >> newusn;
                }
                cout << "Enter the new Username: ";
                cin >> newUsername;
                cout << "Enter the new password: ";
                cin >> newPassword;

                // Update the username and password
                usn = newusn;
                username = newUsername;
                password = newPassword;
            }

            // Write the updated line to the temporary file
            if (userType == "Student" || userType == "student")
                tempFile << usn << ',' << username << ',' << password << endl;
            else
                tempFile << username << ',' << password << endl;
        }
        inputFile.close();
        tempFile.close();

        // Remove the original file and rename the temporary file
        remove((userType + "Details.csv").c_str());
        rename("temp.csv", (userType + "Details.csv").c_str());

        cout << "User details updated successfully.\n";
    }
    else
    {
        cerr << "Error: Unable to open files for editing user details.\n";
    }
}
int main()
{
    system("cls");
    string userType;
    int choice;
    string currentDate = getCurrentDate();
    cout << "----- 🌟 WELCOME TO STUDENT MANAGEMENT SYSTEM 🌟 -----" << endl;
    while (true)
    {
        cout << "-------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. 📚 Tutor New Register\n";
        cout << "2. 🎓 Student New Register\n";
        cout << "3. 📚 Tutor login\n";
        cout << "4. 🎓 Student login\n";
        cout << "5. ❌ Exit\n";
        cout << "-------------------------\n";
        cout << "👉 Enter your choice: ";
        cin >> choice;
        cout << "\n";
        switch (choice)
        {
        case 1:
            TutorSignup("Tutor");
            break;
        case 2:
            StudentSignup("Student");
            break;
        case 3:
            if (TutorLogin("Tutor"))
            {
                int tutorOption;
                while (true)
                {
                    cout << "\n--------------------------------------\n";
                    cout << "1. Take Attendance\n";
                    cout << "2. Display Absentees\n";
                    cout << "3. View Students\n";
                    cout << "4. View Attendance For Current Date\n";
                    cout << "5. View Timetable\n";
                    cout << "6. View Attendance For Other Date\n";
                    cout << "7. Display Absentees For Other Date\n";
                    cout << "8. View Attendance percentage\n";
                    cout << "9. Give remarks\n";
                    cout << "10. Message\n";
                    cout << "11. See Message\n";
                    cout << "12. Edit user details\n";
                    cout << "13. Search user details\n";
                    cout << "14. Go Back\n";
                    cout << "0. Exit\n";
                    cout << "--------------------------------------\n";
                    cout << "Enter your choice: ";
                    cin >> tutorOption;

                    switch (tutorOption)
                    {
                    case 1:
                        takeAttendance("student");
                        break;
                    case 2:
                        displayAbsentees();
                        break;
                    case 3:
                        viewStudents("STUDENT");
                        break;
                    case 4:
                        viewAttendanceForCurrentDate();
                        break;
                    case 5:
                        viewTimetable();
                        break;

                    case 6:
                        viewAttendanceForOtherDate();
                        break;
                    case 7:
                        displayAbsenteesForOtherDate();
                        break;
                    case 8:
                        calculateStudentAttendancePercentage("Tutor");
                        break;
                    case 9:
                        giveRemarks();
                        break;
                    case 10:
                        message();
                        break;
                    case 11:
                        displaymessage();
                        break;
                    case 12:
                        cout << "Student or Tutor? :";
                        cin >> userType;
                        editUserDetails(userType);
                        break;
                    case 13:
                        searchUser();
                        break;
                    case 14:
                        cout << "Going back.\n";
                        break;
                    case 0:
                        exit(0);
                    default:
                        cout << "Invalid option. Please try again.\n";
                    }

                    if (tutorOption == 13)
                        break;
                } // while tutor actions
            }
            else
            {
                exit(0);
            }
            break;
        case 4:
            if (StudentLogin("Student"))
            {
                int studentOption;
                while (true)
                {
                    cout << "\n--------------------------------------\n";
                    cout << "1. View Attendance For Current Date\n";
                    cout << "2. View Timetable\n";
                    cout << "3. Display Absentees\n";
                    cout << "4. View Attendace Percentage\n";
                    cout << "5. Display remarks\n";
                    cout << "6. View Attendance for Other Date\n";
                    cout << "7. Display Absentees for Other Date\n";
                    cout << "8. Message\n";
                    cout << "9. See Message\n";
                    cout << "10. Delete a Student Record\n";
                    cout << "11. Edit user details\n";
                    cout << "12. Search user details\n";
                    cout << "13. Go back\n";
                    cout << "0. Exit\n";
                    cout << "--------------------------------------\n";
                    cout << "👉 Enter your choice: ";

                    cin >> studentOption;
                    cout << "\n";
                    switch (studentOption)
                    {
                    case 1:
                        viewAttendanceForCurrentDate();
                        break;
                    case 2:
                        viewTimetable();
                        break;
                    case 3:
                        displayAbsentees();
                        break;
                    case 4:
                        calculateStudentAttendancePercentage("Student");
                        break;
                    case 5:
                        displayRemarks(); // Display remarks for the student
                        break;
                    case 6:
                        viewAttendanceForOtherDate();
                        break;
                    case 7:
                        displayAbsenteesForOtherDate();
                        break;
                    case 8:
                        message();
                        break;
                    case 9:
                        displaymessage();
                        break;
                    case 10:
                        deleteStudentRecord("Student");
                        break;
                    case 11:
                        cout << "Student or Tutor? :";
                        cin >> userType;
                        editUserDetails(userType);
                        break;
                    case 12:
                        searchUser();
                        break;
                    case 13:
                        cout << "Going back.\n";
                        break;
                    case 0:
                        exit(0);
                    default:
                        cout << "Invalid option. Please try again.\n";
                    } // switch student actions

                    if (studentOption == 13)
                        break;
                } // while student actions
            }     // if student actions
            else
            {
                exit(0);
            }
            break;
            
        case 5:
            exit(0);

        default:
            cout << "Invalid option. Please try again.\n";
        } // switch
    }     // while
    return 0;
}