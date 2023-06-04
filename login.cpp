#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>

using namespace std;

int main();
bool menuTransaksi(sqlite3 *db, int &type, string &userSekarang);
bool menuBarang(sqlite3 *db, int &type, string &userSekarang);
void menuLaporan(sqlite3 *db, int &type, string &userSekarang);
void menuPengaturan(sqlite3 *db, int &type, string &userSekarang);
void FixCin();

void centerText(string text)
{
    int lebar = 100;
    int spasiKiri = (lebar - text.length()) / 2;
    int spasiKanan = lebar - text.length() - spasiKiri;
    cout << setw(spasiKiri) << left << "" << text << setw(spasiKanan) << "" << endl;
}

void headerUser(sqlite3 *db)
{
    InformasiToko infoToko = readInformasi(db);
    cout << "===================================================================================================" << endl;
    cout << endl;
    centerText("APLIKASI POINT OF SALE");
    centerText(infoToko.nama);
    cout << endl;
    cout << "===================================================================================================" << endl;
    cout << endl;
}

void headerProgram(sqlite3 *db, int &type, string userSekarang)
{
    InformasiToko infoToko = readInformasi(db);
    cout << "===================================================================================================" << endl;
    cout << endl;
    centerText("APLIKASI POINT OF SALE");
    centerText(infoToko.nama);
    cout << endl;
    string typeUser = (type == 1) ? "Admin" : (type == 2) ? "Kasir"
                                          : (type == 3)   ? "Supplier"
                                                          : "";
    centerText("Login sebagai " + userSekarang + " (" + typeUser + ")");
    cout << endl;
    cout << "===================================================================================================" << endl;
    cout << endl;
    cout << endl;
}

void loadingScreen()
{
    system("cls");
    cout << "\e[?25l";
    cout << "\n\n\n\n\n\n";
    cout << "\t\t\t\t______     _       _            __   _____       _      " << endl;
    cout << "\t\t\t\t| ___ \\   (_)     | |          / _| /  ___|     | |     " << endl;
    cout << "\t\t\t\t| |_/ /__  _ _ __ | |_    ___ | |_  \\ `--.  __ _| | ___ " << endl;
    cout << "\t\t\t\t|  __/ _ \\| | '_ \\| __|  / _ \\|  _|  `--. \\/ _` | |/ _ \\" << endl;
    cout << "\t\t\t\t| | | (_) | | | | | |_  | (_) | |   /\\__/ / (_| | |  __/" << endl;
    cout << "\t\t\t\t\\_|  \\___/|_|_| |_|\\__|  \\___/|_|   \\____/ \\__,_|_|\\___|" << endl;
    cout << "\n\n\n\t\t\t\t   ";

    for (int i = 0; i < 50; i++)
    {
        cout << (char)219;
        Sleep(50);
    }
    cout << "\n\n\n\n\n\n";
    cout << "\033[?25h";
    system("cls");
}

string GetPassword()
{
    string password;
    char c;

    while ((c = _getch()) != '\r') // Tombol Enter
    {
        if (c == '\b') // Tombol Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b"; // Menghapus karakter dari layar
            }
        }
        else
        {
            password += c;
            cout << '*'; // Menampilkan karakter bintang (*) sebagai gantinya
        }
    }

    cout << endl; // Baris baru setelah selesai membaca password

    return password;
}

bool menuUser(sqlite3 *db, int &type, string &userSekarang)
{
    // Menampilkan data dari database
    if (!readUsers(db, "*"))
    {
        cout << "Gagal membaca tabel USER" << endl;
        return false;
    }

    string login, password;
    bool loginSuccess = false;
    system("CLS");
    do
    {
        headerUser(db);
        cout << "Masukkan User       : ";
        getline(cin, login);
        cout << "Masukkan password   : ";
        password = GetPassword();
        for (const auto &user : users)
        {
            if (user.id == login && user.pass == password)
            {
                loginSuccess = true;
                type = user.type;
                userSekarang = user.id;
                break;
            }
        }
        if (!loginSuccess)
        {
            cout << "\nMaaf user atau password yang anda masukan tidak sesuai,\nSilakan coba kembali" << endl;
            system("PAUSE");
            system("CLS");
        }
        else
        {
            string typeUser;
            switch (type)
            {
            case 1:
                typeUser = "Admin";
                break;

            case 2:
                typeUser = "Kasir";
                break;

            case 3:
                typeUser = "Supplier";
                break;

            default:
                cout << "Invalid user type" << endl;
                loginSuccess = false;
            }
            cout << "\nHallo, Selamat Datang " << userSekarang << endl;
            cout << "Masuk sebagai: " << typeUser << endl;
            cout << endl;
            system("PAUSE");
            system("CLS");
        }
    } while (!loginSuccess);
    users.clear();
    return true;
}

void mainMenu(sqlite3 *db, int &type, string &userSekarang)
{
    int pilihanMenu;
    bool menuState = true;
    bool subMenuState = true;

    switch (type)
    {
    // login sebagai admin
    case 1:
        do
        {
            headerProgram(db, type, userSekarang);
            cout << "======================================================" << endl;
            cout << "MAIN MENU" << endl;
            cout << "======================================================" << endl;
            cout << "1. Menu Transaksi" << endl;
            cout << "2. Menu Supplier" << endl;
            cout << "3. Menu Laporan" << endl;
            cout << "4. Menu Pengaturan" << endl;
            cout << "5. Log Out" << endl;
            cout << "6. Keluar Program" << endl;
            cout << "======================================================" << endl;
            cout << " Pilih Menu: ";
            cin >> pilihanMenu;
            FixCin();
            switch (pilihanMenu)
            {
            case 1:
                system("CLS");
                menuTransaksi(db, type, userSekarang);
                break;
            case 2:
                system("CLS");
                menuBarang(db, type, userSekarang);
                break;
            case 3:
                system("CLS");
                menuLaporan(db, type, userSekarang);
                break;
            case 4:
                system("CLS");
                menuPengaturan(db, type, userSekarang);
                break;
            case 5:
                menuState = false;
                system("CLS");
                break;
            case 6:
                menuState = false;
                system("CLS");
                cout << "Terimakasih sudah menggunakan aplikasi ini." << endl;
                system("PAUSE");
                exit(0);
                break;
            default:
                cout << "Pilihan menu tidak tersedia" << endl;
                system("PAUSE");
                system("CLS");
                break;
            }
        } while (menuState);
        main();
        break;
    // login sebagai kasir
    case 2:
        do
        {
            subMenuState = menuTransaksi(db, type, userSekarang);
        } while (subMenuState);
        main();
        break;

    // login sebagai supplier
    case 3:
        do
        {
            subMenuState = menuBarang(db, type, userSekarang);
        } while (subMenuState);
        main();
        break;
    default:
        break;
    }
}
