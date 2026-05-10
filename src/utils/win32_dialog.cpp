#include "win32_dialog.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shobjidl.h>

#include <sstream>
#include <fstream>

string load_file(HWND owner) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    string file_name;

    if (SUCCEEDED(hr)) {
        IFileOpenDialog* file_open = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&file_open));

        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC types[] = {
                { L"Image Files", L"*.png;*.jpg;*.jpeg;" },
                { L"PNG Files",   L"*.png" }, 
                { L"JPEG Files",  L"*.jpg;*.jpeg" },
            };

            file_open->SetFileTypes(3, types);
            hr = file_open->Show(owner);
            if (SUCCEEDED(hr)) {
                IShellItem* item = nullptr;
                hr = file_open->GetResult(&item);

                if (SUCCEEDED(hr)) {
                    PWSTR path = nullptr;
                    item->GetDisplayName(SIGDN_FILESYSPATH, &path);

                    if (path) {
                        int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
                        file_name.resize(size - 1);
                        WideCharToMultiByte(CP_UTF8, 0, path, -1, file_name.data(), size, nullptr, nullptr);
                    }

                    CoTaskMemFree(path);
                    item->Release();
                }
            }
            file_open->Release();
        }
        CoUninitialize();
    }

    return file_name;
}