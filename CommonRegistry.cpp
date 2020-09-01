#include "pch.h"
#include "CommonRegistry.h"

//========================================================================================================================
// CONSTRUCTOR & DESTRUCTOR
//========================================================================================================================
CRegistry::CRegistry()
	: bRenewed(FALSE), dwRegType(REG_BINARY), dwSize(CREG_BYTE_SIZE), m_hConfig(0)
{

}

CRegistry::~CRegistry() {
	// CLOSE REGISTRY
	lResult = RegCloseKey(m_hRegKey);
	m_hRegKey = NULL;

	if (lResult != ERROR_SUCCESS)
		AfxMessageBox(_T("WARNING: Failed to close registry!"));
}

//========================================================================================================================
// OVERRIDDEN
//========================================================================================================================

///<summary>CommonRegistry::PROGRAM NAME</summary>
void CRegistry::operator() (CString programName) {

	// CREATE (OPEN if already exist) "programName" REGISTRY
	lResult = RegCreateKeyExW(
		// registry HKEY location
		HKEY_CURRENT_USER,
		// registry subkey location
		L"Software\\" + programName,
		NULL, NULL,							// fixed setting
		REG_OPTION_NON_VOLATILE,			// registry option: non-volatile memory
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,	// registry access right & set 64-bit registry
		NULL,								// security option whether registry handle can be inherited
		&m_hRegKey,							// pointer to a variable for registry handle
		&dwDisposition						// pointer to return disposition information
	);

	if (lResult != ERROR_SUCCESS)
		AfxMessageBox(_T("ERROR: Failed to open registry!"));
	
	if (IsCreatedNew())
	{
		SaveRegistry(); LoadRegistry();
	}
	// IF REGISTRY CONNOT BE QUERIED, THEN CREATE!
	else if (!LoadRegistry())
	{
		AfxMessageBox(_T("Config registry not found! Loading built-in registry."));
		SaveRegistry(); LoadRegistry(); bRenewed = TRUE;
		
	}
}

///<summary>CommonRegistry::SAVE REGISTRY</summary>
void CRegistry::SaveRegistry()
{
	BYTE byteRegConfig[CREG_BYTE_SIZE] = { 0 };
	dwRegType = REG_BINARY; dwSize = CREG_BYTE_SIZE;

	// SAVE or CREATE CONFIG REGISTRY
	REG2ARR(byteRegConfig);
	lResult = RegSetValueExW(
		m_hRegKey,				// registry handle
		L"Config",				// registry value name (aka. key)
		NULL,					// fixed setting
		dwRegType,				// registry type
		byteRegConfig,			// pointer to registry value
		dwSize				// registry value byte size
	);

	if (lResult != ERROR_SUCCESS) {
		AfxMessageBox(_T("ERROR: Failed to load configuration!"));
	}
}

///<summary>CommonRegistry::LOAD REGISTRY</summary>
BOOL CRegistry::LoadRegistry()
{
	BYTE byteRegConfig[CREG_BYTE_SIZE] = { 0 };
	dwRegType = REG_BINARY; dwSize = CREG_BYTE_SIZE;

	lResult = RegQueryValueExW(
		m_hRegKey,				// registry handle
		L"Config",				// registry value name (aka. key)
		NULL,
		&dwRegType,
		byteRegConfig,
		&dwSize
	); ARR2REG(byteRegConfig);

	if (lResult != ERROR_SUCCESS) return FALSE;
	else return TRUE;
}

//========================================================================================================================
// SETTER
//========================================================================================================================

///<summary>CommonRegistry::1-BYTE CONFIGURATION & SAVING</summary>
void CRegistry::SetRegBytes(UINT creg_info, UINT byte)
{
	switch (creg_info) {
#if	CREG_BYTE_SIZE > 4
	case(CREG_INFO_BYTE8):
		m_hConfig = (m_hConfig & 0x00FFFFFFFFFFFFFF) | (byte << 56);
		break;
	case(CREG_INFO_BYTE7):
		m_hConfig = (m_hConfig & 0xFF00FFFFFFFFFFFF) | (byte << 48);
		break;
	case(CREG_INFO_BYTE6):
		m_hConfig = (m_hConfig & 0xFFFF00FFFFFFFFFF) | (byte << 40);
		break;
	case(CREG_INFO_BYTE5):
		m_hConfig = (m_hConfig & 0xFFFFFF00FFFFFFFF) | (byte << 32);
		break;
#endif
#if CREG_BYTE_SIZE > 2
	case(CREG_INFO_BYTE4):
		m_hConfig = (m_hConfig & 0xFFFFFFFF00FFFFFF) | (byte << 24);
		break;
	case(CREG_INFO_BYTE3):
		m_hConfig = (m_hConfig & 0xFFFFFFFFFF00FFFF) | (byte << 16);
		break;
#endif
	case(CREG_INFO_BYTE2):
		m_hConfig = (m_hConfig & 0xFFFFFFFFFFFF00FF) | (byte <<  8);
		break;
	case(CREG_INFO_BYTE1):
	default:
		m_hConfig = (m_hConfig & 0xFFFFFFFFFFFFFF00) | (byte <<  0);
		break;
	}
	SaveRegistry();
}

