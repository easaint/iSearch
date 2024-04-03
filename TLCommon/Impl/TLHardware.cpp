#include "../Export/TLHardware.h"

#ifdef _WINDOWS
#pragma comment(lib, "wbemuuid.lib")
#endif

namespace tl {

	CTLHardware::CTLHardware()
	{
#ifdef _WINDOWS
        m_phRet = NULL;
        initContext();
#endif
	}

    CTLHardware::CTLHardware(HRESULT* hRet)
    {
        this->m_phRet = hRet;
        initContext();
    }

	CTLHardware::~CTLHardware()
	{
#ifdef _WINDOWS
        //Cleanup
        this->m_pCtx->pSvc->Release();
        this->m_pCtx->pLoc->Release();
        delete this->m_pCtx;
        this->m_pCtx = nullptr;
        if (this->m_phRet == nullptr)
        {
            CoUninitialize();
        }
#endif;
	}

	TLOSInfo CTLHardware::osInfo()
	{
        TLOSInfo osInfo;
#ifdef _WINDOWS
		IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_OperatingSystem");
        // Step 7:
    // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"BuildNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.buildNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.version = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"InstallDate", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.installDate = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"OSArchitecture", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.osArchitecture = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"RegisteredUser", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.registerdUser = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                osInfo.serialNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return osInfo;
	}

    std::vector<TLGraphicsCard> CTLHardware::graphicsCard()
    {
        std::vector<TLGraphicsCard> graphicsCards;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_VideoController");
        // Step 7:
// Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        TLGraphicsCard videoController;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                videoController.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                videoController.deviceId = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            graphicsCards.push_back(videoController);

            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return graphicsCards;
    }

    std::vector<TLDiskDrive> CTLHardware::diskDrive()
    {
        std::vector<TLDiskDrive> diskDrives;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_DiskDrive");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        TLDiskDrive diskDrive;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                diskDrive.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                diskDrive.deviceId = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                diskDrive.serialNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Size", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                VarUI8FromStr(vtProp.bstrVal, NULL, LOCALE_NOUSEROVERRIDE, &diskDrive.size);
            }
            VariantClear(&vtProp);

            diskDrives.push_back(diskDrive);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return diskDrives;
    }

    TLBaseBoard CTLHardware::baseBoard()
    {
        TLBaseBoard baseBoard;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_BaseBoard");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                baseBoard.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                baseBoard.manufacturer = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                baseBoard.product = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                baseBoard.version = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                baseBoard.serialNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return baseBoard;
    }

    TLBios CTLHardware::bois()
    {
        TLBios bios;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_BIOS");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"ReleaseDate", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                bios.releaseDate = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                bios.manufacturer = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                bios.serialNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return bios;
    }

    std::vector<TLPhysicalMemory> CTLHardware::physicalMemory()
    {
        std::vector<TLPhysicalMemory> memorys;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_PhysicalMemory");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        TLPhysicalMemory physicalMemory;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                physicalMemory.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                physicalMemory.manufacturer = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                physicalMemory.serialNumber = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Speed", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
                physicalMemory.speed = vtProp.uintVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Capacity", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                VarUI8FromStr(vtProp.bstrVal, NULL, LOCALE_NOUSEROVERRIDE, &physicalMemory.size);
            }
            VariantClear(&vtProp);

            memorys.push_back(physicalMemory);

            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return memorys;
    }

    std::vector<TLProcessor> CTLHardware::processor()
    {
        std::vector<TLProcessor> processors;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_Processor");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        TLProcessor processor;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                processor.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Description", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                processor.desc = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                processor.manufacturer = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
                processor.numberOfCores = vtProp.uintVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"ThreadCount", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
                processor.threadCount = vtProp.uintVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                processor.processId = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
                processor.maxClockSpeed = static_cast<std::double_t>(vtProp.uintVal) / 1000.0;
            }
            VariantClear(&vtProp);

            processors.push_back(processor);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return processors;
    }

    std::vector<TLNetworkAdapter> CTLHardware::networkAdapter()
    {
        std::vector<TLNetworkAdapter> networkAdapters;
#ifdef _WINDOWS
        IEnumWbemClassObject* pEnumerator = ExecQuery(L"Win32_NetworkAdapter");

        // Step 7:
        // Get the data from the query in step 6

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        TLNetworkAdapter networkAdapter;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                networkAdapter.name = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Description", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                networkAdapter.desc = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                networkAdapter.manufacturer = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                networkAdapter.macAddress = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            hr = pclsObj->Get(L"AdapterType", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
                networkAdapter.adapterType = vtProp.bstrVal;
            }
            VariantClear(&vtProp);

            networkAdapters.push_back(networkAdapter);
            pclsObj->Release();
        }
        pEnumerator->Release();
