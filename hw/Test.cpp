#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <iostream>

TEST_CASE("Equals") {
    SECTION("NumExpr") {
        PTR(NumExpr) num1 = NEW(NumExpr)(1);
        PTR(NumExpr) num2 = NEW(NumExpr)(2);
        PTR(NumExpr) num3 = NEW(NumExpr)(1);
        PTR(NumExpr) num4 = NEW(NumExpr)(-1);
        CHECK_FALSE(num1->equals(num2));
        CHECK_FALSE(num2->equals(num1));
        CHECK(num1->equals(num3));
        CHECK(num3->equals(num1));
        CHECK_FALSE(num2->equals(num3));
        CHECK_FALSE(num3->equals(num2));
        CHECK(!num1->equals(num4));
        CHECK(!num1->equals(NEW(VarExpr)("x")));
    }

    SECTION("Add_simple") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1));
        PTR(AddExpr) add3 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
        PTR(AddExpr) add4 = NEW(AddExpr)(1, 3);
        CHECK_FALSE(add1->equals(add2));
        CHECK_FALSE(add2->equals(add1));
        CHECK(add1->equals(add3));
        CHECK(add3->equals(add1));
        CHECK(!add2->equals(add3));
        CHECK(!add3->equals(add2));
        CHECK(!add1->equals(NEW(MultExpr)(1, 2)));
        CHECK(!add1->equals(add4));
    }

    SECTION("Add_nested") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(MultExpr)(3, 4));
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(MultExpr)(3, 4), NEW(AddExpr)(1, 2));
        PTR(AddExpr) add3 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(MultExpr)(3, 4));
        CHECK_FALSE(add1->equals(add2));
        CHECK_FALSE(add2->equals(add1));
        CHECK(add1->equals(add3));
        CHECK(add3->equals(add1));
        CHECK(!add2->equals(add3));
        CHECK(!add3->equals(add2));
        CHECK(!add1->equals(NEW(NumExpr)(5)));
    }

    SECTION("Mult_simple") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(1, 2);
        PTR(MultExpr) mult2 = NEW(MultExpr)(2, 1);
        PTR(MultExpr) mult3 = NEW(MultExpr)(1, 2);
        PTR(MultExpr) mult4 = NEW(MultExpr)(1, 3);
        CHECK_FALSE(mult1->equals(mult2));
        CHECK_FALSE(mult2->equals(mult1));
        CHECK(mult1->equals(mult3));
        CHECK(mult3->equals(mult1));
        CHECK(!mult2->equals(mult3));
        CHECK(!mult3->equals(mult2));
        CHECK(!mult1->equals(NEW(VarExpr)("x")));
        CHECK(!mult1->equals(mult4));
    }

    SECTION("Mult_nested") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(AddExpr)(1, 2), NEW(MultExpr)(3, 4));
        PTR(MultExpr) mult2 = NEW(MultExpr)(NEW(MultExpr)(3, 4), NEW(AddExpr)(1, 2));
        PTR(MultExpr) mult3 = NEW(MultExpr)(NEW(AddExpr)(1, 2), NEW(MultExpr)(3, 4));
        CHECK_FALSE(mult1->equals(mult2));
        CHECK_FALSE(mult2->equals(mult1));
        CHECK(mult1->equals(mult3));
        CHECK(mult3->equals(mult1));
        CHECK(!mult2->equals(mult3));
        CHECK(!mult3->equals(mult2));
    }

    SECTION("Variable") {
        PTR(VarExpr) var1 = NEW(VarExpr)("a");
        PTR(VarExpr) var2 = NEW(VarExpr)("1");
        PTR(VarExpr) var3 = NEW(VarExpr)("a");
        CHECK_FALSE(var1->equals(var2));
        CHECK(var1->equals(var3));
        CHECK(!var2->equals(var3));
        CHECK(!var1->equals(NEW(NumExpr)(1)));
    }
};

TEST_CASE("Interp") {
    SECTION("NumExpr") {
        CHECK((NEW(NumExpr)(0))->interp()->equals(NEW(NumVal) (0)));
        CHECK((NEW(NumExpr)(-100))->interp()->equals(NEW(NumVal) (-100)));
    }

    SECTION("Add_simple") {
        CHECK((NEW(AddExpr)(1, 3))->interp()->equals(NEW(NumVal) (4)));
        CHECK((NEW(AddExpr)(-2019, 5072))->interp()->equals(NEW(NumVal) (3053)));
        CHECK((NEW(AddExpr)(NEW(NumExpr)(57), NEW(AddExpr)(3, -800)))->interp()->equals(NEW(NumVal) (-740)));
    }

    SECTION("Add_nested") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(AddExpr)(1, 9), NEW(MultExpr)(2, 50));
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(AddExpr)(-31000, 19), NEW(AddExpr)(20, 61));
        PTR(AddExpr) add3 = NEW(AddExpr)(NEW(AddExpr)(1010, 2), NEW(MultExpr)(0, 10000));
        CHECK(add1->interp()->equals(NEW(NumVal) (110)));
        CHECK(add2->interp()->equals(NEW(NumVal) (-30900)));
        CHECK(add3->interp()->equals(NEW(NumVal) (1012)));
    }

    SECTION("Mult_simple") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(-3, 2);
        PTR(MultExpr) mult2 = NEW(MultExpr)(-1034, -730);
        PTR(MultExpr) mult3 = NEW(MultExpr)(12345678, 0);
        PTR(MultExpr) mult4 = NEW(MultExpr)(2, "x");
        CHECK(mult1->interp()->equals(NEW(NumVal) (-6)));
        CHECK(mult2->interp()->equals(NEW(NumVal) (754820)));
        CHECK(mult3->interp()->equals(NEW(NumVal) (0)));
        CHECK_THROWS_WITH(mult4->interp(), "A variable has no value!");
    }

    SECTION("Mult_nested") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(AddExpr)(2, 3), NEW(MultExpr)(4, 5));
        PTR(MultExpr) mult2 = NEW(MultExpr)(NEW(MultExpr)(-5, 10), NEW(MultExpr)(-300, 20));
        PTR(MultExpr) mult3 = NEW(MultExpr)(NEW(MultExpr)(2, 6), NEW(AddExpr)(1, -9));
        CHECK(mult1->interp()->equals(NEW(NumVal) (100)));
        CHECK(mult2->interp()->equals(NEW(NumVal) (300000)));
        CHECK(mult3->interp()->equals(NEW(NumVal) (-96)));
    }

    SECTION("Variable") {
        CHECK_THROWS_WITH((NEW(VarExpr)("x")) -> interp(), "A variable has no value!");
        CHECK_THROWS_WITH((NEW(AddExpr)(6, "y")) -> interp(), "A variable has no value!");
    }
}

