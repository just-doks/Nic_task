#include <iostream>

#include "task.h"

int main (int argc, char * argv[]) {

	std::cout << "======================================================\n";
	Task *task = new Task();

	int a = 0;

	std::cout << "Set Data:" << std::endl;
	for (int i = 3; i >= 0; i--) {

		int flag = 0;

		if (i == 0) {
			std::cout << "Program shutdown." << std::endl;
			delete task;
			return 0;
		}

		std::cout << "(:" << i << ") Set Randomly(1), restore from file(2) or set manually(3): ";
		std::cin >> a;

		switch(a) {
			case 1: {
				task->setRandomData();
				break;
			}
			case 2: {
				if (task->restoreData("save.txt") != 0) {
					std::cout << "Unable to restore. Select another option." << std::endl;
					flag = 1;
				}
				break;
			}
			case 3: {
				task->setManually();
				break;
			}
			default: {
				std::cout << "Try again" << std::endl;
				flag = 1;
				break;
			}
		}

		if (flag == 0) break;		
	}

	task->getData();
	task->printEquationsSystem();

	std::cout << "\nChoose method:\n";

	for (int i = 5; i >= 0; i--) {
		int flag = 0;

		if (i == 0) {
			std::cout << "Program shutdown." << std::endl;
			delete task;
			return 0;
		}
		std::cout << "(:" << i << ") Iteration(1), symplex(2), symplex-short(3), exit(0): ";
		std::cin >> a;

		switch(a) {
			case 0: {
				flag = 1;
				break;
			}
			case 1: {
				task->iteration_Method();
				break;
			}
			case 2: {
				task->symplex_Method(0);
				break;
			}
			case 3: {
				task->symplex_Method(1);
				break;
			}
			default: {
				break;
			}
		}

		if (flag == 1) break;
	}

	std::cout << "Do you want to save data? Yes(1), No(anything else): ";
	std::cin >> a;
	switch (a) {
		case 1: {
			if(task->saveData("save.txt") != 0) {
				std::cout << "Unable to save data" << std::endl;
			}
			break;
		}
		case 2: {
			break;
		}
		default: {
			break;
		}
	}

	delete task;
	std::cout << "======================================================\n";

	return 0;
}