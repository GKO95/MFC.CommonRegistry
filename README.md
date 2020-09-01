# MFC.CommonRegistry

MFC.CommonRegistry is a repository containing `CRegistry` class which allows simple and easy-to-use MFC registry setup and utilization. For the version under Visual Studio 2017 requires renaming precompiled header from `pch.h` to `stdafx.h` in the source file.

`CRegistry` class supports 2, 4, and 8-byte binary registry and can be changed using macro on Line 6:

```cpp
#define CREG_BYTE_SIZE    4
```

To resize the binary registry, previously generated registry must be deleted firsthand. However, this can be only done using Registry Editor for now.

### Declaration

`CRegistry` class does not rely on constructor and can be instantiated only from declaration:

```cpp
// DECLARATION
CRegistry RegConfig;
```

Declaration alone does not create/open any registry. It is recommended to declare the `CRegistry` instance on the header file of working project.

### Creating/Opening Registry Key

`CRegistry` have operator `()` which can define the name of the registry key.

```cpp
// CREATE/OPEN REGISTRY KEY
RegConfig(CString("Program Name"));
```

This will open the registry key with specified name and, if it does not exist, create a new registry key.

### Closing Registry Key

`CRegistry` class has destructor which automatically closes registry key upon exiting MFC application.

## API

CRegistry has two methods for getting registry data, and two methods for setting registry data. Accessing and modifying registry data can be done either by a single byte or a half-byte (4-bit).

### `GetRegBytes()`
```cpp
BYTE    GetRegBytes(UINT creg_info);
```
* `creg_info`: location of byte to retrieve from registry.

### `GetReg4Bits()`
```cpp
BYTE	GetReg4Bits(UINT creg_info, UINT byte_info);
```
* `creg_info`: location of byte to retrieve from registry.
* `byte_info`: select MSB or LSB from retrieved registry byte.

### `SetRegBytes()`
```cpp
void    SetRegBytes(UINT creg_info, UINT byte);
```
* `creg_info`: location of byte to modify on registry.
* `byte`: a single byte value to change to on selected registry data.

### `SetReg4Bits()`
```cpp
void 	SetReg4Bits(UINT creg_info, UINT byte_info, UINT bits);
```
* `creg_info`: location of byte to modify on registry.
* `byte_info`: select MSB or LSB from retrieved registry byte.
* `byte`: a single byte value to change to on selected registry half-byte.

## Changelog
* 2020.09.01 [ver.1.1.0]<br/>: Major bug fixes on CRegistry not properly saving and loading; accessing/modifying half-byte is now available.
* 2020.05.14 [ver.1.0.0]<br/>: Initial commit!