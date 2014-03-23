/*!
 * @file
 */
#define NOMINMAX
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

namespace {

	const char FILENAME_TO_EXECUTE[] = "_Game.exe";
	const char FILENAME_HOOK_PREBOOT[] = "hook_preboot.bat";
	const char FILENAME_HOOK_POSTBOOT[] = "hook_postboot.bat";

	void waitForConfirmOnError(int exitcode);

	DWORD bootProcess(const char *filename, char *commandLineArguments);

}	// unnamed namespace

int main(int argc, char *argv[])
{
	char * commandLineArguments = "";
	if (argc > 1) {
		char * itr = argv[1];
		char * end = argv[argc-1];
		for (; itr < end; ++itr) {
			// replace \0 -> space to use it as command line arguments
			if (*itr == '\0') { *itr = ' '; }
		}
		commandLineArguments = argv[1];
	}
	
	// call a pre-boot hook script
	{
		const std::string hookcommand = std::string(FILENAME_HOOK_PREBOOT) + std::string(" ") + std::string(commandLineArguments);
		std::vector<char> command(hookcommand.length());
		command.assign(hookcommand.begin(), hookcommand.end());
		command.push_back('\0');

		bootProcess(NULL, &command[0]);
	}

	// boot Game.exe
	const DWORD exitcode = bootProcess(FILENAME_TO_EXECUTE, commandLineArguments);

	// call a post-boot hook script
	{
		const std::string hookcommand = std::string(FILENAME_HOOK_POSTBOOT) + std::string(" ") + std::string(commandLineArguments);
		std::vector<char> command(hookcommand.length());
		command.assign(hookcommand.begin(), hookcommand.end());
		command.push_back('\0');

		bootProcess(NULL, &command[0]);
	}

#ifdef _DEBUG
	printf("it will exit with exitcode %d\n", exitcode);
	printf("please push any key.\n");
	getchar();
#endif

	return exitcode;
}

namespace {

	void waitForConfirmOnError(int exitcode)
	{
		printf("please push any key.\n");
		getchar();
		exit(exitcode);
	}

	DWORD bootProcess(const char *filename, char *commandLineArguments)
	{
		printf("run %s with: %s\n", filename, commandLineArguments);

		STARTUPINFO startupinfo = { 0 };
		GetStartupInfo(&startupinfo);

		PROCESS_INFORMATION processinfo = { 0 };

		{
			const BOOL result = CreateProcess(filename, commandLineArguments, NULL, NULL, TRUE, 0, NULL, NULL, &startupinfo, &processinfo);
			if (result != TRUE) {
				const DWORD errorcode = GetLastError();
				printf("failed to CreateProcess(%s) because of errorcode %d.\n", filename, errorcode);
				waitForConfirmOnError(-1);
			}
		}
		CloseHandle(processinfo.hThread);

		WaitForSingleObject(processinfo.hProcess, INFINITE);

		DWORD exitcode = 0;
		{
			const BOOL result = GetExitCodeProcess(processinfo.hProcess, &exitcode);
			CloseHandle(processinfo.hProcess);

			if (result != TRUE) {
				const DWORD errorcode = GetLastError();
				printf("failed to GetExitCodeProcess(%s) because of errorcode %d.\n", filename, errorcode);
				waitForConfirmOnError(-2);
			}
		}

		return exitcode;
	}

}	// unnamed namespace
