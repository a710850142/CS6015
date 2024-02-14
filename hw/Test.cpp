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
////HW3 Test
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

TEST_CASE("Complex expression printing with LetExpr") {
    std::stringstream ss;
    LetExpr complexLet("x", new Num(5), new Add(new VarExpr("x"), new Mult(new Num(2), new VarExpr("y"))));

    SECTION("Complex LetExpr print and to_string") {
        complexLet.print(ss);
        std::string expectedOutput = "(_let x=5 _in (x + (2 * y)))";
        CHECK(ss.str() == expectedOutput);
        CHECK(complexLet.to_string() == expectedOutput);
    }
}

// 测试 LetExpr 中对异常的处理，尤其是在使用 interp 方法时
TEST_CASE("LetExpr interp exception handling") {
    LetExpr letExprWithVar("x", new VarExpr("y"), new Add(new VarExpr("x"), new Num(3)));

    SECTION("interp with undefined variable") {
        // 由于 LetExpr 的 interp 方法未实现，预期会抛出异常
        REQUIRE_THROWS_AS(letExprWithVar.interp(), std::runtime_error);
    }
}

TEST_CASE("LetExpr functionality") {
    LetExpr let("x", new Num(5), new Add(new VarExpr("x"), new Num(3)));

    SECTION("has_variable") {
        // 测试 Let 表达式中变量的存在性
        REQUIRE(let.has_variable() == true); // 因为 body 中可能引用了变量
    }

    SECTION("subst") {
        // 测试替换功能，应该能够替换掉 Let 表达式中的变量
        Expr* substExpr = let.subst("x", new Num(10));
        REQUIRE(dynamic_cast<LetExpr*>(substExpr) != nullptr);
        REQUIRE(substExpr->has_variable() == true); // 替换后，表达式中依然可能存在变量
    }

    SECTION("interp throws") {
        // 测试解释执行时抛出异常，因为 LetExpr 的 interp 方法未实现
        REQUIRE_THROWS_AS(let.interp(), std::runtime_error);
    }

    SECTION("print and to_string") {
        std::stringstream ss;
        let.print(ss);
        std::string expectedOutput = "(_let x=5 _in (x + 3))";
        CHECK(ss.str() == expectedOutput);
        CHECK(let.to_string() == expectedOutput);
    }
}

TEST_CASE("Nested LetExpr evaluation") {
    LetExpr nestedLet("x", new Num(5), new LetExpr("y", new Num(10), new Add(new VarExpr("x"), new VarExpr("y"))));

    SECTION("has_variable in nested LetExpr") {
        // 测试嵌套 LetExpr 中变量的存在性
        REQUIRE(nestedLet.has_variable() == true);
    }

    SECTION("subst in nested LetExpr") {
        // 测试在嵌套 LetExpr 中执行替换
        Expr* substExpr = nestedLet.subst("x", new Num(20));
        REQUIRE(substExpr->has_variable() == true); // 替换后，表达式中依然可能存在变量
    }
}
TEST_CASE("LetExpr substitution and variable handling") {
    LetExpr let("x", new Num(5), new Add(new VarExpr("x"), new Num(3)));

    SECTION("subst with variable present") {
        Expr* substExpr = let.subst("x", new Num(4));
        // 在这个场景中，由于 "x" 是 LetExpr 中的变量，替换操作会影响表达式。
        // 但因为 LetExpr 的特殊性，外部替换不会改变 body 内部对 "x" 的引用。
        // 所以这里检查替换后的表达式是否正确处理了变量，可能需要更细致的逻辑来验证。
        REQUIRE(substExpr->has_variable() == true);
    }

    SECTION("equals after subst on non-existent variable") {
        Expr* substExpr = let.subst("y", new Num(4)); // 替换一个不在 Let 表达式中的变量
        // 由于 "y" 不是 Let 表达式中的变量，因此替换不会改变表达式，它们应该是等价的。
        REQUIRE(let.equals(substExpr)); // 注意这里的逻辑改变了，我们现在期望它们是等价的。
    }
}

TEST_CASE("VarExpr interp throws exception") {
    VarExpr varExpr("x");
    REQUIRE_THROWS_AS(varExpr.interp(), std::runtime_error);
}

TEST_CASE("VarExpr to_string") {
    VarExpr varExpr("x");
    CHECK(varExpr.to_string() == "x");
}

TEST_CASE("LetExpr basic pretty print") {
    std::stringstream ss;
    LetExpr letExpr("x", new Num(10), new Add(new VarExpr("x"), new Num(5)));
    std::streampos last_newline = ss.tellp();

    letExpr.pretty_print_at(ss, prec_none, last_newline);

    CHECK(ss.str() == "_let x = 10\n_in  (x + 5)");
}
TEST_CASE("Nested LetExpr pretty print") {
    std::stringstream ss;
    LetExpr nestedLet(
            "x", new Num(5),
            new LetExpr("y", new Num(10), new Add(new VarExpr("x"), new VarExpr("y")))
    );
    std::streampos last_newline = ss.tellp();

    nestedLet.pretty_print_at(ss, prec_none, last_newline);

    std::string expectedOutput = "_let x = 5\n_in  _let y = 10\n    _in  (x + y)";
    CHECK(ss.str() == expectedOutput);
}
TEST_CASE("LetExpr pretty print with variable shadowing") {
    std::stringstream ss;
    LetExpr outerLet(
            "x", new Num(1),
            new LetExpr("x", new Num(2), new Add(new VarExpr("x"), new Num(3)))
    );
    std::streampos last_newline = ss.tellp();

    outerLet.pretty_print_at(ss, prec_none, last_newline);

    std::string expectedOutput = "_let x = 1\n_in  _let x = 2\n    _in  (x + 3)";
    CHECK(ss.str() == expectedOutput);
}

TEST_CASE("LetExpr interp throws not implemented exception") {
    LetExpr letExpr("x", new Num(5), new Add(new VarExpr("x"), new Num(3)));

    REQUIRE_THROWS_AS(letExpr.interp(), std::runtime_error);
}

TEST_CASE("LetExpr has_variable with complex expressions") {
    LetExpr complexExpr("x", new Num(5),
                        new Add(new VarExpr("x"), new Mult(new Num(2), new VarExpr("y")))
    );

    REQUIRE(complexExpr.has_variable() == true);
}

TEST_CASE("Nested LetExpr print and pretty_print") {
    LetExpr nestedLet("x", new Num(5),
                      new LetExpr("y", new Num(10), new Add(new VarExpr("x"), new VarExpr("y")))
    );
    std::stringstream printOutput;
    std::stringstream prettyPrintOutput;
    std::streampos last_newline = prettyPrintOutput.tellp();

    nestedLet.print(printOutput);
    nestedLet.pretty_print_at(prettyPrintOutput, prec_none, last_newline);

    CHECK(printOutput.str() == "(_let x=5 _in (_let y=10 _in (x + y)))");
    CHECK(prettyPrintOutput.str() == "_let x = 5\n_in  _let y = 10\n    _in  (x + y)");
}




