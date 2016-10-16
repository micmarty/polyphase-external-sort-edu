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
#define RECORDS_NUMBER 12

using namespace std;

/*  displays argument to the console
 *  each buffer element is a Cone
 *
 *  params:
 *  1 -> vector of Cones
*/
void display_buffer_content(vector<Cone> &buffer) {
    for (Cone n : buffer)
        cout << n.getVolume() << " ";
    cout << endl;
}

/*  generates Cone objects into INPUT tape
 *
 *  params:
 *  1 -> how many to produce and put onto a tape
 *  2 -> path to empty INPUT tape
*/
void generate_records(long amount, string destinationPath) {
    //DEBUG
    float tab[] = {1,6,2,4,3,2,4,5,6,7,6,5};
    vector<Cone> cones;
    std::ofstream destinationFile(destinationPath, ios::out | ios::binary);

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

int main() {
    // BASIC DECLARATIONS
    std::srand(time(0));
    const string projectPath = "/home/miczi/ClionProjects/polyphase-external-sort-edu/";
    const string tape_INPUT_path = projectPath + "INPUT";
    const string tape_A_path = projectPath + "A";
    const string tape_B_path = projectPath + "B";

    // FILL TAPE
    generate_records(RECORDS_NUMBER, tape_INPUT_path);
    cout << endl << "-------------------------------------------------------------------" << endl;

    // DISTRIBUTION
    vector<Cone> readBuffer(BUFFER_SIZE);
    vector<Cone> A_Buffer;
    vector<Cone> B_Buffer;

    std::ifstream tapeINPUT(tape_INPUT_path, ios::in | ios::binary);
    std::ofstream tapeA(tape_A_path, ios::out | ios::binary);
    std::ofstream tapeB(tape_B_path, ios::out | ios::binary);


    bool toTapeA = true;                        // od ktorej tasmy zaczynamy
    std::vector<Cone>* bufferToPut = &A_Buffer; // wskaznik na tasme do ktorej mamy pisac
    Cone last = Cone(INT_MIN, INT_MIN);         // udajemy ze na tasmie A cos jest

    //napelnij bufor read
    while(tapeINPUT.read(reinterpret_cast<char *>(readBuffer.data()), sizeof(Cone) * BUFFER_SIZE)){
        //dla kazdego elementu w buforze
        for(int readBufferIndex = 0;readBufferIndex < BUFFER_SIZE; readBufferIndex++){
            //jesli ostatnio wstawiany element do bufora A lub B jest mniejszy od tego ktory mamy w readBuffer
            if(last < readBuffer.at(readBufferIndex)){
                //jesli bufor a jest pelny to zapisz najpierw na tasme i wyczysc bufror

                //to wpisz do tego bufora z ktorego pochodzi last
                bufferToPut->push_back(readBuffer.at(readBufferIndex));
                if(bufferToPut == &A_Buffer){
                    cout<<readBuffer.at(readBufferIndex).getVolume()<< " pushed to buffer A"<<endl;
                } else{
                    cout<<readBuffer.at(readBufferIndex).getVolume()<< " pushed to buffer B"<<endl;
                }
            } else{
                //jesli bufor B jest pelny to zapisz najpierw na tasme i wyczysc bufror

                //to wpisz to przeciwnego bufora niz ten z ktorego jest last

                toTapeA = !toTapeA;
                bufferToPut = (toTapeA) ? &A_Buffer : &B_Buffer;
                bufferToPut->push_back(readBuffer.at(readBufferIndex));
                if(bufferToPut == &A_Buffer){
                    cout<<"buffer swapped to A"<<endl;
                    cout<<readBuffer.at(readBufferIndex).getVolume()<< " pushed to buffer A"<<endl;
                } else{
                    cout<<"buffer swapped to B"<<endl;
                    cout<<readBuffer.at(readBufferIndex).getVolume()<< " pushed to buffer B"<<endl;
                }
            }
            last = bufferToPut->back();//zaktualizuj ostatni element
            cout<< "last: " << last.getVolume()<<endl;
        }
    }
    display_buffer_content(A_Buffer);
    display_buffer_content(B_Buffer);

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




