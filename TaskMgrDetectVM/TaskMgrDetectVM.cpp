// TestPcw.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>

void GetCpuInfo(int& _rax, int& _rbx, int& _rcx, int& _rdx) {
    int cpuInfo[4];
    CpuIdEx(cpuInfo, _rax, 0);
    _rax = cpuInfo[0];
    _rbx = cpuInfo[1];
    _rcx = cpuInfo[2];
    _rdx = cpuInfo[3];
}

int main()
{
    int cpuInfo[4];
    int _rax = 1;
    CpuIdEx(cpuInfo, _rax, 0);

    int _rcx, _rdx, _rbx;

    GetCpuInfo(_rax, _rbx, _rcx, _rdx);

    if (_rcx & 0x80000000) { // hypervisor presend
        printf("hypervisor presend\n");
        _rax = 0x40000000;
        GetCpuInfo(_rax, _rbx, _rcx, _rdx);

        std::string vm;
        vm += std::string((const char*)&_rbx, 4);
        vm += std::string((const char*)&_rcx, 4);
        vm += std::string((const char*)&_rdx, 4);

        std::cout << vm << std::endl;
    }
    _rax = 0;
    GetCpuInfo(_rax, _rbx, _rcx, _rdx);
    std::string vendor;
    vendor += std::string((const char*)&_rbx, 4);
    vendor += std::string((const char*)&_rdx, 4);
    vendor += std::string((const char*)&_rcx, 4);

    std::cout << vendor << std::endl;

    bool supportVT = false;
    if (vendor == "GenuineIntel") {
        _rax = 1;
        GetCpuInfo(_rax, _rbx, _rcx, _rdx);
        int vmx = _rcx >> 5;
        supportVT = vmx & 1;
        if (supportVT) {
            printf("Support VT!\n");
        }
        else {
            printf("Unsupport VT!\n");
        }
    }
    else if (vendor == "AuthenticAMD") {
        _rax = 1;
        GetCpuInfo(_rax, _rbx, _rcx, _rdx);
        int svm = _rcx >> 2;
        supportVT = svm & 1;
        if (supportVT) {
            printf("Support VT!\n");
        }
        else {
            printf("Unsupport VT!\n");
        }
    }

    if (supportVT) {
        if (IsProcessorFeaturePresent(PF_VIRT_FIRMWARE_ENABLED)) {
            printf("Virtualization is enabled in the firmware and made available by the operating system.\n");
        }
        if (IsProcessorFeaturePresent(PF_SECOND_LEVEL_ADDRESS_TRANSLATION)) {
            printf("Second Level Address Translation is supported by the hardware.\n");
            if (IsProcessorFeaturePresent(PF_NX_ENABLED)) {
                printf("Data execution prevention is enabled.\n");
            }
        }
    }

    system("pause");
}


