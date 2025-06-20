#pragma once

// Transmission

// TXn Buffer control
#define TXB0CTRL				0x30
#define TXB1CTRL				0x40
#define TXB2CTRL				0x50
// RW - Priority between buffers (0 - lowest, 3 - highest)
#define TXP0					0
#define TXP1					1
// RW - Transmit request
#define TXREQ					3
// R - A bus error occurred
#define TXERR					4
// R - Message lost arbitration
#define MLOA					5
// R - Message was aborted
#define ABTF					6

// TX Pins functionality
#define TXRTSCTRL				0x0D
// RW - Pin Mode: 0 - Digital Input, 1 - Request Transmission
#define B0RTSM					0
#define B1RTSM					1
#define B2RTSM					2
// R - Pin state when Digital Input
#define B0RTS					3
#define B1RTS					4
#define B2RTS					5

// TX Standard Identifier High bits [10..3]
#define TXB0SIDH				0x31
#define TXB1SIDH				0x41
#define TXB2SIDH				0x51

// TX Standard Identifier Low bits [2..0], Extended [17..16]
#define TXB0SIDL				0x32
#define TXB1SIDL				0x42
#define TXB2SIDL				0x52
// RW - Extended Identifier Enable
#define EXIDE					3

// TX Extended Identifier High bits [15..8]
#define TXB0EID8				0x33
#define TXB1EID8				0x43
#define TXB2EID8				0x53

// TX Extended Identifier Low bits [7..0]
#define TXB0EID0				0x34
#define TXB1EID0				0x44
#define TXB2EID0				0x54

// TX Data Length
#define TXB0DLC					0x35
#define TXB1DLC					0x45
#define TXB2DLC					0x55
// RW - Remote Transmission Request
#define RTR						6

// TX Data
#define TXB0D0					0x36
#define TXB1D0					0x46
#define TXB2D0					0x56

// Reception

// RX0 Buffer control
#define RXB0CTRL				0x60
// R - Indicates which acceptance filter enabled reception of message (0-1)
#define FILHIT0					0
// RW - Enables roll over RX0 -> RX1
#define BUKT					2
// R - Indicates wherever Remote Transfer Request was received
#define RXRTR					3
// RW - 11 - Turns masks/filters off. 00 - Enables them
#define RXM0					5
#define RXM1					6

// RX1 Buffer control
#define RXB1CTRL				0x70
// R - Indicates which acceptance filter enabled reception of message (0-5). Filters 0 and 1 can appear here only when roll over is enabled
#define FILHIT0					0
#define FILHIT1					1
#define FILHIT2					2
// R - Indicates wherever Remote Transfer Request was received
#define RXRTR					3
// RW - 11 - Turns masks/filters off. 00 - Enables them
#define RXM0					5
#define RXM1					6

// RX Pins functionality
#define BFPCTRL					0x0C
// RW - Pin Mode: 0 - Digital Output, 1 - Interrupt Output
#define B0BFM					0
#define B1BFM					1
// RW - Pin Function Enable (0 - Disabled, high-impedance)
#define B0BFE					2
#define B1BFE					3
// RW - Pin State when Digital Output
#define B0BFS					4
#define B1BFS					5

// RX Standard Identifier High bits [10..3]
#define RXB0SIDH				0x61
#define RXB1SIDH				0x71

// RX Standard Identifier Low bits [2..0], Extended [17..16]
#define RXB0SIDL				0x62
#define RXB1SIDL				0x72
// R - Extended Identifier Flag
#define IDE						3
// R - Remote Transfer Request (for Standard Identifiers only)
#define SRR						4

// RX Extended Identifier High bits [15..8]
#define RXB0EID8				0x63
#define RXB1EID8				0x73

// RX Extended Identifier Low bits [7..0]
#define RXB0EID0				0x64
#define RXB1EID0				0x74

// RX Data Length
#define RXB0DLC					0x65
#define RXB1DLC					0x75
// R - Remote Transfer Request (for Extended Identifiers only)
#define RTR						6

// RX Data
#define RXB0D0					0x66
#define RXB1D0					0x76

// Filtering

