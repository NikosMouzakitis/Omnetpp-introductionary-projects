
/*

The first node sends a message to itself, and by that triggers the transmission after a second to the other node.
The other one as well, after beeing trigered by an event, (of receiving the message will reply back).

*/
// File : tictac1.ned

simple Txc1
{
    parameters:
        int limit =10;
    gates:
        input in;
        output out;
}
network Tictoc1
{
    @display("bgb=396.99002,224.73001");
    submodules:
        tic: Txc1;
        toc: Txc1;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        toc.out --> {  delay = 100ms; } --> tic.in;

}




// File: Txc1.cc

#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class Txc1 : public cSimpleModule
{
    private:
        cMessage *event;
        cMessage *tictocMsg;
        int counter;
    public:
        Txc1();
        virtual ~Txc1();
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};
Define_Module(Txc1);
Txc1::Txc1()
{
    event=tictocMsg=NULL;
}
Txc1::~Txc1()
{
    cancelAndDelete(event);
    delete tictocMsg;
}
void Txc1::initialize()
{

    event = new cMessage("event");
    cMessage *msg = new cMessage("tictocMsg");
    tictocMsg = new cMessage("tictoc msg");
    if (strcmp("tic", getName()) == 0) {


        EV << "Scheduling first to send at 5s\n";

        scheduleAt(5.0,event);

    }
}
void Txc1::handleMessage(cMessage *msg)
{

    if(msg == event) {
        EV << "Wait period over\n";
        tictocMsg= new cMessage("tictocMsg");
        send(tictocMsg, "out");
        tictocMsg=NULL;
    } else {
        EV<< "Message arrived.Wait 1 sec\n";
        scheduleAt(simTime()+1.0, event);

    }
}

