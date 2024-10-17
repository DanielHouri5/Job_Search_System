//
// Created by david on 22/02/2024.
//
#include <windows.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include "employer.h"
#include "candidate.h"
#include "authentication.h"
#include "menus.h"
#include "TableCreation.h"

#define WIDTH 45
#define BORDER_CHAR '|'
#define FILL_CHAR '-'
#define BACK_TO_MENU "b"
#define BACK_TO_PREVIOUS_DETAIL "p"
#define CONTINUE "0"
#define ALL_GOOD "1"
enum ResumeDetailsMenu{FULL_NAME_DETAIL = 1, AGE_DETAIL, DEGREE1_DETAIL, DEGREE2_DETAIL, DEGREE3_DETAIL, WORK_EXPERIENCE_DETAIL, YEARS_OF_EXPERIENCE_DETAIL, END_OF_RESUME};
enum DegreesMenu{DOES_NOT_HAVE = '1', BA, MA, PHD};
enum EditingSearchMenu{CHANGE_NAME = '1', CHANGE_AGE, CHANGE_PASSWORD,CHANGE_FREETEXT,CHANGE_QUESTION, BACK_TO_MENU_};
enum CategoriesSearchMenu{SEARCH_BY_LOCATION = '1', SEARCH_BY_SCOPE, SEARCH_BY_EXPERIENCE_YEARS, SEARCH_BY_PROFESSION, BACK_TO_LOOK_FOR_JOBS_MENU};
using namespace std;

// Designing
void printLine(int width, char borderChar, char fillChar) {
    cout << setfill(borderChar) << setw(width) << borderChar << endl;
}
void printRow(string text, int width, char borderChar)
{
    int textLength = text.length();
    int fillerLength = width - 4 - textLength; // 4 is for the spaces and borders
    cout << borderChar << "   " << text << setw(fillerLength) << setfill(' ') << " " << borderChar << endl;
}