TEST_CASE("subst") {
    SECTION("NumExpr") {
        CHECK((NEW(NumExpr)(4))->subst("x", NEW(NumExpr)(3)) -> equals(NEW(NumExpr)(4)));
    }

    SECTION("Add_simple") {
        CHECK(NEW(AddExpr)("x", 7)->subst("x", NEW(VarExpr)("y"))->equals(NEW(AddExpr)("y", 7)));
        CHECK(NEW(AddExpr)(2, 3)->subst("x", NEW(NumExpr)(1))->equals(NEW(AddExpr)(2, 3)));
    }

    SECTION("Add_nested") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)("a", 9));
        PTR(AddExpr) res11 = NEW(AddExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)(100, 9));
        PTR(AddExpr) res12 = NEW(AddExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)("b", 9));
        PTR(AddExpr) res13 = NEW(AddExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)("a", 9));
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(AddExpr)("x", 3), NEW(MultExpr)("y", -10));
        PTR(AddExpr) res21 = NEW(AddExpr)(NEW(AddExpr)("y", 3), NEW(MultExpr)("y", -10));
        PTR(AddExpr) res22 = NEW(AddExpr)(NEW(AddExpr)(NEW(AddExpr)(1, 3), NEW(NumExpr)(3)), NEW(MultExpr)("y", -10));
        CHECK(add1->subst("a", NEW(NumExpr)(100)) -> equals(res11));
        CHECK(add1->subst("a", NEW(VarExpr)("b")) -> equals(res12));
        CHECK(add1->subst("b", NEW(NumExpr)(7)) -> equals(res13));
        CHECK(add2->subst("x", NEW(VarExpr)("y")) -> equals(res21));
        CHECK(add2->subst("x", NEW(AddExpr)(1, 3)) -> equals(res22));
    }

    SECTION("Mult_simple") {
        CHECK(NEW(MultExpr)("x", 7)->subst("x", NEW(VarExpr)("y"))->equals(NEW(MultExpr)("y", 7)));
        CHECK(NEW(MultExpr)(2, 3)->subst("x", NEW(NumExpr)(1))->equals(NEW(MultExpr)(2, 3)));
    }

    SECTION("Mult_nested") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(AddExpr)(1, 5), NEW(MultExpr)("a", 9));
        PTR(MultExpr) res11 = NEW(MultExpr)(NEW(AddExpr)(1, 5), NEW(MultExpr)(100, 9));
        PTR(MultExpr) res12 = NEW(MultExpr)(NEW(AddExpr)(1, 5), NEW(MultExpr)("b", 9));
        PTR(MultExpr) res13 = NEW(MultExpr)(NEW(AddExpr)(1, 5), NEW(MultExpr)("a", 9));
        PTR(MultExpr) mult2 = NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)), NEW(MultExpr)("y", -10));
        PTR(MultExpr) res21 = NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)), NEW(MultExpr)("y", -10));
        PTR(MultExpr) res22 = NEW(MultExpr)(NEW(MultExpr)(NEW(AddExpr)(1, 3), NEW(NumExpr)(3)), NEW(MultExpr)("y", -10));
        CHECK(mult1->subst("a", NEW(NumExpr)(100)) -> equals(res11));
        CHECK(mult1->subst("a", NEW(VarExpr)("b")) -> equals(res12));
        CHECK(mult1->subst("b", NEW(NumExpr)(7)) -> equals(res13));
        CHECK(mult2->subst("x", NEW(VarExpr)("y")) -> equals(res21));
        CHECK(mult2->subst("x", NEW(AddExpr)(1, 3)) -> equals(res22));
    }

    SECTION("Variable") {
        CHECK((NEW(VarExpr)("x"))->subst("x", NEW(AddExpr)("y", 7)) -> equals(NEW(AddExpr)("y", 7)));
        CHECK(NEW(VarExpr)("x")->subst("y", NEW(NumExpr)(3)) -> equals(NEW(VarExpr)("x")));
    }
}

TEST_CASE("Interpt + Subst") {
    PTR(AddExpr) add1 = NEW(AddExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)("a", 9));
    CHECK(add1->subst("a", NEW(NumExpr)(100))->interp()->equals(NEW(NumVal) (115)));
    PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(AddExpr)(1, 5), NEW(AddExpr)("a", 9));
    CHECK(mult1->subst("a", NEW(NumExpr)(100))->interp()->equals(NEW(NumVal) (654)));

};

TEST_CASE("to_string + print") {
    SECTION("NumExpr") {
        CHECK(NEW(NumExpr)(10)->to_string() == "10");
        CHECK(NEW(NumExpr)(-10)->to_string() == "-10");
    }

    SECTION("Add_simple") {
        CHECK(NEW(AddExpr)(1, 2)->to_string() == "(1+2)");
        CHECK(NEW(AddExpr)("x", 2)->to_string() == "(x+2)");
        CHECK(NEW(AddExpr)(-3, "speed")->to_string() == "(-3+speed)");
        CHECK(NEW(AddExpr)(-10, -5)->to_string() == "(-10+-5)");
    }

    SECTION("Add_nested") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(-10, 2));
        CHECK(add1->to_string() == "(1+(-10+2))");
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(AddExpr)(3, 4));
        CHECK(add2->to_string() == "((1+2)+(3+4))");
        PTR(AddExpr) add3 = NEW(AddExpr)(NEW(AddExpr)(1001, 50), NEW(VarExpr)("p"));
        CHECK(add3->to_string() == "((1001+50)+p)");
    }

    SECTION("Mult_simple") {
        CHECK(NEW(MultExpr)(-1, 2)->to_string() == "(-1*2)");
        CHECK(NEW(MultExpr)("x", "y")->to_string() == "(x*y)");
        CHECK(NEW(MultExpr)("z", 2)->to_string() == "(z*2)");
    }

    SECTION("Mult_nested") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(-10, 2));
        CHECK(mult1->to_string() == "(1*(-10*2))");
        PTR(MultExpr) mult2 = NEW(MultExpr)(NEW(AddExpr)(2, 7), NEW(MultExpr)(5, -20));
        CHECK(mult2->to_string() == "((2+7)*(5*-20))");
        PTR(MultExpr) mult3 = NEW(MultExpr)(NEW(MultExpr)(1001, 50), "p");
        CHECK(mult3->to_string() == "((1001*50)*p)");
    }

    SECTION("Variable") {
        CHECK(NEW(VarExpr)("xyz")->to_string() == "xyz");
    }
};

TEST_CASE("pretty_print") {
    SECTION("num") {
        CHECK(NEW(NumExpr)(1)->to_pretty_string() == "1");
    }

    SECTION("add") {
        PTR(AddExpr) add1 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(NumExpr)(3));
        CHECK(add1->to_pretty_string() == "(1 + 2) + 3");
        PTR(AddExpr) add2 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(2, 3));
        CHECK(add2->to_pretty_string() == "1 + 2 + 3");
        PTR(AddExpr) add3 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(AddExpr)(NEW(NumExpr)(3), NEW(AddExpr)(4, 5)));
        CHECK(add3->to_pretty_string() == "(1 + 2) + 3 + 4 + 5");
        PTR(AddExpr) add4 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(AddExpr)(NEW(AddExpr)(3, 4), NEW(NumExpr)(5)));
        CHECK(add4->to_pretty_string() == "(1 + 2) + (3 + 4) + 5");
        PTR(AddExpr) add5 = NEW(AddExpr)(NEW(MultExpr)(1, 2), NEW(MultExpr)(3, 4));
        CHECK(add5->to_pretty_string() == "1 * 2 + 3 * 4");
        PTR(AddExpr) add6 = NEW(AddExpr)(NEW(AddExpr)(1, 2), NEW(MultExpr)(3, 4));
        CHECK(add6->to_pretty_string() == "(1 + 2) + 3 * 4");
    }

    SECTION("mult") {
        PTR(MultExpr) mult1 = NEW(MultExpr)(NEW(MultExpr)(1, 2), NEW(NumExpr)(3));
        CHECK(mult1->to_pretty_string() == "(1 * 2) * 3");
        PTR(MultExpr) mult2 = NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(2, 3));
        CHECK(mult2->to_pretty_string() == "1 * 2 * 3");
        PTR(MultExpr) mult3 = NEW(MultExpr)(NEW(MultExpr)(1, 2), NEW(MultExpr)(3, NEW(MultExpr)(4, 5)));
        CHECK(mult3->to_pretty_string() == "(1 * 2) * 3 * 4 * 5");
        PTR(MultExpr) mult4 = NEW(MultExpr)(NEW(MultExpr)(1, 2), NEW(MultExpr)(NEW(MultExpr)(3, 4), NEW(NumExpr)(5)));
        CHECK(mult4->to_pretty_string() == "(1 * 2) * (3 * 4) * 5");
        PTR(MultExpr) mult5 = NEW(MultExpr)(NEW(MultExpr)(1, 2), NEW(AddExpr)(3, 4));
        CHECK(mult5->to_pretty_string() == "(1 * 2) * (3 + 4)");
        PTR(MultExpr) mult6 = NEW(MultExpr)(NEW(MultExpr)(1, 2), NEW(VarExpr)("x"));
        CHECK(mult6->to_pretty_string() == "(1 * 2) * x");
    }

    SECTION("add + mult") {
        CHECK(NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(2, 3))->to_pretty_string() == "1 + 2 * 3");
        CHECK(NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(2, 3))->to_pretty_string() == "1 * (2 + 3)");
    }

    SECTION("Variable") {
        CHECK(NEW(VarExpr)("xyz")->to_string() == "xyz");
    }
}

