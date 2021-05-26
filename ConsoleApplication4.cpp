#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

struct ObI
{
	string name;
	int type;
	bool mass;
};

vector<string> C, L;
vector<ObI> I;
vector<string> K = { "and", "array", "begin", "boolean", "break", "case", "char", "declare", "default", "do", "downto", "else", "end", "false", "for", "if", "integer", "let", "main", "not", "of", "or", "program", "read", "repeat", "then", "to", "true", "until", "var", "void", "while", "write", "xor" };
vector<string> R = { "!", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "[", "]", "^", "{", "|", "}" };
vector<string> D = { "!=", "&&", "*=", "++", "+=", "--", "-=", "/*", "//", "/=", "<<", "<=", "<>", "==", ">=", ">>", "||" };

FILE* pf;
ofstream asm1;
vector<string> vectemp;
vector<string> dataCode;
vector<string> mainCode;
string strTemp;
int markCount = 0;
int cmpType;
int  k = 0;

string GenerateMark()
{
	return ("@M" + to_string(markCount++));
}

struct Token
{
	int n;
	char ch;
};

bool Comp(string s1, string s2)
{
	return s1 < s2;
}

int Find(vector<string> A, int begin, int end, string str)
{
	if (begin == end && A[begin] != str)
		return -1;
	if ((end - begin) / 2 == 0)
		if (A[begin] != str && A[end] != str)
			return -1;
	if (A[end] == str)
		return end;
	if (A[begin] == str)
		return begin;
	if (A[((end - begin) / 2) + begin] == str)
		return(end - begin) / 2 + begin;
	if (A[((end - begin) / 2) + begin] < str)
		return Find(A, ((end - begin) / 2) + begin, end, str);
	else
		return Find(A, begin, ((end - begin) / 2) + begin, str);
}

int Find(vector<ObI> A, int begin, int end, string str)
{
	if (begin == end && A[begin].name != str)
		return -1;
	if ((end - begin) / 2 == 0)
		if (A[begin].name != str && A[end].name != str)
			return -1;
	if (A[end].name == str)
		return end;
	if (A[begin].name == str)
		return begin;
	if (A[((end - begin) / 2) + begin].name == str)
		return(end - begin) / 2 + begin;
	if (A[((end - begin) / 2) + begin].name < str)
		return Find(A, ((end - begin) / 2) + begin, end, str);
	else
		return Find(A, begin, ((end - begin) / 2) + begin, str);
}

int Find(vector<string> A, string str)
{
	if (A.size() == 0)
		return -1;
	if (A[A.size() / 2] == str)
		return A.size() / 2;
	if (A[A.size() / 2] < str)
		return Find(A, A.size() / 2, A.size() - 1, str);
	else
		return Find(A, 0, A.size() / 2, str);
}

int Find(vector<ObI> A, string str)
{
	if (A.size() == 0)
		return -1;
	if (A[A.size() / 2].name == str)
		return A.size() / 2;
	if (A[A.size() / 2].name < str)
		return Find(A, A.size() / 2, A.size() - 1, str);
	else
		return Find(A, 0, A.size() / 2, str);
}

