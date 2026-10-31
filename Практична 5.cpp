#include <iostream>
using namespace std;

// Статистика роботи алгоритму
struct Stats {
    long long cmp = 0, swaps = 0;
    int depth = 0, calls = 0;
};

// Обмін двох елементів
void change(int a[], int i, int j, Stats& s) {
    if (i == j) return;

    int t = a[i];
    a[i] = a[j];
    a[j] = t;
    s.swaps++;
}

// Завдання 2. Розбиття Ломуто
// 0 - останній, 1 - перший, 2 - медіана трьох
int lomuto(int a[], int l, int r, Stats& s, int mode) {
    int m = l + (r - l) / 2;
    int p = r;

    if (mode == 1) p = l;

    if (mode == 2) {
        s.cmp++;

        if (a[l] < a[m]) {
            s.cmp++;
            if (a[m] < a[r]) p = m;
            else {
                s.cmp++;
                p = a[l] < a[r] ? r : l;
            }
        }
        else {
            s.cmp++;
            if (a[l] < a[r]) p = l;
            else {
                s.cmp++;
                p = a[m] < a[r] ? r : m;
            }
        }
    }

    change(a, p, r, s);

    int pivot = a[r];
    int i = l;

    for (int j = l; j < r; j++) {
        s.cmp++;

        if (a[j] < pivot) {
            change(a, i, j, s);
            i++;
        }
    }

    change(a, i, r, s);
    return i;
}

// Завдання 3. QuickSort Ломуто
void quickL(int a[], int l, int r, Stats& s,
    int mode = 0, int d = 1, bool trace = false) {
    if (l >= r) return;

    s.calls++;
    if (d > s.depth) s.depth = d;

    int pivot = mode == 1 ? a[l] : a[r];
    int p = lomuto(a, l, r, s, mode);

    if (trace)
        cout << "Ломуто: глибина=" << d
        << " межі=" << l << "," << r
        << " опорний=" << (mode == 2 ? a[p] : pivot)
        << " поділ=" << p << '\n';

    quickL(a, l, p - 1, s, mode, d + 1, trace);
    quickL(a, p + 1, r, s, mode, d + 1, trace);
}

// Завдання 4. Розбиття Гоара
int hoare(int a[], int l, int r, Stats& s) {
    int pivot = a[l + (r - l) / 2];
    int i = l - 1;
    int j = r + 1;

    while (true) {
        do {
            i++;
            s.cmp++;
        } while (a[i] < pivot);

        do {
            j--;
            s.cmp++;
        } while (a[j] > pivot);

        if (i >= j) return j;
        change(a, i, j, s);
    }
}

// Завдання 5. QuickSort Гоара
void quickH(int a[], int l, int r, Stats& s,
    int d = 1, bool trace = false) {
    if (l >= r) return;

    s.calls++;
    if (d > s.depth) s.depth = d;

    int pivot = a[l + (r - l) / 2];
    int p = hoare(a, l, r, s);

    if (trace)
        cout << "Гоар: глибина=" << d
        << " межі=" << l << "," << r
        << " опорний=" << pivot
        << " поділ=" << p << '\n';

    quickH(a, l, p, s, d + 1, trace);
    quickH(a, p + 1, r, s, d + 1, trace);
}

// Тристороннє розбиття для масивів із повтореннями
void quick3(int a[], int l, int r, Stats& s, int d = 1) {
    if (l >= r) return;

    s.calls++;
    if (d > s.depth) s.depth = d;

    int pivot = a[l + (r - l) / 2];
    int lt = l, i = l, gt = r;

    while (i <= gt) {
        s.cmp++;

        if (a[i] < pivot) {
            change(a, lt, i, s);
            lt++;
            i++;
        }
        else {
            s.cmp++;

            if (a[i] > pivot) {
                change(a, i, gt, s);
                gt--;
            }
            else i++;
        }
    }

    quick3(a, l, lt - 1, s, d + 1);
    quick3(a, gt + 1, r, s, d + 1);
}

// Запуск одного з п'яти алгоритмів
void run(int a[], int n, Stats& s, int k, bool trace = false) {
    if (k == 0) quickL(a, 0, n - 1, s, 0, 1, trace);
    if (k == 1) quickH(a, 0, n - 1, s, 1, trace);
    if (k == 2) quickL(a, 0, n - 1, s, 1);
    if (k == 3) quickL(a, 0, n - 1, s, 2);
    if (k == 4) quick3(a, 0, n - 1, s);
}

// Перевірка правильності сортування
bool correct(const int before[], const int after[], int n) {
    for (int i = 1; i < n; i++)
        if (after[i - 1] > after[i]) return false;

    for (int i = 0; i < n; i++) {
        int x = 0, y = 0;

        for (int j = 0; j < n; j++) {
            if (before[j] == before[i]) x++;
            if (after[j] == before[i]) y++;
        }

        if (x != y) return false;
    }

    return true;
}

