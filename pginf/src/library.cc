#include "library.h"

#ifdef C_OS_WIN
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

PGINF_NAMESPACE_BEGIN()

Library::_Symbol
Library::GetSymbol(_Handle handle, const std::string& symbol_name) {
    _Symbol symbol;

#ifdef C_OS_WIN
    symbol = (_Symbol)(::GetProcAddress((HMODULE)handle, symbol_name.c_str()));
#else
    symbol = (_Symbol)(::dlsym(handle, symbol_name.c_str()));
#endif

    if (!symbol) {
        LOGGER("[Error] Library symbol \"%s\" not found.", symbol_name.c_str());
        return nullptr;
    }
    return symbol;
}

Library::Library(_Id id, _Handle handle)
    : identifier_(id), handle_for_library_(handle) {
    // Nothing to do
}

Library* 
Library::Load(const std::string & path) {
    if (path.empty()) {
        LOGGER("[Error] Failed to load library: Empty path");
        return nullptr;
    }

    _Handle handle = nullptr;
    _Id     id{};

    try {
        // Load library - OS dependent operation
#ifdef C_OS_WIN
        handle = ::LoadLibraryA(path.c_str());
        if (!handle) {
            LOGGER("[Error] Failed to load library \"%s\".", path.c_str());
            return nullptr;
        }
#else
        handle = ::dlopen(path.c_str(), RTLD_NOW);
        if (!handle) {
            const char* error_string = ::dlerror();
            LOGGER("[Error] Failed to load library \"%s\".\n", path.c_str());
            if(error_string) 
                LOGGER("   OS returned error: \"%s\".", error_string);
            LOGGER("\n");
            return nullptr;
        }
#endif
    } catch(const std::exception& e) {
        LOGGER("[Error] library::load() error: \"%s\".", e.what());
    }

    try {
        // Get func [get_id]
        _Fn_Get_Id* id_fn;
        id_fn = reinterpret_cast<_Fn_Get_Id*>(GetSymbol(handle, "get_id"));

        if (!id_fn) {
            LOGGER("[Error] Failed to load library \"%s\": [get_id] function not found!", path.c_str());
#ifdef C_OS_WIN
            ::FreeLibrary((HMODULE)handle);
#else
            ::dlclose(handle);
#endif
            return nullptr;
        }

        // Get identifier
        id_fn(id);

        // return a library with the DLL handle
        return new Library(id, handle);
    } catch(const std::exception& e) {
        LOGGER("[Error] library::load() error: \"%s\".", e.what());
    }
    
    return nullptr;
}

Library::Library(Library && another) {
    identifier_.swap(another.identifier_);
    handle_for_library_         = another.handle_for_library_;
    another.handle_for_library_ = nullptr;
}

Library::~Library() {
    if (handle_for_library_ ) {
#ifdef C_OS_WIN
        ::FreeLibrary((HMODULE)handle_for_library_);
#else
        ::dlclose(handle_for_library_);
#endif
    }
}

Library & 
Library::operator=(Library && another) {
    identifier_.swap(another.identifier_);
    handle_for_library_         = another.handle_for_library_;
    another.handle_for_library_ = nullptr;
    return *this;
}

Library::_Symbol 
Library::GetSymbol(const std::string & symbol_name) {
    if (!handle_for_library_) {
        LOGGER("[Error] Cannot inspect library symbols, library isn't loaded.");
        return nullptr;
    }

    return GetSymbol(handle_for_library_, symbol_name);
}

PGINF_NAMESPACE_END()
