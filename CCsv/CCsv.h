#pragma once
#include <iostream>
#include <vector>
#include <string>

class CCsv
{
public:
    CCsv(std::istream& fin = std::cin, std::string sep = ",");
    ~CCsv(void);

    int getline(std::string&);          //�ǂݍ��񂾃t�@�C���̈�s��Ԃ�
    std::string getfield(int n);	        //���ڂ̒l��Ԃ��B
    int getnfield() const;              //���ڐ���Ԃ��B

private:
    std::istream& m_fin;	        //���̓|�C���^
    std::string m_line;         //���͍s
    std::vector<std::string> m_field;   //���̓t�B�[���h
    size_t m_nfield;	                    //�t�B�[���h��
    std::string m_fieldsep;             //�Z�p���[�^��


    size_t split();
    int endofline(char);
    size_t advplain(const std::string& line, std::string& fld, size_t);
    size_t advquoted(const std::string& line, std::string& fld, size_t);
};