TEST_CASE("let") {
    PTR(LetExpr) letBase1 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)("x", 1));
    PTR(LetExpr) letBase2 = NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)("x", 1));

    SECTION("equals") {
        PTR(LetExpr) let1 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)("x", 1));
        CHECK(letBase1->equals(let1));
        CHECK(!letBase2->equals(let1));
        CHECK(!letBase1->equals(NEW(AddExpr)(1, 2)));
    }

    SECTION("subst + interp") {
        CHECK(letBase1->interp()->equals(NEW(NumVal) (6)));
        CHECK(letBase2->interp()->equals(NEW(NumVal) (7)));
        PTR(LetExpr) let1 = NEW(LetExpr)("x", NEW(AddExpr)(5, 2), NEW(MultExpr)("x", 3));
        CHECK(let1->interp()->equals(NEW(NumVal) (21)));
        PTR(LetExpr) let2 = NEW(LetExpr)("x", NEW(AddExpr)(5, 2), NEW(AddExpr)("x", 1));
        CHECK(let2->interp()->equals(NEW(NumVal) (8)));
        PTR(LetExpr) let3 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)("x", 1)));
        CHECK(let3->interp()->equals(NEW(NumVal) (7)));
        PTR(LetExpr) let4 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(AddExpr)("x", 1)));
        CHECK(let4->interp()->equals(NEW(NumVal) (6)));
        PTR(LetExpr) let5 = NEW(LetExpr)("x", NEW(AddExpr)(2, 5), NEW(LetExpr)("x", NEW(AddExpr)("x", -13), NEW(AddExpr)("x", 1)));
        CHECK(let5->interp()->equals(NEW(NumVal) (-5)));
        PTR(LetExpr) let6 = NEW(LetExpr)("x", NEW(AddExpr)(-1, -6), NEW(LetExpr)("x", NEW(AddExpr)(-3, 2), NEW(AddExpr)("x", 3)));
        CHECK(let6->interp()->equals(NEW(NumVal) (2)));
        PTR(LetExpr) let2Nested = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(LetExpr)("y", NEW(NumExpr)(8), NEW(AddExpr)("x", "y")));
        CHECK(let2Nested->interp()->equals(NEW(NumVal) (13)));
        PTR(LetExpr) let3Nested = NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(LetExpr)("y", NEW(NumExpr)(2), NEW(LetExpr)("z", NEW(NumExpr)(3), NEW(AddExpr)("x", NEW(AddExpr)("y", "z")))));
        CHECK(let3Nested->interp()->equals(NEW(NumVal) (6)));
        PTR(LetExpr) let3Nested2 = NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(LetExpr)("y", NEW(AddExpr)("x", 1), NEW(LetExpr)("z", NEW(AddExpr)("y", 1), NEW(AddExpr)("x", NEW(AddExpr)("y", "z")))));
        let3Nested2->interp();
        CHECK(let3Nested2->interp()->equals(NEW(NumVal) (6)));
        PTR(LetExpr) letError = NEW(LetExpr)("x", NEW(AddExpr)("y", 2), NEW(AddExpr)("x", 1));
        CHECK_THROWS_WITH(letError->interp(), "A variable has no value!");
    }

    SECTION("print") {
        PTR(LetExpr) let1 = NEW(LetExpr) ("x",
                                          NEW(NumExpr)(5),
                                          NEW(AddExpr) (
                                                  NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                                  NEW(VarExpr)("x")));
        CHECK(let1->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }

    SECTION("pretty_print") {
        CHECK(letBase1->to_pretty_string() == "_let x = 5\n_in  x + 1");
        CHECK(letBase2->to_pretty_string() == "_let x = 6\n_in  x + 1");

        PTR(LetExpr) let2 = NEW(LetExpr)("x", NEW(NumExpr)(5),letBase2);
        CHECK(let2->to_pretty_string() == "_let x = 5\n"
                                          "_in  _let x = 6\n"
                                          "     _in  x + 1");

        PTR(LetExpr) let3 = NEW(LetExpr)("x",NEW(NumExpr)(5),NEW(AddExpr)(letBase2, "x"));
        CHECK(let3->to_pretty_string() == "_let x = 5\n"
                                          "_in  (_let x = 6\n"
                                          "      _in  x + 1) + x");

        PTR(LetExpr) let4 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(letBase2, "x"));
        CHECK(let4->to_pretty_string() == "_let x = 5\n"
                                          "_in  (_let x = 6\n"
                                          "      _in  x + 1) * x");

        PTR(MultExpr) let5 = NEW(MultExpr)(NEW(NumExpr)(2), letBase1);
        CHECK(let5->to_pretty_string() == "2 * _let x = 5\n"
                                          "    _in  x + 1");

        PTR(AddExpr) let6 = NEW(AddExpr)( NEW(AddExpr)(NEW(NumExpr)(2), letBase1), NEW(NumExpr)(3));
        CHECK(let6->to_pretty_string() == "(2 + _let x = 5\n"
                                          "     _in  x + 1) + 3");

        PTR(MultExpr) let7 = NEW(MultExpr)(NEW(MultExpr)(2, letBase1), NEW(NumExpr)(3));
        CHECK(let7->to_pretty_string() == "(2 * _let x = 5\n"
                                          "     _in  x + 1) * 3");

        PTR(LetExpr) let8 = NEW(LetExpr)("x", letBase1, NEW(AddExpr)("x", 6));
        CHECK(let8->to_pretty_string() == "_let x = _let x = 5\n"
                                          "         _in  x + 1\n"
                                          "_in  x + 6");
        PTR(LetExpr) let9 = NEW(LetExpr)("x", NEW(AddExpr)(letBase1, NEW(NumExpr)(2)), NEW(AddExpr)("x", 6));
        CHECK(let9->to_pretty_string() == ("_let x = (_let x = 5\n"
                                           "          _in  x + 1) + 2\n"
                                           "_in  x + 6"));
        PTR(AddExpr) let10 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(3, letBase1));
        CHECK(let10->to_pretty_string() == "1 + 3 * _let x = 5\n"
                                           "        _in  x + 1");
        PTR(MultExpr) let11 = NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(2, letBase1));
        CHECK(let11->to_pretty_string() == "1 * 2 * _let x = 5\n"
                                           "        _in  x + 1");
        PTR(AddExpr) let12 = NEW(AddExpr)(NEW(MultExpr)(2, letBase1), NEW(NumExpr)(5));
        CHECK(let12->to_pretty_string() == "2 * (_let x = 5\n"
                                           "     _in  x + 1) + 5");
        PTR(AddExpr) let13 = NEW(AddExpr)(let11, NEW(NumExpr)(5));
        CHECK(let13->to_pretty_string() == "1 * 2 * (_let x = 5\n"
                                           "         _in  x + 1) + 5");
        PTR(AddExpr) let14 = NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(2, letBase1)), NEW(NumExpr)(1));
        CHECK(let14->to_pretty_string() == "(1 + 2 * _let x = 5\n"
                                           "         _in  x + 1) + 1");
        PTR(AddExpr) let15 = NEW(AddExpr)(NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(2), letBase1), NEW(NumExpr)(1)), NEW(NumExpr)(1));
        CHECK(let15->to_pretty_string() == "(2 + _let x = 5\n"
                                           "     _in  x + 1) * 1 + 1");
        PTR(AddExpr) let16 = NEW(AddExpr)(NEW(LetExpr)("x",NEW(NumExpr)(5),NEW(LetExpr)("y", NEW(NumExpr)(3),NEW(AddExpr)("y", 2))),NEW(VarExpr)("x"));
        CHECK(let16->to_pretty_string() == "(_let x = 5\n"
                                           " _in  _let y = 3\n"
                                           "      _in  y + 2) + x");
    }
}

