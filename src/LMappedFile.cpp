#include "LMappedFile.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

MappedFile::MappedFile(const std::string& path)
{
    struct stat fileStat;

    m_descriptor = open(path.c_str(), O_RDONLY);

    if (m_descriptor == -1)
        return;

    fstat(m_descriptor, &fileStat);
    m_size = fileStat.st_size;

    m_data = reinterpret_cast<char*>(mmap(nullptr, fileStat.st_size, PROT_READ, MAP_PRIVATE, m_descriptor, 0));
}

MappedFile::~MappedFile()
{
    if (m_data != MAP_FAILED && m_data != nullptr)
        munmap(m_data, m_size);

    if (m_descriptor != -1)
        close(m_descriptor);
}

bool MappedFile::isOpen() const
{
    return m_data != MAP_FAILED && m_data != nullptr;
}

uint64_t MappedFile::size() const
{
    return m_size;
}

const char* MappedFile::data() const
{
    return m_data;
}
