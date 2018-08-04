#include "../../src/components/component_base.h"
#include "../../src/core/component_manager.h"
#include <gtest/gtest.h>

using namespace ::testing;
struct TestComponent : public ComponentBase {
    int indicator = 0;
};

struct Test2Component : public ComponentBase {
    //
};

class ComponentManagerTest : public ::testing::Test {
public:
    ComponentManager manager;
    EntityId entity = 1234;
};

TEST_F(ComponentManagerTest, makeCreatesNewComponent) {
    // Given
    manager.make<TestComponent>(entity);

    // When
    auto test = manager.get<TestComponent>(entity);
    auto test2 = manager.get<Test2Component>(entity);

    // Then
    EXPECT_NE(nullptr, test);
    EXPECT_EQ(nullptr, test2);
}

TEST_F(ComponentManagerTest, makeDoesNotAddSameComponentTwice) {
    // Given
    manager.make<TestComponent>(entity);
    manager.get<TestComponent>(entity)->indicator = 1;

    // When
    manager.make<TestComponent>(entity);

    // Then
    auto test = manager.get<TestComponent>(entity);
    EXPECT_EQ(1, test->indicator);
}

TEST_F(ComponentManagerTest, getReturnsNullptrIfNotFound) {
    // Given

    // When
    auto test = manager.get<TestComponent>(entity);

    // Then
    EXPECT_EQ(nullptr, test);
}

TEST_F(ComponentManagerTest, returnedComponentsAreDistinct) {
    // Given
    manager.make<TestComponent>(entity);
    manager.make<Test2Component>(entity);

    // When
    auto test = manager.get<TestComponent>(entity);
    auto test2 = manager.get<Test2Component>(entity);

    // Then
    EXPECT_NE(nullptr, test);
    EXPECT_NE(nullptr, test2);
    EXPECT_NE(static_cast<ComponentBase*>(test),
              static_cast<ComponentBase*>(test2));
}

TEST_F(ComponentManagerTest, getAllReturnsAllMakedComponents) {
    // Given
    manager.make<TestComponent>(entity);
    manager.make<Test2Component>(entity);

    // When
    auto holder = manager.getAll(entity);

    // Then
    EXPECT_EQ(2, holder.size());
}

TEST_F(ComponentManagerTest, removeOnlyRemovesSpecificComponent) {
    // Given
    manager.make<TestComponent>(entity);
    manager.make<Test2Component>(entity);

    // When
    manager.remove<Test2Component>(entity);

    // Then
    auto test = manager.get<TestComponent>(entity);
    auto test2 = manager.get<Test2Component>(entity);
    auto holder = manager.getAll(entity);

    EXPECT_NE(nullptr, test);
    EXPECT_EQ(nullptr, test2);
    EXPECT_EQ(1, holder.size());
}

TEST_F(ComponentManagerTest, removesNothingIfNotFound) {
    // Given
    manager.make<TestComponent>(entity);

    // When
    manager.remove<Test2Component>(entity);

    // Then
    auto test = manager.get<TestComponent>(entity);
    auto test2 = manager.get<Test2Component>(entity);
    auto holder = manager.getAll(entity);

    EXPECT_NE(nullptr, test);
    EXPECT_EQ(nullptr, test2);
    EXPECT_EQ(1, holder.size());
}

TEST_F(ComponentManagerTest, removeAllRemovesAll) {
    // Given
    manager.make<TestComponent>(entity);
    manager.make<Test2Component>(entity);

    // When
    manager.removeAll(entity);

    // Then
    auto test = manager.get<TestComponent>(entity);
    auto test2 = manager.get<Test2Component>(entity);
    auto holder = manager.getAll(entity);

    EXPECT_EQ(nullptr, test);
    EXPECT_EQ(nullptr, test2);
    EXPECT_EQ(0, holder.size());
}