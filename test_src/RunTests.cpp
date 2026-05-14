#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>
/// The fixture for testing class Slingshot.
class SlingshotTest : public testing::Test {
public:
    std::unique_ptr<Slingshot> slingshot;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.



    SlingshotTest() {
        // You can do set-up work for each test here.

    }

    ~SlingshotTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        slingshot = std::make_unique<Slingshot>(); // All enemnies in this test suite start with 50 HP.

    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

// The fixture for testing class Enemy.
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

//Non fixture slingshot Bird Type tests expecting true, non fatal:
TEST(Slingshot, First_birdType_expectTrue) {
    Slingshot s;
    EXPECT_TRUE(s.getBirdType() == "Red");
}
TEST(Slingshot, Second_birdType_expectTrue) {
    Slingshot s;
    EXPECT_TRUE(s.getBirdType() != "Blue");
}

//Non fixture slingshot Bird Type tests expecting false, non fatal:
TEST(Slingshot, First_birdType_expectFalse) {
    Slingshot s;
    EXPECT_FALSE(s.getBirdType() != "Red");
}
TEST(Slingshot, Second_birdType_expectFalse) {
    Slingshot s;
    EXPECT_FALSE(s.getBirdType() == "Blue");
}

//Non fixture slingshot bird type tests, non fatal
TEST(Slingshot, First_birdType_expectEQ) {
    Slingshot s;
    EXPECT_EQ(s.getBirdType(), "Red");
}
TEST(Slingshot, Second_birdType_expectNE) {
    Slingshot s;
    EXPECT_NE(s.getBirdType(), "Blue");
}

//Non fixture slingshot Bird Type tests expecting true, fatal:
TEST(Slingshot, First_birdType_expectTrue_fatal) {
    Slingshot s;
    ASSERT_TRUE(s.getBirdType() == "Red");
}
TEST(Slingshot, Second_birdType_expectTrue_fatal) {
    Slingshot s;
    ASSERT_TRUE(s.getBirdType() != "Blue");
}

//Non fixture slingshot Bird Type tests expecting false, fatal:
TEST(Slingshot, First_birdType_expectFalse_fatal) {
    Slingshot s;
    ASSERT_FALSE(s.getBirdType() != "Red");
}
TEST(Slingshot, Second_birdType_expectFalse_fatal) {
    Slingshot s;
    ASSERT_FALSE(s.getBirdType() == "Blue");
}

//Non fixture slingshot bird type tests, fatal
TEST(Slingshot, First_birdType_expectEQ_fatal) {
    Slingshot s;
    ASSERT_EQ(s.getBirdType(), "Red");
}
TEST(Slingshot, Second_BirdType_ExpectNE_fatal) {
    Slingshot s;
    ASSERT_NE(s.getBirdType(), "Blue");
}

//Non fixture slingshot tension tests, non fatal:
TEST(Slingshot, First_tension) {
    Slingshot s;
    EXPECT_EQ(s.getTension(), 0);
}
TEST(Slingshot, Second_tension) {
    Slingshot s;
    EXPECT_LT(s.getTension(), 1);
}
TEST(Slingshot, Third_tension) {
    Slingshot s;
    EXPECT_LE(s.getTension(), 0);
}
TEST(Slingshot, Fourth_tension) {
    Slingshot s;
    EXPECT_GT(s.getTension(), -1);
}
TEST(Slingshot, Fith_tention) {
    Slingshot s;
    EXPECT_GE(s.getTension(), 0);
}
TEST(Slingshot, Sixth_tention) {
    Slingshot s;
    EXPECT_NE(s.getTension(), 2);
}

//Fixture slingshot tension tests, non fatal:
TEST_F(SlingshotTest, First_tention) {
    EXPECT_EQ(slingshot->getTension(), 0);
}
TEST_F(SlingshotTest, Second_tention) {
    EXPECT_NE(slingshot->getTension(), 2);
}
TEST_F(SlingshotTest, Third_tention) {
    EXPECT_LT(slingshot->getTension(), 1);
}
TEST_F(SlingshotTest, Fourth_tention) {
    EXPECT_LE(slingshot->getTension(), 0);
}
TEST_F(SlingshotTest, Fith_tention) {
    EXPECT_GT(slingshot->getTension(), -1);
}
TEST_F(SlingshotTest, Sixth_tention) {
    EXPECT_GE(slingshot->getTension(), 0);
}

//Non fixture slingshot tension tests, fatal:
TEST(Slingshot, First_tention_fatal) {
    Slingshot s;
    ASSERT_EQ(s.getTension(), 0);
}
TEST(Slingshot, Second_tention_fatal) {
    Slingshot s;
    ASSERT_LT(s.getTension(), 1);
}
TEST(Slingshot, Third_tention_fatal) {
    Slingshot s;
    ASSERT_LE(s.getTension(), 0);
}
TEST(Slingshot, Fourth_tention_fatal) {
    Slingshot s;
    ASSERT_GT(s.getTension(), -1);
}
TEST(Slingshot, Fith_tention_fatal) {
    Slingshot s;
    ASSERT_GE(s.getTension(), 0);
}
TEST(Slingshot, Sixth_tention_fatal) {
    Slingshot s;
    ASSERT_NE(s.getTension(), 2);
}

//Fixture slingshot tension tests, fatal:
TEST_F(SlingshotTest, First_tention_fatal) {
    ASSERT_EQ(slingshot->getTension(), 0);
}
TEST_F(SlingshotTest, Second_tention_fatal) {
    ASSERT_NE(slingshot->getTension(), 2);
}
TEST_F(SlingshotTest, Third_tention_fatal) {
    ASSERT_LT(slingshot->getTension(), 1);
}
TEST_F(SlingshotTest, Fourth_tention_fatal) {
    ASSERT_LE(slingshot->getTension(), 0);
}
TEST_F(SlingshotTest, Fith_tention_fatal) {
    ASSERT_GT(slingshot->getTension(), -1);
}
TEST_F(SlingshotTest, Sixth_tention_fatal) {
    ASSERT_GE(slingshot->getTension(), 0);
}

//A single test, not a fixture. No setup is called chacking enemy health.
TEST(Enemy, First_enemyHealth) {
    Enemy e(100);
    EXPECT_EQ(e.getHealth(), 100);
}
TEST(Enemy, Second_enemyHealth) {
    Enemy e(100);
    EXPECT_NE(e.getHealth(), 101);
}
TEST(Enemy, Third_enemyHealth) {
    Enemy e(100);
    EXPECT_GT(e.getHealth(),99);
}
TEST(Enemy, Fourth_enemyHealth) {
    Enemy e(100);
    EXPECT_GE(e.getHealth(), 100);
}
TEST(Enemy, Fith_enemyHealth) {
    Enemy e(100);
    EXPECT_LT(e.getHealth(), 101);
}
TEST(Enemy, Sixth_enemyHealth) {
    Enemy e(100);
    EXPECT_LE(e.getHealth(), 100);
}

//Non fixture checking enemy health fatal tests:
TEST(Enemy, First_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_EQ(e.getHealth(), 100);
}
TEST(Enemy, Second_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_NE(e.getHealth(), 101);
}
TEST(Enemy, Third_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_GT(e.getHealth(), 99);
}
TEST(Enemy, Fourth_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_GE(e.getHealth(), 100);
}
TEST(Enemy, Fith_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_LT(e.getHealth(), 101);
}
TEST(Enemy, Sixth_enemyHealth_fatal) {
    Enemy e(100);
    ASSERT_LE(e.getHealth(), 100);
}

//Fixture chacking enemy health, non fatal:
TEST_F(EnemyTest, First_enemyHealth) {
    EXPECT_EQ(enemy->getHealth(), 50);
}
TEST_F(EnemyTest, Second_enemyHealthtest) {
    EXPECT_NE(enemy->getHealth(), 51);
}
TEST_F(EnemyTest, Third_enemyHealth) {
    EXPECT_GT(enemy->getHealth(), 49);
}
TEST_F(EnemyTest, Fourth_enemyHealth) {
    EXPECT_GE(enemy->getHealth(), 50);
}
TEST_F(EnemyTest, Fith_enemyHealth) {
    EXPECT_LT(enemy->getHealth(), 51);
}
TEST_F(EnemyTest, Sixth_enemyHealth) {
    EXPECT_LE(enemy->getHealth(), 50);
}

//Fixture checking enemy health, fatal:
TEST_F(EnemyTest, First_enemyHealth_fatal) {
    ASSERT_EQ(enemy->getHealth(), 50);
}
TEST_F(EnemyTest, Second_enemyHealth_fatal) {
    ASSERT_NE(enemy->getHealth(), 51);
}
TEST_F(EnemyTest, Third_enemyHealth_fatal) {
    ASSERT_GT(enemy->getHealth(), 49);
}
TEST_F(EnemyTest, Fourth_enemyHealth_fatal) {
    ASSERT_GE(enemy->getHealth(), 50);
}
TEST_F(EnemyTest, Fith_enemyHealth_fatal) {
    ASSERT_LT(enemy->getHealth(), 51);
}
TEST_F(EnemyTest, Sixth_enemyHealth_fatal) {
    ASSERT_LE(enemy->getHealth(), 50);
}

//Fixture checking enemy death, non fatal:
TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}
TEST_F(EnemyTest, NonLethalDamagePopsPig) {
    enemy->takeDamage(30);
    EXPECT_FALSE(enemy->checkIfPopped());
}

//Fixture checking enemy death, fatal:
TEST_F(EnemyTest, LethalDamagePopsPigFatal) {
    enemy->takeDamage(60);
    ASSERT_TRUE(enemy->checkIfPopped());
}
TEST_F(EnemyTest, NonLethalDamagePopsPigFatal) {
    enemy->takeDamage(30);
    ASSERT_FALSE(enemy->checkIfPopped());
}

//Non fixture checking enemy death, non fatal:
TEST(Enemy, LethalDamagePopsPig) {
    Enemy e(50);
    e.takeDamage(60);
    EXPECT_TRUE(e.checkIfPopped());
}
TEST(Enemy, NonLethalDamagePopsPig) {
    Enemy e(100);
    e.takeDamage(30);
    EXPECT_FALSE(e.checkIfPopped());
}

//non fixture checking enemy death, fatal:
TEST(Enemy, LethalDamagePopsPigFatal) {
    Enemy e(50);
    e.takeDamage(60);
    ASSERT_TRUE(e.checkIfPopped());
}
TEST(Enemy, NonLethalDamagePopsPigFatal) {
    Enemy e(100);
    e.takeDamage(30);
    ASSERT_FALSE(e.checkIfPopped());
}

//Runs all tests using google tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}