#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

void MasukkanDataBarang(sqlite3 *db, int &type, string &userSekarang)
{
awal:
    system("cls");
    {
        vector<string> barangColumns = {"KODEBARANG", "NAMA_BARANG", "QTY", "HARGA"};
        DaftarBarang newBarang;
        cout << "barang baru" << endl;
        cout << "Masukkan kode barang: ";
        cin >> newBarang.id;
        FixCin();
        cout << "Masukkan nama barang: ";
        getline(cin, newBarang.nama);
        cout << "Masukkan stok barang: ";
        cin >> newBarang.qty;
        FixCin();
        cout << "Masukkan harga barang: ";
        cin >> newBarang.harga;
        FixCin();
        vector<string> barangValues = {to_string(newBarang.id), newBarang.nama, to_string(newBarang.qty), to_string(newBarang.harga)};
        if (insertDB(db, "DAFTARBARANG", barangColumns, barangValues))
        {
            DaftarBarangs.push_back(newBarang);
            cout << "Data barang berhasil ditambahkan." << endl;
        }
        else
        {
            cout << "Gagal menambahkan data barang." << endl;
        }
    }
    system("pause");
    string t;
    cout << ">> ingin menambah data lagi ? " << endl;
    cout << ">>(yes/no)";
    cin >> t;
    system("cls");
    if (t == "yes" || t == "Yes")
    {
        goto awal;
    }
    else
    {
        menuBarang(db, type, userSekarang);
    }
}

bool tampilBarang(sqlite3 *db, int &type, string &userSekarang)
{
    if (!readDaftarBarangs(db, "*"))
    {
        cout << "Gagal membaca tabel barang" << endl;
        return false;
    }
    else
    {
        cout << "-----------------------------------------------------------------------------------------" << endl;
        cout << "Kode Barang        Nama Barang            Stok Barang            Harga Barang            " << endl;
        cout << "-----------------------------------------------------------------------------------------" << endl;
        for (const auto &barang : DaftarBarangs)
        {
            cout << setiosflags(ios::left) << setw(20) << barang.id;
            cout << setiosflags(ios::left) << setw(25) << barang.nama;
            cout << setiosflags(ios::left) << setw(22) << barang.qty;
            cout << setiosflags(ios::left) << setw(22) << barang.harga;
            cout << endl;
        }
        cout << "-----------------------------------------------------------------------------------------" << endl;
    }
    return true;
}

