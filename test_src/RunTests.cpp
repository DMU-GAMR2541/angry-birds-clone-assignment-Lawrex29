#include <gtest/gtest.h>
//#include "Enemy.h"   - Did project 1 a little strange, changed to be pigs so this no longer works
#include "GameObject.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Slingshot.h"
#include "Pig.h"
#include "Block.h"
#include "Bird.h"

//Tests for GameObject:
// Using bird for testing to get around abstract class resiction
TEST(GameObjectTest, Constructor) {
	Bird bird(sf::Vector2f(100.f, 200.f));

	EXPECT_FLOAT_EQ(bird.getPosition().x, 100.f);
	EXPECT_FLOAT_EQ(bird.getPosition().y, 200.f);
}

TEST(GameObjectTest, GetsAndSets) {
	Bird bird;

	//Position
	bird.setPosition(sf::Vector2f(150.f, 250.f));
	EXPECT_FLOAT_EQ(bird.getPosition().x, 150.f);
	EXPECT_FLOAT_EQ(bird.getPosition().y, 250.f);
	
	//Rotation
	bird.setRotation(45.f);
	EXPECT_FLOAT_EQ(bird.getRotation(), 45.f);
}

TEST(GameObjectTest, DynamicObjNotStatic) {
	Bird bird;
	EXPECT_FALSE(bird.isStatic());
}

//Using block to test for static
TEST(GameObjectTest, StaticObjIsStatic) {
	Block block;
	EXPECT_TRUE(block.isStatic());
}

//Null Body till set
TEST(GameObjectTest, NullBody) {
	Bird bird;
	ASSERT_EQ(bird.getBody(), nullptr);
}

TEST(GameObjectTest, GlobalBoundsArePositive) {
	Bird bird;
	sf::FloatRect bounds = bird.getGlobalBounds();
	EXPECT_GE(bounds.left, 0.f);
	EXPECT_GE(bounds.top, 0.f);
	EXPECT_GE(bounds.width, 0.f);
	EXPECT_GE(bounds.height, 0.f);
}

//Using fixtures for leaf classes
//Bird Tests:
class BirdTest : public ::testing::Test {
protected:
    std::unique_ptr<Bird> bird;

    void SetUp() override {
        bird = std::make_unique<Bird>(sf::Vector2f(0.f, 0.f));
    }
};

TEST_F(BirdTest, RedIsDefault) {
	EXPECT_EQ(bird->getBirdType(), Bird::BirdType::Red);
}

TEST_F(BirdTest, GetsAreCorrect) {
	EXPECT_EQ(bird->getBirdType(), Bird::BirdType::Red);
	//Tested rest through GameObject
}

//Speed multipliers us the correct float
TEST_F(BirdTest, SpeedMultiplierRedCorrect) {
		EXPECT_FLOAT_EQ(bird->getSpeedMultiplier(), 2.8f);
	}
TEST_F(BirdTest, SpeedMultiplierBlueCorrect) {
	bird->setBirdType(Bird::BirdType::Blue);
	EXPECT_FLOAT_EQ(bird->getSpeedMultiplier(), 1.4f);
}
TEST_F(BirdTest, SpeedMultiplierYellowCorrect) {
	bird->setBirdType(Bird::BirdType::Yellow);
	EXPECT_FLOAT_EQ(bird->getSpeedMultiplier(), 0.7f);
}

TEST_F(BirdTest, SetBirdTypeChangesColourOfBirds) {
	bird->setBirdType(Bird::BirdType::Blue);
	EXPECT_EQ(bird->getBirdType(), Bird::BirdType::Blue);
	bird->setBirdType(Bird::BirdType::Yellow);
	EXPECT_EQ(bird->getBirdType(), Bird::BirdType::Yellow);
	bird->setBirdType(Bird::BirdType::Red);
	EXPECT_EQ(bird->getBirdType(), Bird::BirdType::Red);
}

//Collision Tests for birds
class BirdCollisionTest : public ::testing::Test {
protected:
    b2World* world = nullptr;

    void SetUp() override {
        b2Vec2 gravity(0.f, 0.f);
        world = new b2World(gravity);
    }

