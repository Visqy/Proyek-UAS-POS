#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <sqlite3.h>

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
        cout << "===================================================================================================" << endl;
        centerText("RIWAYAT TRANSAKSI");
        cout << "===================================================================================================" << endl;
        cout << "Kode Transaksi        Waktu Transaksi            Harga Total            User            " << endl;
        cout << "===================================================================================================" << endl;
        for (const auto &transaksi : TransaksiList)
        {
            cout << setiosflags(ios::left) << setw(22) << transaksi.nomorTransaksi;
            cout << setiosflags(ios::left) << setw(27) << transaksi.waktuTransaksi;
            cout << setiosflags(ios::left) << setw(23) << transaksi.hargaTransaksi;
            cout << setiosflags(ios::left) << setw(20) << transaksi.namaUser;
            cout << endl;
        }
        cout << "===================================================================================================" << endl;
        return true;
    }
}

void printReceipt(sqlite3 *db, const Transaksi &transaksi)
{
    InformasiToko infoToko = readInformasi(db);
    cout << "===================================================================================================" << endl;
    centerText("STRUK TRANSAKSI");
    centerText(infoToko.nama);
    centerText("Alamat: " + infoToko.alamat);
    centerText("Telp: " + infoToko.telp);
    cout << "===================================================================================================" << endl;
    cout << "Nomor Transaksi: " << transaksi.nomorTransaksi << endl;
    cout << "Waktu Transaksi: " << transaksi.waktuTransaksi << endl;
    cout << "Nama Kasir: " << transaksi.namaUser << endl;
    cout << "Daftar Barang:" << endl;
    cout << "---------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(5) << "No" << setw(15) << "NAMA BARANG" << setw(10) << "JUMLAH" << setw(20) << "HARGA PER ITEM" << setw(20) << "TOTAL HARGA" << endl;
    cout << "---------------------------------------------------------------------------------------------------" << endl;

    double total = 0.0;
    int count = 1;
    for (const Barang &barang : transaksi.daftarBarangTransaksi)
    {
        double itemTotal = barang.quantitas * barang.hargaPerBarang;
        total += itemTotal;

        cout << left << setw(5) << count << setw(15) << barang.nama << setw(10) << barang.quantitas << setw(20) << barang.hargaPerBarang << setw(15) << itemTotal << endl;
        count++;
    }

    cout << "===================================================================================================" << endl;
    cout << "TOTAL: " << total << endl;
    cout << "===================================================================================================" << endl;
}

void printStruk(sqlite3 *db)
{
    int kodeStruk;
    do
    {
        tampilTransaksi(db);
        cout << endl;
        cout << "==============================================================================" << endl;
        cout << "RIWAYAT STRUK TRANSAKSI" << endl;
        cout << "==============================================================================" << endl;
        cout << "Masukkan kode struk (0 untuk kembali): ";
        cin >> kodeStruk;
        FixCin();

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
                    system("cls");
                    printReceipt(db, transaksi);
                    ditemukan = true;
                    system("pause");
                    system("CLS");
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

void menuLaporan(sqlite3 *db, int &type, string &userSekarang)
{
    int menu;
    bool menuState = true;
    do
    {
        headerProgram(db, type, userSekarang);
        cout << "======================================================" << endl;
        cout << "MENU LAPORAN TRANSAKSI" << endl;
        cout << "======================================================" << endl;
        cout << "1. Cetak keseluruhan" << endl;
        cout << "2. Cetak Struk" << endl;
        cout << "3. Kembali" << endl;
        cout << "4. Log Out" << endl;
        cout << "5. Keluar Program" << endl;
        cout << "======================================================" << endl;
        cout << "Masukkan menu: ";
        cin >> menu;
        FixCin();

        switch (menu)
        {
        case 1:
            system("CLS");
            headerProgram(db, type, userSekarang);
            tampilTransaksi(db);
            system("PAUSE");
            system("CLS");
            break;
        case 2:
            system("CLS");
            printStruk(db);
            system("PAUSE");
            system("CLS");
            break;
        case 3:
            menuState = false;
            system("CLS");
            break;
        case 4:
            menuState = false;
            system("CLS");
            menuUser(db, type, userSekarang);
            break;
        case 5:
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
}
