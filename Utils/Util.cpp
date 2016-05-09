#include <time.h>
#include "Util.h"
#include <cstring>

using namespace std;

// Obtenemos la fecha con el siguiente formato -> YYYY-MM-DD.HH:mm:ss
string Util::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


bool Util::sonDigitos(char* str){
	for (unsigned int i = 0; i < strlen (str); i++) {
		if (! isdigit (str[i])) {
			return false;
		}
	}
	return true;
}

