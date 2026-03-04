#include <stdio.h>
#include <stdlib.h>
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
    print_row("Лабораторна робота \xe2\x84\x96\x37. Завдання 7.2 (Варіант 9)", w);
    print_row("Виконав: студент 1-го курсу групи КБ-25", w);
    print_row("Спеціальність: Кібербезпека та захист інформації", w);
    print_row("ЦНТУ (Центральноукраїнський національний технічний університет)", w);
    print_row("Ім'я: Бурейко Олександр Григорович", w);
    printf("\xe2\x95\x9a"); for(int i=0; i<w+2; i++) printf("\xe2\x95\x90"); printf("\xe2\x95\x9d\n\n");
}

// БЛОК ОЧИЩЕННЯ ТА ЗАТРИМКИ

// Функція для очищення залишків вводу (\n або помилкових літер)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait_for_exit() {
    printf("\nНатисніть Enter для завершення програми...");
    fflush(stdout);

#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hStdin);
    wchar_t dummy[2];
    DWORD read;
    ReadConsoleW(hStdin, dummy, 1, &read, NULL);
#else
    clear_input_buffer(); // у Linux просто чекаємо на новий рядок
#endif
}

// ЛОГІКА
int main(void) {
    // налаштування UTF-8 для виводу тексту
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
#else
    setlocale(LC_ALL, "");
#endif

    print_about();

    int arr[15]; // масив для 15 чисел
    int i = 0;

    printf("Введіть 15 натуральних чисел (більших за 0):\n");

    // цикл зчитування чисел
    while (i < 15) {
        printf("Елемент [%d]: ", i + 1);

        // перевірка, чи користувач ввів саме число (scanf має повернути 1)
        if (scanf("%d", &arr[i]) != 1) {
            printf("Помилка: введено не число. Спробуйте ще раз.\n");
            clear_input_buffer(); // очищаємо сміття (літери тощо), щоб scanf не зациклився
            continue;
        }

        // перевірка, чи є число натуральним
        if (arr[i] <= 0) {
            printf("Помилка: число має бути натуральним (> 0). Спробуйте ще раз.\n");
            continue;
        }

        i++; // переходимо до наступного елемента лише якщо введення коректне
    }

    // після успішного введення всіх чисел очищаємо буфер від останнього натискання Enter,
    // щоб наприкінці програми wait_for_exit не спрацював хибно.
    clear_input_buffer();

    // ініціалізуємо мінімум і максимум першим елементом масиву
    int min = arr[0];
    int max = arr[0];

    // проходимо по масиву (починаючи з другого елемента) для пошуку min і max
    for (int j = 1; j < 15; j++) {
        if (arr[j] > max) {
            max = arr[j];
        }
        if (arr[j] < min) {
            min = arr[j];
        }
    }

    // обчислюємо суму та різницю
    int sum = max + min;
    int diff = max - min;

    // вивід результатів
    printf("\n--- Результати ---\n");
    printf("Найбільший елемент: %d\n", max);
    printf("Найменший елемент: %d\n", min);
    printf("Сума (max + min): %d\n", sum);
    printf("Різниця (max - min): %d\n", diff);

    wait_for_exit();
    return 0;
}
