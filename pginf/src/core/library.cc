#include "core/library.h"

#include <pginf/base/logging.h>
#include <pginf/base/util.h>

#ifdef P_OS_WIN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace pginf {
namespace core {

    Library::Symbol Library::getSymbol(Handle handle, const std::string& symbol_name)
    {
#ifdef P_OS_WIN
        auto symbol = ::GetProcAddress((HMODULE)handle, symbol_name.c_str());
#else
        auto symbol = ::dlsym(handle, symbol_name.c_str());
#endif

        if (!symbol) {
            LOG_ERROR() << "Library symbol[" << symbol_name << "] not found.";
            return nullptr;
        }
        return symbol;
    }

    Library::Library(Id id, Handle handle)
        : id_(id)
        , handle_for_library_(handle)
    {
        // Nothing to do
    }

    Library* Library::load(const std::string& path)
    {
        if (path.empty()) {
            LOG_ERROR() << "Failed to load library: Empty path";
            return nullptr;
        }

        Handle handle = nullptr;

        try {
            // Load library - OS dependent operation
#ifdef P_OS_WIN
            handle = ::LoadLibraryA(path.c_str());
            if (!handle) {
                LOG_ERROR() << "Failed to load library[" << path << "].";
                return nullptr;
            }
#else
            handle = ::dlopen(path.c_str(), RTLD_NOW);
            if (!handle) {
                LOG_ERROR() << "Failed to load library[" << path
                            << "]. Detail:\n  "
                            << ::dlerror();
                return nullptr;
            }
#endif
        } catch (const std::exception& e) {
            LOG_ERROR() << "library::load() error: \'" << e.what() << "\'.";
        }

        try {
            // Get func [get_id]
            Fn_GetId* get_id;
            get_id = reinterpret_cast<Fn_GetId*>(getSymbol(handle, "getId"));

            if (!get_id) {
                LOG_ERROR() << "Failed to load library[" << path << "]: [getId] symbol not found!";
#ifdef P_OS_WIN
                ::FreeLibrary((HMODULE)handle);
#else
                ::dlclose(handle);
#endif
                return nullptr;
            }

            // Get identifier
            Id id {};
            get_id(id);

            // return a library with the DLL handle
            return new Library(id, handle);
        } catch (const std::exception& e) {
            LOG_ERROR() << "library::load() error: \'" << e.what() << "\'.";
        }

        return nullptr;
    }

    Library::Library(Library&& another)
    {
        id_.swap(another.id_);
        handle_for_library_ = another.handle_for_library_;
        another.handle_for_library_ = nullptr;
    }

    Library::~Library()
    {
        if (handle_for_library_) {
#ifdef P_OS_WIN
            ::FreeLibrary((HMODULE)handle_for_library_);
#else
            ::dlclose(handle_for_library_);
#endif
        }
    }

    Library&
    Library::operator=(Library&& another)
    {
        id_.swap(another.id_);
        std::swap(handle_for_library_, another.handle_for_library_);
        return *this;
    }

    Library::Symbol Library::getSymbol(const std::string& symbol_name)
    {
        if (!handle_for_library_) {
            LOG_ERROR() << "Cannot inspect library symbols, library isn't loaded.";
            return nullptr;
        }

        return getSymbol(handle_for_library_, symbol_name);
    }

} // namespace plugin
} // namespace pginf
