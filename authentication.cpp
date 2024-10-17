#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include <limits>
#include "menus.h"
#include "TableCreation.h"

#define MAX_PASSWORD_SIZE 12
#define MIN_PASSWORD_SIZE 6
#define BACK_TO_MENU "b"
#define BACK_TO_PREVIOUS_DETAIL "p"
#define CONTINUE "0"
#define ALL_GOOD "1"
#define SPACE ' '
enum Details{ID_DETAIL = 1, PASSWORD_DETAIL, NAME_DETAIL, AGE_DETAIL, FREE_TEXT_DETAIL, QUESTION_DETAIL, ROLE_DETAIL, END_OF_REGISTER};
enum {SIGN_UP = '1', LOGIN,FORGOT_PASSWORD,EXIT};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT};
enum {COUNT_FEEDBACK = 0,COUNT_FEEDBACK1,COUNT_FEEDBACK2,COUNT_FEEDBACK3};
enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK_TO_PREVIOUS_DETAIL, GO_BACK_TO_MENU};
enum SelectRole{CANDIDATE = '1', EMPLOYER, GET_BACK_TO_PREVIOUS_DETAIL, GET_BACK_TO_MENU};
#define ID_VALID 9


using namespace SQLite;
using namespace std;

//Validation of fields
//Space
//***************************************************
bool NotValidSpace(string& string)
{
    for(char i : string)
        if(i == SPACE)
        {
            cout << "You can't enter a space!\n";
            return false;
        }
    return true;
}
//***************************************************

//ID
//***************************************************
bool CheckIfIdIsDigits(string &id)
{
    for (char ch : id)
        if (!isdigit(ch))
            return false;
    return true;
}
bool validateId(string &id) {
    if (id.length() == ID_VALID && CheckIfIdIsDigits(id))
        return true;
    else
    {
        cout << "You entered an invalid ID - ID must contain exactly 9 digits.\n";
        return false;
    }
}
string EnterIdTillValid(string& id)
{
    do {
        cout << "Please enter your ID (ID must contain 9 digits).   |   Back - 'b'.:\n";
        fflush(stdin);
        getline(cin, id);
        if (id == BACK_TO_MENU)
            return BACK_TO_MENU;
    } while (!validateId(id));
    return ALL_GOOD;
}
//***************************************************

//Password
//***************************************************
int numOfCharacters(string&password)
{
    string feedback;
    bool special_character = false, lowercase = false, uppercase = false, digits = false;
    int counter = 0;
    for (char i : password)
    {
        if (isdigit(i) && !digits)
        {
            digits = true;
            ++counter;
        }
        if (islower(i) && !lowercase)
        {
            lowercase = true;
            ++counter;
        }
        if (isupper(i) && !uppercase)
        {
            uppercase = true;
            ++counter;
        }
        if (ispunct(i) && !special_character)
        {
            special_character = true;
            ++counter;
        }
    }
    return counter;
}
void PasswordDifficulty(int num)
{
    if (num == COUNT_FEEDBACK || num == COUNT_FEEDBACK1)
        cout << "The password is weak\n";
    if (num == COUNT_FEEDBACK2)
        cout << "The password is medium\n";
    if (num >= COUNT_FEEDBACK3)
        cout << "The password is strong\n";
}
string DoYouWantChangePassword(string& password)
{
    string choice;
    do {
        cout << "Do you want to change your password?\n"
                "For yes - press '0'.\n"
                "For no  - press '1' \n"
                "Back to previous detail - press 'p'.\n"
                "Back to menu - press 'b'." << endl;
        fflush(stdin);
        getline(cin, choice);
    } while (choice != CONTINUE && choice != ALL_GOOD && choice != BACK_TO_MENU && choice != BACK_TO_PREVIOUS_DETAIL);
    return choice;
}
bool validPassword(string &password)
{
    if(password.length() < MIN_PASSWORD_SIZE || password.length() > MAX_PASSWORD_SIZE)
    {
        cout << "You entered invalid password - password must contain 6 to 12 characters!\n";
        return false;
    }
    return NotValidSpace(password);
}
string EnterPasswordTillValid(string& password)
{
    string choice;
    do {
        do {
            cout << "Please enter a password (6 to 12 characters, no spaces).   |   Back - 'b'.\n"
                    "Feedback on password difficulty:\n"
                    "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
                    "- Medium: exactly two types of characters.\n"
                    "- Weak: exactly one type of character.\n";
            fflush(stdin);
            getline(cin, password);
            if (password == BACK_TO_MENU)
                return BACK_TO_MENU;
            if (password == BACK_TO_PREVIOUS_DETAIL)
                return BACK_TO_PREVIOUS_DETAIL;
        } while (!validPassword(password));
        PasswordDifficulty(numOfCharacters(password));
        choice = DoYouWantChangePassword(password);
    } while (choice == CONTINUE);
    return choice;
}
//***************************************************

