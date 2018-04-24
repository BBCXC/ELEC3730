#include "Ass-02.h"

double parseFormula(){
  cal_p.result = parseSub();
  if(*cal_p.input == '\0'){
    return cal_p.result;
  }
  // printf("Expected end of cal_p.input but found %c\n", *cal_p.input);
  printf("Syntax Error\n");
  return 0;
}

double parseSub(){
  double sub_1 = parseSum();
  while(*cal_p.input == '-'){
    ++cal_p.input;
    double sub_2 = parseSum();
    sub_1 = sub_1 - sub_2;
  }
  return sub_1;
}

double parseSum(){
  double sum_1 = parsePro();
  while(*cal_p.input == '+'){
    ++cal_p.input;
    double sum_2 = parsePro();
    sum_1 = sum_1 + sum_2;
  }
  return sum_1;
}

double parsePro(){
  double pro_1 = parseDiv();
  while(*cal_p.input == '*'){
    ++cal_p.input;
    double pro_2 = parseDiv();
    pro_1 = pro_1 * pro_2;
  }
  return pro_1;
}

double parseDiv(){
  double div_1 = parsePow();
  while(*cal_p.input == '/'){
    ++cal_p.input;
    double div_2 = parsePow();
    div_1 = div_1 / div_2;
  }
  return div_1;
}

double parsePow(){
  double pow_1 = parseFactor();
  while(*cal_p.input == '^'){
    ++cal_p.input;
    double pow_2 = parseFactor();
    pow_1 = pow(pow_1, pow_2);
  }
  return pow_1;
}

double parseFactor(){

  if(*cal_p.input >= '0' && *cal_p.input <= '9'){
    return parseNumber();
  }
  else if(*cal_p.input == '-'){
    return parseNumber();
  }
  else if(*cal_p.input == '('){
    ++cal_p.input;
    double temp = parseSub();
    ++cal_p.input;
    return temp;
  }
  //PI
  else if(*cal_p.input == 'p'){
    ++cal_p.input;
    if(*cal_p.input == 'i'){
      ++cal_p.input;
      return M_PI;
    }
  }
  //sin sqrt
  else if(*cal_p.input == 's'){
    ++cal_p.input;
    if(*cal_p.input == 'i'){
      ++cal_p.input;
      if(*cal_p.input == 'n'){
        ++cal_p.input;
        if(*cal_p.input == '('){
            ++cal_p.input;
            double temp = parseSub();
            temp = sin(temp*M_PI/180);
            ++cal_p.input;
            return temp;
        }

      }
    }
    else if(*cal_p.input == 'q'){
      ++cal_p.input;
      if(*cal_p.input == 'r'){
        ++cal_p.input;
        if(*cal_p.input == 't'){
            ++cal_p.input;
            if(*cal_p.input == '('){
            ++cal_p.input;
            double temp = parseSub();
            temp = sqrt(temp);
            ++cal_p.input;
            return temp;
          }

        }
      }
    }
  }
  //cos
  else if(*cal_p.input == 'c'){
    ++cal_p.input;
    if(*cal_p.input == 'o'){
      ++cal_p.input;
      if(*cal_p.input == 's'){
        ++cal_p.input;
        if(*cal_p.input == '('){
          ++cal_p.input;
          double temp = parseSub();
          temp = cos(temp*M_PI/180);
          ++cal_p.input;
          return temp;
        }

      }
    }
  }
  //tan
  else if(*cal_p.input == 't'){
    ++cal_p.input;
    if(*cal_p.input == 'a'){
      ++cal_p.input;
      if(*cal_p.input == 'n'){
        ++cal_p.input;
        if(*cal_p.input == '('){
          ++cal_p.input;
          double temp = parseSub();
          temp = tan(temp*M_PI/180);
          ++cal_p.input;
          return temp;
        }

      }
    }
  }
  //asin acos atan
  else if(*cal_p.input == 'a'){
    ++cal_p.input;
    if(*cal_p.input == 's'){
      ++cal_p.input;
      if(*cal_p.input == 'i'){
        ++cal_p.input;
        if(*cal_p.input == 'n'){
          ++cal_p.input;
          if(*cal_p.input == '('){
            ++cal_p.input;
            double temp = parseSub();
            temp = asin(temp*M_PI/180);
            ++cal_p.input;
            return temp;
          }

        }
      }
    }
    else if(*cal_p.input == 'c'){
      ++cal_p.input;
      if(*cal_p.input == 'o'){
        ++cal_p.input;
        if(*cal_p.input == 's'){
          ++cal_p.input;
          if(*cal_p.input == '('){
            ++cal_p.input;
            double temp = parseSub();
            temp = acos(temp*M_PI/180);
            ++cal_p.input;
            return temp;
          }

        }
      }
    }
    else if(*cal_p.input == 't'){
      ++cal_p.input;
      if(*cal_p.input == 'a'){
        ++cal_p.input;
        if(*cal_p.input == 'n'){
          ++cal_p.input;
          if(*cal_p.input == '('){
            ++cal_p.input;
            double temp = parseSub();
            temp = atan(temp*M_PI/180);
            ++cal_p.input;
            return temp;
          }

        }
      }
    }
  }
  //exp
  else if(*cal_p.input == 'e'){
    ++cal_p.input;
    if(*cal_p.input == 'x'){
      ++cal_p.input;
      if(*cal_p.input == 'p'){
        ++cal_p.input;
        if(*cal_p.input == '('){
          ++cal_p.input;
          double temp = parseSub();
          temp = exp(temp);
          ++cal_p.input;
          return temp;
        }

      }
    }
  }
  //ln log10
  else if(*cal_p.input == 'l'){
    ++cal_p.input;
    if(*cal_p.input == 'n'){
      ++cal_p.input;
      if(*cal_p.input == '('){
        ++cal_p.input;
        double temp = parseSub();
        temp = log(temp);
        ++cal_p.input;
        return temp;
      }
    }
    else if(*cal_p.input == 'o'){
      ++cal_p.input;
      if(*cal_p.input == 'g'){
        ++cal_p.input;
        if(*cal_p.input == '('){
          ++cal_p.input;
          double temp = parseSub();
          temp = log10(temp);
          ++cal_p.input;
          return temp;
        }

      }
    }
  }
  else{
    printf("Syntax Error\n");
    printf("Unknown symbol %c", *cal_p.input);
  }
  return 0;
}

double parseNumber(){

  double number = 0;
  int neg_flag = 1;
  //TODO check the first character is a number or a minus
  if(*cal_p.input >= '0' && *cal_p.input <= '9'){
  }
  else if(*cal_p.input == '-'){
    neg_flag = -1;
    ++cal_p.input;
  }
  else{
    printf("Syntax Error");
  }


  while(*cal_p.input >= '0' && *cal_p.input <= '9'){
    number = number * 10;
    number = number + (int)(*cal_p.input - '0');
    ++cal_p.input;
  }

  if(*cal_p.input == '.'){
    ++cal_p.input;

    //Check the next character is a number, else error

    if(*cal_p.input >= '0' && *cal_p.input <= '9'){
      double weight = 1;
      while(*cal_p.input >= '0' && *cal_p.input <= '9'){
        weight = weight / 10.0;
        double scaled = (int)(*cal_p.input - '0') * weight;
        number = number + scaled;
        ++cal_p.input;
      }
    }
    else{
      printf("Syntax Error");
    }
  }

  return (number * neg_flag);
}
