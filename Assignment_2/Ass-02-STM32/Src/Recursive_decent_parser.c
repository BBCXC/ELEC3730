#include "Ass-02.h"

double parseFormula(){
  grid_space_p.result = parseSub();
  if(*grid_space_p.input == '\0'){
    return grid_space_p.result;
  }
  // printf("Expected end of grid_space_p.input but found %c\n", *grid_space_p.input);
  printf("Syntax Error\n");
  return 0;
}

double parseSub(){
  double sub_1 = parseSum();
  while(*grid_space_p.input == '-'){
    ++grid_space_p.input;
    double sub_2 = parseSum();
    sub_1 = sub_1 - sub_2;
  }
  return sub_1;
}

double parseSum(){
  double sum_1 = parsePro();
  while(*grid_space_p.input == '+'){
    ++grid_space_p.input;
    double sum_2 = parsePro();
    sum_1 = sum_1 + sum_2;
  }
  return sum_1;
}

double parsePro(){
  double pro_1 = parseDiv();
  while(*grid_space_p.input == '*'){
    ++grid_space_p.input;
    double pro_2 = parseDiv();
    pro_1 = pro_1 * pro_2;
  }
  return pro_1;
}

double parseDiv(){
  double div_1 = parsePow();
  while(*grid_space_p.input == '/'){
    ++grid_space_p.input;
    double div_2 = parsePow();
    div_1 = div_1 / div_2;
  }
  return div_1;
}

double parsePow(){
  double pow_1 = parseFactor();
  while(*grid_space_p.input == '^'){
    ++grid_space_p.input;
    double pow_2 = parseFactor();
    pow_1 = pow(pow_1, pow_2);
  }
  return pow_1;
}

double parseFactor(){

  if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
    return parseNumber();
  }
  else if(*grid_space_p.input == '-'){
    return parseNumber();
  }
  else if(*grid_space_p.input == '('){
    ++grid_space_p.input;
    double temp = parseSub();
    ++grid_space_p.input;
    return temp;
  }
  //PI
  else if(*grid_space_p.input == 'p'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'i'){
      ++grid_space_p.input;
      return M_PI;
    }
  }
  //sin sqrt
  else if(*grid_space_p.input == 's'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'i'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'n'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = sin(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
        }

      }
    }
    else if(*grid_space_p.input == 'q'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'r'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 't'){
            ++grid_space_p.input;
            if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = sqrt(temp);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
  }
  //cos
  else if(*grid_space_p.input == 'c'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'o'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 's'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = cos(temp*M_PI/180);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //tan
  else if(*grid_space_p.input == 't'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'a'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'n'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = tan(temp*M_PI/180);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //asin acos atan
  else if(*grid_space_p.input == 'a'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 's'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'i'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 'n'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = asin(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
    else if(*grid_space_p.input == 'c'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'o'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 's'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = acos(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
    else if(*grid_space_p.input == 't'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'a'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 'n'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = atan(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
  }
  //exp
  else if(*grid_space_p.input == 'e'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'x'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'p'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = exp(temp);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //ln log10
  else if(*grid_space_p.input == 'l'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'n'){
      ++grid_space_p.input;
      if(*grid_space_p.input == '('){
        ++grid_space_p.input;
        double temp = parseSub();
        temp = log(temp);
        ++grid_space_p.input;
        return temp;
      }
    }
    else if(*grid_space_p.input == 'o'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'g'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = log10(temp);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  else{
    printf("Syntax Error\n");
    printf("Unknown symbol %c", *grid_space_p.input);
  }
  return 0;
}

double parseNumber(){

  double number = 0;
  int neg_flag = 1;
  //TODO check the first character is a number or a minus
  if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
  }
  else if(*grid_space_p.input == '-'){
    neg_flag = -1;
    ++grid_space_p.input;
  }
  else{
    printf("Syntax Error");
  }


  while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
    number = number * 10;
    number = number + (int)(*grid_space_p.input - '0');
    ++grid_space_p.input;
  }

  if(*grid_space_p.input == '.'){
    ++grid_space_p.input;

    //Check the next character is a number, else error

    if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
      double weight = 1;
      while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
        weight = weight / 10.0;
        double scaled = (int)(*grid_space_p.input - '0') * weight;
        number = number + scaled;
        ++grid_space_p.input;
      }
    }
    else{
      printf("Syntax Error");
    }
  }

  return (number * neg_flag);
}
