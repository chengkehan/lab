#include <iostream>
#include <string>

//int main()
//{
//	std::cout << "hello world" << std::endl;
//	system("pause");
//	return 0;
//}

//int main()
//{
//	std::cout << "Enter tow numbers:" << std::endl;
//	int v1, v2;
//	std::cin >> v1 >> v2;
//	std::cout << "The sum of " << v1 << " and " << v2 << " is " << v1 + v2 << std::endl;
//	system("pause");
//	return 0;
//}

int main()
{
	std::string s;
	typedef std::string* pstring;
	const pstring cstr1 = &s;
	pstring const cstr2 = &s;
	cstr2[0] = 'a';
	return 0;
}