void quickSort(vector <string>& arr, int left, int right) {
	int i = left, j = right;
	string tmp;
	string pivot = arr[(left + right) / 2];
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

void quickSort(vector <ObI>& arr, int left, int right) {
	int i = left, j = right;
	ObI tmp;
	string pivot = arr[(left + right) / 2].name;
	while (i <= j) {
		while (arr[i].name < pivot)
			i++;
		while (arr[j].name > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

void Error(string nameFunc, int number)
{
	switch (number)
	{
	case 0:
	{cout << "Функция: " + nameFunc + ": Неизвестный символ."; fclose(pf); break; }
	case 1:
	{cout << "Функция: " + nameFunc + ": Неверный идентификатор."; fclose(pf); break; }
	case 2:
	{cout << "Функция: " + nameFunc + ": Повтор подряд идентификатора."; fclose(pf); break; }
	case 3:
	{cout << "Функция: " + nameFunc + ": Отсутвие нужного идентификатора."; fclose(pf); break; }
	case 4:
	{cout << "Функция: " + nameFunc + ": Логическая ошибка."; fclose(pf); break; }
	default:
		break;
	}
}

Token scan(bool f)
{
	int n = 0;
	char x;
	Token token;
	string buf = "";
	ObI tempObI;
	tempObI.mass = false;
	tempObI.type = 0;
	while (!feof(pf))
	{
		x = getc(pf);
		if (x == -1)
			break;
		if (feof(pf))
			break;
		switch (n)
		{
		case 0:
		{

			if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'))
			{
				buf += x;
				n = 1;
				break;
			}
			else if (x >= '0' && x <= '9')
			{
				buf += x;
				n = 2;
				break;

			}

			else if ((x == '!') || (x >= '#' && x <= '/') || (x >= ':' && x <= '?') || (x >= '[' && x <= '^') || (x >= '{' && x <= '}'))
			{
				buf += x;
				n = 3;
				break;
			}

			else if (x == '"')
			{
				buf = x;
				n = 4;
				break;
			}
			else if ((x == ' ') || (x == '\n') || (x == '\t') || (x == '\r')) {
				n = 0;
			}
			else
			{
				Error("scan", 0);
				cout << " " << (int)x << endl;
				//fclose(pf);
			}
			break;
		}
		case 1:
		{



			if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9') || x == '_')
			{
				buf += x;
				break;
			}
			else
			{
				token.n = Find(K, buf);
				if (token.n != -1)
				{
					token.ch = 'K';
					fseek(pf, -1, SEEK_CUR);
					if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
						fseek(pf, -1, SEEK_CUR);
					if (f)
						fseek(pf, -((int)buf.length() + 1), SEEK_CUR);
					return token;
				}
				if (buf != "")
				{
					token.n = Find(I, buf);
					if (token.n == -1)
					{
						tempObI.name = buf;
						I.push_back(tempObI);
						quickSort(I, 0, I.size() - 1);
						token.n = Find(I, buf);
					}
					token.ch = 'I';
					fseek(pf, -1, SEEK_CUR);
					if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
						fseek(pf, -1, SEEK_CUR);
					if (f)
						fseek(pf, -(int)buf.length(), SEEK_CUR);
					return token;

				}
			}
			break;
		}
		case 2:
		{
			if ((x >= '0' && x <= '9'))
			{
				buf += x;
				break;
			}
			else
			{
				token.n = Find(C, buf);
				if (token.n == -1)
				{
					C.push_back(buf);
					quickSort(C, 0, C.size() - 1);
					token.n = Find(C, buf);
				}
				token.ch = 'C';
				fseek(pf, -1, SEEK_CUR);
				if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
					fseek(pf, -1, SEEK_CUR);
				if (f)
					fseek(pf, -(int)buf.length(), SEEK_CUR);
				return token;
			}
			break;
		}
		case 3:
		{
			buf += x;
			token.n = Find(D, buf);
			if (token.n != -1)
			{
				token.ch = 'D';
				if (D[token.n] == "/*")
				{
					n = 5;
					break;
				}
				if (D[token.n] == "//")
				{
					n = 6;
					break;
				}
				if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
					fseek(pf, -1, SEEK_CUR);
				if (f)
					fseek(pf, -(int)buf.length(), SEEK_CUR);
				return token;

			}
			if (buf != "")
			{

				buf = buf[0];
				token.n = Find(R, buf);
				if (token.n != -1)
				{
					token.ch = 'R';
					fseek(pf, -1, SEEK_CUR);
					if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
						fseek(pf, -1, SEEK_CUR);
					if (f)
						fseek(pf, -1, SEEK_CUR);
					return token;
				}
			}
			break;
		}
		case 4:
		{
			if (x != '"')
			{
				buf += x;
				break;
			}
			else
			{
				buf += x;
				token.n = Find(L, buf);
				if (token.n == -1)
				{
					token.ch = 'L';
					L.push_back(buf);
					quickSort(L, 0, L.size() - 1);
					token.n = Find(L, buf);
				}
				token.ch = 'L';
				if (((x == '\n') || (x == '\t') || (x == '\r')) && f)
					fseek(pf, -1, SEEK_CUR);
				if (f)
					fseek(pf, -(int)buf.length(), SEEK_CUR);
				return token;
				buf = "";
				n = 0;
				break;
			}
			break;
		}
		case 5:
		{
			if (buf.substr(buf.size() - 1) + x != "*/")
			{
				buf += x;
				break;
			}
			else
			{
				buf += x;
				cout << buf << endl;
				buf = "";
				n = 0;
				break;
			}
			break;
		}
		case 6:
		{
			if (x != '\n')
			{
				buf += x;
				break;
			}
			else
			{
				buf += x;
				cout << buf << endl;
				buf = "";
				n = 0;
				break;
			}
			break;
		}
		}
	}
}

bool E();

bool F()
{
	Token temp = scan(true);
	if (temp.ch == 'R' && temp.n == Find(R, "("))
	{
		temp = scan(false);
		if (!E())
			return false;
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, ")"))
			return true;
	}
	else if (temp.ch == 'R' && temp.n == Find(R, "["))
	{
		temp = scan(false);
		if (!E())
			return false;
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "]"))
			return true;
	}
	else
	{
		temp = scan(true);
		if (temp.ch == 'I')
		{
			temp = scan(false);
			ObI ident = I[temp.n];
			temp = scan(true);
			if (temp.ch == 'R' && temp.n == Find(R, "["))
			{
				temp = scan(false);
				if (!E())
					return false;
				temp = scan(false);
				if (temp.ch == 'R' && temp.n == Find(R, "]"))
				{
					mainCode.push_back("pop di\n");
					if (ident.type == 0)
					{
						mainCode.push_back("shl di, 1\n");
						mainCode.push_back("mov ax, " + ident.name + "[di]\n");
						mainCode.push_back("push ax\n");
					}
					else
						if (ident.type == 2)
						{
							mainCode.push_back("mov ax, 0\n");
							mainCode.push_back("mov al, " + ident.name + "[di]\n");
							mainCode.push_back("push ax\n");
						}
						else
						{
							Error("F", 4);
							return false;
						}
					return true;
				}
				else
				{
					Error("F", 1);
					return false;
				}
			}
			else
			{
				if (ident.type == 0)
				{
					mainCode.push_back("mov di, 0\n");
					mainCode.push_back("shl di, 1\n");
					mainCode.push_back("mov ax, " + ident.name + "[di]\n");
					mainCode.push_back("push ax\n");
				}
				else
					if (ident.type == 2)
					{
						mainCode.push_back("mov di, 0\n");
						mainCode.push_back("mov ax, 0\n");
						mainCode.push_back("mov al, " + ident.name + "[di]\n");
						mainCode.push_back("push ax\n");
					}
					else
					{
						Error("F", 4);
						return false;
					}
				return true;
			}
		}
		else
			if (temp.ch == 'C')
			{
				temp = scan(false);
				mainCode.push_back("mov ax, " + C[temp.n] + "\npush ax\n");
				return true;
			}
		Error("F", 1);
		return false;
	}
	Error("F", 1);
	return false;
}

bool T()
{
	int tmp;
	if (!F())
		return false;
	Token temp = scan(true);
	while ((temp.ch == 'R') && (temp.n == Find(R, "*") || temp.n == Find(R, "/")))
	{
		temp = scan(false);
		tmp = temp.n;
		temp = scan(true);
		if (((temp.ch == 'R') && (temp.n == Find(R, "*") || temp.n == Find(R, "/"))))
		{
			Error("T", 2);
			return false;
		}
		if (!F())
			return false;
		if (tmp == Find(R, "/"))
		{
			mainCode.push_back("pop bx\n");
			mainCode.push_back("pop ax\n");
			mainCode.push_back("xor dx, dx\n");
			mainCode.push_back("div bx\n");
			mainCode.push_back("push ax\n");
		}
		else
		{
			mainCode.push_back("pop bx\n");
			mainCode.push_back("pop ax\n");
			mainCode.push_back("xor dx, dx\n");
			mainCode.push_back("mul bx\n");
			mainCode.push_back("push ax\n");
		}
		temp = scan(true);
	}
	return true;
}

bool E()
{
	int tmp;
	if (!T())
		return false;
	Token temp = scan(true);
	while ((temp.ch == 'R') && (temp.n == Find(R, "+") || temp.n == Find(R, "-")))
	{
		temp = scan(false);
		tmp = temp.n;
		temp = scan(true);
		if (((temp.ch == 'R') && (temp.n == Find(R, "+") || temp.n == Find(R, "-"))))
		{
			Error("E", 2);
			return false;
		}
		if (!T())
			return false;
		if (tmp == Find(R, "+"))
		{
			mainCode.push_back("pop bx\n");
			mainCode.push_back("pop ax\n");
			mainCode.push_back("add ax, bx\n");
			mainCode.push_back("push ax\n");
		}
		else
		{
			mainCode.push_back("pop bx\n");
			mainCode.push_back("pop ax\n");
			mainCode.push_back("sub ax, bx\n");
			mainCode.push_back("push ax\n");
		}
		temp = scan(true);
	}
	return true;
}