// Порівняння п'яти алгоритмів
void compare(const int source[], int n, const char* title) {
    const char* names[5] = {
        "Ломуто останній", "Гоар середній",
        "Ломуто перший", "Ломуто медіана 3",
        "Три частини"
    };

    cout << "\n" << title << " (n=" << n << ")\n";

    for (int k = 0; k < 5; k++) {
        int a[100] = {};
        Stats s;

        for (int i = 0; i < n; i++)
            a[i] = source[i];

        run(a, n, s, k);

        cout << names[k]
            << ": порівнянь=" << s.cmp
            << ", обмінів=" << s.swaps
            << ", глибина=" << s.depth
            << ", викликів=" << s.calls
            << "; " << (correct(source, a, n)
                ? "Успішно" : "ПОМИЛКА") << '\n';
    }
}

// Виведення масиву
void printArray(const int a[], int n) {
    for (int i = 0; i < n; i++)
        cout << a[i] << ' ';

    cout << '\n';
}

// Виведення результату та статистики
void result(const int source[], const int a[], int n, const Stats& s) {
    cout << "Результат: ";
    printArray(a, n);

    cout << "Порівнянь=" << s.cmp
        << ", обмінів=" << s.swaps
        << ", глибина=" << s.depth
        << ", викликів=" << s.calls
        << "; " << (correct(source, a, n)
            ? "Успішно" : "ПОМИЛКА") << '\n';
}

int main() {
    const int n = 15;

    int variant[n] = {
        20, 33, 7, 25, 11, 3, 28, 14,
        1, 22, 9, 30, 5, 17, 12
    };

    int a[100] = {};
    Stats s;

    cout << "Практична №5. QuickSort. Варіант 7\n";

    // Завдання 1
    cout << "\n===== Завдання 1. Підготовка даних =====\n";
    cout << "Початковий масив: ";
    printArray(variant, n);
    cout << "Статистика: порівняння, обміни, глибина, виклики\n";

    // Завдання 2
    cout << "\n===== Завдання 2. Розбиття Ломуто =====\n";

    for (int i = 0; i < n; i++) a[i] = variant[i];
    s = Stats{};

    int p = lomuto(a, 0, n - 1, s, 0);

    cout << "Опорний=" << a[p]
        << ", позиція=" << p << "\nПісля розбиття: ";
    printArray(a, n);

    // Завдання 3
    cout << "\n===== Завдання 3. QuickSort Ломуто =====\n";

    for (int i = 0; i < n; i++) a[i] = variant[i];
    s = Stats{};

    quickL(a, 0, n - 1, s);
    result(variant, a, n, s);

    // Завдання 4
    cout << "\n===== Завдання 4. Розбиття Гоара =====\n";

    for (int i = 0; i < n; i++) a[i] = variant[i];
    s = Stats{};

    p = hoare(a, 0, n - 1, s);

    cout << "Межа розбиття=" << p << "\nПісля розбиття: ";
    printArray(a, n);

    // Завдання 5
    cout << "\n===== Завдання 5. QuickSort Гоара =====\n";

    for (int i = 0; i < n; i++) a[i] = variant[i];
    s = Stats{};

    quickH(a, 0, n - 1, s);
    result(variant, a, n, s);

    // Завдання 6
    cout << "\n===== Завдання 6. Трасування рекурсії =====\n";

    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < n; i++)
            a[i] = variant[i];

        s = Stats{};

        cout << (k == 0 ? "\nЛомуто:\n" : "\nГоар:\n");
        run(a, n, s, k, true);
        result(variant, a, n, s);
    }

    // Завдання 7
    cout << "\n===== Завдання 7. Автоматичні тести =====\n";

    int tests[12][15] = {
        {}, {8}, {2,1}, {1,2}, {3,3,3},
        {5,4,3,2,1}, {1,2,3,4,5},
        {2,1,2,1,2}, {-3,0,-1,2,1},
        {9,1,9,3,9}, {0,0,1,0,1},
        {20,33,7,25,11,3,28,14,1,22,9,30,5,17,12}
    };

    int sizes[12] = {
        0,1,2,2,3,5,5,5,5,5,5,15
    };

    int passed = 0;

    for (int t = 0; t < 12; t++) {
        bool ok = true;

        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < sizes[t]; i++)
                a[i] = tests[t][i];

            s = Stats{};
            run(a, sizes[t], s, k);

            if (!correct(tests[t], a, sizes[t]))
                ok = false;
        }

        if (ok) passed++;

        cout << "Тест " << t + 1 << ": "
            << (ok ? "Успішно" : "ПОМИЛКА") << '\n';
    }

    cout << "Пройдено " << passed << "/12\n";

    // Завдання 8
    cout << "\n===== Завдання 8. Порівняльний експеримент =====\n";

    const char* titles[4] = {
        "Зростання", "Спадання",
        "Однакові", "Змішані"
    };

    for (int kind = 0; kind < 4; kind++) {
        for (int i = 0; i < 100; i++) {
            if (kind == 0) a[i] = i;
            else if (kind == 1) a[i] = 100 - i;
            else if (kind == 2) a[i] = 7;
            else a[i] = (i * 37 + 13) % 100;
        }

        compare(a, 100, titles[kind]);
    }

    // Дослідження вибору опорного елемента
    cout << "\n===== Дослідження вибору опорного елемента =====\n";
    compare(variant, n, "Порівняння п'яти способів сортування. Варіант 7");

    return 0;
}