#endif
        return networkAdapters;
    }

#ifdef _WINDOWS
    IEnumWbemClassObject* CTLHardware::ExecQuery(const std::wstring className)
    {
        IEnumWbemClassObject* pEnumerator = NULL;
        std::wstring cmd = L"SELECT * FROM ";
        cmd += className;

        //std::wcout << cmd << std::endl;

        this->m_pCtx->hResult = this->m_pCtx->pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t(cmd.data()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        if (FAILED(this->m_pCtx->hResult))
        {
            this->m_pCtx->message.clear();
            this->m_pCtx->message << "Query for xxx failed." << " Error code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
            this->m_pCtx->pSvc->Release();
            this->m_pCtx->pLoc->Release();
            CoUninitialize();
            throw HardwareException(this->m_pCtx->message.str());
            // Program has failed.
        }
        return pEnumerator;
    }

    void CTLHardware::initContext()
    {
        this->m_pCtx = new WMICContext();
        //Step 1
        //Initialize COM
        if (this->m_phRet == nullptr)
        {
            this->m_pCtx->hResult = CoInitializeEx(0, COINIT_MULTITHREADED);
        }
        else
        {
            this->m_pCtx->hResult = *this->m_phRet;
        }
        if (FAILED(this->m_pCtx->hResult)) {
            this->m_pCtx->message.clear();
            this->m_pCtx->message << "Failed to initialize COM library. Error code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
            throw HardwareException(this->m_pCtx->message.str());
        }

        ////Step 2
        ////Set general COM security levels
        //this->m_pCtx->hResult = CoInitializeSecurity(
        //    NULL,
        //    -1,                          // COM authentication
        //    NULL,                        // Authentication services
        //    NULL,                        // Reserved
        //    RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        //    RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        //    NULL,                        // Authentication info
        //    EOAC_NONE,                   // Additional capabilities
        //    NULL                         // Reserved
        //);
        //if (FAILED(this->m_pCtx->hResult))
        //{
        //    this->m_pCtx->message.clear();
        //    this->m_pCtx->message << "Failed to initialize security. Error code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
        //    if (this->m_phRet == nullptr)
        //        CoUninitialize();
        //    throw HardwareException(this->m_pCtx->message.str());
        //    // Program has failed.
        //}

        //Step 3
        //Obtain the initial locator to WMI
        this->m_pCtx->hResult = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&this->m_pCtx->pLoc);

        if (FAILED(this->m_pCtx->hResult))
        {
            this->m_pCtx->message.clear();
            this->m_pCtx->message << "Failed to create IWbemLocator object." << " Err code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
            if (this->m_phRet == nullptr)
            {
                CoUninitialize();
            }
            throw HardwareException(this->m_pCtx->message.str());
            // Program has failed.
        }

        //Step 4
        //Connect to WMI through the IWbemLocator::ConnectServer method

        // Connect to the root\cimv2 namespace with
        // the current user and obtain pointer pSvc
        // to make IWbemServices calls.
        this->m_pCtx->hResult = this->m_pCtx->pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name. NULL = current user
            NULL,                    // User password. NULL = current
            0,                       // Locale. NULL indicates current
            NULL,                    // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object
            &this->m_pCtx->pSvc              // pointer to IWbemServices proxy
        );

        if (FAILED(this->m_pCtx->hResult))
        {
            this->m_pCtx->message.clear();
            this->m_pCtx->message << "Could not connect. Error code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
            this->m_pCtx->pLoc->Release();
            if (this->m_phRet == nullptr)
                CoUninitialize();
            throw HardwareException(this->m_pCtx->message.str());
            // Program has failed.
        }

        //Step 5
        //Set security levels on the proxy
        this->m_pCtx->hResult = CoSetProxyBlanket(
            this->m_pCtx->pSvc,                  // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            NULL,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities
        );

        if (FAILED(this->m_pCtx->hResult))
        {
            this->m_pCtx->message.clear();
            this->m_pCtx->message << "Could not set proxy blanket. Error code = 0x" << std::hex << this->m_pCtx->hResult << std::endl;
            this->m_pCtx->pSvc->Release();
            this->m_pCtx->pLoc->Release();
            if (this->m_phRet == nullptr)
                CoUninitialize();
            throw HardwareException(this->m_pCtx->message.str());
            // Program has failed.
        }
    }
#endif

}