#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include <conio.h>
#include "database.cpp"
#include "login.cpp"
#include "supplier.cpp"
#include "laporan.cpp"
#include "transaksi.cpp"
#include "pengaturan.cpp"

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }

    // login screen
    string userSekarang;
    int typeUser;
    if (!menuUser(db, typeUser, userSekarang))
    {
        cout << "Terjadi Kesalahan pada menu login" << endl;
        return 1;
    }
    // main menu
    mainMenu(db, typeUser, userSekarang);
}