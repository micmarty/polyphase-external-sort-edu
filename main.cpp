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
#define RECORDS_NUMBER 18

using namespace std;
vector<Cone>::iterator aIterator,bIterator;

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
    float tab[] = {1,6,2,8,7,9,4,5,6,7,6,5,1,2,3,4,3,2};
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

void ustawLastAiB(Cone &lastOnA, Cone& lastOnB, vector<Cone> &A_Buffer, vector<Cone>& B_Buffer) {

    lastOnA = (A_Buffer.empty()) ? Cone(INT_MIN,INT_MIN) : A_Buffer.back();
    lastOnB = (B_Buffer.empty()) ? Cone(INT_MIN,INT_MIN) : B_Buffer.back();

    cout<< "lastA: " << lastOnA.getVolume()<<endl;
    cout<< "lastB: " << lastOnB.getVolume()<<endl;

    aIterator = A_Buffer.end()-2;
    bIterator = B_Buffer.end()-2;

}

class FibonacciGenerator{
    unsigned int fib1 = 1;
    unsigned int fib2 = 1;
    unsigned int newFib = 1;

public:
    FibonacciGenerator(){
    }

    unsigned int limitForA(){
        return fib2;
    }
    unsigned int limitForB(){
        return fib1;
    }

    unsigned int nextForA(){
        newFib = fib1 + fib2;
        fib1 = fib2;
        fib2 = newFib;

        return newFib;
    }

    unsigned int nextForB(){
        return fib1;
    }

};

