/***************************************************************
|	File:		MessageID.h
|	Author:		Jason Bloomfield
|	Course:		N/A
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H


/**************************************************************/
// MessageID enum
//	- enumerated list of message identifiers
enum class MessageID 
{ 
	MSG_UNKNOWN, 
	MSG_DESTROY_OBJECT,
	MSG_CREATE_BULLET,
};


#endif //MESSAGEID_H