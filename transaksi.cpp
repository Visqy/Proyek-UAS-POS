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

void tambahTransaksi(vector<Transaksi>& transaksiList, sqlite3* db)
{
    char jawaban = 'y';

    while (jawaban == 'y' || jawaban == 'Y')
    {
        Transaksi transaksi;

        cout << "Masukkan nama pengguna: ";
        cin >> transaksi.namaUser;

        // Mendapatkan waktu saat ini
        time_t now = time(0);
        struct tm* timeInfo = localtime(&now);

        stringstream ss;
        ss << put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
        transaksi.waktuTransaksi = ss.str();

        cout << "Masukkan nomor transaksi : ";
        cin >> transaksi.nomorTransaksi;

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

void printReceipt(const vector<Transaksi>& transaksiList)
{
    for (const Transaksi& transaksi : transaksiList)
    {
        cout << "======================================" << endl;
        cout << "            STRUK BELANJA             " << endl;
        cout << "======================================" << endl;
        cout << "Nomor Transaksi: " << transaksi.nomorTransaksi << endl;
        cout << "Waktu Transaksi: " << transaksi.waktuTransaksi << endl;
        cout << "Nama Pengguna: " << transaksi.namaUser << endl;
        cout << "Daftar Barang:" << endl;
        cout << left << setw(5) << "No" << setw(15) << "NAMA BARANG" << setw(10) << "JUMLAH" << setw(15) << "HARGA PER ITEM" << setw(15) << "TOTAL HARGA" << endl;

        double total = 0.0;
        int count = 1;
        for (const Barang& barang : transaksi.daftarBarangTransaksi)
        {
            double itemTotal = barang.quantitas * barang.hargaPerBarang;
            total += itemTotal;

            cout << left << setw(5) << count << setw(15) << barang.nama << setw(10) << barang.quantitas << setw(15) << barang.hargaPerBarang << setw(15) << itemTotal << endl;
            count++;
        }

        cout << "======================================" << endl;
        cout << "TOTAL: " << total << endl;
        cout << "======================================" << endl;
    }
}

int main()
{
    sqlite3* db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }

    tambahTransaksi(TransaksiList, db);
    printReceipt(TransaksiList);

    sqlite3_close(db);

    return 0;
}
