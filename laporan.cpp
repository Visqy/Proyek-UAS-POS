#include <iostream>
#include <vector>

using namespace std;

struct Barang
{
    string nama;
    int quantitas;
    int hargaPerBarang;
};

struct Transaksi
{
    int nomorTransaksi;
    string waktuTransaksi;
    vector<Barang> daftarBarangTransaksi;
    string namaUser;
};

vector<Transaksi> Transaksi;