#include "transit.h"

// ----- Конструктор по умолчанию
// --
// --
Transit::Transit()
{
    // Последний символ
    ultimate = '0';
    // Сервисная строка
    infix = "0";
    // Строка вывода
    input = "0";

    open_brackets = 0;
    close_brackets = 0;
}

// ----- Метод распределения символа
// --
// --
void Transit::Insert(std::string &__buf)
{
    static char current;


    if (__buf == "x^y") // Проверяем, что пришёл текст "x^y"
    {
        current = '^';  // Используем символ "^"
        __buf = "^";    // Обновляем буфер для корректной вставки символа "^"
    }
    else if (__buf == "Ln")
    {
        current = 'l';
        __buf = "Ln"; // Обновляем буфер для корректной вставки символа
    }
    else if (__buf == "+/-")
    {
        current = '~'; // Обрабатываем унарный минус
    }
    else
    {
        current = __buf[0]; // Обычные символы
    }


    switch (current)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        Numeral_Insert(__buf);
        break;
    case '/':
    case '*':
    case '-':
    case '+':
    case '^':
        Operator_Insert(__buf);
        break;
    case 'c':
    case 's':
    case 't':
    case 'g':
    case 'l':
        Function_Insert(__buf);
        break;
    case '(':
        Left_Bracket_Insert();
        break;
    case ')':
        Right_Bracket_Insert();
        break;
    case '.':
        Point_Insert();
        break;
    case '~':
        Unary_Minus_Change();
        break;
    default:
        throw std::invalid_argument("ERROR: unknown symbol");
        break;
    }
}

// ----- Метод показа текущего выражения
// --
// --
QString Transit::Show()
{
    return input;
}

// ----- Метод удаления последнего символа
// --
// --
void Transit::Erase()
{
    // Если размер выражения равен одному, тогда обнуляем всё
    if (infix.size() == 1)
    {
        input = "0";
        infix = "0";
    }
    // Если размер больше единицы, тогда удаляем только один символ за раз
    else
    {
        // Проверяем последний символ infix
        char last_char = infix.back();

        switch (last_char)
        {
        // Для тригонометрических функций нужно удалить 1 символ из строки и 3 символа из окна
        case 's':
        case 'c':
        case 't':
        case 'g':
            // Тригонометрическая функция всегда удаляется за один шаг (3 символа из input, 1 из infix)
            if (input.size() >= 3) // Проверяем, чтобы строка была достаточно длинной
                input.chop(3); // Удаляем 3 символа из строки input
            infix.erase(infix.size() - 1, 1); // Удаляем последний символ из строки infix
            break;

            // Если точка
        case '.':
            input.chop(1); // Удаляем один символ
            infix.erase(infix.size() - 1, 1); // Удаляем один символ
            break;

            // Если унарный минус
        case '~':
            // Проверяем, есть ли перед унарным минусом открывающая скобка
            if (infix.size() > 2 && infix[infix.size() - 2] == '(' && infix[infix.size() - 3] == '~') {
                // Удаляем унарный минус и скобку
                input.chop(2); // Удаляем "(-"
                infix.erase(infix.size() - 2, 2);
                open_brackets--; // Уменьшаем счётчик скобок
            } else {
                // Иначе просто удаляем минус
                input.chop(1);
                infix.erase(infix.size() - 1, 1);
            }
            break;

            // Если открывающая скобка
        case '(':
            input.chop(1);
            infix.erase(infix.size() - 1, 1);
            open_brackets--;
            break;

            // Если закрывающая скобка
        case ')':
            input.chop(1);
            infix.erase(infix.size() - 1, 1);
            close_brackets--;
            break;

        default:
            // Для любых других символов удаляем по одному символу за раз
            input.chop(1); // Удаляем последний символ из строки input
            infix.erase(infix.size() - 1, 1); // Удаляем последний символ из строки infix
            break;
        }
    }
}



// ----- Метод очистки выражения
// --
// --
void Transit::Clear()
{
    input = "0"; // Обнуляем строки
    infix = "0";
    open_brackets = 0; // Обнуляем балансы скобок
    close_brackets = 0;
}

