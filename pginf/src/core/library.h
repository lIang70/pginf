#ifndef _PGINF_PLUGIN_LIBRARY_H_
#define _PGINF_PLUGIN_LIBRARY_H_

#include <pginf/base/detail/non_copyable.h>

#include <string>

namespace pginf {
namespace core {

    //!
    //! @brief Manages a Dynamic Linking Library.
    //!
    class Library : public NonCopyable {
        friend class Manage;

        using Handle = void*;
        using Symbol = void*;
        using Id = std::string;

        // function for plugin
        using Fn_GetId = void(std::string&);

        Id identifier_ {}; // unique idetifier.
        Handle handle_for_library_ { nullptr }; // Library handle.

    private:
        //!
        //! @brief Get the Symbol object from handle of library.
        //!
        //! @param handle The handle of library.
        //! @param symbol_name The name of the symbol we're looking for.
        //! @return Pointer to what the symbol refers to, or nullptr if the symbol is not found.
        //!
        static Symbol getSymbol(Handle handle, const std::string& symbol_name);

        //!
        //! @brief Construct a new library object.
        //! Library instances cannot be created, use Load instead.
        //!
        //! @param handle Library handle.
        //! @see Load(const std::string & _path)
        //!
        Library(Id id, Handle handle = nullptr);

    public:
        //!
        //! @brief  Load a library.
        //!
        //! @param  path Path to the library.
        //! @return Pointer to the loaded library, or nullptr if failed.
        //!
        static Library* load(const std::string& path);

        //!
        //! @brief Move-construct for library object
        //!
        //! @param another
        //!
        Library(Library&& another);

        //!
        //! @brief Destroy the library object,
        //! Close and free the opened library (if any).
        //!
        //!
        ~Library();

        Library& operator=(Library&& another);

        inline std::string id() { return identifier_; }

        //!
        //! @brief Get the symbol object
        //!
        //! @param symbol_name The name of the symbol we're looking for.
        //! @return Pointer to what the symbol refers to, or nullptr if the symbol is not found.
        //!
        Symbol getSymbol(const std::string& symbol_name);
    };

} // namespace plugin
} // namespace pginf

#endif // !_PGINF_PLUGIN_LIBRARY_H_