bool EI();

bool Zn()
{
	Token temp = scan(true);
	if (temp.ch == 'R' && temp.n == Find(R, "<"))
	{
		temp = scan(false);
		cmpType = 0;
		return true;
	}
	else if (temp.ch == 'R' && temp.n == Find(R, ">"))
	{
		temp = scan(false);
		cmpType = 1;
		return true;
	}
	else if (temp.ch == 'D' && (temp.n == Find(D, "!=") || temp.n == Find(D, "<>")))
	{
		temp = scan(false);
		cmpType = 2;
		return true;
	}
	else if (temp.ch == 'D' && temp.n == Find(D, "<="))
	{
		temp = scan(false);
		cmpType = 3;
		return true;
	}
	else if (temp.ch == 'D' && temp.n == Find(D, "=="))
	{
		temp = scan(false);
		cmpType = 4;
		return true;
	}
	else if (temp.ch == 'D' && temp.n == Find(D, ">="))
	{
		temp = scan(false);
		cmpType = 5;
		return true;
	}
	Error("Zn", 1);
	return false;
}

bool FI()
{
	Token temp = scan(true);
	string trueMark, falseMark;
	if (temp.ch == 'I' && !(I[temp.n].type == 2))
	{
		if (!E())
			return false;
		/*temp = scan(false);
		temp = scan(true);
		if (temp.ch == 'R' && temp.n == Find(R, "["))
		{
			temp = scan(false);
			if (!E())
				return false;
			temp = scan(false);
			if (temp.ch == 'R' && temp.n == Find(R, "]"))
			{
				return true;
			}
			else { Error("FI", 1); return false; }
		}*/
		temp = scan(true);
		if ((temp.ch == 'D' && !(temp.n == Find(D, "&&") && temp.n == Find(D, "||"))) || (temp.ch == 'R' || (temp.n == Find(R, "<")) && (temp.n == Find(R, ">"))))
		{
			if (!Zn())
				return false;
			if (!E())
				return false;
			mainCode.push_back("pop bx\n");
			mainCode.push_back("pop ax\n");
			mainCode.push_back("cmp ax, bx\n");
			trueMark = GenerateMark();
			falseMark = GenerateMark();
			switch (cmpType)
			{
			case 0: mainCode.push_back("jb " + trueMark + "\n"); break; // <
			case 1: mainCode.push_back("ja " + trueMark + "\n"); break; // >
			case 2: mainCode.push_back("jne " + trueMark + "\n"); break; // !=
			case 3: mainCode.push_back("jbe " + trueMark + "\n"); break; // <=
			case 4: mainCode.push_back("je " + trueMark + "\n"); break; // ==
			case 5: mainCode.push_back("jae " + trueMark + "\n"); break; // >=
			}
			mainCode.push_back("push 0\n");
			mainCode.push_back("jmp " + falseMark + "\n");
			mainCode.push_back(trueMark + ":\n");
			mainCode.push_back("push 1\n");
			mainCode.push_back(falseMark + ":\n");
		}
		return true;
	}
	else if (temp.ch == 'R' && temp.n == Find(R, "("))
	{
		temp = scan(false);
		if (!EI())
			return false;
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, ")"))
			return true;
	}
	else if (temp.ch == 'R' && temp.n == Find(R, "["))
	{
		temp = scan(false);
		if (!E())
			return false;
		if (!Zn())
			return false;
		if (!E())
			return false;
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "]"))
			return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "true"))
	{
		temp = scan(false);
		mainCode.push_back("push 1\n");
		temp = scan(true);
		if (!(temp.ch == 'K' && temp.n == Find(K, "true")))
			return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "false"))
	{
		temp = scan(false);
		mainCode.push_back("push 0\n");
		temp = scan(true);
		if (!(temp.ch == 'K' && temp.n == Find(K, "false")))
			return true;
	}
	else if ((temp.ch == 'R' && temp.n == Find(R, "!")) || (temp.ch == 'K' && temp.n == Find(K, "not")))
	{
		temp = scan(false);
		if (!FI())
			return false;
		mainCode.push_back("pop ax\n");
		mainCode.push_back("not ax\n");
		mainCode.push_back("push ax\n");
		temp = scan(true);
		if (!((temp.ch == 'R' && temp.n == Find(R, "!")) || (temp.ch == 'K' && temp.n == Find(K, "not"))))
			return true;
	}
	else
	{
		if (!E())
			return false;
		if (!Zn())
			return false;
		if (!EI())
			return false;
		mainCode.push_back("pop bx\n");
		mainCode.push_back("pop ax\n");
		mainCode.push_back("cmp ax, bx\n");
		trueMark = GenerateMark();
		falseMark = GenerateMark();
		switch (cmpType)
		{
		case 0: Error("FI", 1); break; // <
		case 1: Error("FI", 1); break; // >
		case 2: mainCode.push_back("jne " + trueMark + "\n"); break; // !=
		case 3: Error("FI", 1); break; // <=
		case 4: mainCode.push_back("je " + trueMark + "\n"); break; // ==
		case 5: Error("FI", 1); break; // >=
		}
		mainCode.push_back("push 0\n");
		mainCode.push_back("jmp " + falseMark + "\n");
		mainCode.push_back(trueMark + ":\n");
		mainCode.push_back("push 1\n");
		mainCode.push_back(falseMark + ":\n");
		return true;
	}
	Error("FI", 1);
	return false;

}

bool TI()
{
	if (!FI())
		return false;
	Token temp = scan(true);
	while ((temp.ch == 'K' && temp.n == Find(K, "and")) || (temp.ch == 'D' && temp.n == Find(D, "&&")))
	{
		temp = scan(false);
		temp = scan(true);
		if ((temp.ch == 'K' && temp.n == Find(K, "and")))
		{
			Error("TI", 2); return false;
		}
		if ((temp.ch == 'D' && temp.n == Find(D, "&&")))
		{
			Error("TI", 2); return false;
		}
		if (!FI())
			return false;
		mainCode.push_back("pop bx\n");
		mainCode.push_back("pop ax\n");
		mainCode.push_back("and ax, bx\n");
		mainCode.push_back("push ax\n");
		temp = scan(true);
	}
	return true;
}

