#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <iostream>

TEST_CASE("Equals") {
    SECTION("NumExpr") {
        NumExpr num1(1);
        NumExpr num2(2);
        NumExpr num3(1);
        NumExpr num4(-1);
        CHECK_FALSE(num1.equals(&num2));
        CHECK_FALSE(num2.equals(&num1));
        CHECK(num1.equals(&num3));
        CHECK(num3.equals(&num1));
        CHECK_FALSE(num2.equals(&num3));
        CHECK_FALSE(num3.equals(&num2));
        CHECK(!num1.equals(&num4));
        CHECK(!num1.equals(new VarExpr("x")));
    }

    SECTION("Add_simple") {
        AddExpr add1(new NumExpr(1), new NumExpr(2));
        AddExpr add2(new NumExpr(2), new NumExpr(1));
        AddExpr add3(new NumExpr(1), new NumExpr(2));
        AddExpr add4(1, 3);
        CHECK_FALSE(add1.equals(&add2));
        CHECK_FALSE(add2.equals(&add1));
        CHECK(add1.equals(&add3));
        CHECK(add3.equals(&add1));
        CHECK(!add2.equals(&add3));
        CHECK(!add3.equals(&add2));
        CHECK(!add1.equals(new MultExpr(1, 2)));
        CHECK(!add1.equals(&add4));
    }

    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 2), new MultExpr(3, 4));
        AddExpr add2(new MultExpr(3, 4), new AddExpr(1, 2));
        AddExpr add3(new AddExpr(1, 2), new MultExpr(3, 4));
        CHECK_FALSE(add1.equals(&add2));
        CHECK_FALSE(add2.equals(&add1));
        CHECK(add1.equals(&add3));
        CHECK(add3.equals(&add1));
        CHECK(!add2.equals(&add3));
        CHECK(!add3.equals(&add2));
        CHECK(!add1.equals(new NumExpr(5)));
    }

    SECTION("Mult_simple") {
        MultExpr mult1(1, 2);
        MultExpr mult2(2, 1);
        MultExpr mult3(1, 2);
        MultExpr mult4(1, 3);
        CHECK_FALSE(mult1.equals(&mult2));
        CHECK_FALSE(mult2.equals(&mult1));
        CHECK(mult1.equals(&mult3));
        CHECK(mult3.equals(&mult1));
        CHECK(!mult2.equals(&mult3));
        CHECK(!mult3.equals(&mult2));
        CHECK(!mult1.equals(new VarExpr("x")));
        CHECK(!mult1.equals(&mult4));
    }

    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(1, 2), new MultExpr(3, 4));
        MultExpr mult2(new MultExpr(3, 4), new AddExpr(1, 2));
        MultExpr mult3(new AddExpr(1, 2), new MultExpr(3, 4));
        CHECK_FALSE(mult1.equals(&mult2));
        CHECK_FALSE(mult2.equals(&mult1));
        CHECK(mult1.equals(&mult3));
        CHECK(mult3.equals(&mult1));
        CHECK(!mult2.equals(&mult3));
        CHECK(!mult3.equals(&mult2));
    }

    SECTION("Variable") {
        VarExpr var1("a");
        VarExpr var2("1");
        VarExpr var3("a");
        CHECK_FALSE(var1.equals(&var2));
        CHECK(var1.equals(&var3));
        CHECK(!var2.equals(&var3));
        CHECK(!var1.equals(new NumExpr(1)));
    }
};

TEST_CASE("Interp") {
    SECTION("NumExpr") {
        CHECK((new NumExpr(0))->interp()->equals(new NumVal(0)));
        CHECK((new NumExpr(-100))->interp()->equals(new NumVal(-100)));
    }

    SECTION("Add_simple") {
        CHECK((new AddExpr(1, 3))->interp()->equals(new NumVal(4)));
        CHECK((new AddExpr(-2019, 5072))->interp()->equals(new NumVal(3053)));
        CHECK((new AddExpr(new NumExpr(57), new AddExpr(3, -800)))->interp()->equals(new NumVal(-740)));
    }

    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 9), new MultExpr(2, 50));
        AddExpr add2(new AddExpr(-31000, 19), new AddExpr(20, 61));
        AddExpr add3(new AddExpr(1010, 2), new MultExpr(0, 10000));
        CHECK(add1.interp()->equals(new NumVal(110)));
        CHECK(add2.interp()->equals(new NumVal(-30900)));
        CHECK(add3.interp()->equals(new NumVal(1012)));
    }

    SECTION("Mult_simple") {
        MultExpr mult1(-3, 2);
        MultExpr mult2(-1034, -730);
        MultExpr mult3(12345678, 0);
        CHECK(mult1.interp()->equals(new NumVal(-6)));
        CHECK(mult2.interp()->equals(new NumVal(754820)));
        CHECK(mult3.interp()->equals(new NumVal(0)));
    }

    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(2, 3), new MultExpr(4, 5));
        MultExpr mult2(new MultExpr(-5, 10), new MultExpr(-300, 20));
        MultExpr mult3(new MultExpr(2, 6), new AddExpr(1, -9));
        CHECK(mult1.interp()->equals(new NumVal(100)));
        CHECK(mult2.interp()->equals(new NumVal(300000)));
        CHECK(mult3.interp()->equals(new NumVal(-96)));
    }

    SECTION("Variable") {
        CHECK_THROWS_WITH((new VarExpr("x")) -> interp(), "A variable has no value!");
        CHECK_THROWS_WITH((new AddExpr(6, "y")) -> interp(), "A variable has no value!");
    }
}

