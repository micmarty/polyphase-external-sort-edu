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
#define RECORDS_NUMBER 30
#define A true
#define B false

using namespace std;


class FibonacciGenerator {
    unsigned int fib1 = 1;
    unsigned int fib2 = 1;
    unsigned int newFib = 1;
public:
    FibonacciGenerator() {
    }

    unsigned int limitForA() {
        return newFib;
    }

    unsigned int limitForB() {
        return newFib;
    }

    unsigned int limit() {
        return newFib;
    }

    unsigned int next() {
        newFib = fib1 + fib2;
        fib1 = fib2;
        fib2 = newFib;
        return newFib;
    }

    unsigned int nextForA() {
        newFib = fib1 + fib2;
        fib1 = fib2;
        fib2 = newFib;

        return newFib;
    }

    unsigned int nextForB() {
        return fib1;
    }

} fibonacciGenerator;


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
    float tab[] = {10, 8, 2, 1, 2, 2, 7, 9, 8, 8, 5, 8, 9, 4, 7, 1, 6, 1, 6, 6, 8, 5, 2, 4, 10, 2, 3, 6, 1, 3};
    //float tab[] = {3,2,1,6,5,1};
    vector<Cone> cones;
    std::ofstream destinationFile(destinationPath, ios::out | ios::binary);

    //TODO move to Cone class
    //generate 2 parameters and push to vector, then display
    for (int i = 0; i < amount; i++) {
        float radius = (float) tab[i];//(std::rand() % 10 + 1);
        float height = (float) (std::rand() % 10 + 1);
        cones.push_back(Cone(radius, height));

        cout << cones[i].getVolume() << "\t";
    }
    //write to output file
    destinationFile.write(reinterpret_cast<const char *>(cones.data()), cones.size() * sizeof(Cone));
    destinationFile.close();
}


void write_to_tape(std::ofstream &tape, vector<Cone> &records) {
    tape.write(reinterpret_cast<const char *>(records.data()), records.size() * sizeof(Cone));
}


Cone ostatni_z_biezacego_bufora(bool whichTape, vector<Cone> bufferA, vector<Cone> bufferB) {
    if (whichTape == A && !bufferA.empty())
        return bufferA.back();
    else if (whichTape == B && !bufferB.empty())
        return bufferB.back();
    return Cone(INT_MIN, INT_MIN);
}

void wstaw_do_bufora_tasmy(bool whichTape, vector<Cone>& bufferA, vector<Cone>& bufferB, Cone &newElement,
                           ofstream& tapeA, ofstream& tapeB) {

    string which;

    if (whichTape == A && bufferA.size() != BUFFER_SIZE) {
        bufferA.push_back(newElement);
        which = "A";
    } else if (whichTape == B && bufferB.size() != BUFFER_SIZE) {
        bufferB.push_back(newElement);
        which = "B";
    }else if(whichTape == A && bufferA.size() == BUFFER_SIZE){
        cout<<"~  bufor A sie wypelnil, zrzucam na tasme: ";display_buffer_content(bufferA);cout<<endl;
        write_to_tape(tapeA,bufferA);
        //bufferA.at(0) = bufferA.back();
        bufferA.clear();
        //bufferA.resize(1);

        bufferA.push_back(newElement);
        which = "A";
    }else if(whichTape == B && bufferB.size() == BUFFER_SIZE){
        cout<<"~  bufor B sie wypelnil, zrzucam na tasme: ";display_buffer_content(bufferB);cout<<endl;
        write_to_tape(tapeB,bufferB);
        //bufferB.at(0) = bufferB.back();
        bufferB.clear();
        //bufferB.resize(1);

        bufferB.push_back(newElement);
        which = "B";
    }
    cout << "-> wpisano " << newElement.getVolume() << " do bufora " << which << endl;
}

bool fib_pozwala(bool whichTape, unsigned int seriesOnA, unsigned int seriesOnB) {
    unsigned int series, limit;
    if (whichTape == A) {
        series = seriesOnA;
        limit = fibonacciGenerator.limitForA();
    } else if (whichTape == B) {
        series = seriesOnB;
        limit = fibonacciGenerator.limitForB();
    }

    if (series < limit)
        return true;
    else {
        cout << "osiagnieto max serii=" << limit << endl;
        return false;
    }

}


void zmien_tasme(bool &whichTape) {
    whichTape = !whichTape;
    if (whichTape == A)
        cout << "zmieniono tasme na A" << endl;
    else
        cout << "zmieniono tasme na B" << endl;
}

