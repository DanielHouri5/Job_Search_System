//
// Created by hdani on 29/02/2024.
//
#include <SQLiteCpp/SQLiteCpp.h>
#include "employer.h"
#include "authentication.h"
#include "candidate.h"
#include "TableCreation.h"

// Validation

bool TestTableExists(Database& db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='tests';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}
bool UsersTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='users';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << std::endl;
    }
    return false;
}
bool ForgotPasswordTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='forgot_password';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << std::endl;
    }
    return false;
}
bool JobsListExists (Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='jobs_list';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}
bool InterviewInvitationsExist(Database&db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='Interview_invitations';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << std::endl;
    }
    return false;
}
bool ResumesTableExists(Database& db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='resumes';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}
bool ResumeSubmissionsTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='submission';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}

// Creation

void CreateJobsListTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS jobs_list ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                "employer_id INTEGER NOT NULL,"
                "company_name TEXT NOT NULL,"
                "location TEXT NOT NULL,"
                "position TEXT NOT NULL,"
                "description TEXT NOT NULL,"
                "scope TEXT NOT NULL,"
                "experience INTEGER NOT NULL,"
                "salary INTEGER NOT NULL,"
                "FOREIGN KEY (employer_id) REFERENCES users(id));");
        cout << "jobs_list table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateInterviewInvitationTable(Database& db)
{
    try {
        db.exec("CREATE TABLE IF NOT EXISTS Interview_invitations ("
                "employer_id TEXT NOT NULL,"
                "candidate_id TEXT NOT NULL,"
                "job_id TEXT NOT NULL,"
                "UNIQUE(candidate_id, job_id)"
                ");");
        cout << "Interview_invitations table created successfully.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateTestTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS tests ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "employer_id INTEGER NOT NULL,"
                "candidate_id INTEGER NOT NULL,"
                "question TEXT NOT NULL,"
                "answer1 TEXT NOT NULL,"
                "answer2 TEXT NOT NULL,"
                "answer3 TEXT NOT NULL,"
                "answer4 TEXT NOT NULL,"
                "correct_answer INTEGER NOT NULL,"
                "grade INTEGER,"
                "FOREIGN KEY(employer_id) REFERENCES users(id),"
                "FOREIGN KEY(candidate_id) REFERENCES users(id),"
                "UNIQUE(employer_id, candidate_id)"
                ");");
        cout << "tests table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateForgotPasswordTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS forgot_password ("
                "id INTEGER PRIMARY KEY,"
                "question TEXT NOT NULL,"
                "answer TEXT NOT NULL,"
                "FOREIGN KEY(id) REFERENCES users(id)"
                ");");
        cout << "Forgot password table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateUsersTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY UNIQUE,"
                "password TEXT NOT NULL,"
                "name TEXT NOT NULL,"
                "age INTEGER NOT NULL,"
                "freeText TEXT NOT NULL,"
                "role TEXT NOT NULL);");
        cout << "Users table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateResumeTable(Database& db)
{
    try {
        db.exec("CREATE TABLE IF NOT EXISTS resumes ("
                "candidate_id INTEGER PRIMARY KEY UNIQUE,"
                "full_name TEXT NOT NULL,"
                "age INTEGER NOT NULL,"
                "degree1 TEXT,"
                "degree2 TEXT,"
                "degree3 TEXT,"
                "work_experience TEXT NOT NULL,"
                "years_of_experience INTEGER NOT NULL);");
        cout << "resumes table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateResumeSubmissionsTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS submission ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "job_id INTEGER,"
                "candidate_id INTEGER NOT NULL,"
                "status TEXT DEFAULT 'pending',"
                "FOREIGN KEY(job_id) REFERENCES jobs_list(id),"
                "FOREIGN KEY(candidate_id) REFERENCES users(id));");
        cout << "submissions table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
//inserting

void InsertJobToDatabase(Database& db, string& id, string& name, string& location,string& position, string& description,string& scope,string& experience, string& salary) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO jobs_list (employer_id, company_name, location, description,position, scope,experience,salary) VALUES (?, ?, ?,?, ?,?,?,?);");

        // Bind values to the statement
        query.bind(1, stoi(id));
        query.bind(2, name);
        query.bind(3, location);
        query.bind(4, description);
        query.bind(5, position);
        query.bind(6, scope);
        query.bind(7, stoi(experience));
        query.bind(8, stoi(salary));

        // Execute the statement
        query.exec();
        cout <<"job successfully added to the database.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void insertDataToTestsTable(Database& db,string& employer_id,string& candidate_id,string& question,string& answer1,string& answer2,string& answer3,string& answer4,string& correct_answer)
{
    try {
        Statement query(db, "INSERT INTO tests (employer_id, candidate_id, question,answer1,answer2,answer3,answer4,correct_answer ) VALUES (?,?,?,?,?,?,?,?)");
        query.bind(1, stoi(employer_id));
        query.bind(2, stoi(candidate_id));
        query.bind(3, question);
        query.bind(4, answer1);
        query.bind(5, answer2);
        query.bind(6, answer3);
        query.bind(7, answer4);
        query.bind(8, correct_answer);
        query.exec();
        cout << "Question sent to the candidate successfully.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id,string&employee_id)
{
    if(!InterviewInvitationsExist(db)) {
        CreateInterviewInvitationTable(db);
    }
    try {
        Statement query(db, "INSERT INTO Interview_invitations (employer_id, candidate_id, job_id) VALUES (? ,?, ?)");
        query.bind(1, employee_id);
        query.bind(2, candidate_id);
        query.bind(3, job_id);
        query.exec();
        cout << "Interview invitation inserted successfully.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
bool InsertForgotPasswordDetailsToDatabase(Database& db, string& id, string& question ,string& answer) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO forgot_password (id, question ,answer) VALUES (?, ?, ?);");
        // Bind values to the statement
        query.bind(1, stoi(id)); // Assuming ID is a numeric value stored as text
        query.bind(2, question);
        query.bind(3, answer);

        // Execute the statement
        query.exec();
        cout << "Password recovery details saved.\n";
        return true;
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
bool InsertUserToDatabase(Database& db, string& id, string& password, string& name, string& age, string& role,string&freetext) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO users (id, password, name, age,freeText, role) VALUES (?, ?, ?,?, ?, ?);");

        // Bind values to the statement
        query.bind(1, stoi(id)); // Assuming ID is a numeric value stored as text
        query.bind(2, password);
        query.bind(3, name);
        query.bind(4, stoi(age)); // Convert age from string to integer
        query.bind(5, freetext);
        query.bind(6, role);

        // Execute the statement
        query.exec();
        cout << "User " << name << " successfully added to the database.\n";
        return true;
    } catch (const exception& e) {
        cout << "There is an account with this ID already!" << endl;
        return false;
    }
}
void InsertResumeToTable(Database& db, string& id, string& full_name, string& age, string& degree1, string& degree2, string& degree3, string& work_experience, string& years_of_experience)
{
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO resumes (candidate_id, full_name, age, degree1, degree2, degree3, work_experience, years_of_experience) VALUES (?, ?, ?, ?, ?, ?, ?, ?);");

        // Bind values to the statement
        query.bind(1, stoi(id));
        query.bind(2, full_name);
        query.bind(3, age);
        query.bind(4, degree1);
        query.bind(5, degree2);
        query.bind(6, degree3);
        query.bind(7, work_experience);
        query.bind(8, years_of_experience);

        // Execute the statement
        query.exec();
        cout <<"resume successfully added to the database.\n";
    } catch (const exception& e) {
        cout << "You already created a resume!" << endl;
    }
}
void InsertSubmitToTable(Database& db, string& candidate_id, string& jobId)
{
    if(!ResumeSubmissionsTableExists(db)) {
        CreateResumeSubmissionsTable(db);
    }
    try {
        // Prepare a SQL insert statement
        Statement query1(db, "SELECT * FROM jobs_list WHERE id = ?" );
        query1.bind(1, stoi(jobId));
        Statement query2(db, "SELECT * FROM resumes WHERE candidate_id = ?");
        query2.bind(1, stoi(candidate_id));
        Statement queryIfThereIsSubmission(db, "SELECT job_id, candidate_id FROM submission WHERE (job_id, candidate_id) = (?,?);");
        queryIfThereIsSubmission.bind(1, stoi(jobId));
        queryIfThereIsSubmission.bind(2, stoi(candidate_id));
        while (queryIfThereIsSubmission.executeStep())
        {
            string dbJobId = queryIfThereIsSubmission.getColumn(0).getText();
            string dbCandidate = queryIfThereIsSubmission.getColumn(1).getText();
            if (dbJobId == jobId && dbCandidate == candidate_id)
            {
                cout << "You already entered your resume for this job, go find other jobs." << endl;
                return;
            }
        }
        if (query2.executeStep())
        {
            if (query1.executeStep())
            {
                string employerId = query1.getColumn(1).getText();
                Statement query3(db, "INSERT INTO submission (job_id, candidate_id) VALUES (?,?);");

                // Bind values to the statement
                query3.bind(1, stoi(jobId));
                query3.bind(2, stoi(candidate_id));

                // Execute the statement
                query3.exec();
                cout <<"Submission successfully added to the database.\n";
            }
        }
        else
        {
            string choice;
            cout << "You didn't create a resume!" << endl;
            CreateResume(db, candidate_id);
        }

    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreatTables(Database& db)
{
    if(!InterviewInvitationsExist(db))
        CreateInterviewInvitationTable(db);
    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    if(!UsersTableExists(db))
        CreateUsersTable(db);
    if(!JobsListExists(db))
        CreateJobsListTable(db);
    if(!ResumeSubmissionsTableExists(db))
        CreateResumeSubmissionsTable(db);
    if(!ResumesTableExists(db))
        CreateResumeTable(db);
    if(!TestTableExists(db)) {
        CreateTestTable(db);
    }
}