TEST_CASE("has_variable") {
    SECTION("NumExpr") {
        CHECK(!(new NumExpr(3)) -> has_variable());
    }

    SECTION("Add_simple") {
        CHECK((new AddExpr(7, "x")) -> has_variable());
        CHECK(!(new AddExpr(2, 3)) -> has_variable());
    }

    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 5), new AddExpr(new NumExpr(1), new AddExpr(9, "speed")));
        AddExpr add2(new AddExpr(1010, 2), new MultExpr(0, 10000));
        CHECK(add1.has_variable());
        CHECK(!add2.has_variable());
    }

    SECTION("Mult_simple") {
        MultExpr mult1(3, 2);
        MultExpr mult2(8, "yy");
        CHECK(!mult1.has_variable());
        CHECK(mult2.has_variable());
    }

    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(2, 3), new MultExpr(4, 5));
        MultExpr mult2(new MultExpr(1, 5), new AddExpr(new NumExpr(1), new AddExpr(9, "scale")));
        CHECK(!mult1.has_variable());
        CHECK(mult2.has_variable());
    }

    SECTION("Variable") {
        CHECK((new VarExpr("any thing")) -> has_variable());
    }
}

TEST_CASE("subst") {
    SECTION("NumExpr") {
        CHECK((new NumExpr(4)) -> subst("x", new NumExpr(3)) -> equals(new NumExpr(4)));
    }

    SECTION("Add_simple") {
        CHECK(AddExpr("x", 7).subst("x", new VarExpr("y"))->equals(new AddExpr("y", 7)));
        CHECK(AddExpr(2, 3).subst("x", new NumExpr(1))->equals(new AddExpr(2, 3)));
    }

    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 5), new AddExpr("a", 9));
        AddExpr res11(new AddExpr(1, 5), new AddExpr(100, 9));
        AddExpr res12(new AddExpr(1, 5), new AddExpr("b", 9));
        AddExpr res13(new AddExpr(1, 5), new AddExpr("a", 9));
        AddExpr add2(new AddExpr("x", 3), new MultExpr("y", -10));
        AddExpr res21(new AddExpr(new VarExpr("y"), new NumExpr(3)), new MultExpr("y", -10));
        AddExpr res22(new AddExpr(new AddExpr(1, 3), new NumExpr(3)), new MultExpr("y", -10));
        CHECK(add1.subst("a", new NumExpr(100)) -> equals(&res11));
        CHECK(add1.subst("a", new VarExpr("b")) -> equals(&res12));
        CHECK(add1.subst("b", new NumExpr(7)) -> equals(&res13));
        CHECK(add2.subst("x", new VarExpr("y")) -> equals(&res21));
        CHECK(add2.subst("x", new AddExpr(1, 3)) -> equals(&res22));
    }

    SECTION("Mult_simple") {
        CHECK( MultExpr("x", 7).subst("x", new VarExpr("y"))->equals(new MultExpr("y", 7)));
        CHECK(MultExpr(2, 3).subst("x", new NumExpr(1))->equals(new MultExpr(2, 3)));
    }

    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(1, 5), new MultExpr("a", 9));
        MultExpr res11(new AddExpr(1, 5), new MultExpr(100, 9));
        MultExpr res12(new AddExpr(1, 5), new MultExpr("b", 9));
        MultExpr res13(new AddExpr(1, 5), new MultExpr("a", 9));
        MultExpr mult2(new MultExpr(new VarExpr("x"), new NumExpr(3)), new MultExpr("y", -10));
        MultExpr res21(new MultExpr(new VarExpr("y"), new NumExpr(3)), new MultExpr("y", -10));
        MultExpr res22(new MultExpr(new AddExpr(1, 3), new NumExpr(3)), new MultExpr("y", -10));
        CHECK(mult1.subst("a", new NumExpr(100)) -> equals(&res11));
        CHECK(mult1.subst("a", new VarExpr("b")) -> equals(&res12));
        CHECK(mult1.subst("b", new NumExpr(7)) -> equals(&res13));
        CHECK(mult2.subst("x", new VarExpr("y")) -> equals(&res21));
        CHECK(mult2.subst("x", new AddExpr(1, 3)) -> equals(&res22));
    }

    SECTION("Variable") {
        CHECK((new VarExpr("x"))->subst("x", new AddExpr("y", 7)) -> equals(new AddExpr("y", 7)));
        CHECK(VarExpr("x").subst("y", new NumExpr(3)) -> equals(new VarExpr("x")));
    }
}

TEST_CASE("Interpt + Subst") {
    AddExpr add1(new AddExpr(1, 5), new AddExpr("a", 9));
    CHECK(add1.subst("a", new NumExpr(100))->interp()->equals(new NumVal(115)));
    MultExpr mult1(new AddExpr(1, 5), new AddExpr("a", 9));
    CHECK(mult1.subst("a", new NumExpr(100))->interp()->equals(new NumVal(654)));

};

TEST_CASE("to_string + print") {
    SECTION("NumExpr") {
        CHECK(NumExpr(10).to_string() == "10");
        CHECK(NumExpr(-10).to_string() == "-10");
    }

    SECTION("Add_simple") {
        CHECK(AddExpr(1, 2).to_string() == "(1+2)");
        CHECK(AddExpr("x", 2).to_string() == "(x+2)");
        CHECK(AddExpr(-3, "speed").to_string() == "(-3+speed)");
        CHECK(AddExpr(-10, -5).to_string() == "(-10+-5)");
    }

    SECTION("Add_nested") {
        AddExpr add1(new NumExpr(1), new AddExpr(-10, 2));
        CHECK(add1.to_string() == "(1+(-10+2))");
        AddExpr add2(new AddExpr(1, 2), new AddExpr(3, 4));
        CHECK(add2.to_string() == "((1+2)+(3+4))");
        AddExpr add3(new AddExpr(1001, 50), new VarExpr("p"));
        CHECK(add3.to_string() == "((1001+50)+p)");
    }

    SECTION("Mult_simple") {
        CHECK(MultExpr(-1, 2).to_string() == "(-1*2)");
        CHECK(MultExpr("x", "y").to_string() == "(x*y)");
        CHECK(MultExpr("z", 2).to_string() == "(z*2)");
    }

    SECTION("Mult_nested") {
        MultExpr mult1(new NumExpr(1), new MultExpr(-10, 2));
        CHECK(mult1.to_string() == "(1*(-10*2))");
        MultExpr mult2(new AddExpr(2, 7), new MultExpr(5, -20));
        CHECK(mult2.to_string() == "((2+7)*(5*-20))");
        MultExpr mult3(new MultExpr(1001, 50), new VarExpr("p"));
        CHECK(mult3.to_string() == "((1001*50)*p)");
    }

    SECTION("Variable") {
        CHECK(VarExpr("xyz").to_string() == "xyz");
    }
};

