#include "Ass-03.h"


int element = 0;

typedef struct {
    char* formula;
    int pos;
    double result;
    double prev_ans;
} formula;
formula graph;

typedef struct {
    char* formula;
    int pos;
    double result;
    double prev_ans;
} result_mem;
result_mem output;

int Simulated_DMA() {
    if (SIM_FLAG == 4) {
        // DMA has got new data
        // Process my DMA
        return 1;
        SIM_FLAG = 0;
    }
    return 0;
}

void Simulated_data() {
    // \sin\left(\sin\left(x\right)\cdot0.1x\right)

    // Generate 2000 points of data to spam to the actual code every 40 samples


    const char* const_formula = "sin(sin(X)*0.1*X+2*X)";

    // Store the string
    static char graph_formula[50];

    // Loop through 2000 samples of the data and every 40 samples
    for (int i = 0; i < 2000; i++) {
        strcpy(graph_formula, const_formula);
        graph.formula = graph_formula;
        if (parseFormula() == 0) {
            simulated_data[i] = Map_Data(output.result);
        }
    }
    // now i have 2000 points of simulated data
}

int Map_Data(double Input) {
    int min = -1;
    int max = 1;
    return ((Input - min) / (max - min) * (1023 - 0) + 0);
}

// clang-format off
/**********************************************************************************************************************
************************************************Recursive Desent Parser************************************************
**********************************************************************************************************************/
// clang-format on