    Bird* launchNewBird(Bird::BirdType type, b2Vec2 velocity = { 10.f, 0.f }) {
        Bird* bird1 = new Bird(sf::Vector2f(0.f, 0.f));
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.f, 0.f);
        b2Body* body = world->CreateBody(&bodyDef);
        b2CircleShape shape;
        shape.m_radius = 1.f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.f;

        if (body == nullptr) {
            delete bird1;
            return nullptr;
        }

        body->CreateFixture(&fixtureDef);
        bird1->setBirdType(type);
        body->SetLinearVelocity(velocity);
        bird1->setBody(body);
        return bird1;
    }

    
    Block* newBlock(sf::Vector2f position, Block::BlockType blockType = Block::BlockType::Wood) {
        Block* block = new Block(position, 0.f, blockType);

        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(position.x / 30.f, position.y / 30.f);
        b2Body* body = world->CreateBody(&bodyDef);

        b2PolygonShape shape;

        b2FixtureDef fixtureDef;
        shape.SetAsBox(1.f, 1.f);
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.f;
        body->CreateFixture(&fixtureDef);
        block->setBody(body);

        if (body == nullptr) {
            delete block;
            return nullptr;
            }
        return block;
    }
    void TearDown() override {
        delete world;
        world = nullptr;
    }
};

//If collision successful pig health will be less than before
TEST_F(BirdCollisionTest, BirdDamagesPigOnCollision) {
    Bird* bird1 = launchNewBird(Bird::BirdType::Red);
    ASSERT_NE(bird1, nullptr);
    b2BodyDef bodyDef;
    Pig pig(sf::Vector2f(680.f, 555.f), 0.f, 1.f, 100);
    b2Body* pigBody = world->CreateBody(&bodyDef);
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(680.f / 30.f, 555.f / 30.f);

    b2CircleShape shape;
    shape.m_radius = 1.f;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.shape = &shape;
    pigBody->CreateFixture(&fixtureDef);

    ASSERT_NE(pigBody, nullptr);
    pig.setBody(pigBody);

    const int healthBefore = pig.getHealth();
    bird1->onCollision(pig);
    EXPECT_LT(pig.getHealth(), healthBefore);

    delete bird1;
}

//If collision succesful block health will be less than before
TEST_F(BirdCollisionTest, BlockDamagedByBirdWhenCOlliding) {
    Block* block = newBlock(sf::Vector2f(300.f, 400.f), Block::BlockType::Wood);
    ASSERT_NE(block, nullptr);

    Bird* bird = launchNewBird(Bird::BirdType::Red);
    ASSERT_NE(bird, nullptr);

    const int healthBefore = block->getHealth();
    bird->onCollision(*block);
    EXPECT_LT(block->getHealth(), healthBefore);

    delete bird;
    delete block;
}

//Red desroys ice
TEST_F(BirdCollisionTest, BlockShouldBeDestroyedIfDamagedEnough) {
    Block* block = newBlock(sf::Vector2f(300.f, 400.f), Block::BlockType::Ice);
    ASSERT_NE(block, nullptr);

    Bird* bird = launchNewBird(Bird::BirdType::Yellow, { 20.f, 0.f });
    ASSERT_NE(bird, nullptr);
    bird->onCollision(*block);
    EXPECT_TRUE(block->isDestroyed());

    delete bird;
    delete block;
}

//Doesn't get destroyed, blue doesn't deal enough damage to stone
TEST_F(BirdCollisionTest, BlockNonLethalDamage) {
    Block* block = newBlock(sf::Vector2f(300.f, 400.f), Block::BlockType::Stone);
    Bird* bird = launchNewBird(Bird::BirdType::Blue, { 1.f, 0.f });
    ASSERT_NE(block, nullptr);
    ASSERT_NE(bird, nullptr);

    bird->onCollision(*block);
    EXPECT_FALSE(block->isDestroyed());
    delete bird;
    delete block;
}

//Pig tests:
class PigTest : public ::testing::Test {
protected:
    Pig* pig = nullptr;
    void TearDown() override {
        delete pig;
        pig = nullptr;
    }