//Name
//***************************************************
bool CheckIfNameIsLetters(string& name)
{
    for (char c : name)
        if (!isalpha(c))
            return false;
    return true;
}
bool validateName(string &name) {
    if(name.length() == 0 || name.length() >= 50 || !NotValidSpace(name) || !CheckIfNameIsLetters(name))
    {
        cout << "You entered an invalid name. Name must contain only letters and not exceed 50 characters. Please try again.\n";
        return false;
    }
    return true;
}
string EnterNameTillValid(string& name)
{
    do {
        cout << "Please enter your name (must contain only letters and not more than 50 letters)   |   Back - 'b':\n";
        fflush(stdin);
        getline(cin, name);
        if (name == BACK_TO_MENU)
            return BACK_TO_MENU;
        if (name == BACK_TO_PREVIOUS_DETAIL)
            return BACK_TO_PREVIOUS_DETAIL;
    } while (!validateName(name));
    return ALL_GOOD;
}
//***************************************************

//Age
//***************************************************
bool validateAge(string &age) {

    if(!CheckIfIdIsDigits(age) || age.length() == 0)
    {
        cout << "You entered an invalid age (Must be between 18 - 99) Please try again.\n";
        return false;
    }
    int ageNumber = stoi(age);
    if(ageNumber < 18 || ageNumber > 99 || !NotValidSpace(age))
    {
        cout << "You entered an invalid age (Must be between 18 - 99) Please try again.\n";
        return false;
    }
    return true;
}
string EnterAgeTillValid(string& age)
{
    cout << "Please enter your age (18 - 99)   |   Back - 'b':\n";
    do {
        fflush(stdin);
        getline(cin, age);
        if (age == BACK_TO_MENU)
            return BACK_TO_MENU;
        if (age == BACK_TO_PREVIOUS_DETAIL)
            return BACK_TO_PREVIOUS_DETAIL;
    } while (!validateAge(age));
    return ALL_GOOD;
}
//***************************************************

