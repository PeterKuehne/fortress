#include "grouping_manager.h"

Grouping& GroupingManager::createNewGrouping(const std::string& name) {
    if (name.empty()) {
        throw std::string("A Grouping must have a name");
    }
    auto retval =
        m_groupings.insert(GroupingContainer::value_type(name, Grouping(name)));
    if (!retval.second) {
        throw std::string("Grouping '" + name + "' already exists");
    }
    return retval.first->second;
}

GroupingContainer& GroupingManager::getGroupings() { return m_groupings; }

Grouping& GroupingManager::getGrouping(const std::string& name) {
    auto iter = m_groupings.find(name);
    if (iter == m_groupings.end()) {
        throw std::string("Can't find grouping '" + name + "'");
    }
    return iter->second;
}

/// Effectively sets the relation between A -> B, but not B -> A
void GroupingManager::setRelationship(Grouping& from, Grouping& to,
                                      int relationship) {
    getGrouping(from.getName()).setRelationship(to.getName(), relationship);
}

void GroupingManager::setRelationship(const std::string& from,
                                      const std::string& to, int relationship) {

    getGrouping(from).setRelationship(to, relationship);
}
