#pragma once
#include <string>
#include <iostream>

namespace Helpers
{

	// HEADER SUMMARY
	// A set of helper functions that are used throughout the program, cleaning
	// up code that is heavily reused (Especially Receiving use input)

	// Return cin to default state ready for future requests, this prevents bad input completely messing up the program.
	void ClearCin();

	// Receives input from user. Returns true if users enters 'y', false if user enter 'n'
	bool ReceiveYN(std::string _prompt);

	std::string ReceiveFileName();
	std::string ReceiveFileNameForWrite(std::string _existsPrompt);

	bool FileExists(std::string _fileName);
}
