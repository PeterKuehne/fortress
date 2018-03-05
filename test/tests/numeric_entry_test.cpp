#include <gtest/gtest.h>
#include "../../src/core/utility.h"
#include "../../src/windows/numeric_entry.h"
#include "../mocks/graphics_mock.h"

using namespace ::testing;

TEST(NumericEntry, drawsNumber)
{
    GraphicsMock graphics;
    NumericEntry num;

    const char* text = nullptr;
    EXPECT_CALL(graphics, drawString(_, Ne(1), _, _, _)).WillRepeatedly(Return(0));
    EXPECT_CALL(graphics, drawString(_, Eq(1), _,_,_)).WillOnce(SaveArg<2>(&text));

    num.setGraphics(&graphics);
    num.setNumber(23);

    num.render();

    EXPECT_STREQ("23", text);
}

TEST(NumericEntry, DoesNotPrintAngleBracketsWhenNotSensitive)
{
    GraphicsMock graphics;
    NumericEntry num;

    const char* text = nullptr;
    EXPECT_CALL(graphics, drawString(_, Ne(1), _, _, _)).Times(0);
    EXPECT_CALL(graphics, drawString(_, Eq(1), _,_,_)).WillOnce(SaveArg<2>(&text));

    num.setGraphics(&graphics);
    num.setNumber(23);
    num.setSensitive(false);

    num.render();

    EXPECT_STREQ("23", text);
}

TEST(NumericEntry, PlusKeyIncreases)
{
    NumericEntry num;
    num.setNumber(23);

    num.keyPress('+');

    EXPECT_EQ(num.getNumber(), 24);
}

TEST(NumericEntry, MinusKeyIncreases)
{
    NumericEntry num;
    num.setNumber(23);

    num.keyPress('-');

    EXPECT_EQ(num.getNumber(), 22);
}

TEST(NumericEntry, RightArrowKeyIncreases)
{
    NumericEntry num;
    num.setNumber(23);

    num.keyPress(KEY_RIGHT);

    EXPECT_EQ(num.getNumber(), 24);
}

TEST(NumericEntry, LeftArrowKeyIncreases)
{
    NumericEntry num;
    num.setNumber(23);

    num.keyPress(KEY_LEFT);

    EXPECT_EQ(num.getNumber(), 22);
}

TEST(NumericEntry, WidthIsSetToTextSize)
{
    NumericEntry num;
    num.setNumber(100);

    EXPECT_EQ(num.getWidth(), 5);
}
