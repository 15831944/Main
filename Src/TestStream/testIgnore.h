#include "istream"
#include "limits"
#include "string"
class IgnoreLines
{
private: 
	int num;
public:
	explicit IgnoreLines(int n = 1) :num(n){}
	template <class charT, class traits> 
	friend std::basic_istream<charT, traits>& operator >> (std::basic_istream<charT, traits>& strm, const IgnoreLines& ign)
	{
		using namespace std;
		//string str("��������");
		//str += std::to_string(ign.num) + "��";
		//for (auto & item : str)
		//	strm.putback(item);
		//strm.putback('\n');
		
		//strm.putback("��������N��");		
		 for (int i = 0; i < ign.num; i++)
			strm.ignore(std::numeric_limits<std::streamsize>::max(), strm.widen('\n'));

		return strm;
	}
};