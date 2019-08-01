#include <cstdio>
#include <functional>

void lambdaProblem() {
  int param = 10;
  std::function<void()> fn;

  printf("begin lambdaProblem\n");

  fn = [&]() {
    printf("1) param[0x%X] = %i\n", &param, param);

    // what if I replace a function instance inside his execution?
    fn = []() { printf("==replaced==\n"); };

    // captured parameters are now undefined
    printf("2) param[0x%X] = %i\n", &param, param);
  };

  fn();
  fn();

  printf("end lambdaProblem\n\n");
}

void solution1() {
  int param = 10;
  std::function<void()> fn;

  printf("begin solution1\n");
  fn = [&]() {
    // solution 1, make a copy of parameters you want to save
    int paramL = param;
    printf("1) param[0x%X] = %i\n", &paramL, paramL);

    // what if I replace a function instance inside his execution?
    fn = []() { printf("==replaced==\n"); };

    // captured parameters are now undefined
    printf("2) param[0x%X] = %i\n", &paramL, paramL);
  };

  fn();
  fn();

  printf("end solution1\n\n");
}

int main() {
  lambdaProblem();
  solution1();
}
