// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "object.hpp"

TEST_CASE("Test String Hash Key") {
    auto hello1 = std::make_shared<String>(String{"Hello World"});
    auto hello2 = std::make_shared<String>(String{"Hello World"});
    auto diff1 = std::make_shared<String>(String{"My name is Johnny"});
    auto diff2 = std::make_shared<String>(String{"My name is Johnny"});

    if (hello1->hash_key() != hello2->hash_key()) {
        std::cerr << "strings with same content have different hash keys" << std::endl;
    }
    REQUIRE(hello1->hash_key() == hello2->hash_key());

    if (diff1->hash_key() != diff2->hash_key()) {
        std::cerr << "strings with same content have different hash keys" << std::endl;
    }
    REQUIRE(diff1->hash_key() == diff2->hash_key());

    if (hello1->hash_key() == diff1->hash_key()) {
        std::cerr << "strings with different content have same hash keys" << std::endl;
    }
    REQUIRE(hello1->hash_key() != diff1->hash_key());
}

TEST_CASE("Test Boolean Hash Key") {
    auto true1 = std::make_shared<Boolean>(Boolean{true});
    auto true2 = std::make_shared<Boolean>(Boolean{true});
    auto false1 = std::make_shared<Boolean>(Boolean{false});
    auto false2 = std::make_shared<Boolean>(Boolean{false});

    if (true1->hash_key() != true2->hash_key()) {
        std::cerr << "trues do not have same hash key" << std::endl;
    }
    REQUIRE(true1->hash_key() == true2->hash_key());

    if (false1->hash_key() != false2->hash_key()) {
        std::cerr << "falses do not have same hash key" << std::endl;
    }
    REQUIRE(false1->hash_key() == false2->hash_key());

    if (true1->hash_key() == false1->hash_key()) {
        std::cerr << "true has same hash key as false" << std::endl;
    }
    REQUIRE(true1->hash_key() != false1->hash_key());
}

TEST_CASE("Test Integer Hash Key") {
    auto one1 = std::make_shared<Integer>(Integer{1});
    auto one2 = std::make_shared<Integer>(Integer{1});
    auto two1 = std::make_shared<Integer>(Integer{2});
    auto two2 = std::make_shared<Integer>(Integer{2});

    if (one1->hash_key() != one2->hash_key()) {
        std::cerr << "integers with same content have different hash keys" << std::endl;
    }
    REQUIRE(one1->hash_key() == one2->hash_key());

    if (two1->hash_key() != two2->hash_key()) {
        std::cerr << "integers with same content have different hash keys" << std::endl;
    }
    REQUIRE(two1->hash_key() == two2->hash_key());

    if (one1->hash_key() == two1->hash_key()) {
        std::cerr << "integers with different content have same hash keys" << std::endl;
    }
    REQUIRE(one1->hash_key() != two1->hash_key());
}
