#include "lua_manager.h"
#include <dirent.h>
#include <glog/logging.h>

template <class T>
void defineLuaClass(sol::state& state) {}

template<>
void defineLuaClass<Location>(sol::state& state) {
    state.new_usertype<Location>(
        "Location", "x", &Location::x, "y", &Location::y, "z", &Location::z,
        "area", &Location::area, "is_valid", &Location::isValid);
}

template<>
void defineLuaClass<AddEntityEvent>(sol::state& state) {
    state.new_usertype<AddEntityEvent>("AddEntityEvent", "entity",
                                        &AddEntityEvent::entity, "location",
                                        &AddEntityEvent::location);
}

void LuaManager::initialise (EventManager* events) {
    m_events = events;

    m_lua.open_libraries(sol::lib::base, sol::lib::package);
    loadLuaScripts();
    m_lua.script("print('scripts loaded')");

    defineLuaClass<Location>(m_lua);
    m_events->subscribe<AddEntityEvent>([this](auto event) {
        auto result = m_lua["on_add_entity_event"](event);
        if (!result.valid()) {
            sol::error err = result;
            std::cout << err.what() << std::endl;
        }
    });
}

void LuaManager::loadLuaScripts() {
    const char CONSOLE_DIR[] = "../scripts/mods";

    DIR* directory = opendir(CONSOLE_DIR);
    if (directory == nullptr) {
        LOG(ERROR) << "Failed to open '" << CONSOLE_DIR << "'" << std::endl;
        throw std::runtime_error("Failed to open console directory");
    }
    struct dirent* file = nullptr;
    while ((file = readdir(directory)) != nullptr) {
        std::string filename(CONSOLE_DIR);
        filename.append("/").append(file->d_name);
        if (filename.find(".lua") != std::string::npos) {
            LOG(INFO) << "Loading lua script: " << filename << std::endl;
            try {
                m_lua.script_file(filename);
            } catch (const std::runtime_error& error) {
                LOG(ERROR) << "Failed to load " << filename << std::endl;
            }
        }
    }
    closedir(directory);
}

