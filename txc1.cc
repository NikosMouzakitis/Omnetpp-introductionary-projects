#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc1 : public cSimpleModule
{

  private:
    int counter;
  protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(Txc1);

void Txc1::initialize()
{
    counter = par("limit");
    WATCH(counter);

    if (strcmp("tic", getName()) == 0) {

        cMessage *msg = new cMessage("tictocMsg");
        EV << "Sending initial message\n";
        send(msg, "out");

    }
}

void Txc1::handleMessage(cMessage *msg)
{

    counter--;

    if(counter == 0) {
        EV << "End of simulation\n";
        delete msg;
    } else {

        EV << "Counter: " << counter << "\n";
        send(msg,"out");
    }
}
