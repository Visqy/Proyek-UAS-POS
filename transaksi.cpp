#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "database.cpp"

using namespace std;

void tambahTransaksi(vector<Transaksi> &transaksiList, sqlite3 *db)
{
    char jawaban = 'y';

    while (jawaban == 'y' || jawaban == 'Y')
    {
        Transaksi transaksi;

        // Generate nomor transaksi secara acak
        srand(time(0));
        transaksi.nomorTransaksi = rand() % 1000 + 1;

        // Mendapatkan waktu saat ini
        time_t now = time(0);
        struct tm *timeInfo = localtime(&now);

        stringstream ss;
        ss << put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
        transaksi.waktuTransaksi = ss.str();

        cout << "Masukkan nama pengguna: ";
        cin >> transaksi.namaUser;

        int harga = 0;
        char tambahBarangJawaban = 'y';
        while (tambahBarangJawaban == 'y' || tambahBarangJawaban == 'Y')
        {
            Barang barang;
            cout << "Masukkan nama barang: ";
            cin >> barang.nama;
            cout << "Masukkan quantitas barang: ";
            cin >> barang.quantitas;
            cout << "Masukkan harga per barang: ";
            cin >> barang.hargaPerBarang;
            harga += barang.hargaPerBarang * barang.quantitas;
            transaksi.daftarBarangTransaksi.push_back(barang);

            cout << "Tambah barang lagi? (y/n): ";
            cin >> tambahBarangJawaban;
        }

        transaksi.hargaTransaksi = harga;
        // Memasukkan transaksi ke database
        if (insertTransaksi(db, transaksi))
        {
            transaksiList.push_back(transaksi);
            cout << "Transaksi berhasil ditambahkan!" << endl;
        }
        else
        {
            cout << "Gagal menambahkan transaksi." << endl;
        }

        cout << "Tambah transaksi lagi? (y/n): ";
        cin >> jawaban;
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

    tambahTransaksi(TransaksiList, db);

    sqlite3_close(db);
    return 0;
}