TEST_CASE("parse") {
    PTR(LetExpr) letBase1 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)("x", 1));
    PTR(LetExpr) letBase2 = NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(MultExpr)("x", 1));
    PTR(IfExpr) ifBase1 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(IfExpr) ifBase2 = NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1), NEW(NumExpr)(2));

    SECTION("num") {
        CHECK(parse_str("123")->equals(NEW(NumExpr)(123)));
        CHECK(parse_str("-123")->equals(NEW(NumExpr)(-123)));
        CHECK(parse_str("     123")->equals(NEW(NumExpr)(123)));
        CHECK(parse_str("     123     ")->equals(NEW(NumExpr)(123)));
        CHECK_THROWS_WITH(parse_str("12x"), "bad input");
        CHECK_THROWS_WITH(parse_str("1000000000000"), "the number is out of the range of integer!");
    }

    SECTION("var") {
        CHECK(parse_str("ABc")->equals(NEW(VarExpr)("ABc")));
        CHECK(parse_str("ABc   ")->equals(NEW(VarExpr)("ABc")));
        CHECK_THROWS_WITH(parse_str("abc+ "), "bad input");
        CHECK_THROWS_WITH(parse_str("abc* "), "bad input");
        CHECK_THROWS_WITH(parse_str("a2"), "invalid variable name");
        CHECK_THROWS_WITH(parse_str("x y"), "bad input");
        CHECK_THROWS_WITH(parse_str("1 y"), "bad input");
    }

    SECTION("let + mult + add") {

        std::string let1a = "_let x = 5 _in x + 1";
        CHECK(parse_str(let1a)->equals(letBase1));
        std::string let1b = "(_let x = 5 _in x + 1)";
        CHECK(parse_str(let1b)->equals(letBase1));
        std::string let1c = "_let x = 5 _in (x + 1)";
        CHECK(parse_str(let1c)->equals(letBase1));
        std::string let1d = "_let x = (5) _in x + 1";
        CHECK(parse_str(let1d)->equals(letBase1));
        std::string let1e = "_let x = (5)\n"
                            "_in x + 1";
        CHECK(parse_str(let1e)->equals(letBase1));
        std::string let1f = "_let x =                5 _in x + 1";
        CHECK(parse_str(let1f)->equals(letBase1));
        std::string let1g = "(_let x = (5 _in x + 1))"; // wrong parentheses
        CHECK_THROWS_WITH(parse_str(let1g), "parentheses mismatch");

        std::string let2 = "(_let x = 5 _in  x) + 1";
        PTR(AddExpr) let2res = NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1));
        CHECK(parse_str(let2)->equals(let2res));

        std::string let3 = "_let x = 2 _in _let x = 5 _in x + 1";
        PTR(LetExpr) let3res = NEW(LetExpr)("x", NEW(NumExpr)(2), letBase1);
        CHECK(parse_str(let3)->equals(let3res));

        std::string let4 = "1 * 2 * _let x = 5 _in x + 1";
        PTR(MultExpr) let4res = NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(2, letBase1));
        CHECK(parse_str(let4)->equals(let4res));

        std::string let5 = "_let x = 5\n"
                           "_in  (_let x = 6\n"
                           "      _in  x * 1) + x";
        PTR(LetExpr) let5res = NEW(LetExpr)("x",NEW(NumExpr)(5),NEW(AddExpr)(letBase2, NEW(VarExpr)("x")));
        CHECK(parse_str(let5)->equals(let5res));


        std::string let6a = "_let x";
        CHECK_THROWS_WITH(parse_str(let6a), "wrong format for let expression");
        std::string let6b = "_led x";
        CHECK_THROWS_WITH(parse_str(let6b), "unknown keyword: led");
        std::string let6c = "_led x = 55 _in";
        CHECK_THROWS_WITH(parse_str(let6c), "unknown keyword: led");


        std::string let7 = "_let x = y _in x + 1";
        CHECK_THROWS_WITH(parse_str(let7)->interp(), "A variable has no value!");

        std::string let8 = "_let x = _let x = 5\n"
                           "         _in  x + 1\n"
                           "_in  x + 6";
        PTR(LetExpr) let8res = NEW(LetExpr)("x", letBase1, NEW(AddExpr)("x", 6));
        CHECK(parse_str(let8)->equals(let8res));

    }

    SECTION("if + let") {
        std::string if1a = "_if _true _then 1 _else 2";
        CHECK(parse_str(if1a)->equals(ifBase1));
        std::string if1b = "(_if _true _then 1 _else 2)";
        CHECK(parse_str(if1b)->equals(ifBase1));
        std::string if1c = "(_if _false _then 1 _else 2)";
        CHECK(parse_str(if1c)->equals(ifBase2));

        std::string if2 = "_if _false _then 1";
        CHECK_THROWS_WITH(parse_str(if2), "wrong format for if expression");
        std::string if3 = "(_if _true _then 1 _else 2) + 1";
        CHECK(parse_str(if3)->equals(NEW(AddExpr)(ifBase1, 1)));
    }

    SECTION("equal") {
        std::string eq1 = "1 == 2";
        CHECK(parse_str(eq1)->equals(NEW(EqExpr)(1, 2)));
        PTR(EqExpr) eq2 = NEW(EqExpr)(letBase1, NEW(NumExpr)(2));
        CHECK(eq2->to_pretty_string() == "(_let x = 5\n"
                                         " _in  x + 1) == 2");
        std::string eq3 = "1 + 1 == 2 + 0";
        CHECK(parse_str(eq3)->equals(NEW(EqExpr)(NEW(AddExpr)(1, 1), NEW(AddExpr)(2, 0))));
        CHECK(parse_str(eq3)->interp()->equals(NEW(BoolVal) (true)));
    }
}

