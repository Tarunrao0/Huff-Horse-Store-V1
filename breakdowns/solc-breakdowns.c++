//0x6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220ee718384a20a40f58b676e7f7ba7521f229a5f787acec88f60bea1052b0d150d64736f6c63430008140033

// Solidity has this special feature where it uses the 0x40 memory slot to indicate a free space in the memory
// So whatever is stored in the 0x40 is the offset of the free memory slot

PUSH1 0x80      //[0x80]
PUSH1 0x40      //[0x40,0x80]
MSTORE          //[]            //memory 0x40->0x80

CALLVALUE       //[msg.value]
DUP1            //[msg.value, msg.value]
ISZERO          //[msg.value==0, msg.value]
PUSH1 0x0e      //[0x0e, msg.value==0, msg.value]
JUMPI           //[msg.value]
PUSH0           //[0x00, msg.value]
DUP1            //[0x00, 0x00, msg.value]
REVERT          //[msg.value]   //reverts if any wei was sent because non payable

//Makes sure no wei is sent since its not needed

JUMPDEST        //[msg.value]   //jumps here from JUMPI (0x0e)
POP             //[]
PUSH1 0xa5      //[0xa5]        //165
DUP1            //[0xa5, 0xa5]
PUSH2 0x001b    //[0x001b, 0xa5, 0xa5]      //27
PUSH0           //[0x00, 0x001b, 0xa5, 0xa5]    //[destOffset, offset, size (bytes size to copy)]
CODECOPY        //[0xa5]                        //memory:[runtime code]
PUSH0           //[0x00, 0xa5]
RETURN          //[]
INVALID

//allocate space for the runtime code. Contract creation completed

PUSH1 0x80      //[0x80]
PUSH1 0x40      //[0x40,0x80]
MSTORE          //[]            //memory 0x40->0x80
CALLVALUE       //[msg.value]
DUP1            //[msg.value, msg.value]
ISZERO          //[msg.value==0, msg.value]
PUSH1 0x0e      //[0x0e, msg.value==0, msg.value]
JUMPI           //[msg.value]

PUSH0           //[0x00, msg.value]
DUP1            //[0x00, 0x00, msg.value]
REVERT          //[msg.value]   //reverts if any wei was sent because non payable

//jump here!!
JUMPDEST        //[msg.value]   
POP             //[]
PUSH1 0x04      //[0x04]
CALLDATASIZE    //[calldata_size, 0x04]
LT              //[calldata_size<0x04]      //checks if calldata size is lesser than 4 bytes (function selector!)
PUSH1 0x30      //[0x30, calldata_size<0x04]    //0x30 counter, jumpdest
JUMPI           //[]

//function selector->0xcdfead2e

PUSH0           //[0x00]
CALLDATALOAD    //[big_function_selector]
PUSH1 0xe0      //[0xe0, big_function_selector]
SHR             //[function_selector]
DUP1            //[function_selector, function_selector]
PUSH4 0xcdfead2e//[0xcdfead2e, function_selector, function_selector]    
EQ              //[true/false, function_selector]
PUSH1 0x34      //[0x34, true/false, function_selector]
JUMPI           //[function selector]

//function selector->0xe026c017

DUP1            //[function selector, function selector]
PUSH4 0xe026c017//[0xe026c017, function selector, function selector]
EQ              //[true/false, function_selector]
PUSH1 0x45      //[0X45, function selector]
JUMPI           //[function selector]

//No function selector then REVERT

JUMPDEST        //[]
PUSH0           //[0x00]
DUP1            //[0x00, 0x00]
REVERT          //[]

//JUMPA
JUMPDEST        //[function selector]
PUSH1 0x43      //[0x43, function selector]        //67
PUSH1 0x3f      //[0x3f, 0x43, function selector]  //63
CALLDATASIZE    //[calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x04      //[0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x59      //[0x59, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMP            //[0x04, calldata_size, 0x3f, 0x43, function selector]

//jump c
JUMPDEST        //[calldataAfter4bytesOffset, 0x43 , function selector]
PUSH0           //[0x00, calldataAfter4bytesOffset, 0x43, function selector]
SSTORE          //[0x43, function selector]
JUMP            //[function selector]

JUMPDEST        //[function selector]
STOP        


JUMPDEST        //[function selector]
PUSH0           //[0x00, function selector]
SLOAD           //[numOfHorses, function selector]
PUSH1 0x40      //[0x40, numOfHorses, function selector]
MLOAD           //[0x80, numOfHorses, function selector]        Memory: 0x40
SWAP1           //[numOfHorses, 0x80, function selector]
DUP2            //[0x80, numOfHorses, 0x80, function selector]
MSTORE          //[0x80, function selector]
PUSH1 0x20      //[0x20, 0x80, function selector]
ADD             //[0xa0, function selector]
PUSH1 0x40      //[0x40, 0xa0, function selector]
MLOAD           //[0x80, 0xa0, function selector]
DUP1            //[0x80,0x80, 0xa0, function selector]
SWAP2           //[0x80, 0x80, 0xa0, function selector]
SUB             //[0x20, 0x80, function selector]
SWAP1           //[0x80, 0x20, function selector]
RETURN          //[function selector]                       returns the number of horses

//jumpA
//JUMPS if theres additional calldata. This means that a parameter was given to update the horse number
//If just given function selector it reverts

JUMPDEST    //[0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH0       //[0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x20  //[0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP3        //[0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP5        //[calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
SUB         //[calldata_size-0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
//Checks if there is more to the calldata => calldata+data(32bytes)
SLT         //[calldata_size-0x04 < 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
//will be zero if it is a function selector EG: size(0x28e546ef) - 4 < 32 = 0
ISZERO      //[calldata_size-0x04 < 0x20 = 0, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x68  //[0x68, true/false,0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMPI       //[0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
//JUMPB

PUSH0       //[0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43]
DUP1        //[0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43]
REVERT      //[0x00, 0x04, calldata_size, 0x3f, 0x43]

//jump b
JUMPDEST    //[0x00, 0x04, calldata_size, 0x3f, 0x43]
POP         //[0x04, calldata_size, 0x3f, 0x43]
CALLDATALOAD//[calldataAfter4bytesOffset, calldata_size, 0x3f, 0x43 ]
SWAP2       //[0x3f, calldata_size, calldataAfter4bytesOffset, 0x43 ]
SWAP1       //[calldata_size, 0x3f, calldataAfter4bytesOffset, 0x43 ]
POP         //[0x3f, calldataAfter4bytesOffset, 0x43 ]
JUMP        //[calldataAfter4bytesOffset, 0x43 ]
//JUMP C

//3. Metadata
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
SHA3
INVALID
PUSH18 0x8384a20a40f58b676e7f7ba7521f229a5f78
PUSH27 0xcec88f60bea1052b0d150d64736f6c63430008140033