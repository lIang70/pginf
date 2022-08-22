#pragma once
#ifndef _PGINF_LIBRARY_H_
#define _PGINF_LIBRARY_H_

#include "internal_global.h"

PGINF_NAMESPACE_BEGIN()

/****************************************
 * @brief Manages a Dynamic Linking Library.
 * 
 ****************************************/
class Library {
    friend class Manager_Impl;

    using _Handle   = void *;
    using _Symbol   = void *;
    using _Id       = std::string;

    // func
    typedef void _Fn_Get_Id(std::string &);

    _Id     identifier_{};                  // unique idetifier.
    _Handle handle_for_library_ = nullptr;  // Library handle.

private:
    /****************************************
     * @brief Get the Symbol object from handle of library.
     * 
     * @param handle The handle of library.
     * @param symbol_name The name of the symbol we're looking for.
     * @return Pointer to what the symbol refers to, or nullptr if the symbol is not found.
     ****************************************/
    static _Symbol GetSymbol(_Handle handle, const std::string& symbol_name); 

    /****************************************
     * @brief Construct a new library object.
     * Library instances cannot be created, use Load instead.
     * 
     * @param handle Library handle.
     * @see Load(const std::string & _path)
     ****************************************/
    Library(_Id id, _Handle handle = nullptr);

public:
    /****************************************
     * @brief  Load a library.
     * 
     * @param  path Path to the library.
     * @return Pointer to the loaded library, or nullptr if failed. 
     ****************************************/
    static Library* Load(const std::string& path);

    /****************************************
     * @brief Move-construct for library object
     * 
     * @param another 
     ****************************************/
    Library(Library&& another);

    /****************************************
     * @brief Destroy the library object, 
     * Close and free the opened library (if any).
     * 
     ****************************************/
    ~Library();

    /****************************************
     * @brief Move func.
     * 
     * @param another 
     * @return library& 
     ****************************************/
    Library & operator=(Library&& another);

    /****************************************
     * @brief Get the symbol object
     * 
     * @param symbol_name The name of the symbol we're looking for.
     * @return Pointer to what the symbol refers to, or nullptr if the symbol is not found.
     ****************************************/
    _Symbol GetSymbol(const std::string& symbol_name);

    // Delete copy-construct/copy
    Library(const Library& another) = delete;
    Library& operator=(const Library& another) = delete;

};

PGINF_NAMESPACE_END()

#endif // !_PGINF_LIBRARY_H_