TEST_CASE("parse given tests") {

    CHECK_THROWS_WITH( parse_str("()"), "bad input");
    CHECK( parse_str("(1)")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_str("(((1)))")->equals(NEW(NumExpr)(1)) );
    CHECK_THROWS_WITH( parse_str("(1"), "parentheses mismatch" );

    CHECK( parse_str("1")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_str("10")->equals(NEW(NumExpr)(10)) );
    CHECK( parse_str("-3")->equals(NEW(NumExpr)(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "invalid input" );
    CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

    CHECK( parse_str("x")->equals(NEW(VarExpr)("x")) );
    CHECK( parse_str("xyz")->equals(NEW(VarExpr)("xyz")) );
    CHECK( parse_str("xYz")->equals(NEW(VarExpr)("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "invalid variable name" );

    CHECK( parse_str("x + y")->equals(NEW(AddExpr)("x", "y")) );
    CHECK( parse_str("x * y")->equals(NEW(MultExpr)("x", "y")) );
    CHECK( parse_str("z * x + y")->equals(NEW(AddExpr)(NEW(MultExpr)("z", "x"),NEW(VarExpr)("y"))));
    CHECK( parse_str("z * (x + y)")->equals(NEW(MultExpr)("z",NEW(AddExpr)("x", "y"))));
}

TEST_CASE("NumVal") {

    SECTION("equals") {
        CHECK(NEW(NumVal) (1)->equals(NEW(NumVal) (1)));
        CHECK(!NEW(NumVal) (1)->equals(nullptr));
        CHECK(NEW(AddExpr)(2, 3)->interp()->equals(NEW(NumVal) (5)));
    }

    SECTION("to_expr") {
        CHECK(NEW(NumVal) (1)->to_expr()->equals(NEW(NumExpr)(1)));
    }

    SECTION("add_to") {
        CHECK(NEW(NumVal) (3)->add_to(NEW(NumVal) (2))->equals(NEW(NumVal) (5)));
        CHECK(NEW(NumVal) (3)->add_to(NEW(NumVal) (2))->add_to(NEW(NumVal) (-1))->equals(NEW(NumVal) (4)));
        CHECK_THROWS_WITH(NEW(NumVal) (3)->add_to(nullptr), "add of non-number");
    }

    SECTION("mult_with") {
        CHECK(NEW(NumVal) (3)->mult_with(NEW(NumVal) (2))->equals(NEW(NumVal) (6)));
        CHECK(NEW(NumVal) (3)->mult_with(NEW(NumVal) (2))->mult_with(NEW(NumVal) (-1))->equals(NEW(NumVal) (-6)));
        CHECK_THROWS_WITH(NEW(NumVal) (3)->mult_with(nullptr), "multiply with non-number");
    }

    SECTION("to_string") {
        CHECK(NEW(NumVal) (120)->to_string() == "120");
    }
}

TEST_CASE("BoolVal") {

    SECTION("equals") {
        CHECK(NEW(BoolVal) (true)->equals(NEW(BoolVal) (true)));
        CHECK(!NEW(BoolVal) (false)->equals(NEW(BoolVal) (true)));
        CHECK(!NEW(BoolVal) (true)->equals(NEW(NumVal) (1)));
    }

    SECTION("to_expr") {
        CHECK(NEW(BoolVal) (true)->to_expr()->equals(NEW(BoolExpr)(true)));
        CHECK(NEW(BoolVal) (false)->to_expr()->equals(NEW(BoolExpr)(false)));
    }

    SECTION("add_to") {
        CHECK_THROWS_WITH(NEW(BoolVal) (true)->add_to(NEW(NumVal) (1)), "add of non-number");
    }

    SECTION("mult_with") {
        CHECK_THROWS_WITH(NEW(BoolVal) (3)->mult_with(nullptr), "multiply with non-number");
    }

    SECTION("to_string") {
        CHECK(NEW(BoolVal) (true)->to_string() == "_true");
        CHECK(NEW(BoolVal) (false)->to_string() == "_false");
    }
}

TEST_CASE("BoolExpr") {
    SECTION("equals") {
        CHECK(!NEW(BoolExpr)(true)->equals(NEW(BoolExpr)(false)));
        CHECK(NEW(BoolExpr)(true)->equals(NEW(BoolExpr)(true)));
        CHECK(!NEW(BoolExpr)(true)->equals(NEW(NumExpr)(3)));
    }

    SECTION("interp") {
        CHECK(NEW(BoolExpr)(true)->interp()->equals(NEW(BoolVal) (true)));
        CHECK(NEW(BoolExpr)(false)->interp()->equals(NEW(BoolVal) (false)));
        CHECK(!NEW(BoolExpr)(true)->interp()->equals(NEW(BoolVal) (false)));
        CHECK(!NEW(BoolExpr)(true)->interp()->equals(NEW(NumVal) (3)));
    }

    SECTION("subst") {
        CHECK(NEW(BoolExpr)(true)->subst("x", NEW(NumExpr)(1))->equals(NEW(BoolExpr)(true)));
    }

    SECTION("print") {
        CHECK(NEW(BoolExpr)(true)->to_string() == "_true");
        CHECK(NEW(BoolExpr)(false)->to_string() == "_false");
    }

    SECTION("pretty_print") {
        CHECK(NEW(BoolExpr)(true)->to_pretty_string() == "_true");
        CHECK(NEW(BoolExpr)(false)->to_pretty_string() == "_false");
    }
}

TEST_CASE("EqExpr") {
    PTR(EqExpr) eq1 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(EqExpr) eq2 = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1));
    PTR(EqExpr) eq3 = NEW(EqExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"));
    PTR(EqExpr) eq4 = NEW(EqExpr)(NEW(NumExpr)(2), NEW(AddExpr)(1, 1));
    PTR(EqExpr) eq5 = NEW(EqExpr)(NEW(MultExpr)(2, 1), NEW(AddExpr)(1, 1));
    PTR(EqExpr) eq6 = NEW(EqExpr)(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)("x", 3)), NEW(MultExpr)(2, 3));

    SECTION("equals") {
        CHECK(!eq1->equals(eq2));
        CHECK(eq1->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))));
        CHECK(!eq1->equals(NEW(NumExpr)(3)));
        CHECK(!eq4->equals(eq5));
        CHECK(!eq5->equals(eq6));
    }

    SECTION("interp") {
        CHECK(eq1->interp()->equals(NEW(BoolVal) (false)));
        CHECK(eq2->interp()->equals(NEW(BoolVal) (true)));
        CHECK_THROWS_WITH(eq3->interp(), "A variable has no value!");
        CHECK(eq4->interp()->equals(NEW(BoolVal) (true)));
        CHECK(eq5->interp()->equals(NEW(BoolVal) (true)));
        CHECK(eq6->interp()->equals(NEW(BoolVal) (false)));
    }

    SECTION("subst") {
        CHECK(eq1->subst("x", NEW(NumExpr)(2))->equals(eq1));
    }

    SECTION("print") {
        CHECK(eq1->to_string() == "(1==2)");
        CHECK(eq2->to_string() == "(1==1)");
        CHECK(eq3->to_string() == "(x==x)");
        CHECK(eq4->to_string() == "(2==(1+1))");
        CHECK(eq5->to_string() == "((2*1)==(1+1))");
        CHECK(eq6->to_string() == "((_let x=4 _in (x+3))==(2*3))");
    }

    SECTION("pretty_print") {
        CHECK(eq1->to_pretty_string() == "1 == 2");
        CHECK(eq2->to_pretty_string() == "1 == 1");
        CHECK(eq3->to_pretty_string() == "x == x");
        CHECK(eq4->to_pretty_string() == "2 == 1 + 1");
        CHECK(eq5->to_pretty_string() == "2 * 1 == 1 + 1");

        CHECK(eq6->to_pretty_string() == "(_let x = 4\n"
                                         " _in  x + 3) == 2 * 3");
        PTR(IfExpr) ifBase = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2));
        PTR(LetExpr) letBase1 = NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)("x", 1));
        PTR(LetExpr) letBase2 = NEW(LetExpr)("y", NEW(NumExpr)(2), NEW(AddExpr)("y", 2));
        PTR(EqExpr) eq7 = NEW(EqExpr)(ifBase, NEW(NumExpr)(2));
        CHECK(eq7->to_pretty_string() == "(_if _true\n"
                                         " _then 1\n"
                                         " _else 2) == 2");
        PTR(EqExpr) eq8 = NEW(EqExpr)(letBase1, NEW(NumExpr)(2));
        CHECK(eq8->to_pretty_string() == "(_let x = 1\n"
                                         " _in  x + 1) == 2");

        PTR(EqExpr) eq9 = NEW(EqExpr)(NEW(NumExpr)(1), ifBase);
        CHECK(eq9->to_pretty_string() == "1 == _if _true\n"
                                         "     _then 1\n"
                                         "     _else 2");
        PTR(EqExpr) eq10 = NEW(EqExpr)(NEW(NumExpr)(2), letBase1);
        CHECK(eq10->to_pretty_string() == "2 == _let x = 1\n"
                                          "     _in  x + 1");
        PTR(AddExpr) eq11 = NEW(AddExpr)(eq9, NEW(NumExpr)(3));
        CHECK(eq11->to_pretty_string() == "(1 == _if _true\n"
                                          "      _then 1\n"
                                          "      _else 2) + 3");

        PTR(AddExpr) eq12a = NEW(AddExpr)(eq10, NEW(NumExpr)(3));
        CHECK(eq12a->to_pretty_string() == "(2 == _let x = 1\n"
                                           "      _in  x + 1) + 3");
        PTR(AddExpr) eq12b = NEW(AddExpr)(NEW(NumExpr)(3), eq10);
        CHECK(eq12b->to_pretty_string() == "3 + (2 == _let x = 1\n"
                                           "          _in  x + 1)");
        PTR(AddExpr) eq12c = NEW(AddExpr)(eq12b, NEW(NumExpr)(5));
        CHECK(eq12c->to_pretty_string() == "(3 + (2 == _let x = 1\n"
                                           "           _in  x + 1)) + 5");
        PTR(AddExpr) eq12d = NEW(AddExpr)(eq1, 3);
        CHECK(eq12d->to_pretty_string() == "(1 == 2) + 3");

        PTR(EqExpr) eq13 = NEW(EqExpr)(NEW(NumExpr)(2), NEW(AddExpr)(ifBase, NEW(NumExpr)(3)));
        CHECK(eq13->to_pretty_string() == "2 == (_if _true\n"
                                          "      _then 1\n"
                                          "      _else 2) + 3");
        PTR(EqExpr) eq14 = NEW(EqExpr)(NEW(NumExpr)(2), NEW(AddExpr)(letBase1, NEW(NumExpr)(3)));
        CHECK(eq14->to_pretty_string() == "2 == (_let x = 1\n"
                                          "      _in  x + 1) + 3");
        PTR(EqExpr) eq15 = NEW(EqExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(3), ifBase));
        CHECK(eq15->to_pretty_string() == "2 == 3 + _if _true\n"
                                          "         _then 1\n"
                                          "         _else 2");
        PTR(EqExpr) eq16 = NEW(EqExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(3), letBase1));
        CHECK(eq16->to_pretty_string() == "2 == 3 + _let x = 1\n"
                                          "         _in  x + 1");
        CHECK(eq16->interp()->equals(NEW(BoolVal) (false)));

        PTR(EqExpr) eq17 = NEW(EqExpr)(eq4, NEW(BoolExpr)(true));
        CHECK(eq17->to_pretty_string() == "(2 == 1 + 1) == _true");
        PTR(EqExpr) eq18 = NEW(EqExpr)(NEW(BoolExpr)(false), eq4);
        CHECK(eq18->to_pretty_string() == "_false == 2 == 1 + 1");
        PTR(EqExpr) eq19 = NEW(EqExpr)(NEW(AddExpr)(1, 1), NEW(AddExpr)(2, 0));
        CHECK(eq19->interp());
        PTR(LetExpr) eq20 = NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
        CHECK(eq20->interp());
        PTR(EqExpr) eq21 = NEW(EqExpr)(NEW(AddExpr)(letBase1, letBase2), NEW(NumExpr)(6));
        CHECK(eq21->interp());
        eq21->to_pretty_string();
        CHECK(eq21->to_pretty_string() == "(_let x = 1\n"
                                          " _in  x + 1) + (_let y = 2\n"
                                          "                _in  y + 2) == 6");
    }
}

