/********************************************************
 * Автор:     Иккерт А.С.                               *
 * Название:  Арифметика длинных чисел                  *
 * Вариант:   7                                         *
 ********************************************************/



#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Функция для сравнения двух строк-чисел
bool isFirstLessThan(const string& num1, const string& num2) {
    if (num1.length() != num2.length()) {
        return num1.length() < num2.length();
    }
    return num1 < num2;
}

// Функция для сложения двух положительных чисел в виде строк
string addStrings(const string& num1, const string& num2) {
    string result;
    int carry = 0;
    int index1 = num1.length() - 1;
    int index2 = num2.length() - 1;
    
    while (index1 >= 0 || index2 >= 0 || carry > 0) {
        int digit1 = (index1 >= 0) ? num1[index1] - '0' : 0;
        int digit2 = (index2 >= 0) ? num2[index2] - '0' : 0;
        
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
        
        index1--;
        index2--;
    }
    
    reverse(result.begin(), result.end());
    return result;
}

// Функция для вычитания двух положительных чисел (num1 >= num2)
string subtractStrings(const string& num1, const string& num2) {
    if (isFirstLessThan(num1, num2)) {
        return "0";
    }
    
    string result;
    int borrow = 0;
    int index1 = num1.length() - 1;
    int index2 = num2.length() - 1;
    
    while (index1 >= 0) {
        int digit1 = num1[index1] - '0' - borrow;
        int digit2 = (index2 >= 0) ? num2[index2] - '0' : 0;
        
        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.push_back((digit1 - digit2) + '0');
        
        index1--;
        index2--;
    }
    
    // Удаляем ведущие нули
    while (result.length() > 1 && result.back() == '0') {
        result.pop_back();
    }
    
    reverse(result.begin(), result.end());
    return result;
}

// Функция для умножения двух положительных чисел
string multiplyStrings(const string& num1, const string& num2) {
    if (num1 == "0" || num2 == "0") {
        return "0";
    }
    
    int length1 = num1.length();
    int length2 = num2.length();
    vector<int> resultArray(length1 + length2, 0);
    
    for (int i = length1 - 1; i >= 0; i--) {
        for (int j = length2 - 1; j >= 0; j--) {
            int product = (num1[i] - '0') * (num2[j] - '0');
            int sum = product + resultArray[i + j + 1];
            
            resultArray[i + j + 1] = sum % 10;
            resultArray[i + j] += sum / 10;
        }
    }
    
    string result;
    for (int digit : resultArray) {
        if (!(result.empty() && digit == 0)) {
            result.push_back(digit + '0');
        }
    }
    
    return result.empty() ? "0" : result;
}

// Функция для деления двух положительных чисел (целочисленное деление)
string divideStrings(const string& num1, const string& num2) {
    if (num2 == "0") {
        return "ERROR: Division by zero";
    }
    if (isFirstLessThan(num1, num2)) {
        return "0";
    }
    if (num2 == "1") {
        return num1;
    }
    
    string result;
    string current;
    
    for (char digitChar : num1) {
        current.push_back(digitChar);
        
        // Удаляем ведущие нули
        while (current.length() > 1 && current[0] == '0') {
            current.erase(0, 1);
        }
        
        if (!isFirstLessThan(current, num2)) {
            int count = 0;
            string temp = current;
            
            while (!isFirstLessThan(temp, num2)) {
                temp = subtractStrings(temp, num2);
                count++;
            }
            
            result.push_back(count + '0');
            current = temp;
        } else {
            if (!result.empty()) {
                result.push_back('0');
            }
        }
    }
    
    return result.empty() ? "0" : result;
}

// Основные арифметические операции с учетом знака
string addBigInt(const string& num1, const string& num2) {
    bool num1Negative = (num1[0] == '-');
    bool num2Negative = (num2[0] == '-');
    
    string absNum1 = num1Negative ? num1.substr(1) : num1;
    string absNum2 = num2Negative ? num2.substr(1) : num2;
    
    if (!num1Negative && !num2Negative) {
        return addStrings(absNum1, absNum2);
    } else if (num1Negative && num2Negative) {
        return "-" + addStrings(absNum1, absNum2);
    } else if (num1Negative && !num2Negative) {
        if (isFirstLessThan(absNum1, absNum2)) {
            return subtractStrings(absNum2, absNum1);
        } else {
            return "-" + subtractStrings(absNum1, absNum2);
        }
    } else {
        if (isFirstLessThan(absNum2, absNum1)) {
            return subtractStrings(absNum1, absNum2);
        } else {
            return "-" + subtractStrings(absNum2, absNum1);
        }
    }
}

