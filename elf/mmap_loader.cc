// Copyright (c) 2013 Austin T. Clements. All rights reserved.
// Use of this source code is governed by an MIT license
// that can be found in the LICENSE file.

#include "elf++.hh"

#include <system_error>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#ifdef _MSC_VER

#include <io.h>
#include <Windows.h>

ELFPP_BEGIN_NAMESPACE

class mmap_loader : public loader
{
    void *base;
    size_t lim;
    int fd_;
    HANDLE mapping_;

public:
    mmap_loader(int fd) : fd_(fd)
    {
        off_t end = _lseek(fd, 0, SEEK_END);
        if (end == (off_t)-1)
            throw system_error(errno, system_category(),
                "finding file length");
        lim = end;

        HANDLE handle = (HANDLE)_get_osfhandle(fd);

        if (handle == INVALID_HANDLE_VALUE)
            throw system_error(errno, system_category(),
                "mmap'ing file :invalid handle");

        mapping_ = CreateFileMappingA(
            handle,
            NULL,
            PAGE_READONLY,
            0,
            0,
            NULL);
        if (mapping_ == NULL)
            raise_last_error("CreateFileMappingA");

        base = MapViewOfFile(
            mapping_,
            FILE_MAP_READ,
            0,0,
            lim);

        if (base == NULL)
        {
            CloseHandle(mapping_);
            mapping_ = nullptr;
            raise_last_error("MapViewOfFile");
        }

    }

    ~mmap_loader()
    {
        UnmapViewOfFile(base);
        CloseHandle(mapping_);
        mapping_ = nullptr;
        _close(fd_);
    }

    const void *load(off_t offset, size_t size)
    {
        if (offset + size > lim)
            throw range_error("offset exceeds file size");
        return (const char*)base + offset;
    }

private:
    static void raise_last_error(const std::string& message)
    {
        LPSTR msg_buffer;
        DWORD lm = GetLastError();
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            0,
            lm,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&msg_buffer,
            0,
            NULL
        );
        std::string emsg = msg_buffer;
        LocalFree(msg_buffer);
        throw system_error(errno, system_category(),
            message + " (error " + std::to_string(lm) + "): " + emsg);

    }
};

#else
#include <sys/mman.h>
#include <unistd.h>

ELFPP_BEGIN_NAMESPACE

class mmap_loader : public loader
{
        void *base;
        size_t lim;

public:
        mmap_loader(int fd)
        {
                off_t end = lseek(fd, 0, SEEK_END);
                if (end == (off_t)-1)
                        throw system_error(errno, system_category(),
                                           "finding file length");
                lim = end;

                base = mmap(nullptr, lim, PROT_READ, MAP_SHARED, fd, 0);
                if (base == MAP_FAILED)
                        throw system_error(errno, system_category(),
                                           "mmap'ing file");
                close(fd);
        }

        ~mmap_loader()
        {
                munmap(base, lim);
        }

        const void *load(off_t offset, size_t size)
        {
                if (offset + size > lim)
                        throw range_error("offset exceeds file size");
                return (const char*)base + offset;
        }
};

#endif

std::shared_ptr<loader>
create_mmap_loader(int fd)
{
        return make_shared<mmap_loader>(fd);
}

ELFPP_END_NAMESPACE
