#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Login.h"

using namespace std;

void Login::ventanaLogin(char* ipChar, int& portNumber, string& name)
{


	bool ok = false;
	cout << "--- Log in ---" << endl;
	cout << "Por favor ingrese los siguientes datos: " << endl;

	//getline(cin,name);

	while (!ok){

		cout << "Nombre de usuario:" << endl;
		cin >> name;
		if (!cin){
			cout << "Reingrese el nombre de usuario:" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			ok = false;
		}else
			ok = true;
	}
	ok = false;

	cout << "Ip del servidor: " << endl;
	cin >> ipChar;
	while (!ok){
		cout << "Puerto: " << endl;
		cin >> portNumber;
		if (!cin){
			cout << "Error: Debe ingresar un número" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			ok = false;
		}else
			ok = true;
	}
}
