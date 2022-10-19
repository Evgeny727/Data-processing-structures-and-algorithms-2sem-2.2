#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct student {
    int numZachBook;
    int numGroup;
    char FIO[100];
};

// создание бинарного фала из текстового
int CreateBinFile(string nametf, string namebf, int numg) {
    ifstream tf(nametf);
    if (tf) {
        fstream bf(namebf, ios::out | ios::binary);
        student s;
        while (!tf.eof()) {
            tf >> s.numZachBook;
            tf.get();//прочитать до пробела(вместе с пробелом)
            tf >> s.numGroup;
            tf.get();//прочитать до пробела(вместе с пробелом)
            tf.getline(s.FIO, 100, '\n');
            if (s.numGroup == numg)
                bf.write((char*)&s, sizeof(student));
        }
        tf.close();
        bf.close();
        return 0;
    }
    return 1;
}

//вывод бинарного файла на монитор
void outBinFile(string namebf) {
    fstream bf(namebf, ios::in | ios::binary);
    cout << "Содержание бинарного файла: \n";
    student s;
    bf.read((char*)&s, sizeof(student));
    while (!bf.eof()) {
        cout << s.numZachBook << " " << s.numGroup << " " << s.FIO << endl;
        bf.read((char*)&s, sizeof(student));
    }
    bf.close();
}

//функция ищет запись с заданным ключом и при успешном поиске возвращает порядковый номер в записи в файле
int searchKeyBinFile(string namebf, int key) {
    ifstream bf;
    bf.open(namebf, ios::binary);
    if (!bf) return -2; //не открылся
    student s;
    int numstudent = 0;
    bf.read((char*)&s, sizeof(student));
    while (!bf.eof()) {
        if (s.numZachBook == key) {
            bf.close();
            return numstudent;
        }
        numstudent++;
        bf.read((char*)&s, sizeof(student));
    }
    bf.close();
    return -1;
}

//удаление записи с заданным значением ключа
void deleteKeyBinFile(string namebf, int key) {
    ifstream bf(namebf, ios::binary);
    bf.seekg(0, ios::end);
    int size = bf.tellg();
    size = size / sizeof(student);
    student* Students_1 = new student[size];
    bf.seekg(0, ios::beg);
    bf.read((char*)Students_1, sizeof(student) * size);
    bf.close();
    ofstream obf(namebf, ios::binary);
    for (int i = 0; i < size; i++)
    {
        if (i != key)
            obf.write((char*)&Students_1[i], sizeof(student));
    }
    obf.close();
}

//перевод студентов
void perevodStudents(string nametf, string namebf, int newGroup, int numg) {
    ifstream pstf("perevodStudents.txt");
    ifstream tf(nametf);
    ofstream bf(namebf, ios::binary);
    vector<int> vec;
    int temp;
    while (!pstf.eof()) {
        pstf >> temp;
        vec.push_back(temp);
    }
    pstf.close();
    student s;
    while (!tf.eof()) {
        tf >> s.numZachBook;
        tf.get();
        tf >> s.numGroup;
        tf.get();
        tf.getline(s.FIO, 100, '\n');
        for (int i = 0; i < vec.size(); i++) {
            if ((s.numZachBook == vec[i])) {
                s.numGroup = newGroup;
            }
        }
        if (s.numGroup == numg)
            bf.write((char*)&s, sizeof(student));
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    cout << "Введите номер группы: ";
    int numg;
    cin >> numg;
    if (CreateBinFile("programTextFile.txt", "Data.dat", numg)) {
        cout << "Error";
    }
    outBinFile("Data.dat");
    cout << "Введите номер группы для перевода студентов: ";
    int newGroup;
    cin >> newGroup;
    perevodStudents("programTextFile.txt", "Data.dat", newGroup, numg);
    cout << endl;
    outBinFile("Data.dat");
    return 0;
}