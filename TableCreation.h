//
// Created by hdani on 29/02/2024.
//

#ifndef MAIN_CPP_TABLECREATION_H
#define MAIN_CPP_TABLECREATION_H

#include "employer.h"
#include "authentication.h"
#include "candidate.h"

using namespace std;
//Exist

bool TestTableExists(Database& db);
bool UsersTableExists(Database& db);
bool ForgotPasswordTableExists(Database& db);
bool JobsListExists (Database& db);
bool InterviewInvitationsExist(Database&db);
bool ResumesTableExists(Database& db);
bool ResumeSubmissionsTableExists(Database& db);
//Must add another table

// Creation
void CreateJobsListTable(Database& db);
void CreateInterviewInvitationTable(Database& db);
void CreateTestTable(Database& db);
void CreateForgotPasswordTable(Database& db);
void CreateUsersTable(Database& db);
void CreateResumeTable(Database& db);
void CreateResumeSubmissionsTable(Database& db);
void CreatTables(Database& db);

// Insert
void InsertJobToDatabase(Database& db, string& id, string& name, string& location,string& position, string& description,string& scope,string& experience, string& salary);
void insertDataToTestsTable(Database& db,string& employer_id,string& candidate_id,string& question,string& answer1,string& answer2,string& answer3,string& answer4,string& correct_answer);
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id,string&employee_id);
bool InsertForgotPasswordDetailsToDatabase(Database& db, string& id, string& question ,string& answer);
bool InsertUserToDatabase(Database& db, string& id, string& password, string& name, string& age, string& role,string&freetext);
void InsertResumeToTable(Database& db, string& id, string& full_name, string& age, string& degree1, string& degree2, string& degree3, string& work_experience, string& years_of_experience);
void InsertSubmitToTable(Database& db, string& candidate_id, string& jobId);

#endif //MAIN_CPP_TABLECREATION_H