string subtractBigInt(const string& num1, const string& num2) {
    bool num1Negative = (num1[0] == '-');
    bool num2Negative = (num2[0] == '-');
    
    string absNum1 = num1Negative ? num1.substr(1) : num1;
    string absNum2 = num2Negative ? num2.substr(1) : num2;
    
    if (!num1Negative && !num2Negative) {
        if (isFirstLessThan(absNum1, absNum2)) {
            return "-" + subtractStrings(absNum2, absNum1);
        } else {
            return subtractStrings(absNum1, absNum2);
        }
    } else if (num1Negative && num2Negative) {
        if (isFirstLessThan(absNum2, absNum1)) {
            return subtractStrings(absNum2, absNum1);
        } else {
            return "-" + subtractStrings(absNum1, absNum2);
        }
    } else if (num1Negative && !num2Negative) {
        return "-" + addStrings(absNum1, absNum2);
    } else {
        return addStrings(absNum1, absNum2);
    }
}

string multiplyBigInt(const string& num1, const string& num2) {
    bool num1Negative = (num1[0] == '-');
    bool num2Negative = (num2[0] == '-');
    
    string absNum1 = num1Negative ? num1.substr(1) : num1;
    string absNum2 = num2Negative ? num2.substr(1) : num2;
    
    string result = multiplyStrings(absNum1, absNum2);
    
    if ((num1Negative && !num2Negative) || (!num1Negative && num2Negative)) {
        if (result != "0") {
            return "-" + result;
        }
    }
    
    return result;
}

string divideBigInt(const string& num1, const string& num2) {
    bool num1Negative = (num1[0] == '-');
    bool num2Negative = (num2[0] == '-');
    
    string absNum1 = num1Negative ? num1.substr(1) : num1;
    string absNum2 = num2Negative ? num2.substr(1) : num2;
    
    string result = divideStrings(absNum1, absNum2);
    
    if ((num1Negative && !num2Negative) || (!num1Negative && num2Negative)) {
        if (result != "0" && result[0] != 'E') {
            return "-" + result;
        }
    }
    
    return result;
}

// Функции сравнения
bool isGreaterThan(const string& num1, const string& num2) {
    bool num1Negative = (num1[0] == '-');
    bool num2Negative = (num2[0] == '-');
    
    string absNum1 = num1Negative ? num1.substr(1) : num1;
    string absNum2 = num2Negative ? num2.substr(1) : num2;
    
    if (!num1Negative && num2Negative) return true;
    if (num1Negative && !num2Negative) return false;
    if (!num1Negative && !num2Negative) return !isFirstLessThan(absNum1, absNum2) && absNum1 != absNum2;
    return isFirstLessThan(absNum1, absNum2) && absNum1 != absNum2;
}

bool isLessThan(const string& num1, const string& num2) {
    return !isGreaterThan(num1, num2) && num1 != num2;
}

bool isEqual(const string& num1, const string& num2) {
    return num1 == num2;
}

int main() {
    string num1, num2;
    char operation;
    
    cout << "Enter first number: ";
    cin >> num1;
    
    cout << "Enter second number: ";
    cin >> num2;
    
    cout << "Enter operation (+, -, *, /, >, <, =): ";
    cin >> operation;
    
    string result;
    bool comparisonResult;
    
    switch (operation) {
        case '+':
            result = addBigInt(num1, num2);
            cout << "Result: " << result << endl;
            break;
        case '-':
            result = subtractBigInt(num1, num2);
            cout << "Result: " << result << endl;
            break;
        case '*':
            result = multiplyBigInt(num1, num2);
            cout << "Result: " << result << endl;
            break;
        case '/':
            result = divideBigInt(num1, num2);
            cout << "Result: " << result << endl;
            break;
        case '>':
            comparisonResult = isGreaterThan(num1, num2);
            cout << "Result: " << (comparisonResult ? "true" : "false") << endl;
            break;
        case '<':
            comparisonResult = isLessThan(num1, num2);
            cout << "Result: " << (comparisonResult ? "true" : "false") << endl;
            break;
        case '=':
            comparisonResult = isEqual(num1, num2);
            cout << "Result: " << (comparisonResult ? "true" : "false") << endl;
            break;
        default:
            cout << "Invalid operation!" << endl;
    }
    
    return 0;
}