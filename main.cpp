#include <iostream>
#include "Parser.h"
#include "thread_pool.h"

int main() {
    Parser parser;
    parser.parse("../assets/kaggle/");
    return 0;
}