TEST_CASE("pretty_print") {
    SECTION("num") {
        CHECK(NumExpr(1).to_pretty_string() == "1");
    }

    SECTION("add") {
        AddExpr add1(new AddExpr(1, 2), new NumExpr(3));
        CHECK(add1.to_pretty_string() == "(1 + 2) + 3");
        AddExpr add2(new NumExpr(1), new AddExpr(2, 3));
        CHECK(add2.to_pretty_string() == "1 + 2 + 3");
        AddExpr add3(new AddExpr(1, 2), new AddExpr(new NumExpr(3), new AddExpr(4, 5)));
        CHECK(add3.to_pretty_string() == "(1 + 2) + 3 + 4 + 5");
        AddExpr add4(new AddExpr(1, 2), new AddExpr(new AddExpr(3, 4), new NumExpr(5)));
        CHECK(add4.to_pretty_string() == "(1 + 2) + (3 + 4) + 5");
        AddExpr add5(new MultExpr(1, 2), new MultExpr(3, 4));
        CHECK(add5.to_pretty_string() == "1 * 2 + 3 * 4");
        AddExpr add6(new AddExpr(1, 2), new MultExpr(3, 4));
        CHECK(add6.to_pretty_string() == "(1 + 2) + 3 * 4");
    }

    SECTION("mult") {
        MultExpr mult1(new MultExpr(1, 2), new NumExpr(3));
        CHECK(mult1.to_pretty_string() == "(1 * 2) * 3");
        MultExpr mult2(new NumExpr(1), new MultExpr(2, 3));
        CHECK(mult2.to_pretty_string() == "1 * 2 * 3");
        MultExpr mult3(new MultExpr(1, 2), new MultExpr(new NumExpr(3), new MultExpr(4, 5)));
        CHECK(mult3.to_pretty_string() == "(1 * 2) * 3 * 4 * 5");
        MultExpr mult4(new MultExpr(1, 2), new MultExpr(new MultExpr(3, 4), new NumExpr(5)));
        CHECK(mult4.to_pretty_string() == "(1 * 2) * (3 * 4) * 5");
        MultExpr mult5(new MultExpr(1, 2), new AddExpr(3, 4));
        CHECK(mult5.to_pretty_string() == "(1 * 2) * (3 + 4)");
        MultExpr mult6(new MultExpr(1, 2), new VarExpr("x"));
        CHECK(mult6.to_pretty_string() == "(1 * 2) * x");
    }

    SECTION("add + mult") {
        CHECK(AddExpr(new NumExpr(1), new MultExpr(2, 3)).to_pretty_string() == "1 + 2 * 3");
        CHECK(MultExpr(new NumExpr(1), new AddExpr(2, 3)).to_pretty_string() == "1 * (2 + 3)");
    }

    SECTION("Variable") {
        CHECK(VarExpr("xyz").to_string() == "xyz");
    }
}

