#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;

// buat nambah barang baru
void tambahBarang(sqlite3 *db)
{
    vector<string> barangColumns = {"KODEBARANG", "NAMA_BARANG", "QTY", "HARGA"};
    DaftarBarang newBarang;
    cout << "barang baru" << endl;
    cout << "Masukkan kode barang: ";
    cin >> newBarang.id;
    cout << "Masukkan nama barang: ";
    cin >> newBarang.nama;
    cout << "Masukkan stok barang: ";
    cin >> newBarang.qty;
    cout << "Masukkan harga barang: ";
    cin >> newBarang.harga;
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
// buat nampil semua barang
bool tampilBarang(sqlite3 *db)
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
            cout << setiosflags(ios::left) << setw(15) << barang.id;
            cout << setiosflags(ios::left) << setw(23) << barang.nama;
            cout << setiosflags(ios::left) << setw(20) << barang.qty;
            cout << setiosflags(ios::left) << setw(20) << barang.harga;
            cout << endl;
        }
        cout << "---------------------------------------------------------------" << endl;
        return true;
    }
}
// buat delete salah satu barang
void deleteBarang(sqlite3 *db)
{
    tampilBarang(db);
    string barang;
    cout << "pilih kode barang : ";
    cin >> barang;
    if (deleteDB(db, "DAFTARBARANG", "KODEBARANG", barang))
    {
        cout << "Hapus data berhasil." << endl;
    }
    else
    {
        cout << "Terjadi kesalahan saat menghapus data." << endl;
    }
}
// update ... salah satu barang
void updateBarang(sqlite3 *db)
{
    tampilBarang(db);
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
}

int main()
{
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }
    // code disini ya bang
    sqlite3_close(db);
    return 0;
}