bool EI()
{
	if (!TI())
		return false;
	Token temp = scan(true);
	while ((temp.ch == 'K' && temp.n == Find(K, "or")) || (temp.ch == 'D' && temp.n == Find(D, "||")))
	{
		temp = scan(false);
		temp = scan(true);
		if ((temp.ch == 'K' && temp.n == Find(K, "or")))
		{
			Error("EI", 2); return false;
		}
		if ((temp.ch == 'D' && temp.n == Find(D, "||")))
		{
			Error("EI", 2); return false;
		}
		if (!TI())
			return false;
		mainCode.push_back("pop bx\n");
		mainCode.push_back("pop ax\n");
		mainCode.push_back("or ax, bx\n");
		mainCode.push_back("push ax\n");
		temp = scan(true);
	}
	return true;
}

bool BaseType()
{
	Token temp = scan(false);
	if (temp.ch == 'K' && temp.n == Find(K, "integer"))
	{
		strTemp = " dw ";
		for (int i = 0; i < vectemp.size(); i++)
		{
			I[Find(I, vectemp[i])].type = 0;
			if (!I[Find(I, vectemp[i])].mass)
				dataCode.push_back(vectemp[i] + strTemp + " (?)\n");
		}
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "char"))
	{
		strTemp = " db ";
		for (int i = 0; i < vectemp.size(); i++)
		{
			I[Find(I, vectemp[i])].type = 1;
			if (!I[Find(I, vectemp[i])].mass)
				dataCode.push_back(vectemp[i] + strTemp + " (?)\n");
		}
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "boolean"))
	{
		strTemp = " db ";
		for (int i = 0; i < vectemp.size(); i++)
		{
			I[Find(I, vectemp[i])].type = 2;
			if (!I[Find(I, vectemp[i])].mass)
				dataCode.push_back(vectemp[i] + strTemp + " (?)\n");
		}
		return true;
	}
	Error("BaseType", 1);
	return false;
}

bool Array()
{
	Token temp = scan(false);
	if (temp.ch == 'K' && temp.n == Find(K, "array"))
	{
		for (int i = 0; i < vectemp.size(); i++) I[Find(I, vectemp[i])].mass = true;
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "["))
		{
			temp = scan(false);
			if (temp.ch == 'C')
			{
				string ln = C[temp.n];
				temp = scan(false);
				if (temp.ch == 'R' && temp.n == Find(R, "]"))
				{
					temp = scan(false);
					if (temp.ch == 'K' && temp.n == Find(K, "of"))
						if (BaseType())
						{
							//strTemp +=to_string(ln);
							for (int i = 0; i < vectemp.size(); i++)
							{
								dataCode.push_back(vectemp[i] + strTemp + ln + " DUP (?)\n");
							}
							return true;
						}
				}
			}
		}
	}
	Error("Array", 1);
	return false;
}

bool Type()
{
	Token temp = scan(true);
	if (temp.ch == 'K' && temp.n == Find(K, "array"))
	{
		if (Array())
			return true;
	}
	else
		if (BaseType())
			return true;
	Error("Type", 1);
	return false;
}

bool Declaration()
{
	Token temp = scan(false);
	if (temp.ch == 'K' && temp.n == Find(K, "var"))
	{
		asm1 << "; user data\n";
		temp = scan(true);
		do
		{
			if (!(temp.ch == 'I')) { Error("Declaration", 1); return false; }
			temp = scan(false);
			strTemp += I[temp.n].name;
			vectemp.push_back(I[temp.n].name);
			temp = scan(true);
			while (!(temp.ch == 'R' && temp.n == Find(R, ":")))
			{
				if (temp.ch == 'R' && temp.n == Find(R, ","))
				{
					temp = scan(false);
					temp = scan(false);
					if (temp.ch != 'I') { Error("Declaration", 1); return false; }
					strTemp += ", " + I[temp.n].name;
					vectemp.push_back(I[temp.n].name);
				}
				temp = scan(true);
			}
			temp = scan(false);
			if (!Type())
				return false;
			temp = scan(false);
			if (!(temp.ch == 'R' && temp.n == Find(R, ";"))) { Error("Declaration", 1); return false; }
			vectemp.clear();
			strTemp = "";
			temp = scan(true);
		} while (!(temp.ch == 'K' && temp.n == Find(K, "main")));
		return true;
	}
	Error("Declaration", 1);
	return false;
}

bool Operation();

bool Appropriation()
{
	Token temp = scan(false);
	if (temp.ch == 'K' && temp.n == Find(K, "let"))
		temp = scan(false);
	if (temp.ch == 'I')
	{
		mainCode.push_back("; Appr\n");
		ObI ident = I[temp.n];
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "["))
		{
			if (!E())
				return false;
			temp = scan(false);
			if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Appropriation", 1); return false; }
			temp = scan(false);
		}
		if (temp.ch == 'R' && temp.n == Find(R, "="))
		{
			temp = scan(true);
			if (temp.ch == 'K' && temp.n == Find(K, "true"))
			{
				temp = scan(false);
				mainCode.push_back("mov " + ident.name + "[di], 1\n");
				return true;
			}
			if (temp.ch == 'K' && temp.n == Find(K, "false"))
			{
				temp = scan(false);
				mainCode.push_back("mov " + ident.name + "[di], 0\n");
				return true;
			}
			if (temp.ch == 'L')
			{
				temp = scan(false);
				string str = "";
				str += L[temp.n][1];
				mainCode.push_back("push \"" + str + "\"\n");
				mainCode.push_back("pop ax\n");
				if (ident.mass)
				{
					mainCode.push_back("pop di\n");
					mainCode.push_back("mov " + ident.name + "[di], al\n");
				}
				else
					mainCode.push_back("mov " + ident.name + ", al\n");
				return true;
			}
			if (temp.ch == 'I' && I[temp.n].type == 1)
			{
				temp = scan(false);
				string tempI = I[temp.n].name;
				temp = scan(true);
				if (temp.ch == 'R' && temp.n == Find(R, "["))
				{
					temp = scan(false);
					if (!E())
						return false;
					temp = scan(false);
					if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Appropriation", 1); return false; }
					temp = scan(false);
				}
				else
				{
					mainCode.push_back("mov di, 0\n");
				}
				mainCode.push_back("mov al, " + tempI + "[di]\n");
				mainCode.push_back("push ax\n");
				mainCode.push_back("pop ax\n");
				if (ident.mass)
				{
					mainCode.push_back("pop bx\n");
					mainCode.push_back("mov " + ident.name + "[bx], al\n");
				}
				else
					mainCode.push_back("mov " + ident.name + ", al\n");
				return true;
			}
			if (E())
			{
				mainCode.push_back("pop ax\n");
				if (ident.mass)
				{
					mainCode.push_back("pop di\n");
					mainCode.push_back("shl di, 1\n");
					mainCode.push_back("mov " + ident.name + "[di], ax\n");
				}
				else
					mainCode.push_back("mov " + ident.name + ", ax\n");
				return true;
			}
		}
	}
	Error("Appropriation", 1);
	return false;
}

