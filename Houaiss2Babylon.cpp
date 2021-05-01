/** Conversor de Houaiss para Babylon

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2008-04

@desc Permite que os arquivos do Houaiss sejam usados para construir
um dicion�rio do Babylon atrav�s do Babylon Builder.

Linque:
Houaiss: http://www.dicionariohouaiss.com.br
Babylon: http://www.babylon.com
Babylon Builder: http://www.babylon.com/display.php?id=15&tree=3&level=2

Obs.:
Esse programa n�o se destina � pirataria. Ele n�o fornece o dicion�rio, 
que devem ser adquirido legalmente. Sua �nica fun��o � permitir o uso 
de um �timo dicion�rios independente de sua �nica interface.

Bom proveito!
*/
#define _CRT_SECURE_NO_DEPRECATE

#include "Houaiss2Babylon.h"
#include "Common.h"

#include <windows.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#define HOUAISS_PATH "C:\\Tests\\Dicionario"


using namespace std;


void Format(string& str)
{
	string::size_type pos1 = 0;
	string::size_type pos2 = 0;

	while( (pos1 = str.find('<')) != string::npos )
		str.replace(pos1, 1, "&lt;");

	while( (pos1 = str.find('>')) != string::npos )
		str.replace(pos1, 1, "&gt;");

	while( (pos1 = str.find('{')) != string::npos )
	{
		if( pos1 && str[pos1 - 1] == '\\' ) // caractere de escape
			str.replace(pos1 - 1, 2, "&#123;");
		else
		{
			string subStr;

			pos2 = str.find('}', pos1);

			if( pos2 != string::npos )
				subStr = str.substr(pos1 + 1, pos2 - pos1 - 1);
			else
				subStr = str.substr(pos1 + 1);

			istringstream is(subStr);

			string fmt;
			string word;
			is >> fmt;
			getline(is, word);
			if( word[0] == ' ' )
				word.erase(0, 1);

			if( fmt.find("\\i") != string::npos )
				word = "<i>" + word + "</i>";

			if( fmt.find("\\b") != string::npos )
				word = "<b>" + word + "</b>";

			if( fmt.find("\\f20") != string::npos )
				word = "<font style=\"text-transform: uppercase;\">" + word + "</font>";

			if( fmt.find("\\super") != string::npos )
				word = "<font style=\"vertical-align: super;\">" + word + "</font>";

			if( pos2 != string::npos )
				str.replace(pos1, pos2 - pos1 + 1, word);
			else
				str.replace(pos1, pos2, word);
		}
	}
}


int Houaiss2Babylon()
{
	char houaissPath[MAX_PATH];
	int ret = GetHouaissPath(houaissPath);

	if( ret == 0 )
	{
		char cmd;
		string line;
		string word;
		int count = 0;
		char houaissPath2[MAX_PATH];
		char babylonPath[MAX_PATH];

		strcpy(babylonPath, houaissPath);
		strcpy(houaissPath2, houaissPath);
		strcat(houaissPath2, "\\Dicionario\\deah001.txt");
		strcat(babylonPath, "\\Dicionario\\Houaiss.txt");
		ifstream houaiss(houaissPath2);
		ofstream babylon(babylonPath);

		if( houaiss && babylon )
		{
			while( getline(houaiss, line) )
			{
				cmd = line[0];
				line.erase(0, 1);
				Format(line);

				// at� agora: *:VPnCS\-#dvTIRUDLp!c<&0-9rO
				switch( cmd )
				{
				case '*': // verbete
					++count;
					word = line;
					babylon << '\n' << word << '\n';
					break;

				case ':': // defini��o
					babylon << line << "<br>\n";
					break;

				case 'V': // alternativa
				case 'P':
					babylon << "tb. " << line << "<br>\n";
					break;

				case 'n': // n�mero do verbete
					babylon << "<font style=\"vertical-align: super;\">" << line << "</font>";
					babylon << word << "<br>\n";
					break;

				case 'C': // classifica��o
					if( line != "#IND#" )
						babylon << "<font color=\"blue\">" << line << "</font><br>\n";
					break;

				case 'S': // classifica��o
					if( line != "#IND#" )
						babylon << "<font color=\"blue\">" << line << "</font><br>\n";
					break;

				case '-': // numera��o
					babylon << line << " ";
					break;

				case '#': // locu��o
					babylon << "<font color=\"brown\">Locu��o: " << line << "</font><br>\n";
					break;

				case 'd': // data��o
					babylon << "<font color=\"brown\">Data��o: " << line << "</font><br>\n";
					break;

				case 'v': // varia��o
					babylon << "<font color=\"brown\">" << line << "</font><br>\n";
					break;

				case 'T': // rubrica
					babylon << "<font color=\"brown\">Rubrica: " << line << "</font><br>\n";
					break;

				case 'I': // diacronismo
					babylon << "<font color=\"brown\">Diacronismo: " << line << "</font><br>\n";
					break;

				case 'R': // regionalismo
				case 'r':
					babylon << "<font color=\"brown\">Regionalismo: " << line << "</font><br>\n";
					break;

				case 'U': // uso
					babylon << "<font color=\"brown\">Uso: " << line << "</font><br>\n";
					break;

				case 'D': // deriva��o
					babylon << "<font color=\"brown\">Deriva��o: " << line << "</font><br>\n";
					break;

				case 'L': // l�ngua
					babylon << "<font color=\"brown\">L�ngua: " << line << "</font><br>\n";
					break;

				case 'p': // pron�ncia
					babylon << "<font color=\"brown\">Pron�ncia: " << line << "</font><br>\n";
					break;

				case '!': // observa��o
				case 'c':
					babylon << "<font color=\"purple\">Obs.: " << line << "</font><br>\n";
					break;

				case '<': // exemplo
					babylon << "<font color=\"green\">Ex.: " << line << "</font><br>\n";
					break;

				case 'O': // no��o
					babylon << "<font color=\"blue\">No��o</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '&': // etimologia
					babylon << "<font color=\"blue\">Etimologia</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '0': // gram�tica
					babylon << "<font color=\"blue\">Gram�tica</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '1': // gram�tica e uso
					babylon << "<font color=\"blue\">Gram�tica e uso</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '2': // uso
					babylon << "<font color=\"blue\">Uso</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '3': // sin�nimos/variantes
					babylon << "<font color=\"blue\">Sin�nimos/Variantes</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '4': // ant�nimos
					babylon << "<font color=\"blue\">Ant�nimos</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '5': // coletivos
					babylon << "<font color=\"blue\">Coletivos</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '6': // hom�nimos
					babylon << "<font color=\"blue\">Hom�nimos</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '7': // par�nimos
					babylon << "<font color=\"blue\">Par�nimos</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '8': // desconhecido
					babylon << "<font color=\"blue\">Informa��o adicional 8</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				case '9': // vozes de animais
					babylon << "<font color=\"blue\">Vozes de Animais</font><br>\n";
					babylon << "<font color=\"gray\">" << line << "</font><br>\n";
					break;

				default:
					 //cerr << "Unknown command: " << line << '\n';
					break;
				}
			}

			houaiss.close();
			babylon.close();
		}
		else
			ret = -1;
	}

	return ret;
}
