#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

// РАМКА
int get_visible_len(const char *text) {
    int count = 0;
    while (*text) {
        if ((*text & 0xc0) != 0x80) count++;
        text++;
    }
    return count;
}

void print_row(const char *text, int width) {
    printf("\xe2\x95\x91 %s", text);
    int visible = get_visible_len(text);
    for (int i = 0; i < width - visible; i++) printf(" ");
    printf(" \xe2\x95\x91\n");
}

void print_about() {
    int w = 68;
    printf("\xe2\x95\x94"); for(int i=0; i<w+2; i++) printf("\xe2\x95\x90"); printf("\xe2\x95\x97\n");
    print_row("Навчальна дисципліна: БМТП", w);
    print_row("Лабораторна робота \xe2\x84\x96\x37. Завдання 7.1 (Варіант 9)", w);
    print_row("Виконав: студент 1-го курсу групи КБ-25", w);
    print_row("Спеціальність: Кібербезпека та захист інформації", w);
    print_row("ЦНТУ (Центральноукраїнський національний технічний університет)", w);
    print_row("Ім'я: Бурейко Олександр Григорович", w);
    printf("\xe2\x95\x9a"); for(int i=0; i<w+2; i++) printf("\xe2\x95\x90"); printf("\xe2\x95\x9d\n\n");
}

// ЗАТРИМКА ПЕРЕД ЗАВЕРШЕННЯМ
void wait_for_exit() {
    printf("\nНатисніть Enter для завершення програми...");
    fflush(stdout); // Примусово виводимо текст на екран

#ifdef _WIN32
    // Для Windows: очищаємо буфер вводу від попередніх натискань і чекаємо на новий ввід
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hStdin);
    wchar_t dummy[2];
    DWORD read;
    // Програма зависне тут, поки користувач не натисне клавішу
    ReadConsoleW(hStdin, dummy, 1, &read, NULL);
#else
    // Для Linux: просто чекаємо на символ нового рядка (Enter)
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Очищення залишків
    getchar(); // Безпосередньо пауза
#endif
}

// ЛОГІКА
int is_consonant(wchar_t c) {
    c = towlower(c);
    const wchar_t consonants[] = L"бвгґджзйклмнпрстфхцчшщ";
    for (int i = 0; consonants[i] != L'\0'; i++) {
        if (c == consonants[i]) return 1;
    }
    return 0;
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
#else
    setlocale(LC_ALL, "");
#endif

    print_about();

    wchar_t word[1024];
    size_t len = 0;

    printf("Введіть слово (без проміжків, кирилиця, закінчується на %%, ~ або *): ");
    fflush(stdout);

#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD charsRead;
    if (!ReadConsoleW(hStdin, word, 1024, &charsRead, NULL)) {
        printf("Помилка читання вводу Windows.\n");
        wait_for_exit();
        return 1;
    }
    while (charsRead > 0 && (word[charsRead - 1] == L'\r' || word[charsRead - 1] == L'\n')) {
        charsRead--;
    }
    word[charsRead] = L'\0';
    len = charsRead;
#else
    char input[1024];
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Помилка читання вводу.\n");
        wait_for_exit();
        return 1;
    }
    input[strcspn(input, "\r\n")] = '\0';
    size_t converted = mbstowcs(word, input, 1024);
    if (converted == (size_t)-1) {
        printf("Помилка обробки кирилиці.\n");
        wait_for_exit();
        return 1;
    }
    len = converted;
#endif

    // валідація
    if (wcschr(word, L' ') != NULL) {
        printf("Помилка: слово має бути без проміжків.\n");
        wait_for_exit();
        return 1;
    }

    if (len == 0 || (word[len - 1] != L'%' && word[len - 1] != L'~' && word[len - 1] != L'*')) {
        printf("Помилка: слово повинно закінчуватися на %%, ~ або *.\n");
        wait_for_exit();
        return 1;
    }

    // обчислення
    int ma_count = 0;
    for (size_t i = 0; i < len - 1; i++) {
        if (towlower(word[i]) == L'м' && towlower(word[i+1]) == L'а') {
            ma_count++;
        }
    }

    // вивід результатів
    if (ma_count > 0) {
        printf("Кількість складів \"ма\": %d\n", ma_count);
    } else {
        printf("Склад \"ма\" відсутній. Приголосні літери: ");
        for (size_t i = 0; i < len; i++) {
            if (is_consonant(word[i])) {
#ifdef _WIN32
                char utf8_buf[MB_CUR_MAX + 1];
                int bytes = WideCharToMultiByte(CP_UTF8, 0, &word[i], 1, utf8_buf, sizeof(utf8_buf), NULL, NULL);
                if (bytes > 0) {
                    utf8_buf[bytes] = '\0';
                    printf("%s ", utf8_buf);
                }
#else
                char utf8_char[MB_CUR_MAX + 1];
                int bytes = wctomb(utf8_char, word[i]);
                if (bytes > 0) {
                    utf8_char[bytes] = '\0';
                    printf("%s ", utf8_char);
                }
#endif
            }
        }
        printf("\n");
    }

    // зупинка консолі перед фактичним завершенням програми
    wait_for_exit();

    return 0;
}
