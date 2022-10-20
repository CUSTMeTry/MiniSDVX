// MiniSDVX_HID_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <hidsdi.h>

HANDLE userHidFileHandle;

using namespace std;

BOOL find_device(uint32_t PID, uint32_t VID);

int main()
{
    BOOL isReadFile = false;
    uint8_t readbuf[11];
    DWORD numberOfByteRead;
    std::cout << "Hello World!\n";
    if (find_device(22532, 1155)) {
        while (true) {
            Sleep(1);
            isReadFile = ReadFile(
                userHidFileHandle,
                readbuf,
                11,
                &numberOfByteRead,
                NULL
            );
            if (isReadFile) {
                {
                    cout << "read data byte:" << numberOfByteRead << endl;
                    for (int i = 0; i < numberOfByteRead; i++) {
                        cout << "readData:" << readbuf[i] << endl;
                    }
                } }
        }
    }
}

void show_guid(LPGUID HidGuid) {
    cout << hex << HidGuid->Data1 << "-";
    cout << hex << HidGuid->Data2 << "-";
    cout << hex << HidGuid->Data3 << "-";
    for (int i = 0; i < 8; i++)
    {
        cout << hex << (uint32_t)(HidGuid->Data4[i]);
    }
    cout << endl;
}

BOOL find_device(uint32_t PID, uint32_t VID) {
    BOOL isFind = false;
    GUID hid_guid;
    
    HDEVINFO device_info;

    BOOL isGetDeviceInterface = false;
    BOOL isGetDeviceInterfaceDetail = false;
    BOOL isGetAttributes;
    HIDD_ATTRIBUTES hid_attributes;
    uint32_t deviceIndex = 0;
    SP_DEVICE_INTERFACE_DATA device_interface_data;
    PSP_DEVICE_INTERFACE_DETAIL_DATA p_device_interface_detail_data;
    DWORD RequiredSize = 0;
    HANDLE fileHandle;
    HidD_GetHidGuid(&hid_guid);
    show_guid(&hid_guid);

    device_info = SetupDiGetClassDevs(&hid_guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    do {
        device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        isGetDeviceInterface = SetupDiEnumDeviceInterfaces(
            device_info,
            NULL,
            &hid_guid,
            deviceIndex,
            &device_interface_data
        );

        

        deviceIndex++;
        if (isGetDeviceInterface) {
            cout << "GetDeviceInterface Succcess" << endl;
            /*获取 RequireSize*/
            isGetDeviceInterfaceDetail = SetupDiGetDeviceInterfaceDetail(
                device_info,
                &device_interface_data,
                NULL,
                0,
                &RequiredSize,
                NULL
            );
            cout << "RequiredSize-" << RequiredSize << endl;
            p_device_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
            p_device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
            isGetDeviceInterfaceDetail = SetupDiGetDeviceInterfaceDetail(
                device_info,
                &device_interface_data,
                p_device_interface_detail_data,
                RequiredSize,
                NULL,
                NULL
            );
            if (isGetDeviceInterfaceDetail) {
                cout << "GetDeviceInterfaceDetail Succcess" << endl;
                fileHandle = CreateFile(
                    p_device_interface_detail_data->DevicePath,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );
                if (fileHandle == INVALID_HANDLE_VALUE) {
                    cout << "CreateFile ERROR" << endl;
                }
                else {
                    cout << "CreateFile SUCCESS" << endl;
                    isGetAttributes = HidD_GetAttributes(fileHandle, &hid_attributes);
                    if (isGetAttributes) {
                        cout << "PID:" << dec<< hid_attributes.ProductID << endl;
                        cout << "VID:" << dec <<hid_attributes.VendorID<< endl;
                    }
                    if (hid_attributes.ProductID == PID && hid_attributes.VendorID == VID) {
                        userHidFileHandle = fileHandle;
                        return true;
                    }
                }
            }
            
        }
    } while (isGetDeviceInterface);
    

    return isFind;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
