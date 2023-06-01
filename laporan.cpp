#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
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
        cout << "----------------------------------------------------------------------------------------" << endl;
        return true;
    }
}

void printReceipt(const Transaksi &transaksi)
{
    cout << "======================================" << endl;
    cout << "            STRUK BELANJA             " << endl;
    cout << "======================================" << endl;
    cout << "Nomor Transaksi: " << transaksi.nomorTransaksi << endl;
    cout << "Waktu Transaksi: " << transaksi.waktuTransaksi << endl;
    cout << "Nama Pengguna: " << transaksi.namaUser << endl;
    cout << "Daftar Barang:" << endl;
    cout << "-------------------------------------------------------------------------------"<<endl;
    cout << left << setw(5) << "No" << setw(15) << "NAMA BARANG" << setw(10) << "JUMLAH" << setw(15) << "HARGA PER ITEM" << setw(15) << "TOTAL HARGA" << endl;
    cout << "-------------------------------------------------------------------------------"<<endl;

    double total = 0.0;
    int count = 1;
    for (const Barang &barang : transaksi.daftarBarangTransaksi)
    {
        double itemTotal = barang.quantitas * barang.hargaPerBarang;
        total += itemTotal;

        cout << left << setw(5) << count << setw(15) << barang.nama << setw(10) << barang.quantitas << setw(15) << barang.hargaPerBarang << setw(15) << itemTotal << endl;
        count++;
    }

    cout << "==============================================================================" << endl;
    cout << "TOTAL: " << total << endl;
    cout << "==============================================================================" << endl;
}

void printStruk(sqlite3 *db)
{
    cout << "Transaksi Belanja" << endl;
    cout << "================" << endl;

    int kodeStruk;
    do
    {
        cout << "Masukkan kode struk (0 untuk kembali): ";
        cin >> kodeStruk;

        if (kodeStruk == 0)
        {
            break;
        }

        if (!readTransaksi(db))
        {
            cout << "Gagal membaca tabel transaksi" << endl;
        }
        else
        {
            bool ditemukan = false;
            for (const auto &transaksi : TransaksiList)
            {
                if (transaksi.nomorTransaksi == kodeStruk)
                {
                    printReceipt(transaksi);
                    ditemukan = true;
                    break;
                }
            }

            if (!ditemukan)
            {
                cout << "Transaksi dengan kode struk " << kodeStruk << " tidak ditemukan." << endl;
            }
        }
    } while (true);
}

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }

    int menu;
    do
    {
        cout << "========================================" << endl;
        cout << "         CETAK LAPORAN BELANJA" << endl;
        cout << "========================================" << endl;
        cout << "1. Cetak keseluruhan" << endl;
        cout << "2. Cetak Struk" << endl;
        cout << "0. Keluar" << endl;
        cout << "Masukkan menu: ";
        cin >> menu;

        switch (menu)
        {
        case 1:
            system("CLS");
            tampilTransaksi(db);
            system("PAUSE");
            system("CLS");
            break;
        case 2:
            system("CLS");
            tampilTransaksi(db);
            printStruk(db);
            system("PAUSE");
            system("CLS");
            break;
        case 0:
            system("CLS");
            break;
        default:
            cout << "Angka yang dimasukan salah" << endl;
            break;
        }
    } while (menu != 0);

    sqlite3_close(db);
    return 0;
}
