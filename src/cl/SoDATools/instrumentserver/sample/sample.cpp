#include <iostream>

void a();
void b();
void c();
void d();
void e();

void a()
{
    std::cout << "a() called" << std::endl;
    e();
}

void b()
{
    std::cout << "b() called" << std::endl;
}

void c()
{
    std::cout << "c() called" << std::endl;
    b();
}

void d()
{
    std::cout << "d() called" << std::endl;
}

void e()
{
    std::cout << "e() called" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        a();
        c();
    } else {
        d();
    }
}