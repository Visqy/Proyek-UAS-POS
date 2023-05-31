#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }
    // Menampilkan data dari database
    if (!readUsers(db, "*"))
    {
        cout << "Gagal membaca tabel USER" << endl;
        return 1;
    }

    string login, password;
    cout << "Masukkan login: ";
    cin >> login;
    cout << "Masukkan password: ";
    cin >> password;

    bool loginSuccess = false;
    int type;
    for (const auto &user : users)
    {
        if (user.id == login && user.pass == password)
        {
            loginSuccess = true;
            type = user.type;
            break;
        }
    }
    // pemilihan type user
    if (!loginSuccess)
    {
        cout << "die" << endl;
        return 0;
    }

    cout << type << endl;

    sqlite3_close(db);
    return 0;
}