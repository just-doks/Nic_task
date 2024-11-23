#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <typeinfo>
#include <cmath>
#include <fstream>

#include "task.h"

// PRIVATE:
void Task::deleteData() {
	for (int i = 0; i < productsAmount; i++) {
		delete [] A[i];
	}
	delete [] A;
	delete [] b;
	delete [] c;
}


// PUBLIC:
int Task::saveData(const char* string) {
	std::ofstream file (string);
	if (!file.is_open()) {
		std::cerr << "File is not opened" << std::endl;
		return 1;
	}
	file << productsAmount << ' ' << resourcesAmount << std::endl;

	for ( int j = 0; j < resourcesAmount; j++) {
		for (int i = 0; i < productsAmount; i++) {
			file << A[i][j];
			if (i < productsAmount - 1) file << ' ';
		}
		file << std::endl;
	}

	for (int j = 0; j < resourcesAmount; j++) {
		file << b[j];
		if (j < resourcesAmount - 1) file << ' ';
	}
	file << std::endl;

	for (int i = 0; i < productsAmount; i++) {
		file << c[i];
		if (i < productsAmount - 1) file << ' ';
	}			
	file << std::endl;

	file.close();

	return 0;
}



int Task::restoreData(const char* string) {
	std::ifstream file;
	file.open(string);
	if (!file.is_open()) {
		std::cerr << "Error" << std::endl;
		return 1;
	}

	file >> productsAmount >> resourcesAmount;

	A = new float* [productsAmount];
	for (int i = 0; i < productsAmount; i++) {
		A[i] = new float[resourcesAmount];
	}

	for (int j = 0; j < resourcesAmount; j++) {
		for (int i = 0; i < productsAmount; i++) {
			file >> A[i][j];
		}
	}

	b = new float[resourcesAmount];
	for (int j = 0; j < resourcesAmount; j++) {
		file >> b[j];
	}

	c = new float[productsAmount];
	for (int i = 0; i < productsAmount; i++) {
		file >> c[i];
	}			

	file.close();

	return 0;
}



void Task::getData() {
	std::cout << " Input Data: " << std::endl;
	std::cout << "________________________________________" << std::endl;
	std::cout << "  Products amount(i): " << productsAmount << std::endl;
	std::cout << "  Resources amount(j): " << resourcesAmount << std::endl;
	std::cout << "  A[j][i] = (" << std::endl;
	for (int j = 0; j < resourcesAmount; j++) {
		std::cout << "\t";
		for (int i = 0; i < productsAmount; i++) {
			std::cout << A[i][j] << "    ";
		}
		std::cout << std::endl;
	}
	std::cout << "  )" << std::endl << std::endl;
	for (int j = 0; j < resourcesAmount; j++) {
		std::cout << "  b[" << j+1 << "] = " << b[j] << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < productsAmount; i++) {
		std::cout << "  c[" << i+1 << "] = " << c[i] << std::endl;
	}
	//std::cout << typeid(c[0]).name() << endl;
	std::cout << "________________________________________" << std::endl;
	std::cout << std::endl;						
}



void Task::printEquationsSystem() {
	std::cout << "________________________________________" << std::endl;
	std::cout << "\tF = ";
	for (int i = 0; i < productsAmount; i++) {
		std::cout << c[i] << "*X" << i+1 << " ";
		if (i < productsAmount - 1) std::cout << "+ ";
	}
	std::cout << " ->  max" << std::endl << std::endl;
	for (int j = 0; j < resourcesAmount; j++) {
		std::cout << "\t";
		for (int i = 0; i < productsAmount; i++) {
			std::cout << A[i][j] << "*X" << i+1 << " ";
			if (i < productsAmount - 1) std::cout << "+ ";
		}
		std::cout << "<= " << b[j] << std::endl;
	}
	std::cout << "________________________________________" << std::endl;
	std::cout << std::endl;
}