    void SetUp() override {
        pig = new Pig(sf::Vector2f(0.f, 0.f), 0.f, 1.f, 100);
    }
};

TEST_F(PigTest, ReturntypeOfPig) {
    EXPECT_EQ(pig->getType(), "Pig");
}

TEST_F(PigTest, HealthMatchSet100) {
    ASSERT_EQ(pig->getHealth(), 100);
}

TEST_F(PigTest, TakeNonFatalDamage) {
    pig->takeDamage(30);
    EXPECT_EQ(pig->getHealth(), 70);
    EXPECT_FALSE(pig->isDestroyed());
}

TEST_F(PigTest, TakeFatalDamage) {
    pig->takeDamage(100);
    EXPECT_TRUE(pig->isDestroyed());
    EXPECT_EQ(pig->getHealth(), 0);
}

TEST_F(PigTest, NonFatalIfHealthRemainsGT0) {
    pig->takeDamage(99);
    EXPECT_GT(pig->getHealth(), 0);
    EXPECT_FALSE(pig->isDestroyed());
}

TEST_F(PigTest, BlockCOllisionDealsDamage) {
    Block block(sf::Vector2f(5.f, 0.f), 0.f, Block::BlockType::Stone);
    ASSERT_FALSE(block.isDestroyed());
    pig->onCollision(block);
    EXPECT_LT(pig->getHealth(), 100);
}

//Fixture class for pig physics tests, has box2d and body to check collisions
class PigPhysicsTest : public ::testing::Test {
protected:
    b2World* world = nullptr;
    Pig* pig = nullptr;

    void SetUp() override {
        b2Vec2 gravity(0.f, 0.f);
        pig = new Pig(sf::Vector2f(0.f, 0.f), 0.f, 1.f, 100);
        b2BodyDef pigBodyDef;
        world = new b2World(gravity);
        pigBodyDef.type = b2_dynamicBody;
        pigBodyDef.position.Set(1.f, 0.f);
        b2Body* pigBody = world->CreateBody(&pigBodyDef);

        b2CircleShape pigShape;
        pigShape.m_radius = 1.f;
        b2FixtureDef pigFixture;
        pigFixture.density = 1.f;
        pigFixture.shape = &pigShape;
        pigBody->CreateFixture(&pigFixture);
        pig->setBody(pigBody);
    }
    void TearDown() override {
        delete pig;
        pig = nullptr;
        delete world;
        world = nullptr;
    }
};

//Bird collision test
TEST_F(PigPhysicsTest, BirdDealsDamage) {
    //Creates insatance of bird
    Bird bird(sf::Vector2f(0.f, 0.f));
    //Give bird body for collision
    b2BodyDef birdBodyDef;
    birdBodyDef.type = b2_dynamicBody;
    birdBodyDef.position.Set(0.f, 0.f);
    b2Body* birdBody = world->CreateBody(&birdBodyDef);

    //Shape for collision
    b2CircleShape birdShape;
    birdShape.m_radius = 1.f;

	//Fixture for collision
    b2FixtureDef birdFixture;
    birdFixture.shape = &birdShape;
    birdFixture.density = 1.f;
    birdBody->CreateFixture(&birdFixture);

    //Gives bird velocity to hit pig, damage is based of this
    birdBody->SetLinearVelocity(b2Vec2(10.f, 0.f));
    bird.setBody(birdBody);

    //If bird hits pig it should reduce the pigs health
    pig->onCollision(bird);
    EXPECT_LT(pig->getHealth(), 100);
}


//Block Tests
class BlockTest : public ::testing::Test {
protected:
    Block* wood = nullptr;
    Block* stone = nullptr;
    Block* ice = nullptr;
    void SetUp() override {
        wood = new Block(sf::Vector2f(0.f, 0.f), 0.f, Block::BlockType::Wood);
        stone = new Block(sf::Vector2f(0.f, 0.f), 0.f, Block::BlockType::Stone);
        ice = new Block(sf::Vector2f(0.f, 0.f), 0.f, Block::BlockType::Ice);
    }
    void TearDown() override {
        delete ice;   
        ice = nullptr;
        delete wood;  
        wood = nullptr;
        delete stone; 
        stone = nullptr;
    }
};

