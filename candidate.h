#ifndef MAIN_CPP_CANDIDATE_H
#define MAIN_CPP_CANDIDATE_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <iomanip>
#include <cstdlib>
#include <vector>
using namespace SQLite;

// Designing
void printLine(int width, char borderChar, char fillChar);
void printRow(string text, int width, char borderChar);

// Jobs
void PrintJobMinDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience);
void PrintJobMaxDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience, string& description, string& salary);
void ViewAllJobs(Database& db);
string SelectJob(Database& db, string& id, vector<string>& filteredJobs);
string SearchByCategory(Database& db, vector<string>& filteredJobs);
char SelectFilter();
char AnswerTheFilter(vector<string>& answerForFilter);
//Resumes
void CreateResume(Database& db, string& id);
int SelectDegree();
bool validateYearsOfExperience2(string & years_of_experience, const string& strMessage);

// Resume submissions
bool CheckIfOnlyLettersOrSpace(string& str);
bool strIsValid(string &str, const string& strMessage);
string EnterTillValid(string& str, const string& strMessage);
string EnterYearsOfExperienceTillValid(string& age, const string& strMessage);
void SubmitResume(Database& db, string& id, string& jobId);
void ViewAllSubmittedJobs(Database& db, string& candidate_id);

//Edit profile
void editName(Database&,  string&);
void editAge(Database&,  string&);
void editPassword(Database&,  string&);
void editFreeText(Database&,  string&);
void editQuestion(Database&,  string&);
void editProfile(Database&, string&);

void RejectAcceptInterviewInvitation(Database&db,string&id);
void ViewAllInterviewQuestions(Database&, string&);
#endif //MAIN_CPP_CANDIDATE_H