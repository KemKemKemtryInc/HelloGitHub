#pragma once
#include <iostream>
#include <vector>
#include <string>

class CCsv
{
public:
    CCsv(std::istream& fin = std::cin, std::string sep = ",");
    ~CCsv(void);

    int getline(std::string&);          //読み込んだファイルの一行を返す
    std::string getfield(int n);	        //項目の値を返す。
    int getnfield() const;              //項目数を返す。

private:
    std::istream& m_fin;	        //入力ポインタ
    std::string m_line;         //入力行
    std::vector<std::string> m_field;   //入力フィールド
    size_t m_nfield;	                    //フィールド数
    std::string m_fieldsep;             //セパレータ数


    size_t split();
    int endofline(char);
    size_t advplain(const std::string& line, std::string& fld, size_t);
    size_t advquoted(const std::string& line, std::string& fld, size_t);
};
