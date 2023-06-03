#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>

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
    int kode;
    string nama;
    int quantitas;
    int hargaPerBarang;
};

struct Transaksi
{
    int nomorTransaksi;
    string waktuTransaksi;
    int hargaTransaksi;
    vector<Barang> daftarBarangTransaksi;
    string namaUser;
};

struct InformasiToko
{
    string nama;
    string alamat;
    string telp;
};

vector<Transaksi> TransaksiList;
vector<DaftarBarang> DaftarBarangs;
vector<User> users;

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

static int selectQtyCallback(void *data, int argc, char **argv, char **colNames)
{
    int *qty = static_cast<int *>(data);
    if (argc > 0 && argv[0])
        *qty = stoi(argv[0]);
    return SQLITE_OK;
}

static int callbackTransaksi(void *data, int argc, char **argv, char **)
{
    Transaksi rowData;

    rowData.nomorTransaksi = stoi(argv[0]);
    rowData.waktuTransaksi = argv[1];
    rowData.hargaTransaksi = stoi(argv[2]);
    rowData.namaUser = argv[3];

    // Memasukkan barang ke dalam daftar barang transaksi
    Barang barang;
    barang.kode = stoi(argv[4]);
    barang.nama = argv[5];
    barang.quantitas = stoi(argv[6]);
    barang.hargaPerBarang = stoi(argv[7]);
    rowData.daftarBarangTransaksi.push_back(barang);

    // Mencari transaksi dengan nomor transaksi yang sama dan menggabungkan daftar barangnya
    for (auto &transaksi : TransaksiList)
    {
        if (transaksi.nomorTransaksi == rowData.nomorTransaksi)
        {
            transaksi.hargaTransaksi += rowData.hargaTransaksi;
            transaksi.daftarBarangTransaksi.push_back(barang);
            return 0;
        }
    }

    // Jika tidak ada transaksi dengan nomor transaksi yang sama, tambahkan transaksi baru ke daftar transaksi
    TransaksiList.push_back(rowData);

    return 0;
}

static int callbackUsers(void *, int argc, char **argv, char **)
{
    User rowData;

    rowData.id = argv[0];
    rowData.pass = argv[1];
    rowData.type = stoi(argv[2]);

    users.push_back(rowData);
    return 0;
}

static int callbackDaftarBarangs(void *, int argc, char **argv, char **)
{
    DaftarBarang rowData;

    rowData.id = stoi(argv[0]);
    rowData.nama = argv[1];
    rowData.qty = stoi(argv[2]);
    rowData.harga = stoi(argv[3]);

    DaftarBarangs.push_back(rowData);

    return 0;
}

