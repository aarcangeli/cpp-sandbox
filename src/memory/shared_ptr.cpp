#include <iostream>
#include <memory>
#include <cassert>

class TestClass {
    char i = 0;

    // this will force a complete class
    int padding[500];

public:
    TestClass(char i) : i(i) {
        printf("  # TestClass::TestClass(%c)\n", i);
    }

    ~TestClass() {
        printf("  # %c deleted\n", i);
    }

    void foo() {
        assert(this);
        printf("  # %c->foo();\n", i);
    }
};

void shared_ptr_standard() {
    printf("\nshared_ptr_standard():\n");

    printf("  - creating a\n");
    TestClass *a = new TestClass{'a'};

    {
        printf("  - creating shared from a\n");
        std::shared_ptr<TestClass> shared{a};

        printf("  - deleting shared\n");
        // a is deleted too
    }
}

void shared_ptr_multi() {
    printf("\nshared_ptr_multi():\n");

    printf("  - creating b\n");
    TestClass *b = new TestClass{'b'};
    {
        printf("  - creating shared from b\n");
        std::shared_ptr<TestClass> shared{b};
        printf("  - shared.use_count() = %li\n", shared.use_count());

        {
            printf("  - creating shared2 from b\n");
            std::shared_ptr<TestClass> shared2 = shared;
            printf("  - shared.use_count() = %li\n", shared.use_count());
            printf("  - shared2.use_count() = %li\n", shared2.use_count());

            printf("  - deleting shared2\n");
            // b is NOT deleted
        }

        printf("  - shared.use_count() = %li\n", shared.use_count());

        printf("  - deleting shared\n");
        // b is now deleted
    }
}

void shared_ptr_and_unique_ptr() {
    printf("\nshared_ptr_and_unique_ptr():\n");

    printf("  - creating c\n");
    TestClass *c = new TestClass{'c'};

    printf("  - creating unique from c\n");
    std::unique_ptr<TestClass> unique{c};

    // a shared pointer
    printf("  - creating shared from unique with std::move\n");
    std::shared_ptr<TestClass> shared{std::move(unique)};

    // unique is now a void pointer, this should crash
    // unique->foo();

    printf("  - deleting shared\n");
}

void shared_ptr_and_heap() {
    printf("\nshared_ptr_and_heap():\n");

    printf("  - creating b\n");
    TestClass *c = new TestClass{'c'};

    printf("  - creating shared from c\n");
    std::shared_ptr<TestClass> *shared = new std::shared_ptr<TestClass>(c);

    printf("  - creating shared2 from c\n");
    std::shared_ptr<TestClass> *shared2 = new std::shared_ptr<TestClass>(*shared);

    printf("  - deleting shared\n");
    delete shared;

    printf("  - deleting shared2\n");
    delete shared2;
    // c is now deleted
}

int main() {
    shared_ptr_standard();
    shared_ptr_multi();
    shared_ptr_and_unique_ptr();
    shared_ptr_and_heap();

    return 0;
}
