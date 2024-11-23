#ifndef TASK_H
#define TASK_H

class Task {
	private:
		int productsAmount;
		int resourcesAmount;
		float **A;
		float *b;
		float *c;

		void deleteData();

	public:
		int saveData(const char* string);

		int restoreData(const char* string);

		void getData();

		void printEquationsSystem();

		void iteration_Method();

		void symplex_Method(unsigned int mode);

		void setRandomData();

		void setManually();

		Task();

		~Task();
};

#endif /*TASK_H*/