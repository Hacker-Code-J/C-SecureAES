// g++ -g -Wall -Wextra file_io.cpp -o file_io
#include <iostream>
#include <fstream>
using namespace std;

void textfile_input1() {
    ifstream fin;       // ifstream(class)
    char line[256];     // buffer

    fin.open("input-test.txt");
    if (fin.fail()) {
        cerr << "File Open Error!" << std::endl;
        return;
    }

    fin >> line;
    cout << line << endl;

    fin >> line;
    cout << line << endl;
    
    fin.close();
}

void textfile_input2() {
    ifstream fin;       // ifstream -> ifstream
    char ch;
    char buffer[2048];

    fin.open("input-test.txt");
    if (fin.fail()) {
        cerr << "File Open Error!" << std::endl;
        return;
    }

    int cnt = 0;
    while (fin.get(ch)) {
        buffer[cnt] = ch;
        cnt++;
        if (cnt > 2048) break;
    }
    buffer[cnt] = 0x00;
    cout << buffer << endl;
    fin.close();
}

void textfile_copy() {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open("input-test.txt");
    if (fin.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }

    fout.open("output-test.txt");
    if (fout.fail()) {
        cerr << "Output File Open Error" << endl;
        return;
    }

    while (fin.get(ch))
        fout.put(ch);

    fin.close();
    fout.close();
}

void binfile_output() {
    ofstream fout;
    char ch;
    fout.open("bin-input-test", ios::binary);
    if (fout.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }

    ch = 'A';
    fout.write(&ch,1);

    int x = 0x01020304;
    fout.write((char*)&x, sizeof(int));   // type-casting
    fout.close();
}

void binfile_copy() {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open("bin-input-test");
    if (fin.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }

    fout.open("bin-output-new");
    if (fout.fail()) {
        cerr << "Output File Open Error" << endl;
        return;
    }

    while (fin.read(&ch, 1))
        fout.write(&ch, 1);

    fin.close();
    fout.close();
}

void file_size1() {
    ifstream fin;
    char ch;
    int cnt;

    fin.open("bin-input-test");
    if (fin.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }
    
    while (fin.read(&ch, 1)) {
        cnt++;
    }

    cout << "file size = " << cnt << " bytes" << endl;
    
    fin.close();
}
void file_size2() {
    ifstream fin;
    int file_length;

    fin.open("bin-input-test");
    if (fin.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }
    
    fin.seekg(0, fin.end);
    file_length = fin.tellg();

    // while (fin.read(&ch, 1)) {
    //     cnt++;
    // }

    cout << "file size = " << file_length << " bytes" << endl;
    
    fin.close();
}

void gen_bin() {
    ofstream fout;
    fout.open("before-pad.bin", ios::binary);
    if (fout.fail()) {
        cerr << "Input File Open Error" << endl;
        return;
    }

    long long data[20] = { 0x00, };
    long long default0 = 0x0706050403020100;
    long long default1 = 0x0F0E0D0C0B0A0908;

    long long temp = 0x1010101010101010;
    for (int i = 0; i < 10; i++) {
        data[2 * i + 0] = default0 + i * temp; 
        data[2 * i + 1] = default1 + i * temp;
    }

    for (int i = 0; i < 20; i++)
        fout.write((char*)&data[i], sizeof(data[i]));
    fout.close();
}

int main() {
    // textfile_input1();
    // textfile_input2();
    // textfile_copy();
    // binfile_output();
    // binfile_copy();
    file_size1();
    file_size2();
    // gen_bin();
}