#include <iostream>
#include <sqlite3.h>

int main(int argc, char const *argv[])
{
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);
    if (exit)
    {
        std::cerr << "Error opening database "<<sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    else{
        std::cout << "Opened database succesfully! " << std::endl;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS blockchain("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "student_id TEXT NOT NULL, "
        "block_hash TEXT NOT NULL, "
        "prev_hash TEXT NOT NULL, "
        "certificate_hash TEXT NOT NULL, "
        "timestamp INTEGER);";
    
    sqlite3_exec(DB, sql, nullptr, nullptr, nullptr);

    sqlite3_close(DB);
    return 0;
}
