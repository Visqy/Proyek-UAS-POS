#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;

bool tampilTransaksi(sqlite3 *db)
{
    if (!readTransaksi(db))
    {
        cout << "Gagal membaca tabel transaksi" << endl;
        return false;
    }
    else
    {
        cout << "-----------------------------------------------------------------------------------------" << endl;
        cout << "Kode Transaksi        Waktu Transaksi            Harga Total            User            " << endl;
        cout << "-----------------------------------------------------------------------------------------" << endl;
        for (const auto &transaksi : TransaksiList)
        {
            cout << setiosflags(ios::left) << setw(15) << transaksi.nomorTransaksi;
            cout << setiosflags(ios::left) << setw(23) << transaksi.waktuTransaksi;
            cout << setiosflags(ios::left) << setw(20) << transaksi.hargaTransaksi;
            cout << setiosflags(ios::left) << setw(20) << transaksi.namaUser;
            cout << endl;
        }
        cout << "---------------------------------------------------------------" << endl;
        return true;
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
    tampilTransaksi(db);
    sqlite3_close(db);
    return 0;
}