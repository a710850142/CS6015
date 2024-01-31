#include "catch.h"
#include "expr.h"

TEST_CASE("Num equals") {
    Num num1(5);
    Num num2(5);
    Num num3(3);

    REQUIRE(num1.equals(&num2));
    REQUIRE_FALSE(num1.equals(&num3));
}

TEST_CASE("Add equals") {
    Add add1(new Num(2), new Num(3));
    Add add2(new Num(2), new Num(3));
    Add add3(new Num(3), new Num(2));

    REQUIRE(add1.equals(&add2));
    REQUIRE_FALSE(add1.equals(&add3));
}

TEST_CASE("Mult equals") {
    Mult mult1(new Num(4), new Num(2));
    Mult mult2(new Num(4), new Num(2));
    Mult mult3(new Num(2), new Num(4));

    REQUIRE(mult1.equals(&mult2));
    REQUIRE_FALSE(mult1.equals(&mult3));
}

TEST_CASE("VarExpr equals") {
    VarExpr var1("x");
    VarExpr var2("x");
    VarExpr var3("y");

    REQUIRE(var1.equals(&var2));
    REQUIRE_FALSE(var1.equals(&var3));

}
//HW3 Test
TEST_CASE("Num interp, has_variable, and subst") {
    Num num(5);

    SECTION("interp") {
        REQUIRE(num.interp() == 5);
    }

    SECTION("has_variable") {
        REQUIRE_FALSE(num.has_variable());
    }

    SECTION("subst") {
        REQUIRE(num.subst("x", new Num(10))->equals(&num));
    }
}
TEST_CASE("Add interp, has_variable, and subst") {
    Add add(new Num(3), new Num(4));
    Add addWithVar(new VarExpr("x"), new Num(7));

    SECTION("interp") {
        REQUIRE(add.interp() == 7);
    }

    SECTION("has_variable") {
        REQUIRE_FALSE(add.has_variable());
        REQUIRE(addWithVar.has_variable());
    }

    SECTION("subst") {
        REQUIRE(addWithVar.subst("x", new Num(5))->equals(new Add(new Num(5), new Num(7))));
    }
}

TEST_CASE("Mult interp, has_variable, and subst") {
    Mult mult(new Num(4), new Num(2));
    Mult multWithVar(new VarExpr("x"), new Num(7));

    SECTION("interp") {
        REQUIRE(mult.interp() == 8);
    }

    SECTION("has_variable") {
        REQUIRE_FALSE(mult.has_variable());
        REQUIRE(multWithVar.has_variable());
    }

    SECTION("subst") {
        REQUIRE(multWithVar.subst("x", new Num(5))->equals(new Mult(new Num(5), new Num(7))));
    }
}

TEST_CASE("VarExpr interp, has_variable, and subst") {
    VarExpr varExpr("x");

    SECTION("interp") {
        REQUIRE_THROWS_WITH(varExpr.interp(), "No value for variable");
    }

    SECTION("has_variable") {
        REQUIRE(varExpr.has_variable());
    }

    SECTION("subst") {
        REQUIRE(varExpr.subst("x", new Num(5))->equals(new Num(5)));
        REQUIRE(varExpr.subst("y", new Num(5))->equals(&varExpr));
    }
}