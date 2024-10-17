/*
NAME AND ID OF STUDENTS:
Daniel Houri 209445071
Meryl Hassid 324569714
Daniel Boguslavsky 207915729
Liron Sasonker 207354366
Yehuda Baza 208029819
David Moalem 203387337
*/
#include <iostream>
#include "SQLiteCpp/SQLiteCpp.h"
#include "menus.h"
#include "sqlite3.h"
#include "TableCreation.h"
#include <stdio.h>

using namespace std;
using namespace SQLite;

void printGoodbye() {
    cout << " -------------------------------------------- \n";
    cout << "|                                            |\n";
    cout << "|         Thank you for your time.           |\n";
    cout << "|       Goodbye and have a wonderful day!    |\n";
    cout << "|                                            |\n";
    cout << " -------------------------------------------- \n";
}
int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    CreatTables(db);
    MainMenu(db);
    printGoodbye();
    return 0;
}