// Takes formula string
// Calls next precedent
// Returns answer at end
double parseFormula() {
    if (1) safe_printf("%sSYSTEM_INFO:%s Formula Parsed: %s\n", SYS_M, DEFAULT_COLOUR_M, output.formula);
    output.result = parseSub();
    if (*output.formula == '\0') {
        // return output.result;
        return 0;
    }
    safe_printf("Expected end of output.input but found %c\n", *output.formula);
    safe_printf("Syntax Error\n");
    return 1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseSub() {
    double sub_1 = parseSum();
    while (*output.formula == '-') {
        ++output.formula;
        double sub_2 = parseSum();
        sub_1        = sub_1 - sub_2;
    }
    return sub_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseSum() {
    double sum_1 = parsePro();
    while (*output.formula == '+') {
        ++output.formula;
        double sum_2 = parsePro();
        sum_1        = sum_1 + sum_2;
    }
    return sum_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parsePro() {
    double pro_1 = parseDiv();
    while (*output.formula == 'x' || *output.formula == '*') {
        ++output.formula;
        double pro_2 = parseDiv();
        pro_1        = pro_1 * pro_2;
    }
    return pro_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseDiv() {
    double div_1 = parsePow();
    while (*output.formula == '/') {
        ++output.formula;
        double div_2 = parsePow();
        div_1        = div_1 / div_2;
    }
    return div_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parsePow() {
    double pow_1 = parseFactor();
    while (*output.formula == '^') {
        ++output.formula;
        double pow_2 = parseFactor();
        pow_1        = pow(pow_1, pow_2);
    }
    return pow_1;
}

// Decides whether next factor is a
// Number
// Word operation
// Bracket
double parseFactor() {
    if (*output.formula >= '0' && *output.formula <= '9') {
        return parseNumber();
    }
    else if (*output.formula == '-') {
        return parseNumber();
    }
    else if (*output.formula == '+') {
        return parseNumber();
    }
    else if (*output.formula == '(') {
        ++output.formula;
        double temp = parseSub();
        ++output.formula;
        return temp;
    }
    // PI
    else if (*output.formula == 'p') {
        ++output.formula;
        if (*output.formula == 'i') {
            ++output.formula;
            return M_PI;
        }
    }
    // sin sqrt
    else if (*output.formula == 's') {
        ++output.formula;
        if (*output.formula == 'i') {
            ++output.formula;
            if (*output.formula == 'n') {
                ++output.formula;
                if (*output.formula == '(') {
                    ++output.formula;
                    double temp = parseSub();
                    if (Get_Angle_Mode() == 1) {
                        temp = sin(temp);
                    }
                    else {
                        temp = sin(temp * M_PI / 180);
                    }
                    ++output.formula;
                    return temp;
                }
            }
        }
        else if (*output.formula == 'q') {
            ++output.formula;
            if (*output.formula == 'r') {
                ++output.formula;
                if (*output.formula == 't') {
                    ++output.formula;
                    if (*output.formula == '(') {
                        ++output.formula;
                        double temp = parseSub();
                        temp        = sqrt(temp);
                        ++output.formula;
                        return temp;
                    }
                }
            }
        }
    }
    // cos
    else if (*output.formula == 'c') {
        ++output.formula;
        if (*output.formula == 'o') {
            ++output.formula;
            if (*output.formula == 's') {
                ++output.formula;
                if (*output.formula == '(') {
                    ++output.formula;
                    double temp = parseSub();
                    if (Get_Angle_Mode() == 1) {
                        temp = cos(temp);
                    }
                    else {
                        temp = cos(temp * M_PI / 180);
                    }
                    ++output.formula;
                    return temp;
                }
            }
        }
    }
    // tan
    else if (*output.formula == 't') {
        ++output.formula;
        if (*output.formula == 'a') {
            ++output.formula;
            if (*output.formula == 'n') {
                ++output.formula;
                if (*output.formula == '(') {
                    ++output.formula;
                    double temp = parseSub();
                    if (Get_Angle_Mode() == 1) {
                        temp = tan(temp);
                    }
                    else {
                        temp = tan(temp * M_PI / 180);
                    }
                    ++output.formula;
                    return temp;
                }
            }
        }
    }
    // asin acos atan
    else if (*output.formula == 'a') {
        ++output.formula;
        if (*output.formula == 's') {
            ++output.formula;
            if (*output.formula == 'i') {
                ++output.formula;
                if (*output.formula == 'n') {
                    ++output.formula;
                    if (*output.formula == '(') {
                        ++output.formula;
                        double temp = parseSub();
                        if (Get_Angle_Mode() == 1) {
                            temp = asin(temp);
                        }
                        else {
                            temp = asin(temp * M_PI / 180);
                        }
                        ++output.formula;
                        return temp;
                    }
                }
            }
        }
        else if (*output.formula == 'c') {
            ++output.formula;
            if (*output.formula == 'o') {
                ++output.formula;
                if (*output.formula == 's') {
                    ++output.formula;
                    if (*output.formula == '(') {
                        ++output.formula;
                        double temp = parseSub();
                        if (Get_Angle_Mode() == 1) {
                            temp = acos(temp);
                        }
                        else {
                            temp = acos(temp * M_PI / 180);
                        }
                        ++output.formula;
                        return temp;
                    }
                }
            }
        }
        else if (*output.formula == 't') {
            ++output.formula;
            if (*output.formula == 'a') {
                ++output.formula;
                if (*output.formula == 'n') {
                    ++output.formula;
                    if (*output.formula == '(') {
                        ++output.formula;
                        double temp = parseSub();
                        if (Get_Angle_Mode() == 1) {
                            temp = atan(temp);
                        }
                        else {
                            temp = atan(temp * M_PI / 180);
                        }
                        ++output.formula;
                        return temp;
                    }
                }
            }
        }
        else if (*output.formula == 'n') {
            ++output.formula;
            if (*output.formula == 's') {
                ++output.formula;
                double temp = output.prev_ans;
                return temp;
            }
        }
    }
    // exp
    else if (*output.formula == 'e') {
        ++output.formula;
        if (*output.formula == 'x') {
            ++output.formula;
            if (*output.formula == 'p') {
                ++output.formula;
                if (*output.formula == '(') {
                    ++output.formula;
                    double temp = parseSub();
                    temp        = exp(temp);
                    ++output.formula;
                    return temp;
                }
            }
        }
    }
    // ln log10
    else if (*output.formula == 'l') {
        ++output.formula;
        if (*output.formula == 'n') {
            ++output.formula;
            if (*output.formula == '(') {
                ++output.formula;
                double temp = parseSub();
                temp        = log(temp);
                ++output.formula;
                return temp;
            }
        }
        else if (*output.formula == 'o') {
            ++output.formula;
            if (*output.formula == 'g') {
                ++output.formula;
                if (*output.formula == '(') {
                    ++output.formula;
                    double temp = parseSub();
                    temp        = log10(temp);
                    ++output.formula;
                    return temp;
                }
            }
        }
    }
    else if (Get_Graph_Mode() == 1) {
        if (*output.formula == 'X') {
            ++output.formula;
            return (Get_Graph_Increment());
        }
    }
    else {
        safe_printf("Syntax Error\n");
        safe_printf("Unknown symbol %c\n", *output.formula);
    }
    return 0;
}

// If the current factor is a number
// Decide if it is nugative or positive
// Find any decimals
// Return number as double
double parseNumber() {
    double number = 0;
    int neg_flag  = 1;
    if (*output.formula >= '0' && *output.formula <= '9') {
    }
    else if (*output.formula == '-') {
        neg_flag = -1;
        ++output.formula;
    }
    else if (*output.formula == '+') {
        neg_flag = 1;
        ++output.formula;
    }
    else {
        safe_printf("Syntax Error\n");
    }

    while (*output.formula >= '0' && *output.formula <= '9') {
        number = number * 10;
        number = number + (int) (*output.formula - '0');
        ++output.formula;
    }

    if (*output.formula == '.') {
        ++output.formula;

        // Check the next character is a number, else error

        if (*output.formula >= '0' && *output.formula <= '9') {
            double weight = 1;
            while (*output.formula >= '0' && *output.formula <= '9') {
                weight        = weight / 10.0;
                double scaled = (int) (*output.formula - '0') * weight;
                number        = number + scaled;
                ++output.formula;
            }
        }
        else {
            safe_printf("Syntax Error\n");
        }
    }
    if (1) safe_printf("%sSYSTEM_INFO:%s Number Found: %lf\n", SYS_M, DEFAULT_COLOUR_M, (number * neg_flag));
    return (number * neg_flag);
}
