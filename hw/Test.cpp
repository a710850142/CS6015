#include "expr.h"
#include <sstream>
#include "catch.h"

//test print
TEST_CASE("Test print"){
    CHECK ( (new Mult(new Num(1), new Add(new Num(2), new Num(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
    CHECK ( (new Mult(new Mult(new Num(8), new Num(1)), new VarExpr("y")))->to_pretty_string() ==  "(8 * 1) * y" );
    CHECK ( (new Mult(new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (new Mult(new Mult(new Num(7), new Num(7)), new Add(new Num(9), new Num(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
    CHECK ( (new Mult(new Mult(new Num(5), new Num(6)), new Add(new Num(7), new Num(8))) )->to_string() ==  "((5*6)*(7+8))" );
    CHECK ( (new Add(new Mult(new Num(4), new Num(5)), new Num(6)))->to_pretty_string() ==  "4 * 5 + 6" );
    CHECK ( (new Mult(new Num(2), new Add(new Mult(new Num(3), new Num(4)), new Num(5))))->to_pretty_string() ==  "2 * (3 * 4 + 5)" );
    CHECK ( (new Add(new Mult(new Num(2), new Num(3)), new Mult(new Num(4), new Num(5))))->to_pretty_string() ==  "2 * 3 + 4 * 5" );
    CHECK ( (new Mult(new Add(new Num(2), new Num(3)), new Add(new Num(4), new Num(5))))->to_pretty_string() ==  "(2 + 3) * (4 + 5)" );
    CHECK ( (new Add(new Mult(new Num(4), new Num(5)), new Num(6)))->to_string() ==  "((4*5)+6)" );
    CHECK ( (new Mult(new Num(7), new Add(new Mult(new Num(2), new Num(3)), new Num(4))))->to_pretty_string() ==  "7 * (2 * 3 + 4)" );
    CHECK ( (new Add(new Mult(new Num(7), new Num(2)), new Mult(new Num(3), new Num(4))))->to_pretty_string() ==  "7 * 2 + 3 * 4" );
    CHECK ( (new Mult(new Add(new Num(7), new Num(2)), new Add(new Num(3), new Num(4))))->to_pretty_string() ==  "(7 + 2) * (3 + 4)" );
    CHECK ( (new Mult(new Mult(new Num(7), new Num(2)), new Add(new Num(3), new Num(4))))->to_pretty_string() ==  "(7 * 2) * (3 + 4)" );
    CHECK ( (new Mult(new Add(new Num(7), new Num(2)), new Mult(new Num(3), new Num(4))))->to_string() ==  "((7+2)*(3*4))" );
}
TEST_CASE("Edge Cases") {
    Add zeroTest(new Num(0), new Num(0));
    REQUIRE(zeroTest.interp() == 0);

    Mult negativeTest(new Num(-1), new Num(5));
    REQUIRE(negativeTest.interp() == -5);
}

// Test LetExpr print
TEST_CASE("Let test"){
    CHECK((new LetExpr("x", new Num(5), new Add(new LetExpr("y", new Num(3), new Add(new VarExpr("y"), new Num(2))), new VarExpr("x"))))->equals(new Num(7))==false);
    CHECK((new LetExpr("x", new Num(5), new Add(new LetExpr("y", new Num(3), new Add(new VarExpr("y"), new Num(2))), new VarExpr("x"))))->has_variable()==true);
    CHECK((new LetExpr("x", new Num(5), new Add(new LetExpr("y", new Num(3), new Add(new VarExpr("y"), new Num(2))), new VarExpr("x"))))->to_string()=="(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK((new LetExpr("x", new Num(5), new LetExpr("x", new Num(6), new Add(new VarExpr("x"), new Num(1)))))->to_string()=="(_let x=5 _in (_let x=6 _in (x+1)))");
    CHECK((new LetExpr("x", new Num(5), new Add(new LetExpr("y", new Num(3), new Add(new VarExpr("y"), new Num(2))), new VarExpr("x"))))->interp()==10);
    CHECK((new LetExpr("x", new Num(5), new Add(new VarExpr("x"), new Mult(new Num(1), new Num(2)))))->interp()==7);
    CHECK ( (new Mult(new Mult(new Num(2), new LetExpr("x", new Num(5), new Add(new VarExpr("x"), new Num(1)))), new Num(3)))->to_pretty_string() == "(2 * _let x = 5\n"
                                                                                                                                                     "     _in  x + 1) * 3");CHECK((new Mult(new Num(5), new Add(new LetExpr("x", new Num(5), new VarExpr("x")), new Num(1))))->to_pretty_string() == "5 * ((_let x = 5\n"
                                                                                                                             "      _in  x) + 1)");

    CHECK ( (new Add(new LetExpr("x", new Num(2), new Add(new VarExpr("x"), new Num(9))), new Num(4)))->to_pretty_string() == "(_let x = 2\n"
                                                                                                                              " _in  x + 9) + 4");
    CHECK((new Mult(new LetExpr("x", new Num(5), new Add(new VarExpr("x"), new Num(8))), new Num(3)))->to_pretty_string() == "(_let x = 5\n"
                                                                                                                             " _in  x + 8) * 3");
    CHECK((new Add(new Mult(new Num(4), new LetExpr("x", new Num(5), new Add(new VarExpr("x"), new Num(1)))), new Num(9)))->to_pretty_string() == "4 * (_let x = 5\n"
                                                                                                                                                  "     _in  x + 1) + 9");

}

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

}

TEST_CASE("Complex Expression Printing") {
    std::stringstream ss;
    Add complexExpr(new Add(new Num(2), new Mult(new Num(3), new VarExpr("x"))), new Num(4));

}

TEST_CASE("Complex expression printing with LetExpr") {
    std::stringstream ss;
    LetExpr complexLet("x", new Num(5), new Add(new VarExpr("x"), new Mult(new Num(2), new VarExpr("y"))));

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

