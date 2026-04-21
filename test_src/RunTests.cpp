#include <gtest/gtest.h>
#include "Enemy.h"

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    

    EnemyTest() {
        // You can do set-up work for each test here.
                    
    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {
    Enemy e(100);
    EXPECT_EQ(e.getHealth(), 100);
}

TEST(Enemy, Second_test) {
    Enemy e(100);
    EXPECT_NE(e.getHealth(), 101);
}

TEST(Enemy, Third_test) {
    Enemy e(100);
    EXPECT_GT(e.getHealth(),99);
}

TEST(Enemy, Fourth_test) {
    Enemy e(100);
    EXPECT_GE(e.getHealth(), 100);
}

TEST(Enemy, Fith_test) {
    Enemy e(100);
    EXPECT_LT(e.getHealth(), 101);
}

TEST(Enemy, Sixth_test) {
    Enemy e(100);
    EXPECT_LE(e.getHealth(), 100);
}

TEST(Enemy, First_test_fatal) {
    Enemy e(100);
    ASSERT_EQ(e.getHealth(), 100);
}

TEST(Enemy, Second_test_fatal) {
    Enemy e(100);
    ASSERT_NE(e.getHealth(), 101);
}

TEST(Enemy, Third_test_fatal) {
    Enemy e(100);
    ASSERT_GT(e.getHealth(), 99);
}

TEST(Enemy, Fourth_test_fatal) {
    Enemy e(100);
    ASSERT_GE(e.getHealth(), 100);
}

TEST(Enemy, Fith_test_fatal) {
    Enemy e(100);
    ASSERT_LT(e.getHealth(), 101);
}

TEST(Enemy, Sixth_test_fatal) {
    Enemy e(100);
    ASSERT_LE(e.getHealth(), 100);
}

TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, NonLethalDamagePopsPig) {
    enemy->takeDamage(30);
    EXPECT_FALSE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, LethalDamagePopsPigFatal) {
    enemy->takeDamage(60);
    ASSERT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, NonLethalDamagePopsPigFatal) {
    enemy->takeDamage(30);
    ASSERT_FALSE(enemy->checkIfPopped());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}