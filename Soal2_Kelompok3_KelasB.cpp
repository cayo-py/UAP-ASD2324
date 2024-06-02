#include <iostream>
using namespace std;

struct Student {
    string nama;
    string nim;
    int nilai[5]; float avgNilai;
    float presensi;
};

/* hintung rata-rata nilai untuk keperluan sorting */
float average(int arr[], int n) {
    int sum = 0;
    for (int i=0; i<n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

/* methot sorting */
void bubbleSort(Student arr[], int n, string sortBy, string sortOrder) {
    for (int i = 1; i < n-1; i++) {
        for (int j = n-1; j >= i; j--) { 
            bool tukar = false;
            
            if (sortBy == "nama") {
                if (sortOrder == "ascending") {
                    if (arr[j-1].nama > arr[j].nama) {
                        tukar = true;
                    }
                }
                else {
                    if (arr[j-1].nama < arr[j].nama) {
                        tukar = true;
                    }
                }
            }
            else if (sortBy == "NIM" || sortBy == "nim") {
                if (sortOrder == "ascending") {
                    if (arr[j-1].nim > arr[j].nim) {
                        tukar = true;
                    }
                }
                else {
                    if (arr[j-1].nim < arr[j].nim) {
                        tukar = true;
                    }
                }
            }
            else if (sortBy == "nilai" || sortBy == "nilai rata-rata") {
                if (sortOrder == "ascending") {
                    if (arr[j-1].avgNilai > arr[j].avgNilai) {
                        tukar = true;
                    }
                } 
                else {
                    if (arr[j-1].avgNilai < arr[j].avgNilai) {
                        tukar = true;
                    }
                }
            }
            else if (sortBy == "presensi") {
                if (sortOrder == "ascending") {
                    if (arr[j-1].presensi > arr[j].presensi) {
                        tukar = true;
                    }
                }
                else {
                    if (arr[j-1].presensi < arr[j].presensi) {
                        tukar = true;
                    }
                }
            }
            else {
                cout << "Invalid input" << endl;
                return;
            }
            
            if (tukar) {
                swap(arr[j-1], arr[j]);
            }
        }
    }
}

int main() {
    const int MAX_STUDENTS = 40;
    Student students[MAX_STUDENTS];
    int n;
    char menu;

    /* Data default */
    n = 3;
    students[0].nama = "Cahyo Prasetyo";    students[1].nama = "Baru Wira";         students[2].nama = "Margaretha";
    students[0].nim = "235150300111019";    students[1].nim = "235150301111021";    students[2].nim = "235150307111017";
    students[0].nilai[0] = 90;              students[1].nilai[0] = 70;              students[2].nilai[0] = 90;
    students[0].nilai[1] = 85;              students[1].nilai[1] = 75;              students[2].nilai[1] = 85;
    students[0].nilai[2] = 90;              students[1].nilai[2] = 85;              students[2].nilai[2] = 80;
    students[0].nilai[3] = 85;              students[1].nilai[3] = 85;              students[2].nilai[3] = 75;
    students[0].nilai[4] = 95;              students[1].nilai[4] = 90;              students[2].nilai[4] = 80;
    students[0].presensi = 85.0;            students[1].presensi = 80.0;            students[2].presensi = 75.0;
    
    /* Jika mau input manual, hilangkan comment */
/*
    cout << "Masukkan jumlah mahasiswa: "; cin >> n;
    
    for (int i = 3; i<n; i++) {
        cout << "Data mahasiswa ke-" << i+1 << endl;
        
        cout << "Nama: ";
        getline (cin >> ws, students[i].nama);
        
        cout << "NIM: ";
        cin >> students[i].nim;
        
        cout << "Nilai (5 integer, pisahkan dengan spasi): ";
        for (int j = 0; j < 5; j++) {
            cin >> students[i].nilai[j];
        }
        
        cout << "Persentase presensi (float tanpa '%'): ";
        cin >> students[i].presensi;
    }
*/

    // Menghitung rata-rata nilai
    for (int i=0; i<n; i++) {
        students[i].avgNilai = average(students[i].nilai, 5);
    }
    
    string sortBy, sortOrder;

    Menu:
    cout << "Pilih menu: (0 untuk keluar, 9 untuk clear output)\n"
            << "1. Sorting data\n"
            << "2. Tampilkan List Mahasiswa\n"
            << "3. Tampilkan List Mahasiswa dengan Presensi di atas 80%\n"
            << "Pilih menu: "; cin >> menu;

    switch (menu) {
        case '1':
            cout << "\nSort by: nama / NIM / rata-rata nilai / presensi" << endl;
            cout << "Pilih menu sort by: ";
            getline (cin >> ws, sortBy);
            
            cout << "Sorting order: ascending / descending" << endl;
            cout << "Pilih menu sorting order: ";
            cin >> sortOrder;
            
            bubbleSort(students, n, sortBy, sortOrder);
            
            cout << "\n-- Hasil pengurutan --" << endl;
            for (int i = 0; i < n; i++) {
                cout << "Nama: " << students[i].nama << endl;
                cout << "NIM: " << students[i].nim << endl;
                cout << "Nilai: ";
                for (int j = 0; j < 5; j++) {
                    cout << students[i].nilai[j] << " ";
                }
                cout << "(rata-rata: " << students[i].avgNilai << ")";
                cout << endl;
                cout << "Presensi: " << students[i].presensi << "%" << endl;
                cout << endl;
            }
            goto Menu;

        case '2':
            cout << "\n-- List Mahasiswa --" << endl;
            for (int i = 0; i < n; i++) {
                cout << "Nama: " << students[i].nama << endl;
                cout << "NIM: " << students[i].nim << endl;
                cout << "Nilai: ";
                for (int j = 0; j < 5; j++) {
                    cout << students[i].nilai[j] << " ";
                }
                cout << " (rata-rata: " << students[i].avgNilai << ")";
                cout << endl;
                cout << "Presensi: " << students[i].presensi << "%" << endl;
                cout << endl;
            }
            goto Menu;

        case '3':
            cout << "\n-- List Mahasiswa dengan Presensi di atas 80% --" << endl;
            for (int i = 0; i < n; i++) {
                if (students[i].presensi >= 80) {
                    cout << "Nama: " << students[i].nama << endl;
                    cout << "NIM: " << students[i].nim << endl;
                    cout << "Nilai: ";
                    for (int j = 0; j < 5; j++) {
                        cout << students[i].nilai[j] << " ";
                    }
                    cout << " (rata-rata: " << students[i].avgNilai << ")";
                    cout << endl;
                    cout << "Presensi: " << students[i].presensi << "%" << endl;
                    cout << endl;
                }
            }
            goto Menu;

        case '9':
            system("clear");
            goto Menu;

        case '0':
            break;

        default:
            cout << "Invalid input" << endl;
            goto Menu;
    }
      
    return 0;
}