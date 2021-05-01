/** Desofuscador do dicion�rio Houaiss

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2008-04

@desc Permite ver o conte�do dos arquivos do dicion�rio eletr�nico Houaiss 
de forma a ser importado para outros dicion�rios, ou usado independente
da interface oficial.

Linque:
Houaiss: http://www.dicionariohouaiss.com.br

Obs.:
Esse programa n�o se destina � pirataria. Ele n�o fornece nenhum dos
dois dicion�rios, que devem ser adquiridos legalmente. Sua �nica
fun��o � permitir o uso de um �timo dicion�rios em uma excelente
interface de busca de significado de palavras.

Bom proveito!
*/
#define _CRT_SECURE_NO_DEPRECATE

#include "Decryptor.h"
#include "Common.h"

#include <windows.h>
#include <stdio.h>


int Decryptor()
{
	char houaissPath[MAX_PATH];
	int ret = GetHouaissPath(houaissPath);

	if( ret != 0 )
	{
		MessageBox(NULL, "Error getting Houaiss path", 
			"Houaiss2Babylon", MB_OK | MB_ICONERROR);
	}
	else
	{
		strcat(houaissPath, "\\Dicionario\\");

		for( int fileIdx = 1; fileIdx < 64; ++fileIdx )
		{
			CHAR path1[MAX_PATH];
			CHAR path2[MAX_PATH];

			sprintf(path1, "%sdeah%03d.dhx", houaissPath, fileIdx);
			sprintf(path2, "%sdeah%03d.txt", houaissPath, fileIdx);

			HANDLE file1 = CreateFile(path1, GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, 0, NULL);

			HANDLE file2 = CreateFile(path2, GENERIC_READ | GENERIC_WRITE, 0,
				NULL, CREATE_ALWAYS, 0, NULL);

			if( file1 != INVALID_HANDLE_VALUE && file2 != INVALID_HANDLE_VALUE )
			{
				DWORD fileSize = GetFileSize(file1, NULL);

				if( SetFilePointer(file2, fileSize, NULL, FILE_BEGIN) )
				{
					SetEndOfFile(file2);

					HANDLE map1 = CreateFileMapping(file1, NULL, PAGE_READONLY, 0, 0, NULL);
					HANDLE map2 = CreateFileMapping(file2, NULL, PAGE_READWRITE, 0, 0, NULL);

					if( map1 && map2 )
					{
						PBYTE view1 = (PBYTE) MapViewOfFile(map1, FILE_MAP_READ, 0, 0, 0);
						PBYTE view2 = (PBYTE) MapViewOfFile(map2, FILE_MAP_WRITE, 0, 0, 0);

						if( view1 && view2 )
						{
							for( DWORD i = 0; i < fileSize; ++i )
							{
								view2[i] = view1[i] + 0x0B;
							}
						}

						if( view1 )
							UnmapViewOfFile(view1);
						if( view2 )
							UnmapViewOfFile(view2);
					}

					if( map1 )
						CloseHandle(map1);
					if( map2 )
						CloseHandle(map2);
				}
			}

			if( file1 )
				CloseHandle(file1);
			if( file2 )
				CloseHandle(file2);
		}
	}

	return ret;
}
