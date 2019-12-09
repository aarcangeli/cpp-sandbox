#define EXP __declspec(dllexport)

const int ret = 0;
const int *ret1;
const int **ret2;

/**
 *
 * 'YA' (pointer_type)* ('?B')? (tipo)? 'XZ'
 *
 * ?B: const type
 *
 * pointer_type
 * AA: reference
 * PA: pointer
 * PB: const pointer
 * QA: const value
 * QB: const pointer value
 *
 * tipo:
 * X: void
 * _N: bool
 * D: char
 * _W: wchar_t
 * _S: char16_t
 * _U: char32_t
 * E: unsigned char
 * F: short
 * G: unsigned short
 * H: int
 * I: unsigned int (e unsigned float)
 * J: long
 * K: unsigned long
 * _J: long long
 * _K: unsigned long long
 * M: float
 * N: double
 * O: long double
 * '?AT' name '@@': a union
 * '?AU' name '@@': a struct
 * '?AV' name '@@': a class
 * '?AW' name '@@': a enum
 */

void (*ciao)();

int a = sizeof(int *[34]);

using palla = int(*)();

// ?ciao3@@YAPAPAHXZ int**
// ?ciao3@@YA?AUpalla@@XZ
// ?ciao3@@YAP6AHXZXZ
EXP palla ciao3(){return {};}
