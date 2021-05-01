#include "Decryptor.h"
#include "Houaiss2Babylon.h"


int main(int argc, char* argv[])
{
	int ret;

	ret = Decryptor();

	if( ret == 0 )
		ret = Houaiss2Babylon();

	return ret;
}
