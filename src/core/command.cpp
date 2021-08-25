#include "core/command.h"


namespace zq{


VSCommand::VSCommand (int32 numArguments, char** arguments)
{
	for (int32 i = 0; i < numArguments; ++i)
	{
		VSString Command(arguments[i]);
        VSString Value;
		if (Value.GetString(Command, '=', 1, false))
		{
            VSString Key;
			Key.GetString(Command, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}
	}
}

VSCommand::VSCommand (char* commandLine)
{
	if (!commandLine)
	{
		return;
	}

	char * pNextToken = NULL;
	char* token = VSStrtok(commandLine, " ",&pNextToken);
	while (token)
	{
        VSString Command(token);
        VSString Value;
		if (Command.GetString(Value, '=', 1, false))
		{
            VSString Key;
			Command.GetString(Key, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}

		token = VSStrtok(NULL, " ",&pNextToken);
	}
}

VSCommand::~VSCommand ()
{
}

bool VSCommand::GetName(const char* name)
{

	if (m_CommandMap.Find(name) != m_CommandMap.GetNum())
	{
		return true;
	}

	return false;
}

bool VSCommand::GetInteger (const char* name, int& value)
{
	uint32 i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToInt(m_CommandMap[i].Value);
		return true;
	}

	return false;
}

bool VSCommand::GetReal(const char* name, float& value)
{
	uint32 i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToReal(m_CommandMap[i].Value);
		return true;
	}

	return false;
}

bool VSCommand::GetString (const char* name, VSString & value)
{
	uint32 i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = m_CommandMap[i].Value;
		return true;
	}
	
	return false;
}

}