// ----- Метод подсчёта результата
// --
// --
big Transit::Calculate()
{
    ultimate = infix.substr(infix.size() - 1, 1)[0]; // Последний символ строки

    // Если последний символ не является числом или скобкой
    if (!(ultimate >= '0' && ultimate <= '9') && ultimate != ')')
    {
        if (ultimate != '(') 
        {
            erase_buf = input;
            erase_buf.chop(1); // Удалить последний символ
            input = erase_buf;
            infix.erase(infix.size() - 1, 1); // Удалить последний символ
        }
    }

    // Проверка баланса скобок
    if (open_brackets != close_brackets)
    {
        throw std::invalid_argument("ERROR: unbalanced brackets");
    }

    return Calc.Calculate(infix);
}


// ----- Метод вставки чисел
// --
// --
void Transit::Numeral_Insert(std::string __buf)
{
    ultimate = infix.substr(infix.size() - 1, 1)[0];  // Последний символ строки
    // Если в выражении больше одного символа и последний символ = 0
    if (infix.size() > 1 && ultimate == '0')
    {
        // Если до нуля что-то было, значит это продолжение полноценного числа
        penultimate = infix.substr(infix.size() - 2, 1)[0];  // Предпоследний символ строки
        if (Calc.Priority(penultimate) == 5) // Если приоритет символа равен приоритету чисел и точки
        {
            // Просто добавляем цифру в строку
            input += QString::fromStdString(__buf);
            infix += __buf;
        }
        // Если до нуля ничего не было, значит это не число, а просто ноль

        else
        {
            // Прежде добавления удаляем этот ноль
            erase_buf = input;
            erase_buf.chop(1); // Удаляем ноль
            input = erase_buf + QString::fromStdString(__buf);  // Добавляем цифру в строку
            infix.erase(infix.size() - 1, 1); // Удаляем ноль
            infix += __buf; // Добавляем цифру в строку
        }
    }
    // Если в строке только ноль
    else if (infix.size() == 1 && ultimate == '0') 
    {
        // Заменяем этот ноль числом
        input = QString::fromStdString(__buf);
        infix.erase(infix.size() - 1, 1);
        infix += __buf;
    }
    // Во всех остальных случаях
    else
    {
        // Добавляем цифру к строке
        input += QString::fromStdString(__buf);
        infix += __buf;
    }
}

// ----- Метод вставки оператора
// --
// --
void Transit::Operator_Insert(std::string __buf)
{
    ultimate = infix.back();  // Последний символ инфиксной строки

    // Если перед вставляемой операцией есть число или закрывающая скобка
    if (isdigit(ultimate) || ultimate == ')')
    {
        input += QString::fromStdString(__buf);
        infix += __buf;                          //Добавляем оператор
    }
    // Если перед вставляемой операцией уже стоит оператор, заменяем его новым оператором
    else if (ultimate == '+' || ultimate == '-' || ultimate == '*' || ultimate == '/' || ultimate == '^')
    {
        // Удаляем последний оператор
        input.chop(1);                   // Убираем последний символ из строки для красивого вывода
        infix.erase(infix.size() - 1);   // Убираем последний символ из сервисной строки

        // Добавляем новый оператор
        input += QString::fromStdString(__buf);
        infix += __buf;
    }
    // Если перед оператором открывающая скобка, ничего не делаем
    else if (ultimate == '(')
    {
        // Ничего не делаем
    }
    // Если последний символ — точка, заменяем оператор после удаления точки
    else if (ultimate == '.')
    {
        erase_buf = input;
        erase_buf.chop(1);
        input = erase_buf + QString::fromStdString(__buf);
        infix.erase(infix.size() - 1, 1);
        infix += __buf;
    }
    else
    {
        std::cerr << "Unhandled ultimate character before operator: " << ultimate << std::endl;  // Поток вывода об ошибках
    }
}


