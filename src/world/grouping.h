#pragma once

#include "../core/entity.h"
#include <map>
#include <set>
#include <string>

/// @brief A generalised grouping of entities
/// @details Every association of entities can be represented by a Grouping,
/// including families, kingdoms, warbands, hordes, etc.
class Grouping {
public:
    Grouping() = delete;
    ~Grouping() = default;

    /// @brief Constructs a new grouping with the given name
    /// @param[in] name The name of the grouping
    explicit Grouping(const std::string& name);

    /// @brief Provides the name of the grouping
    const std::string& getName() const { return m_name; }

    /// @brief Sets the relationship to the given grouping
    /// @param[in] other The other party to set the relationship for
    /// @param[in] relationship Absolute value to set the relationship to
    ///                         Ranges between -100 and 100;
    void setRelationship(const std::string& other, int relationship);

    /// @brief Return the value of the relationship to the other party
    /// @param[in] other The other party to get the relationship for
    /// @returns The value of the relationship in the range (-100, 100)
    int getRelationship(const std::string& other) const;

    /// @brief Sets the relationship to apply by default to other groupings
    /// @param[in] relationship The default to apply
    void setDefaultRelationship(int relationship);

    /// @brief Add a new member to the group
    /// @param[in] id The entity to add
    void addMember(EntityId id);

    /// @brief Removes a member from the group
    /// @param[in] id The entity to remove
    void removeMember(EntityId id);

    /// @brief Provides access to the list of members
    /// @returns The list of members
    const std::set<EntityId>& getMembers() const { return m_members; }

    /// @brief Checks whether a given Entity is a member of the group
    /// @param[in] id The entity to find
    /// @returns True if the entity is a member of the group, false otherwise
    bool hasMember(EntityId id) const;

    /// @brief Sets the parent for this Grouping
    /// @param[in] parent The name of the parent Grouping
    void setParentGrouping(const std::string& parent) { m_parent = parent; }

    /// @brief Gets the parent for this Grouping
    /// @returns The name of the parent Grouping
    const std::string& getParentGrouping() { return m_parent; }

    bool operator<(const Grouping& rhs) const;

private:
    std::string m_name;
    std::map<std::string, int> m_relationships;
    int m_defaultRelationship = 0;
    std::set<EntityId> m_members;
    std::string m_parent;
};