void Task::iteration_Method() {

	// вычисляем максимально возможное количество продкции, 
	// которое можно получить из доступных ресурсов 
	// (для каждого типа отдельно, независимо от других)
	int *max_copies = new int[productsAmount];

	for (int i = 0; i < productsAmount; i++) {
		int min = b[0]/A[i][0];
		for (int j = 1; j < resourcesAmount; j++) {
			if (b[j]/A[i][j] < min) min = b[j]/A[i][j];
		}
		max_copies[i] = min;
	}	

	// создаём матрицу для всех возможных и невозможных комбинаций
	// производства продукции
	int size = max_copies[0] + 1;
	for (int i = 1; i < productsAmount; i++) {
		size *= (max_copies[i] + 1);
	}

	int **Mattrix = new int* [productsAmount];
	for (int i = 0; i < productsAmount; i ++) {
		Mattrix[i] = new int[size];
	}

	// заполняем матрицу
	for (int i = 0; i < productsAmount; i++) {
		int counter = 1;
		for (int j = i + 1; j < productsAmount; j++) {
			counter *= (max_copies[j] + 1);
		} 
		int max_c = max_copies[i];
		int ctr = counter;

		for (int s = 0; s < size; s++) {
			if (ctr < 1) {
				ctr = counter;
				max_c--;
			}
			if (max_c < 0) { 
					max_c = max_copies[i]; 
			}
		
			Mattrix[i][s] = max_c;
			ctr--; 
		}				
	}

	// создаём массив значений выгоды 
	// для каждого набора продукции
	int *F = new int[size];

	// заполняем массив
	for (int s = 0; s < size; s++) {
		F[s] = 0;
		for (int j = 0; j < resourcesAmount; j++) {
			int Z = 0;
			
			for (int i = 0; i < productsAmount; i++) {
				Z += Mattrix[i][s] * A[i][j];
			}

			if (Z > b[j]) {
				break;
			}

			if (j == resourcesAmount - 1) {
				for (int i = 0; i < productsAmount; i++) {
					F[s] += c[i] * Mattrix[i][s];
				}
			}
		}
	}

	// находим максимальное значение
	int max_index = -1;
	for (int s = 0; s < size; s++) {
		if (F[s] == 0) continue;
		if (F[s] > 0 && max_index == -1) {
			max_index = s;
			continue;
		}
		if (F[s] > F[max_index]) max_index = s;
	}

	// Выводим результаты
	for (int i = 0; i < productsAmount; i++) {
		std::cout << "  X" << i + 1 << " = " << Mattrix[i][max_index] << std::endl;
	} 
	std::cout << "  Fmax = " << F[max_index] << std::endl;


	delete [] max_copies;

	for (int i = 0; i < productsAmount; i++) {
		delete [] Mattrix[i];
	}

	delete [] Mattrix;

	delete [] F;
}



void Task::symplex_Method(unsigned int mode) {
	int n = productsAmount + resourcesAmount + 1;
	int m = resourcesAmount + 1;
	float **L = new float* [n];
	for (int i = 0; i < n; i++) {
		L[i] = new float[m];
	}

	// заполнение массива L нулями
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			L[i][j] = 0.f;
		}
	}

	// заносим единицы в позиции основных переменных
	for (int j = 0; j < resourcesAmount; j++) {
		L[j + productsAmount][j] = 1.f;
	}

	// переносим элементы массива A[][] в массив L
	for (int i = 0; i < productsAmount; i++) {
		for (int j = 0; j < resourcesAmount; j++) {
			L[i][j] = A[i][j];
		}
	}

	// заносим элементы массива b[] в массив L
	for (int j = 0; j < resourcesAmount; j++) {
		L[productsAmount + resourcesAmount][j] = b[j];
	}

	// заносим элементы массива c[] в массив L
	for (int i = 0; i < productsAmount; i++) {
		L[i][resourcesAmount] = c[i] * (-1.f);
	}

	// создаём вспомогательный массив для получения
	// в конце исполнения функции значений X и функции F
	int *Xc = new int[n - 1];
	for (int i = 0; i < productsAmount; i++) {
		Xc[i] = -1;
	}
	for (int i = productsAmount; i < n - 1; i++) {
		Xc[i] = i - productsAmount;
	}


	// MAIN BLOCK ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	for (int counter = 0; counter < ((mode == 0) ? 1000 : mode); counter++) {
		// 1
		int iLmin = -1;

		for (int i = 0; i < n - 1; i++) {
			if ((L[i][m-1] < -0.001) && (iLmin == -1)) {
				iLmin = i;
				continue;
			} 
			if ((L[i][m-1] < -0.001) && (iLmin != -1)) {
				if (L[i][m-1] < L[iLmin][m-1]) iLmin = i;
			}
		}

		if (iLmin == -1) {
			break;
		}

		// 2
		int jLmin = -1;
		for (int j = 0; j < m - 1; j++) {
			if (L[iLmin][j] == 0 || 
				((L[iLmin][j] > -0.001) && (L[iLmin][j] < 0.001))) continue;
			if ((L[iLmin][j] < 0) && (L[n-1][j] < 0)) continue;
			if (jLmin == -1) {
				jLmin = j;
				continue;
			}

			if (L[n-1][j]/L[iLmin][j] < L[n-1][jLmin]/L[iLmin][jLmin]) jLmin = j;
		}

		if (jLmin == -1) break;			

		// 3
		float minL = L[iLmin][jLmin];
		for (int i = 0; i < n; i++) {
			L[i][jLmin] = L[i][jLmin]/minL;
		}

		// 4
		
		for (int j = 0; j < m; j++) {

			if (j == jLmin) continue;

			float iL = L[iLmin][j] * (-1.f);

			for (int i = 0; i < n; i++) {
				L[i][j] = L[i][j] + L[i][jLmin] * iL;
			}
		}

		Xc[iLmin] = jLmin;
		Xc[productsAmount + jLmin] = -1;
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////// MAIN BLOCK //

	// Выводим полученные значения X и функции F
	for (int i = 0; i < productsAmount; i++) {
		std::cout << "  X[" << i + 1 << "] = ";
		std::cout << std::fixed << std::setprecision(3) << ( (Xc[i] == -1) ? 0 : L[n-1][Xc[i]] );
		std::cout << std::endl;
	}

	float F = 0;
	for (int i = 0; i < productsAmount; i++) {
		F += c[i] * ( (Xc[i] == -1) ? 0 : L[n-1][Xc[i]] );
	}

	std::cout << std::fixed << std::setprecision(3) << "  F = " << F << std::endl;

	// Очищаем память, выделенную под массивы
	delete [] Xc;

	for (int i = 0; i < n; i++) {
		delete [] L[i];
	}
	delete [] L;
}


