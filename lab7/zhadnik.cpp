#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size(); // O(1) по памяти: получаем размер вектора
        int sum = 0; // O(1) по памяти: переменная для хранения суммы

        // Подсчет общей суммы платьев
        for (int num : machines) {
            sum += num; // O(n) по времени
        }
        // Проверяем, делится ли сумма на количество машин
        if (sum % n != 0) {
            return -1; //
        }
        int avg = sum / n; //  O(1) по памяти: среднее число платьев для каждой машины
        vector<int> leftSums(n, 0); // O(n) по памяти: массив для накопленных левых сумм
        vector<int> rightSums(n, 0); // O(n) по памяти: массив для накопленных правых сумм
        // Вычисляем накопленные суммы слева
        for (int i = 1; i < n; ++i) {
            leftSums[i] = leftSums[i - 1] + machines[i - 1]; // O(n) по времени
        }
        // Вычисляем накопленные суммы справа
        for (int i = n - 2; i >= 0; --i) {
            rightSums[i] = rightSums[i + 1] + machines[i + 1]; // O(n) по времени
        }
        int maxMoves = 0; // O(1) по памяти: переменная для хранения максимального числа движений
        // Вычисляем минимальное количество движений
        for (int i = 0; i < n; ++i) {
            int expectedLeft = i * avg; //O(1) по памяти
            int expectedRight = (n - i - 1) * avg; // O(1) по памяти
            int leftDeficit = max(0, expectedLeft - leftSums[i]); // O(1) по памяти
            int rightDeficit = max(0, expectedRight - rightSums[i]); // O(1) по памяти
            maxMoves = max(maxMoves, leftDeficit + rightDeficit); // O(1) по памяти
        }
        return maxMoves;
    }
};

/*
Общий подсчет временной и памятной сложности:
- Временная сложность:
  1. Подсчет суммы: O(n)
  2. Вычисление левых и правых накопленных сумм: O(n) + O(n) = O(n)
  3. Цикл вычисления движений: O(n)
  ИТОГО: O(n), так как все операции линейные.

- Памятная сложность:
  1. Массивы leftSums и rightSums: O(n) + O(n) = O(n)
  2. Константные переменные: O(1)
  ИТОГО: O(n), так как доминирующим фактором является использование массивов.
*/
