#include <gtest/gtest.h>

#include "besm-666/exec/csr.hpp"
#include "besm-666/exec/csrf.hpp"

TEST(CSRStructure, Dummy) {
    using namespace besm;

    using Field1Type = exec::CSRUncheckedField<0b11>;
    using Field2Type = exec::CSRUncheckedField<0b1100>;
    using Field3Type = exec::CSRWPRIField<0b1110000>;

    exec::CSRF csrf;
    exec::CSRStructure<Field1Type, Field2Type, Field3Type> reg(
        csrf, exec::ICSR::NUM_IDS);

    EXPECT_TRUE(reg.set<Field1Type>(3));
    EXPECT_EQ(reg.get<Field1Type>(), 3);

    EXPECT_TRUE(reg.set<Field2Type>(2));
    EXPECT_EQ(reg.get<Field2Type>(), 2);

    EXPECT_EQ(reg.write(0b1010110), Exception::NO_EXCEPTION);
    EXPECT_EQ(reg.get<Field1Type>(), 0b10);
    EXPECT_EQ(reg.get<Field2Type>(), 0b01);
    EXPECT_EQ(reg.get<Field3Type>(), 0); // WPRI Specification WORKS !!!!
}