// ----- Метод вставки тригонометрической функции и логарифма
void Transit::Function_Insert(std::string __buf)
{
    ultimate = infix.substr(infix.size() - 1, 1)[0];

    // Обработка логарифма
    if (__buf == "Ln")
    {
        // Если перед логарифмом стоит ноль
        if (ultimate == '0' && infix.size() == 1)
        {
            input = "Ln(";
            infix = "l("; // Используем символ 'l' для инфиксной строки
            open_brackets++;
        }
        // Если перед логарифмом стоит оператор или открывающая скобка
        else if (Calc.Priority(ultimate) == 1 || ultimate == '(')
        {
            input += "Ln(";
            infix += "l(";
        }
        // Если перед логарифмом стоит число
        else if (ultimate >= '0' && ultimate <= '9')
        {
            input += "*" + QString::fromStdString("Ln(");
            infix += "*l(";
            open_brackets++;
        }
        else
        {
            input += "Ln(";
            infix += "l(";
            open_brackets++;
        }
    }
    else
    {
        // Обработка других функций
        if (ultimate == '0' && infix.size() == 1) 
        {
            input = QString::fromStdString(__buf) + "(";
            infix = __buf.substr(0, 1) + "(";
            open_brackets++;
        }
        else if (Calc.Priority(ultimate) == 1 || ultimate == '(')
        {
            input += QString::fromStdString(__buf) + "(";
            infix += __buf.substr(0, 1) + "(";
            open_brackets++;
        }
        else if (ultimate >= '0' && ultimate <= '9')
        {
            input += "*" + QString::fromStdString(__buf) + "(";
            infix += "*" + __buf.substr(0, 1) + "(";
            open_brackets++;
        }
        else
        {
            input += QString::fromStdString(__buf) + "(";
            infix += __buf.substr(0, 1) + "(";
            open_brackets++;
        }
    }
}

// ----- Метод вставки левой скобки
// --
// --
void Transit::Left_Bracket_Insert()
{
    ultimate = infix.substr(infix.size() - 1, 1)[0]; // Последний символ строки
    // Если перед скобкой стоит оператор
    if (Calc.Priority(ultimate) == 1)
    {
        input += "("; // Добавляем "("
        infix += "(";
        open_brackets++;
    // Если перед скобкой стоит функция
    }
    else if (ultimate == 's' || ultimate == 'c' || ultimate == 't' || ultimate == 'g')
    {
        input += "(";
        infix += "(";
        open_brackets++;
    }
    // Если перед скобкой стоит другое число
    else if (ultimate > '0' && ultimate <= '9')
    {
        input += "*(";
        infix += "*(";
        open_brackets++;
    }
    // Если перед скобкой стоит 0
    else if (infix.size() == 1 && ultimate == '0')
    {
        input.chop(1);
        infix.erase(0,1);
        input += "(";
        infix += "(";
        open_brackets++;
    }
    // Если перед скобкой закрывающая скобка
    else if (ultimate == ')')
    {
        input += "*(";
        infix += "*(";
        open_brackets++;
    }
    // Если перед скобкой стоит точка
    else if (ultimate == '.')
    {
        erase_buf = input;
        erase_buf.chop(1);
        input = erase_buf + "*(";
        infix.erase(infix.size() - 1, 1);
        infix += "*(";
        open_brackets++;
    }
    else
    {
        input += "(";
        infix += "(";
        open_brackets++;
    }
}

// ----- Метод вставки правой скобки
// --
// --
void Transit::Right_Bracket_Insert()
{
    ultimate = infix.substr(infix.size() - 1, 1)[0];
    // Закрываем скобку, если перед ней стоит число
    if ((ultimate >= '0' && ultimate <= '9') || ultimate == ')')
    {
        // Закрываем скобку, только если открывающих скобок больше
        if (open_brackets > close_brackets)
        {
            input += ")";
            infix += ")";
            close_brackets++;
        }
    }
}

