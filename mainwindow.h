#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <fstream>
#include<string>
//#include<stdlib.h>
#include <iostream>
#include <sstream>
//#include "Symbol_table.h"
#include "AST.h"


////////////////////////////////////////////////////

const std::string Identifiers[] = {
"program", "var", "constant","integer", "boolean",
"string", "float", "true","false", "if", "fi", "then", "else",
"while", "do", "od", "and", "or", "read", "write",
"for", "from","to", "by", "function", "procedure","return",
"not", "begin", "end"
};

const std::string Operators [] =  {"(",")","[","]",":",".",";",",",":=","+","-","*","/","=","!=","<","<=",">",">="};


typedef enum
{
     tok_ident, tok_integer, tok_string,tok_float,eof,kw_ignore,
   kw_program,
    kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
    kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
    kw_while, kw_do, kw_od,
    kw_and, kw_or,
    kw_read, kw_write,
    kw_for, kw_from, kw_to, kw_by,
    kw_function, kw_procedure,kw_return, kw_not, kw_begin, kw_end,undefined,lx_Iparen, lx_rparen, lx_lbracket, lx_rbracket,
    Ix_colon, lx_dot, lx_semicolon, lx_comma, Ix_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,lx_uminus,lx_newline,kw_block,invalid_float,invalid_int,inivalid_str
}
tok;





class TOKEN
{
public:
    int type;
    std::string value;
    int int_value;
    float float_value;

};


class SCANNER {
public:
//    TOKEN Scan(std::string mystring,TOKEN &token,int &index) {
////        cout << "inside the scanner" << endl;
////        stringstream mystream(expres);
//      std::string value;
//        char c;
//        std::string substr;
//    // ifstream fd;
//    // fd.open(filename);

//        if(index >= mystring.length()){
//            token.type = eof;
//            return token;
//        }
////        TOKEN * token = mytoken;
//            // while (fscanf(fptr,"%c", &c)) {
////             c = mystring[index++];
//              c = mystring[index++];
////              cout <<"chara is sc is : " << c << endl;
////             if(feof(fptr)){
////                 token.type = eof;
////                 return token;
////             }
//             if(c >= 'a' && c <= 'z'){
//                //  cout << "we entered the first loop" << endl;
//                 while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'){
//                    //  cout << "we entered the second loop"<<endl;
//                     value += c;
////                     mystream >> c;
//                     if(index >= mystring.length())break;
//                     c = mystring[index++];
//                 }
//                 index--;
////                 fseek(fptr, -1, SEEK_CUR);
////                  c = mystring[index++];
//                //  cout <<"its an ident"<<endl;
//                 token.value = value;
//                 token.type = tok_ident;
//                 int length = sizeof(Identifiers)/ sizeof(Identifiers[0]);
//                 for(int i = 0 ; i < length ;i++){
//                  if(token.value == Identifiers[i]){
//                     token.type = i + 6;
//                     // cout << token.value << ":";
//                     // cout <<  Identifiers[i]<<"Keyword"<< endl;
//                  }
//                 }
//                return token ;
//             }
//            else if( c == '"'){
//                     value += c;
//                       c = mystring[index++];
////                        if (feof(fptr)){
////                        // cout << "we at aend boys" << endl;
////                        token.type = eof;
////                        return token;
////                    }
//                    //   cout << "the new value of c is " << c << endl;
//                   while(c != '"'){
//                        if(c == '\n'){
//                        // cout << "ok imma headout" << endl;
//                        token.type = inivalid_str;
//                        return token ;
//                    }                            //  cout << "we entered the second loop"<<endl;
//                     value += c;
//                     if(index >= mystring.length())break;
//                      c = mystring[index++];
//                 }
//                if(c == '"'){
//                value += c;
//                //  cout <<"its a string"<<endl;
//                 token.value = value;
//                 token.type = tok_string;
//                return token;
//                }
////                c = mystring[index++];
//                index--;
//             token.value = value;
//             token.type = inivalid_str;
//            return token;
//            }
//             else if(c >= '0' && c <= '9'){
////                 cout << "it might be integer" << endl;
//                 while(c >= '0' && c <= '9'){
////                     cout << "looping" << endl;
//                    value += c;
//                    if(index >= mystring.length()){
////                        cout << "reaching end of file at : " << value << endl;
//                        break;}
//                     c = mystring[index++];
//                 }
////                     c = mystring[index++];
////                 index--;
////                 cout << "outisde loop" << endl;
//                 if(c == '.'){
//                     value += c;
//                       c = mystring[index++];
//                     while(c >= '0' && c <= '9'){
//                         value+= c;
//                         if(index >= mystring.length())break;
//                         c = mystring[index++];
//                     }
//                        token.type = tok_float;
////                     while(c != ' ' && c != '\n' && c != '+' && c != '*' && c != '/' && c != ';'){
////                        token.type = invalid_float;
////                         value += c;
////                          c = mystring[index++];
////                        if(index >= mystring.length())break;
////                     }
////                     c = mystring[index++];
//                        index--;
//                    //  cout << "its a float"<<endl;
//                         token.value = value;
//                     return token;
//                 }
//                     token.type = tok_integer;
////                     while(c != ' ' && c != '\n' && c != '+' && c != '*' && c != '/' && c != ';'){
////                        token.type = invalid_int;
////                         value += c;
////                         mystream >> c;
//////                         cout << "u are corrput" << endl;
////                         if(index >= mystring.length())break;
////                     }
////                   c = mystring[index++];
//                     index--;
//                //  cout << "its an integer"<<endl;
//                 token.value = value;
////                 cout << "Value in scanner is : " << value << endl;
//                return token;
//             }
//            else if (c == '\t' || c == ' '){
//                // cout << "its a space" << endl;
//                token.type = kw_ignore;
//                return token;
//            }
//            else if(c == '#'){
//                   c = mystring[index++];
//                  if(c == '#'){
//                  while(c != '\n' && c != '\r'){
//                      if(index >= mystring.length())break;
//                       c = mystring[index++];
//                 }
////                mystream <<  c;
//                //  cout <<"its a comment"<<endl;
//                 token.type = kw_ignore;
//                return token;
//                  }
//            }
//            else if (c == '\n' || c == '\r'){
//                // cout << "its a new line"<<endl;
//                token.type = lx_newline;
//                return token;
//            }
//             else if (c == ':'){
//                 value +=c;
//                 if(index < mystring.length())
//                 c = mystring[index++];
//                 if(c == '='){
//                     value+=c;
//                     token.type = Ix_colon_eq;
//                     return token;
//                 }
//                 index--;
//             }
//             else if (c == '>'){
//                 value +=c;
//                 if(index < mystring.length())
//                 c = mystring[index++];
//                 if(c == '='){
//                     value+=c;
//                     token.type = lx_ge;
//                     return token;
//                 }
//                 index--;
//             }
//             else if (c == '<'){
//                 value +=c;
//                 if(index < mystring.length())
//                 c = mystring[index++];
//                 if(c == '='){
//                     value+=c;
//                     token.type = lx_le;
//                     return token;
//                 }
//                 index--;
//             }
//             else if (c == '!'){
//                 value +=c;
//                 if(index < mystring.length())
//                 c = mystring[index++];
//                 if(c == '='){
//                     value+=c;
//                     token.type = lx_neq;
//                     return token;
//                 }
//                 index--;
//             }
//        // cout << "the value is undefined" << endl;
//        c = mystring[index - 1];
//        std::cout << "token.value is " << token.value << std::endl;
//        token.value = c;
//        token.type = undefined;
//        for(int i = 0 ; i <19;i++){
//         if(token.value == Operators[i]){
//            // cout << Operators[i] << endl;
//            token.type = i + 37;
//            // cout << token.value << ":";
//            // cout <<  Opera_def[i] <<  endl;
//         }
//    }
//        return token;

//        //}

//    };

///////////////////////////////////////////////// Scanning From File //////////////////////////////////////////////////////


