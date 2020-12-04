
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include<stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

string keyword[] = {"identifier","integer","String","Float","EOF","Igonored",
"program", "var", "constant","integer", "boolean",
"string", "float", "true","false", "if", "fi", "then", "else",
"while", "do", "od", "and", "or", "read", "write",
"for", "from","to", "by", "function", "procedure","return",
"not", "begin", "end","undefined","lparen","rparen","lbracket","rpracket","colon","dot","semicolon","comma","colon_eq","plus","minus","star",
"slash","equal","not_equal","less_than","less_than_equal","greater_than","greater_than_equal"
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_parse_clicked()
{

    table = new SymbolTable();
    scanner = new SCANNER();

    offset = 0;
    index = 1;
    lineno = 0;
//   filepath = "C:/Users/Bashar Sader/Desktop/test_parser.txt";
    filepath = (char *)ui->lineEdit->text().toStdString().c_str();

      if ((fptr = fopen(filepath,"r")) == NULL){
          printf("Error! opening file");
          // Program exits if the file pointer returns NULL.
          exit(1);
      }


    cout << "Starting the Parser" << endl;
    get_token();
    ast_node* prog= parseProgram();
//    PrinTree(prog);
    genTree(prog);

    cout << "remain token type is " << keyword[token.type] << endl;
}

ast_node* MainWindow::parseProgram(){
    ast_node* node = new ast_node();

    node->type = kw_program;

    node->f.a_program.decs = parseDec_list();
    node->f.a_program.stmts = parseStmtlist(table);

    return node;
}


ast_list* MainWindow:: parseDec_list(){

//        cout << "inside dec list" << endl;
        ast_list* node_list = new ast_list();
        ast_node* node = new ast_node();
        if(token.type == kw_var){
        node = parseDec();
//        cout << "returned from dec" << endl;
        if(table->Put_symbol(node->f.a_var_decl.name) == NULL){
            wr_error("id already defined");
//            cout << "id already defnied" << endl;
        }
//        l_table->Put_symbol(node->f.a_var_decl.name);

        if(token.type == lx_semicolon){
//            cout << "semicolon" << endl
            get_token();
            node_list->head = node;
            node_list->tail = parseDec_list();
        }
        else wr_error("Expected ;");
        }

        else if(token.type == kw_constant){
            node = parseDec();
    //        cout << "returned from dec" << endl;
            if(table->Put_symbol(node->f.a_const_decl.name) == NULL){
                wr_error("id already defined");
    //            cout << "id already defnied" << endl;
            }
    //        l_table->Put_symbol(node->f.a_var_decl.name);

            if(token.type == lx_semicolon){
                get_token();
                node_list->head = node;
                node_list->tail = parseDec_list();
            }
            else wr_error("Expected ;");
            }
//        cout << "returning list" << endl;
        return node_list;
 }









ast_node* MainWindow::parseDec() //E = F#2 Eb
{
//cout << "inside general  :"  << keyword[token.type]<< endl;
//    cout << "index of var is " << kw_var << " current type index is " << token.type << endl;

    ast_node * node = new ast_node();
    TableEntry * entry = new TableEntry();
    if(token.type == kw_var){                       //TODO : create a symbol table entry for the variable dec
        node->type = kw_var;
        entry->entry_type = ste_var;
//        cout << "inside 1" << endl;
        get_token();
        if(token.type == tok_ident){
//            cout << "inside 2" << endl;
            entry->name = token.value;
            get_token();
            if(token.type == Ix_colon){
//                cout << "inside 3" << endl;
                get_token();
                if(token.type == kw_integer){
                    entry->var.type = type_integer;
                    get_token();
//                     cout << "inside int" << endl;
//                    node->f.a_var_decl.name =
                    node->f.a_var_decl.type = type_integer;
                }
                else if(token.type == kw_string){
                    entry->var.type = type_string;
                    get_token();
//                     cout << "inside str" << endl;
                    node->f.a_var_decl.type = type_string;
                }
                else if(token.type == kw_boolean){
                    entry->var.type = type_boolean;
                    get_token();
//                     cout << "inside bool" << endl;
                    node->f.a_var_decl.type = type_boolean;
                }
               else {
                    wr_error("Invalid Type");
//                    throw "error";
                }
//               if(table->Put_symbol(entry) == NULL){
//                   wr_error("id already defnied");
////                   cout << "id already defnied" << endl;
////                   throw "error";
//               }
                entry->offset = offset;
                offset += 4;
               node->f.a_var_decl.name = entry;
            }
            else {
                wr_error("expected column");
//                throw "expected column";
            }
        }
       else {
            wr_error("expected an identifier");
//            throw "expected an identifier";
        }
    }

    else if (token.type == kw_constant){   //TODO: create a symbol table entry for the constant decl
//        cout << "inside constant" << endl;
        entry->entry_type = ste_const;
        get_token();
        if(token.type == tok_ident){
//        cout << "inside id" << endl;
         entry->name = token.value;
            get_token();
            if(token.type == lx_eq){
//         cout << "inside equel" << endl;
                get_token();
                node->type =kw_constant;
//                table->Put_symbol(entry);
//                 cout << "before parsing E" << endl;
                entry->offset = offset;
                offset += 4;
                node->f.a_const_decl.exp_value = parseExpr_E(table);
                node->f.a_const_decl.name = entry;
//                cout << "equel node :"<< node->type << "right child is : " <<  node->f.a_assign.rhs->type << endl;
//                node = node->f.a_assign.rhs;

//                ast_node *node1 = node->f.a_binary_op.larg;  ast_node *node2= node->f.a_binary_op.rarg;
//                cout << "node :"<< node1->type << "left child is : " <<  node1->f.a_binary_op.larg->type <<  " right child is : " << node1->f.a_binary_op.rarg->type << endl;
//                cout << "node :"<< node2->type << "child is : " <<  node2->f.a_unary_op.arg->type << endl;
//                ast_node *node1 = node->f.a_binary_op.larg;  ast_node *node2= node->f.a_binary_op.rarg;
//                cout << "node :"<< node1->type << "left child is : " <<  node1->f.a_binary_op.larg->type <<  " right child is : " << node1->f.a_binary_op.rarg->type << endl;
//                cout << "node :"<< node2->type << "left child is : " <<  node2->f.a_binary_op.larg->type <<  " right child is : " << node2->f.a_binary_op.rarg->type << endl;
                // parse expr E ;
//                if(node->f.a_assign.rhs == NULL){
//                    throw "invalid aassignment"
//                }
            }
            else{
                wr_error("expecteds equel operator");
//                throw ("error");
            }
        }
        else {
            wr_error("Expected an identifier");
        }
    }
    else {
        wr_error("Invalid Decleration");
    }
    return node;
}



ast_node* MainWindow::parseStmt(SymbolTable * l_table){

    ast_node* node = new ast_node();
    if(token.type == tok_ident){
//        cout << "ident dec" << endl;
        TableEntry * entry = new TableEntry();
        entry = l_table->Get_symbol(token.value);
        if(entry == NULL){
            if(table->Get_symbol(token.value) != NULL){
                entry = table->Get_symbol(token.value);
            }
            else
            wr_error("id doesnt exist");
        }
        get_token();
        node = parseId_stmt(l_table);
//        cout << "my right hand side is :" << node->f.a_assign.rhs->type << endl;
        node->f.a_assign.lhs = entry;
    }

    else if(token.type == kw_if){
//        cout << "if" << endl;
        get_token();
        node->type = kw_if;
        node->f.a_if.predicate = parseExpr_E(l_table);
        if(token.type == kw_then){
//            cout << "then" << endl;
            get_token();
            node->f.a_if.conseq = parseStmt(l_table);
            node->f.a_if.altern = parseIf_stmt(l_table);
        }
        else {wr_error("expected then");}
//    cout << "if node :"<< node->type <<"predicate is " << node->f.a_if.predicate->type <<  "conseq is : " <<  node->f.a_if.conseq->type <<  " altern child is : " << node->f.a_if.altern->type << endl;
    }

    else if(token.type ==kw_while){
        get_token();
        node->type = kw_while;
        node->f.a_while.predicate = parseExpr_E(l_table);
        if(token.type == kw_do){
             get_token();
             node->f.a_while.body = parseStmt(l_table);
             if(token.type != kw_od){
                wr_error("expected od");
             }
             get_token();
        }
        else wr_error("expected do");
// cout << "while node :"<< node->type <<"predicate is " << node->f.a_while.predicate->type <<  "body is : " <<node->f.a_while.body->type << endl;
    }


    else if(token.type ==kw_write){
              string val;
            node->type = kw_write;
            get_token();
            if(token.type == lx_Iparen){
                get_token();
                if(token.type != tok_ident){
                        wr_error("Expected ident");
                }
                TableEntry * entry = new TableEntry();
                entry = l_table->Get_symbol(token.value);
                if(entry == NULL){
                    if(table->Get_symbol(token.value) != NULL){
                        entry = table->Get_symbol(token.value);
                    }
                    else
                    wr_error("id doesnt exist");
                }
                node->f.a_write.var = entry;
                val = token.value;
                get_token();
                if(token.type != lx_rparen){
                    wr_error("Expected Right parenthesis");
                }
                get_token();
            }
            else wr_error("Expected left parenthesis");
//            cout << "write operation" << node->type << "with enrty :" << val << endl;
    }


    else if(token.type ==kw_read){
        string val;
        node->type = kw_read;
        get_token();
        if(token.type == lx_Iparen){
            get_token();
            if(token.type != tok_ident){
                wr_error("Expected Identifier");
            }
            TableEntry * entry = new TableEntry();
            entry = l_table->Get_symbol(token.value);
            if(entry == NULL){
                if(table->Get_symbol(token.value) != NULL){
                    entry = table->Get_symbol(token.value);
                }
                else
                wr_error("id doesnt exist");
            }
            node->f.a_read.var = entry;
            val = token.value;
            get_token();
            if(token.type != lx_rparen){
               wr_error("Expected Right parenthesis");
            }
            get_token();
        }
        else wr_error("Expected left parenthesis");
//        cout << "read operation" << node->type << "with enrty :" << val << endl;
    }


    else if(token.type == kw_for){
        node->type = kw_for;
        get_token();
        if(token.type != tok_ident){
            wr_error("Expected Identifier");
        }
        TableEntry * entry = new TableEntry();
        entry = l_table->Get_symbol(token.value);
        if(entry == NULL){
            if(table->Get_symbol(token.value) != NULL){
                entry = table->Get_symbol(token.value);
            }
            else
            wr_error("id doesnt exist");
        }
        node->f.a_for.var = entry;
        get_token();
        if(token.type != Ix_colon_eq){
             wr_error("Expected :=");
        }
        get_token();
        node->f.a_for.lower_bound = parseExpr_E(l_table);
        if(token.type != kw_to){
            wr_error("Expected to");
        }
        get_token();
        node->f.a_for.upper_bound = parseExpr_E(l_table);
        if(token.type != kw_do){
            wr_error("Expected do");
        }
        get_token();
        node->f.a_for.body = parseStmt(l_table);
        if(token.type != kw_od){
           wr_error("Expected od");
        }
        get_token();
//    cout << "for node :"<< node->type <<"lower is " <<  node->f.a_for.lower_bound->type <<  "upper is : " <<  node->f.a_for.upper_bound->type <<  "body is : " <<  node->f.a_for.body->type << endl;
    }
    else if (token.type == kw_begin){
//        cout << "inside begin" << endl;
        node->type = kw_block;
        get_token();
        node->f.a_block.BTable = new SymbolTable();
        node->f.a_block.vars = parseVar_Declist(node->f.a_block.BTable);
        node->f.a_block.stmts = parseStmtlist(node->f.a_block.BTable);
        if(token.type != kw_end){
            wr_error("Expected end");
        }
//        cout << "end" << endl;
//        node->f.a_block.BTable->print_all();
//           ast_list* print = node->f.a_block.vars;
//           while(print->head != NULL){
//               cout << "var node: " << print->head->f.a_var_decl.type << endl;
//               print = print->tail;
//           }
//           print = node->f.a_block.stmts;
//           while(print->head != NULL){
//               cout << "stmt node: " << print->head->type << endl;
//               print = print->tail;
//           }

        get_token();
    }
    else wr_error("Invalid Statement");

    return node;
}


ast_node* MainWindow::parseId_stmt(SymbolTable * l_table){
//    cout << "parseID" << endl;
    ast_node* node = new ast_node();
//    cout << "true" << Ix_colon_eq << "false" << token.type << endl;
    if(token.type == Ix_colon_eq){
//        cout << ":=" << endl;
        get_token();
        node->type = lx_eq;
//                 cout << "before parsing E" << endl;
        node->f.a_assign.rhs = parseExpr_E(l_table);
    }
    else wr_error("Expected :=");
    return node;
}

ast_node* MainWindow::parseIf_stmt(SymbolTable * l_table){
//    cout << "parseIF"<<endl;
    ast_node* node = new ast_node();
    if(token.type == kw_fi){
//        cout << "fi" << endl;
        get_token();
    }
    else if(token.type == kw_else){
//        cout << "else" << endl;
        get_token();
        node = parseStmt(l_table);
        if(token.type != kw_fi){
          wr_error("Expected fi");
        }
//        cout << "fi done" << endl;
        get_token();
    }
    else wr_error("Invalid If Statement");
    return node;
}


 ast_node* MainWindow::parseExpr_E(SymbolTable * l_table) //E = F#2 Eb
{       ast_node * node = new ast_node();

        node = parseExpr_T(l_table);
        node = parseExpr_E_star(node,l_table);
        return node;

}

 ast_node* MainWindow::parseExpr_E_star(ast_node* E,SymbolTable * l_table) //Eb =  +F #1 Eb or lambda
{
//       cout << "E star" << endl;
  if (token.type == kw_and || token.type == kw_or)
  {     ast_node * node = new ast_node();
//      cout << "plus operation" << endl;
      node->type = token.type;
      node->f.a_binary_op.larg = E;
      get_token();
      node->f.a_binary_op.rarg = parseExpr_T(l_table);
      node = parseExpr_E_star(node,l_table);
      return node;
  }

  return E;
}

 ast_node* MainWindow::parseExpr_T(SymbolTable * l_table) //E = F#2 Eb
{
//     cout << "T" << endl;
        ast_node * node = new ast_node();

        node = parseExpr_F(l_table);
        node = parseExpr_T_star(node,l_table);
        return node;
}

 ast_node* MainWindow::parseExpr_T_star(ast_node* T,SymbolTable * l_table) //Eb =  +F #1 Eb or lambda
 {
//         cout << "T star" << endl;
//         cout << "current type is " << token.type << "True" << lx_gt << endl;
   if (token.type == lx_eq || token.type == lx_neq || token.type == lx_lt || token.type == lx_le || token.type == lx_gt|| token.type == lx_ge)
   {    ast_node * node = new ast_node();
//       cout << "comp operation" << endl;
       node->type = token.type;
       node->f.a_binary_op.larg = T;
       get_token();
       node->f.a_binary_op.rarg = parseExpr_F(l_table);
       node = parseExpr_T_star(node,l_table);
       return node;
   }

   return T;
}

 ast_node* MainWindow::parseExpr_F(SymbolTable * l_table) //E = F#2 Eb
{         ast_node * node = new ast_node();
//         cout << "F" << endl;
          node = parseExpr_G(l_table);
          node = parseExpr_F_star(node,l_table);
//            cout << "plus node :"<< node->type << "left child is : " <<  node->f.a_binary_op.larg->type <<  " right child is : " << node->f.a_binary_op.rarg->type << endl;
          return node;
}

 ast_node* MainWindow::parseExpr_F_star(ast_node* F,SymbolTable * l_table) //Eb =  +F #1 Eb or lambda
 {
//    cout << "inside F star" << endl;
   if (token.type == lx_plus || token.type == lx_minus)
   {   ast_node * node = new ast_node();
//       cout << "inside Plus" << endl;
       node->type = token.type;
       node->f.a_binary_op.larg = F;
       get_token();
       node->f.a_binary_op.rarg = parseExpr_G(l_table);
       node = parseExpr_F_star(node,l_table);

 //      cout << "plus operation" << endl;
       return node;
   }

   return F;
}

 ast_node* MainWindow::parseExpr_G(SymbolTable * l_table) //E = F#2 Eb
 {         ast_node * node = new ast_node();
//            cout << "G" << endl;
           node = parseExpr_H(l_table);
           node = parseExpr_G_star(node,l_table);
//           cout << " node :"<< node->type << "left child is : " <<  node->f.a_binary_op.larg->type <<  " right child is : " << node->f.a_binary_op.rarg->type << endl;
           return node;
}

 ast_node* MainWindow::parseExpr_G_star(ast_node* G,SymbolTable * l_table) //Eb =  +F #1 Eb or lambda
 {
//        cout << "G star" << endl;
       if (token.type == lx_star || token.type == lx_slash)
       {    ast_node * node = new ast_node();
//           cout << "multi operation" << endl;
           node->type = token.type;
           node->f.a_binary_op.larg = G;
           get_token();
           node->f.a_binary_op.rarg = parseExpr_H(l_table);
           node = parseExpr_G_star(node,l_table);
           return node;
       }

       return G;
}


 ast_node* MainWindow::parseExpr_H(SymbolTable * l_table) //Eb =  +F #1 Eb or lambda
{   ast_node * node = new ast_node();
//    cout << "H" << endl;
//    get_token();
    if(token.type == kw_not){
        node->type = kw_not;
        get_token();
        node->f.a_unary_op.arg = parseExpr_I(l_table);
//        node->f.a_unary_op.type =  node->f.a_unary_op.arg->type; // i`m not sure about that but do an if statement for each j_type returned
    }
    else if (token.type == lx_minus){
        node->type = lx_uminus;
        get_token();
        node->f.a_unary_op.arg = parseExpr_I(l_table);
    }
    else node = parseExpr_I(l_table);

    return node;
}


 ast_node* MainWindow::parseExpr_I(SymbolTable * l_table) //E = F#2 Eb
{
//     cout << "I" << endl;
     ast_node * node = new ast_node();
     if(token.type == tok_ident){
//         cout <<"ident" << endl;
         if(l_table->Get_symbol(token.value) == NULL){
             if(l_table->Get_symbol(token.value) == NULL){
                   wr_error("id doesnt exist");
             }
//             cout << "it doesnt exist" << endl;
         }
         node->type = tok_ident;
         node->f.a_var.var = l_table->Get_symbol(token.value);
         get_token();

     }
    else if(token.type == tok_integer){
//        cout << "int" << endl;
         stringstream ss(token.value);
         get_token();
         node->type = tok_integer;
//         node->f.a_integer.value = stoi(token.value);  // convert to integer
         ss >> node->f.a_integer.value;
     }
     else if (token.type == tok_string){
//          cout << "str" << endl;
         node->type = tok_string;/*
         cout << "before copying val is : " << token.value << endl;*/
//         strcpy(node->f.a_string.string,(char *)token.value.c_str());        //check if this actually works
         node->f.a_string.string = (char *)token.value.c_str();
         get_token();

     }
     else if (token.type == kw_true){ //BOOLEAN
         get_token();
         node->type = kw_true;
         node->f.a_boolean.value = 1;
     }
     else if (token.type == kw_false){ //BOOLEAN
         get_token();
         node->type = kw_false;
         node->f.a_boolean.value = 0;
     }
     else if(token.type == lx_Iparen){
         get_token();
         node = parseExpr_E(l_table);
//         get_token();
         if(token.type != lx_rparen){
             wr_error("Expected right parenthesis");
         }
         get_token();
     }
     else wr_error("Invalid Expression");;
    return node;
}


 ////////////////////////////////// Block //////////////////////////////////

ast_list* MainWindow:: parseVar_Declist(SymbolTable *l_table){

//        cout << "inside dec list" << endl;
        ast_list* node_list = new ast_list();
        ast_node* node = new ast_node();
        if(token.type == kw_var){
        node = parseVar_Dec();
//        cout << "returned from dec" << endl;
        if(l_table->Put_symbol(node->f.a_var_decl.name) == NULL){
            wr_error("id already defined");
//            cout << "id already defnied" << endl;
        }
//        l_table->Put_symbol(node->f.a_var_decl.name);

        if(token.type == lx_semicolon){
            get_token();
            node_list->head = node;
            node_list->tail = parseVar_Declist(l_table);
        }
        else wr_error("Expected ;");
        }
//        cout << "returning list" << endl;
        return node_list;
 }





ast_node* MainWindow:: parseVar_Dec(){
//        cout <<"in Dec" << endl;
        ast_node * node = new ast_node();
        if(token.type == kw_var){                       //TODO : create a symbol table entry for the variable dec
            node->type = kw_var;
            TableEntry * entry = new TableEntry();
            entry->entry_type = ste_var;
//            cout << "inside 1" << endl;
            get_token();
            if(token.type == tok_ident){
//                cout << "inside 2" << endl;
                entry->name = token.value;
                get_token();
                if(token.type == Ix_colon){
//                    cout << "inside 3" << endl;
                    get_token();
                    if(token.type == kw_integer){
                        entry->var.type = type_integer;
                        get_token();
//                         cout << "inside int" << endl;
    //                    node->f.a_var_decl.name =
                        node->f.a_var_decl.type = type_integer;
                    }
                    else if(token.type == kw_string){
                        entry->var.type = type_string;
                        get_token();
//                         cout << "inside str" << endl;
                        node->f.a_var_decl.type = type_string;
                    }
                    else if(token.type == kw_boolean){
                        entry->var.type = type_boolean;
                        get_token();
//                         cout << "inside bool" << endl;
                        node->f.a_var_decl.type = type_boolean;
                    }
                   else wr_error("Invalid type");
                   entry->offset = offset;
                   offset += 4;
                   node->f.a_var_decl.name = entry;
                }
                else wr_error("Expected :");
            }
           else wr_error("Expected an identifier");
        }
//    cout << "returning from dec" << endl;
    return node;
}



ast_list* MainWindow:: parseStmtlist(SymbolTable * l_table){
//    cout << "in Stmt List" << endl;
    ast_list* node_list = new ast_list();
    ast_node* node = new ast_node();
    if(token.type == tok_ident || token.type == kw_if || token.type == kw_while || token.type == kw_for || token.type == kw_read || token.type == kw_write || token.type == kw_begin){
    node = parseStmt(l_table);
    if(token.type == lx_semicolon){
        get_token();
        node_list->head = node;
        node_list->tail = parseStmtlist(l_table);
    }
    else wr_error("Expected ;");
    }
    return node_list;
}




 void MainWindow::wr_error(QString qs)
 {
//     ui->label_2->setText(qs);
       cout << qs.toStdString() << endl;
       throw "error";
 }





// void MainWindow::get_token(){

////    stringstream token_s(qs.toStdString());
//    token = scanner->Scan(qs.toStdString(),token,index);
////    cout << "this is the real lparan  " << lx_Iparen << "  this is the fake lpaen" << token.type << endl;
//    if(token.type == kw_ignore){
////        cout <<"i`m in space" << endl;
//        get_token();
//    }
////       cout << "current token value" << token.value << endl;
////    cout <<"current index is : " << index <<endl;
//    if(token.type == kw_ignore){

//    }

// }


 void MainWindow::get_token()
 {

        token = scanner->Scan(fptr,filepath);
            if(token.type == eof){
//                cout << "reached End of File" << endl;
                return;
            }
            else if (token.type == kw_ignore)
            get_token();
            if (token.type == lx_newline){
                        lineno++;
                        get_token();
                        return;
            }
            if(token.type == invalid_float){
                cout << "Error: Bad floating point number on line" << lineno <<"of prog"<<endl;
               return;
            }
            else if (token.type == invalid_int){
                cout << "Error: Bad integer number on line" << lineno <<"of prog"<<endl;
                return;
            }
                else if (token.type == inivalid_str){
                cout << "Error: Bad String on line" << lineno <<"of prog"<<endl;
                return;
            }

 }



 void MainWindow::PrinTree(ast_node * program)
 {
    cout << "program \n{\n" << endl;
     ast_list * Declerations = program->f.a_program.decs;
     cout << "Declerations \n{\n" << endl;
     while(Declerations->head != NULL){
        if(Declerations->head->type == kw_var){
                cout << "Variable Dec \n{\n\n";
                cout << "name : " <<  Declerations->head->f.a_var_decl.name->name << endl;
                cout << "type : " << Declerations->head->f.a_var_decl.type << endl;
                cout << "} //End of Variable Dec \n\n";
        }
        else if (Declerations->head->type = kw_constant){
            cout << "Constant Dec {\n\n";
            cout << "name : " <<  Declerations->head->f.a_const_decl.name->name << endl;
            cout << "value : \n{\n\n";
            printExpr(Declerations->head->f.a_const_decl.exp_value);
            cout << "\n} //End of value \n\n";
            cout << "\n} //End of Constand Dec \n\n";
        }
          Declerations = Declerations->tail;
     }
     cout << "\n} //End of Declerations \n\n";
//     cout << "out of decs" << endl;
     ast_list * Stmts = program->f.a_program.stmts;
     cout << "Statements \n{" << endl;
     while(Stmts->head != NULL){
          PrintStmt(Stmts->head);
          Stmts = Stmts->tail;
     }
//     cout << "out of stmts" << endl;
     cout << "\n} // End of Statements \n\n";
     cout << "\n} //End of Program \n\n";
//     cout << "Done" <<endl;

 }


 void MainWindow::PrintStmt(ast_node *node){
     if(node->type == lx_eq){
         cout << "\nAssignment stmnt \n{\n\n";
         cout << "Variable (lhs) : " << node->f.a_assign.lhs->name << endl;
         cout << "Expression (rhs) : \n{\n\n";
         printExpr(node->f.a_assign.rhs);
          cout << "\n} // End of Expression(rhs) \n\n";
         cout << "\n} //End of Assignment Stmt \n\n";
         return;
     }
     else if(node->type == kw_if){
         cout << "if stmnt \n{\n\n";
         cout << "Predicate \n{\n\n";
         printExpr(node->f.a_if.predicate);
         cout << "\n} //End of Prediacate \n\n";
         cout << "Consequence \n{\n\n";
         PrintStmt(node->f.a_if.conseq);
         cout << "\n} //End of Conseq \n\n";
         cout << "Alternative \n{\n\n";
//         if(node->f.a_if.altern == NULL){
//             cout << "Null" << endl;
//         }
//         else
         PrintStmt(node->f.a_if.altern);
         cout << "\n} //End of Alternative \n\n";
         cout << "\n} //End of IF \n\n";
         return;
     }
     else if (node->type == kw_for){
         cout << "for stmnt \n{\n\n";
         cout << "variable : " << node->f.a_for.var->name << endl;
         cout << "Lower bound \n}\n\n";
         printExpr(node->f.a_for.lower_bound);
         cout << "\n} // End of Lower bound \n\n";
         cout << "Upper bound \n}\n\n";
         printExpr(node->f.a_for.upper_bound);
         cout << "\n} // End of Upper bound \n\n";
         cout << "body \n{\n\n";
         PrintStmt(node->f.a_for.body);
         cout << "\n} // End of Body \n\n";
         cout << "\n} //End of For \n\n";
         return;
     }
     else if (node->type == kw_while){
         cout << "while stmnt \n{\n\n";
         cout << "Predicate \n{\n\n";
         printExpr(node->f.a_while.predicate);
         cout << "\n} // End of predicate \n\n";
         cout << "body \n{\n\n";
         PrintStmt(node->f.a_while.body);
         cout << "\n} // End of Body \n\n";
         cout << "\n} // End of While \n\n";
         return;
     }
    else if (node->type == kw_read){
        cout << "read stmt \n{\n\n";
        cout << "variable (Parameter): " << node->f.a_read.var->name << endl;
        cout << "\n} // End of Read \n\n";
        return;
     }
     else if (node->type == kw_write){
         cout << "write stmt \n{\n\n";
         cout << "variable (Parameter): " << node->f.a_write.var->name << endl;
         cout << "\n} // End of Write \n\n";
         return;
      }

     else if(node->type == kw_block){

        ast_list* print = node->f.a_block.vars;
        cout << "Block stmt \n{\n\n";
        cout << "Variables \n{\n\n";
        while(print->head != NULL){
            cout << "Variable Dec \n{\n\n";
            cout << "name : " <<  print->head->f.a_var_decl.name->name << endl;
            cout << "type : " << print->head->f.a_var_decl.type << endl;
            cout << "\n} // End of Variable Dec \n\n";
            print = print->tail;
        }
        cout << "\n} //End of Variables \n\n";
        print = node->f.a_block.stmts;
        cout << "Statements \n{\n\n";
        while(print->head != NULL){
            PrintStmt(print->head);
            print = print->tail;
        }
        cout << "\n} // End of Statements \n\n";
        cout << "\n} // End of Block \n\n";
     }

 }
 void MainWindow::printExpr(ast_node *node){
        cout << "" << endl;
        if(node->type == kw_not || node->type == lx_uminus){
                cout << "Negate Operation \n{\n\n";
               cout << "Argument \n{\n";
               printExpr(node->f.a_unary_op.arg);
               cout << "\n} // End of Argument \n\n";
               cout << "\n} // End of Negate Operation \n\n";
               return;
        }
        else if (node->type == tok_ident){
            cout << "Identifier \n{\n\n";
            cout << "name: " << node->f.a_var.var->name;
            cout << "\n} // End of Identifier \n\n";
            return;
        }
        else if(node->type == tok_integer){
            cout << "Integer \n{\n\n";
            cout << "value: " << node->f.a_integer.value;
            cout << "\n} // End of Integer \n\n";
            return;
        }
        else if(node->type == tok_string){
            cout << "String {\n\n";
            cout << "value: " << node->f.a_string.string;
            cout << "\n} // End of String \n\n";
            return;
        }
        else if(node->type == kw_true || node->type == kw_false){
            cout << "Boolean {\n\n";
            cout << "value: " << node->f.a_boolean.value;
            cout << "\n} // End of Boolean \n\n";
            return;
        }
        else if(node->type == kw_and){
            cout << "AND Operation \n{\n\n";
            cout << "Left Arg \n{\n\n";
            printExpr(node->f.a_binary_op.larg);
            cout << "\n} // End of left Argument \n\n";
            cout << "Right Arg \n{\n\n";
            printExpr(node->f.a_binary_op.rarg);
            cout << "\n} // End of Right Argument \n\n";
            cout << "\n} // End of AND Operation \n\n";
            return;
        }
        else if(node->type == kw_or){
            cout << "OR Operation \n{\n\n";
            cout << "Left Arg \n{\n\n";
            printExpr(node->f.a_binary_op.larg);
            cout << "\n} // End of left Argument \n\n";
            cout << "Right Arg \n{\n\n";
            printExpr(node->f.a_binary_op.rarg);
            cout << "\n} // End of Right Argument \n\n";
            cout << "\n} // End of AND Operation \n\n";
            return;
        }
        else{
            cout << Operators[node->type - 37] << " Operation \n{\n\n";
            cout << "Left Arg \n{\n\n";
            printExpr(node->f.a_binary_op.larg);
            cout << "\n} // End of left Argument \n\n";
            cout << "Right Arg \n{\n\n";
            printExpr(node->f.a_binary_op.rarg);
            cout << "\n} // End of Right Argument \n\n";
             cout << "\n} // End of " << Operators[node->type - 37] <<  " Operation \n\n";
            return;
        }
        return;

 }







































  void MainWindow::genTree(ast_node * program){
//       ast_list * Declerations = program->f.a_program.decs;
//       while(Declerations->head != NULL){
//          if(Declerations->head->type == kw_var){

//          }
//          else if (Declerations->head->type = kw_constant){

//          }
//            Declerations = Declerations->tail;
//       }

       ast_list * Stmts = program->f.a_program.stmts;
       while(Stmts->head != NULL){
            genStmt(Stmts->head);
            Stmts = Stmts->tail;
       }


  }

  void MainWindow::genExpr(ast_node *node){
//      cout << "" << endl;
       if (node->type == tok_ident){ // var
          cout << "lw $t0 , "<<node->f.a_var.var->offset<< "($fp)" << endl;
          cout << "addiu $sp, $sp, -4" << endl;
          cout << "sw $t0, 4($sp)" << endl;
          return;
      }
      else if(node->type == tok_integer){
           cout << "li $t0, " << node->f.a_integer.value << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t0, 4($sp)" << endl;
          return;
      }
      else if(node->type == tok_string){
           cout << "li $t0, " << node->f.a_string.string << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t0, 4($sp)" << endl;
          return;
      }
      else if(node->type == kw_true || node->type == kw_false){
           cout << "li $t0, " << node->f.a_boolean.value << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t0, 4($sp)" << endl;
          return;
      }
      else if(node->type == lx_plus){
          genExpr(node->f.a_binary_op.larg);
          genExpr(node->f.a_binary_op.rarg);

          cout << "lw $t1, 4($sp)" << endl;
          cout << "lw $t2, 8($sp)" << endl;
          cout << "addiu $sp, $sp, 8" << endl;
          cout << "addu $t3, $t2, t1" << endl;
          cout << "addiu $sp, $sp, -4" << endl;
          cout << "sw $t3, 4($sp)" << endl;
          return;
      }
      else if(node->type == lx_star){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "mul $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;

          return;
      }
      else if(node->type == lx_slash){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "subu $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;

          return;
      }
      else if(node->type == lx_minus){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "div $t2, t1" << endl;
           cout << "mflo $t3" << endl;              //assuming the resukt us akways <= 32 bit
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
          return;
      }
       else if(node->type == lx_eq){

            genExpr(node->f.a_binary_op.larg);
            genExpr(node->f.a_binary_op.rarg);

            cout << "lw $t1, 4($sp)" << endl;
            cout << "lw $t2, 8($sp)" << endl;
            cout << "addiu $sp, $sp, 8" << endl;
            cout << "seq $t3, $t2, t1" << endl;
            cout << "addiu $sp, $sp, -4" << endl;
            cout << "sw $t3, 4($sp)" << endl;
           return;
       }
       else if(node->type == lx_gt){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "sgt $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
           return;
       }
       else if(node->type == lx_lt){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "sgt $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
           return;
       }
       else if(node->type == lx_ge){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "sge $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
           return;
       }
       else if(node->type == lx_le){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "sle $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
           return;
       }
       else if(node->type == lx_neq){

           genExpr(node->f.a_binary_op.larg);
           genExpr(node->f.a_binary_op.rarg);

           cout << "lw $t1, 4($sp)" << endl;
           cout << "lw $t2, 8($sp)" << endl;
           cout << "addiu $sp, $sp, 8" << endl;
           cout << "sne $t3, $t2, t1" << endl;
           cout << "addiu $sp, $sp, -4" << endl;
           cout << "sw $t3, 4($sp)" << endl;
           return;
       }
      else{
//          cout << Operators[node->type - 37] << " Operation \n{\n\n";
          return;
      }
      return;

  }
  void MainWindow::genStmt(ast_node *node){
      if(node->type == lx_eq){
           genExpr(node->f.a_assign.rhs);
           cout << "lw $t0 , 4($sp)" << endl;
           cout << "addiu $sp, $sp, 4" << endl;
           cout << "sw $t0, " << node->f.a_assign.lhs->offset  << "($fp)" << endl;
          return;
      }
      else if(node->type == kw_if){

          int else_l = index++;
          int endif_l = index++;

          genExpr(node->f.a_if.predicate);
          cout << "lw $t0 , 4($sp)" << endl;
          cout << "addiu $sp, $sp, 4" << endl;
          cout << "beqz $t0 , Else_" << else_l << endl;
          genStmt(node->f.a_if.conseq);
          cout << "j EndIf_" << endif_l << endl;
          cout << "Else_" << else_l <<" : " << endl;
          genStmt(node->f.a_if.altern);
          cout << "EndIf_" << endif_l <<" : " << endl;
          return;
      }
      else if (node->type == kw_for){
          int for_l = index++;
          int endfor_l = index++;

          genExpr(node->f.a_for.lower_bound);
          genExpr(node->f.a_for.upper_bound);
           cout << "lw $t1 , 4($sp)" << endl;   //upper
           cout << "lw $t0 , 8($sp)" << endl;   // lower
           cout << "addiu $sp, $sp, 8" << endl;

          cout << "For_" << for_l <<" : " << endl;
           cout << "beq $t0, $t1, EndFor_"<< endfor_l <<endl;
           cout << "addiu $t0, 1" << endl;
           genStmt(node->f.a_for.body);
          cout << "j For_" << for_l << endl;
          cout << "EndFor_" << endfor_l <<" : " << endl;
          return;
      }
      else if (node->type == kw_while){
          int while_l = index++;
          int endwhile_l = index++;

          cout << "while_" << while_l <<" : " << endl;
          genExpr(node->f.a_while.predicate);
          cout << "lw $t0 , 4($sp)" << endl;
          cout << "addiu $sp, $sp, 4" << endl;
          cout << "beqz $t0 , EndWhile_" << endwhile_l << endl;
          genStmt(node->f.a_while.body);
          cout << "j while_" << while_l << endl;
          cout << "EndWhile_" << endwhile_l <<" : " << endl;
          return;
      }

  }