// ----- Метод изменения унарного минуса
// --
// --
void Transit::Unary_Minus_Change()
{
    ultimate = infix.back();  // Получаем последний символ инфиксной строки

    // Если в строке только ноль
    if (infix == "0")
    {
        // Заменяем ноль на унарный минус с открытой скобкой
        input = "(-";
        infix = "~(";
        open_brackets++;
    }
    // Проверяем, есть ли перед унарным минусом унарный минус
    if(infix.size() >= 2)
    {
        ultimate=infix[infix.size()-1];
        penultimate=infix[infix.size()-2];
        if (penultimate == '~' && ultimate == '(')
        {
            // Удаляем унарный минус
            infix.erase(infix.size() - 2,2);  // убираем "~("
            input.remove(input.size()- 2,2); // убираем "(-"
            open_brackets--;
        }
    }


    // Если последний символ - цифра или точка
    if (isdigit(ultimate) || ultimate =='.')
    {
        // Ищем начало числа
        size_t InfixPos = infix.size() - 1;
        size_t InputPos = input.size() - 1;
        while (InfixPos > 0 && (isdigit(infix[InfixPos - 1]) || infix[InfixPos - 1] == '.'))
        {
            InfixPos--;
            InputPos--;
        }

        // Проверяем, есть ли перед числом унарный минус
        if (InfixPos >= 2 && infix[InfixPos - 2] == '~' && infix[InfixPos - 1] == '(')
        {
            // Удаляем унарный минус
            infix.erase(InfixPos - 2, 2);  // убираем "~("
            input.remove(InputPos - 2, 2); // убираем "(-"
            open_brackets--;
        }
        else
        {
            // Добавляем унарный минус перед числом
            infix.insert(InfixPos, "~(");
            input.insert(InputPos, "(-");
            open_brackets++;
        }
    }
    // Если последний символ - закрывающая скобка
    else if (ultimate == ')')
    {
        // Ищем позицию соответствующей открывающей скобки
        size_t open_bracket_pos = infix.rfind('(', infix.size() - 2);
        if (open_bracket_pos != std::string::npos && open_bracket_pos >= 2
            && infix[open_bracket_pos - 2] == '~' && infix[open_bracket_pos - 1] == '(') //если перед найдной скобкрй есть унарный минус
        {
            // Если перед открывающей скобкой был унарный минус, убираем его
            infix.erase(open_bracket_pos - 2, 2);  // убираем "~("
            input.remove(open_bracket_pos - 2, 2); // убираем "(-"
            open_brackets--;
        }
        else
        {
            // Иначе добавляем новый унарный минус перед выражением в скобках
            input += "*(-";
            infix += "*~(";
            open_brackets++;
        }
    }
    // Если последний символ - оператор или открывающая скобка
    else if (Calc.Priority(ultimate) == 1 || ultimate == '(')
    {
        input += "(-";
        infix += "~(";
        open_brackets++;
    }

}

// ----- добавить плавающую точку
// --
// --
void Transit::Point_Insert()
{
    // Убедиться, что в числе нет второй точки
    int flag_point = 0;
    for (unsigned i = 1; i <= infix.size(); i++)
    {
        // Если достигнуто начало выражения и ничего не найдено, тогда добавить точку
        if (infix.size() == i && flag_point == 0)
        {
            input += ".";
            infix += ".";
            break;
        }
        ultimate = infix.substr(infix.size() - i, 1)[0];
        switch (ultimate)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '(':
        case '~':
            // Если найдена операция, не найдена точка, не найдено число, тогда добавить ноль и точку
            if (flag_point == 0 && i == 1)
            {
                input += "0.";
                infix += "0.";
                flag_point = 1;
            }
            // Если найдена операция, не найдена точка, но найдено число, тогда добавить точку
            else if (flag_point == 0 && i > 1)
            {
                input += ".";
                infix += ".";
                flag_point = 1;
            }
            // Если ничего не найдено, надо вырубить метод
            else
                flag_point = 1;
            break;
        case '.':
            // Если найдена точка,  вырубаем метод
            flag_point = 1;
            break;
        default:
            break;
        }
        // Если точка найдена, тогда ничего не делать
        if (flag_point == 1)
            break;
    }


}
