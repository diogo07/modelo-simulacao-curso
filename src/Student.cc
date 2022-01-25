#include "Student.h"
#include <math.h>
#include <string.h>
#include <omnetpp.h>

Student::Student() {
    // TODO Auto-generated constructor stub
}

Student::~Student() {
    // TODO Auto-generated destructor stub
}

int Student::getRace(){
    return this->race;
}

void Student::setRace(int race){
    this->race = race;
}

void Student::setDisapprovals(int index, int disapprovals){
    this->disapprovals[index] = disapprovals;
}

void Student::setEntryPeriod(int index, int entry){
    this->entryPeriod[index] = entry;
}

void Student::setExitPeriod(int index, int exit){
    this->exitPeriod[index] = exit;
}

void Student::setBeginner(bool beginner){
    this->beginner = beginner;
}

void Student::setBondDuration(int bondDuration){
    this->bondDuration = bondDuration;
}

void Student::setTimeEntry(double timeEntry){
    this->timeEntry = timeEntry;
}

void Student::setExitTime(double exitTime){
    this->exitTime = exitTime;
}

void Student::setRegistrationCount(int qtde){
    this->registrationCount = qtde;
}

int Student::getRegistrationCount(){
    return this->registrationCount;
}

int Student::getDisapprovals(int index){
    return this->disapprovals[index];
}

int Student::getEntryPeriod(int index){
    return this->entryPeriod[index];
}

int Student::getExitPeriod(int index){
    return this->exitPeriod[index];
}

int Student::getTotalDisapprovals(){
    int total = 0;
    for (int i = 0; i > 10; i++){
        total += this->disapprovals[i];
    }

    return total;
}

bool Student::getBeginner(){
    return this->beginner;
}

int Student::getBondDuration(){
    return this->bondDuration;
}

double Student::getTimeEntry(){
    return this->timeEntry;
}

double Student::getExitTime(){
    return this->exitTime;
}

int Student::getNumberOfSemestersTaken(double exit){
    return (int)((exit - this->timeEntry) / 6);
}

bool Student::graduationIsPossible(double currentSemester, int currentPeriod){
    int semesters = (int) ((currentSemester - this->timeEntry) / 6);
    return (semesters + (10 - currentPeriod)) <= 20;
}

