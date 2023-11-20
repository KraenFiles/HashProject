#include <iostream>
#include <sstream>
#include <random>
#include "Config.h"
#include "HashTable.h"

#if TESTS
#include <gtest/gtest.h>
#endif

using std::string;
using std::cout;
using std::cin; 

int main(int argc, char **argv)
{

#if TESTS
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	if (result)
		return -1;
#endif

	HashTable<int> table;

	bool exit = false;
	do
	{
		cout << "Выберите действие\n(1-Ввод значений, 2-Генерация значений, 3-Удаление значения 4-Вывод значений, 5-Сохранить в файл, 6-Выход): ";
		string change;
		std::getline(cin, change);

		switch (std::atoi(change.c_str()))
		{
		case 1:
		{
			do
			{
				cout << "Введите ключ и значение: ";
				string text;
				std::getline(cin, text);
				size_t pos = 0;
				if ((pos = text.find(" ")) != string::npos)
				{
					std::pair<string, int> value;
					value.first = text.substr(0, pos);
					value.second = std::atoi(text.substr(pos, text.length()).c_str());
					if (Hash::CheckKey(value.first))
						table.Add(value.first, value.second);
					else
						cout << "Неправильный ключ\n";
				}
				else
				{
					break;
				}
			} while (!std::cin.bad());
			break;
		}

		case 2:
		{
			cout << "Введите количество значений для генерации: ";
			int generateCount = 0;
			cin >> generateCount;
			std::random_device rand;
    		std::default_random_engine rand_engine(rand());
    		std::uniform_int_distribution<int> value_dist(0, 1000);
			for(int i = 0; i < generateCount; ++i)
			{
				string key = Hash::KeyGenerate();
				int value = value_dist(rand_engine);
				try
				{
					table.Add(key, value);
				}
				catch(const std::overflow_error::exception & ex)
				{
					cout << "Таблица переполнена!" << "\n";
					break;
				}
			}
			break;
		}
		case 3:
			{
				cout << "Введите ключ или позицию(A-для полной очистки): ";
				string text;
				std::getline(cin, text);
				if(Hash::CheckKey(text))
				{
					table.Delete(text);
				}
				else
				{
					if((text[0] == 'A' || text[0] == 'a') && text.length() == 1)
					{
						table.ClearAll();
					}
					else
					{
						int pos = 0;
						try
						{
							pos = std::atoi(text.c_str());
						}
						catch(const std::exception& e)
						{
							std::cerr << "Неправильный путь или значение\n";
							break;
						}
						table.Delete(pos);
					}
					
				}
				break;
			}
		case 4:
			table.ShowTable();
			break;
		case 5:
		{
			cout << "Укажите путь: ";
			string path;
			std::getline(cin, path);
			table.LoadInFile(path);
			break;
		}	
		case 6:
			exit = true;
			break;

		default:
			cout << "Неправильное значение\n";
			break;
		}

	} while (!exit);

	return 0;
}
