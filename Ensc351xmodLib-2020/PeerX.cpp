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

//P BEGIN
//	/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
//	   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
//	   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
//	   after waiting the interval specified therein.  Returns the number of ready
//	   descriptors, or -1 for errors.
//
//	   This function is a cancellation point and therefore not marked with
//	   __THROW.  */
//	extern int select (int __nfds, fd_set *__restrict __readfds,
//	           fd_set *__restrict __writefds,
//	           fd_set *__restrict __exceptfds,
//	           struct timeval *__restrict __timeout);
//    int rv = PE(select( max(mediumD,inD)+1, &set, NULL, NULL, NULL ));
//P END
	//      fd_set set;
	//      FD_ZERO(&set);
	//      FD_SET(mediumD, &set);
	//      FD_SET(inD, &set);
	//
	//      int rv = PE(select( max(mediumD,
	//                        inD)+1, &set, NULL, NULL, NULL ));
	/*P
	        fd_set set;
	        FD_ZERO(&set);
	        FD_SET(mediumD, &set);
	        FD_SET(consoleInId, &set); //P consoleInId is input from the console I think
	//      int max_fd_t = max(d[TERM1], d[TERM2]);
	//      int max_fd = max(max_fd_t, STDIN_FILENO)+1;
	//      int rv = PE(select(max_fd, &set, NULL, NULL, NULL));
	//      FD_ISSET(STDIN_FILENO, &set);

	         *  SOMEHOW Determine what type of input is coming in, probably relying on the select function
	         *
	        fd_set set;
	        FD_ZERO(&set); // Resets "set"'s bits to zero
	        input this somewhere in this loop to check:
	        int rv = PE(select( max(mediumD,inD)+1, &set, NULL, NULL, NULL ));
	        "Hey, select function" was there input from the keyboard, medium descriptor or was there a timeout?"
	        */

	//        int max_fd_t = max(d[TERM1], d[TERM2]);
	//        int max_fd = max(max_fd_t, STDIN_FILENO)+1;

	//        FD_SET(consoleInId, &set); //P consoleInId is input from the console I think

	struct timeval tv;

	while(mySM->isRunning()) {
		// ************* this loop is going to need more work ************
		tv.tv_sec=0;
		tv.tv_usec=0;//p from Debug menu tv_usec is included

//        //p Check current bits
//        cout << "Current input bit is! " << set.fds_bits[0] << endl;
//        if(set.fds_bits[0] == 8){
//               cout << "transferCommon: &s" << endl;
//        }
//        else if(set.fds_bits[0] == 32){
//               cout << "transferCommon: &r" << endl;
//        }

        fd_set set;
        FD_ZERO(&set);
        FD_SET(mediumD, &set); //p Add mediumD to set
        FD_SET(consoleInId, &set); //p Add console input as a set



		uint32_t now = elapsed_usecs();
        if (now >= absoluteTimeout) {
            //...
            //P We are leaving due to timeout, what do we reset??!
            mySM->postEvent(TM);
        } else {
            // ...
            tv.tv_usec = absoluteTimeout - now; //p Relative time
            int max_fd = max(mediumD, consoleInId)+1;
            int rv = PE(select( max_fd, &set, NULL, NULL, &tv ));
//            cout << "Selects input RV is " << rv << endl;
            //P Determine where the input is coming from?
//            /****/ {
            // If the mediumD has something to read from
            if(FD_ISSET(mediumD, &set)) {
                //read character from medium
				char byteToReceive;
				PE_NOT(myReadcond(mediumD, &byteToReceive, 1, 1, 0, 0), 1); // data should be available right //P This is waiting forever... is it supposed to wait forever?!?
				//PE_NOT(myRead(mediumD, &byteToReceive, 1), 1);

				if (reportInfo)
					COUT << logLeft << 1.0*(absoluteTimeout - now)/MILLION << ":" << (int)(unsigned char) byteToReceive << ":" << byteToReceive << logRight << flush;
				mySM->postEvent(SER, byteToReceive);
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

