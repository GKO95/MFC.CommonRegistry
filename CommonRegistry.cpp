#include "pch.h"
#include "CommonRegistry.h"

/*========================================================================================================================
// CONSTRUCTOR & DESTRUCTOR
========================================================================================================================*/
CRegistry::CRegistry()
	: dwRegType(REG_BINARY), dwConfig(CREG_BYTE_SIZE)
{

}

CRegistry::~CRegistry() {
	// CLOSE REGISTRY
	lResult = RegCloseKey(m_hRegKey);
	m_hRegKey = NULL;

	if (lResult != ERROR_SUCCESS)
		AfxMessageBox(_T("WARNING: Failed to close registry!"));
}

/*========================================================================================================================
// OVERRIDDEN
========================================================================================================================*/
void CRegistry::operator() (CString programName) {

	program = programName;

	// CREATE (OPEN if already exist) REGISTRY
	lResult = RegCreateKeyExW(
		// registry HKEY location
		HKEY_CURRENT_USER,
		// registry subkey location
		L"Software\\" + program,
		NULL, NULL,							// fixed setting
		REG_OPTION_NON_VOLATILE,			// registry option: non-volatile memory
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,	// registry access right & set 64-bit registry
		NULL,								// security option whether registry handle can be inherited
		&m_hRegKey,							// pointer to a variable for registry handle
		&dwDisposition						// pointer to return disposition information
	);

	if (lResult != ERROR_SUCCESS)
		AfxMessageBox(_T("ERROR: Failed to open registry!"));

	BYTE byteRegConfig[CREG_BYTE_SIZE];
	if (dwDisposition == REG_CREATED_NEW_KEY) {
		m_hConfig = 
#if CREG_BYTE_SIZE > 4
			CREG_BYTE8_NULL | CREG_BYTE7_NULL | 
			CREG_BYTE6_NULL | CREG_BYTE5_NULL |
#endif
#if CREG_BYTE_SIZE > 2
			CREG_BYTE4_NULL | CREG_BYTE3_NULL |
#endif
			CREG_BYTE2_NULL | CREG_BYTE1_NULL;

		asArray(byteRegConfig);

		// CREATE CONFIG REGISTRY
		lResult = RegSetValueExW(
			m_hRegKey,				// registry handle
			L"Config",				// registry value name (aka. key)
			NULL,					// fixed setting
			dwRegType,				// registry type
			byteRegConfig,			// pointer to registry value
			dwConfig				// registry value byte size
		);

		if (lResult != ERROR_SUCCESS) {
			AfxMessageBox(_T("ERROR: Failed to create built-in setting!"));
		}
	}
	else if (dwDisposition == REG_OPENED_EXISTING_KEY) {

		// QUERY CONFIG REGISTRY
		lResult = RegQueryValueExW(
			m_hRegKey,				// registry handle
			L"Config",				// registry value name (aka. key)
			NULL,
			&dwRegType,
			byteRegConfig,
			&dwConfig
		);

		fromArray(byteRegConfig);
	}

}

/*========================================================================================================================
// SETTER
========================================================================================================================*/
void CRegistry::SetRegBytes(UINT creg_info, UINT byte) {

	switch (creg_info) {
#if	CREG_BYTE_SIZE > 4
	case(CREG_INFO_BYTE8):
		m_hConfig = (m_hConfig & 0x00FFFFFFFFFFFFFF) | byte;
		break;
	case(CREG_INFO_BYTE7):
		m_hConfig = (m_hConfig & 0xFF00FFFFFFFFFFFF) | byte;
		break;
	case(CREG_INFO_BYTE6):
		m_hConfig = (m_hConfig & 0xFFFF00FFFFFFFFFF) | byte;
		break;
	case(CREG_INFO_BYTE5):
		m_hConfig = (m_hConfig & 0xFFFFFF00FFFFFFFF) | byte;
		break;
#endif
#if CREG_BYTE_SIZE > 2
	case(CREG_INFO_BYTE4):
		m_hConfig = (m_hConfig & 0xFFFFFFFF00FFFFFF) | byte;
		break;
	case(CREG_INFO_BYTE3):
		m_hConfig = (m_hConfig & 0xFFFFFFFFFF00FFFF) | byte;
		break;
#endif
	case(CREG_INFO_BYTE2):
		m_hConfig = (m_hConfig & 0xFFFFFFFFFFFF00FF) | byte;
		break;
	case(CREG_INFO_BYTE1):
	default:
		m_hConfig = (m_hConfig & 0xFFFFFFFFFFFFFF00) | byte;
		break;
	}

	_SetRegistry();
}