// Standard Identifier Filter High bits
#define RXF0SIDH				0x00
#define RXF1SIDH				0x04
#define RXF2SIDH				0x08
#define RXF3SIDH				0x10
#define RXF4SIDH				0x14
#define RXF5SIDH				0x18

// Standard Identifier Filter Low bits
#define RXF0SIDL				0x01
#define RXF1SIDL				0x05
#define RXF2SIDL				0x09
#define RXF3SIDL				0x11
#define RXF4SIDL				0x15
#define RXF5SIDL				0x19
// RW - Extended Identifier Enable
#define EXIDE					3

// Extended Identifier Filter High bits
#define RXF0EID8				0x02
#define RXF1EID8				0x06
#define RXF2EID8				0x0A
#define RXF3EID8				0x12
#define RXF4EID8				0x16
#define RXF5EID8				0x1A

// Extended Identifier Filter Low bits
#define RXF0EID0				0x03
#define RXF1EID0				0x07
#define RXF2EID0				0x0B
#define RXF3EID0				0x13
#define RXF4EID0				0x17
#define RXF5EID0				0x1B

// Standard Identifier Mask High bits
#define RXM0SIDH				0x20
#define RXM1SIDH				0x24

// Standard Identifier Mask Low bits
#define RXM0SIDL				0x21
#define RXM1SIDL				0x25

// Extended Identifier Mask High bits
#define RXM0EID8				0x22
#define RXM1EID8				0x26

// Extended Identifier Mask Low bits
#define RXM0EID0				0x23
#define RXM1EID0				0x27

// Bit Timing

#define CNF1					0x2A
// Baud Rate Prescaler
#define BRP0					0
// Synchronization Jump Width Length
#define SJW0					6

#define CNF2					0x29
// Propagation Segment Length
#define PRSEG0					0
// PS1 Length
#define PHSEG10					3
// When 1 - Bus line is sampled three times. When 0 - once
#define SAM						6
// PS2 Bit Time Length. When 0, length is greater of PS1 and 2. When 1 - configured separately
#define BTLMODE					7

#define CNF3					0x28
// PS2 Length
#define PHSEG20					0
// Wake-up low-pass filter is enabled (prevents from waking up due to short glitches o the bus)
#define WAKFIL					6
// Start-of-Frame signal on out pin. When 1 - SOF signal. When 0 - Clock Out.
#define SOF						7

// Error detection

// R - Transmit Error Counter
#define TEC						0x1C

// R - Receive Error Counter
#define REC						0x1D

// Error flags
#define EFLG					0x2D
// R - Different warnings when counters achieve different values
#define EWARN					0
#define RXWAR					1
#define TXWAR					2
#define RXEP					3
#define TXEP					4
#define TXBO					5
// RW - Receive Buffer Overflow Flag
#define RX0OVR					6
#define RX1OVR					7

// Interrupts

// Interrupt enable
#define CANINTE					0x2B
// RW - Interrupt when message was received
#define RX0IE					0
#define RX1IE					1
// RW - Interrupt on TX buffer becoming empty
#define TX0IE					2
#define TX1IE					3
#define TX2IE					4
// RW - Interrupt on EFLG error condition change (e.g. overflow, see error flags in that register)
#define ERRIE					5
// RW - Interrupt on CAN bus activity
#define WAKIE					6
// RW - Interrupt on error during message reception or transmission
#define MERRE					7

// Interrupt flags
#define CANINTF					0x2C
#define RX0IF					0
#define RX1IF					1
#define TX0IF					2
#define TX1IF					3
#define TX2IF					4
#define ERRIF					5
#define WAKIF					6
#define MERRF					7

// Modes of operation

// CAN Control
#define CANCTRL					0x0F
// RW - CLK out pin prescaler
#define CLKPRE0					0
#define CLKPRE1					1
// RW - Enable CLK out pin
#define CLKEN					2
// RW - One-Shot Mode (when enabled, messages will only attempt to transmit one time)
#define OSM						3
// RW - Requests abort of all pending transmit buffers
#define ABAT					4
// RW - Operation Mode
#define REQOP0					5

// CAN Status
#define CANSTAT					0x0E
// R - Interrupt Flag Code - Indicates interrupt source (the most significant active interrupt)
#define ICOD0					0
// R - Operation Mode 
#define OPMOD0					5