bool Operator();

bool Condition()
{
	Token temp = scan(false);
	mainCode.push_back("; Cond()\n");
	temp = scan(false);
	if (temp.ch == 'R' && temp.n == Find(R, "("))
	{
		if (!EI())
			return false;
		string thenMark = GenerateMark();
		string elseMark = GenerateMark();
		string endMark = GenerateMark();
		temp = scan(false);
		if (!(temp.ch == 'R' && temp.n == Find(R, ")"))) { Error("Condition", 1); return false; }
		mainCode.push_back("pop ax\n");
		mainCode.push_back("cmp ax, 0\n");
		mainCode.push_back("jnz " + thenMark + "\n");
		mainCode.push_back("jmp " + elseMark + "\n");
		mainCode.push_back(thenMark + ":\n");
		temp = scan(false);
		if (!(temp.ch == 'K' && temp.n == Find(K, "then"))) { Error("Condition", 1); return false; }
		if (!Operator())
			return false;
		mainCode.push_back("jmp " + endMark + "\n");
		temp = scan(true);
		mainCode.push_back(elseMark + ":\n");
		if (temp.ch == 'K' && temp.n == Find(K, "else"))
		{
			temp = scan(false);
			if (!Operator())
				return false;
		}
		mainCode.push_back(endMark + ":\n");
		return true;
	}
	Error("Condition", 1);
	return false;
}

bool Cycle()
{
	Token temp = scan(false);
	mainCode.push_back("; Cycle()\n");
	string whileMark = GenerateMark();
	string endMark = GenerateMark();
	string doMark = GenerateMark();
	temp = scan(false);
	if (temp.ch == 'R' && temp.n == Find(R, "("))
	{
		mainCode.push_back(whileMark + ":\n");
		if (!EI())
			return false;
		temp = scan(false);
		if (!(temp.ch == 'R' && temp.n == Find(R, ")"))) { Error("Cycle", 1); return false; }
		temp = scan(false);
		if (!(temp.ch == 'K' && temp.n == Find(K, "do"))) { Error("Cycle", 1); return false; }
		mainCode.push_back("pop ax\n");
		mainCode.push_back("cmp ax, 0\n");
		mainCode.push_back("jnz " + doMark + "\n");
		mainCode.push_back("jmp " + endMark + "\n");
		mainCode.push_back(doMark + ":\n");
		if (!Operator())
			return false;
		mainCode.push_back("jmp " + whileMark + "\n");
		mainCode.push_back(endMark + ":\n");
		return true;
	}
	Error("Cycle", 1);
	return false;
}


bool Repeat()
{
	Token temp = scan(false);
	mainCode.push_back("; Repeat()\n");
	string repeatMark = GenerateMark();
	string endMark = GenerateMark();
	mainCode.push_back(repeatMark + ":\n");
	do
	{
		if (!Operator())
			return false;
		temp = scan(true);
	} while (!(temp.ch == 'K' && temp.n == Find(K, "until")));
	temp = scan(false);
	if (!(temp.ch == 'K' && temp.n == Find(K, "until"))) { Error("Repeat", 1); return false; }
	temp = scan(false);
	if (temp.ch == 'R' && temp.n == Find(R, "("))
	{
		if (!EI())
			return false;
		temp = scan(false);
		if (!(temp.ch == 'R' && temp.n == Find(R, ")"))) { Error("Repeat", 1); return false; }
		mainCode.push_back("pop ax\n");
		mainCode.push_back("cmp ax, 0\n");
		mainCode.push_back("jnz " + endMark + "\n");
		mainCode.push_back("jmp " + repeatMark + "\n");
		mainCode.push_back(endMark + ":\n");
		return true;
	}
	Error("Repeat", 1);
	return false;
}


