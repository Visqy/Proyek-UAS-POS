#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>

using namespace std;
// buat nambah user baru
void tambahUser(sqlite3 *db)
{
    bool go = false;
    vector<string> userColumns = {"USER", "PASSWORD", "TYPE"};
    User newUser;
    cout << "user baru" << endl;
    cout << "Masukkan user: ";
    FixCin();
    getline(cin, newUser.id);
    cout << "Masukkan password: ";
    cin >> newUser.pass;
    do
    {
        cout << "Masukkan type: ";
        cin >> newUser.type;
        if (newUser.type > 3 || newUser.type < 1)
        {
            cout << "Type tidak tersedia" << endl;
            go = true;
        }
        else
        {
            go = false;
        }

    } while (go);

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
    system("pause");
    system("cls");
}
// buat nampil semua user
bool tampilUser(sqlite3 *db)
{
    if (!readUsers(db, "*"))
    {
        cout << "Gagal membaca tabel USER" << endl;
        return false;
    }
    else
    {
        cout << "----------------------------------------------------------------------" << endl;
        cout << "No.   User                          Password               Type       " << endl;
        cout << "----------------------------------------------------------------------" << endl;
        int i = 1;
        for (const auto &user : users)
        {
            cout << setiosflags(ios::left) << setw(6) << i;
            cout << setiosflags(ios::left) << setw(30) << user.id;
            cout << setiosflags(ios::left) << setw(23) << user.pass;
            cout << setiosflags(ios::left) << setw(7) << user.type;
            cout << endl;
            i++;
        }
        cout << "----------------------------------------------------------------------" << endl;
        return true;
    }
    system("pause");
    system("cls");
}
// buat delete salah satu user
void deleteUser(sqlite3 *db)
{
    tampilUser(db);
    string user;
    cout << "pilih user : ";
    FixCin();
    getline(cin, user);
    if (deleteDB(db, "USER", "USER", user))
    {
        cout << "Hapus data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat menghapus data." << endl;
    }
    system("pause");
    system("cls");
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
    system("pause");
    system("cls");
}

void manageUser(sqlite3 *db, int &type, string &userSekarang)
{
    int pil;
    do
    {
        headerProgram(db, type, userSekarang);
        cout << "Menu Management User" << endl;
        cout << "1.     Tambah User            " << endl;
        cout << "2.     Delete User            " << endl;
        cout << "3.     Update Password User   " << endl;
        cout << "4.     Tampil User            " << endl;
        cout << "5.     Kembali Menu Pengaturan" << endl;
        cout << "Pilih Menu : ";
        cin >> pil;
        switch (pil)
        {
        case 1:
            system("cls");
            headerProgram(db, type, userSekarang);
            tambahUser(db);
            break;

        case 2:
            system("cls");
            headerProgram(db, type, userSekarang);
            deleteUser(db);
            break;

        case 3:
            system("cls");
            headerProgram(db, type, userSekarang);
            updatePassUser(db);
            break;

        case 4:
            system("cls");
            headerProgram(db, type, userSekarang);
            tampilUser(db);
            break;

        case 5:
            break;

        default:
            cout << "Menu tidak tersedia" << endl;
            system("cls");
        }
    } while (pil != 5);
    system("cls");
}

void manageHeader(sqlite3 *db, int &type, string &userSekarang)
{
    int opsi;
    do
    {
        InformasiToko informasi = readInformasi(db);

        headerProgram(db, type, userSekarang);
        cout << "=== Informasi Toko ===" << endl;
        cout << "Nama: " << informasi.nama << endl;
        cout << "Alamat: " << informasi.alamat << endl;
        cout << "Telepon: " << informasi.telp << endl;

        cout << endl;
        cout << "Opsi:" << endl;
        cout << "1. Ubah Informasi Toko" << endl;
        cout << "2. Keluar" << endl;

        cout << "Pilih opsi: ";
        cin >> opsi;

        switch (opsi)
        {
        case 1:
        {
            system("cls");
            headerProgram(db, type, userSekarang);
            InformasiToko newInformasi;
            cout << "Masukkan informasi toko baru:" << endl;
            cout << "Nama: ";
            cin.ignore();
            getline(cin, newInformasi.nama);
            cout << "Alamat: ";
            getline(cin, newInformasi.alamat);
            cout << "Telepon: ";
            getline(cin, newInformasi.telp);

            updateInformasi(db, newInformasi);
            system("cls");
            break;
        }
        case 2:
            system("cls");
            return;
        default:
            cout << "Opsi tidak valid. Silakan coba lagi." << endl;
            system("cls");
            break;
        }
    } while (opsi != 2);
    return;
}

void menuPengaturan(sqlite3 *db, int &type, string &userSekarang)
{
    int menu;
    bool menuState = true;
    do
    {
        headerProgram(db, type, userSekarang);
        cout << "======================================================" << endl;
        cout << "MENU PENGATURAN" << endl;
        cout << "======================================================" << endl;
        cout << "1. Pengaturan Header" << endl;
        cout << "2. Pengaturan Management User" << endl;
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
            manageHeader(db, type, userSekarang);
            system("CLS");
            break;
        case 2:
            system("CLS");
            manageUser(db, type, userSekarang);
            break;
        case 3:
            menuState = false;
            system("CLS");
            break;
        case 4:
            menuState = false;
            system("CLS");
            main();
            break;
        case 5:
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
}