TEST_F(BlockTest, GetReturnsBlock)
{
    EXPECT_EQ(wood->getType(), "Block");
}

//CHecksBlocks are asigned correct health
TEST_F(BlockTest, WoodHealthCHeck)
{
    ASSERT_EQ(wood->getHealth(), 30);
}
TEST_F(BlockTest, StoneHealthCheck)
{
    ASSERT_EQ(stone->getHealth(), 60);
}
TEST_F(BlockTest, IceHealthcheck)
{
    ASSERT_EQ(ice->getHealth(), 15);
}


TEST_F(BlockTest, Getreturnscorrectblocktype)
{
    EXPECT_EQ(wood->getBlockType(), Block::BlockType::Wood);
    EXPECT_EQ(stone->getBlockType(), Block::BlockType::Stone);
    EXPECT_EQ(ice->getBlockType(), Block::BlockType::Ice);
}

TEST_F(BlockTest, TakeDamageCHeck)
{
    wood->takeDamage(10);
    EXPECT_EQ(wood->getHealth(), 20);
}

TEST_F(BlockTest, TakeDamagedestroyIf0orLess)
{
    wood->takeDamage(30);
    EXPECT_TRUE(wood->isDestroyed());
    EXPECT_EQ(wood->getHealth(), 0);
}



TEST_F(BlockTest, DifferentBlocksHaveDifferentHealth)
{
	// 20 destroys ice but not wood
    ice->takeDamage(20);
    wood->takeDamage(20);
    EXPECT_TRUE(ice->isDestroyed());
    EXPECT_FALSE(wood->isDestroyed());
}

TEST_F(BlockTest, BirdCollisiontoBlockDealsDamage)
{
    Bird bird;
    int healthBefore = wood->getHealth();
    wood->onCollision(bird);
    EXPECT_LT(wood->getHealth(), healthBefore);
}

TEST_F(BlockTest, PigCOllidesDealsDamageToPig)
{
    Pig pig;
    int healthBefore = stone->getHealth();
    stone->onCollision(pig);
    EXPECT_LT(stone->getHealth(), healthBefore);
}

//Dynamic and Static Object tests, was mostly covered by previous tests
class DynamicObjectTest : public ::testing::Test {
protected:

    void SetUp() override {
        obj = new DynamicObject();
    }
    void TearDown() override {
        delete obj;
        obj = nullptr;
    }
    DynamicObject* obj = nullptr;
};

TEST_F(DynamicObjectTest, GetTypeReturnsDynamicObject)
{
    EXPECT_EQ(obj->getType(), "DynamicObject");
}


class StaticObjectTest : public ::testing::Test {
    protected:
    StaticObject* obj = nullptr;
    void SetUp() override {
        obj = new StaticObject();
    }
    void TearDown() override {
        delete obj;
        obj = nullptr;
    }
};

TEST_F(StaticObjectTest, GetTypeReturnsStaticObject)
{
    EXPECT_EQ(obj->getType(), "StaticObject");
}


//Slinshot Tests

//Commented out everything for the first assignment, got a bit confused and didn't use the classes we were originally given in the annoyed flocks section as I thought they were copies not the same (sorry)
/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>
/// The fixture for testing class Slingshot.
//class SlingshotTest : public testing::Test {
//public:
//    std::unique_ptr<Slingshot> slingshot;
//protected:
//    // You can remove any or all of the following functions if their bodies would
//    // be empty.
//
//
//
//    SlingshotTest() {
//        // You can do set-up work for each test here.
//
//    }
//
//    ~SlingshotTest() override {
//        // You can do clean-up work that doesn't throw exceptions here.
//    }
//
//    // If the constructor and destructor are not enough for setting up
//    // and cleaning up each test, you can define the following methods:
//
//    void SetUp() override {
//        // Code here will be called immediately after the constructor (right
//        // before each test).
//        slingshot = std::make_unique<Slingshot>(); // All enemnies in this test suite start with 50 HP.
//
//    }
//
//    void TearDown() override {
//        // Code here will be called immediately after each test (right
//        // before the destructor).
//    }
//
//
//};