///<summary>CommonRegistry::HALF BYTE CONFIGURATION & SAVING</summary>
void CRegistry::SetReg4Bits(UINT creg_info, UINT byte_info, UINT bits)
{
	switch (creg_info) {
#if CREG_BYTE_SIZE > 4
	case(CREG_INFO_BYTE8):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0x0FFFFFFFFFFFFFFF) | (bits << 60);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xF0FFFFFFFFFFFFFF) | (bits << 56);
		break;
	case(CREG_INFO_BYTE7):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFF0FFFFFFFFFFFFF) | (bits << 52);
		else if(byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFF0FFFFFFFFFFFF) | (bits << 48);
		break;
	case(CREG_INFO_BYTE6):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFF0FFFFFFFFFFF) | (bits << 44);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFF0FFFFFFFFFF) | (bits << 40);
		break;
	case(CREG_INFO_BYTE5):
	default:
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFFFF0FFFFFFFFF) | (bits << 36);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFFFF0FFFFFFFF) | (bits << 32);
		break;
#endif
#if CREG_BYTE_SIZE > 2
	case(CREG_INFO_BYTE4):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFF0FFFFFFF) | (bits << 28);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFF0FFFFFF) | (bits << 24);
		break;
	case(CREG_INFO_BYTE3):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFF0FFFFF) | (bits << 20);
		else if(byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFFF0FFFF) | (bits << 16);
		break;
#endif
	case(CREG_INFO_BYTE2):
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFFFF0FFF) | (bits << 12);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFFFFF0FF) | (bits << 8);
		break;
	case(CREG_INFO_BYTE1):
	default:
		if (byte_info == WREG_BYTE_MSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFFFFFF0F) | (bits << 4);
		else if (byte_info == WREG_BYTE_LSB)
			m_hConfig = (m_hConfig & 0xFFFFFFFFFFFFFFF0) | (bits << 0);
		break;
	}
	SaveRegistry();
}

//========================================================================================================================
// GETTER
//========================================================================================================================

///<summary>CommonRegistry::CHECK WHETHER REGKEY IS NEWLY CREATED</summary>
BOOL CRegistry::IsCreatedNew() {
	if (dwDisposition == REG_CREATED_NEW_KEY)
		return TRUE;
	else
		return FALSE;
}

///<summary>CommonRegistry::GET REGISTRY KEY</summary>
HKEY CRegistry::returnHKEY() {
	return m_hRegKey;
}

///<summary>CommonRegistry::GET REGISTRY DISPOSITION</summary>
DWORD CRegistry::returnTYPE() {
	return dwRegType;
}

///<summary>CommonRegistry::GET REGISTRY SIZE</summary>
DWORD CRegistry::returnSIZE() {
	return dwSize;
}

///<summary>CommonRegistry::GET 1-BYTE OF CONFIGURATION</summary>
BYTE CRegistry::GetRegBytes(UINT creg_info)
{
	LoadRegistry();
	BYTE byte;
	switch (creg_info) {
#if CREG_BYTE_SIZE > 4
	case(CREG_INFO_BYTE8):
		byte = (m_hConfig & 0xFF00000000000000) >> 56;
		break;
	case(CREG_INFO_BYTE7):
		byte = (m_hConfig & 0x00FF000000000000) >> 48;
		break;
	case(CREG_INFO_BYTE6):
		byte = (m_hConfig & 0x0000FF0000000000) >> 40;
		break;
	case(CREG_INFO_BYTE5):
		byte = (m_hConfig & 0x000000FF00000000) >> 32;
		break;
#endif
#if CREG_BYTE_SIZE > 2
	case(CREG_INFO_BYTE4):
		byte = (m_hConfig & 0x00000000FF000000) >> 24;
		break;
	case(CREG_INFO_BYTE3):
		byte = (m_hConfig & 0x0000000000FF0000) >> 16;
		break;
#endif
	case(CREG_INFO_BYTE2):
		byte = (m_hConfig & 0x000000000000FF00) >> 8;
		break;
	case(CREG_INFO_BYTE1):
	default:
		byte = (m_hConfig & 0x00000000000000FF) >> 0;
		break;
	}
	return byte;
}

///<summary>CommonRegistry::GET HALF BYTE OF CONFIGURATION</summary>
BYTE CRegistry::GetReg4Bits(UINT creg_info, UINT byte_info) {

	BYTE bits; BYTE byte = GetRegBytes(creg_info);
	if (byte_info == WREG_BYTE_MSB) bits = (byte & 0xF0) >> 4;
	else bits = (byte & 0x0F) >> 0;

	return bits;
}