int main() {
    // BASIC DECLARATIONS
    std::srand(time(0));
    const string projectPath = "/home/miczi/ClionProjects/polyphase-external-sort-edu/";
    const string tape_INPUT_path = projectPath + "INPUT";
    const string tape_A_path = projectPath + "A";
    const string tape_B_path = projectPath + "B";
    FibonacciGenerator fibonacciGenerator = FibonacciGenerator();

    // FILL TAPE
    generate_records(RECORDS_NUMBER, tape_INPUT_path);
    cout << endl << "-------------------------------------------------------------------" << endl;

    // DISTRIBUTION
    std::vector<Cone> readBuffer(BUFFER_SIZE);
    std::vector<Cone> A_Buffer;
    std::vector<Cone> B_Buffer;

    std::ifstream tapeINPUT(tape_INPUT_path, ios::in | ios::binary);
    std::ofstream tapeA(tape_A_path, ios::out | ios::binary);
    std::ofstream tapeB(tape_B_path, ios::out | ios::binary);


    unsigned int seriesOnTapeA = 0, seriesOnTapeB = 0;

    bool toTapeA = true;                        // od ktorej tasmy zaczynamy
    std::vector<Cone>* bufferToPut = &A_Buffer; // wskaznik na tasme do ktorej mamy pisac
    Cone last = Cone(INT_MIN, INT_MIN);         // udajemy ze na tasmie A cos jest
    Cone lastOnA,lastOnB;

    unsigned int fibOnA = 1,fibOnB = 1;


    //napelnij bufor read
    while(tapeINPUT.read(reinterpret_cast<char *>(readBuffer.data()), sizeof(Cone) * BUFFER_SIZE)){
        //dla kazdego elementu w buforze
        for(int readBufferIndex = 0;readBufferIndex < BUFFER_SIZE; readBufferIndex++){

            if(toTapeA && !A_Buffer.empty()){
                last = A_Buffer.back();
            }else if(!toTapeA && !B_Buffer.empty()){
                last = B_Buffer.back();
            }

            if(last < readBuffer.at(readBufferIndex))
            {
                //1.    zostajemy na tej samej tasmie
                //1.1   wpisujemy na ta tasme
                bufferToPut = (toTapeA) ? &A_Buffer : &B_Buffer;
                bufferToPut->push_back(readBuffer.at(readBufferIndex));
                //DEBUG
                if(bufferToPut == &A_Buffer)
                    cout<< "wpisuje " << readBuffer.at(readBufferIndex).getVolume()<<" do bufora A"<<endl;
                else
                    cout<< "wpisuje " <<readBuffer.at(readBufferIndex).getVolume()<<" do bufora B"<<endl;
            }
            else//ma sie zaczac nowa seria lub kontynuacja
            {
                //trzeba sprawdzic, czy mozemy zaczynac serie na drugiej tasmie, czy zostajemy
                if(toTapeA){
                    seriesOnTapeA++;
                    if(seriesOnTapeA < fibonacciGenerator.limitForA()){//jesli mamy mniej serii niz powinno być
                        bufferToPut = &A_Buffer;
                        cout << "taśma A: limit -> " << fibonacciGenerator.limitForA()<<endl;

                    }else{//jesli fibonacci nie pozwala juz wiecej wstawic, to zmieniamy tasme
                        toTapeA = !toTapeA;
                        bufferToPut = &B_Buffer;
                        //DEBUG
                        (toTapeA)?(cout<<"zamiana tasmy na A"<<endl):(cout<<"zamiana tasmy na B"<<endl);

                        //zliczamy zakonczona lub kontynuowana serie
                        if(!bufferToPut->empty() && bufferToPut->back() <= readBuffer.at(readBufferIndex))
                            cout<< "kontynuuje serie " << seriesOnTapeB <<" na tasmie B"<<endl;
                        else
                            cout<< "zliczylem zakonczona serie " << seriesOnTapeA <<" na tasmie A"<<endl;

                        fibonacciGenerator.nextForA();
                    }
                }else if(!toTapeA){
                    seriesOnTapeB++;
                    if (seriesOnTapeB < fibonacciGenerator.limitForB()){
                        bufferToPut = &B_Buffer;
                        cout << "taśma B: limit -> " << fibonacciGenerator.limitForB()<<endl;

                    }else{
                        toTapeA = !toTapeA;
                        bufferToPut = &A_Buffer;
                        //DEBUG
                        (toTapeA)?(cout<<"zamiana tasmy na A"<<endl):(cout<<"zamiana tasmy na B"<<endl);

                        //zliczamy zakonczona lub kontynuowana serie
                        if(!bufferToPut->empty() && bufferToPut->back() <= readBuffer.at(readBufferIndex))
                            cout<< "kontynuuje serie " << seriesOnTapeA <<" na tasmie A"<<endl;
                        else
                            cout<< "zliczylem zakonczona serie " << seriesOnTapeB <<" na tasmie B"<<endl;

                        fibonacciGenerator.nextForB();
                    }
                }

                bufferToPut->push_back(readBuffer.at(readBufferIndex));
                //DEBUG
                if(bufferToPut == &A_Buffer)
                    cout<< "wpisuje " << readBuffer.at(readBufferIndex).getVolume()<<" do bufora A"<<endl;
                else
                    cout<< "wpisuje " <<readBuffer.at(readBufferIndex).getVolume()<<" do bufora B"<<endl;
            }
        }
    }
    display_buffer_content(A_Buffer);cout<<"series: "<<seriesOnTapeA<<endl;
    display_buffer_content(B_Buffer);cout<<"series: "<<seriesOnTapeB<<endl;

    tapeA.close();
    std::ifstream tapeAA(tape_A_path, ios::in | ios::binary);
    std::vector<Cone> bufferA(BUFFER_SIZE);
    tapeAA.read(reinterpret_cast<char *>(bufferA.data()), sizeof(Cone) * BUFFER_SIZE);
    tapeAA.close();

    tapeB.close();
    std::ifstream tapeBB(tape_A_path, ios::in | ios::binary);
    std::vector<Cone> bufferB(7);
    tapeBB.read(reinterpret_cast<char *>(bufferB.data()), sizeof(Cone) * 7);
    tapeBB.close();
    tapeINPUT.close();
    return 0;
}




