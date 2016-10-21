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
#define RECORDS_NUMBER 20
#define A true
#define B false

using namespace std;
vector<Cone>::iterator aIterator,bIterator;


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

    unsigned int next(){
        newFib = fib1+ fib2;
        fib1 = fib2;
        fib2 = newFib;
        return newFib;
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

}fibonacciGenerator;



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
    float tab[] = {1,6,2,8,7,9,4,5,6,7,6,5,1,2,3,4,3,2,1,0};
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



Cone ostatni_z_biezacego_bufora(bool whichTape,vector<Cone> bufferA, vector<Cone> bufferB) {
    if(whichTape == A && !bufferA.empty())
        return bufferA.back();
    else if(whichTape == B && !bufferB.empty())
        return bufferB.back();
    return Cone(INT_MIN,INT_MIN);
}

void wstaw_do_bufora_tasmy(bool whichTape, vector<Cone>& bufferA, vector<Cone>& bufferB,Cone newElement) {
    string which;

    if(whichTape == A){
        bufferA.push_back(newElement);  which = "A";
    }else if(whichTape == B){
        bufferB.push_back(newElement);  which = "B";
    }
    cout<< "wpisano " << newElement.getVolume()<<" do bufora " << which << endl;
}

bool fib_pozwala(bool whichTape, unsigned int seriesOnA, unsigned int seriesOnB) {
    unsigned int series,limit;
    if(whichTape==A){
        series = seriesOnA;
        limit = fibonacciGenerator.limitForA();
    }else if(whichTape==B){
        series = seriesOnB;
        limit = fibonacciGenerator.limitForB();
    }

    if(series < limit)
        return true;
    else{
        cout<<"osiagnieto max serii="<<limit<<endl;
        return false;
    }

}


void zmien_tasme(bool& whichTape) {
    whichTape = !whichTape;
    if(whichTape==A)
        cout<< "zmieniono tasme na A"<<endl;
    else
        cout<< "zmieniono tasme na B"<<endl;
}

void zlicz_serie(bool tape, unsigned int& seriesOnTapeA, unsigned int& seriesOnTapeB, vector<Cone>& a,vector<Cone>& b) {

    if(tape==A){
        seriesOnTapeA++;
        cout<< "zliczono " << seriesOnTapeA<<"/"<< fibonacciGenerator.limitForA() <<" serii na A, ostatni = "<<a.back().getVolume()<<endl;
    }else if(tape==B){
        seriesOnTapeB++;
        cout<< "zliczono " << seriesOnTapeB <<"/"<< fibonacciGenerator.limitForA() <<" serii na B, ostatni = "<<b.back().getVolume()<<endl;
    }
}

void kontynuuj_serie(bool tape, unsigned int& seriesOnTapeA, unsigned int& seriesOnTapeB) {
    if(tape == A && seriesOnTapeA!=0){
        cout<< "kontynuuje serie " << seriesOnTapeA <<" na tasmie A"<<endl;
        seriesOnTapeA--;
    }else if(tape==B && seriesOnTapeA!=1)
    {
        cout<< "kontynuuje serie " << seriesOnTapeB <<" na tasmie B"<<endl;
        seriesOnTapeB--;
    }


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
    std::vector<Cone> readBuffer(BUFFER_SIZE);
    std::vector<Cone> A_Buffer;
    std::vector<Cone> B_Buffer;

    std::ifstream tapeINPUT(tape_INPUT_path, ios::in | ios::binary);
    std::ofstream tapeA(tape_A_path, ios::out | ios::binary);
    std::ofstream tapeB(tape_B_path, ios::out | ios::binary);


    unsigned int seriesOnTapeA = 0, seriesOnTapeB = 0;

    bool toTapeA = true;                        // od ktorej tasmy zaczynamy
    std::vector<Cone>* bufferToPut = &A_Buffer; // wskaznik na tasme do ktorej mamy pisac
    Cone ostatni = Cone(INT_MIN, INT_MIN);         // udajemy ze na tasmie A cos jest
    Cone lastOnA,lastOnB;


    bool whichTape = A;
    unsigned int fibOnA = 1,fibOnB = 1;
    bool definitywnyKoniec=false;

    //napelnij bufor read
    while(tapeINPUT.read(reinterpret_cast<char *>(readBuffer.data()), sizeof(Cone) * BUFFER_SIZE)){
        //dla kazdego elementu w buforze
        for(int readBufferIndex = 0;readBufferIndex < BUFFER_SIZE; readBufferIndex++){


            start:
            ostatni = ostatni_z_biezacego_bufora(whichTape, A_Buffer,B_Buffer);
            if(fib_pozwala(whichTape,seriesOnTapeA,seriesOnTapeB))
            {
                if(ostatni <= readBuffer.at(readBufferIndex) || definitywnyKoniec)
                {
                    wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, readBuffer.at(readBufferIndex));
                    definitywnyKoniec=false;
                }
                else
                {
                    definitywnyKoniec=true;
                    zlicz_serie(whichTape,seriesOnTapeA,seriesOnTapeB,A_Buffer,B_Buffer);
                    goto start;
                }
            } else
            {

                if(whichTape==A && seriesOnTapeA==fibonacciGenerator.limitForA())
                    fibonacciGenerator.next();
                else if(whichTape==B && seriesOnTapeB==fibonacciGenerator.limitForB())
                    fibonacciGenerator.next();
                zmien_tasme(whichTape);

                ostatni = ostatni_z_biezacego_bufora(whichTape, A_Buffer,B_Buffer);
                if(ostatni <= readBuffer.at(readBufferIndex))
                {
                    kontynuuj_serie(whichTape,seriesOnTapeA,seriesOnTapeB);
                    wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, readBuffer.at(readBufferIndex));
                }
                else
                {

                    //zmien_tasme(whichTape);
                    goto start;
                }
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




