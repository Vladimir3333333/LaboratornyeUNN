#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define N 10000               // размер массива
#define RANGE 10000           // диапазон значений для counting sort
#define RUNS 5                // количество прогонов для усреднения времени

// Cами сортировки

void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        int tmp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = tmp;
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return i + 1;
}

void quick_sort_rec(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_rec(arr, low, pi - 1);
        quick_sort_rec(arr, pi + 1, high);
    }
}

void quick_sort(int arr[], int n) {
    quick_sort_rec(arr, 0, n - 1);
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void merge_sort_rec(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_rec(arr, l, m);
        merge_sort_rec(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge_sort(int arr[], int n) {
    merge_sort_rec(arr, 0, n - 1);
}

void counting_sort(int arr[], int n, int range) {
    int* output = (int*)calloc(n, sizeof(int));
    int* count = (int*)calloc(range, sizeof(int));

    for (int i = 0; i < n; i++)
        count[arr[i]]++;

    int idx = 0;
    for (int i = 0; i < range; i++)
        while (count[i]-- > 0)
            output[idx++] = i;

    for (int i = 0; i < n; i++)
        arr[i] = output[i];

    free(output);
    free(count);
}

void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Доп функции

void copy_array(int src[], int dst[], int n) {
    memcpy(dst, src, n * sizeof(int));
}

bool is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}

long long array_sum(int arr[], int n) {
    long long s = 0;
    for (int i = 0; i < n; i++)
        s += arr[i];
    return s;
}

void fill_sorted(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
}

void fill_reverse(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
}

void fill_random(int arr[], int n, int range) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % range;
}

// Тестики

typedef void (*sort_func_t)(int[], int);

double time_sort(sort_func_t sort, int arr[], int n, long long original_sum) {
    double total = 0.0;
    for (int run = 0; run < RUNS; run++) {
        int* test_arr = (int*)malloc(n * sizeof(int));
        copy_array(arr, test_arr, n);

        clock_t start = clock();
        sort(test_arr, n);
        clock_t end = clock();

        if (!is_sorted(test_arr, n) || array_sum(test_arr, n) != original_sum) {
            printf("ERROR: Sort failed correctness check!\n");
            free(test_arr);
            return -1.0;
        }

        total += ((double)(end - start)) / CLOCKS_PER_SEC;
        free(test_arr);
    }
    return total / RUNS;
}

int main() {
    srand((unsigned int)time(NULL));

    struct {
        const char* name;
        sort_func_t func;
        bool needs_range;
    } sorts[] = {
        {"Bubble Sort",       (sort_func_t)bubble_sort,       false},
        {"Selection Sort",    (sort_func_t)selection_sort,    false},
        {"Quick Sort",        (sort_func_t)quick_sort,        false},
        {"Merge Sort",        (sort_func_t)merge_sort,        false},
        {"Counting Sort",     (sort_func_t)counting_sort,     true},
        {"Insertion Sort",    (sort_func_t)insertion_sort,    false}
    };
    int num_sorts = sizeof(sorts) / sizeof(sorts[0]);

    int* original = (int*)malloc(N * sizeof(int));
    long long sum;

    // Упорядоченный массив
    printf("=== Sorted Array ===\n");
    fill_sorted(original, N);
    sum = array_sum(original, N);
    for (int i = 0; i < num_sorts; i++) {
        double t;
        if (sorts[i].needs_range) {
            int* temp = (int*)malloc(N * sizeof(int));
            copy_array(original, temp, N);
            clock_t start = clock();
            for (int r = 0; r < RUNS; r++) {
                copy_array(original, temp, N);
                counting_sort(temp, N, RANGE);
            }
            clock_t end = clock();
            t = ((double)(end - start)) / CLOCKS_PER_SEC / RUNS;
            free(temp);
        }
        else {
            t = time_sort(sorts[i].func, original, N, sum);
        }
        if (t >= 0)
            printf("%-15s: %.6f sec\n", sorts[i].name, t);
    }

    // Обратно упорядоченный массив
    printf("\n=== Reverse Sorted Array ===\n");
    fill_reverse(original, N);
    sum = array_sum(original, N);
    for (int i = 0; i < num_sorts; i++) {
        double t;
        if (sorts[i].needs_range) {
            int* temp = (int*)malloc(N * sizeof(int));
            copy_array(original, temp, N);
            clock_t start = clock();
            for (int r = 0; r < RUNS; r++) {
                copy_array(original, temp, N);
                counting_sort(temp, N, RANGE);
            }
            clock_t end = clock();
            t = ((double)(end - start)) / CLOCKS_PER_SEC / RUNS;
            free(temp);
        }
        else {
            t = time_sort(sorts[i].func, original, N, sum);
        }
        if (t >= 0)
            printf("%-15s: %.6f sec\n", sorts[i].name, t);
    }

    // Случайный массив
    printf("\n=== Random Array ===\n");
    fill_random(original, N, RANGE);
    sum = array_sum(original, N);
    for (int i = 0; i < num_sorts; i++) {
        double t;
        if (sorts[i].needs_range) {
            int* temp = (int*)malloc(N * sizeof(int));
            copy_array(original, temp, N);
            clock_t start = clock();
            for (int r = 0; r < RUNS; r++) {
                copy_array(original, temp, N);
                counting_sort(temp, N, RANGE);
            }
            clock_t end = clock();
            t = ((double)(end - start)) / CLOCKS_PER_SEC / RUNS;
            free(temp);
        }
        else {
            t = time_sort(sorts[i].func, original, N, sum);
        }
        if (t >= 0)
            printf("%-15s: %.6f sec\n", sorts[i].name, t);
    }

    free(original);
    return 0;
}