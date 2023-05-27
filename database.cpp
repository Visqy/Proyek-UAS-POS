#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <vector>

using namespace std;

struct User
{
    string id;
    string pass;
    int type;
};

struct DaftarBarang
{
    int id;
    string nama;
    int qty;
    int harga;
};

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

vector<User> User;
vector<DaftarBarang> DaftarBarang;
vector<Transaksi> Transaksi;

static int table1Callback(void * /*data*/, int argc, char **argv, char ** /*azColName*/)
{
    User rowData;

    // Mendapatkan nilai kolom dari setiap baris
    rowData.id = argv[0];
    rowData.pass = argv[1];
    rowData.type = stoi(argv[2]);

    // Menambahkan data ke dalam vektor global User
    User.push_back(rowData);

    return 0;
}

static int table2Callback(void * /*data*/, int argc, char **argv, char ** /*azColName*/)
{
    DaftarBarang rowData;

    // Mendapatkan nilai kolom dari setiap baris
    rowData.id = stoi(argv[0]);
    rowData.nama = argv[1];
    rowData.qty = stoi(argv[2]);
    rowData.harga = stoi(argv[2]);

    // Menambahkan data ke dalam vektor global Barang
    Barang.push_back(rowData);

    return 0;
}

bool checkDB(sqlite3 *db)
{
    sqlite3 *db;
    int rc;
    char *sql;

    if (filesystem::exists("database.db"))
    {
        cout << "Database sudah dibuat" << endl;
        return (true);
    }
    try
    {
        rc = sqlite3_open("database.db", &db);
        if (rc)
        {
            throw runtime_error("Tidak dapat membuka database: " + string(sqlite3_errmsg(db)));
        }
        sql = "CREATE TABLE USER ("
              "USER           TEXT PRIMARY KEY     NOT NULL,"
              "PASSWORD       TEXT    NOT NULL,"
              "TYPE           INT     NOT NULL);"
              "CREATE TABLE BARANG ("
              "KODE INT PRIMARY KEY   NOT NULL,"
              "NAMA           TEXT    NOT NULL,"
              "QTY            INT     NOT NULL,"
              "HARGA          INT     NOT NULL);"
              "CREATE TABLE TRANSAKSI ("
              "KODE INT PRIMARY KEY   NOT NULL,"
              "WAKTU        TEXT    NOT NULL,"
              "TRANSAKSI      TEXT    NOT NULL,"
              "HARGA          INT     NOT NULL,"
              "USER           INT     NOT NULL);";
        rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)
        {
            throw runtime_error("Kesalahan dalam membuat tabel: " + string(sqlite3_errmsg(db)));
        }
    }
    catch (const exception &e)
    {
        cout << "Terjadi kesalahan: " << e.what() << endl;
        return (false);
    }

    cout << "Database berhasil dibuat" << endl;
    return (true);
}

bool insertDB(sqlite3 *db, const string &tableName, const vector<string> &columns, const vector<string> &values)
{
    if (columns.size() != values.size())
    {
        cout << "Jumlah kolom dan nilai tidak cocok." << endl;
        return false;
    }

    string query = "INSERT INTO " + tableName + " (";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        query += columns[i];
        if (i != columns.size() - 1)
        {
            query += ", ";
        }
    }
    query += ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i)
    {
        query += "'" + values[i] + "'";
        if (i != values.size() - 1)
        {
            query += ", ";
        }
    }
    query += ")";

    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

bool updateDB(sqlite3 *db, const string &tableName, const string &columnName, const string &columnValue, const string &conditionColumn, const string &conditionValue)
{
    string query = "UPDATE " + tableName + " SET " + columnName + "='" + columnValue + "' WHERE " + conditionColumn + "='" + conditionValue + "'";

    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

bool readDB(sqlite3 *db, const string &tableName, void (*callback)(void *, int, char **, char **))
{
    string query = "SELECT * FROM " + tableName;

    int rc = sqlite3_exec(db, query.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

int main()
{
    sqlite3 *db;
    bool x = checkDB(db);
    if (x)
    {
        cout << "next" << endl;
    }
    else
    {
        cout << "gagal" << endl;
    }
    sqlite3_close(db);
}