bool tampilSemuaBarang(sqlite3 *db, int &type, string &userSekarang)
{
    tampilBarang(db, type, userSekarang);
    system("pause");
    system("cls");
    return true;
}
void UpdateData(sqlite3 *db, int &type, string &userSekarang)
{
awal:
    system("cls");
    tampilBarang(db, type, userSekarang);
    string barang, qty;
    cout << "pilih kode barang : ";
    cin >> barang;
    cout << "tambah stok :";
    cin >> qty;

    if (updateDB(db, "DAFTARBARANG", "QTY", qty, "KODEBARANG", barang))
    {
        cout << "Perbarui data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat memperbarui data." << endl;
    }
    string t;
    cout << ">> ingin mengup-date data lagi ? " << endl;
    cout << ">>(yes/no)";
    cin >> t;
    system("CLS");
    if (t == "yes" || t == "Yes")
    {
        goto awal;
    }
    else
    {
        menuBarang(db, type, userSekarang);
    }
}
bool CariData(sqlite3 *db, int &type, string &userSekarang)
{
    if (!readDaftarBarangs(db, "*"))
    {
        cout << "Gagal membaca tabel barang" << endl;
        return false;
    }
    else
    {
    awal:
        int key;
        cout << "Code barang : ";
        cin >> key;
        FixCin();
        int found = 0, lokasi;
        for (const auto &barang : DaftarBarangs)
        {
            if (barang.id == key)
            {
                found += 1;
                if (found == 1)
                {
                    cout << "-----------------------------------------------------------------------------------------" << endl;
                    cout << "Kode Barang        Nama Barang            Stok Barang            Harga Barang            " << endl;
                    cout << "-----------------------------------------------------------------------------------------" << endl;
                    cout << setiosflags(ios::left) << setw(20) << barang.id;
                    cout << setiosflags(ios::left) << setw(25) << barang.nama;
                    cout << setiosflags(ios::left) << setw(22) << barang.qty;
                    cout << setiosflags(ios::left) << setw(22) << barang.harga;
                    cout << endl;
                    cout << "-----------------------------------------------------------------------------------------" << endl;
                }
            }
        }
        if (found == 0)
        {
            cout << "Code Barang: " << key << " >>DATA BARANG BELUM DIKETAHUI!<<" << endl;
        }
        cout << endl;
        system("pause");
        string t;
        cout << ">> ingin mencari data barang lagi ? " << endl;
        cout << ">>(yes/no)";
        cin >> t;
        system("cls");
        if (t == "yes" || t == "Yes")
        {
            goto awal;
        }
        else
        {
            menuBarang(db, type, userSekarang);
        }
    }
    return true;
}
void HapusData(sqlite3 *db, int &type, string &userSekarang)
{
awal:
    int CodeDicari;
    tampilBarang(db, type, userSekarang);
    string barang;
    cout << "pilih kode barang : ";
    cin >> barang;
    FixCin();
    if (deleteDB(db, "DAFTARBARANG", "KODEBARANG", barang))
    {
        cout << "Hapus data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat menghapus data." << endl;
    }
    system("pause");
    string t;
    cout << ">> ingin kembali ke menu ? " << endl;
    cout << ">>(yes/no)";
    cin >> t;
    system("cls");
    if (t == "yes" || t == "Yes")
    {
        menuBarang(db, type, userSekarang);
    }
    else
    {
        system("cls");
        goto awal;
    }
}

bool menuBarang(sqlite3 *db, int &type, string &userSekarang)
{
    int menu;
    bool menuState = true;
    do
    {
        headerProgram(db, type, userSekarang);
        cout << "======================================================" << endl;
        cout << "MENU SUPPLIER BARANG" << endl;
        cout << "======================================================" << endl;
        cout << "1. Masukkan data barang                " << endl;
        cout << "2. Cari Data                           " << endl;
        cout << "3. Data barang keseluruhan      " << endl;
        cout << "4. Hapus data                   " << endl;
        cout << "5. Edit data                           " << endl;
        cout << "6. Kembali" << endl;
        cout << "7. Log Out" << endl;
        cout << "8. Keluar Program" << endl;
        cout << "======================================================" << endl;
        cout << "Masukkan menu: ";
        cin >> menu;
        FixCin();

        switch (menu)
        {
        case 1:
            system("CLS");
            headerProgram(db, type, userSekarang);
            MasukkanDataBarang(db, type, userSekarang);
            break;
        case 2:
            system("CLS");
            headerProgram(db, type, userSekarang);
            CariData(db, type, userSekarang);
            break;
        case 3:
            system("CLS");
            headerProgram(db, type, userSekarang);
            tampilSemuaBarang(db, type, userSekarang);
            break;
        case 4:
            system("CLS");
            headerProgram(db, type, userSekarang);
            HapusData(db, type, userSekarang);
            break;
        case 5:
            system("CLS");
            headerProgram(db, type, userSekarang);
            UpdateData(db, type, userSekarang);
            break;
        case 6:
            menuState = false;
            system("CLS");
            if (type == 1)
            {
                return false;
            }
            else
            {
                return true;
            }
            break;
        case 7:
            menuState = false;
            system("CLS");
            main();
            break;
        case 8:
            menuState = false;
            system("CLS");
            cout << "Terimakasih sudah menggunakan aplikasi ini." << endl;
            system("PAUSE");
            exit(0);
        default:
            cout << "Angka yang dimasukan salah" << endl;
            system("PAUSE");
            system("CLS");
            break;
        }
    } while (menuState);
    return false;
}
