#pragma once
#include <afxwin.h>
#include <stdint.h>

// EDIT BYTE SIZE: 2, 4, OR 8-BYTE
#define CREG_BYTE_SIZE		4

#define CREG_BYTE_NULL		0x00

// REGISTRY DATA
#if CREG_BYTE_SIZE == 8
typedef uint64_t CReg;

#define CREG_INFO_BYTE8		7
#define CREG_INFO_BYTE7		6
#define CREG_INFO_BYTE6		5
#define CREG_INFO_BYTE5		4

///<summary>CommonRegistry::CONVERT ARRAY TO REGISTRY DATA</summary>
#if !defined(ARR2REG) && !defined(REG2ARR)
#define ARR2REG(x)			(m_hConfig = ((x)[0] << 56)|\
										 ((x)[1] << 48)|\
										 ((x)[2] << 40)|\
										 ((x)[3] << 32)|\
										 ((x)[4] << 24)|\
										 ((x)[5] << 16)|\
										 ((x)[6] <<  8)|\
										 ((x)[7] <<  0))

///<summary>CommonRegistry::CONVERT REGISTRY DATA TO ARRAY</summary>
#define REG2ARR(x)			(x[0] = (m_hConfig & 0xFF00000000000000) >> 56);\
							(x[1] = (m_hConfig & 0x00FF000000000000) >> 48);\
							(x[2] = (m_hConfig & 0x0000FF0000000000) >> 40);\
							(x[3] = (m_hConfig & 0x000000FF00000000) >> 32);\
							(x[4] = (m_hConfig & 0x00000000FF000000) >> 24);\
							(x[5] = (m_hConfig & 0x0000000000FF0000) >> 16);\
							(x[6] = (m_hConfig & 0x000000000000FF00) >>  8);\
							(x[7] = (m_hConfig & 0x00000000000000FF) >>  0)
#endif

#define CREG_BYTE_SIZE_VALID
#endif

#if	CREG_BYTE_SIZE == 4 || defined CREG_BYTE_SIZE_VALID
#ifndef CREG_BYTE_SIZE_VALID
typedef uint32_t CReg;
#endif

#define CREG_INFO_BYTE4		3
#define CREG_INFO_BYTE3		2

///<summary>CommonRegistry::CONVERT ARRAY TO REGISTRY DATA</summary>
#if !defined(ARR2REG) && !defined(REG2ARR)
#define ARR2REG(x)			(m_hConfig = ((x)[0] << 24)|\
										 ((x)[1] << 16)|\
										 ((x)[2] <<  8)|\
										 ((x)[3] <<  0))

///<summary>CommonRegistry::CONVERT REGISTRY DATA TO ARRAY</summary>
#define REG2ARR(x)			(x[0] = (m_hConfig & 0xFF000000) >> 24);\
							(x[1] = (m_hConfig & 0x00FF0000) >> 16);\
							(x[2] = (m_hConfig & 0x0000FF00) >>  8);\
							(x[3] = (m_hConfig & 0x000000FF) >>  0)
#endif

#define CREG_BYTE_SIZE_VALID
#endif

#if	CREG_BYTE_SIZE == 2 || defined CREG_BYTE_SIZE_VALID
#ifndef CREG_BYTE_SIZE_VALID
typedef uint16_t CReg;
#endif

#define CREG_INFO_BYTE2		1
#define CREG_INFO_BYTE1		0

///<summary>CommonRegistry::CONVERT ARRAY TO REGISTRY DATA</summary>
#if !defined(ARR2REG) && !defined(REG2ARR)
#define ARR2REG(x)			(m_hConfig = ((x)[0] << 8)|\
										 ((x)[1] << 0))


///<summary>CommonRegistry::CONVERT REGISTRY DATA TO ARRAY</summary>
#define REG2ARR(x)			(x[0] = (m_hConfig & 0xFF00) >> 8);\
							(x[1] = (m_hConfig & 0x00FF) >> 0);\

#endif

#define CREG_BYTE_SIZE_VALID
#endif

#ifndef CREG_BYTE_SIZE_VALID
#error Invalid "CREG_BYTE_SIZE" macro definition.
#endif
#undef CREG_BYTE_SIZE_VALID

#define WREG_BYTE_MSB		1
#define WREG_BYTE_LSB		0

///<summary>CommonRegistry::CLASS</summary>
class CRegistry
{
public:
	CRegistry();
	~CRegistry();

	LSTATUS lResult;
	BOOL	bRenewed;

private:
	///<summary>CommonRegistry::REGISTRY KEY</summary>
	HKEY 	m_hRegKey;
	///<summary>CommonRegistry::REGISTRY DISPOSITION</summary>
	DWORD 	dwDisposition;
	///<summary>CommonRegistry::REGISTRY TYPE</summary>
	DWORD 	dwRegType;
	///<summary>CommonRegistry::REGISTRY SIZE</summary>
	DWORD 	dwSize;

	///<summary>CommonRegistry::REGISTRY DATA</summary>
	CReg	m_hConfig;

// SETTER MEMBERS
public:
	void operator() (CString programName);

	void SaveRegistry();
	BOOL LoadRegistry();

// SETTER MEMBERS
#pragma region SETTER MEMBERS
public:
	void 	SetRegBytes(UINT creg_info, UINT byte);
	void 	SetReg4Bits(UINT creg_info, UINT byte_info, UINT bits);

#pragma endregion

// GETTER MEMBERS
#pragma region GETTER MEMBERS
public:
	BOOL	IsCreatedNew();
	HKEY	returnHKEY();
	DWORD	returnTYPE();
	DWORD	returnSIZE();

	BYTE    GetRegBytes(UINT creg_info);
	BYTE	GetReg4Bits(UINT creg_info, UINT byte_info);
};