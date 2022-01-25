#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include <string.h>


class Student : public omnetpp::cMessage{
private:
    int number;
    int race;
    int registrationCount;
    bool beginner;
    double timeEntry;
    double exitTime;
    int bondDuration;
    int disapprovals [10] = {};
    int entryPeriod [10] = {};
    int exitPeriod [10] = {};
public:
    Student();
    virtual ~Student();

    virtual void setRace(int race);

    virtual void setDisapprovals(int index, int disapprovals);
    virtual void setEntryPeriod(int index, int entry);
    virtual void setExitPeriod(int index, int exit);
    virtual void setBeginner(bool beginner);
    virtual void setBondDuration(int bondDuration);

    virtual void setTimeEntry(double timeEntry);
    virtual void setExitTime(double exitTime);

    virtual int getRace();

    virtual int getDisapprovals(int index);
    virtual int getEntryPeriod(int index);
    virtual int getExitPeriod(int index);
    virtual int getTotalDisapprovals();
    virtual bool getBeginner();
    virtual int getBondDuration();
    virtual double getTimeEntry();
    virtual double getExitTime();

    virtual int getRegistrationCount();
    virtual void setRegistrationCount(int qtde);

    virtual int getNumberOfSemestersTaken(double saida);

    virtual bool graduationIsPossible(double currentSemester, int currentPeriod);


};