vector<unsigned int>
zlicz_serie(bool tape, unsigned int &seriesOnTapeA, unsigned int &seriesOnTapeB, vector<Cone> &a, vector<Cone> &b) {
    vector<unsigned int> saved = {seriesOnTapeA, seriesOnTapeB};

    if (tape == A) {
        seriesOnTapeA++;
        cout << "*  zliczono " << seriesOnTapeA << "/" << fibonacciGenerator.limitForA() << " serii na A, ostatni = "
             << a.back().getVolume() << endl;

    } else if (tape == B) {
        seriesOnTapeB++;
        cout << "*  zliczono " << seriesOnTapeB << "/" << fibonacciGenerator.limitForB() << " serii na B, ostatni = "
             << b.back().getVolume() << endl;
    }

    return saved;
}

void kontynuuj_serie(bool tape, unsigned int &seriesOnTapeA, unsigned int &seriesOnTapeB) {
    if (tape == A && seriesOnTapeA != 0) {
        cout << "kontynuuje serie " << seriesOnTapeA << " na tasmie A" << endl;
        seriesOnTapeA--;
    } else if (tape == B && seriesOnTapeA != 1) {
        cout << "kontynuuje serie " << seriesOnTapeB << " na tasmie B" << endl;
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

    vector<Cone> readBuffer(BUFFER_SIZE);
    vector<Cone> A_Buffer;
    vector<Cone> B_Buffer;

    ifstream tapeINPUT(tape_INPUT_path, ios::in | ios::binary);
    ofstream tapeA(tape_A_path, ios::out | ios::binary);
    ofstream tapeB(tape_B_path, ios::out | ios::binary);

    // FILL TAPE
    generate_records(RECORDS_NUMBER, tape_INPUT_path);
    cout << endl << "-------------------------------------------------------------------" << endl;

    // DISTRIBUTION
    unsigned int seriesOnTapeA = 0, seriesOnTapeB = 0;
    bool whichTape = A;
    bool wstawionoElement=false;

    std::vector<Cone> *bufferToPut = &A_Buffer;     // wskaznik na tasme do ktorej mamy pisac
    Cone last = Cone(INT_MIN, INT_MIN);             // udajemy ze na tasmie A cos jest

    //  read file by chunks
    while (tapeINPUT.read(reinterpret_cast<char *>(readBuffer.data()), sizeof(Cone) * BUFFER_SIZE)) {
        for (int readBufferIndex = 0; readBufferIndex < BUFFER_SIZE; readBufferIndex++) {
            do {
                last = ostatni_z_biezacego_bufora(whichTape, A_Buffer, B_Buffer);

                if (last <= readBuffer.at(readBufferIndex)) {
                    wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, readBuffer.at(readBufferIndex), tapeA, tapeB);
                    wstawionoElement = true;
                } else {
                    vector<unsigned int> seriePrzedInkrementacja = zlicz_serie(whichTape, seriesOnTapeA, seriesOnTapeB,
                                                                               A_Buffer, B_Buffer);

                    if (!fib_pozwala(whichTape, seriesOnTapeA, seriesOnTapeB)) {
                        zmien_tasme(whichTape);
                        fibonacciGenerator.next();

                        if (ostatni_z_biezacego_bufora(whichTape, A_Buffer, B_Buffer) <= readBuffer.at(readBufferIndex) &&
                            !B_Buffer.empty()) {
                            whichTape == A ? seriesOnTapeA-- : seriesOnTapeB--;
                            //cofamy do stanu sprzed zwiekszenia licznika, dla tej tasmy gdzie byla kontynuacja
                            //whichTape==A?seriesOnTapeA = seriePrzedInkrementacja[0]:seriesOnTapeB = seriePrzedInkrementacja[1];
                            whichTape == A ? cout << "kontynuacja na tasmie A nadal jest " << seriesOnTapeA << " serii"
                                                  << endl : cout << "kontynuacja na tasmie B nadal jest "
                                                                 << seriesOnTapeB << " serii" << endl;;
                        }

                        wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, readBuffer.at(readBufferIndex),
                                              tapeA, tapeB);//zakomentowac ewentualnie
                        wstawionoElement = true;//false
                    } else {
                        wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, readBuffer.at(readBufferIndex),
                                              tapeA, tapeB);
                        wstawionoElement = true;
                    }
                }
            } while (!wstawionoElement);
        }
    }
    
    //add_empty_series();
    // TODO replace it with something reasonable(DON'T REALLY WRITE TO THE BUFFER)
    int i = -1;
    while (fib_pozwala(whichTape, seriesOnTapeA, seriesOnTapeB)) {
        Cone empty = Cone(i, i);
        wstaw_do_bufora_tasmy(whichTape, A_Buffer, B_Buffer, empty, tapeA, tapeB);
        zlicz_serie(whichTape, seriesOnTapeA, seriesOnTapeB, A_Buffer, B_Buffer);
        i--;
    }

    
    
    
    
    
    
    

    display_buffer_content(A_Buffer);
    cout << "series: " << seriesOnTapeA << endl;
    display_buffer_content(B_Buffer);
    cout << "series: " << seriesOnTapeB << endl;

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




