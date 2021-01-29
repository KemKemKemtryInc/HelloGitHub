#include "CCsv.h"

CCsv::CCsv(std::istream& fin, std::string sep)
    : m_fin(fin)
    , m_fieldsep(sep)
    , m_nfield(0)
    , m_field(0)
    , m_line()
{}

CCsv::~CCsv(void)
{}

//一行取得し、必要に応じて伸張
int CCsv::getline(std::string& str)
{
    char c;
    for ( m_line = ""; m_fin.get(c) && !endofline(c);) {
        m_line += c;
    }

    split();
    str = m_line;

    return(!m_fin.eof());
}
std::string CCsv::getfield(int n)
{
    if ( n < 0 || n > (int)m_nfield ) {
        return("");
    } else {
        return(m_field[n]);
    }
}

int CCsv::getnfield() const
{
    return(m_nfield);
}

// 行をフィールドに分割
size_t CCsv::split()
{
    std::string fld;
    size_t i, j;

    m_field.clear();
    m_nfield = 0;   //フィールド(項目) 番号

    if ( m_line.length() == 0 ) {
        return(0);
    }

    i = 0;
    do {
        if ( i < m_line.length() && m_line[i] == '"' ) {
            j = advquoted(m_line, fld, ++i); //クォートをスキップ
        } else {
            j = advplain(m_line, fld, i);
        }

        if ( m_nfield >= m_field.size() ) {
            m_field.push_back(fld);   //追加
        } else {
            m_field[m_nfield] = fld;//更新
        }

        m_nfield++;
        i = j + 1;

    } while ( j < m_line.length() );

    return(m_nfield);
}
/************************************************************************
 * DESCRIPTION    : \r \n \r\n EOFをチェックして捨てる
 * INPUT            : char c           文字
 * OUTPUT      : なし
 * UPDATE      : なし
 * RETURN      : チェック結果
 ************************************************************************/
int CCsv::endofline(char c)
{
    int eol;
    eol = (c =='\r' || c == '\n');

    if ( c == '\r' ) {
        m_fin.get(c);
        if ( !m_fin.eof() && c != '\n' ) {
            m_fin.putback(c); //読みすぎ
        }
    }

    return(eol);
}
/************************************************************************
 * DESCRIPTION  : クォートでくくられないフィールド
 * INPUT        : const std::string& line   フィールド文字
 *              : int i                  一行の中の文字位置
 * OUTPUT       : std::string& fld         フィールド文字
 * UPDATE       : なし
 * RETURN       : 次のセパレータのインデックスを返す
 ************************************************************************/
size_t CCsv::advplain(const std::string& s, std::string& fld, size_t i)
{
    size_t j;

    j = s.find_first_of(m_fieldsep, i);

    //if(j > s.length()){
    if ( j == std::string::npos ) {   //区切り文字が無い場合
        j = s.length();
    }

    fld = std::string(s, i, j-i);

    return(j);
}
/************************************************************************
 * CSU NAME      : advquoted
 * DESCRIPTION   : クォートでくくられたフィールドのクォートを削除
 * INPUT           : onst std::string& s   フィールド文字
 * OUTPUT       : std::string& fld      クォートを削除したフィールド文字
 * UPDATE       : なし
 * RETURN       : 次のセパレータのインデックスを返す
 ************************************************************************/
size_t CCsv::advquoted(const std::string& s, std::string& fld, size_t i)
{
    size_t j;

    fld = "";
    for ( j = i; j < s.length(); j++ ) {
        if ( s[j] == '"' && s[++j] != '"' ) {   //クォートの連続判定

            size_t k = s.find_first_of(m_fieldsep, j);

            //if( k > s.length()){         //セパレータが見つからなかった。
            if ( k ==  std::string::npos ) {   //セパレータが見つからなかった。
                k = s.length();
            }

            for ( k -= j; k-- > 0; ) {
                fld += s[j++];
            }
            break;
        }

        fld += s[j];
    }

    return(j);
}