//FreeText
//***************************************************
bool validFreeText(string &freeText)
{
    if (freeText.length() > 200)
    {
        cout << "You entered an invalid free text. text must contain not more then 200 characters.\n";
        return false;
    }
    return true;

}
string EnterFreeTextTillValid(string& freeText)
{
    do {
        cout << "Please enter some information about yourself   |   Back - 'b':\n";
        fflush(stdin);
        getline(cin, freeText);
        if (freeText == BACK_TO_MENU)
            return BACK_TO_MENU;
        if (freeText == BACK_TO_PREVIOUS_DETAIL)
            return BACK_TO_PREVIOUS_DETAIL;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
//***************************************************

//SelectQuestion
//***************************************************
string selectQuestion(string &question, string &answer)
{
    bool flagToContinue = true;
    while (flagToContinue)
    {
        cout << "Please select a number of question:\n";
        cout << "1. What is the name of your father?\n"
             << "2. What is the name of your mother?\n"
             << "3. What is the name of the school you attended?\n"
             << "4. What year were you born?\n"
             << "5. What is your favorite food?\n"
             << "6. Back to the previous detail.\n"
             << "7. Back to menu.\n";
        char option;
        option = UserChoice();
        switch (option)
        {
            case QUESTION_1:
                question = "What is the name of your father?";
                flagToContinue = false;
                break;
            case QUESTION_2:
                question = "What is the name of your mother?";
                flagToContinue = false;
                break;
            case QUESTION_3:
                question = "What is the name of the school you attended?";
                flagToContinue = false;
                break;
            case QUESTION_4:
                question = "What year were you born?";
                flagToContinue = false;
                break;
            case QUESTION_5:
                question = "What is your favorite food?";
                flagToContinue = false;
                break;
            case GO_BACK_TO_PREVIOUS_DETAIL:
                return BACK_TO_PREVIOUS_DETAIL;
            case GO_BACK_TO_MENU:
                return BACK_TO_MENU;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
    cout << "Please enter your answer for \"" << question << "\":\n";
    fflush(stdin);
    getline(cin, answer);
    if (answer == BACK_TO_MENU)
        return BACK_TO_MENU;
    if (answer == BACK_TO_PREVIOUS_DETAIL)
        return BACK_TO_PREVIOUS_DETAIL;
    return ALL_GOOD;
}
//***************************************************

//Role
//***************************************************
string SelectCandidateOrEmployer(string& role){
    while (true) {
        cout << "Please select who are you:\n";
        cout << "1. Candidate\n"
             << "2. Employer\n"
                "3. Back to the previous detail.\n"
             << "4. Back to menu.\n";
        char option = UserChoice();
        switch (option) {
            case CANDIDATE:
                role = "candidate";
                return ALL_GOOD;
            case EMPLOYER:
                role = "employer";
                return ALL_GOOD;
            case GET_BACK_TO_PREVIOUS_DETAIL:
                return BACK_TO_PREVIOUS_DETAIL;
            case GET_BACK_TO_MENU:
                return BACK_TO_MENU;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
}
//***************************************************

//ForgotPassword
//***************************************************
bool CheckUserAnswer(Database& db, string& id, string& question, string& answer) {
    if (!ForgotPasswordTableExists(db)) {
        cout << "forgot_password table does not exist.\n";
        return false;
    }
    try {
        Statement query(db, "SELECT question,answer FROM forgot_password WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep()) {
            question = query.getColumn(0).getText();
            string dbAnswer = query.getColumn(1).getText();
            int currentDetail = QUESTION_DETAIL;
            string newPassword = "";
            printForgotPasswordSection(currentDetail, id, question, answer, newPassword);
            cout << "For change your password answer the question.\n";
            fflush(stdin);
            getline(cin, answer);
            if (dbAnswer == answer) {
                printForgotPasswordSection(currentDetail, id, question, answer, newPassword);
                cout << "Correct answer.\n";
                return true;
            } else {
                cout << "Wrong answer.\n";
                return false;
            }
        } else {
            cout << "User ID not found.\n";
            return false;
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
void ChangePassword (Database& db, string& id, string& question, string& answer) {
    string newPassword;
    printForgotPasswordSection(PASSWORD_DETAIL, id, question, answer, newPassword);
    if (EnterPasswordTillValid(newPassword) == BACK_TO_MENU)
        return;
    try {
        Statement query(db, "UPDATE users SET password = ? WHERE id = ?");
        query.bind(1, newPassword);
        query.bind(2, stoi(id));
        query.exec();
        printForgotPasswordSection(PASSWORD_DETAIL, id, question, answer, newPassword);
        cout << "Password updated successfully.\n";
    } catch(exception &e) {
        cerr << "SQLite exception: " << e.what() << "\n";
    }
}
void ForgotPassword(Database& db)
{
    string id="",question="",answer="",newPassword="";
    int currentDetail = 1;
    printForgotPasswordSection(currentDetail, id, question, answer, newPassword);
    cout << "Enter Your ID (must be 9 digits)   |   Back - 'b':\n";
    do {
        fflush(stdin);
        getline(cin, id);
        if (id == BACK_TO_MENU)
            return;
        printForgotPasswordSection(currentDetail, id, question, answer, newPassword);
    } while (!validateId(id));
    if(CheckUserAnswer(db,id, question, answer))
        ChangePassword(db,id,question,answer);
}
//***************************************************

//Register and Login
//***************************************************
void Register (Database& db)
{
    string id="", password="", name="", age="",role="",question="", answer="",freeText="";
    if(!UsersTableExists(db))
        CreateUsersTable(db);

    int detailNum = 1;
    string detailStatus;
    while (detailNum != END_OF_REGISTER)
    {
        if (detailNum == ID_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            id = "";
            detailStatus = EnterIdTillValid(id);
            if (detailStatus == BACK_TO_MENU)
                return;
            else
                detailNum = PASSWORD_DETAIL;
        }
        if (detailNum == PASSWORD_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = EnterPasswordTillValid(password);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                password = "";
                detailNum = ID_DETAIL;
            }            else
                detailNum = NAME_DETAIL;
        }
        if (detailNum == NAME_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = EnterNameTillValid(name);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                name = "";
                detailNum = PASSWORD_DETAIL;
            }            else
                detailNum = AGE_DETAIL;
        }
        if (detailNum == AGE_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = EnterAgeTillValid(age);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                age = "";
                detailNum = NAME_DETAIL;
            }            else
                detailNum = FREE_TEXT_DETAIL;
        }
        if (detailNum == FREE_TEXT_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = EnterFreeTextTillValid(freeText);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                freeText = "";
                detailNum = AGE_DETAIL;
            }            else
                detailNum = QUESTION_DETAIL;
        }
        if (detailNum == QUESTION_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = selectQuestion(question,answer);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                question = "";
                answer = "";
                detailNum = FREE_TEXT_DETAIL;
            }            else
                detailNum = ROLE_DETAIL;
        }
        if (detailNum == ROLE_DETAIL)
        {
            printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
            detailStatus = SelectCandidateOrEmployer(role);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                role = "";
                detailNum = QUESTION_DETAIL;
            }            else
                detailNum = END_OF_REGISTER;
        }
    }
    printRegisterSection(detailNum, id, password, name, age, question, answer, freeText, role);
    if(!InsertUserToDatabase(db ,id ,password ,name ,age , role, freeText))
        return;
    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    InsertForgotPasswordDetailsToDatabase(db,id,question,answer);
}
string Login(Database& db, string& name)
{
    if (!UsersTableExists(db))
    {
        cout << "Users table does not exist.\n";
        return "ERROR";
    }

    string id = "", password = "";
    while (true)
    {
        try
        {
            int currentDetail = ID_DETAIL;
            printLoginSection(currentDetail, id, password);
            if (EnterIdTillValid(id) == BACK_TO_MENU)
                return BACK_TO_MENU;
            currentDetail = PASSWORD_DETAIL;
            bool validPasswordEntered = false; // Flag to track if a valid password is entered

            while (!validPasswordEntered)
            {
                printLoginSection(currentDetail, id, password);
                cout << "Enter Your password (must be between 6-12 characters, without spaces).\n"
                        "Back to the previous detail - press 'p'.\n"
                        "Back to the menu - press 'b'.\n";
                do {
                    fflush(stdin);
                    getline(cin, password);
                    if (password == BACK_TO_MENU)
                        return BACK_TO_MENU;
                    if (password == BACK_TO_PREVIOUS_DETAIL) {
                        password = "";
                        currentDetail = ID_DETAIL;
                        break;
                    printLoginSection(currentDetail, id, password);
                    }
                } while (!validPassword(password));
                if (currentDetail == ID_DETAIL)
                    break;
                Statement query(db, "SELECT password,role,name FROM users WHERE id = ?");
                query.bind(1, stoi(id));
                if (query.executeStep())
                {
                    string dbPassword = query.getColumn(0).getText();
                    string role = query.getColumn(1).getText();
                    name = query.getColumn(2).getText();
                    if (dbPassword == password)
                    {
                        printLoginSection(currentDetail, id, password);
                        return id;
                    }
                    else
                    {
                        password = "";
                        cout << "Invalid password \n";
                        // Allow the user to try again from the top of the password loop
                    }
                }
                else
                {
                    id = "";
                    password = "";
                    cout << "User ID not found.\n";
                    // Allow the user to try again from the top of the password loop
                    break;
                }
            }
        }
        catch (exception& e)
        {
            cerr << "SQLite exception: " << e.what() << endl;
            return "ERROR";
        }
    }
}



//***************************************************
string GetUserRole(Database& db, string& id) {
    try {
        Statement query(db, "SELECT role FROM users WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep()) {
            return query.getColumn(0).getText();
        }
        return "Error";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return "Error";
}
void printRegisterSection(int currentDetail, string& id, string& password, string& name, string& age, string& question, string& answer, string&freetext, string& role)
{
    cout << " ------------------------------------------------------------------------------- \n"
         << "|                               Register Section                                |\n"
         << " ------------------------------------------------------------------------------- \n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == ID_DETAIL) cout << "--->>>    ";else cout <<"          ";  cout << "Id        -  |" << left << setw(50) << id << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == PASSWORD_DETAIL) cout << "--->>>    ";else cout <<"          ";cout  << "Password  -  |" << left << setw(50) << password << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == NAME_DETAIL) cout << "--->>>    ";else cout <<"          ";  cout  <<        "Name      -  |" << left << setw(50) << name << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == AGE_DETAIL) cout << "--->>>    ";else cout <<"          ";   cout  <<       "Age       -  |" << left << setw(50) << age << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == FREE_TEXT_DETAIL) cout << "--->>>    ";else cout <<"          ";   cout  <<       "Free-text -  |" << left << setw(50) << freetext << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == QUESTION_DETAIL) cout << "--->>>    ";else cout <<"          ";    cout  <<      "Question  -  |" << left << setw(50) << question << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == QUESTION_DETAIL) cout << "--->>>    ";else cout <<"          ";   cout  <<       "Answer    -  |" << left << setw(50) << answer << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == ROLE_DETAIL) cout << "--->>>    ";else cout <<"          ";    cout  <<      "role      -  |" << left << setw(50) << role << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << " ------------------------------------------------------------------------------- \n";
}
void printLoginSection(int currentDetail, string& id, string& password)
{
    cout << " ------------------------------------------------------------------------------- \n"
         << "|                                Login Section                                  |\n"
         << " ------------------------------------------------------------------------------- \n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == ID_DETAIL) cout << "--->>>    ";else cout <<"          ";  cout << "Id        -  |" << left << setw(50) << id << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|";if (currentDetail == PASSWORD_DETAIL) cout << "--->>>    ";else cout <<"          ";cout  << "Password  -  |" << left << setw(50) << password << "|    |\n"
         << "|                        --------------------------------------------------     |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << "|                                                                               |\n"
         << " ------------------------------------------------------------------------------- \n";
}
void printForgotPasswordSection(int currentDetail, string& id, string& question, string& answer, string& newPassword)
{
        cout << " ------------------------------------------------------------------------------- \n"
             << "|                          Forgot password Section                              |\n"
             << " ------------------------------------------------------------------------------- \n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|";if (currentDetail == ID_DETAIL) cout << "--->>> ";else cout <<"       ";  cout << "Id            - |" << left << setw(50) << id << "|    |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|";if (currentDetail == QUESTION_DETAIL) cout << "--->>> ";else cout <<"       ";cout  << "Question      - |" << left << setw(50) << question << "|    |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|";if (currentDetail == QUESTION_DETAIL) cout << "--->>> ";else cout <<"       ";cout  << "Answer        - |" << left << setw(50) << answer << "|    |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|";if (currentDetail == PASSWORD_DETAIL) cout << "--->>> ";else cout <<"       ";cout  << "New password  - |" << left << setw(50) << newPassword << "|    |\n"
             << "|                        --------------------------------------------------     |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << "|                                                                               |\n"
             << " ------------------------------------------------------------------------------- \n";
}