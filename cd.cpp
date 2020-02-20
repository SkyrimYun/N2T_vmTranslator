#include "Header.h"

CodeWriter::CodeWriter(string& s)
{
	ost = ofstream(s.c_str());
	if (!ost) throw runtime_error("cannot open output file");


	for (char c : s)
	{
		if (c != '.')
			prefix.push_back(c);
		else break;
	}
}

void CodeWriter::writeArithmetic(string s)
{


	if (s == "add")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 + D
		ost << "D=D+M\n";
		ost << spEqualD; // *sp = D
		ost << spPlus; //sp++
	}
	else if (s == "sub")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 - D
		ost << "D=D-M\n";
		ost << spEqualD; // *sp = D
		ost << spPlus; //sp++
	}
	else if (s == "neg")
	{
		ost << spMinus; //sp--
		ost << DEqualsp; // D=*sp
		ost << "D=-D\n";
		ost << spEqualD; // *sp = D
		ost << spPlus; //sp++
	}
	else if (s == "eq")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 - D
		ost << "D=D-M\n";
		ost << "@EQUAL" << lableCount << '\n'; // if equal, goto EQUAL
		ost << "D;JEP\n";
		ost << "@SP\nA=M\nM=0\n"; // *sp = 0; not equal
		ost << spPlus; //sp++
		ost << "@END" << lableCount << '\n'; //jump to the end
		ost << "0;JMP\n";
		ost << "(EQUAL" << lableCount << ")\n";
		ost << "@SP\nA=M\nM=1\n"; // *sp = 1; equal
		ost << spPlus; //sp++
		ost << "(END" << lableCount << ")\n";

		lableCount++;
	}
	else if (s == "gt")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 - D
		ost << "D=D-M\n";
		ost << "@GRATER" << lableCount << '\n'; // if greater, goto GREATER
		ost << "D;JGT\n";
		ost << "@SP\nA=M\nM=0\n"; // *sp = 0; not greater
		ost << spPlus; //sp++
		ost << "@END" << lableCount << '\n'; //jump to the end
		ost << "0;JMP\n";
		ost << "(GRATER" << lableCount << ")\n";
		ost << "@SP\nA=M\nM=1\n"; // *sp = 1; greater
		ost << spPlus; //sp++
		ost << "(END" << lableCount << ")\n";

		lableCount++;
	}
	else if (s == "lt")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 - D
		ost << "D=D-M\n";
		ost << "@LESS" << lableCount << '\n'; // if less, goto LESS
		ost << "D;JLT\n";
		ost << "@SP\nA=M\nM=0\n"; // *sp = 0; not less
		ost << spPlus; //sp++
		ost << "@END" << lableCount << '\n'; //jump to the end
		ost << "0;JMP\n";
		ost << "(LESS" << lableCount << ")\n";
		ost << "@SP\nA=M\nM=1\n"; // *sp = 1; less
		ost << spPlus; //sp++
		ost << "(END" << lableCount << ")\n";

		lableCount++;
	}
	else if (s == "and")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 + D
		ost << "D=D+M\n";
		ost << "D=D-1\nD=D-1\n"; //D-2
		ost << "@TRUE" << lableCount << '\n'; // if true, goto TRUE
		ost << "D;JEQ\n";
		ost << "@SP\nA=M\nM=0\n"; // *sp = 0; false
		ost << spPlus; //sp++
		ost << "@END" << lableCount << '\n'; //jump to the end
		ost << "0;JMP\n";
		ost << "(TRUE" << lableCount << ")\n";
		ost << "@SP\nA=M\nM=1\n"; // *sp = 1; true
		ost << spPlus; //sp++
		ost << "(END" << lableCount << ")\n";

		lableCount++;
	}
	else if (s == "or")
	{
		ost << pop2; //sp--// D=*sp// R13=D//sp--// D=*sp= pop2
		ost << "@13\n"; //D = R13 + D
		ost << "D=D+M\n";
		ost << "@TRUE" << lableCount << '\n'; // if true, goto TRUE
		ost << "D;JGT\n";
		ost << "@SP\nA=M\nM=0\n"; // *sp = 0; false
		ost << spPlus; //sp++
		ost << "@END" << lableCount << '\n'; //jump to the end
		ost << "0;JMP\n";
		ost << "(TRUE" << lableCount << ")\n";
		ost << "@SP\nA=M\nM=1\n"; // *sp = 1; true
		ost << spPlus; //sp++
		ost << "(END" << lableCount << ")\n";

		lableCount++;
	}
}

