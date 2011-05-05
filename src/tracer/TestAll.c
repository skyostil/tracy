#include <stdio.h>

#include "CuTest.h"

CuSuite* CuApiGetSuite();
CuSuite* CuUtilGetSuite();
CuSuite* CuPlayerGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, CuUtilGetSuite());
	CuSuiteAddSuite(suite, CuPlayerGetSuite());
	CuSuiteAddSuite(suite, CuApiGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
	return 0;
}