void CRegistry::fromArray(BYTE* arr) {
	m_hConfig = 
#if CREG_BYTE_SIZE > 4
		(arr[CREG_BYTE_SIZE - 8] << CREG_BYTE_SIZE * 8) |
		(arr[CREG_BYTE_SIZE - 7] << CREG_BYTE_SIZE * 7) |
		(arr[CREG_BYTE_SIZE - 6] << CREG_BYTE_SIZE * 6) |
		(arr[CREG_BYTE_SIZE - 5] << CREG_BYTE_SIZE * 5) |
#endif
#if CREG_BYTE_SIZE > 2
		(arr[CREG_BYTE_SIZE - 4] << CREG_BYTE_SIZE * 3) |
		(arr[CREG_BYTE_SIZE - 3] << CREG_BYTE_SIZE * 2) |
#endif
		(arr[CREG_BYTE_SIZE - 2] << CREG_BYTE_SIZE * 1) |
		(arr[CREG_BYTE_SIZE - 1] << CREG_BYTE_SIZE * 0);
}

void CRegistry::_SetRegistry() {

	BYTE arr[4];
	asArray(arr);
	
	lResult = RegSetValueExW(
		m_hRegKey,
		L"Config",
		NULL,
		dwRegType,
		arr,
		dwConfig
	);

	if (lResult != ERROR_SUCCESS) {
		AfxMessageBox(_T("ERROR: Failed to save registry!"));
	}
}

/*========================================================================================================================
// GETTER
========================================================================================================================*/
BOOL CRegistry::IsCreatedNew() {
	if (dwDisposition == REG_CREATED_NEW_KEY)
		return TRUE;
	else
		return FALSE;
}

HKEY CRegistry::returnHKEY() {
	return m_hRegKey;
}

DWORD CRegistry::returnTYPE() {
	return dwRegType;
}

DWORD CRegistry::returnSIZE() {
	return dwConfig;
}


void CRegistry::asArray(BYTE* arr) {
#if CREG_BYTE_SIZE > 4
	arr[CREG_BYTE_SIZE - 8] = GetRegBytewise(CREG_INFO_BYTE8);
	arr[CREG_BYTE_SIZE - 7] = GetRegBytewise(CREG_INFO_BYTE7);
	arr[CREG_BYTE_SIZE - 6] = GetRegBytewise(CREG_INFO_BYTE6);
	arr[CREG_BYTE_SIZE - 5] = GetRegBytewise(CREG_INFO_BYTE5);
#endif
#if CREG_BYTE_SIZE > 2
	arr[CREG_BYTE_SIZE - 4] = GetRegBytewise(CREG_INFO_BYTE4);
	arr[CREG_BYTE_SIZE - 3] = GetRegBytewise(CREG_INFO_BYTE3);
#endif
	arr[CREG_BYTE_SIZE - 2] = GetRegBytewise(CREG_INFO_BYTE2);
	arr[CREG_BYTE_SIZE - 1] = GetRegBytewise(CREG_INFO_BYTE1);
}


CReg CRegistry::GetRegBytes(UINT creg_info) {

	CReg bytes;

	switch (creg_info) {
#if CREG_BYTE_SIZE > 4
	case(CREG_INFO_BYTE8):
		bytes = m_hConfig & 0xFF00000000000000;
		break;
	case(CREG_INFO_BYTE7):
		bytes = m_hConfig & 0x00FF000000000000;
		break;
	case(CREG_INFO_BYTE6):
		bytes = m_hConfig & 0x0000FF0000000000;
		break;
	case(CREG_INFO_BYTE5):
		bytes = m_hConfig & 0x000000FF00000000;
		break;
#endif
#if CREG_BYTE_SIZE > 2
	case(CREG_INFO_BYTE4):
		bytes = m_hConfig & 0x00000000FF000000;
		break;
	case(CREG_INFO_BYTE3):
		bytes = m_hConfig & 0x0000000000FF0000;
		break;
#endif
	case(CREG_INFO_BYTE2):
		bytes = m_hConfig & 0x000000000000FF00;
		break;
	case(CREG_INFO_BYTE1):
	default:
		bytes = m_hConfig & 0x00000000000000FF;
		break;
	}
	return bytes;
}

BYTE CRegistry::GetRegBytewise(UINT creg_info) {

	CReg bytes = GetRegBytes(creg_info);
	BYTE byte = bytes >> 8 * (creg_info);

	return byte;
}