TEST_CASE("let") {
    LetExpr letBase1("x", new NumExpr(5), new AddExpr("x", 1));
    LetExpr letBase2("x", new NumExpr(6), new AddExpr("x", 1));

    SECTION("equals") {
        LetExpr let1("x", new NumExpr(5), new AddExpr("x", 1));
        CHECK(letBase1.equals(&let1));
        CHECK(!letBase2.equals(&let1));
        CHECK(!letBase1.equals(new AddExpr(1, 2)));
    }

    SECTION("subst + interp") {
        CHECK(letBase1.interp()->equals(new NumVal(6)));
        CHECK(letBase2.interp()->equals(new NumVal(7)));
        LetExpr let1("x", new AddExpr(5, 2), new MultExpr("x", 3));
        CHECK(let1.interp()->equals(new NumVal(21)));
        LetExpr let2("x", new AddExpr(5, 2), new AddExpr("x", 1));
        CHECK(let2.interp()->equals(new NumVal(8)));
        LetExpr let3("x", new NumExpr(5), new LetExpr("x", new NumExpr(6), new AddExpr("x", 1)));
        CHECK(let3.interp()->equals(new NumVal(7)));
        LetExpr let4("x", new NumExpr(5), new LetExpr("y", new NumExpr(6), new AddExpr("x", 1)));
        CHECK(let4.interp()->equals(new NumVal(6)));
        LetExpr let5("x", new AddExpr(2, 5), new LetExpr("x", new AddExpr("x", -13), new AddExpr("x", 1)));
        CHECK(let5.interp()->equals(new NumVal(-5)));
        LetExpr let6("x", new AddExpr(-1, -6), new LetExpr("x", new AddExpr(-3, 2), new AddExpr("x", 3)));
        CHECK(let6.interp()->equals(new NumVal(2)));
        LetExpr let2Nested("x", new NumExpr(5), new LetExpr("y", new NumExpr(8), new AddExpr("x", "y")));
        CHECK(let2Nested.interp()->equals(new NumVal(13)));
        LetExpr let3Nested("x", new NumExpr(1), new LetExpr("y", new NumExpr(2), new LetExpr("z", new NumExpr(3), new AddExpr(new VarExpr("x"), new AddExpr("y", "z")))));
        CHECK(let3Nested.interp()->equals(new NumVal(6)));
        LetExpr let3Nested2("x", new NumExpr(1), new LetExpr("y", new AddExpr("x", 1), new LetExpr("z", new AddExpr("y", 1), new AddExpr(new VarExpr("x"), new AddExpr("y", "z")))));
        let3Nested2.interp();
        CHECK(let3Nested2.interp()->equals(new NumVal(6)));
        LetExpr letError("x", new AddExpr("y", 2), new AddExpr("x", 1));
        CHECK_THROWS_WITH(letError.interp(), "A variable has no value!");
    }

    SECTION("has_variable") {
        LetExpr let1("x", new NumExpr(5), new AddExpr(3, 1));
        CHECK(!let1.has_variable());
        CHECK(letBase1.has_variable()); // body has variable
        LetExpr let2("x", new VarExpr("x + 1"), new AddExpr(3, 1)); // rhs has variable
        CHECK(let2.has_variable());
        LetExpr let3("x", new VarExpr("x"), new AddExpr("x", 1)); // both has variable
        CHECK(let3.has_variable());
    }

    SECTION("print") {
        LetExpr let1("x",
                     new NumExpr(5),
                     new AddExpr(
                             new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))),
                             new VarExpr("x")));
        CHECK(let1.to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }

    SECTION("pretty_print") {
        CHECK(letBase1.to_pretty_string() == "_let x = 5\n_in  x + 1");
        CHECK(letBase2.to_pretty_string() == "_let x = 6\n_in  x + 1");
        // one nested let
        LetExpr let2("x", new NumExpr(5),&letBase2);
        CHECK(let2.to_pretty_string() == "_let x = 5\n"
                                         "_in  _let x = 6\n"
                                         "     _in  x + 1");
        // let body as left arg of add
        LetExpr let3("x",new NumExpr(5),new AddExpr(&letBase2, new VarExpr("x")));
        CHECK(let3.to_pretty_string() == "_let x = 5\n"
                                         "_in  (_let x = 6\n"
                                         "      _in  x + 1) + x");
        // let body as left arg of mult
        LetExpr let4("x", new NumExpr(5), new MultExpr(&letBase2, new VarExpr("x")));
        CHECK(let4.to_pretty_string() == "_let x = 5\n"
                                         "_in  (_let x = 6\n"
                                         "      _in  x + 1) * x");
        // let as right arg of unparenthized mult
        MultExpr let5(new NumExpr(2), &letBase1);
        CHECK(let5.to_pretty_string() == "2 * _let x = 5\n"
                                         "    _in  x + 1");
        // let as right arg of parenthized add
        AddExpr let6( new AddExpr(new NumExpr(2), &letBase1), new NumExpr(3));
        CHECK(let6.to_pretty_string() == "(2 + _let x = 5\n"
                                         "     _in  x + 1) + 3");
        // let as right arg of parenthized mult
        MultExpr let7(new MultExpr(new NumExpr(2), &letBase1), new NumExpr(3));
        CHECK(let7.to_pretty_string() == "(2 * _let x = 5\n"
                                         "     _in  x + 1) * 3");
        // let as rhs
        LetExpr let8("x", &letBase1, new AddExpr("x", 6));
        CHECK(let8.to_pretty_string() == "_let x = _let x = 5\n"
                                         "         _in  x + 1\n"
                                         "_in  x + 6");
        LetExpr let9("x", new AddExpr(&letBase1, new NumExpr(2)), new AddExpr("x", 6));
        CHECK(let9.to_pretty_string() == ("_let x = (_let x = 5\n"
                                          "          _in  x + 1) + 2\n"
                                          "_in  x + 6"));
        AddExpr let10(new NumExpr(1), new MultExpr(new NumExpr(3), &letBase1));
        CHECK(let10.to_pretty_string() == "1 + 3 * _let x = 5\n"
                                          "        _in  x + 1");
        MultExpr let11(new NumExpr(1), new MultExpr(new NumExpr(2), &letBase1));
        CHECK(let11.to_pretty_string() == "1 * 2 * _let x = 5\n"
                                          "        _in  x + 1");
        AddExpr let12(new MultExpr(new NumExpr(2), &letBase1), new NumExpr(5));
        CHECK(let12.to_pretty_string() == "2 * (_let x = 5\n"
                                          "     _in  x + 1) + 5");
        AddExpr let13(&let11, new NumExpr(5));
        CHECK(let13.to_pretty_string() == "1 * 2 * (_let x = 5\n"
                                          "         _in  x + 1) + 5");
        AddExpr let14(new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), &letBase1)), new NumExpr(1));
        CHECK(let14.to_pretty_string() == "(1 + 2 * _let x = 5\n"
                                          "         _in  x + 1) + 1");
        AddExpr let15(new MultExpr(new AddExpr(new NumExpr(2), &letBase1), new NumExpr(1)), new NumExpr(1));
        CHECK(let15.to_pretty_string() == "(2 + _let x = 5\n"
                                          "     _in  x + 1) * 1 + 1");
        AddExpr let16(new LetExpr("x",new NumExpr(5),new LetExpr("y", new NumExpr(3),new AddExpr("y", 2))),new VarExpr("x"));
        CHECK(let16.to_pretty_string() == "(_let x = 5\n"
                                          " _in  _let y = 3\n"
                                          "      _in  y + 2) + x");
    }
}

