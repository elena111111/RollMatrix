#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

class RollGenerator {
public:
	RollGenerator(const int row, const int col) : n_row(row), n_col(col) {
		border.right = col - 1;
		border.down = row - 1;
		border.left = 0;
		border.top = 1;
		i = 0, j = -1;
		way = right;
		cnt = 1;
	}

	const pair <int, int> GetNextIJ() {
		if (way == right) {
			if (j == border.right) {
				border.right--;
				way = down;
			}
			else j++;
		}

		if (way == down) {
			if (i == border.down) {
				border.down--;
				way = left;
			}
			else i++;
		}

		if (way == left) {
			if (j == border.left) {
				border.left++;
				way = top;
			}
			else j--;
		}

		if (way == top) {
			if (i == border.top) {
				j++;
				border.top++;
				way = right;
			}
			else i--;
		}

		return make_pair(i, j);
	}

	const bool IsEnd() {
		if (n_col*n_row >= cnt) {
			cnt++;
			return false;
		}
		else {
			return true;
		}
	}

private:
	const int n_row, n_col;
	int i, j;
	enum { right, down, left, top } way;
	struct Border {
		int right, down, left, top;
	} border;
	int cnt;
};

class RollMatrix {
public:
	RollMatrix(const int row, const int col) : res(row, vector<int>(col, 0)), Gen(row, col), cnt(1), n_row(row), n_col(col) {
		while (!Gen.IsEnd()) {
			pair<int, int> p = Gen.GetNextIJ();
			res[p.first][p.second] = cnt++;
		}
	}

	const vector< vector <int> > GetMatrix() {
		return res;
	}

	int GetElement(int i, int j) {
		return res[i][j];
	}

	const void PrintMatrix() {
		for (int i = 0; i < n_row; i++) {
			for (int j = 0; j < n_col; j++) {
				cout << setw(2) << res[i][j] << " ";
			}
			cout << endl;
		}
	}

	const bool test(vector< vector <int> > que) {
		for (int i = 0; i < n_row; i++) {
			for (int j = 0; j < n_col; j++) {
				if (que[i][j] != res[i][j]) return false;
			}
		}
		return true;
	}

private:
	vector< vector <int> > res;
	RollGenerator Gen;
	int cnt, n_row, n_col;
};



int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	int n = 7, m = 7;

	if (strcmp(argv[1], "--help") == 0) cout << "Это программа строит спиральную матрицу." << endl <<
		"Может работать со следующими параметрами:" << endl <<
		"--test - запуск для разных размерностей матрицы и сравнение с эталонным результатом. Вывод результата." << endl <<
		"--gen-test - генерация тестовых случаев от 1x1 до указанной размерности." << endl <<
		"Можно единоразово запустить программу с указанием размерности матрицы n и m." << endl;

	else if (strcmp(argv[1], "--test") == 0) {
		ifstream fileIn("E:\\test\\inputMatrix.txt");
		ifstream fileOut("E:\\test\\outputMatrix.txt");
		if (!fileIn.is_open() || !fileOut.is_open()) cout << "Ошибка при открытии файла." << endl;
		while (!fileIn.eof()) {
			fileIn >> n >> m;
			RollMatrix M(n, m);
			vector< vector <int> > tmp(n, vector<int>(m, 0));
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					fileOut >> tmp[i][j];
				}
			}
			cout << n << " " << m << endl;
			if (M.test(tmp)) {
				cout << "Ok" << endl;
				M.PrintMatrix();
			}
			else {
				cout << "Неправильный ответ" << endl;
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						cout << "Эталонный результат:" << endl << setw(2) << tmp[i][j] << " " << endl
							<< "Полученный результат:" << endl; M.PrintMatrix();
					}
					cout << endl;
				}
			}
			cout << endl;
		}
		fileIn.close();
		fileOut.close();
	}

	else if (strcmp(argv[1], "--gen-test") == 0) {	//запишем тесты в файл
		ofstream fileIn("E:\\test\\inputMatrix.txt");
		ofstream fileOut("E:\\test\\outputMatrix.txt");
		fileIn.clear();
		fileOut.clear();
		if (!fileIn.is_open() || !fileOut.is_open()) cout << "Ошибка при открытии файла" << endl;
		cout << "Введите максимальное желаемое количество строк и столбцов для теста:" << endl;
		cin >> n >> m;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= m; j++) {
				fileIn << i << " " << j;
				if (i != n || j != m) fileIn << endl;
				RollMatrix M(i, j);
				for (int tmp_i = 0; tmp_i < i; tmp_i++) {
					for (int tmp_j = 0; tmp_j < j; tmp_j++) {
						fileOut << M.GetElement(tmp_i, tmp_j) << " ";
					}
					fileOut << endl;
				}

			}
		}

		fileIn.close();
		fileOut.close();
	}
	else {
		n = atoi(argv[1]), m = atoi(argv[2]);
		RollMatrix M(n, m);
		M.PrintMatrix();
	}

	return 0;
}