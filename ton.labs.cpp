/**
 * @file Тестовое задание Ton Labs.
 * @author Владимир Лазарев (mailto:solock@mail.ru)
 * @details Необходимо реализовать функцию, которая принимает в качестве аргумента строку s,
 * содержащую любые буквы латинского алфавита а также скобки вида (){}[], и выдает в результате
 * наибольшую возможную строку, такую что она является подстрокой бесконечной строки вида sssssss...
 * (состоит из бесконечной конкатенации строчек s) и скобочные символы в ней составляют правильную
 * скобочную последовательность.
 * Если такая строка имеет бесконечную длину, вернуть строку "Infinite".
 * Также необходимо реализовать тесты.
 * @example
 * 1)
 * Input: }](){
 * Output: (){}
 *
 * 2)
 * Input: sh(dh)}
 * Output: sh(dh)
 *
 * 3)
 * Input: ]h({hdb}b)[
 * Output: Infinite
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <map>

using namespace std;

/**
 * @brief Функция, вовзращающая подстроку в соответствии с заданием.
 * @param s исходная строка.
 * @return Подстрока в соответствии с заданием.
 */
string getMaxCorrectString (const string&);

/**
 * @brief Проверяет соответствие открывающей и закрывающей скобок друг другу.
 * @param openBracket открывающая скобка.
 * @param closeBracket закрывающая скобка
 * @return true, если скобки соответствуют друг другу, false - во всех иных случаях.
 */
bool areBracketsMatched (char openBracket, char closeBracket);

/**
 * @brief Проверяет начинается ли лексема со скобки.
 * @param lexeme лексема для проверки.
 * @return true, если лексема открывается с любой скобки, false - во всех остальных случаях.
 */
bool isBracketOpenedLexeme (const string& lexeme);

/**
 * @brief Раскручивает стек лексем: переносит из каждого его уровня корректные лексемы в набор корректных лексем.
 * @param lexemeStack стек для раскрутки.
 * @param correctLexemes набор лексем, куда перемещаются лексемы из стека.
 * @details По завершении работы функции стек будет пустым.
 */
void moveLexemesFromStackToSet(stack<string>& lexemeStack, set<string>& correctLexemes);

int main()
{
    // Тестовые пары: входная строка => ожидаемый результат
    vector<pair<string,string>> tests {
        {"",            ""},
        {"a",           "Infinite" },
        {"}](){",       {"(){}"}},
        {"}(){",        {"Infinite"}},
        {"sh(dh)}",     "sh(dh)"},
        {"sh(dh)",      "Infinite"},
        {"]h({hdb}b)[", "Infinite"}};

    for_each(cbegin(tests), cend(tests), [](auto& testPair) {
        auto result = getMaxCorrectString(testPair.first);
        cout << "\"" << testPair.first << "\" => \"" << result << "\"" << (result == testPair.second? " (OK)" : " (Fault)") << endl;
    });

/*
    string s;
    cin >> s;
    cout << getMaxCorrectString(s) << endl;
*/
    return 0;
}

string getMaxCorrectString(const string& s) {
    if (s.empty()) return s;   // Для пустой строки ответ тривиален.

    // Алгоритм разбирает строку на лексемы, используя стек для реализации вложенности.
    // Текущая лексема, которая разбирается в данный момент - находится на вершине стека.
    // Стек инициализируется начальной, "корневой", по-умолчанию - пустой лексемой.
    stack<string> lexemeStack;
    lexemeStack.push("");

    set<string> correctLexemes;   // Найденные корректные, изолированные друг от друга в строке лексемы

    // Двойным прогоном имитируется конкатенация строки, чтобы обнаружить сопоставления скобок из хвоста в начало.
    for (int i = 0; i < 2; ++i) {
        for (auto sym: s)
            switch (sym) {
                case '(': case '{': case '[':         // Начинается новая, вложенная лексема.
                    lexemeStack.push(string{sym}); // Поместить её на вершину стека - теперь работа идёт с нею.
                    break;

                case ')': case '}': case ']':         // Завершение лексемы.
                    if (isBracketOpenedLexeme(lexemeStack.top()) &&
                        areBracketsMatched(lexemeStack.top()[0], sym)) {
                        // Это скобочная лексема и она корректно закрыта парной, закрывающей скобкой.
                        lexemeStack.top() += sym;           // Завершить текущую лексему.
                        // Убрать лексему с вершины стека и прикрепить её к лексеме ниже уровнем.
                        string tmp = lexemeStack.top(); lexemeStack.pop();
                        lexemeStack.top().append(tmp);
                    }
                    else {  // Несмотря на закрывающую скобку - лексема корректно не закрыта.
                        moveLexemesFromStackToSet(lexemeStack, correctLexemes); // Сохранить накопленные лексемы.
                        lexemeStack.push("");                              // Подготовить стек для дальнейшей работы.
                    }
                    break;

                default:    // Все символы кроме скобок - просто принимаются, как заполнители текущей лексемы.
                    lexemeStack.top() += sym;
                    break;
            }
    }

    // После обработки строки - сохранить накопленные лексемы.
    moveLexemesFromStackToSet(lexemeStack, correctLexemes);

    // Найти максимальную корректную лексему.
    auto maxStr = max_element(cbegin(correctLexemes),
                              cend(correctLexemes),
                              [](auto& fst, auto& snd) { return fst.length() < snd.length();});

    // Обработка "увязки" лексем через границу конца/начала строки с образованием бесконечно длинной корректной лексемы:
    // Если корректная лексема бесконечная, то две строки должны давать одинаковый вклад.
    auto substr = maxStr->substr(0, maxStr->length() / 2);
    // Для конкатенации "в хвост" подстрока должна находиться в конце исходной строки
    auto substrIndex = s.rfind(substr);
    if (substrIndex == s.length() - substr.length()) return "Infinite";

    // Условия для Infinite не удовлетворяются - вернуть найденную строку.
    return *maxStr;
}

bool areBracketsMatched(char openBracket, char closeBracket) {
    switch (openBracket) {
        case '(': return closeBracket == ')';
        case '{': return closeBracket == '}';
        case '[': return closeBracket == ']';
        default : return false;
    }
}

bool isBracketOpenedLexeme(const string& lexeme)
{
    if (lexeme.empty()) return false;
    return lexeme[0] == '(' || lexeme[0] == '{' || lexeme[0] == '[';
}

void moveLexemesFromStackToSet(stack<string>& lexemeStack, set<string>& correctLexemes) {
    // На всех уровнях стека, кроме самого нижнего будут находиться "вложенные" лексемы, начинающаиеся со скобки.
    // Раз они не прикрепились к лексемам нижних уровней, значит с ними что-то не в порядке: скобки в них не закрыты.
    // В таком случае, корректная лексема внутри них может быть только начиная с 1 символа: открывающую скобку - не
    // учитывать.
    // "Корневая" лексема (самого нижнего уровня) - переписывается целиком.
    for (; !lexemeStack.empty(); lexemeStack.pop())
        if (lexemeStack.size() > 1)
            correctLexemes.insert(lexemeStack.top().substr(1));
        else
            correctLexemes.insert(lexemeStack.top());
}
