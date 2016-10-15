#include <iostream>
#include <zconf.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include "Cone.h"

#define BUFFER_SIZE 3
#define RECORDS_NUMBER 10

using namespace std;

/*  displays argument to the console
 *  each buffer element is a Cone
 *
 *  params:
 *  1 -> vector of Cones
*/
void display_buffer_content(vector<Cone> &buffer) {
//    for (Cone n : buffer)
//        cout << n.radius << "\t" << n.height << endl;
//    cout << "~~~~~~~" << endl;

    for (Cone n : buffer)
        cout << n.getVolume() << " ";
    cout << endl;
}

/*  uses previously opened stream and read by chunks, which size is specified in 2nd param
 *
 *  params:
 *  1 -> buffer we want to fill
 *  2 -> how many records we want to read at once
 *  3 -> opened file stream
*/
void fill_buffer_from_file(vector<Cone> &buffer, int recordsInBuffer, std::ifstream &fileToRead) {

    //display_buffer_content(buffer);
}

/*  generates Cone objects into tape A
 *
 *  params:
 *  1 -> temporary vector of freshly generated cones
 *  2 -> how many to produce and put onto a tape
 *  3 -> path to empty tape A
*/
void generate_records(long amount, string destinationPath) {
    //DEBUG
    float tab[10] = {1,6,2,4,3,2,4,5,6,7};
    vector<Cone> cones;
    std::ofstream destinationFile(destinationPath, ios::in | ios::binary);

    //TODO move to Cone class
    //generate 2 parameters and push to vector, then display
    for (int i = 0; i < amount; i++) {
        float radius = (float) tab[i];//(std::rand() % 10 + 1);
        float height = (float) (std::rand() % 10 + 1);
        cones.push_back(Cone(radius, height));

        cout << cones[i].getVolume()<< "\t";
    }
    //write to output file
    destinationFile.write(reinterpret_cast<const char *>(cones.data()), cones.size() * sizeof(Cone));
    destinationFile.close();
}


void write_to_tape(std::ofstream& tape, vector<Cone> &records) {
    tape.write(reinterpret_cast<const char *>(records.data()), records.size() * sizeof(Cone));
}


int is_vector_sorted(vector<Cone> &v){
    for(int i=0; i < v.size();i++){
        if(i!=0 && v[i-1] > v[i])
            return i;
    }
}

int main() {
    // BASIC DECLARATIONS
    std::srand(time(0));
    string tape_INPUT_path = "/home/miczi/ClionProjects/polyphase_sorting_by_miczi/INPUT";
    string tape_A_path = "/home/miczi/ClionProjects/polyphase_sorting_by_miczi/A";
    string tape_B_path = "/home/miczi/ClionProjects/polyphase_sorting_by_miczi/B";

    // FILL TAPE
    generate_records(RECORDS_NUMBER, tape_INPUT_path);
    cout << endl << "-------------------------------------------------------------------" << endl;

    // DISTRIBUTION
    vector<Cone> readBuffer(BUFFER_SIZE);
    vector<Cone> A_Buffer(BUFFER_SIZE);
    vector<Cone> B_uffer(BUFFER_SIZE);

    std::ifstream tapeINPUT(tape_INPUT_path, ios::in | ios::binary);
    std::ofstream tapeA(tape_A_path, ios::in | ios::binary);
    std::ofstream tapeB(tape_B_path, ios::in | ios::binary);


    bool toTapeA = true;

    while(tapeINPUT.read(reinterpret_cast<char *>(buffer.data()), sizeof(Cone) * BUFFER_SIZE)){ //czytaj wielkosc bufora



        //w buffer mamy trzy elementy
        std::size_t const sortedPartLength = is_vector_sorted(buffer);
        std::vector<Cone> series(buffer.begin(), buffer.begin() + sortedPartLength);
        toTapeA ? write_to_tape(tapeA, series) : write_to_tape(tapeB, series);

        toTapeA != toTapeA;

        std::vector<Cone> newSeries(buffer.begin() + sortedPartLength, buffer.end());

        toTapeA ? write_to_tape(tapeA, newSeries) : write_to_tape(tapeB, newSeries);
    }

    tapeA.close();
    std::ifstream tapeAA(tape_A_path, ios::in | ios::binary);
    vector<Cone> bufferA(BUFFER_SIZE);
    tapeAA.read(reinterpret_cast<char *>(bufferA.data()), sizeof(Cone) * BUFFER_SIZE);
    tapeAA.close();

    tapeB.close();
    std::ifstream tapeBB(tape_A_path, ios::in | ios::binary);
    vector<Cone> bufferB(7);
    tapeBB.read(reinterpret_cast<char *>(bufferB.data()), sizeof(Cone) * 7);
    tapeBB.close();

    display_buffer_content(bufferA);
    display_buffer_content(bufferB);

    tapeINPUT.close();
    return 0;
}