// Jobs
void ViewAllJobs(Database& db)
{
    if (!JobsListExists(db))
    {
        cout << "JobsList table does not exist.\n";
        return;
    }
    try {
        Statement query(db, "SELECT * FROM jobs_list");
        while (query.executeStep())
        {
            string jobId = query.getColumn(0).getText();
            string companyName = query.getColumn(2).getText();
            string location = query.getColumn(3).getText();
            string position = query.getColumn(4).getText();
            string scope = query.getColumn(6).getText();
            string experience = query.getColumn(7).getText();
            PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
        }
    }
    catch(exception & e)
    {
        cout << "No jobs found.\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
string SelectJob(Database& db, string& id, vector<string>& filteredJobs)
{
    string jobIdChoice;
    cout << "Select a job - press its number.   |   Back - press 'b'." << endl;
    bool firstIter = true;

    do {
        if (!firstIter)
            cout << "You entered an invalid selection, try again   |   Back - 'b'." << endl;
        firstIter = false;
        fflush(stdin);
        getline(cin, jobIdChoice);
        if (jobIdChoice == "b")
            return "b";
    } while (!CheckIfIdIsDigits(jobIdChoice));

    try {
        if (!filteredJobs.empty())
        {
            bool thereIsAJob = false;
            for (string job: filteredJobs)
                if (jobIdChoice == job)
                {
                    thereIsAJob = true;
                    break;
                }
            if (!thereIsAJob) {
                cout << "No jobs found.\n";
                return "ERROR";
            }
        }
        Statement query(db, "SELECT * FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(jobIdChoice));
        if (query.executeStep()) {
            string jobId = query.getColumn(0);
            if (jobId == jobIdChoice) {
                string companyName = query.getColumn(2).getText();
                string location = query.getColumn(3).getText();
                string position = query.getColumn(4).getText();
                string description = query.getColumn(5).getText();
                string scope = query.getColumn(6).getText();
                string experience = query.getColumn(7).getText();
                string salary = query.getColumn(8).getText();
                PrintJobMaxDetails(jobId, companyName, location, position, scope, experience, description, salary);
                return jobId;
            }
        }
        else
        {
            cout << "No jobs found.\n";
            return "ERROR";
        }
    } catch(exception& e) {
        cout << "No jobs found.\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
    return "ERROR";
}
char SelectFilter()
{
    while (true) {
        cout << "Please select the filter you want to activate:\n";
        cout << "1. Filter by location.\n"
             << "2. Filter by scope.\n"
             << "3. Filter by experience years.\n"
                "4  Filter by your wanted profession.\n"
                "5  Back.\n";
        char option = UserChoice();
        switch (option)
        {
            case SEARCH_BY_LOCATION:
                return SEARCH_BY_LOCATION;
            case SEARCH_BY_SCOPE:
                return SEARCH_BY_SCOPE;
            case SEARCH_BY_EXPERIENCE_YEARS:
                return SEARCH_BY_EXPERIENCE_YEARS;
            case SEARCH_BY_PROFESSION:
                return SEARCH_BY_PROFESSION;
            case BACK_TO_LOOK_FOR_JOBS_MENU:
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
}
char AnswerTheFilter(vector<string>& answerForFilter)
{
    char option = SelectFilter();
    switch (option)
    {
        case SEARCH_BY_LOCATION: {
            string locationChoice;
            if (EnterTillValid(locationChoice, "location") == BACK_TO_MENU)
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            answerForFilter.push_back(locationChoice);
            return SEARCH_BY_LOCATION;
        }
        case SEARCH_BY_SCOPE: {
            string scopeChoice = SelectScope();
            answerForFilter.push_back(scopeChoice);
            return SEARCH_BY_SCOPE;
        }
        case SEARCH_BY_EXPERIENCE_YEARS: {
            string minYearsOfExperience, maxYearsOfExperience;
            bool firstIter = true;
            do {
                if (!firstIter)
                    cout << "The min of years can't be bigger than the max of year!" << endl;
                firstIter = false;
                cout << "Please enter the min of experience years (0 - 50)   |   Back - 'b':\n";
                if (EnterYearsOfExperienceTillValid(minYearsOfExperience, "min of ") == BACK_TO_MENU)
                    return BACK_TO_LOOK_FOR_JOBS_MENU;
                cout << "Please enter the max of experience years (0 - 50)   |   Back - 'b':\n";
                if (EnterYearsOfExperienceTillValid(maxYearsOfExperience, "max of ") == BACK_TO_MENU)
                    return BACK_TO_LOOK_FOR_JOBS_MENU;
            } while (stoi(minYearsOfExperience) > stoi(maxYearsOfExperience));
            answerForFilter.push_back(minYearsOfExperience);
            answerForFilter.push_back(maxYearsOfExperience);
            return SEARCH_BY_EXPERIENCE_YEARS;
        }
        case SEARCH_BY_PROFESSION: {
            string professionChoice;
            if (EnterTillValid(professionChoice, "profession") == BACK_TO_MENU)
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            answerForFilter.push_back(professionChoice);
            return SEARCH_BY_PROFESSION;
        }
        case BACK_TO_LOOK_FOR_JOBS_MENU:
            return BACK_TO_LOOK_FOR_JOBS_MENU;
        default:
            cout << "You entered an illegal option. Please try again!\n";
    }
}
string SearchByCategory(Database& db, vector<string>& filteredJobs)
{
    if (!JobsListExists(db))
    {
        cout << "JobsList table does not exist.\n";
        return "ERROR";
    }
    try {
        vector<string> answerForFilter;
        char choice = AnswerTheFilter(answerForFilter);;
        if (choice == BACK_TO_LOOK_FOR_JOBS_MENU)
            return "b";
        Statement query(db, "SELECT * FROM jobs_list");
        bool flagIfAnyJobWasPrinted = false;
        while (query.executeStep()) {
            string jobId = query.getColumn(0).getText();
            string companyName = query.getColumn(2).getText();
            string location = query.getColumn(3).getText();
            string position = query.getColumn(4).getText();
            string scope = query.getColumn(6).getText();
            string experience = query.getColumn(7).getText();
            if (choice == SEARCH_BY_LOCATION)
                if (location == answerForFilter[0])
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            if (choice == SEARCH_BY_SCOPE)
                if (scope == answerForFilter[0])
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            if (choice == SEARCH_BY_EXPERIENCE_YEARS)
            {
                int experienceInt = stoi(experience);
                if (experienceInt >= stoi(answerForFilter[0]) && experienceInt <= stoi(answerForFilter[1]))
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            }
            if (choice == SEARCH_BY_PROFESSION)
                if (position == answerForFilter[0]) {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
        }
        if (!flagIfAnyJobWasPrinted)
        {
            cout << "No jobs found.\n";
            return "ERROR";
        }
    }
    catch(exception & e)
    {
        cout << "No jobs found.\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
    return "ALL_GOOD";
}
void PrintJobMinDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience)
{
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow(jobId, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow("Position: " + position, WIDTH, BORDER_CHAR);
    printRow("Location: " + location, WIDTH, BORDER_CHAR);
    printRow("Company: " + companyName, WIDTH, BORDER_CHAR);
    printRow("Scope: " + scope, WIDTH, BORDER_CHAR);
    printRow("Experience years: " + experience, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
}
void PrintJobMaxDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience, string& description, string& salary)
{
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow(jobId, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow("Position: " + position, WIDTH, BORDER_CHAR);
    printRow("Location: " + location, WIDTH, BORDER_CHAR);
    printRow("Company: " + companyName, WIDTH, BORDER_CHAR);
    printRow("Scope: " + scope, WIDTH, BORDER_CHAR);
    printRow("Experience years: " + experience, WIDTH, BORDER_CHAR);
    printRow("Description : " + description, WIDTH, BORDER_CHAR);
    printRow("Salary: " + salary, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
}
bool CheckIfOnlyLettersOrSpace(string& name)
{
    for (char c : name)
    {   if(c == ' ')
        {
            continue;
        }
        else if (!isalpha(c) )
            return false;
    }
    return true;
}
bool strIsValid(string &str, const string& strMessage) {
    if(str.length() == 0 || str.length() >= 50 ||   !CheckIfOnlyLettersOrSpace(str))
    {
        cout << "You entered an invalid " << strMessage << "(Must contain only letters and not exceed 50 characters).\n"
             << "Please try again.\n";
        if (strMessage != "location" && strMessage != "profession")
            cout << "Or back to the previous detail - press 'p'.\n";
        cout << "Or back to menu - 'b'.\n";
        return false;
    }
    return true;
}
string EnterTillValid(string& str, const string& strMessage)
{
        cout << "Please enter your " << strMessage << " (must contain only letters and not more than 50 letters).\n";
        if (strMessage != "location" && strMessage != "profession")
            cout << "Or back to the previous detail - press 'p'.\n";
        cout << "Or back to menu - 'b'.\n";

    do {
        fflush(stdin);
        getline(cin, str);
        if (str == BACK_TO_MENU)
            return BACK_TO_MENU;
        if (str == BACK_TO_PREVIOUS_DETAIL)
            return BACK_TO_PREVIOUS_DETAIL;
    } while (!strIsValid(str, strMessage));
    return ALL_GOOD;
}
bool validateYearsOfExperience2(string & years_of_experience, const string& strMessage)
{
    if(!CheckIfIdIsDigits(years_of_experience)|| years_of_experience.length() == 0)
    {
        cout << "You entered an invalid " << strMessage << "experience years(Must be between 0 - 50) Please try again.\n";
        if (strMessage == "")
            cout << "Or back to the previous detail - press 'p'.\n";
        cout << "Or back to menu - 'b'.\n";
        return false;
    }
    int ageNumber = stoi(years_of_experience);
    if(ageNumber < 0 || ageNumber > 50 || !NotValidSpace(years_of_experience))
    {
        cout << "You entered an invalid " << strMessage << "experience years(Must be between 0 - 50) Please try again.\n";
        if (strMessage == "")
            cout << "Or back to the previous detail - press 'p'.\n";
        cout << "Or back to menu - 'b'.\n";
        return false;
    }
    return true;
}
string EnterYearsOfExperienceTillValid(string& years_of_experience, const string& strMessage)
{

    do {
        fflush(stdin);
        getline(cin, years_of_experience);
        if (years_of_experience == BACK_TO_MENU)
            return BACK_TO_MENU;
        if (years_of_experience == BACK_TO_PREVIOUS_DETAIL)
            return BACK_TO_PREVIOUS_DETAIL;
    } while (!validateYearsOfExperience2(years_of_experience, strMessage));
    return ALL_GOOD;
}
//***********

// Resumes
void CreateResume(Database& db, string& id)
{
    string choice;
    cout << "Create your resume - press 'r'.   |   Back - press 'b'." << endl;
    bool firstIter = true;
    do
    {
        if (!firstIter)
            cout << "You entered an invalid choice, please try again   |   Back - 'b'." << endl;
        firstIter = false;
        fflush(stdin);
        getline(cin, choice);
        if (choice == BACK_TO_MENU)
            return;
    } while (choice!="r");

    string full_name, age, degree1, degree2, degree3, work_experience, years_of_experience;
    int detailNum = 1;
    string detailStatus;
    while (detailNum != END_OF_RESUME)
    {
        //FullName
        if (detailNum == FULL_NAME_DETAIL)
        {
            detailStatus = EnterTillValid(full_name, "full name");
            if (detailStatus == BACK_TO_MENU || detailStatus == BACK_TO_PREVIOUS_DETAIL)
                return;
            else
                detailNum = AGE_DETAIL;
        }
        //Age
        if (detailNum == AGE_DETAIL)
        {
            detailStatus = EnterAgeTillValid(age);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                age = "";
                detailNum = FULL_NAME_DETAIL;
            }
            else
                detailNum = DEGREE1_DETAIL;
        }

        //Degrees
        int degreesNumber;
        if (detailNum == DEGREE1_DETAIL) {
            degreesNumber = SelectDegree();
            if (degreesNumber == 0)
                detailNum = WORK_EXPERIENCE_DETAIL;
            if (degreesNumber >= 1 && degreesNumber <= 3)
            {
                detailStatus = EnterTillValid(full_name, "Bachelor's degree (B.A.)");
                if (detailStatus == BACK_TO_MENU)
                    return;
                else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
                {
                    degree1 = "";
                    detailNum = AGE_DETAIL;
                }
                else {
                    if (degreesNumber == 1)detailNum = WORK_EXPERIENCE_DETAIL;
                    if (degreesNumber == 2 || degreesNumber == 3)detailNum = DEGREE2_DETAIL;
                    }
            }
        }
        if (detailNum == DEGREE2_DETAIL)
        {
            if (degreesNumber >= 2 && degreesNumber <= 3)
            {
                detailStatus = EnterTillValid(full_name, "Master's degree (M.A.)");
                if (detailStatus == BACK_TO_MENU)
                    return;
                else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
                {
                    degree2 = "";
                    detailNum = DEGREE1_DETAIL;
                }
                else {
                    if (degreesNumber == 2)detailNum = WORK_EXPERIENCE_DETAIL;
                    if (degreesNumber == 3)detailNum = DEGREE3_DETAIL;
                }
            }
        }
        if (detailNum == DEGREE3_DETAIL)
        {
            if (degreesNumber == 3)
            {
                detailStatus = EnterTillValid(full_name, "Ph.D degree");
                if (detailStatus == BACK_TO_MENU)
                    return;
                if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
                {
                    degree3 = "";
                    detailNum = DEGREE2_DETAIL;
                }
                else
                    detailNum = WORK_EXPERIENCE_DETAIL;
            }
        }
        if (detailNum == WORK_EXPERIENCE_DETAIL)
        {//WorkExperience
            detailStatus = EnterFreeTextTillValid(work_experience);
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                work_experience = "";
                if (degreesNumber == 0)detailNum = DEGREE1_DETAIL;
                if (degreesNumber == 1)detailNum = DEGREE1_DETAIL;
                if (degreesNumber == 2)detailNum = DEGREE2_DETAIL;
                if (degreesNumber == 3)detailNum = DEGREE3_DETAIL;
            }
            else
                detailNum = YEARS_OF_EXPERIENCE_DETAIL;
        }
        if (detailNum == YEARS_OF_EXPERIENCE_DETAIL)
        {//YearsOfExperience
            cout << "Please enter your years of experience(Must be between 0 - 50)."
                    "Or back to the previous detail - press 'p'.\n"
                    "Or back to menu - 'b'.\n";
            detailStatus = EnterYearsOfExperienceTillValid(years_of_experience, "");
            if (detailStatus == BACK_TO_MENU)
                return;
            else if (detailStatus == BACK_TO_PREVIOUS_DETAIL)
            {
                years_of_experience = "";
                detailNum = WORK_EXPERIENCE_DETAIL;
            }
            else
                detailNum = END_OF_RESUME;
        }

    }

    string confirm;
    do {
        cout << "Confirm - press 'c'   |   Back - press 'b'." << endl;
        fflush(stdin);
        getline(cin, confirm);
        if (confirm == "b")
            return;
    } while (confirm != "c");

    if (!ResumesTableExists(db))
        CreateResumeTable(db);
    InsertResumeToTable(db, id, full_name, age, degree1, degree2, degree3, work_experience, years_of_experience);
}
int SelectDegree(){
    while (true) {
        cout << "Please select which degree you have, if any:\n";
        cout << "1. You don't have any degree.\n"
             << "2. Bachelor's degree (B.A.).\n"
             << "3. Master's degree (M.A.).\n"
             << "4. PhD.\n";
        char option = UserChoice();
        switch (option)
        {
            case DOES_NOT_HAVE:
                return 0;
            case BA:
                return 1;
            case MA:
                return 2;
            case PHD:
                return 3;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
}


// Resume submissions
void SubmitResume(Database& db, string& id, string& jobId)
{
    string choice;
    do {
        cout << "Submit your resume - press 's'   |   Back - press 'b'." << endl;
        fflush(stdin);
        getline(cin, choice);
        if (choice == "b")
            return;
    } while (choice != "s");

    if (!ResumeSubmissionsTableExists(db))
        CreateResumeSubmissionsTable(db);
    InsertSubmitToTable(db, id, jobId);
}
void ViewAllSubmittedJobs(Database& db, string& candidate_id)
{
    if (!ResumeSubmissionsTableExists(db))
    {
        cout << "Submissions table does not exist.\n";
        return;
    }
    try {
        Statement query1(db, "SELECT * FROM submission WHERE candidate_id = ?");
        query1.bind(1, stoi(candidate_id));
        bool flagIfAnyJobWasPrinted = false;
        while (query1.executeStep())
        {
            string submittedJobId = query1.getColumn(1).getText();
            Statement query2(db, "SELECT * FROM jobs_list");
            while (query2.executeStep())
            {
                string jobId = query2.getColumn(0).getText();
                if (jobId == submittedJobId)
                {
                    flagIfAnyJobWasPrinted = true;
                    string companyName = query2.getColumn(2).getText();
                    string location = query2.getColumn(3).getText();
                    string position = query2.getColumn(4).getText();
                    string scope = query2.getColumn(6).getText();
                    string experience = query2.getColumn(7).getText();
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                }
            }
        }
        if (!flagIfAnyJobWasPrinted)
            cout << "No jobs found.\n";
    }
    catch(exception & e)
    {
        cout << "No jobs found.\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}

//Edit profile
void editName(Database& db,  string& id)
{
    string newName;
    if (EnterNameTillValid(newName) == "b")
        return;
    try {
        db.exec("UPDATE users SET name = '" + newName + "' WHERE id = '" + id + "'");
        cout << "Name updated successfully.\n";
    } catch (exception& e) {
        cout << "Id not found!\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editAge(Database& db,  string& id) {
    string newAge;
    if (EnterAgeTillValid(newAge) == "b")
        return;
    try {
        db.exec("UPDATE users SET age = " + newAge + " WHERE id = '" + id + "'");
        cout << "Age updated successfully.\n";
    } catch (exception& e) {
        cout << "Id not found!\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editPassword(Database& db,  string& id)
{
    string newPassword;
    if (EnterPasswordTillValid(newPassword) == "b")
        return;
    try {
        db.exec("UPDATE users SET password = '" + newPassword + "' WHERE id = '" + id + "'");
        cout << "Password updated successfully.\n";
    } catch (exception& e) {
        cout << "Id not found!\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editFreeText(Database& db,  string& id) {
    string newFreetext;
    if (EnterFreeTextTillValid(newFreetext) == "b")
        return;
    try {
        // Update the database with the new free text
        db.exec("UPDATE users SET freetext = '" + newFreetext + "' WHERE id = '" + id + "'");
        cout << "Free text updated successfully.\n";
    } catch (exception& e) {
        cout << "Id not found!\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editQuestion(Database& db,  string& id)
{
    string newQuestion, newAnswer;
    if (selectQuestion(newQuestion, newAnswer) == "b")
        return;
    string query = "UPDATE forgot_password SET question = '" + newQuestion + "', answer = '" + newAnswer + "' WHERE id = '" + id + "'";
    try {
        db.exec(query);
        cout << "Question and answer updated successfully.\n";
    } catch (exception& e) {
        cout << "Id not found!\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editProfile(Database& db, string& id) {
    char option;
    while (true)
    {
        cout << "Choose an option to edit your profile:\n"
                "1. Edit name.\n"
                "2. Edit age.\n"
                "3. Edit password.\n"
                "4. Edit Freetext.\n"
                "5. Edit questions.\n"
                "6. Back.\n";
        option = UserChoice();
        switch (option)
        {
            case CHANGE_NAME:
                editName(db, id);
                break;
            case CHANGE_AGE:
                editAge(db, id);
                break;
            case CHANGE_PASSWORD:
                editPassword(db, id);
                break;
            case CHANGE_FREETEXT:
                editFreeText(db, id);
                break;
            case CHANGE_QUESTION:
                editQuestion(db, id);
                break;
            case BACK_TO_MENU_:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
//Invitations
void RejectAcceptInterviewInvitation(Database&db,string&id)
{
    while (true)
    {
        bool flag = true;
        try
        {
            // Select data from submission table where status is accepted and candidate_id matches the provided id
            Statement selectQuery(db, "SELECT job_id FROM submission WHERE status = 'accepted' AND candidate_id = ?;");

            // Bind the candidate_id parameter
            selectQuery.bind(1, id);

            // Execute the query
            bool foundInvitation = false;
            while (selectQuery.executeStep())
            {
                string job_id = selectQuery.getColumn(0).getText(); // Column index starts from 0
                cout << "You have an interview invitation for job id: " << job_id << endl;
                foundInvitation = true;
            }

            // If no invitation is found, return from the function
            if (!foundInvitation) {
                cout << "You dont have interview invitation\n";
                return;
            }
        } catch (const exception &e)
        {
            cerr << "SQLite exception: " << e.what() << endl;
        }
        Statement selectQuery(db, "SELECT job_id FROM submission WHERE status = 'accepted' AND candidate_id = ? AND job_id = ?;");
        string job_id;
        cout << "select a job id you want to accept/reject the invitation   |    Back - 'b'.\n";
        do
        {

            fflush(stdin);
            getline(cin , job_id);
            selectQuery.bind(1, id);
            selectQuery.bind(2, job_id);

            // Execute the query
            bool foundInvitation = false;
            while (selectQuery.executeStep())
            {
                foundInvitation = true;
            }
            if (!foundInvitation)
            {
                cout << "Invalid job ID. Please select from the provided invitations.\n";
                flag = false;

            }
            if (job_id == BACK_TO_MENU)
                return;
        }
        while(!CheckIfIdIsDigits(job_id));
        if(flag)
        {
            int count = 0;
            while (true)
            {
                char choice;
                string status;
                bool flagToContinue = true;
                while (flagToContinue) {
                    cout << "1. Confirms arrival" << endl;
                    cout << "2. Cancels arrival" << endl;
                    cout << "3. Go Back to the menu" << endl;
                    choice = UserChoice();
                    switch (choice) {
                        case '1':
                            status = "confirms_arrival";
                            flagToContinue = false;
                            break;
                        case '2':
                            status = "cancels_arrival";
                            flagToContinue = false;
                            break;
                        case '3':
                        {
                            count = 1;
                            flagToContinue = false;
                            return;
                        }
                        default:
                            cout << "Invalid choice" << endl;
                            break;
                    }
                }

                if(count == 1)
                    break;
                try {
                    // Update submission status based on choice
                    db.exec("UPDATE submission SET status = '" + status + "' WHERE job_id = '" + job_id +
                            "' AND candidate_id = '" + id + "'");

                    cout << "Invitation " << status << endl;
                    return;
                } catch (exception &e)
                {
                    cerr << "SQLite exception: " << e.what() << endl;
                }
            }
        }
    }
}
void ViewAllInterviewQuestions(Database& db, string& candidate_id) {
    string question,answer1,answer2,answer3,answer4,employer_id,employer_name,question_id , question_choice,answer_choice;
    bool flag_query1 = false;
    bool flag_query2 = false;
    try {
        Statement query1(db, "SELECT question, answer1, answer2, answer3, answer4, employer_id, id FROM tests WHERE candidate_id = ? AND grade IS NULL");
        query1.bind(1, stoi(candidate_id));
        while (query1.executeStep()) {
            flag_query1 = true;
            question = query1.getColumn(0).getText();
            answer1 = query1.getColumn(1).getText();
            answer2 = query1.getColumn(2).getText();
            answer3 = query1.getColumn(3).getText();
            answer4 = query1.getColumn(4).getText();
            employer_id = query1.getColumn(5).getText();
            question_id = query1.getColumn(6).getText();

            cout << "Question number : " <<question_id << "\n" << question <<"\n"
                 << "1. "<< answer1 << "\n"
                 << "2. "<< answer2 << "\n"
                 << "3. "<< answer3 << "\n"
                 << "4. "<< answer4 << "\n"
                 << " ===================================== \n \n";
        }
        if (!flag_query1) {
            cout << "No tests.\n";
            return;
        }
        cout << "Enter number of question you want answer : \n";
        bool firstIter = true;
        do {
            if (!firstIter)
                cout << "You entered an illegal choice, please try again:" << endl;
            firstIter = false;
            fflush(stdin);
            getline(cin, question_choice);
        } while (question_choice < "1" || question_choice > "4");

        try {
            Statement query2(db,"SELECT id,correct_answer FROM tests WHERE id = ?");
            query2.bind(1, stoi(question_choice));
            while (query2.executeStep()) {
                flag_query2 = true;
                string storedCorrectAnswer = query2.getColumn(1).getText();
                cout << "Enter the answer 1-4 : \n";
                bool firstIter2 = true;
                do {
                    if (!firstIter2)
                        cout << "You entered an illegal choice, please try again:" << endl;
                    firstIter2 = false;
                    fflush(stdin);
                    getline(cin, answer_choice);
                } while (answer_choice < "1" || answer_choice > "4");
                if(storedCorrectAnswer == answer_choice) {
                    int grade = 100;
                    Statement query3(db,"UPDATE tests SET grade = ? WHERE id = ?");
                    query3.bind(1, grade);
                    query3.bind(2, stoi(question_choice));
                    query3.exec();
                    cout << "Correct\n";
                }
                else {
                    int grade = 0;
                    Statement query3(db,"UPDATE tests SET grade = ? WHERE id = ?");
                    query3.bind(1, grade);
                    query3.bind(2, stoi(question_choice));
                    query3.exec();
                    cout << "Wrong\n";
                }
            }
            if(!flag_query2) {
                cout << "Wrong question choice\n";
            }
        } catch(exception & e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }

    }
    catch(exception & e)
    {
        cout << "No tests.\n";
        //cerr << "SQLite exception: " << e.what() << endl;
    }
}