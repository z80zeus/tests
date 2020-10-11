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

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>

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
 * @brief Раскручивает стек: переносит из каждого его уровня корректные лексемы в набор корректных лексем.
 * @param lexemeStack стек для раскрутки.
 * @param correctLexemes набор лексем, куда перемещаются лексемы из стека.
 * @details По завершении работы функции стек станет пустым.
 */
void moveLexemesFromStackToSet(stack<string>& lexemeStack, set<string>& correctLexemes);

int main()
{
    string s;
    cin >> s;
    cout << getMaxCorrectString(s) << endl;

    return 0;
}

string getMaxCorrectString(const string& s) {
    stack<string> lexemeStack;    // Стэк лексем.
    string currentLexeme;         // Здесь будет посимвольно "набираться" текущая лексема.
    set<string> correctLexemes;   // Найденные правильные, изолированные друг от друга лексемы

    for (auto sym: s)
        switch (sym) {
            case '(': case '{': case '[':           // Начинается новая вложенная лексема
                lexemeStack.push(currentLexeme);    // Текущую - сохранить в стек.
                currentLexeme = sym;
                break;

            case ')': case '}': case ']':           // Завершение лексемы
                if (isBracketOpenedLexeme(currentLexeme) && areBracketsMatched(currentLexeme[0], sym)) {
                    currentLexeme += sym;
                    currentLexeme = lexemeStack.top().append(currentLexeme);
                    lexemeStack.pop();
                    break;
                };
                moveLexemesFromStackToSet(lexemeStack, correctLexemes);
                break;

            default:
                currentLexeme += sym;
                break;
        }

    moveLexemesFromStackToSet(lexemeStack, correctLexemes);
    return "Infinite";
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

}