void CodeWriter::WritePushPop(const int n, string segment, int index)
{


	if (n == C_PUSH)
	{
		if (segment == "local" || segment == "argument")
		{
			//D=index
			//addr=addr+D
			//D=*addr
			ost << "@" << index << '\n';
			ost << "D=A\n";
			ost << "@" << segment << '\n';
			ost << "D=D+M\n";
			//*sp = D
			//sp++
			ost << spEqualD;
			ost << spPlus;
		}
		else if (segment == "constant")
		{
			//D=index
			ost << "@" << index << '\n';
			ost << "D=A\n";
			//*sp=index
			ost << spEqualD;
			//sp++
			ost << spPlus;
		}
		else if (segment == "temp")
		{
			//D=index
			//addr=5+D
			//D=*addr
			ost << "@" << index << '\n';
			ost << "D=A\n";
			ost << "@5\n";
			ost << "D=D+M\n";
			//*sp = D
			//sp++
			ost << spEqualD;
			ost << spPlus;
		}
		else if (segment == "pointer")
		{
			//D=THAT or THIS
			if (index == 0)
			{
				ost << "@3\n";
				ost << "D=A\n";
			}
			else if (index == 1)
			{
				ost << "@4\n";
				ost << "D=A\n";
			}
			//*sp=D
			ost << spEqualD;
			//sp++
			ost << spPlus;
		}
		else if (segment == "static")
		{
			//D=static index
			ost << "@" + prefix + "." << index << '\n';
			ost << "D=M\n";
			//*sp = D
			//sp++
			ost << spEqualD;
			ost << spPlus;
		}
	}

	if (n == C_POP)
	{
		if (segment == "local" || segment == "argument")
		{
			//addr=addr+D
			ost << "@" << index << '\n';
			ost << "D=A\n";
			ost << "@" << segment << '\n';
			ost << "D=D+M\n";
			// R13=D
			ost << "@13\n";
			ost << "M=D\n";
			//sp--
			ost << spMinus;
			// D=*sp
			ost << DEqualsp;
			//*addr = D
			ost << "@13\n";
			ost << "A=M\n";
			ost << "M=D\n";
		}
		else if (segment == "temp")
		{
			//addr=addr+D
			ost << "@" << index << '\n';
			ost << "D=A\n";
			ost << "@5\n";
			ost << "D=D+M\n";
			// R13=D
			ost << "@13\n";
			ost << "M=D\n";
			//sp--
			ost << spMinus;
			// D=*sp
			ost << DEqualsp;
			//*addr = D
			ost << "@13\n";
			ost << "A=M\n";
			ost << "M=D\n";
		}
		else if (segment == "pointer")
		{
			//sp--
			ost << spMinus;
			// D=*sp
			ost << DEqualsp;
			//THIS OR THAT = D
			if (index == 0)
			{
				ost << "@3\n";
				ost << "M=D";
			}
			else if (index == 1)
			{
				ost << "@4\n";
				ost << "M=D";
			}
		}
		else if (segment == "static")
		{
			//sp--
			ost << spMinus;
			// D=*sp
			ost << DEqualsp;
			//prefix.index = D
			ost << "@" + prefix + "." << index << '\n';
			ost << "D=A\n";
		}
	}
}

void CodeWriter::writeInit()
{
	ost<<
}

