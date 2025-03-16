#ifndef __KOAI_DISK_INFO__
#define __KOAI_DISK_INFO__

#if defined(_WIN32)
    #define ROOT_PATH "C:\\"
#elif defined(__APPLE__)
    #define ROOT_PATH "/"
#else
    #define ROOT_PATH "/"
#endif

#include <cstddef>

namespace disk_info{
    class DiskInfo{
        public:
        DiskInfo()=default;
        DiskInfo(const DiskInfo&)=delete;
        DiskInfo&operator=(const DiskInfo&)=delete;
        DiskInfo(DiskInfo&&)=delete;
        DiskInfo&operator=(DiskInfo&&)=delete;
        size_t getAvailableDiskSizeBytes();
        size_t getAvailableDiskSizeGb();
        bool canStoreFile(size_t sizeKB);
        size_t bytesToMegabytes(size_t bytes);
        ~DiskInfo()=default;
    };
}

#endif // ! __KOAI_DISK_INFO__  INCLUDED