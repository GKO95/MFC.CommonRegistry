#pragma once
#include <afxwin.h>
#include <stdint.h>

// EDIT BYTE SIZE: 2, 4, OR 8-BYTE
#define CREG_BYTE_SIZE		4

// REGISTRY DATA
#if CREG_BYTE_SIZE == 8
typedef uint64_t CReg;
#define CREG_BYTE8_NULL		0x00 << CREG_BYTE_SIZE * 7
#define CREG_BYTE7_NULL		0x00 << CREG_BYTE_SIZE * 6
#define CREG_BYTE6_NULL		0x00 << CREG_BYTE_SIZE * 5
#define CREG_BYTE5_NULL		0x00 << CREG_BYTE_SIZE * 4

#define CREG_INFO_BYTE8		7
#define CREG_INFO_BYTE7		6
#define CREG_INFO_BYTE6		5
#define CREG_INFO_BYTE5		4

#define CREG_BYTE_SIZE_VALID
#endif

#if	CREG_BYTE_SIZE == 4 || defined CREG_BYTE_SIZE_VALID
#ifndef CREG_BYTE_SIZE_VALID
typedef uint32_t CReg;
#endif

#define CREG_BYTE4_NULL		0x00 << CREG_BYTE_SIZE * 3
#define CREG_BYTE3_NULL		0x00 << CREG_BYTE_SIZE * 2

#define CREG_INFO_BYTE4		3
#define CREG_INFO_BYTE3		2

#define CREG_BYTE_SIZE_VALID
#endif

#if	CREG_BYTE_SIZE == 2 || defined CREG_BYTE_SIZE_VALID
#ifndef CREG_BYTE_SIZE_VALID
typedef uint16_t CReg;
#endif

#define CREG_BYTE2_NULL		0x00 << CREG_BYTE_SIZE * 1
#define CREG_BYTE1_NULL		0x00 << CREG_BYTE_SIZE * 0

#define CREG_INFO_BYTE2		1
#define CREG_INFO_BYTE1		0

#define CREG_BYTE_SIZE_VALID
#endif

#ifndef CREG_BYTE_SIZE_VALID
#error Invalid "CREG_BYTE_SIZE" macro definition.
#endif
#undef CREG_BYTE_SIZE_VALID

class CRegistry
{
public:
	CRegistry();
	~CRegistry();

	LSTATUS lResult;

private:
	HKEY 	m_hRegKey;
	DWORD 	dwDisposition;
	DWORD 	dwRegType;
	DWORD 	dwConfig;

	CString	program;
	CReg	m_hConfig;

// SETTER MEMBERS
public:
	void operator() (CString programName);

	void SetRegBytes(UINT creg_info, UINT byte);
	void fromArray(BYTE *arr);

private:
	void _SetRegistry();

// GETTER MEMBERS
public:
	BOOL IsCreatedNew();

	HKEY returnHKEY();
	DWORD returnTYPE();
	DWORD returnSIZE();

	void asArray(BYTE* arr);

	CReg GetRegBytes(UINT creg_info);
	BYTE GetRegBytewise(UINT creg_info);
};