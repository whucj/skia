/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "Test.h"
#include "SkArenaAlloc.h"

namespace {

    static int created, destroyed;

    struct Foo {
        Foo() : x(-2), y(-3.0f) { created++; }
        Foo(int X, float Y) : x(X), y(Y) { created++; }
        ~Foo() { destroyed++; }

        int x;
        float y;
    };

    struct Big {
        Big() {}
        uint32_t array[128];
    };

}

struct WithDtor {
    ~WithDtor() { }
};

DEF_TEST(ArenaAlloc, r) {

    {
        created = 0;
        destroyed = 0;

        SkArenaAlloc arena{nullptr, 0};
        REPORTER_ASSERT(r, *arena.make<int>(3) == 3);
        Foo* foo = arena.make<Foo>(3, 4.0f);
        REPORTER_ASSERT(r, foo->x == 3);
        REPORTER_ASSERT(r, foo->y == 4.0f);
        REPORTER_ASSERT(r, created == 1);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.makeArrayDefault<int>(10);
        int* zeroed = arena.makeArray<int>(10);
        for (int i = 0; i < 10; i++) {
            REPORTER_ASSERT(r, zeroed[i] == 0);
        }
        Foo* fooArray = arena.makeArrayDefault<Foo>(10);
        REPORTER_ASSERT(r, fooArray[3].x == -2);
        REPORTER_ASSERT(r, fooArray[4].y == -3.0f);
        REPORTER_ASSERT(r, created == 11);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.make<typename std::aligned_storage<10,8>::type>();
    }
    REPORTER_ASSERT(r, created == 11);
    REPORTER_ASSERT(r, destroyed == 11);

    {
        created = 0;
        destroyed = 0;
        char block[1024];
        SkArenaAlloc arena{block};

        REPORTER_ASSERT(r, *arena.make<int>(3) == 3);
        Foo* foo = arena.make<Foo>(3, 4.0f);
        REPORTER_ASSERT(r, foo->x == 3);
        REPORTER_ASSERT(r, foo->y == 4.0f);
        REPORTER_ASSERT(r, created == 1);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.makeArrayDefault<int>(10);
        int* zeroed = arena.makeArray<int>(10);
        for (int i = 0; i < 10; i++) {
            REPORTER_ASSERT(r, zeroed[i] == 0);
        }
        Foo* fooArray = arena.makeArrayDefault<Foo>(10);
        REPORTER_ASSERT(r, fooArray[3].x == -2);
        REPORTER_ASSERT(r, fooArray[4].y == -3.0f);
        REPORTER_ASSERT(r, created == 11);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.make<typename std::aligned_storage<10,8>::type>();
    }
    REPORTER_ASSERT(r, created == 11);
    REPORTER_ASSERT(r, destroyed == 11);

    {
        created = 0;
        destroyed = 0;
        std::unique_ptr<char[]> block{new char[1024]};
        SkArenaAlloc arena{block.get(), 1024};

        REPORTER_ASSERT(r, *arena.make<int>(3) == 3);
        Foo* foo = arena.make<Foo>(3, 4.0f);
        REPORTER_ASSERT(r, foo->x == 3);
        REPORTER_ASSERT(r, foo->y == 4.0f);
        REPORTER_ASSERT(r, created == 1);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.makeArrayDefault<int>(10);
        int* zeroed = arena.makeArray<int>(10);
        for (int i = 0; i < 10; i++) {
            REPORTER_ASSERT(r, zeroed[i] == 0);
        }
        Foo* fooArray = arena.makeArrayDefault<Foo>(10);
        REPORTER_ASSERT(r, fooArray[3].x == -2);
        REPORTER_ASSERT(r, fooArray[4].y == -3.0f);
        REPORTER_ASSERT(r, created == 11);
        REPORTER_ASSERT(r, destroyed == 0);
        arena.make<typename std::aligned_storage<10,8>::type>();
    }
    REPORTER_ASSERT(r, created == 11);
    REPORTER_ASSERT(r, destroyed == 11);
}
