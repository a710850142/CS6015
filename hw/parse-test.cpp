#include "catch.h"
#include "expr.h"
#include "parse.h"
#include <sstream>


TEST_CASE("parse") {
    CHECK_THROWS_WITH(parse_str("()"), "bad input");

    CHECK(parse_str("(1)")->equals(new Num(1)));
    CHECK(parse_str("(((1)))")->equals(new Num(1)));

    CHECK_THROWS_WITH(parse_str("(1"), "bad input");

    CHECK(parse_str("1")->equals(new Num(1)));
    CHECK(parse_str("10")->equals(new Num(10)));
    CHECK(parse_str("-3")->equals(new Num(-3)));
    CHECK(parse_str("  \n 5  ")->equals(new Num(5)));
    CHECK_THROWS_WITH(parse_str("-"), "invalid input");

// This was some temporary debugging code:
//  std::istringstream in("-");
//  parse_num(in)->print(std::cout); std::cout << "\n";

    CHECK_THROWS_WITH(parse_str(" -   5  "), "invalid input");

    CHECK(parse_str("x")->equals(new VarExpr("x")));
    CHECK(parse_str("xyz")->equals(new VarExpr("xyz")));
    CHECK(parse_str("xYz")->equals(new VarExpr("xYz")));
    CHECK_THROWS_WITH(parse_str("x_z"), "invalid input");

    CHECK(parse_str("x + y")->equals(new Add(new VarExpr("x"), new VarExpr("y"))));

    CHECK(parse_str("x * y")->equals(new Mult(new VarExpr("x"), new VarExpr("y"))));

    CHECK(parse_str("z * x + y")
                  ->equals(new Add(new Mult(new VarExpr("z"), new VarExpr("x")),
                                   new VarExpr("y"))));

    CHECK(parse_str("z * (x + y)")
                  ->equals(new Mult(new VarExpr("z"),
                                    new Add(new VarExpr("x"), new VarExpr("y")))));

}

