#include <iostream>
#include <vector> //untuk array dinamis
#include <string>
#include <filesystem> //untuk penggunaan memeriksa file ada atau tidak
#include <sqlite3.h>  //untuk penggunaan database sqlite3
#include <conio.h>    //untuk penggunaan getch()
#include <chrono>     //untuk mengubah format waktu
#include <ctime>      //untuk mendapatkan waktu sekarang
#include <iomanip>    //untuk penggunaan setw()
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