TEST_CASE("IfExpr") {
    PTR(IfExpr) ifBase1 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(IfExpr) ifBase2 = NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(IfExpr) ifBase3 = NEW(IfExpr)(NEW(EqExpr)(1, 2), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)), NEW(NumExpr)(88));
    PTR(IfExpr) ifBase4 = NEW(IfExpr)(NEW(EqExpr)(2, 2), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)), NEW(NumExpr)(88));
    PTR(IfExpr) ifBase5 = NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)), NEW(NumExpr)(1), NEW(NumExpr)(0));

    SECTION("equals") {
        PTR(IfExpr) if1 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2));
        PTR(IfExpr) if2 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(2));
        PTR(IfExpr) if3 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(VarExpr)("x"));
        CHECK(ifBase1->equals(if1));
        CHECK(!ifBase1->equals(if2));
        CHECK(!ifBase1->equals(if3));
        CHECK(!ifBase1->equals(ifBase2));
        CHECK(!ifBase1->equals(NEW(NumExpr)(1)));
    }

    SECTION("interp") {
        CHECK(ifBase1->interp()->equals(NEW(NumVal) (1)));
        CHECK(ifBase2->interp()->equals(NEW(NumVal) (2)));
        PTR(LetExpr) if1 = NEW(LetExpr)("x", NEW(EqExpr)(1, 2), ifBase3);
        CHECK(if1->interp()->equals(NEW(NumVal) (88)));
        PTR(LetExpr) if2 = NEW(LetExpr)("x", NEW(EqExpr)(1, 2), ifBase4);
        CHECK_THROWS_WITH(if2->interp(), "add of non-number");
        CHECK(ifBase3->interp()->equals(NEW(NumVal) (88)));
        CHECK_THROWS_WITH(ifBase4->interp(), "add of non-number");
        CHECK_THROWS_WITH(ifBase5->interp(), "A variable has no value!");
    }

    SECTION("subst") {
        CHECK(ifBase1->subst("x", NEW(NumExpr)(10))->equals(ifBase1));
        PTR(IfExpr) if1 = NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(10), NEW(NumExpr)(3)), NEW(NumExpr)(1), NEW(NumExpr)(0));
        CHECK(ifBase5->subst("x", NEW(NumExpr)(10))->equals(if1));
    }

    SECTION("print") {
        CHECK(ifBase1->to_string() == "(_if _true _then 1 _else 2)");
        CHECK(ifBase2->to_string() == "(_if _false _then 1 _else 2)");
        CHECK(ifBase3->to_string() == "(_if (1==2) _then (_false+5) _else 88)");
        CHECK(ifBase4->to_string() == "(_if (2==2) _then (_false+5) _else 88)");
        CHECK(ifBase5->to_string() == "(_if (x==3) _then 1 _else 0)");
    }

    SECTION("pretty_print") {

        CHECK(ifBase1->to_pretty_string() == "_if _true\n_then 1\n_else 2");
        CHECK(ifBase2->to_pretty_string() == "_if _false\n_then 1\n_else 2");
        CHECK(ifBase3->to_pretty_string() == "_if 1 == 2\n_then _false + 5\n_else 88");
        CHECK(ifBase4->to_pretty_string() == "_if 2 == 2\n_then _false + 5\n_else 88");
        CHECK(ifBase5->to_pretty_string() == "_if x == 3\n_then 1\n_else 0");

        PTR(LetExpr) if1 = NEW(LetExpr)("x", NEW(EqExpr)(1, 2), ifBase3);
        CHECK(if1->to_pretty_string() == "_let x = 1 == 2\n"
                                         "_in  _if 1 == 2\n"
                                         "     _then _false + 5\n"
                                         "     _else 88");
        CHECK(parse_str(if1->to_string())->equals(if1));

        PTR(LetExpr) if2 = NEW(LetExpr)("x", ifBase1, NEW(AddExpr)("x", 1));
        CHECK(if2->to_pretty_string() == "_let x = _if _true\n"
                                         "         _then 1\n"
                                         "         _else 2\n"
                                         "_in  x + 1");
        CHECK(parse_str(if2->to_string())->equals(if2));

        PTR(LetExpr) if3 = NEW(LetExpr)("x", NEW(MultExpr)(ifBase1, 3), NEW(AddExpr)("x", 1));
        CHECK(if3->to_pretty_string() == "_let x = (_if _true\n"
                                         "          _then 1\n"
                                         "          _else 2) * 3\n"
                                         "_in  x + 1");
        CHECK(parse_str(if3->to_string())->equals(if3));

        PTR(LetExpr) if4 = NEW(LetExpr)("x", NEW(EqExpr)(ifBase1, NEW(NumExpr)(3)), NEW(AddExpr)("x", 1));
        CHECK(if4->to_pretty_string() == "_let x = (_if _true\n"
                                         "          _then 1\n"
                                         "          _else 2) == 3\n"
                                         "_in  x + 1");
        CHECK(parse_str(if4->to_string())->equals(if4));

        PTR(LetExpr) if5 = NEW(LetExpr)("x", NEW(AddExpr)(ifBase1, NEW(NumExpr)(3)), NEW(AddExpr)("x", 1));
        CHECK(if5->to_pretty_string() == "_let x = (_if _true\n"
                                         "          _then 1\n"
                                         "          _else 2) + 3\n"
                                         "_in  x + 1");
        CHECK(parse_str(if5->to_string())->equals(if5));

        PTR(LetExpr) if6 = NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(ifBase1, NEW(VarExpr)("x")));
        CHECK(if6->to_pretty_string() == "_let x = 1\n"
                                         "_in  (_if _true\n"
                                         "      _then 1\n"
                                         "      _else 2) + x");
        CHECK(if6->interp()->equals(NEW(NumVal) (2)));
        CHECK(parse_str(if6->to_string())->equals(if6));
        PTR(AddExpr) if7 = NEW(AddExpr)(if6, NEW(NumExpr)(1));
        CHECK(if7->to_pretty_string() == "(_let x = 1\n"
                                         " _in  (_if _true\n"
                                         "       _then 1\n"
                                         "       _else 2) + x) + 1");
        CHECK(parse_str(if7->to_string())->equals(if7));

        PTR(IfExpr) if8 = NEW(IfExpr)(NEW(BoolExpr)(true), ifBase2, NEW(NumExpr)(3));
        CHECK(if8->to_pretty_string() == "_if _true\n"
                                         "_then _if _false\n"
                                         "      _then 1\n"
                                         "      _else 2\n"
                                         "_else 3");
        CHECK(if8->interp()->equals(NEW(NumVal) (2)));
        CHECK(parse_str(if8->to_string())->equals(if8));

        PTR(MultExpr) if9 = NEW(MultExpr)(3, ifBase1);
        CHECK(if9->to_pretty_string() == "3 * _if _true\n"
                                         "    _then 1\n"
                                         "    _else 2");
        CHECK(parse_str(if9->to_string())->equals(if9));
        PTR(MultExpr) if10 = NEW(MultExpr)(if9, 4);
        CHECK(if10->to_pretty_string() == "(3 * _if _true\n"
                                          "     _then 1\n"
                                          "     _else 2) * 4");
        CHECK(parse_str(if10->to_string())->equals(if10));
        PTR(AddExpr) if11 = NEW(AddExpr)(1, if9);
        CHECK(if11->to_pretty_string() == "1 + 3 * _if _true\n"
                                          "        _then 1\n"
                                          "        _else 2");
        CHECK(parse_str(if11->to_string())->equals(if11));
        PTR(MultExpr) if12 = NEW(MultExpr)(1, if9);
        CHECK(if12->to_pretty_string() == "1 * 3 * _if _true\n"
                                          "        _then 1\n"
                                          "        _else 2");
        CHECK(parse_str(if12->to_string())->equals(if12));
        PTR(AddExpr) if13 = NEW(AddExpr)(if9, 5);
        CHECK(if13->to_pretty_string() == "3 * (_if _true\n"
                                          "     _then 1\n"
                                          "     _else 2) + 5");
        CHECK(parse_str(if13->to_string())->equals(if13));
        PTR(AddExpr) if14 = NEW(AddExpr)(if12, 5);
        CHECK(if14->to_pretty_string() == "1 * 3 * (_if _true\n"
                                          "         _then 1\n"
                                          "         _else 2) + 5");
        CHECK(parse_str(if14->to_string())->equals(if14));
    }
}