bool Write()
{
	Token temp = scan(false);
	string textp;
	ObI tempI;
	if (temp.ch == 'K' && temp.n == Find(K, "write"))
	{
		mainCode.push_back("; Write()\n");
		mainCode.push_back("lea dx, clrf\n");
		mainCode.push_back("mov ah, 9\n");
		mainCode.push_back("int 21h\n");
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "("))
		{
			temp = scan(true);
			if (temp.ch == 'L')
			{
				textp = GenerateMark();
				string str = L[temp.n];
				str.erase(str.length() - 1, 1);

				dataCode.push_back(textp + " db " + str + "$\"\n");
				mainCode.push_back("lea dx, " + textp + "\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				temp = scan(false);
			}
			else if (temp.ch == 'I' && I[temp.n].type == 1) //char
			{
				temp = scan(false);
				tempI = I[temp.n];
				temp = scan(true);
				if (temp.ch == 'R' && temp.n == Find(R, "["))
				{
					temp = scan(false);
					if (!E())
						return false;
					temp = scan(false);
					if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Write", 1); return false; }
				}
				if (tempI.mass) mainCode.push_back("pop di\n");
				else mainCode.push_back("mov di, 0\n");
				mainCode.push_back("mov ax, 0\n");
				mainCode.push_back("mov al, " + tempI.name + "[di]\n");
				mainCode.push_back("mov dl, al\n");
				mainCode.push_back("mov ah, 2\n");
				mainCode.push_back("int 21h\n");
			}
			else if (temp.ch == 'I' && I[temp.n].type == 2) //bool
			{
				if (!E())
					return false;
				string trueMark = GenerateMark();
				string falseMark = GenerateMark();
				mainCode.push_back("pop ax\n");
				mainCode.push_back("cmp ax, 0\n");
				mainCode.push_back("je " + trueMark + "\n");
				mainCode.push_back("lea dx, @@true\n");
				mainCode.push_back("jmp " + falseMark + "\n");
				mainCode.push_back(trueMark + ":\n");
				mainCode.push_back("lea dx, @@false\n");
				mainCode.push_back(falseMark + ":\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
			}
			else if (!E())
				return false;
			else
			{
				textp = GenerateMark(); //int
				mainCode.push_back("pop ax\n");
				mainCode.push_back("mov bx, 10\n");
				mainCode.push_back("mov di, 0\n");
				mainCode.push_back("mov si, ax\n");
				mainCode.push_back("cmp ax, 0\n");
				mainCode.push_back("jns " + textp + "\n");
				mainCode.push_back("neg si\n");
				mainCode.push_back("mov ah, 2\n");
				mainCode.push_back("mov dl, \"-\"\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("mov ax, si\n");
				mainCode.push_back(textp + ":\n");
				mainCode.push_back("mov dx, 0\n");
				mainCode.push_back("div bx\n");
				mainCode.push_back("add dl, 30h\n");
				mainCode.push_back("mov output[di], dl\n");
				mainCode.push_back("inc di\n");
				mainCode.push_back("cmp al, 0\n");
				mainCode.push_back("jnz " + textp + "\n");
				mainCode.push_back("mov cx, di\n");
				mainCode.push_back("dec di\n");
				mainCode.push_back("mov ah, 2\n");
				textp = GenerateMark();
				mainCode.push_back(textp + ":\n");
				mainCode.push_back("mov dl, output[di]\n");
				mainCode.push_back("dec di\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("loop " + textp + "\n");
			}
			temp = scan(true);
			while ((temp.ch == 'R' && temp.n == Find(R, ",")))
			{
				temp = scan(false);
				temp = scan(true);
				if (temp.ch == 'L')
				{
					textp = GenerateMark();
					dataCode.push_back(textp + " db " + L[temp.n] + "$\n");
					mainCode.push_back("lea dx, " + textp + "\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					temp = scan(false);
				}
				else if (temp.ch == 'I' && I[temp.n].type == 1)
				{
					temp = scan(false);
					tempI = I[temp.n];
					temp = scan(true);
					if (temp.ch == 'R' && temp.n == Find(R, "["))
					{
						temp = scan(false);
						if (!E())
							return false;
						temp = scan(false);
						if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Write", 1); return false; }
					}
					if (tempI.mass) mainCode.push_back("pop di\n");
					else mainCode.push_back("mov di, 0\n");
					mainCode.push_back("mov ax, 0\n");
					mainCode.push_back("mov al, " + tempI.name + "[di]\n");
					mainCode.push_back("mov dl, al\n");
					mainCode.push_back("mov ah, 2\n");
					mainCode.push_back("int 21h\n");
				}
				else if (temp.ch == 'I' && I[temp.n].type == 2)
				{
					if (!E())
						return false;
					string trueMark = GenerateMark();
					string falseMark = GenerateMark();
					mainCode.push_back("pop ax\n");
					mainCode.push_back("cmp ax, 0\n");
					mainCode.push_back("je " + trueMark + "\n");
					mainCode.push_back("lea dx, @@true\n");
					mainCode.push_back("jmp " + falseMark + "\n");
					mainCode.push_back(trueMark + ":\n");
					mainCode.push_back("lea dx, @@false\n");
					mainCode.push_back(falseMark + ":\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
				}
				else if (!E())
					return false;
				else
				{
					textp = GenerateMark();
					mainCode.push_back("pop ax\n");
					mainCode.push_back("mov bx, 10\n");
					mainCode.push_back("mov di, 0\n");
					mainCode.push_back("mov si, ax\n");
					mainCode.push_back("cmp ax, 0\n");
					mainCode.push_back("jns " + textp + "\n");
					mainCode.push_back("neg si\n");
					mainCode.push_back("mov ah, 2\n");
					mainCode.push_back("mov dl, \"-\"\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("mov ax, si\n");
					mainCode.push_back(textp + ":\n");
					mainCode.push_back("mov dx, 0\n");
					mainCode.push_back("div bx\n");
					mainCode.push_back("add dl, 30h\n");
					mainCode.push_back("mov output[di], dl\n");
					mainCode.push_back("inc di\n");
					mainCode.push_back("cmp al, 0\n");
					mainCode.push_back("jnz " + textp + "\n");
					mainCode.push_back("mov cx, di\n");
					mainCode.push_back("dec di\n");
					mainCode.push_back("mov ah, 2\n");
					textp = GenerateMark();
					mainCode.push_back(textp + ":\n");
					mainCode.push_back("mov dl, output[di]\n");
					mainCode.push_back("dec di\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("loop " + textp + "\n");
				}
				temp = scan(true);
			}
			if (temp.ch == 'R' && temp.n == Find(R, ")"))
			{
				temp = scan(false);
				return true;
			}
		}
		mainCode.push_back("lea   dx, clrf\n");
		mainCode.push_back("mov   ah, 9\n");
		mainCode.push_back("int   21h\n");
	}
	Error("Write", 1);
	return false;
}

bool Read()
{
	Token temp = scan(false);
	ObI ident;
	if (temp.ch == 'K' && temp.n == Find(K, "read"))
	{
		temp = scan(false);
		if (temp.ch == 'R' && temp.n == Find(R, "("))
		{
			temp = scan(false);
			if (temp.ch == 'I')
			{
				ident = I[temp.n];
				temp = scan(true);
				if (temp.ch == 'R' && temp.n == Find(R, "["))
				{
					temp = scan(false);
					if (!E())
						return false;
					temp = scan(false);
					if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Read", 1); return false; }
				}
				else mainCode.push_back("push 0\n");
			}
			switch (ident.type)
			{
			case 0: //int
			{
				string lstart = GenerateMark();
				string lMark = GenerateMark();
				string lend = GenerateMark();
				string errorM = GenerateMark();
				mainCode.push_back(lstart + ":\n");
				mainCode.push_back("mov ah, 0Ah\n");
				mainCode.push_back("lea dx, @buffer\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("mov ax, 0\n");
				mainCode.push_back("mov cx, 0\n");
				mainCode.push_back("mov cl, byte ptr[blength]\n");
				mainCode.push_back("mov bx, cx\n");
				mainCode.push_back(lMark + ":\n");
				mainCode.push_back("dec bx\n");
				mainCode.push_back("mov al, @buf[bx]\n");
				mainCode.push_back("cmp al, \"9\"\n");
				mainCode.push_back("ja " + errorM + "\n");
				mainCode.push_back("cmp al, \"0\"\n");
				mainCode.push_back("jb " + errorM + "\n");
				mainCode.push_back("loop " + lMark + "\n");
				mainCode.push_back("mov cl, byte ptr[blength]\n");
				mainCode.push_back("mov di, 0\n");
				mainCode.push_back("mov ax, 0\n");
				lMark = GenerateMark();
				mainCode.push_back(lMark + ":\n");
				mainCode.push_back("mov bl, @buf[di]\n");
				mainCode.push_back("inc di\n");
				mainCode.push_back("sub bl, 30h\n");
				mainCode.push_back("add ax, bx\n");
				mainCode.push_back("mov si, ax\n");
				mainCode.push_back("mov bx, 10\n");
				mainCode.push_back("mul bx\n");
				mainCode.push_back("loop " + lMark + "\n");
				mainCode.push_back("mov ax, si\n");
				mainCode.push_back("pop di\n");
				mainCode.push_back("shl di, 1\n");
				mainCode.push_back("mov " + ident.name + "[di], ax\n");
				mainCode.push_back("jmp " + lend + "\n");
				mainCode.push_back(errorM + ":\n");
				mainCode.push_back("lea dx, err_msg\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("jmp " + lstart + "\n");
				mainCode.push_back(lend + ":\n");
				mainCode.push_back("lea dx, clrf\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				break;
			}
			case 1: //char
			{
				mainCode.push_back("mov ah, 0Ah\n");
				mainCode.push_back("lea dx, @buffer\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("xor dx, dx\n");
				mainCode.push_back("mov dl, @buf[0]\n");
				mainCode.push_back("pop di\n");
				mainCode.push_back("mov " + ident.name + "[di], dl\n");
				mainCode.push_back("lea dx, clrf\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				break;
			}
			case 2: //bool
			{
				string lstart = GenerateMark();
				string l4 = GenerateMark();
				string l5 = GenerateMark();
				string le = GenerateMark();
				string lt = GenerateMark();
				string lend = GenerateMark();
				string lerror = GenerateMark();
				mainCode.push_back(lstart + ":\n");
				mainCode.push_back("mov ah, 0Ah\n");
				mainCode.push_back("lea dx, @buffer\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("cmp blength, 4\n");
				mainCode.push_back("je " + l4 + "\n");
				mainCode.push_back("cmp blength, 5\n");
				mainCode.push_back("je " + l5 + "\n");
				mainCode.push_back("jmp " + lerror + "\n");
				mainCode.push_back(l4 + ":\n");
				mainCode.push_back("lea si, @true\n");
				mainCode.push_back("lea di, @buf\n");
				mainCode.push_back("mov cx, 4\n");
				mainCode.push_back("repe cmpsb\n");
				mainCode.push_back("jz " + le + "\n");
				mainCode.push_back("jmp " + lerror + "\n");
				mainCode.push_back(l5 + ":\n");
				mainCode.push_back("lea si, @false\n");
				mainCode.push_back("lea di, @buf\n");
				mainCode.push_back("mov cx, 5\n");
				mainCode.push_back("repe cmpsb\n");
				mainCode.push_back("jz " + le + "\n");
				mainCode.push_back("jmp " + lerror + "\n");
				mainCode.push_back(le + ":\n");
				mainCode.push_back("cmp @buf[0], \"t\"\n");
				mainCode.push_back("je " + lt + "\n");
				mainCode.push_back("push 0\n");
				mainCode.push_back("jmp " + lend + "\n");
				mainCode.push_back(lt + ":\n");
				mainCode.push_back("push 1\n");
				mainCode.push_back("jmp " + lend + "\n");
				mainCode.push_back(lerror + ":\n");
				mainCode.push_back("lea dx, err_msg\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("jmp " + lstart + "\n");
				mainCode.push_back(lend + ":\n");
				mainCode.push_back("pop ax\n");
				mainCode.push_back("pop di\n");
				mainCode.push_back("mov " + ident.name + "[di], al\n");
				mainCode.push_back("lea dx, clrf\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				break;
			}
			}
			temp = scan(false);
			while ((temp.ch == 'R' && temp.n == Find(R, ",")))
			{
				temp = scan(false);
				if (temp.ch == 'I')
				{
					ident = I[temp.n];
					temp = scan(true);
					if (temp.ch == 'R' && temp.n == Find(R, "["))
					{
						temp = scan(false);
						if (!E())
							return false;
						temp = scan(false);
						if (!(temp.ch == 'R' && temp.n == Find(R, "]"))) { Error("Read", 1); return false; }
					}
					else mainCode.push_back("push 0\n");
				}
				switch (ident.type)
				{
				case 0: //int
				{
					string lstart = GenerateMark();
					string lMark = GenerateMark();
					string lend = GenerateMark();
					string errorM = GenerateMark();
					mainCode.push_back(lstart + ":\n");
					mainCode.push_back("mov ah, 0Ah\n");
					mainCode.push_back("lea dx, @buffer\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("mov ax, 0\n");
					mainCode.push_back("mov cx, 0\n");
					mainCode.push_back("mov cl, byte ptr[blength]\n");
					mainCode.push_back("mov bx, cx\n");
					mainCode.push_back(lMark + ":\n");
					mainCode.push_back("dec bx\n");
					mainCode.push_back("mov al, @buf[bx]\n");
					mainCode.push_back("cmp al, \"9\"\n");
					mainCode.push_back("ja " + errorM + "\n");
					mainCode.push_back("cmp al, \"0\"\n");
					mainCode.push_back("jb " + errorM + "\n");
					mainCode.push_back("loop " + lMark + "\n");
					mainCode.push_back("mov cl, byte ptr[blength]\n");
					mainCode.push_back("mov di, 0\n");
					mainCode.push_back("mov ax, 0\n");
					lMark = GenerateMark();
					mainCode.push_back(lMark + ":\n");
					mainCode.push_back("mov bl, @buf[di]\n");
					mainCode.push_back("inc di\n");
					mainCode.push_back("sub bl, 30h\n");
					mainCode.push_back("add ax, bx\n");
					mainCode.push_back("mov si, ax\n");
					mainCode.push_back("mov bx, 10\n");
					mainCode.push_back("mul bx\n");
					mainCode.push_back("loop " + lMark + "\n");
					mainCode.push_back("mov ax, si\n");
					mainCode.push_back("pop di\n");
					mainCode.push_back("shl di, 1\n");
					mainCode.push_back("mov " + ident.name + "[di], ax\n");
					mainCode.push_back("jmp " + lend + "\n");
					mainCode.push_back(errorM + ":\n");
					mainCode.push_back("lea dx, err_msg\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("jmp " + lstart + "\n");
					mainCode.push_back(lend + ":\n");
					mainCode.push_back("lea dx, clrf\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					break;
				}
				case 1: //char
				{
					mainCode.push_back("mov ah, 0Ah\n");
					mainCode.push_back("lea dx, @buffer\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("xor dx, dx\n");
					mainCode.push_back("mov dl, @buf[0]\n");
					mainCode.push_back("pop di\n");
					mainCode.push_back("mov " + ident.name + "[di], dl\n");
					mainCode.push_back("lea dx, clrf\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					break;
				}
				case 2: //bool
				{
					string lstart = GenerateMark();
					string l4 = GenerateMark();
					string l5 = GenerateMark();
					string le = GenerateMark();
					string lt = GenerateMark();
					string lend = GenerateMark();
					string lerror = GenerateMark();
					mainCode.push_back(lstart + ":\n");
					mainCode.push_back("mov ah, 0Ah\n");
					mainCode.push_back("lea dx, @buffer\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("cmp blength, 4\n");
					mainCode.push_back("je " + l4 + "\n");
					mainCode.push_back("cmp blength, 5\n");
					mainCode.push_back("je " + l5 + "\n");
					mainCode.push_back("jmp " + lerror + "\n");
					mainCode.push_back(l4 + ":\n");
					mainCode.push_back("lea si, @true\n");
					mainCode.push_back("lea di, @buf\n");
					mainCode.push_back("mov cx, 4\n");
					mainCode.push_back("repe cmpsb\n");
					mainCode.push_back("jz " + le + "\n");
					mainCode.push_back("jmp " + lerror + "\n");
					mainCode.push_back(l5 + ":\n");
					mainCode.push_back("lea si, @false\n");
					mainCode.push_back("lea di, @buf\n");
					mainCode.push_back("mov cx, 5\n");
					mainCode.push_back("repe cmpsb\n");
					mainCode.push_back("jz " + le + "\n");
					mainCode.push_back("jmp " + lerror + "\n");
					mainCode.push_back(le + ":\n");
					mainCode.push_back("cmp @buf[0], \"t\"\n");
					mainCode.push_back("je " + lt + "\n");
					mainCode.push_back("push 0\n");
					mainCode.push_back("jmp " + lend + "\n");
					mainCode.push_back(lt + ":\n");
					mainCode.push_back("push 1\n");
					mainCode.push_back("jmp " + lend + "\n");
					mainCode.push_back(lerror + ":\n");
					mainCode.push_back("lea dx, err_msg\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					mainCode.push_back("jmp " + lstart + "\n");
					mainCode.push_back(lend + ":\n");
					mainCode.push_back("pop ax\n");
					mainCode.push_back("pop di\n");
					mainCode.push_back("mov " + ident.name + "[di], al\n");
					mainCode.push_back("lea dx, clrf\n");
					mainCode.push_back("mov ah, 9\n");
					mainCode.push_back("int 21h\n");
					break;
				}
				}
				temp = scan(false);
			}
			if (temp.ch == 'R' && temp.n == Find(R, ")"))
				return true;
		}
	}
	Error("Read", 1);
	return false;
}

bool Operator()
{
	Token temp = scan(true);
	if (temp.ch == 'I' || (temp.ch == 'K' && temp.n == Find(K, "let")))
	{
		if (!Appropriation())
			return false;
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "if"))
	{
		if (!Condition())
			return false;
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "while"))
	{
		if (!Cycle())
			return false;
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "repeat"))
	{
		if (!Repeat())
			return false;
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "write"))
	{
		if (!Write())
			return false;
		return true;
	}
	else if (temp.ch == 'K' && temp.n == Find(K, "read"))
	{
		if (!Read())
			return false;
		return true;
	}
	else
	{
		if (temp.ch == 'R' && temp.n == Find(R, "{"))
			if (!Operation())
				return false;
		return true;
	}
	Error("Operator", 1);
	return false;
}

bool Operation()
{
	Token temp = scan(false);
	if (temp.ch == 'R' && temp.n == Find(R, "{"))
	{
		temp = scan(true);
		while (!(temp.ch == 'R' && temp.n == Find(R, "}")))
		{
			if (!Operator())
				return false;
			temp = scan(false);
			if (!(temp.ch == 'R' && temp.n == Find(R, ";"))) { Error("Operation", 1); return false; }
			temp = scan(true);
		}
		temp = scan(false);
		return true;
	}
	Error("Operation", 1);
	return false;
}

bool Program()
{
	Token temp = scan(false);
	if (temp.ch == 'K' && temp.n == Find(K, "program"))
	{
		temp = scan(false);
		if (temp.ch != 'I') { Error("Program", 1); return false; }
		string nameProg = ";" + I[temp.n].name + "\n";
		asm1 << nameProg;
		temp = scan(false);
		if (!(temp.ch == 'R' && temp.n == Find(R, ";"))) { Error("Program", 1); return false; }
		asm1 << ".8086\n";
		asm1 << ".model small\n";
		asm1 << ".stack 100h\n";
		asm1 << ".data\n";
		asm1 << "; support data\n";
		asm1 << "@buffer db 6\n";
		asm1 << "blength db (?)\n";
		asm1 << "@buf db 256 DUP (?)\n";
		asm1 << "err_msg db \"Input error, try again\", 0Dh, 0Ah, \"$\" \n";
		asm1 << "@true db \"true\"\n";
		asm1 << "@false db \"false\"\n";
		asm1 << "@@true db \"true$\"\n";
		asm1 << "@@false db \"false$\"\n";
		asm1 << "clrf db 0Dh, 0Ah,\"$\"\n";
		asm1 << "output db 6 DUP (?), \"$\"\n";
		temp = scan(true);
		if (temp.ch == 'K' && temp.n == Find(K, "var"))
		{
			if (!Declaration())
				return false;
		}
		temp = scan(false);
		if (temp.ch == 'K' && temp.n == Find(K, "main"))
		{
			mainCode.push_back(".code\n");
			mainCode.push_back("main:\n");
			mainCode.push_back("mov ax, @data\n");
			mainCode.push_back("mov ds, ax\n");
			mainCode.push_back("mov es, ax\n");
			mainCode.push_back("xor ax, ax\n");
			//temp = scan(true);
			if (!Operation())
				return false;
			temp = scan(false);
			if (temp.ch == 'K' && temp.n == Find(K, "end"))
			{
				temp = scan(true);
				if (temp.ch == 'R' && temp.n == Find(R, "."))
					temp = scan(false);
				else { Error("Program", 1); return false; }
				mainCode.push_back("lea dx, clrf\n");
				mainCode.push_back("mov ah, 9\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("mov ax, 4C00h\n");
				mainCode.push_back("int 21h\n");
				mainCode.push_back("end main\n");
			}
			temp = scan(true);
			if (temp.ch == 'K' || temp.ch == 'R' || temp.ch == 'C' || temp.ch == 'I' || temp.ch == 'D' || temp.ch == 'L') { Error("Program", 1); return false; }
			for (int i = 0; i < dataCode.size(); i++)
			{
				asm1 << dataCode[i];
			}
			for (int i = 0; i < mainCode.size(); i++)
			{
				asm1 << mainCode[i];
			}
			return true;
		}
		else { Error("Program", 1); return false; }
	}
	Error("Program", 1);
	return false;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	fopen_s(&pf, "test.txt", "r");
	asm1.open("programm.asm", ios_base::out);
	if (Program())
		cout << "Успешно!" << endl;
	asm1.close();
	fclose(pf);
	system("pause");

}
