#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "database.cpp"

using namespace std;

void MasukkanDataBarang(sqlite3 *db);
void UpdateData(sqlite3 *db);
void HapusData(sqlite3 *db);
bool CariData(sqlite3 *db);
bool tampilBarang(sqlite3 *db);
bool tampilSemuaBarang(sqlite3 *db);

main(){
    sqlite3 *db;
    if (!checkDB(db))
    {
        cout << "Gagal memeriksa database" << endl;
        return 1;
    }
    char menu;
    cout<<"======================================================"<<endl;
    cout<<" =============== Data Supplier Barang ================"<<endl;
    cout<<"======================================================"<<endl;
    cout<<" a].Masukkan data barang                "<<endl;
    cout<<" b].Cari Data                           "<<endl;
    cout<<" c].Data barang gudang keseluruhan      "<<endl;
    cout<<" d].Hapus data gudang                   "<<endl;
    cout<<" e].Edit data                           "<<endl;
    cout<<" x].close                               "<<endl;
    cout<<"======================================================"<<endl;
    cout<<"======================================================"<<endl;
    cout<<" PILIH MENU:";
    cin>>menu;
    system("cls");
    switch(menu){
    case 'a':
    MasukkanDataBarang(db);
    break;
    case 'b':
    CariData(db);
    break;
    case 'c':
    tampilSemuaBarang(db);
    break;
    case 'd':
    HapusData(db);
    break;
    case 'e':
    UpdateData(db);
    break;
    default:
    cout<<endl;
    cout<<endl;
    cout<<" Terima kasih telah menjaga ketertiban dalam pengelolaan data gudang "<<endl;
    cout<<endl;
    cout<<endl;
    system("pause");
    break;
        }
        sqlite3_close(db);
    return 0;
    }
void MasukkanDataBarang(sqlite3 *db){
    awal:
    system("cls");
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
    system("pause");
    string t;
    cout<<">> ingin menambah data lagi ? "<<endl;
    cout<<">>(yes/no)";
    cin>>t;
    system("cls");
    if(t=="yes" || t=="Yes"){
    goto awal;
    }
    else{
    main();
    }
}

bool tampilBarang(sqlite3 *db) {
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

bool tampilSemuaBarang(sqlite3 *db){
    tampilBarang(db);
    system("pause");
    string t;
    cout<<">> ingin kembali ke menu ? "<<endl;
    cout<<">>(yes/no)";
    cin>>t;
    system("cls");
    if(t=="yes" || t=="Yes"){
    main();
    }
    else{
    system("cls");
    }
    return true;
}
void UpdateData (sqlite3 *db){
    awal:
    system("cls");
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
    string t;
    cout<<">> ingin mengup-date data lagi ? "<<endl;
    cout<<">>(yes/no)";
    cin>>t;
    system("CLS");
    if(t=="yes" || t=="Yes"){
    goto awal;
    }
    else{
    main();
    }
}
bool CariData (sqlite3 *db){
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
    int found=0, lokasi;
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
    if(found == 0){
    cout<<"Code Barang: "<<key<<" >>DATA BARANG BELUM DIKETAHUI!<<"<<endl;
    }
    cout<<endl;
    system("pause");
    string t;
    cout<<">> ingin mencari data barang lagi ? "<<endl;
    cout<<">>(yes/no)";
    cin>>t;
    system("cls");
    if(t=="yes" || t=="Yes"){
    goto awal;
    }
    else{
    main();
            }
        }
    return true;
    }
void HapusData(sqlite3 *db){
    int CodeDicari;
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
    system("pause");
    string t;
    cout<<">> ingin kembali ke menu ? "<<endl;
    cout<<">>(yes/no)";
    cin>>t;
    system("cls");
    if(t=="yes" || t=="Yes"){
    main();
    }
    else{
    system("cls");
    }
}