TEST_CASE("FunExpr") {
    PTR(FunExpr) func1 = NEW(FunExpr)("x", NEW(AddExpr)("x", 1));
    PTR(FunExpr) func2 = NEW(FunExpr)("x", NEW(FunExpr)("y", NEW(AddExpr)(NEW(MultExpr)("x", "x"), NEW(MultExpr)("y", "y"))));

    SECTION("equals") {
        CHECK(!func1->equals(NEW(NumExpr)(1)));
        CHECK(!func1->equals(NEW(FunExpr)("x", NEW(AddExpr)("x", 2))));
        CHECK(!func1->equals(NEW(FunExpr)("y", NEW(AddExpr)("x", 1))));
        CHECK(func1->equals(NEW(FunExpr)("x", NEW(AddExpr)("x", 1))));
    }

    SECTION("print") {
        CHECK(func1->to_string() == "(_fun (x) (x+1))");
        CHECK(func2->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y))))");
    }

    SECTION("pretty_print") {
        CHECK(func1->to_pretty_string() == "_fun (x) \n"
                                           "  x + 1");

        CHECK(func2->to_pretty_string() == "_fun (x) \n"
                                           "  _fun (y) \n"
                                           "    x * x + y * y");
    }
}

TEST_CASE("CallExpr") {
    PTR(FunExpr) func1 = NEW(FunExpr)("x", NEW(AddExpr)("x", 1));
    PTR(FunExpr) func2 = NEW(FunExpr)("y", NEW(AddExpr)("y", 2));
    PTR(CallExpr) call1 = NEW(CallExpr)(func1, 2);

    SECTION("equals") {
        CHECK(call1->equals(NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)("x", 1)), 2)));
        CHECK(!call1->equals(NEW(NumExpr)(2)));
        CHECK(!call1->equals(NEW(CallExpr)(func1, 3)));
        CHECK(!call1->equals(NEW(CallExpr)(NEW(FunExpr)("x", NEW(MultExpr)("x", 2)), 2)));
    }

    SECTION("interp + print + pretty-print") {
        CHECK(call1->interp()->equals(NEW(NumVal) (3)));
        std::string s1 = "(_fun (x) (x+1))(2)";
        std::string ps1 = "(_fun (x) \n"
                          "   x + 1)(2)";
        CHECK(call1->to_string() == s1);
        CHECK(call1->to_pretty_string() == ps1);
        CHECK(parse_str(s1)->equals(call1));
        CHECK(parse_str(ps1)->equals(call1));

        PTR(LetExpr) call2 = NEW(LetExpr)("f", func1, NEW(CallExpr)(NEW(VarExpr)("f"), 2));
        CHECK(call2->interp()->equals(NEW(NumVal) (3)));
        std::string s2 = "(_let f=(_fun (x) (x+1)) _in f(2))";
        std::string ps2 = "_let f = _fun (x) \n"
                          "           x + 1\n"
                          "_in  f(2)";
        CHECK(call2->to_string() == s2);
        CHECK(call2->to_pretty_string() == ps2);
        CHECK(parse_str(s2)->equals(call2));
        CHECK(parse_str(ps2)->equals(call2));

        PTR(LetExpr) call3 = NEW(LetExpr)("f", func1, NEW(IfExpr)(false, NEW(CallExpr)("f", 5), NEW(CallExpr)("f", 6)));
        CHECK(call3->interp()->equals(NEW(NumVal) (7)));
        std::string ps3 = "_let f = _fun (x) \n"
                          "           x + 1\n"
                          "_in  _if _false\n"
                          "     _then f(5)\n"
                          "     _else f(6)";
        CHECK(call3->to_pretty_string() == ps3);
        CHECK(parse_str(ps3)->equals(call3));

        PTR(LetExpr) call4 = NEW(LetExpr)("f", func1, NEW(LetExpr)("g", func2, NEW(IfExpr)(true, NEW(CallExpr)("f", 5), NEW(CallExpr)("g", 6))));
        CHECK(call4->interp()->equals(NEW(NumVal) (6)));
        std::string ps4 = "_let f = _fun (x) \n"
                          "           x + 1\n"
                          "_in  _let g = _fun (y) \n"
                          "                y + 2\n"
                          "     _in  _if _true\n"
                          "          _then f(5)\n"
                          "          _else g(6)";
        CHECK(call4->to_pretty_string() == ps4);
        CHECK(parse_str(ps4)->equals(call4));
        PTR(LetExpr) call5 = NEW(LetExpr)("f", func1, NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(CallExpr)("f", NEW(AddExpr)("y", 2))), NEW(CallExpr)("g", 5)));
        CHECK(call5->interp()->equals(NEW(NumVal) (8)));
        std::string ps5 = "_let f = _fun (x) \n"
                          "           x + 1\n"
                          "_in  _let g = _fun (y) \n"
                          "                f(y + 2)\n"
                          "     _in  g(5)";
        CHECK(call5->to_pretty_string() == ps5);
        CHECK(parse_str(ps5)->equals(call5));
        PTR(LetExpr) call6 = NEW(LetExpr)("f", func1, NEW(LetExpr)("g", NEW(FunExpr)("x", NEW(AddExpr)(NEW(CallExpr)("f", 2), "x")), NEW(CallExpr)("g", 5)));
        CHECK(call6->interp()->equals(NEW(NumVal) (8)));
        std::string ps6 = "_let f = _fun (x) \n"
                          "           x + 1\n"
                          "_in  _let g = _fun (x) \n"
                          "                f(2) + x\n"
                          "     _in  g(5)";
        CHECK(call6->to_pretty_string() == ps6);
        CHECK(parse_str(ps6)->equals(call6));
        PTR(LetExpr) call7 = NEW(LetExpr)("f", NEW(IfExpr)(false, func1, func2), NEW(CallExpr)("f", 5));
        CHECK(call7->interp()->equals(NEW(NumVal) (7)));
        std::string ps7 = "_let f = _if _false\n"
                          "         _then _fun (x) \n"
                          "                 x + 1\n"
                          "         _else _fun (y) \n"
                          "                 y + 2\n"
                          "_in  f(5)";
        CHECK(call7->to_pretty_string() == ps7);
        CHECK(parse_str(ps7)->equals(call7));
        PTR(CallExpr) call8 = NEW(CallExpr)(NEW(IfExpr)(false, func1, func2), NEW(NumExpr)(5));
        CHECK(call8->interp()->equals(NEW(NumVal) (7)));
        std::string ps8 = "(_if _false\n"
                          " _then _fun (x) \n"
                          "         x + 1\n"
                          " _else _fun (y) \n"
                          "         y + 2)(5)";
        CHECK(call8->to_pretty_string() == ps8);
        CHECK(parse_str(ps8)->equals(call8));
        PTR(LetExpr) call9 = NEW(LetExpr)("f", NEW(FunExpr)("g", NEW(CallExpr)("g", 5)), NEW(LetExpr)("g", func2, NEW(CallExpr)("f", "g")));
        CHECK(call9->interp()->equals(NEW(NumVal) (7)));
        std::string ps9 = "_let f = _fun (g) \n"
                          "           g(5)\n"
                          "_in  _let g = _fun (y) \n"
                          "                y + 2\n"
                          "     _in  f(g)";
        CHECK(call9->to_pretty_string() == ps9);
        CHECK(parse_str(ps9)->equals(call9));
        PTR(LetExpr) call10 = NEW(LetExpr)("f", NEW(FunExpr)("g", NEW(CallExpr)("g", 5)), NEW(CallExpr)("f", func2));
        CHECK(call10->interp()->equals(NEW(NumVal) (7)));
        std::string ps10 = "_let f = _fun (g) \n"
                           "           g(5)\n"
                           "_in  f(_fun (y) \n"
                           "         y + 2)";
        CHECK(call10->to_pretty_string() == ps10);
        CHECK(parse_str(ps10)->equals(call10));
        PTR(LetExpr) call11 = NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(FunExpr)("y", NEW(AddExpr)("x", "y"))), NEW(CallExpr)(NEW(CallExpr)("f", 5), 1));
        CHECK(call11->interp()->equals(NEW(NumVal) (6)));
        std::string ps11 = "_let f = _fun (x) \n"
                           "           _fun (y) \n"
                           "             x + y\n"
                           "_in  f(5)(1)";
        CHECK(call11->to_pretty_string() == ps11);
        CHECK(parse_str(ps11)->equals(call11));
        PTR(LetExpr) call12 = NEW(LetExpr)("y", NEW(NumExpr)(8), NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(MultExpr)("x", "y")), NEW(CallExpr)("f", 2)));
        CHECK(call12->interp()->equals(NEW(NumVal) (16)));
        std::string ps12 = "_let y = 8\n"
                           "_in  _let f = _fun (x) \n"
                           "                x * y\n"
                           "     _in  f(2)";
        CHECK(call12->to_pretty_string() == ps12);
        CHECK(parse_str(ps12)->equals(call12));
        PTR(LetExpr) call13 = NEW(LetExpr)("x", NEW(NumExpr)(8), NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(MultExpr)("x", "x")), NEW(CallExpr)("f", 2)));
        CHECK(call13->interp()->equals(NEW(NumVal) (4)));
        std::string ps13 = "_let x = 8\n"
                           "_in  _let f = _fun (x) \n"
                           "                x * x\n"
                           "     _in  f(2)";
        CHECK(call13->to_pretty_string() == ps13);
        CHECK(parse_str(ps13)->equals(call13));
        PTR(LetExpr) call14 = NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)("x", "y")), NEW(LetExpr)("y", NEW(NumExpr)(10), NEW(CallExpr)("f", 1)));
        CHECK(call14->interp()->equals(NEW(NumVal) (11)));
        std::string ps14 = "_let f = _fun (x) \n"
                           "           x + y\n"
                           "_in  _let y = 10\n"
                           "     _in  f(1)";
        CHECK(call14->to_pretty_string() == ps14);
        CHECK(parse_str(ps14)->equals(call14));
        PTR(LetExpr) call15 = NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(FunExpr)("y", NEW(AddExpr)(NEW(MultExpr)("x", "x"), NEW(MultExpr)("y", "y")))), NEW(CallExpr)(NEW(CallExpr)("f", 2), 3));
        CHECK(call15->interp()->equals(NEW(NumVal) (13)));
        std::string ps15 = "_let f = _fun (x) \n"
                           "           _fun (y) \n"
                           "             x * x + y * y\n"
                           "_in  f(2)(3)";
        CHECK(call15->to_pretty_string() == ps15);
        CHECK(parse_str(ps15)->equals(call15));
        PTR(LetExpr) call16 = NEW(LetExpr)("factrl", NEW(FunExpr)("factrl", NEW(FunExpr)("x", NEW(IfExpr)(NEW(EqExpr)("x", 1), NEW(NumExpr)(1), NEW(MultExpr)("x", NEW(CallExpr)(NEW(CallExpr)("factrl", "factrl"), NEW(AddExpr)("x", -1)))))), NEW(CallExpr)(NEW(CallExpr)("factrl", "factrl"), 10));
        CHECK(call16->interp()->equals(NEW(NumVal) (3628800)));
        std::string ps16 = "_let factrl = _fun (factrl) \n"
                           "                _fun (x) \n"
                           "                  _if x == 1\n"
                           "                  _then 1\n"
                           "                  _else x * factrl(factrl)(x + -1)\n"
                           "_in  factrl(factrl)(10)";
        CHECK(call16->to_pretty_string() == ps16 );
        CHECK(parse_str(ps16)->equals(call16));
        PTR(LetExpr) call17 = NEW(LetExpr)("y", NEW(NumExpr)(10), NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)("x", "y")), 1));
        CHECK(call17->interp()->equals(NEW(NumVal) (11)));
        std::string ps17 = "_let y = 10\n"
                           "_in  (_fun (x) \n"
                           "        x + y)(1)";
        CHECK(call17->to_pretty_string() == ps17);
        CHECK(parse_str(ps17)->equals(call17));

        PTR(CallExpr) call18 = NEW(CallExpr)(NEW(LetExpr)("y", NEW(NumExpr)(10), NEW(FunExpr)("x", NEW(AddExpr)("x", "y"))), 1);
        CHECK(call18->interp()->equals(NEW(NumVal) (11)));
        std::string ps18 = "(_let y = 10\n"
                           " _in  _fun (x) \n"
                           "        x + y)(1)";
        CHECK(call18->to_pretty_string() == ps18);
        CHECK(parse_str(ps18)->equals(call18));

        PTR(CallExpr) call19 = NEW(CallExpr)(NEW(IfExpr)(false, func1, func2), 5);
        CHECK(call19->interp()->equals(NEW(NumVal) (7)));
        std::string ps19 = "(_if _false\n"
                           " _then _fun (x) \n"
                           "         x + 1\n"
                           " _else _fun (y) \n"
                           "         y + 2)(5)";
        CHECK(call19->to_pretty_string() == ps19);
        CHECK(parse_str(ps19)->equals(call19));
    }

    SECTION("edge for parse") {
        CHECK_THROWS_WITH(parse_str("3(5)")->interp(), "no function to call!");
        CHECK_THROWS_WITH(parse_str("(1+2)(5)")->interp(), "no function to call!");
        CHECK_THROWS_WITH(parse_str("(1*2)(5)")->interp(), "no function to call!");
        CHECK_THROWS_WITH(parse_str("(_true)(5)")->interp(), "no function to call!");
        CHECK_THROWS_WITH(parse_str("(1==2)(5)")->interp(), "no function to call!");
    }
}

