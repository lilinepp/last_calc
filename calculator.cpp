#include "calculator.h"

// Конструктор по умолчанию
Calculator::Calculator()
{
    postfix = std::stack<std::string>(); // Создание стека для постфиксной записи
}

// Приоритет символов
int Calculator::Priority(char& __oper) {
    switch (__oper)
    {
    // Скобка во избежание ошибки
    case '(':
        return 0;
        break;
    // Бинарные операции
    case '+':
    case '-':
       return 1;
       break;
    case '*':
    case '/':
        return 2;
        break;
    case '^':
        return 3;
        break;
    // Унарные операции
    case '~':
    case 'c':
    case 's':
    case 't':
    case 'g':
    case 'l':
        return 4;
        break;
    // Числа
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
    case '.':
        return 5;
        break;
    default:
        return -1;
        break;

    }

}

// Метод преобразования инфиксного выражения в постфиксное
void Calculator::Marshalling_yard(std::string& __inf)
{
    // Стек развёрнутой постфиксной записью
    std::stack<std::string> postfix_reverse;

    // Стек для операций
    std::stack<char> operations;

    // Буфер для чисел
    std::string num_buf = "";

    // Флаг наличия чисел
    bool availabilityOfNumbers = 0;

    // Счётчики для проверки выражения
    unsigned num_count = 0;
    unsigned oper_count = 0;

    // ------ Проверка наличия цифр в выражении
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++)
        if ('0' <= *i && *i <= '9') // Проверяем является ли символ числом
        {
            availabilityOfNumbers = 1;  // Ставим число
            break;
        }
    if (availabilityOfNumbers == 0) // Если числа не найдено
        throw std::invalid_argument("ERROR: number not found"); // Бросаем исключительную ситуацию

    // ----- Проверка на лишнюю унарную операцию
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++)
        if (Priority(*i) == 4 &&      // Унарные операции
            Priority(*(i+1)) != 5 &&  // Следующий символ не является числом
            Priority(*(i+1)) != 4 &&  // Следующий символ не является другим унарным оператором
            Priority(*(i+1)) != 0)    // Следующий символ не является закрывающейся скобкой
            throw std::invalid_argument("ERROR: extra unary operator");

    // ----- Преобразование инфиксного выражения в постфикное
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++) // Достижение конца строки
    {
        if (i == __inf.end()) // Если мы достигли конца строки
        {
            if (num_buf.empty())  // Проверка пуст ли буфер для чисел
                break;
            else
            {
                // Если буфер не пустой в конце прохода строки,
                // Выталкиваем число из этого буфера в стек
                postfix_reverse.push(num_buf);
                // Очищаем строку
                num_buf.clear();
                break;
            }
        }
        // Если текущий символ число или плавающая точка
        if (Priority(*i) == 5)
        {
            // Добавляем текущий символ в буфер
            num_buf += *i;
            continue; // Переход к следующей итерации цикла

            // Если число кончилось  или не найдено
            // Проверяем буфер на заполненность
        }
        else if (!num_buf.empty())
        {
            // Если буфер не пустой,
            // Выталкиваем число в стек
            postfix_reverse.push(num_buf);
            // Очищаем строку
            num_buf.clear();
        }
        // Если на текущей позиции открывающаяся скобка
        if (*i == '(')
        {
            operations.push(*i); // Добавляем ее в стек для операций
            continue;
        }
        // Если на текущей позиции закрывающаяся скобка
        if (*i == ')')
        {
            if (operations.empty()) // Проверяем пуст ли стек для операций
                throw std::invalid_argument("ERROR: extra right bracket"); // Выбрасываем исключение
            // Выталкиваем из стека все операторы
            // До открывающейся скобки
            while (operations.top() != '(') // Обработка операторов до открывающей скобки
            {
                num_buf = operations.top();     // Cохранение верхнего оператора в буфер
                postfix_reverse.push(num_buf);  // Перемещение оператора в постфиксное выражение
                operations.pop();               // Удаление верхнего элемента из стека операций
                if (operations.empty())         // Пуст ли стек
                    // Разобрать на примере
                    throw std::invalid_argument("ERROR: extra right bracket"); // Выбрасываем исключение
            }
                operations.pop(); // Удаляем верхний элемент стека
            // Удаляем закрывающуюся скобку
            num_buf.clear(); // Очищаем буфер
            continue;
        }
        // Если стек операций пуст
        if (operations.empty())
        {
            // Просто закидываем операцию в стек
            operations.push(*i);
            continue;
        }
        // если стек операций не пуст
        // выталкиваем все операторы, имеющие приоритет выше
        else {
            // пока приоритет операции в стеке выше рассматриваемого
            while (!operations.empty())
            {
                if (Priority(operations.top()) >= Priority(*i))
                {   // выталкиваем оператор в выходной стек
                    num_buf = operations.top();     // Сохраняем верхний оператор в буфер
                    postfix_reverse.push(num_buf); // Добавляем в стек
                    operations.pop();              // Удаляем верхний элемент стека
                }
                else
                    break;
            }
            // записываем оператор в стек операций
            operations.push(*i);
            num_buf.clear();
        }
    }

    // ----- проверка соответствия количества операций и чисел
    // --
    // --
    if (!operations.empty() && postfix_reverse.empty())
        throw std::invalid_argument("ERROR: extra operation");

    // выталкиваем оставшиеся операторы
    while (!operations.empty())
    {
        // выталкиваем оператор в выходной стек
        num_buf = operations.top(); // Извлекаем верхний оператор в буфер
        postfix_reverse.push(num_buf); // Добавляем операторы в стек
        operations.pop(); // Удаляем операторы из стека операций
    }

    // ----- заполняем стек с постфиксной записью
    // --
    // --
    while (!postfix_reverse.empty())
    {
        // заодно проверим, соответствуют ли операции числам
        switch (Priority(postfix_reverse.top()[0])) // Посимвольно возвращаем верхние символы стека
        {
        // если найдена скобка
        case '0':
            throw std::invalid_argument("ERROR: extra right bracket");
            break;
        case '1':
        case '2':
        case '3':
            // если бинарная операция
            oper_count += 2;
            break;
        // если унарная операция
        case '4':
            oper_count += 1;
            break;
        // если число
        case '5':
            num_count += 2;
            break;
        }
        // если не соответствует, то ошибка
        if (oper_count > num_count)
            throw std::invalid_argument("ERROR: extra operations");

        postfix.push(postfix_reverse.top()); // Переносим элементы из временного стека в обычный
        postfix_reverse.pop(); // Очищаем временный стек
    }
    // Очищаем эти два стека так как они больше не нужны

    postfix_reverse = std::stack<std::string>();
    operations = std::stack<char>();
}