void CodeWriter::writeLabel(string label)
{
	ost << "(" << label << ")\n";
}

void CodeWriter::writeGoto(string label)
{
	ost << "@" << label << "\n";
	ost << "0;JMP\n";
}

void CodeWriter::writeIf(string label)
{
	//sp--
	//D=*sp
	//D>0 goto label
	ost << spMinus << DEqualsp;
	ost << "@" << label << "\n";
	ost << "D;JGT\n";
}

void CodeWriter::writeFunction(string functionName, int numVars)
{
	writeLabel(functionName);
	for(int i=0;i<numVars;i++)
		WritePushPop(C_PUSH, "constant", 0);
}

void CodeWriter::writeCall(string functionName, int numArgs)
{
	//save caller's frame
	ost << "@RETURN" << retCount << "\n";
	ost << "D=A\n" << spEqualD << spPlus;
	ost << "@LCL\n" << "D=M\n" << spEqualD << spPlus;
	ost << "@ARG\n" << "D=M\n" << spEqualD << spPlus;
	ost << "@THIS\n" << "D=M\n" << spEqualD << spPlus;
	ost << "@THAT\n" << "D=M\n" << spEqualD << spPlus;
	//ARG = sp-5-numArgs
	ost << "@SP\n"<<"D=M\n";
	for (int i = 0; i < 5 + numArgs; i++)
	{
		ost << "D=D-1\n";
	}
	ost << "@ARG\n" << "M=D\n";
	//LCL=SP
	ost << "@SP\n" << "D=M\n";
	ost << "@LCL\n" << "M=D\n";
	//goto functionName
	writeGoto(functionName);

	ost << "(RETURN" << retCount << ")\n";
	retCount++;
}

void CodeWriter::writeReturn()
{
	//R13=LCL
	ost << "@LCL\n" << "D=M\n";
	ost << storeToR;
	//retAddr = *(R13-5) = R14
	ost << "@13\n"<<"D=M\n";
	for (int i = 0; i < 5; i++)
	{
		ost<<"D=D-1";
	}
	ost << "@13\n" << "M=D\n" << "A=M\n"<<"D=M\n";
	ost << "@14\n" << "M=D\n";
	//*ARG=pop()
	ost << spEqualD;
	ost << "@ARG\n" << "A=M\n" << "M=D\n";
	ost << spMinus;
	//sp=ARG+1
	ost << "@ARG\n" << "D=M+1\n";
	ost << "@SP\n" << "M=D\n";
	//THAT=*(R13-1)
	ost << "@13\n" << "D=M\n";
	for (int i = 0; i < 1; i++)
	{
		ost << "D=D-1";
	}
	ost << "@13\n" << "M=D\n" << "A=M\n" << "D=M\n";
	ost << "@THAT\n" << "M=D\n";
	//THIS=*(R13-2)
	ost << "@13\n" << "D=M\n";
	for (int i = 0; i < 2; i++)
	{
		ost << "D=D-1";
	}
	ost << "@13\n" << "M=D\n" << "A=M\n" << "D=M\n";
	ost << "@THIS\n" << "M=D\n";
	//ARG=*(R13-3)
	ost << "@13\n" << "D=M\n";
	for (int i = 0; i < 3; i++)
	{
		ost << "D=D-1";
	}
	ost << "@13\n" << "M=D\n" << "A=M\n" << "D=M\n";
	ost << "@ARG\n" << "M=D\n";
	//LCL=*(R13-4)
	ost << "@13\n" << "D=M\n";
	for (int i = 0; i < 4; i++)
	{
		ost << "D=D-1";
	}
	ost << "@13\n" << "M=D\n" << "A=M\n" << "D=M\n";
	ost << "@LCL\n" << "M=D\n";
	//goto retAddr = R14
	ost << "@R13\n" << "D=M\n";
	writeGoto("D");
}

void CodeWriter::close()
{
	ost.close();
}