void Task::setRandomData() {
	srand(time(0));
	int min = 2;
	int max = 5;
	productsAmount = min + (rand() % (max - min + 1));
	resourcesAmount = min + (rand() % (max - min + 1));


	A = new float* [productsAmount];
	for (int i = 0; i < productsAmount; i++) {
		A[i] = new float [resourcesAmount];
	}

	min = 5;
	max = 10;
	for (int i = 0; i < productsAmount; i++) {
		for (int j = 0; j < resourcesAmount; j++) {
			A[i][j] = static_cast<float>(min + (rand() % (max - min + 1)));
		}
	}

	min = 50;
	max = 100;
	b = new float[resourcesAmount];
	for (int j = 0; j < resourcesAmount; j++) {
		b[j] = static_cast<float>(min + (rand() % (max - min + 1)));
	}

	min = 100;
	max = 200;
	c = new float[productsAmount];
	for (int i = 0; i < productsAmount; i++) {
		c[i] = static_cast<float>(min + (rand() % (max - min + 1)));
	}
}

void Task::setManually() {
	std::cout << "Set prducts amount: ";
	std::cin >> productsAmount;
	std::cout << "Set resources amount: ";
	std::cin >> resourcesAmount;

	b = new float[resourcesAmount];
	std::cout << "Set resource reserves:" << std::endl;
	for (int j = 0; j < resourcesAmount; j++) {
		std::cout << "b[" << j + 1 << "] = ";
		std::cin >> b[j];
	}

	A = new float* [productsAmount];
	for (int i = 0; i < productsAmount; i++) {
		A[i] = new float[resourcesAmount];
	}
	std::cout << "Set resource consumption:(A[resources][products])" << std::endl;
	for (int j = 0; j < resourcesAmount; j++) {
		for (int i = 0; i < productsAmount; i++) {
			std::cout << "A[" << j + 1 << "][" << i + 1 << "] = ";
			std::cin >> A[i][j]; 
		}
	}

	c = new float[productsAmount];
	std::cout << "Set prices of products:" << std::endl;
	for (int i = 0; i < productsAmount; i++) {
		std::cout << "c[" << i + 1 << "] = ";
		std::cin >> c[i];
	}
}



Task::Task() {
	productsAmount = 0;
	resourcesAmount = 0;
	A = NULL;
	b = NULL;
	c = NULL;	
}



Task::~Task() {

	deleteData();
}