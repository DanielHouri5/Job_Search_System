#ifndef MAIN_CPP_MENUS_H
#define MAIN_CPP_MENUS_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>

using namespace std;
using namespace SQLite;

void EditDeleteMenu();
void MainMenu(Database&);
void CandidateMenu(Database& , string&);
void EmployerMenu(Database& , string&);
void CandidateMenu(Database& , string&, string&);
void EmployerMenu(Database& , string&, string&);
void LookForJobsMenu(Database&, string&);
char UserChoice();
#endif //MAIN_CPP_MENUS_H