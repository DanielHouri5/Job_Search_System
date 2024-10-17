#include "menus.h"
#include "authentication.h"
#include "employer.h"
#include "candidate.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>
#include <limits>
#include "iostream"
using namespace SQLite;
using namespace std;
enum SearchMenu{VIEW_ALL_JOBS = '1', SEARCH_BY_CATEGORY, BACK_TO_CANDIDATE_MENU};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE,VIEW_QUESTIONS, LOG_OUT_C};
enum EmployerMenu{PUBLISH_JOB = '1',DELETE_JOB,EDIT_JOB, VIEW_ALL_JOBS_YOU_PUBLISHED,FILTER,EDIT_PROFILE_EMP,SEND_INVITATION,VIEW_INVITATION, LOG_OUT_E};
enum MainMenu{REGISTER = '1', LOGIN,FORGOT_PASSWORD,EXIT};
void name(int mat[5][5]);


void CandidateMenu(Database& db , string& id, string& name) {
    char option;
    while (true) {
        cout << " ---------------------------------------------------------- \n"
             << "| Welcome Back " << left << name << setw(43 - name.length()) <<  "!" << " |\n"
             << "|                         JobSearch                        |\n"
             << " ---------------------------------------------------------- \n"
             << "|     1.  --------------------------------------------     |\n"
             << "|        |               Look for jobs                |    |\n"
             << "|         --------------------------------------------     |\n"
             << "|     2.  --------------------------------------------     |\n"
             << "|        |             Create your resume             |    |\n"
             << "|         --------------------------------------------     |\n"
             << "|     3.  --------------------------------------------     |\n"
             << "|        | View all the jobs you submitted your resume|    |\n"
             << "|         --------------------------------------------     |\n"
             << "|     4.  --------------------------------------------     |\n"
             << "|        | View all the interview invitations you got |    |\n"
             << "|         --------------------------------------------     |\n"
             << "|     5.  --------------------------------------------     |\n"
             << "|        |             Edit your profile              |    |\n"
             << "|         --------------------------------------------     |\n"
             << "|     6.  --------------------------------------------     |\n"
             << "|        |           View interview questions              |\n"
             << "|         --------------------------------------------     |\n"
             << "|     7.  --------------------------------------------     |\n"
             << "|        |                   Log out                  |    |\n"
             << "|         --------------------------------------------     |\n"
             << " ---------------------------------------------------------- \n";

        cout << "Please enter your choice: " << endl;
        option = UserChoice();
        switch (option) {
            case LOOK_FOR_JOBS:
                LookForJobsMenu(db, id);
                break;
            case CREATE_RESUME:
                CreateResume(db, id);
                break;
            case VIEW_JOBS_SUBMITTED:
                ViewAllSubmittedJobs(db, id);
                break;
            case VIEW_INTERVIEW_INVITATIONS:
                RejectAcceptInterviewInvitation(db,id);
                break;
            case EDIT_PROFILE:
                editProfile(db, id);
                break;
            case VIEW_QUESTIONS:
                ViewAllInterviewQuestions(db,id);
                break;
            case LOG_OUT_C:
                return;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void EmployerMenu(Database& db, string& id, string& name) {
    char option;
    while (true)
    {
        cout << " ---------------------------------------------------------- \n";
        cout << "| Welcome Back " << left << name << setw(43 - name.length()) <<  "!" << " |\n";
        cout << "|                         JobSearch                        |\n";
        cout << " ---------------------------------------------------------- \n";
        cout << "|     1.  --------------------------------------------     |\n";
        cout << "|        |               Publish a job                |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     2.  --------------------------------------------     |\n";
        cout << "|        |               Delete a job                 |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     3.  --------------------------------------------     |\n";
        cout << "|        |                Edit a job                  |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     4.  --------------------------------------------     |\n";
        cout << "|        |         View all the published jobs        |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     5.  --------------------------------------------     |\n";
        cout << "|        |             Filter the resumes             |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     6.  --------------------------------------------     |\n";
        cout << "|        |                Edit profile                |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     7.  --------------------------------------------     |\n";
        cout << "|        |           Send interview invitation        |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     8.  --------------------------------------------     |\n";
        cout << "|        | View all the interview invitations you sent|    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << "|     9.  --------------------------------------------     |\n";
        cout << "|        |                   Log out                  |    |\n";
        cout << "|         --------------------------------------------     |\n";
        cout << " ---------------------------------------------------------- \n";
        cout << "Please enter your choice: " << endl;
        option = UserChoice();
        switch (option)
        {
            case PUBLISH_JOB:
                PostJob(db,id);
                break;
            case DELETE_JOB:
                DeleteJob(db,id);
                break;
            case EDIT_JOB:
                EditJob(db,id);
                break;
            case VIEW_ALL_JOBS_YOU_PUBLISHED:
                FetchAllJobs(db,id);
                break;
            case FILTER:
                FilterCandidateResume(db,id);
                break;
            case EDIT_PROFILE_EMP:
                editProfile(db, id);
                break;
            case SEND_INVITATION:
                SendInterviewInvitation(db,id);
                break;
            case VIEW_INVITATION:
                ViewAllInterviewInvitation(db,id);
                break;
            case LOG_OUT_E:
                return;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void MainMenu(Database& db)
{
    char option;
    while (true)
    {
        cout << " --------------------------------------------- \n"
             << "|                 JobSearch                   |\n"
             << " --------------------------------------------- \n"
             << "|     1.  -----------------------------       |\n"
             << "|        |         Register            |      |\n"
             << "|         -----------------------------       |\n"
             << "|     2.  -----------------------------       |\n"
             << "|        |          Login              |      |\n"
             << "|         -----------------------------       |\n"
             << "|     3.  -----------------------------       |\n"
             << "|        |      Forgot password        |      |\n"
             << "|         -----------------------------       |\n"
             << "|     4.  -----------------------------       |\n"
             << "|        |           Exit               |     |\n"
             << "|         -----------------------------       |\n"
             << " --------------------------------------------- \n";
        cout << "Please enter your choice: " << endl;

        option = UserChoice();
        switch (option)
        {

            case REGISTER:
            {
                Register(db);
                break;
            }

            case LOGIN:
            {
                string name;
                string id = Login(db, name);
                if (id == "b")
                    break;
                string role = GetUserRole(db,id);
                if(role == "employer")
                    EmployerMenu(db,id,name);
                else
                    CandidateMenu(db,id,name);
                break;
            }
            case FORGOT_PASSWORD:
            {
                ForgotPassword(db);
                break;
            }
            case EXIT:
                return;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void LookForJobsMenu(Database& db, string& id)
{
    char option;
    while (true) {
        cout << " ---------------------------------------------------- \n"
             << "|                     JobSearch                      |\n"
             << " ---------------------------------------------------- \n"
             << "|     1.  --------------------------------------     |\n"
             << "|        |         View all the jobs            |    |\n"
             << "|         --------------------------------------     |\n"
             << "|     2.  --------------------------------------     |\n"
             << "|        |         Search by category           |    |\n"
             << "|         --------------------------------------     |\n"
             << "|     3.  --------------------------------------     |\n"
             << "|        |               Back                   |    |\n"
             << "|         --------------------------------------     |\n"
             << " ---------------------------------------------------- \n";
        cout << "Please enter your choice: " << endl;

        option = UserChoice();
        vector<string> filteredJobs;
        switch (option)
        {
            case VIEW_ALL_JOBS:
            {
                ViewAllJobs(db);
                string jobId = SelectJob(db, id, filteredJobs);
                if (jobId != "ERROR" && jobId != "b")
                    SubmitResume(db, id, jobId);
                break;
            }
            case SEARCH_BY_CATEGORY:
            {
                string choice = SearchByCategory(db, filteredJobs);
                if (choice != "b" && choice != "ERROR")
                {
                    string jobId = SelectJob(db, id, filteredJobs);
                    if (jobId != "b" && jobId != "ERROR")
                        SubmitResume(db, id, jobId);
                }
                break;
            }
            case BACK_TO_CANDIDATE_MENU:
                return;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}

char UserChoice()
{
    string optionInString;
    fflush(stdin);
    getline(cin, optionInString);
    if (optionInString.length() != 1)
        return '0';
    return optionInString[0];
}