#include "disk_info.hpp"
#include <filesystem>

//disk_info::DiskInfo::DiskInfo()=default;

size_t disk_info::DiskInfo::getAvailableDiskSizeBytes(){
    try{
        std::filesystem::space_info space = std::filesystem::space(ROOT_PATH);
        return space.available;
    }catch(std::exception&ex){
        printf("Error checking storage availability for: %s: %s",ROOT_PATH, ex.what());
    }
    return 0;
}

size_t disk_info::DiskInfo::getAvailableDiskSizeGb(){
    size_t avSpace = getAvailableDiskSizeBytes();
    if(avSpace==0){
        return avSpace;
    }
    constexpr size_t KILOBYTE = 1024;
    size_t avSpaceGB = avSpace / (KILOBYTE*KILOBYTE*KILOBYTE);
    return avSpaceGB;
}

bool disk_info::DiskInfo::canStoreFile(size_t sizeBytes) {
    size_t avSpaceBytes = getAvailableDiskSizeBytes();
    return avSpaceBytes>sizeBytes;
}


size_t disk_info::DiskInfo::bytesToMegabytes(size_t bytes) {
    constexpr size_t KBTE = 1024;
    long double res = bytes / (KBTE * KBTE);
    size_t megaBytes = static_cast<size_t>(res);
    return megaBytes;
}




//disk_info::DiskInfo::~DiskInfo()=default;