TEST_CASE("parse") {
    LetExpr letBase1("x", new NumExpr(5), new AddExpr("x", 1));
    LetExpr letBase2("x", new NumExpr(6), new MultExpr("x", 1));
    IfExpr ifBase1(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
    IfExpr ifBase2(new BoolExpr(false), new NumExpr(1), new NumExpr(2));

    SECTION("num") {
        CHECK(parse_str("123")->equals(new NumExpr(123)));
        CHECK(parse_str("-123")->equals(new NumExpr(-123)));
        CHECK(parse_str("     123")->equals(new NumExpr(123)));
        CHECK(parse_str("     123     ")->equals(new NumExpr(123)));
        CHECK_THROWS_WITH(parse_str("12x"), "bad input");
    }

    SECTION("var") {
        CHECK(parse_str("ABc")->equals(new VarExpr("ABc")));
        CHECK(parse_str("ABc   ")->equals(new VarExpr("ABc")));
        CHECK_THROWS_WITH(parse_str("abc+ "), "bad input");
        CHECK_THROWS_WITH(parse_str("abc* "), "bad input");
        CHECK_THROWS_WITH(parse_str("a2"), "invalid variable name");
        CHECK_THROWS_WITH(parse_str("x y"), "bad input");
        CHECK_THROWS_WITH(parse_str("1 y"), "bad input");
    }

    SECTION("let + mult + add") {
        // parentheses and spaces
        std::string let1a = "_let x = 5 _in x + 1";
        CHECK(parse_str(let1a)->equals(&letBase1));
        std::string let1b = "(_let x = 5 _in x + 1)";
        CHECK(parse_str(let1b)->equals(&letBase1));
        std::string let1c = "_let x = 5 _in (x + 1)";
        CHECK(parse_str(let1c)->equals(&letBase1));
        std::string let1d = "_let x = (5) _in x + 1";
        CHECK(parse_str(let1d)->equals(&letBase1));
        std::string let1e = "_let x = (5)\n"
                            "_in x + 1";
        CHECK(parse_str(let1e)->equals(&letBase1));
        std::string let1f = "_let x =                5 _in x + 1";
        CHECK(parse_str(let1f)->equals(&letBase1));
        std::string let1g = "(_let x = (5 _in x + 1))"; // wrong parentheses
        CHECK_THROWS_WITH(parse_str(let1g), "parentheses mismatch");

        std::string let2 = "(_let x = 5 _in  x) + 1";
        AddExpr let2res(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1));
        CHECK(parse_str(let2)->equals(&let2res));

        std::string let3 = "_let x = 2 _in _let x = 5 _in x + 1";
        LetExpr let3res("x", new NumExpr(2), &letBase1);
        CHECK(parse_str(let3)->equals(&let3res));

        std::string let4 = "1 * 2 * _let x = 5 _in x + 1";
        MultExpr let4res(new NumExpr(1), new MultExpr(new NumExpr(2), &letBase1));
        CHECK(parse_str(let4)->equals(&let4res));

        std::string let5 = "_let x = 5\n"
                           "_in  (_let x = 6\n"
                           "      _in  x * 1) + x";
        LetExpr let5res("x",new NumExpr(5),new AddExpr(&letBase2, new VarExpr("x")));
        CHECK(parse_str(let5)->equals(&let5res));

        // wrong format
        std::string let6a = "_let x";
        CHECK_THROWS_WITH(parse_str(let6a), "wrong format for let expression");
        std::string let6b = "_led x";
        CHECK_THROWS_WITH(parse_str(let6b), "unknown keyword: led");
        std::string let6c = "_led x = 55 _in";
        CHECK_THROWS_WITH(parse_str(let6c), "unknown keyword: led");

        // interp error
        std::string let7 = "_let x = y _in x + 1";
        CHECK_THROWS_WITH(parse_str(let7)->interp(), "A variable has no value!");

        std::string let8 = "_let x = _let x = 5\n"
                           "         _in  x + 1\n"
                           "_in  x + 6";
        LetExpr let8res("x", &letBase1, new AddExpr("x", 6));
        CHECK(parse_str(let8)->equals(&let8res));

        // this case is from my test generator!!
        std::string let9 = "(_let FPsk = _let uDiak = 316958877\n"
                           "             _in  1733846665 * P\n"
                           " _in  1735924491) * 1996497710";
        CHECK_THROWS_WITH(parse_str(let9)->interp(), "A variable has no value!");

        std::string let10 = "_let 1 = 2 _in 3";
        CHECK_THROWS_WITH(parse_str(let10), "invalid variable name");
    }

    SECTION("if + let") {
        std::string if1a = "_if _true _then 1 _else 2";
        CHECK(parse_str(if1a)->equals(&ifBase1));
        std::string if1b = "(_if _true _then 1 _else 2)";
        CHECK(parse_str(if1b)->equals(&ifBase1));
        std::string if1c = "(_if _false _then 1 _else 2)";
        CHECK(parse_str(if1c)->equals(&ifBase2));

        std::string if2 = "_if _false _then 1";
        CHECK_THROWS_WITH(parse_str(if2), "wrong format for if expression");
        std::string if3 = "(_if _true _then 1 _else 2) + 1";
        CHECK(parse_str(if3)->equals(new AddExpr(&ifBase1, 1)));
    }

    SECTION("equal") {
        std::string eq1 = "1 == 2";
        parse_str(eq1);
        CHECK(parse_str(eq1)->equals(new EqExpr(1, 2)));
        EqExpr eq2(&letBase1, new NumExpr(2));
        CHECK(eq2.to_pretty_string() == "(_let x = 5\n"
                                        " _in  x + 1) == 2");
    }
}

