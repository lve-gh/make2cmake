#include "info.hpp"
#include <iostream>

string Info::WriteCMake() {
	switch (this -> token)
	{
	case COMMAND: {
		return("execute_process(COMMAND " + this->command.first + " WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE ${" + this->command.second + "})");
		break;
	}
	case INCLUDE: {
		return "";
		break;
	}
	case IFDEF: {
		return("if(" + this->ifdefVar + ")");
		break;
	}
	case ENDIF: {
		return("endif()");
	}
	/*
	case TARGET: {
		if (this->target.GetLinks().find(".cpp") != std::string::npos ||
			this->target.GetLinks().find(".c") != std::string::npos ||
			this->target.GetLinks().find(".h") != std::string::npos ||
			this->target.GetLinks().find(".hpp") != std::string::npos) 
			return("add_library(" + this->target.GetName() + " " + this->target.GetLinks() + ")");
		else {
			return "";
		}
		break;
	} */
	case TARGET: {
		return this->target;
		break;
	}
	case ASSIGMENT: {
		return("set(" + this->assigment.first + " " + this->assigment.second + ")");
		break;
	}
	default:
		return "";
		break;
	}
}