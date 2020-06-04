
template<typename type>
struct Tmpl_T {
};

template<int num>
struct Tmpl_Int {
};

const unsigned int NUM = 2;

// this is invalid
//Tmpl_Int<NUM >> NUM> g_var1;

// this is also invalid
//Tmpl_T<Tmpl_Int<NUM >> NUM>> g_var2;

// but these are ok
Tmpl_T<Tmpl_Int<(NUM >> NUM)>> g_var3;
Tmpl_Int<(NUM >> NUM)> g_var4;

int main() {
}
