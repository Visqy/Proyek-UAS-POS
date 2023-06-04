#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

void tambahTransaksi(sqlite3 *db, int &type, string &userSekarang)
{
    Transaksi transaksi;
    transaksi.namaUser = userSekarang;
    // Mendapatkan waktu saat ini
    time_t now = time(0);
    struct tm *timeInfo = localtime(&now);

    stringstream ss;
    ss << put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    transaksi.waktuTransaksi = ss.str();

    cout << "Masukkan nomor transaksi : ";
    cin >> transaksi.nomorTransaksi;
    FixCin();

    int harga = 0;
    char tambahBarangJawaban = 'y';
    bool cekbarang = false;
    while (tambahBarangJawaban == 'y' || tambahBarangJawaban == 'Y')
    {
        Barang barang;
        cout << "Daftar barang" << endl;
        tampilBarang(db, type, userSekarang);
        cekbarang = false;

        do
        {
            cout << "Masukkan kode barang: ";
            cin >> barang.kode;
            FixCin();
            if (!isBarangExists(db, barang.kode))
            {
            cout << "Barang dengan kode " << barang.kode << " tidak ada di daftar barang!" << endl;
            cout << "Ulangi Transaksi?(y/n): ";
            cin >> tambahBarangJawaban;

                if (tambahBarangJawaban != 'y' && tambahBarangJawaban != 'Y')
                {
                    cekbarang = true;
                }
            }
            else
            {
                cekbarang = true;
            }
        }
        while (!cekbarang);

        if(!cekbarang)
        {
            continue;
        }

        barang.nama = readBarangTransaksi(db, barang.kode, "NAMA_BARANG");
        barang.hargaPerBarang = stoi(readBarangTransaksi(db, barang.kode, "HARGA"));
        cout << "Barang yang dipilih: " << barang.nama << endl;
        cout << "Harga dari barang yang dipilih: " << barang.hargaPerBarang << endl;
        cout << "Masukkan quantitas barang: ";
        cin >> barang.quantitas;
        FixCin();
        updateStokBarang(db, barang.quantitas, barang.kode);
        harga += barang.hargaPerBarang * barang.quantitas;
        transaksi.daftarBarangTransaksi.push_back(barang);

        cout << "Tambah barang lagi? (y/n): ";
        cin >> tambahBarangJawaban;
}
    transaksi.hargaTransaksi = harga;
    // Memasukkan transaksi ke database
    if (insertTransaksi(db, transaksi))
    {
        TransaksiList.push_back(transaksi);
        cout << "Transaksi berhasil ditambahkan!" << endl;
    }
    else
    {
        cout << "Gagal menambahkan transaksi." << endl;
    }
    system("PAUSE");
    system("CLS");
    printReceipt(db, transaksi);
}

bool menuTransaksi(sqlite3 *db, int &type, string &userSekarang)
{
    int menu;
    bool menuState = true;
    do
    {
        headerProgram(db, type, userSekarang);
        cout << "======================================================" << endl;
        cout << "MENU TRANSAKSI" << endl;
        cout << "======================================================" << endl;
        cout << "1. Tambah Transaksi" << endl;
        cout << "2. Kembali" << endl;
        cout << "3. Log Out" << endl;
        cout << "4. Keluar Program" << endl;
        cout << "======================================================" << endl;
        cout << "Masukkan menu: ";
        cin >> menu;
        FixCin();

        switch (menu)
        {
        case 1:
            system("CLS");
            headerProgram(db, type, userSekarang);
            tambahTransaksi(db, type, userSekarang);
            system("PAUSE");
            system("CLS");
            break;
        case 2:
            menuState = false;
            system("CLS");
            break;
        case 3:
            menuState = false;
            system("CLS");
            return false;
            break;
        case 4:
            menuState = false;
            system("CLS");
            cout << "Terimakasih sudah menggunakan aplikasi ini." << endl;
            system("PAUSE");
            exit(0);
        default:
            cout << "Angka yang dimasukan salah" << endl;
            break;
        }
    } while (menuState);
    return true;
}
