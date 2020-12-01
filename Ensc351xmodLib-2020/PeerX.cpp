// Name        : PeerX.cpp
// ENSC 351 November 2020  Prepared by:
//      - Craig Scratchley, Simon Fraser University

// Version     : November
// Copyright   : Copyright 2020, Craig Scratchley
// Description :  ENSC 351 Project 
//============================================================================

//#include <fstream>
#include <sys/time.h>
#include <string.h>
#include "PeerX.h"
#include "VNPE.h"
#include "Linemax.h"
#include "myIO.h"
#include "AtomicCOUT.h"

using namespace std;

void checksum(uint8_t* sumP, blkT blkBuf)
{
	*sumP = blkBuf[DATA_POS];
	for( int ii=DATA_POS + 1; ii<PAST_CHUNK; ii++ )
		*sumP += blkBuf[ii];
}

PeerX::
PeerX(int d, const char *fname, char left, char right, const char *smLogN, int conInD, int conOutD, bool useCrc)
:result("ResultNotSet"),
 errCnt(0),
 Crcflg(useCrc),
 KbCan(false),
 mediumD(d),
 fileName(fname),
 transferringFileD(-1), 
 logLeft(left),
 logRight(right),
 smLogName(smLogN),
 consoleInId(conInD),
 consoleOutId(conOutD),
 reportInfo(false),
 absoluteTimeout(0),
 holdTimeout(0)
{
	struct timeval tvNow;
	PE(gettimeofday(&tvNow, NULL));
	sec_start = tvNow.tv_sec;
}

//Send a byte to the remote peer across the medium
void
PeerX::
sendByte(uint8_t byte)
{
	if (reportInfo) {
	//*** remove all but last of this block ***
		/*
		if (byte == NAK)
			COUT << "(N)" << flush;
		else if (byte == ACK)
			COUT << "(A)" << flush;
		else if (byte == EOT)
			COUT << "[E]" << flush;
		else */
			COUT << logLeft << byte << logRight << flush;
	}
	PE_NOT(myWrite(mediumD, &byte, sizeof(byte)), sizeof(byte));
}

// get rid of any characters that may have arrived from the medium.
void
PeerX::
dumpGlitches()
{
	const int dumpBufSz = 20;
	char buf[dumpBufSz];
	int bytesRead;
	while (dumpBufSz == (bytesRead = PE(myReadcond(mediumD, buf, dumpBufSz, 0, 0, 0))));
}

