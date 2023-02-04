#include "core/util.h"
#include "core/library.h"
#include "core/host.h"

#include <pginf/base/logging.h>

namespace pginf {
namespace core {

    std::shared_ptr<Library> loadPlugin(std::shared_ptr<Host>& host, const std::string& path)
    {
        std::shared_ptr<Library> lib(Library::load(path));
        if (!lib)
            return lib;

        // try to get registration function.
        Fn_ImportPlugin* import_plugin = reinterpret_cast<Fn_ImportPlugin*>(lib->getSymbol("importPlugin"));

        if (!import_plugin) {
            LOG_ERROR() << "Failed to initialize plugin[" << lib->id()
                        << "], symbol[importPlugin] function not found!";
            lib.reset();
            return lib;
        }

        // try to initialize plugin:
        if (!import_plugin(*host, lib->id())) {
            // plugin decided to fail
            LOG_ERROR() << "Failed to import plug-in[" << lib->id() << "].";
            lib.reset();
            return lib;
        }

        return lib;
    }

} // namespace core
} // namespace pginf