// Метод вычисления выражения
big Calculator::Calculate(std::string& __inf)
{
    // преобразуем инфиксную строку в постфиксную
    Marshalling_yard(__inf);
    // создаём стек-буфер для вычисления
    std::stack<big> buf_calc;  // big= long double.
    // переменную для верхнего числа из стека
    big ultimate = 0.00;
    // переменную для предверхнего числа из стека
    big penultimate = 0.00;

    // пока постфиксная запись не будет пройдена
    while (!postfix.empty())
    {
        // если текущий элемент является числом
        if (Priority(postfix.top()[0]) == 5)
        {
            // заносим его в стек вычисления
            buf_calc.push(stold(postfix.top())); // Переносим из postfix в buf_calc и преобразуем строку в Long Double
            // удаляем из постфиксной записи
            postfix.pop();
        }
        // если текущий элемент является операцией
        else
        {
            // берём последнее число из стека
            ultimate = buf_calc.top();
            buf_calc.pop();
            // если есть предпоследнее число
            if (!buf_calc.empty())
            {
                // то его тоже берём
                penultimate = buf_calc.top();
            }
            // обрабатываем операцию
            switch (postfix.top()[0])
            {
            case '(':
                throw std::invalid_argument("ERROR: extra left breacket");
                break;
            // ... сложения
            case '+':
                buf_calc.pop();
                buf_calc.push(penultimate + ultimate);
                break;
            // ... вычитания
            case '-':
                buf_calc.pop();
                buf_calc.push(penultimate - ultimate);
                break;
            // ... умножения
            case '*':
                buf_calc.pop();
                buf_calc.push(penultimate * ultimate);
                break;
            // ... деления
            case '/':
                buf_calc.pop();
                if (ultimate == 0)
                    throw std::invalid_argument("ERROR: division by zero");
                buf_calc.push(penultimate / ultimate);
                break;
            case '^':
                buf_calc.pop();
                buf_calc.push(pow(penultimate, ultimate));
                break;
            // ... унарного минуса
            case '~':
                buf_calc.push(ultimate * -1);
                break;
            // ... косинуса
            case 'c':
                buf_calc.push(cos(ultimate));
                break;
            // ... синуса
            case 's':
                buf_calc.push(sin(ultimate));
                break;
            // ... тангенса
            case 't':
                buf_calc.push(tan(ultimate));
                break;
            // ... котангенса
            case 'g':
                buf_calc.push(1 / tan(ultimate));
                break;
            case 'l':
                // Проверка: логарифм возможен только для положительных чисел
                if (ultimate <= 0)
                throw std::invalid_argument("ERROR: log of non-positive number");
                buf_calc.push(log(ultimate)); // Вычисление логарифма
                break;
            // если найден какой-то инородный символ
            default:
                throw std::invalid_argument("ERROR: unknown symbol");
                break;
            }
            postfix.pop(); // Удаляем из стека верхний оператор
        }
    }

    last_result = buf_calc.top(); // Передаем результат
    buf_calc = std::stack<big>(); // Очищаем стек
    return last_result;
}

void Calculator::Clear()
{
    postfix = std::stack<std::string>(); // Очищаем стек
}

big Calculator::Get_Last()
{
    return last_result;
}