bool checkDB(sqlite3 *db)
{
    int rc;
    char *errMsg;

    // Memeriksa apakah file database sudah ada
    if (filesystem::exists("database.db"))
    {
        return true;
    }
    else
    {

        // Membuat file database baru dan tabel
        rc = sqlite3_open("database.db", &db);
        if (rc)
        {
            cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        // Membuat tabel USER, BARANG, dan TRANSAKSI jika belum ada
        string createTableQuery = "CREATE TABLE IF NOT EXISTS USER ("
                                  "USER           TEXT PRIMARY KEY     NOT NULL,"
                                  "PASSWORD       TEXT    NOT NULL,"
                                  "TYPE           INT     NOT NULL);"
                                  "CREATE TABLE IF NOT EXISTS DAFTARBARANG ("
                                  "KODEBARANG INT PRIMARY KEY   NOT NULL,"
                                  "NAMA_BARANG         TEXT    NOT NULL,"
                                  "QTY            INT     NOT NULL,"
                                  "HARGA          INT     NOT NULL);"
                                  "CREATE TABLE IF NOT EXISTS TRANSAKSI ("
                                  "TRANSAKSI_ID INT PRIMARY KEY   NOT NULL,"
                                  "WAKTU        TEXT    NOT NULL,"
                                  "HARGA          INT     NOT NULL,"
                                  "USER           TEXT     NOT NULL);"
                                  "CREATE TABLE BARANG ("
                                  "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "TRANSAKSI_ID INTEGER, "
                                  "KODEBARANG INTEGER, "
                                  "NAMABARANG TEXT, "
                                  "QTY INTEGER, "
                                  "HARGA INTEGER, "
                                  "FOREIGN KEY (TRANSAKSI_ID) REFERENCES Transaksi(TRANSAKSI_ID));"
                                  "CREATE TABLE IF NOT EXISTS INFORMASI ("
                                  "NAMATOKO          TEXT     NOT NULL,"
                                  "ALAMATTOKO         TEXT     NOT NULL,"
                                  "TELPTOKO          TEXT     NOT NULL);";

        rc = sqlite3_exec(db, createTableQuery.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK)
        {
            cout << "Kesalahan dalam membuat tabel: " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
}

bool insertDB(sqlite3 *db, const string &tableName, const vector<string> &columns, const vector<string> &values)
{
    if (columns.size() != values.size())
    {
        cout << "Jumlah kolom dan nilai tidak cocok." << endl;
        return false;
    }

    int rc;

    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    string insertQuery = "INSERT INTO " + tableName + " (";
    for (size_t i = 0; i < columns.size(); i++)
    {
        insertQuery += columns[i];
        if (i != columns.size() - 1)
            insertQuery += ",";
    }
    insertQuery += ") VALUES (";

    for (size_t i = 0; i < values.size(); i++)
    {
        insertQuery += "'" + values[i] + "'";
        if (i != values.size() - 1)
            insertQuery += ",";
    }
    insertQuery += ");";

    char *errMsg;
    rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        cout << "Gagal memasukkan data: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool readUsers(sqlite3 *db, const string &columns)
{
    int rc;

    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    users.clear();
    string selectQuery = "SELECT " + columns + " FROM USER;";

    char *errMsg;
    rc = sqlite3_exec(db, selectQuery.c_str(), callbackUsers, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        cout << "Gagal membaca data: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool readDaftarBarangs(sqlite3 *db, const string &columns)
{
    int rc;

    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    else
    {
        DaftarBarangs.clear();
        string selectQuery = "SELECT " + columns + " FROM DAFTARBARANG;";

        char *errMsg;
        rc = sqlite3_exec(db, selectQuery.c_str(), callbackDaftarBarangs, nullptr, &errMsg);
        if (rc != SQLITE_OK)
        {
            cout << "Gagal membaca data: " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }

        return true;
    }
}

bool deleteDB(sqlite3 *db, const string &tableName, const string &conditionColumn, const string &conditionValue)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    string deleteStatement = "DELETE FROM " + tableName + " WHERE " + conditionColumn + " = '" + conditionValue + "'";
    cout << deleteStatement << endl;
    rc = sqlite3_exec(db, deleteStatement.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan DELETE: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    int rowsAffected = sqlite3_changes(db);
    if (rowsAffected == 0)
    {
        cout << "Data dengan kondisi yang diberikan tidak ditemukan." << endl;
        return false;
    }
    return true;
}

bool updateDB(sqlite3 *db, const string &tableName, const string &columnName, const string &columnValue, const string &conditionColumn, const string &conditionValue)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    string updateStatement = "UPDATE " + tableName + " SET " + columnName + "='" + columnValue + "' WHERE " + conditionColumn + "='" + conditionValue + "'";
    rc = sqlite3_exec(db, updateStatement.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan UPDATE: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    int rowsAffected = sqlite3_changes(db);
    if (rowsAffected == 0)
    {
        cout << "Data dengan kondisi yang diberikan tidak ditemukan." << endl;
        return false;
    }
    return true;
}

bool insertTransaksi(sqlite3 *db, Transaksi &transaksi)
{
    int rc;
    // Membuat file database baru dan tabel
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    // Memasukkan data transaksi ke dalam tabel "Transaksi"
    string insertTransaksiQuery = "INSERT INTO Transaksi (TRANSAKSI_ID, WAKTU, HARGA, USER) VALUES ('" +
                                  to_string(transaksi.nomorTransaksi) + "', '" +
                                  transaksi.waktuTransaksi + "', '" +
                                  to_string(transaksi.hargaTransaksi) + "', '" +
                                  transaksi.namaUser + "');";

    rc = sqlite3_exec(db, insertTransaksiQuery.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    // Memasukkan data barang ke dalam tabel "Barang"
    for (const auto &barang : transaksi.daftarBarangTransaksi)
    {
        string insertBarangQuery = "INSERT INTO Barang (TRANSAKSI_ID, KODEBARANG, NAMABARANG, QTY, HARGA) VALUES ('" +
                                   to_string(transaksi.nomorTransaksi) + "', '" +
                                   to_string(barang.kode) + "', '" +
                                   barang.nama + "', " +
                                   to_string(barang.quantitas) + ", " +
                                   to_string(barang.hargaPerBarang) + ");";

        rc = sqlite3_exec(db, insertBarangQuery.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            cout << "Kesalahan dalam mengeksekusi pernyataan: " << sqlite3_errmsg(db) << endl;
            return false;
        }
    }

    return true;
}

bool readTransaksi(sqlite3 *db)
{
    int rc;
    // Membuat file database baru dan tabel
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    else
    {
        TransaksiList.clear();

        string query = "SELECT TRANSAKSI.TRANSAKSI_ID, TRANSAKSI.WAKTU, TRANSAKSI.HARGA, TRANSAKSI.USER, BARANG.KODEBARANG, BARANG.NAMABARANG, BARANG.QTY, BARANG.HARGA "
                       "FROM TRANSAKSI "
                       "INNER JOIN BARANG ON TRANSAKSI.TRANSAKSI_ID = BARANG.TRANSAKSI_ID;";

        char *errMsg;
        int rc;

        rc = sqlite3_exec(db, query.c_str(), callbackTransaksi, nullptr, &errMsg);
        if (rc != SQLITE_OK)
        {
            cout << "Kesalahan dalam membaca transaksi: " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
}

bool updateStokBarang(sqlite3 *db, int &qty, int &kodeBarang)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    string selectQtyQuery = "SELECT QTY FROM DAFTARBARANG WHERE KODEBARANG='" + to_string(kodeBarang) + "'";
    int currentQty = 0;
    rc = sqlite3_exec(db, selectQtyQuery.c_str(), selectQtyCallback, &currentQty, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan SELECT: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    int newQty = currentQty - qty;
    string updateQtyStatement = "UPDATE DAFTARBARANG SET QTY='" + to_string(newQty) + "' WHERE KODEBARANG='" + to_string(kodeBarang) + "'";
    rc = sqlite3_exec(db, updateQtyStatement.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan UPDATE DAFTARBARANG: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    return true;
}

bool isBarangExists(sqlite3 *db, const int &kodeBarang)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    string query = "SELECT COUNT(*) FROM DAFTARBARANG WHERE KODEBARANG = '" + to_string(kodeBarang) + "'";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan SELECT: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

string getNamaBarang(sqlite3 *db, const int &kodeBarang)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    string query = "SELECT NAMA_BARANG FROM DAFTARBARANG WHERE KODEBARANG = '" + to_string(kodeBarang) + "'";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan SELECT: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    string namaBarang = "";
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *result = sqlite3_column_text(stmt, 0);
        namaBarang = reinterpret_cast<const char *>(result);
    }

    sqlite3_finalize(stmt);
    return namaBarang;
}

string readBarangTransaksi(sqlite3 *db, const int &kodeBarang, const string &colomn)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    string query = "SELECT " + colomn + " FROM DAFTARBARANG WHERE KODEBARANG = '" + to_string(kodeBarang) + "'";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cout << "Kesalahan dalam mengeksekusi pernyataan SELECT: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    string isiBarang = "";
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *result = sqlite3_column_text(stmt, 0);
        isiBarang = reinterpret_cast<const char *>(result);
    }

    sqlite3_finalize(stmt);
    return isiBarang;
}

void insertInformasi(sqlite3 *db, const InformasiToko &informasi)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    string query = "INSERT INTO INFORMASI (NAMATOKO, ALAMATTOKO, TELPTOKO) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Tidak dapat mempersiapkan statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, informasi.nama.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, informasi.alamat.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, informasi.telp.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        cerr << "Tidak dapat mengeksekusi statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    cout << "Informasi toko berhasil ditambahkan ke database." << endl;
}

InformasiToko readInformasi(sqlite3 *db)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return InformasiToko();
    }
    string query = "SELECT NAMATOKO, ALAMATTOKO, TELPTOKO FROM INFORMASI;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Tidak dapat mempersiapkan statement: " << sqlite3_errmsg(db) << endl;
        return InformasiToko();
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        cerr << "Tidak dapat mengeksekusi statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return InformasiToko();
    }

    InformasiToko informasi;
    informasi.nama = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    informasi.alamat = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    informasi.telp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

    sqlite3_finalize(stmt);

    return informasi;
}

void updateInformasi(sqlite3 *db, const InformasiToko &informasi)
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        cout << "Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    string query = "UPDATE INFORMASI SET NAMATOKO = ?, ALAMATTOKO = ?, TELPTOKO = ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Tidak dapat mempersiapkan statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, informasi.nama.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, informasi.alamat.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, informasi.telp.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        cerr << "Tidak dapat mengeksekusi statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    cout << "Informasi toko berhasil diperbarui." << endl;
}