    TOKEN Scan(FILE * fptr,char * filename) {
        string value;
        char c;

        TOKEN token;
            // while (fscanf(fptr,"%c", &c)) {
            fscanf(fptr,"%c", &c);
             if(feof(fptr)){
                 token.type = eof;
                 return token;
             }
             if(c >= 'a' && c <= 'z'){
                //  cout << "we entered the first loop" << endl;
                 while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'){
                    //  cout << "we entered the second loop"<<endl;
                     value += c;
                    fscanf(fptr,"%c", &c);
                 }
                 fseek(fptr, -1, SEEK_CUR);
                //  cout <<"its an ident"<<endl;
                 token.value = value;
                 token.type = tok_ident;
                 int length = sizeof(Identifiers)/ sizeof(Identifiers[0]);
                 for(int i = 0 ; i < length ;i++){
                  if(token.value == Identifiers[i]){
                     token.type = i + 6;
                     // cout << token.value << ":";
                     // cout <<  Identifiers[i]<<"Keyword"<< endl;
                  }
                 }
                return token;
             }
            else if( c == '"'){
                     value += c;
                      fscanf(fptr,"%c", &c);
                        if (feof(fptr)){
                        // cout << "we at aend boys" << endl;
                        token.type = eof;
                        return token;
                    }
                    //   cout << "the new value of c is " << c << endl;
                   while(c != '"'){
                        if(c == '\n'){
                        // cout << "ok imma headout" << endl;
                        token.type = inivalid_str;
                        return token;
                    }                            //  cout << "we entered the second loop"<<endl;
                     value += c;
                      fscanf(fptr,"%c", &c);
                 }
                if(c == '"'){
                value += c;
                //  cout <<"its a string"<<endl;
                 token.value = value;
                 token.type = tok_string;
                return token;
                }
                fseek(fptr, -1, SEEK_CUR );
             token.value = value;
             token.type = inivalid_str;
            return token;
            }
             else if(c >= '0' && c <= '9'){
                 while(c >= '0' && c <= '9'){
                    value += c;
                      fscanf(fptr,"%c", &c);
                 }
                 if(c == '.'){
                     value += c;
                      fscanf(fptr,"%c", &c);
                     while(c >= '0' && c <= '9'){
                         value+= c;
                          fscanf(fptr,"%c", &c);
                     }
                        token.type = tok_float;
                     while(c != ' ' && c != '\n' && c != '+' && c != '*' && c != '/' && c != ';'){
                        token.type = invalid_float;
                         value += c;
                         fscanf(fptr,"%c", &c);
                     }
                     fseek(fptr, -1, SEEK_CUR );
                    //  cout << "its a float"<<endl;
                         token.value = value;
                     return token;
                 }
                     token.type = tok_integer;
                     while(c != ' ' && c != '\n' && c != '+' && c != '*' && c != '/' && c != ';'){
                        token.type = invalid_int;
                         value += c;
                         fscanf(fptr,"%c", &c);
                     }
                   fseek(fptr, -1, SEEK_CUR );
                //  cout << "its an integer"<<endl;
                 token.value = value;
                return token;
             }
            else if (c == '\t' || c == ' '){
                // cout << "its a space" << endl;
                token.type = kw_ignore;
                return token;
            }
            else if(c == '#'){
                  fscanf(fptr,"%c", &c);
                  if(c == '#'){
                  while(c != '\n' && c != '\r'){
                      fscanf(fptr,"%c", &c);
                 }
                fseek(fptr, -1, SEEK_CUR );
                //  cout <<"its a comment"<<endl;
                 token.type = kw_ignore;
                return token;
                  }
            }
            else if (c == '\n' || c == '\r'){
                // cout << "its a new line"<<endl;
                token.type = lx_newline;
                return token;
            }
            else if (c == ':'){
//                cout << "i`m colon" << endl;
                value+=c;
                fscanf(fptr,"%c", &c);
                if(c == '='){
                    value+=c;
                    token.type = Ix_colon_eq;
//                    cout << "i`m colon eq " << endl;
                    return token;
                }
                fseek(fptr, -2, SEEK_CUR );
                fscanf(fptr,"%c", &c);
            }
            else if (c == '>'){
                value+=c;
                fscanf(fptr,"%c", &c);
                if(c == '='){
                    value+=c;
                    token.type = lx_ge;
                    return token;
                }
                fseek(fptr, -2, SEEK_CUR );
                fscanf(fptr,"%c", &c);
            }
            else if (c == '<'){
                value+=c;
                fscanf(fptr,"%c", &c);
                if(c == '='){
                    value+=c;
                    token.type = lx_le;
                    return token;
                }
                fseek(fptr, -2, SEEK_CUR );
                fscanf(fptr,"%c", &c);
            }
            else if (c == '!'){
                value+=c;
                fscanf(fptr,"%c", &c);
                if(c == '='){
                    value+=c;
                    token.type = lx_neq;
                    return token;
                }
                fseek(fptr, -2, SEEK_CUR );
                fscanf(fptr,"%c", &c);
            }
//
//         cout << "the operator is" << c <<  endl;
        token.value = c;
        token.type = undefined;
        for(int i = 0 ; i <19;i++){
         if(token.value == Operators[i]){
            // cout << Operators[i] << endl;
            token.type = i + 37;
            // cout << token.value << ":";
            // cout <<  Opera_def[i] <<  endl;
         }
    }
        return token;

