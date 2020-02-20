#include "Header.h"

bool checkasm(string& s)
{
	string ss;

	if (s.size() < 3)
		return false;

	for (int i = s.size() - 3; i < s.size(); i++)
	{
		ss.push_back(s[i]);
	}
	cout << ss << endl;
	if (ss == ".vm")
		return true;
	else
		return false;
}

void getAllFiles(string path, vector<string>& files)
{
	

	if (checkasm(path))
	{
		files.push_back(path);
	}
	else
	{
		long   hFile = 0;

		struct _finddata_t fileinfo;
		string p;

		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				string check;
				check.append(fileinfo.name);
				if (checkasm(check))
				{
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0); 
			_findclose(hFile);
		}
	}
}


int main()
{
	try
	{
		string DATA_DIR = "StackTest.vm";
		string outputFile = "StackTest.asm";
		vector<string> files;
		getAllFiles(DATA_DIR, files);
		
		if (files.size() == 0)
			throw runtime_error("no vaild input file!");
		
		CodeWriter cw(outputFile);
		for (int i = 0; i < files.size(); i++)
		{
			Parser par(files[i]);

			while (par.hasMoreCommands())
			{
				par.advance();
				if (par.commandType() == C_PUSH || par.commandType() == C_POP)
				{
					cw.WritePushPop(C_PUSH, par.arg1(), par.arg2());
				}
				else if (par.commandType() == C_ARITHMETIC)
				{
					cw.writeArithmetic(par.arg1());
				}
				else if (par.commandType() == C_CALL)
				{
					cw.writeCall(par.arg1(), par.arg2());
				}
				else if (par.commandType() == C_FUNCTION)
				{
					cw.writeFunction(par.arg1(), par.arg2());
				}
				else if (par.commandType() == C_GOTO)
				{
					cw.writeGoto(par.arg1());
				}
				else if (par.commandType() == C_IF)
				{
					cw.writeIf(par.arg1());
				}
				else if (par.commandType() == C_LABEL)
				{
					cw.writeLabel(par.arg1());
				}
				else if (par.commandType() == C_RETURN)
				{
					cw.writeReturn();
				}
			}
		}
		
		cw.close();
	}
	catch (runtime_error& e)
	{
		cout << "runtime error: " << e.what() << "\n";
	}
}