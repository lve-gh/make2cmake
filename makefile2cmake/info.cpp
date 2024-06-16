#include "info.hpp"
#include <iostream>

string Info::WriteCMake() {
	switch (this -> token)
	{
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
	case TARGET: {
		return this->target + '\n' + this->recipe;
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