void
PeerX::
transferCommon(std::shared_ptr<StateMgr> mySM, bool reportInfoParam)
{
	reportInfo = reportInfoParam;
	/*
	// use this code to send stateChart logging information to a file.
	ofstream smLogFile; // need '#include <fstream>' above
	smLogFile.open(smLogName, ios::binary|ios::trunc);
	if(!smLogFile.is_open()) {
		CERR << "Error opening sender state chart log file named: " << smLogName << endl;
		exit(EXIT_FAILURE);
	}
//	mySM->setDebugLog(&smLogFile);
	// */

	// comment out the line below if you want to see logging information which will,
	//	by default, go to cout.
	mySM->setDebugLog(nullptr); // this will affect both peers.  Is this okay?

	mySM->start();

	/* ******** You may need to add code here ******** */
	
	struct timeval tv;

	while(mySM->isRunning()) {
		// ************* this loop is going to need more work ************
	    fd_set set;
        FD_ZERO(&set); //p Initialize set
        FD_SET(mediumD, &set); //p Add mediumD to set
        FD_SET(consoleInId, &set); //p Add console input to set

        char byteToReceive[3]; // [0] = ~, [1] = letter, [2] = \n
        tv.tv_sec=0;
        tv.tv_usec=0;//p from Debug menu tv_usec is included
        uint32_t now = elapsed_usecs(); //p Set now to current time
        if (now >= absoluteTimeout) {
            //P Quickly checks the return value to see if there's a possibility of a keyboard cancellation or input in mediumD
            int max_fd = max(mediumD, consoleInId)+1;
            tv.tv_usec = 100; //p Check very quickly!
            int rv = PE(select( max_fd, &set, NULL, NULL, &tv ));

            // If select function returned nothing
            if(rv != 0)
            {
                // Keyboard cancel has priority
                if(FD_ISSET(consoleInId, &set)) { // If the console has something to read from
                    cout << "\n consoleInId" << endl;
                    int returnValue = myReadcond(consoleInId, &byteToReceive, 3, 2, 1, 1);
                    cout << "\n Done reading! console" << endl;

                    if(returnValue == 3 && byteToReceive[0] == '&' && byteToReceive[1] == 'c' && byteToReceive[2] == '\n'){
                        mySM->postEvent(KB_C);
                    }
                }
                if(FD_ISSET(mediumD, &set)) { // If the mediumD has something to read from
                    cout << "\n mediumD" << endl;
                    //read character from medium
                    PE_NOT(myReadcond(mediumD, &byteToReceive[0], 1, 1, 0, 0), 1); // data should be available right //P Set timer to 1, since data should be available right away
                    cout << "\n Done reading! mediumD" << endl;

                    if (reportInfo)
                        COUT << logLeft << 1.0*(absoluteTimeout - now)/MILLION << ":" << (int)(unsigned char) byteToReceive[0] << ":" << byteToReceive[0] << logRight << flush;
                    mySM->postEvent(SER, byteToReceive[0]);
                }
            }
                tv.tv_sec=0;
                tv.tv_usec=0;//p from Debug menu tv_usec is included
                mySM->postEvent(TM);
        } else {
            tv.tv_usec = absoluteTimeout - now; //p Relative time
            int max_fd = max(mediumD, consoleInId)+1;
            int rv = PE(select( max_fd, &set, NULL, NULL, &tv ));

            if(rv == 0){ // Timeout?
                mySM->postEvent(TM);
            }
            else{
                // Keyboard cancel has priority
                if(FD_ISSET(consoleInId, &set)) { // If the console has something to read from
                    int returnValue = myReadcond(consoleInId, &byteToReceive, 3, 3, 1, 1);
                    if(returnValue == 3 && byteToReceive[0] == '&' && byteToReceive[1] == 'c' && byteToReceive[2] == '\n'){
                        mySM->postEvent(KB_C);
                    }
                }
                if(FD_ISSET(mediumD, &set)) { // If the mediumD has something to read froml;
                    //read character from medium
                    PE_NOT(myReadcond(mediumD, &byteToReceive[0], 1, 1, 0, 0), 1); // data should be available right //P Set timer to 1, since data should be available right away
                    if (reportInfo)
                        COUT << logLeft << 1.0*(absoluteTimeout - now)/MILLION << ":" << (int)(unsigned char) byteToReceive[0] << ":" << byteToReceive[0] << logRight << flush;
                    mySM->postEvent(SER, byteToReceive[0]);
                }
            }
		}
	}
//		smLogFile.close();
}

// returns microseconds elapsed since this peer was constructed (within 1 second)
uint32_t PeerX::elapsed_usecs()
{
	struct timeval tvNow;
	PE(gettimeofday(&tvNow, NULL));
	/*_CSTD */ time_t	tv_sec = tvNow.tv_sec;
	return (tv_sec - sec_start) * MILLION + tvNow.tv_usec; // casting needed?
}

/*
set a timeout time at an absolute time timeoutUnits into
the future. That is, determine an absolute time to be used
for the next one or more XMODEM timeouts by adding
timeoutUnits to the elapsed time.
*/
void PeerX::tm(int timeoutUnits)
{
	absoluteTimeout = elapsed_usecs() + timeoutUnits * uSECS_PER_UNIT;
}

/* make the absolute timeout earlier by reductionUnits */
void PeerX::tmRed(int unitsToReduce)
{
	absoluteTimeout -= (unitsToReduce * uSECS_PER_UNIT);
}

/*
Store the current absolute timeout, and create a temporary
absolute timeout timeoutUnits into the future.
*/
void PeerX::tmPush(int timeoutUnits)
{
	holdTimeout = absoluteTimeout;
	absoluteTimeout = elapsed_usecs() + timeoutUnits * uSECS_PER_UNIT;
}

/*
Discard the temporary absolute timeout and revert to the
stored absolute timeout
*/
void PeerX::tmPop()
{
	absoluteTimeout = holdTimeout;
}