        //}

    };



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







};



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_parse_clicked();

private:
    Ui::MainWindow *ui;
    QString qs;
    std::stringstream ss;

    int offset;
    int index ;
    int lineno;
    FILE * fptr;
    TOKEN token;
    char* filepath;

    SCANNER *scanner;
    SymbolTable * table;
    void get_token();
    void wr_error(QString qs);

    ast_node* parseProgram();
    ast_list *parseDec_list();
    ast_node* parseDec();
    ast_node* parseStmt(SymbolTable *l_table);
    ast_node* parseIf_stmt(SymbolTable *l_table);
    ast_node* parseId_stmt(SymbolTable *l_table);
    ast_list* parseVar_Declist(SymbolTable *l_table);
    ast_node* parseVar_Dec();
    ast_list* parseStmtlist(SymbolTable *l_table);
    ast_node* parseExpr_E(SymbolTable * l_table);
    ast_node* parseExpr_E_star(ast_node* Fb,SymbolTable * l_table);
    ast_node* parseExpr_T(SymbolTable * l_table);
    ast_node* parseExpr_T_star(ast_node* Fb,SymbolTable * l_table);
    ast_node* parseExpr_F(SymbolTable * l_table);
    ast_node* parseExpr_F_star(ast_node* Fb,SymbolTable * l_table);
    ast_node* parseExpr_G(SymbolTable * l_table);
    ast_node* parseExpr_G_star(ast_node* Fb,SymbolTable * l_table);
    ast_node* parseExpr_H(SymbolTable * l_table);
    ast_node* parseExpr_I(SymbolTable *l_table);

    void PrinTree(ast_node* node);
    void printExpr(ast_node* node);
    void PrintStmt(ast_node* node);

    void genTree(ast_node* node);
    void genExpr(ast_node * node);
    void genStmt(ast_node* node);

};




#endif // MAINWINDOW_H
