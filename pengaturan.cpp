#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;
// buat nambah user baru
void FixCin()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(256, '\n');
    }
    else
    {
        cin.ignore();
    }
};
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
    if(newUser.type >3 || newUser.type <1) {
        cout << "Type tidak tersedia" << endl;
        go = true;
    } else {
        go = false;
    }

    }while(go);

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
void manageUser(sqlite3 *db);
void menuPengaturan(sqlite3 *db);

void manageUser(sqlite3 *db)
{
    int pil;
    do{
    cout << "Menu Management User" << endl;
    cout << "1.     Tambah User            " << endl;
    cout << "2.     Delete User            " << endl;
    cout << "3.     Update Password User   " << endl;
    cout << "4.     Tampil User            " << endl;
    cout << "5.     Kembali Menu Pengaturan" << endl;
    cout << "Pilih Menu : ";
    cin >> pil;
    switch(pil)
    {
        case 1 :
        system("cls");
        tambahUser(db);
        break;

        case 2:
        system("cls");
        deleteUser(db);
        break;

        case 3:
        system("cls");
        updatePassUser(db);
        break;

        case 4:
        system("cls");
        tampilUser(db);
        break;

        case 5:
        system("cls");
        break;

        default:
        cout << "Menu tidak tersedia" << endl;
    }
    }while(pil != 5);
    menuPengaturan(db);
    system("pause");
    system("cls");
}

void menuPengaturan(sqlite3 *db)
{
    int pil;
    do{
    cout << "Menu Pengaturan" << endl;
    cout << "1. Pengaturan Header " << endl;
    cout << "2. Pengaturan Management User" << endl;
    cout << "3. Kemabali Ke Menu Utama " << endl;
    cout << "Pilih Akses : ";
    cin >> pil;
    switch(pil)
    {
        case 1:
            //header
            break;

        case 2:
            system("cls");
            manageUser(db);
            break;

        case 3:
            exit(0);
        default:
        cout << "Pilihan Menu Tidak Tersedia" << endl;

    }
    }while(pil != 3);
}

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }
    menuPengaturan(db);
    sqlite3_close(db);
    return 0;
}
