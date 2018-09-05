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



//txc1.cc

#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

/*  Simulating the timeout and re-transmission of a message. */

class Txc1 : public cSimpleModule
{
    private:
        double timeout;
        cMessage *timeoutEvent;
        int seq;
        cMessage *message;
    public:
        Txc1();
        virtual ~Txc1();
    protected:
        virtual void sendCopyOf(cMessage *msg);
        virtual cMessage *generateNewMessage();
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc1);

Txc1::Txc1()
{
    timeoutEvent=NULL;
}

Txc1::~Txc1()
{
    cancelAndDelete(timeoutEvent);
    delete timeoutEvent;
}

void Txc1::initialize()
{
    seq = 0;
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    if (strcmp("tic", getName()) == 0){

        EV << "Sending initial message\n";
        message = generateNewMessage();
        sendCopyOf(message);
        // scheduling the timeout expiration, by self message

        scheduleAt(simTime()+timeout, timeoutEvent);

        EV << "Scheduled TOE in tic\n";
    }
}

void Txc1::sendCopyOf(cMessage *msg)
{
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, "out");
}

cMessage * Txc1::generateNewMessage()
{
    char msgname[20];
    sprintf(msgname,"tic-%d",++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Txc1::handleMessage(cMessage *msg)
{
    if(msg == timeoutEvent){

        EV << "Timeout expired, re-sending message and restarting timer.\n";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);

    } else { // Ack message arrives.

        if(strcmp("tic",getName()) == 0)
            EV << "ACK arrived\n";
        else {

            if(uniform(0,1) < 0.45) { // packet loss.
                EV<< "Packet loss!!!\n";
                delete msg;
                cancelEvent(timeoutEvent);

                return ;
            }

            EV << "sending ACK.\n";

        }

        delete msg;
        cancelEvent(timeoutEvent);
        delete message;
        message = generateNewMessage();
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
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




