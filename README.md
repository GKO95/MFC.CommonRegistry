# MFC.CommonRegistry

MFC.CommonRegistry is a repository containing `CRegistry` class which allows simple and easy-to-use MFC registry setup and utilization. For the version under Visual Studio 2017 requires renaming precompiled header from `pch.h` to `stdafx.h`.

`CRegistry` class supports 2, 4, and 8-byte binary registry and can be changed using macro on Line 6:

```c++
#define CREG_BYTE_SIZE	4
```

To resize the binary registry, previously generated registry must be deleted firsthand. However, this can be only done using Registry Editor for now.

### Declaration

`CRegistry` class does not rely on constructor and can be instantiated only from declaration:

```c++
// DECLARATION
CRegistry RegConfig;
```

Declaration alone does not create/open any registry. It is recommended to declare the `CRegistry` instance on the header file.

### Create/Open Registry

`CRegistry` have operator `()` which can define the name of the registry.

```c++
// CREATE/OPEN REGISTRY
RegConfig(CString("Program Name"));
```

This will open the registry with specified name and, if it does not exist, create a new registry.

### Close Registry

`CRegistry` class has destructor which automatically closes registry upon exiting MFC application.