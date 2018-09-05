// tictoc1.ned
simple Txc1
{
    parameters:
        
    	double delayTime = 0.5;
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


/*  txc1.cc */
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
        if(uniform(0,1) < 0.1) {
            EV << " loosing message.\n";
            //delete msg;
        } else {
            double delay = par("delayTime");
            EV<< "Message arrived.Wait: " << delay << " seconds\n";
            scheduleAt(simTime()+ delay, event);
        }
    }
}


//omnetpp.ini

# This file is shared by all tictoc simulations.
# Lines beginning with `#' are comments

[General]
# nothing here
 

[Config Tictoc1]
network = Tictoc1
#Tictoc1.tic.delayTime = exponential(3)
#Tictoc1.toc.delayTime = truncnormal(3,1)


