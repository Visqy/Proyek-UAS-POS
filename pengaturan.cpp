#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;
// buat nambah user baru
void tambahUser(sqlite3 *db)
{
    vector<string> userColumns = {"USER", "PASSWORD", "TYPE"};
    User newUser;
    cout << "user baru" << endl;
    cout << "Masukkan user: ";
    cin >> newUser.id;
    cout << "Masukkan password: ";
    cin >> newUser.pass;
    cout << "Masukkan type: ";
    cin >> newUser.type;
    vector<string> userValues = {newUser.id, newUser.pass, to_string(newUser.type)};
    if (insertDB(db, "USER", userColumns, userValues))
    {
        users.push_back(newUser);
        cout << "Data user berhasil ditambahkan." << endl;
    }
    else
    {
        cout << "Gagal menambahkan data user." << endl;
    }
}
// buat nampil semua user
bool tampilUser(sqlite3 *db)
{
    if (!readUsers(db, "USER", "*"))
    {
        cout << "Gagal membaca tabel USER" << endl;
        return false;
    }
    else
    {
        cout << "-----------------------------------------------------------" << endl;
        cout << "No.   User            Password                  Type       " << endl;
        cout << "-----------------------------------------------------------" << endl;
        int i = 1;
        for (const auto &user : users)
        {
            cout << setiosflags(ios::left) << setw(6) << i;
            cout << setiosflags(ios::left) << setw(15) << user.id;
            cout << setiosflags(ios::left) << setw(23) << user.pass;
            cout << setiosflags(ios::left) << setw(7) << user.type;
            cout << endl;
            i++;
        }
        cout << "---------------------------------------------------------------" << endl;
        return true;
    }
}
// buat delete salah satu user
void deleteUser(sqlite3 *db)
{
    tampilUser(db);
    string user;
    cout << "pilih user : ";
    cin >> user;
    if (deleteDB(db, "USER", "USER", user))
    {
        cout << "Hapus data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat menghapus data." << endl;
    }
}
// update password salah satu user
void updatePassUser(sqlite3 *db)
{
    tampilUser(db);
    string user, pass;
    cout << "pilih user : ";
    cin >> user;
    cout << "ganti password :";
    cin >> pass;

    if (updateDB(db, "USER", "PASSWORD", pass, "USER", user))
    {
        cout << "Perbarui data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat memperbarui data." << endl;
    }
}

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }
    // code disini ya bang
    sqlite3_close(db);
    return 0;
}