TEST_CASE("parse given tests") {
    // parentheses
    CHECK_THROWS_WITH( parse_str("()"), "bad input");
    CHECK( parse_str("(1)")->equals(new NumExpr(1)) );
    CHECK( parse_str("(((1)))")->equals(new NumExpr(1)) );
    CHECK_THROWS_WITH( parse_str("(1"), "parentheses mismatch" );

    // numbers
    CHECK( parse_str("1")->equals(new NumExpr(1)) );
    CHECK( parse_str("10")->equals(new NumExpr(10)) );
    CHECK( parse_str("-3")->equals(new NumExpr(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(new NumExpr(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "invalid input" );
    CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

    // variables
    CHECK( parse_str("x")->equals(new VarExpr("x")) );
    CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "invalid variable name" );

    // add & mult expressions
    CHECK( parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );
    CHECK( parse_str("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );
    CHECK( parse_str("z * x + y")->equals(new AddExpr(new MultExpr("z", "x"),new VarExpr("y"))));
    CHECK( parse_str("z * (x + y)")->equals(new MultExpr(new VarExpr("z"),new AddExpr("x", "y"))));
}

TEST_CASE("NumVal") {

    SECTION("equals") {
        CHECK(NumVal(1).equals(new NumVal(1)));
        CHECK(!NumVal(1).equals(nullptr));
        CHECK(AddExpr(2, 3).interp()->equals(new NumVal(5)));
    }

    SECTION("to_expr") {
        CHECK(NumVal(1).to_expr()->equals(new NumExpr(1)));
    }

    SECTION("add_to") {
        CHECK(NumVal(3).add_to(new NumVal(2))->equals(new NumVal(5)));
        CHECK(NumVal(3).add_to(new NumVal(2))->add_to(new NumVal(-1))->equals(new NumVal(4)));
        CHECK_THROWS_WITH(NumVal(3).add_to(nullptr), "add of non-number");
    }

    SECTION("mult_with") {
        CHECK(NumVal(3).mult_with(new NumVal(2))->equals(new NumVal(6)));
        CHECK(NumVal(3).mult_with(new NumVal(2))->mult_with(new NumVal(-1))->equals(new NumVal(-6)));
        CHECK_THROWS_WITH(NumVal(3).mult_with(nullptr), "multiply with non-number");
    }

    SECTION("to_string") {
        CHECK(NumVal(120).to_string() == "120");
    }
}

TEST_CASE("BoolVal") {

    SECTION("equals") {
        CHECK(BoolVal(true).equals(new BoolVal(true)));
        CHECK(!BoolVal(false).equals(new BoolVal(true)));
        CHECK(!BoolVal(true).equals(new NumVal(1)));
    }

    SECTION("to_expr") {
        CHECK(BoolVal(true).to_expr()->equals(new BoolExpr(true)));
        CHECK(BoolVal(false).to_expr()->equals(new BoolExpr(false)));
    }

    SECTION("add_to") {
        CHECK_THROWS_WITH(BoolVal(true).add_to(new NumVal(1)), "add of non-number");
    }

    SECTION("mult_with") {
        CHECK_THROWS_WITH(BoolVal(3).mult_with(nullptr), "multiply with non-number");
    }

    SECTION("to_string") {
        CHECK(BoolVal(true).to_string() == "_true");
        CHECK(BoolVal(false).to_string() == "_false");
    }
}

TEST_CASE("BoolExpr") {
    SECTION("equals") {
        CHECK(!BoolExpr(true).equals(new BoolExpr(false)));
        CHECK(BoolExpr(true).equals(new BoolExpr(true)));
        CHECK(!BoolExpr(true).equals(new NumExpr(3)));
    }

    SECTION("interp") {
        CHECK(BoolExpr(true).interp()->equals(new BoolVal(true)));
        CHECK(BoolExpr(false).interp()->equals(new BoolVal(false)));
        CHECK(!BoolExpr(true).interp()->equals(new BoolVal(false)));
        CHECK(!BoolExpr(true).interp()->equals(new NumVal(3)));
    }

    SECTION("has_variable") {
        CHECK(!BoolExpr(true).has_variable());
    }

    SECTION("subst") {
        CHECK(BoolExpr(true).subst("x", new NumExpr(1))->equals(new BoolExpr(true)));
    }

    SECTION("print") {
        CHECK(BoolExpr(true).to_string() == "_true");
        CHECK(BoolExpr(false).to_string() == "_false");
    }

    SECTION("pretty_print") {
        CHECK(BoolExpr(true).to_pretty_string() == "_true");
        CHECK(BoolExpr(false).to_pretty_string() == "_false");
    }
}

TEST_CASE("EqExpr") {
    EqExpr eq1(new NumExpr(1), new NumExpr(2));
    EqExpr eq2(new NumExpr(1), new NumExpr(1));
    EqExpr eq3(new VarExpr("x"), new VarExpr("x"));
    EqExpr eq4(new NumExpr(2), new AddExpr(1, 1));
    EqExpr eq5(new MultExpr(2, 1), new AddExpr(1, 1));
    EqExpr eq6(new LetExpr("x", new NumExpr(4), new AddExpr("x", 3)), new MultExpr(2, 3));

    SECTION("equals") {
        CHECK(!eq1.equals(&eq2));
        CHECK(eq1.equals(new EqExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(!eq1.equals(new NumExpr(3)));
        CHECK(!eq4.equals(&eq5));
        CHECK(!eq5.equals(&eq6));
    }

    SECTION("interp") {
        CHECK(eq1.interp()->equals(new BoolVal(false)));
        CHECK(eq2.interp()->equals(new BoolVal(true)));
        CHECK_THROWS_WITH(eq3.interp(), "A variable has no value!");
        CHECK(eq4.interp()->equals(new BoolVal(true)));
        CHECK(eq5.interp()->equals(new BoolVal(true)));
        CHECK(eq6.interp()->equals(new BoolVal(false)));
    }

    SECTION("has_variable") {
        CHECK(!eq1.has_variable());
        CHECK(!eq2.has_variable());
        CHECK(eq3.has_variable());
        CHECK(!eq4.has_variable());
        CHECK(!eq5.has_variable());
        CHECK(eq6.has_variable());
        CHECK((new EqExpr(new NumExpr(1), new VarExpr("x")))->has_variable());
    }

    SECTION("subst") {
        CHECK(eq1.subst("x", new NumExpr(2))->equals(&eq1));
    }

    SECTION("print") {
        CHECK(eq1.to_string() == "(1==2)");
        CHECK(eq2.to_string() == "(1==1)");
        CHECK(eq3.to_string() == "(x==x)");
        CHECK(eq4.to_string() == "(2==(1+1))");
        CHECK(eq5.to_string() == "((2*1)==(1+1))");
        CHECK(eq6.to_string() == "((_let x=4 _in (x+3))==(2*3))");
    }

    SECTION("pretty_print") {
        CHECK(eq1.to_pretty_string() == "1 == 2");
        CHECK(eq2.to_pretty_string() == "1 == 1");
        CHECK(eq3.to_pretty_string() == "x == x");
        CHECK(eq4.to_pretty_string() == "2 == 1 + 1");
        CHECK(eq5.to_pretty_string() == "2 * 1 == 1 + 1");
        // let / if as left arg
        CHECK(eq6.to_pretty_string() == "(_let x = 4\n"
                                        " _in  x + 3) == 2 * 3");
        IfExpr ifBase(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
        LetExpr letBase("x", new NumExpr(1), new AddExpr("x", 1));
        EqExpr eq7(&ifBase, new NumExpr(2));
        CHECK(eq7.to_pretty_string() == "(_if _true\n"
                                        " _then 1\n"
                                        " _else 2) == 2");
        EqExpr eq8(&letBase, new NumExpr(2));
        CHECK(eq8.to_pretty_string() == "(_let x = 1\n"
                                        " _in  x + 1) == 2");
        // left / if as right arg
        EqExpr eq9(new NumExpr(1), &ifBase);
        CHECK(eq9.to_pretty_string() == "1 == _if _true\n"
                                        "     _then 1\n"
                                        "     _else 2");
        EqExpr eq10(new NumExpr(2), &letBase);
        CHECK(eq10.to_pretty_string() == "2 == _let x = 1\n"
                                         "     _in  x + 1");
        AddExpr eq11(&eq9, new NumExpr(3));
        CHECK(eq11.to_pretty_string() == "(1 == _if _true\n"
                                         "      _then 1\n"
                                         "      _else 2) + 3");
        // eq as left/right arg of add
        AddExpr eq12a(&eq10, new NumExpr(3));
        CHECK(eq12a.to_pretty_string() == "(2 == _let x = 1\n"
                                          "      _in  x + 1) + 3");
        AddExpr eq12b(new NumExpr(3), &eq10);
        CHECK(eq12b.to_pretty_string() == "3 + (2 == _let x = 1\n"
                                          "          _in  x + 1)");
        AddExpr eq12c(&eq12b, new NumExpr(5));
        CHECK(eq12c.to_pretty_string() == "(3 + (2 == _let x = 1\n"
                                          "           _in  x + 1)) + 5");
        AddExpr eq12d(&eq1, 3);
        CHECK(eq12d.to_pretty_string() == "(1 == 2) + 3");
        // add as right arg of eq
        EqExpr eq13(new NumExpr(2), new AddExpr(&ifBase, new NumExpr(3)));
        CHECK(eq13.to_pretty_string() == "2 == (_if _true\n"
                                         "      _then 1\n"
                                         "      _else 2) + 3");
        EqExpr eq14(new NumExpr(2), new AddExpr(&letBase, new NumExpr(3)));
        CHECK(eq14.to_pretty_string() == "2 == (_let x = 1\n"
                                         "      _in  x + 1) + 3");
        EqExpr eq15(new NumExpr(2), new AddExpr(new NumExpr(3), &ifBase));
        CHECK(eq15.to_pretty_string() == "2 == 3 + _if _true\n"
                                         "         _then 1\n"
                                         "         _else 2");
        EqExpr eq16(new NumExpr(2), new AddExpr(new NumExpr(3), &letBase));
        CHECK(eq16.to_pretty_string() == "2 == 3 + _let x = 1\n"
                                         "         _in  x + 1");
        // consecutive ==
        EqExpr eq17(&eq4, new BoolExpr(true));
        CHECK(eq17.to_pretty_string() == "(2 == 1 + 1) == _true");
        EqExpr eq18(new BoolExpr(false), &eq4);
        CHECK(eq18.to_pretty_string() == "_false == 2 == 1 + 1");

    }
}

TEST_CASE("IfExpr") {
    IfExpr ifBase1(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
    IfExpr ifBase2(new BoolExpr(false), new NumExpr(1), new NumExpr(2));
    IfExpr ifBase3(new EqExpr(1, 2), new AddExpr(new BoolExpr(false), new NumExpr(5)), new NumExpr(88));
    IfExpr ifBase4(new EqExpr(2, 2), new AddExpr(new BoolExpr(false), new NumExpr(5)), new NumExpr(88));
    IfExpr ifBase5(new EqExpr(new VarExpr("x"), new NumExpr(3)), new NumExpr(1), new NumExpr(0));

    SECTION("equals") {
        IfExpr if1(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
        IfExpr if2(new BoolExpr(true), new NumExpr(3), new NumExpr(2));
        IfExpr if3(new BoolExpr(true), new NumExpr(1), new VarExpr("x"));
        CHECK(ifBase1.equals(&if1));
        CHECK(!ifBase1.equals(&if2));
        CHECK(!ifBase1.equals(&if3));
        CHECK(!ifBase1.equals(&ifBase2));
        CHECK(!ifBase1.equals(new NumExpr(1)));
    }

    SECTION("interp") {
        CHECK(ifBase1.interp()->equals(new NumVal(1)));
        CHECK(ifBase2.interp()->equals(new NumVal(2)));
        LetExpr if1("x", new EqExpr(1, 2), &ifBase3);
        CHECK(if1.interp()->equals(new NumVal(88)));
        LetExpr if2("x", new EqExpr(1, 2), &ifBase4);
        CHECK_THROWS_WITH(if2.interp(), "add of non-number");
        CHECK(ifBase3.interp()->equals(new NumVal(88)));
        CHECK_THROWS_WITH(ifBase4.interp(), "add of non-number");
        CHECK_THROWS_WITH(ifBase5.interp(), "A variable has no value!");
    }

    SECTION("has_variable") {
        CHECK(!ifBase1.has_variable());
        CHECK(!ifBase2.has_variable());
        CHECK(!ifBase3.has_variable());
        CHECK(!ifBase4.has_variable());
        CHECK(ifBase5.has_variable());
        IfExpr if1(new BoolExpr(true), new VarExpr("x"), new NumExpr(1));
        CHECK(if1.has_variable());
        IfExpr if2(new BoolExpr(true), new NumExpr(1), new VarExpr("x"));
        CHECK(if2.has_variable());
    }

    SECTION("subst") {
        CHECK(ifBase1.subst("x", new NumExpr(10))->equals(&ifBase1));
        IfExpr if1(new EqExpr(new NumExpr(10), new NumExpr(3)), new NumExpr(1), new NumExpr(0));
        CHECK(ifBase5.subst("x", new NumExpr(10))->equals(&if1));
    }

    SECTION("print") {
        CHECK(ifBase1.to_string() == "(_if _true _then 1 _else 2)");
        CHECK(ifBase2.to_string() == "(_if _false _then 1 _else 2)");
        CHECK(ifBase3.to_string() == "(_if (1==2) _then (_false+5) _else 88)");
        CHECK(ifBase4.to_string() == "(_if (2==2) _then (_false+5) _else 88)");
        CHECK(ifBase5.to_string() == "(_if (x==3) _then 1 _else 0)");
    }

    SECTION("pretty_print") {
        // simple if
        CHECK(ifBase1.to_pretty_string() == "_if _true\n_then 1\n_else 2");
        CHECK(ifBase2.to_pretty_string() == "_if _false\n_then 1\n_else 2");
        CHECK(ifBase3.to_pretty_string() == "_if 1 == 2\n_then _false + 5\n_else 88");
        CHECK(ifBase4.to_pretty_string() == "_if 2 == 2\n_then _false + 5\n_else 88");
        CHECK(ifBase5.to_pretty_string() == "_if x == 3\n_then 1\n_else 0");
        // if as let body (rhs is eq)
        LetExpr if1("x", new EqExpr(1, 2), &ifBase3);
        CHECK(if1.to_pretty_string() == "_let x = 1 == 2\n"
                                        "_in  _if 1 == 2\n"
                                        "     _then _false + 5\n"
                                        "     _else 88");
        CHECK(parse_str(if1.to_string())->equals(&if1));
        // if as let rhs
        LetExpr if2("x", &ifBase1, new AddExpr("x", 1));
        CHECK(if2.to_pretty_string() == "_let x = _if _true\n"
                                        "         _then 1\n"
                                        "         _else 2\n"
                                        "_in  x + 1");
        CHECK(parse_str(if2.to_string())->equals(&if2));
        // if as left arg of mult in let rhs
        LetExpr if3("x", new MultExpr(&ifBase1, new NumExpr(3)), new AddExpr("x", 1));
        CHECK(if3.to_pretty_string() == "_let x = (_if _true\n"
                                        "          _then 1\n"
                                        "          _else 2) * 3\n"
                                        "_in  x + 1");
        CHECK(parse_str(if3.to_string())->equals(&if3));
        // if as left arg of eq in let rhs
        LetExpr if4("x", new EqExpr(&ifBase1, new NumExpr(3)), new AddExpr("x", 1));
        CHECK(if4.to_pretty_string() == "_let x = (_if _true\n"
                                        "          _then 1\n"
                                        "          _else 2) == 3\n"
                                        "_in  x + 1");
        CHECK(parse_str(if4.to_string())->equals(&if4));
        // if as left arg of add in let rhs
        LetExpr if5("x", new AddExpr(&ifBase1, new NumExpr(3)), new AddExpr("x", 1));
        CHECK(if5.to_pretty_string() == "_let x = (_if _true\n"
                                        "          _then 1\n"
                                        "          _else 2) + 3\n"
                                        "_in  x + 1");
        CHECK(parse_str(if5.to_string())->equals(&if5));
        // if as let body
        LetExpr if6("x", new NumExpr(1), new AddExpr(&ifBase1, new VarExpr("x")));
        CHECK(if6.to_pretty_string() == "_let x = 1\n"
                                        "_in  (_if _true\n"
                                        "      _then 1\n"
                                        "      _else 2) + x");
        CHECK(if6.interp()->equals(new NumVal(2)));
        CHECK(parse_str(if6.to_string())->equals(&if6));
        AddExpr if7(&if6, new NumExpr(1));
        CHECK(if7.to_pretty_string() == "(_let x = 1\n"
                                        " _in  (_if _true\n"
                                        "       _then 1\n"
                                        "       _else 2) + x) + 1");
        CHECK(parse_str(if7.to_string())->equals(&if7));
        // one nested if
        IfExpr if8(new BoolExpr(true), &ifBase2, new NumExpr(3));
        CHECK(if8.to_pretty_string() == "_if _true\n"
                                        "_then _if _false\n"
                                        "      _then 1\n"
                                        "      _else 2\n"
                                        "_else 3");
        CHECK(if8.interp()->equals(new NumVal(2)));
        CHECK(parse_str(if8.to_string())->equals(&if8));
        // if as right arg of mult
        MultExpr if9(3, &ifBase1);
        CHECK(if9.to_pretty_string() == "3 * _if _true\n"
                                        "    _then 1\n"
                                        "    _else 2");
        CHECK(parse_str(if9.to_string())->equals(&if9));
        MultExpr if10(&if9, 4);
        CHECK(if10.to_pretty_string() == "(3 * _if _true\n"
                                         "     _then 1\n"
                                         "     _else 2) * 4");
        CHECK(parse_str(if10.to_string())->equals(&if10));
        AddExpr if11(1, &if9);
        CHECK(if11.to_pretty_string() == "1 + 3 * _if _true\n"
                                         "        _then 1\n"
                                         "        _else 2");
        CHECK(parse_str(if11.to_string())->equals(&if11));
        MultExpr if12(1, &if9);
        CHECK(if12.to_pretty_string() == "1 * 3 * _if _true\n"
                                         "        _then 1\n"
                                         "        _else 2");
        CHECK(parse_str(if12.to_string())->equals(&if12));
        AddExpr if13(&if9, 5);
        CHECK(if13.to_pretty_string() == "3 * (_if _true\n"
                                         "     _then 1\n"
                                         "     _else 2) + 5");
        CHECK(parse_str(if13.to_string())->equals(&if13));
        AddExpr if14(&if12, 5);
        CHECK(if14.to_pretty_string() == "1 * 3 * (_if _true\n"
                                         "         _then 1\n"
                                         "         _else 2) + 5");
        CHECK(parse_str(if14.to_string())->equals(&if14));
    }
}