// The fixture for testing class Enemy.
//class EnemyTest : public testing::Test {
//public:
//    std::unique_ptr<Enemy> enemy;
//protected:
//    // You can remove any or all of the following functions if their bodies would
//    // be empty.
//
//    
//
//    EnemyTest() {
//        // You can do set-up work for each test here.
//                    
//    }
//
//    ~EnemyTest() override {
//        // You can do clean-up work that doesn't throw exceptions here.
//    }
//
//    // If the constructor and destructor are not enough for setting up
//    // and cleaning up each test, you can define the following methods:
//
//    void SetUp() override {
//        // Code here will be called immediately after the constructor (right
//        // before each test).
//        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
//                    
//    }
//
//    void TearDown() override {
//        // Code here will be called immediately after each test (right
//        // before the destructor).
//    }
//
//
//};


//Also ended up creating a different slingshot so these no longer work (Didn't fully understand that rununittests and annoyed flocks were using the same classes, thought they were copies at the start) these were for the first assignment tho so just commented out everything that was for that assignment
// 
// 
// 
// 
//Non fixture slingshot Bird Type tests expecting true, non fatal:
//TEST(Slingshot, First_birdType_expectTrue) {
//    Slingshot s;
//    EXPECT_TRUE(s.getBirdType() == "Red");
//}
//TEST(Slingshot, Second_birdType_expectTrue) {
//    Slingshot s;
//    EXPECT_TRUE(s.getBirdType() != "Blue");
//}
//
////Non fixture slingshot Bird Type tests expecting false, non fatal:
//TEST(Slingshot, First_birdType_expectFalse) {
//    Slingshot s;
//    EXPECT_FALSE(s.getBirdType() != "Red");
//}
//TEST(Slingshot, Second_birdType_expectFalse) {
//    Slingshot s;
//    EXPECT_FALSE(s.getBirdType() == "Blue");
//}
//
////Non fixture slingshot bird type tests, non fatal
//TEST(Slingshot, First_birdType_expectEQ) {
//    Slingshot s;
//    EXPECT_EQ(s.getBirdType(), "Red");
//}
//TEST(Slingshot, Second_birdType_expectNE) {
//    Slingshot s;
//    EXPECT_NE(s.getBirdType(), "Blue");
//}
//
////Non fixture slingshot Bird Type tests expecting true, fatal:
//TEST(Slingshot, First_birdType_expectTrue_fatal) {
//    Slingshot s;
//    ASSERT_TRUE(s.getBirdType() == "Red");
//}
//TEST(Slingshot, Second_birdType_expectTrue_fatal) {
//    Slingshot s;
//    ASSERT_TRUE(s.getBirdType() != "Blue");
//}
//
////Non fixture slingshot Bird Type tests expecting false, fatal:
//TEST(Slingshot, First_birdType_expectFalse_fatal) {
//    Slingshot s;
//    ASSERT_FALSE(s.getBirdType() != "Red");
//}
//TEST(Slingshot, Second_birdType_expectFalse_fatal) {
//    Slingshot s;
//    ASSERT_FALSE(s.getBirdType() == "Blue");
//}
//
////Non fixture slingshot bird type tests, fatal
//TEST(Slingshot, First_birdType_expectEQ_fatal) {
//    Slingshot s;
//    ASSERT_EQ(s.getBirdType(), "Red");
//}
//TEST(Slingshot, Second_BirdType_ExpectNE_fatal) {
//    Slingshot s;
//    ASSERT_NE(s.getBirdType(), "Blue");
//}
//
////Non fixture slingshot tension tests, non fatal:
//TEST(Slingshot, First_tension) {
//    Slingshot s;
//    EXPECT_EQ(s.getTension(), 0);
//}
//TEST(Slingshot, Second_tension) {
//    Slingshot s;
//    EXPECT_LT(s.getTension(), 1);
//}
//TEST(Slingshot, Third_tension) {
//    Slingshot s;
//    EXPECT_LE(s.getTension(), 0);
//}
//TEST(Slingshot, Fourth_tension) {
//    Slingshot s;
//    EXPECT_GT(s.getTension(), -1);
//}
//TEST(Slingshot, Fith_tention) {
//    Slingshot s;
//    EXPECT_GE(s.getTension(), 0);
//}
//TEST(Slingshot, Sixth_tention) {
//    Slingshot s;
//    EXPECT_NE(s.getTension(), 2);
//}
//
////Fixture slingshot tension tests, non fatal:
//TEST_F(SlingshotTest, First_tention) {
//    EXPECT_EQ(slingshot->getTension(), 0);
//}
//TEST_F(SlingshotTest, Second_tention) {
//    EXPECT_NE(slingshot->getTension(), 2);
//}
//TEST_F(SlingshotTest, Third_tention) {
//    EXPECT_LT(slingshot->getTension(), 1);
//}
//TEST_F(SlingshotTest, Fourth_tention) {
//    EXPECT_LE(slingshot->getTension(), 0);
//}
//TEST_F(SlingshotTest, Fith_tention) {
//    EXPECT_GT(slingshot->getTension(), -1);
//}
//TEST_F(SlingshotTest, Sixth_tention) {
//    EXPECT_GE(slingshot->getTension(), 0);
//}
//
////Non fixture slingshot tension tests, fatal:
//TEST(Slingshot, First_tention_fatal) {
//    Slingshot s;
//    ASSERT_EQ(s.getTension(), 0);
//}
//TEST(Slingshot, Second_tention_fatal) {
//    Slingshot s;
//    ASSERT_LT(s.getTension(), 1);
//}
//TEST(Slingshot, Third_tention_fatal) {
//    Slingshot s;
//    ASSERT_LE(s.getTension(), 0);
//}
//TEST(Slingshot, Fourth_tention_fatal) {
//    Slingshot s;
//    ASSERT_GT(s.getTension(), -1);
//}
//TEST(Slingshot, Fith_tention_fatal) {
//    Slingshot s;
//    ASSERT_GE(s.getTension(), 0);
//}
//TEST(Slingshot, Sixth_tention_fatal) {
//    Slingshot s;
//    ASSERT_NE(s.getTension(), 2);
//}
//
////Fixture slingshot tension tests, fatal:
//TEST_F(SlingshotTest, First_tention_fatal) {
//    ASSERT_EQ(slingshot->getTension(), 0);
//}
//TEST_F(SlingshotTest, Second_tention_fatal) {
//    ASSERT_NE(slingshot->getTension(), 2);
//}
//TEST_F(SlingshotTest, Third_tention_fatal) {
//    ASSERT_LT(slingshot->getTension(), 1);
//}
//TEST_F(SlingshotTest, Fourth_tention_fatal) {
//    ASSERT_LE(slingshot->getTension(), 0);
//}
//TEST_F(SlingshotTest, Fith_tention_fatal) {
//    ASSERT_GT(slingshot->getTension(), -1);
//}
//TEST_F(SlingshotTest, Sixth_tention_fatal) {
//    ASSERT_GE(slingshot->getTension(), 0);
//}
//
////A single test, not a fixture. No setup is called chacking enemy health.
//TEST(Enemy, First_enemyHealth) {
//    Enemy e(100);
//    EXPECT_EQ(e.getHealth(), 100);
//}
//TEST(Enemy, Second_enemyHealth) {
//    Enemy e(100);
//    EXPECT_NE(e.getHealth(), 101);
//}
//TEST(Enemy, Third_enemyHealth) {
//    Enemy e(100);
//    EXPECT_GT(e.getHealth(),99);
//}
//TEST(Enemy, Fourth_enemyHealth) {
//    Enemy e(100);
//    EXPECT_GE(e.getHealth(), 100);
//}
//TEST(Enemy, Fith_enemyHealth) {
//    Enemy e(100);
//    EXPECT_LT(e.getHealth(), 101);
//}
//TEST(Enemy, Sixth_enemyHealth) {
//    Enemy e(100);
//    EXPECT_LE(e.getHealth(), 100);
//}
//
////Non fixture checking enemy health fatal tests:
//TEST(Enemy, First_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_EQ(e.getHealth(), 100);
//}
//TEST(Enemy, Second_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_NE(e.getHealth(), 101);
//}
//TEST(Enemy, Third_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_GT(e.getHealth(), 99);
//}
//TEST(Enemy, Fourth_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_GE(e.getHealth(), 100);
//}
//TEST(Enemy, Fith_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_LT(e.getHealth(), 101);
//}
//TEST(Enemy, Sixth_enemyHealth_fatal) {
//    Enemy e(100);
//    ASSERT_LE(e.getHealth(), 100);
//}
//
////Fixture chacking enemy health, non fatal:
//TEST_F(EnemyTest, First_enemyHealth) {
//    EXPECT_EQ(enemy->getHealth(), 50);
//}
//TEST_F(EnemyTest, Second_enemyHealthtest) {
//    EXPECT_NE(enemy->getHealth(), 51);
//}
//TEST_F(EnemyTest, Third_enemyHealth) {
//    EXPECT_GT(enemy->getHealth(), 49);
//}
//TEST_F(EnemyTest, Fourth_enemyHealth) {
//    EXPECT_GE(enemy->getHealth(), 50);
//}
//TEST_F(EnemyTest, Fith_enemyHealth) {
//    EXPECT_LT(enemy->getHealth(), 51);
//}
//TEST_F(EnemyTest, Sixth_enemyHealth) {
//    EXPECT_LE(enemy->getHealth(), 50);
//}
//
////Fixture checking enemy health, fatal:
//TEST_F(EnemyTest, First_enemyHealth_fatal) {
//    ASSERT_EQ(enemy->getHealth(), 50);
//}
//TEST_F(EnemyTest, Second_enemyHealth_fatal) {
//    ASSERT_NE(enemy->getHealth(), 51);
//}
//TEST_F(EnemyTest, Third_enemyHealth_fatal) {
//    ASSERT_GT(enemy->getHealth(), 49);
//}
//TEST_F(EnemyTest, Fourth_enemyHealth_fatal) {
//    ASSERT_GE(enemy->getHealth(), 50);
//}
//TEST_F(EnemyTest, Fith_enemyHealth_fatal) {
//    ASSERT_LT(enemy->getHealth(), 51);
//}
//TEST_F(EnemyTest, Sixth_enemyHealth_fatal) {
//    ASSERT_LE(enemy->getHealth(), 50);
//}
//
////Fixture checking enemy death, non fatal:
//TEST_F(EnemyTest, LethalDamagePopsPig) {
//    enemy->takeDamage(60);
//    EXPECT_TRUE(enemy->checkIfPopped());
//}
//TEST_F(EnemyTest, NonLethalDamagePopsPig) {
//    enemy->takeDamage(30);
//    EXPECT_FALSE(enemy->checkIfPopped());
//}
//
////Fixture checking enemy death, fatal:
//TEST_F(EnemyTest, LethalDamagePopsPigFatal) {
//    enemy->takeDamage(60);
//    ASSERT_TRUE(enemy->checkIfPopped());
//}
//TEST_F(EnemyTest, NonLethalDamagePopsPigFatal) {
//    enemy->takeDamage(30);
//    ASSERT_FALSE(enemy->checkIfPopped());
//}
//
////Non fixture checking enemy death, non fatal:
//TEST(Enemy, LethalDamagePopsPig) {
//    Enemy e(50);
//    e.takeDamage(60);
//    EXPECT_TRUE(e.checkIfPopped());
//}
//TEST(Enemy, NonLethalDamagePopsPig) {
//    Enemy e(100);
//    e.takeDamage(30);
//    EXPECT_FALSE(e.checkIfPopped());
//}
//
////non fixture checking enemy death, fatal:
//TEST(Enemy, LethalDamagePopsPigFatal) {
//    Enemy e(50);
//    e.takeDamage(60);
//    ASSERT_TRUE(e.checkIfPopped());
//}
//TEST(Enemy, NonLethalDamagePopsPigFatal) {
//    Enemy e(100);
//    e.takeDamage(30);
//    ASSERT_FALSE(e.checkIfPopped());
//}
//
////Runs all tests using google tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}