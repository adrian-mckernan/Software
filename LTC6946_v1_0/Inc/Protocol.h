#ifndef PROTOCOL_H
#define PROTOCOL_H


#define startLoc 0
#define nodeLoc 2
#define funcLowLoc 8
#define funcHighLoc 10
#define valueLoc 14
#define endLoc 30
#define startMask 0x00000003
#define nodeMask 0x000000FC
#define funcLowMask 0x00000300
#define funcHighMask 0x00003C00
#define valueMask 0x3FFFC000
#define endMask 0xC0000000
#define startVal 3u << startLoc
#define endVal 2u << endLoc
#define RegReadPKT 0u << funcLowLoc
#define RegWritePKT 2u << funcLowLoc
#define funPKT 1u << funcLowLoc
#define initalisePKT 1u << funcHighLoc
#define phasePKT 2u << funcHighLoc
#define powerPKT 4u << funcHighLoc
#define mutePKT 8u << funcHighLoc


#endif  // PROTOCOL_H
