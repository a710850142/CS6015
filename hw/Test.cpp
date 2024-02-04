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
TEST_CASE("Complex Expression Evaluation") {
    Add complexExpr(new Add(new Num(2), new Mult(new Num(3), new VarExpr("x"))), new Num(4));
    REQUIRE_THROWS_WITH(complexExpr.interp(), "No value for variable");
    Expr* substitutedExpr = complexExpr.subst("x", new Num(2));
    REQUIRE(substitutedExpr->interp() == 12);
    delete substitutedExpr;
}
TEST_CASE("Edge Cases") {
    Add zeroTest(new Num(0), new Num(0));
    REQUIRE(zeroTest.interp() == 0);

    Mult negativeTest(new Num(-1), new Num(5));
    REQUIRE(negativeTest.interp() == -5);
}
TEST_CASE("Equality on Complex Expressions") {
    Add expr1(new Num(1), new Mult(new Num(2), new VarExpr("x")));
    Add expr2(new Num(1), new Mult(new Num(2), new VarExpr("x")));
    REQUIRE(expr1.equals(&expr2));
}


TEST_CASE("MultExpr and NumExpr Interpolation") {
    CHECK((new Mult(new Num(3), new Num(2)))->interp() == 6);
}

TEST_CASE("Nested AddExpr Interpolation") {
    CHECK((new Add(new Add(new Num(10), new Num(15)), new Add(new Num(20), new Num(20))))->interp() == 65);
}

TEST_CASE("Variable Presence Check") {
    Add* addExpr = new Add(new VarExpr("x"), new Num(1));
    CHECK(addExpr->has_variable() == true);
    delete addExpr; // 清理动态分配的内存

    Mult* multExpr = new Mult(new Num(2), new Num(1));
    CHECK(multExpr->has_variable() == false);
    delete multExpr; // 清理动态分配的内存
}


TEST_CASE("Substitution and Equality Checks") {
    CHECK((new Add(new VarExpr("x"), new Num(7)))
                  ->subst("x", new VarExpr("y"))
                  ->equals(new Add(new VarExpr("y"), new Num(7))));

    CHECK((new VarExpr("x"))
                  ->subst("x", new Add(new VarExpr("y"), new Num(7)))
                  ->equals(new Add(new VarExpr("y"), new Num(7))));
}
/** hw4 */

// 测试基本表达式的漂亮打印
// Test case for pretty printing basic expressions
TEST_CASE("Pretty Printing Basic Expressions") {
    std::stringstream ss;
    Add add(new Num(1), new Mult(new Num(2), new Num(3)));

    // 测试加法表达式的漂亮打印
    // Section for testing pretty print of an addition expression
    SECTION("Add expression pretty print") {
        add.pretty_print(ss);
        // 检查打印结果是否为 "1 + 2 * 3"，注意操作符间的空格和优先级处理
        CHECK(ss.str() == "1 + 2 * 3"); // Check if the print result is "1 + 2 * 3", note the spaces between operators and precedence handling
    }
}

// 测试复杂表达式的漂亮打印
// Test case for pretty printing complex expressions
TEST_CASE("Pretty Printing Complex Expressions") {
    std::stringstream ss;
    Add complexExpr(new Mult(new Add(new Num(1), new Num(2)), new Num(3)), new Num(4));

    // 测试复杂表达式的漂亮打印
    // Section for testing pretty print of a complex expression
    SECTION("Complex expression pretty print") {
        complexExpr.pretty_print(ss);
        // 检查打印结果是否为 "(1 + 2) * 3 + 4"，注意优先级和必要的括号
        CHECK(ss.str() == "(1 + 2) * 3 + 4"); // Check if the print result is "(1 + 2) * 3 + 4", note the precedence and necessary parentheses
    }
}

// 测试数字和操作表达式的打印
// Test case for printing of numbers and operation expressions
TEST_CASE("Num and Operation Expression Printing") {
    std::stringstream ss;
    Num num(5);
    Add add(new Num(3), new Num(4));
    Mult mult(new Num(4), new Num(2));

    // 测试数字打印和字符串转换
    // Section for testing printing of a number and its string conversion
    SECTION("Num print and to_string") {
        num.print(ss);
        CHECK(ss.str() == "5");
        CHECK(num.to_string() == "5");
    }

        // 测试加法打印和字符串转换
        // Section for testing printing of addition and its string conversion
    SECTION("Add print and to_string") {
        ss.str(""); // 清空 stringstream
        add.print(ss);
        CHECK(ss.str() == "(3 + 4)");
        CHECK(add.to_string() == "(3 + 4)");
    }

        // 测试乘法打印和字符串转换
        // Section for testing printing of multiplication and its string conversion
    SECTION("Mult print and to_string") {
        ss.str(""); // 清空 stringstream
        mult.print(ss);
        CHECK(ss.str() == "(4 * 2)");
        CHECK(mult.to_string() == "(4 * 2)");
    }
}

// 测试复杂表达式的打印
// Test case for printing of complex expressions
TEST_CASE("Complex Expression Printing") {
    std::stringstream ss;
    Add complexExpr(new Add(new Num(2), new Mult(new Num(3), new VarExpr("x"))), new Num(4));

    // 测试复杂表达式的打印和字符串转换
    // Section for testing printing of a complex expression and its string conversion
    SECTION("Complex expression print and to_string") {
        complexExpr.print(ss);
        CHECK(ss.str() == "((2 + (3 * x)) + 4)");
        CHECK(complexExpr.to_string() == "((2 + (3 * x)) + 4)");
    }
}

// 测试在漂亮打印中的结合性
// Test case for testing associativity in pretty printing
TEST_CASE("Testing Associativity in Pretty Printing") {
    std::stringstream ss;
    Mult rightAssoc(new Num(2), new Mult(new Num(3), new Num(4))); // 2 * (3 * 4)
    Mult leftAssoc(new Mult(new Num(2), new Num(3)), new Num(4)); // (2 * 3) * 4

    // 测试右结合性的漂亮打印
    // Section for testing right associative pretty printing
    SECTION("Right Associative Pretty Printing") {
        rightAssoc.pretty_print(ss);
        // 检查打印结果是否为 "2 * 3 * 4"，不需要额外的括号
        CHECK(ss.str() == "2 * 3 * 4"); // Check if the print result is "2 * 3 * 4", no extra parentheses needed
    }

    ss.str(""); // 清空 stringstream
    // 测试左结合性的漂亮打印
    // Section for testing left associative pretty printing
    SECTION("Left Associative Pretty Printing") {
        leftAssoc.pretty_print(ss);
        // 检查打印结果是否为 "(2 * 3) * 4"，需要括号表明先算 2 * 3
        CHECK(ss.str() == "(2 * 3) * 4"); // Check if the print result is "(2 * 3) * 4", parentheses needed to indicate 2 * 3 is calculated first
    }
}

