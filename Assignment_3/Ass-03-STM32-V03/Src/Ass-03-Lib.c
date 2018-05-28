//     $Date: 2018-05-16 16:40:22 +1000 (Wed, 16 May 2018) $
// $Revision: 1321 $
//   $Author: Peter $

#include "Ass-03.h"

//
// PLACE YOUR COMMON CODE HERE
//

// clang-format off
const command_s Command_list[] = {
    {"analog",  &analog_function,  "analog <time>",				"Time interval on pulse monitor"},
    {"ls",      &ls_function,      "ls <path>",					"List the contents of the current folder"},
    {"cd",      &cd_function,      "cd <path>",					"Change directory"},
    {"mkdir",   &mkdir_function,   "mkdir <path/dirname>",		"Make directory"},
    {"cp",      &cp_function,      "cp <source> <destination>",	"Copy from source to destination"},
    {"rm",      &rm_function,      "rm <path/dirname>",			"Delete folder"},
    {"expr",    &expr_function,    "expr <formula>",			"Solve formula entered"},
    {"debug",   &debug_function,   "debug <on|off>",            "Display debug messages"},
    {"system",  &system_function,  "system <on|off>",           "Display system messages"},
    {"clear",   &clear_function,   "reset",                     "Clear terminal screen"},
    {"reset",   &reset_function,   "clear",                     "Reset terminal scrollback"},
    {"help",    &help_function,    "help <command>",            "Display help messages"},
    {NULL,      NULL,              NULL,                        NULL}};

// clang-format on

// If a command is called function decides which function pointer to return
int command_parser(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    double result = 0;
    int i         = 0;

    // While we haven't checked the whole list
    while (Command_list[i].NameString != NULL) {
        // If we find the function we want, call it
        if (strcmp((*array_of_words_p)[0], Command_list[i].NameString) == 0) {
            debug_printf("Operation: %s\n", Command_list[i].NameString);
            // If the function we wanted returned success
            if (Command_list[i].Function_p(array_of_words_p, word_count, path_p, path_count) == 0) {
                Set_Result(result);
                if (i <= 14)
                    return 0;
                else
                    return 1;
            }
            else {
                return -1;
            }
        }
        i++;
    }
    return -1;
}

// If help is called
// Function decides if specific help is to be diplayed or all
int help_parser(char** array_of_words_p[], int word_count) {
    int i = 0;

    printf("%s" HELP_M
           "%s"
           "\n",
           KGRN,
           "Format",
           "Description",
           KNRM);
    if (word_count > 1) {
        // While we haven't checked the whole list
        while (Command_list[i].NameString != NULL) {
            // If we find the function we wanted, display the help message
            if (strcmp((*array_of_words_p)[1], Command_list[i].NameString) == 0) {
                debug_printf("Operation: %s\n", Command_list[i].NameString);
                // Print Specific command list thing
                printf(HELP_M "\n", Command_list[i].HelpString, Command_list[i].DescriptionString);
                return 0;
            }
            i++;
        }
    }
    else if (word_count == 1) {
        debug_printf("No Operation selected\n");
        // Loop through all command list and print
        while (Command_list[i].NameString != NULL) {
            printf(HELP_M "\n", Command_list[i].HelpString, Command_list[i].DescriptionString);
            i++;
        }
        return 0;
    }
    return 1;
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
    system_printf("Formula Parsed: %s\n", output.formula);
    output.result = parseSub();
    if (*output.formula == '\0') {
        // return output.result;
        DEBUG_P
        return 0;
    }
    printf("Expected end of output.input but found %c\n", *output.formula);
    printf("Syntax Error\n");
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
    else {
        printf("Syntax Error\n");
        printf("Unknown symbol %c\n", *output.formula);
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
        printf("Syntax Error\n");
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
            printf("Syntax Error\n");
        }
    }
    system_printf("Number Found: %lf\n", (number * neg_flag));
    return (number * neg_flag);
}
