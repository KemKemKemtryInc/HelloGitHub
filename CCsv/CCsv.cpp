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

//��s�擾���A�K�v�ɉ����ĐL��
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

// �s���t�B�[���h�ɕ���
size_t CCsv::split()
{
    std::string fld;
    size_t i, j;

    m_field.clear();
    m_nfield = 0;   //�t�B�[���h(����) �ԍ�

    if ( m_line.length() == 0 ) {
        return(0);
    }

    i = 0;
    do {
        if ( i < m_line.length() && m_line[i] == '"' ) {
            j = advquoted(m_line, fld, ++i); //�N�H�[�g���X�L�b�v
        } else {
            j = advplain(m_line, fld, i);
        }

        if ( m_nfield >= m_field.size() ) {
            m_field.push_back(fld);   //�ǉ�
        } else {
            m_field[m_nfield] = fld;//�X�V
        }

        m_nfield++;
        i = j + 1;

    } while ( j < m_line.length() );

    return(m_nfield);
}
/************************************************************************
 * DESCRIPTION    : \r \n \r\n EOF���`�F�b�N���Ď̂Ă�
 * INPUT            : char c           ����
 * OUTPUT      : �Ȃ�
 * UPDATE      : �Ȃ�
 * RETURN      : �`�F�b�N����
 ************************************************************************/
int CCsv::endofline(char c)
{
    int eol;
    eol = (c =='\r' || c == '\n');

    if ( c == '\r' ) {
        m_fin.get(c);
        if ( !m_fin.eof() && c != '\n' ) {
            m_fin.putback(c); //�ǂ݂���
        }
    }

    return(eol);
}
/************************************************************************
 * DESCRIPTION  : �N�H�[�g�ł������Ȃ��t�B�[���h
 * INPUT        : const std::string& line   �t�B�[���h����
 *              : int i                  ��s�̒��̕����ʒu
 * OUTPUT       : std::string& fld         �t�B�[���h����
 * UPDATE       : �Ȃ�
 * RETURN       : ���̃Z�p���[�^�̃C���f�b�N�X��Ԃ�
 ************************************************************************/
size_t CCsv::advplain(const std::string& s, std::string& fld, size_t i)
{
    size_t j;

    j = s.find_first_of(m_fieldsep, i);

    //if(j > s.length()){
    if ( j == std::string::npos ) {   //��؂蕶���������ꍇ
        j = s.length();
    }

    fld = std::string(s, i, j-i);

    return(j);
}
/************************************************************************
 * CSU NAME      : advquoted
 * DESCRIPTION   : �N�H�[�g�ł�����ꂽ�t�B�[���h�̃N�H�[�g���폜
 * INPUT           : onst std::string& s   �t�B�[���h����
 * OUTPUT       : std::string& fld      �N�H�[�g���폜�����t�B�[���h����
 * UPDATE       : �Ȃ�
 * RETURN       : ���̃Z�p���[�^�̃C���f�b�N�X��Ԃ�
 ************************************************************************/
size_t CCsv::advquoted(const std::string& s, std::string& fld, size_t i)
{
    size_t j;

    fld = "";
    for ( j = i; j < s.length(); j++ ) {
        if ( s[j] == '"' && s[++j] != '"' ) {   //�N�H�[�g�̘A������

            size_t k = s.find_first_of(m_fieldsep, j);

            //if( k > s.length()){         //�Z�p���[�^��������Ȃ������B
            if ( k ==  std::string::npos ) {   //�Z�p���[�^��������Ȃ������B
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
