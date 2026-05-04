#include "res.hpp"
#include "upng.hpp"
#include <SDL3/SDL.h>
#include <string>

static SDL_IOStream* open_stream(std::string fp) {
#ifdef JUMBO_BUILD
    void* data = nullptr;
    size_t size = 0;
    // JUMBO_FILL_DATA
    if (!data)
        return nullptr;
    return SDL_IOFromConstMem(data, size);
#else
    return SDL_IOFromFile((std::string("assets/") + fp).c_str(), "rb");
#endif
}

static std::pair<void*, size_t> read_data(SDL_IOStream* io) {
    if (io) {
        Sint64 size = SDL_GetIOSize(io);
        if (size >= 0) {
            size_t total_size = static_cast<size_t>(size);
            void* data = SDL_malloc(total_size);
            if (data) {
                if (SDL_ReadIO(io, data, total_size) == total_size) {
                    return {data, total_size};
                } else {
                    SDL_free(data);
                }
            }
        }
    }
    return {nullptr, 0};
}

static void free_data(void* data) { SDL_free(data); }

SDL_Surface* res_load_surface(std::string fp) {
    auto buf = read_data(open_stream(fp));
    if (!buf.first)
        return nullptr;
    SDL_Surface* ret = nullptr;
    upng_t* handle = upng_new_from_bytes(reinterpret_cast<unsigned char*>(buf.first),
                                         static_cast<unsigned long>(buf.second));
    if (handle) {
        upng_error err = upng_decode(handle);
        if (err == UPNG_EOK) {
            ret = SDL_CreateSurface(static_cast<int>(upng_get_width(handle)),
                                    static_cast<int>(upng_get_height(handle)),
                                    SDL_PIXELFORMAT_RGBA32);
            if (ret) {
                if (SDL_MUSTLOCK(ret))
                    SDL_LockSurface(ret);
                if (upng_get_components(handle) == 4)
                    SDL_memcpy(ret->pixels, upng_get_buffer(handle), upng_get_size(handle));
                else if (upng_get_components(handle) == 3) {
                    unsigned long sz = upng_get_size(handle);
                    const unsigned char* src = upng_get_buffer(handle);
                    unsigned char* dst = (unsigned char*)ret->pixels;
                    for (unsigned long i = 0; i < (sz / 3); i++) {
                        dst[i * 4] = src[i * 3];
                        dst[i * 4 + 1] = src[i * 3 + 1];
                        dst[i * 4 + 2] = src[i * 3 + 2];
                        dst[i * 4 + 3] = 255;
                    }
                }
                if (SDL_MUSTLOCK(ret))
                    SDL_UnlockSurface(ret);
            }
        }
        upng_free(handle);
    }
    return ret;
}

void res_init() {}
